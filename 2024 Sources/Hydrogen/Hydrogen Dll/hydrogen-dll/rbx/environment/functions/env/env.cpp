#include "env.hpp"

#include <dependencies/minhook/minhook.hpp>
#include <rbx/environment/environment.hpp>
#include <dependencies/luau/lmem.h>

#include <rbx/environment/utilities/connection/connection.hpp>
#include <rbx/environment/utilities/thread/thread.hpp>
#include <rbx/environment/utilities/storage/storage.hpp>
#include <rbx/environment/utilities/file/file.hpp>
#include <rbx/environment/utilities/utilities.hpp>

#include <ranges>
#include <set>

#include <cpr/cpr.h>

#define STRTOLOWER(x) std::transform (x.begin(), x.end(), x.begin(), ::tolower)

std::int32_t rbx::environment::c_env::getgenv( lua_State* L )
{
	sol::state_view sv{ g_environment.get_hydrogen_state( ) };

	return sol::stack::push( L, sv.lua_globals( ) );
}

std::int32_t rbx::environment::c_env::getrenv( lua_State* L )
{
	sol::state_view sv{ g_environment.get_roblox_state( ) };

	return sol::stack::push( L, sv.lua_globals( ) );
}

std::int32_t rbx::environment::c_env::getreg( lua_State* L )
{
	sol::state_view sv{ L };

	return sol::stack::push( L, sv.lua_registry( ) );
}

// TODO: make a helper func as there is prolly some vuln associated with allowing users to get threads
sol::object rbx::environment::c_env::getgc( sol::optional< bool > include_tables, sol::optional< bool > include_threads, sol::this_state L )
{
	struct
	{
		lua_State* L;
		bool should_include_tables, should_include_threads;
		std::vector< sol::object > objects;
	} c_ctx{ L, include_tables.value_or( false ), include_threads.value_or( false ) };

	luaM_visitgco( L, &c_ctx, [ ]( void* context, lua_Page* page, GCObject* gco )
	{
		const auto ctx = static_cast< decltype( &c_ctx ) >( context );

		if ( !gco )
			return false;

		if ( isdead( ctx->L->global, gco ) )
			return false;

		const auto tt = gco->gch.tt;

		if ( tt == LUA_TFUNCTION || tt == LUA_TUSERDATA || ( ctx->should_include_tables && tt == LUA_TTABLE ) || ( ctx->should_include_threads && tt == LUA_TTHREAD ) )
		{
			lua_checkstack( ctx->L, 1 );

			ctx->L->top->value.gc = gco;
			ctx->L->top->tt = gco->gch.tt;

			incr_top( ctx->L );

			const auto object = sol::stack::get< sol::object >( ctx->L, -1 );

			ctx->objects.push_back( object );

			lua_pop( ctx->L, 1 );
		}

		return false;
	} );

	return sol::make_object( L,
		sol::as_table( c_ctx.objects ) );
}

std::int32_t rbx::environment::c_env::getinstances( lua_State* L )
{
	sol::state_view sv{ L };

	std::vector< sol::object > container;

	const auto dictionary = sv.lua_registry( ).get
		< sol::table >( sol::make_light( instance::offs::push_instance ) );

	for ( const auto& [key, value] : dictionary )
		container.push_back( value );

	return sol::stack::push( L, sol::as_table( container ) );
}

std::int32_t rbx::environment::c_env::getnilinstances( lua_State* L )
{
	sol::state_view sv{ L };

	auto container = sv[ "getinstances" ].call< sol::as_table_t< std::vector< sol::object > > >( );

	auto nil_instances = container.value( ) | std::views::filter( [ ]( sol::object& elm )
	{
		if ( !elm.is< sol::userdata >( ) )
			return false;

		return elm.as< sol::userdata >( )[ "Parent" ].is< sol::nil_t >( );
	} );

	return sol::stack::push( L, 
		sol::as_table( std::vector< sol::object >{ 
			nil_instances.begin( ), nil_instances.end( ) } ) );
}

std::int32_t rbx::environment::c_env::getscripts( lua_State* L )
{
	sol::state_view sv{ L };

	auto container = sv[ "getinstances" ].call
		< sol::as_table_t< std::vector< sol::object > > >( );

	auto scripts = container.value( ) | std::views::filter( [ ]( sol::object& elm )
	{
		if ( !elm.is< sol::userdata >( ) )
			return false;

		const auto is_a = [ elm ]( std::string_view c ) -> bool
			{ return elm.as< sol::userdata >( )[ "IsA" ].call< bool >( elm, c ); };

		return is_a( "LocalScript" ) || is_a( "ModuleScript" );
	} );

	return sol::stack::push( L,
		sol::as_table( std::vector< sol::object >{
			scripts.begin( ), scripts.end( ) } ) );
}

sol::object rbx::environment::c_env::getloadedmodules( sol::optional< bool > dont_filter, sol::this_state L )
{
	const auto& loaded_modules
		= *reinterpret_cast< std::set< std::weak_ptr< void* > >* >
			( g_environment.get_script_context( ) + rbx::script_context
				::offs::loaded_modules );

	std::vector< sol::object > modules;

	for ( const auto mod : loaded_modules )
	{
		if ( !mod.expired( ) )
		{
			luaD_checkstack( L, 1 );

			rbx::instance::offs
				::push_instance( L, mod.lock( ) );

			const auto object = sol::stack::get< instance_t >( L, -1 );

			const auto get_service
				= std::bind( &c_utilities::get_service< sol::object >, L, std::placeholders::_1 );

			if ( !object.is_descendant_of( get_service( "CoreGui" ), get_service( "CorePackages" ) ) || !dont_filter.value_or( true ) )
				modules.push_back( object );

			lua_pop( L, 1 );
		}
	}

	return sol::make_object( L, sol::as_table( modules ) );
}

sol::object rbx::environment::c_env::getrunningscripts( sol::this_state L )
{
	sol::state_view sv{ L };

	std::vector< sol::object > scripts;

	const auto container = utilities::c_thread::get_scripts_with_thread( L );

	for ( const auto[key, value] : container )
	{
		rbx::instance::offs
			::push_instance( L, key );

		const auto object = sol::stack::get< sol::object >( L, -1 );

		scripts.push_back( object );

		lua_pop( L, 1 );
	}

	return sol::make_object( L,
		sol::as_table( std::vector< sol::object >{
			scripts.begin( ), scripts.end( ) } ) );
}

sol::object rbx::environment::c_env::getconnections( script_signal_t signal, sol::this_state L )
{
	sol::state_view sv{ L };

	const auto temp_connection
		= signal[ "Connect" ]( signal, sol::as_function(
			[ ]( lua_State* ) -> std::int32_t { return 0; } ) ).get< instance_t >( );

	std::uint8_t* connection = temp_connection.get_internal( );

	std::vector< sol::table > connections{ };

	while ( connection = *reinterpret_cast< std::uint8_t** >( connection + 0x10 ) )
	{
		const auto thread = utilities::c_connection::get_connections_thread( connection );
		const auto function = utilities::c_connection::get_connections_function( connection );

		const auto tbl = sv.create_table_with(
			"Object", sol::make_user( connection ),
			"Thread", sol::thread( thread, thread ),
			"Function", function.valid( ) ? function : sol::nil,

			"Enabled", !utilities::g_blocked.should_block( connection ),
			"ForeignState", utilities::c_thread::is_thread_foreign( thread ),
			"LuaConnection", !reinterpret_cast< const Closure* >( function.pointer( ) )->isC,

			"Disconnect", sol::as_function( [ ]( sol::table obj, sol::this_state L )
			{
				lua_checkstack( L, 1 );

				const auto data 
					= lua_newuserdatatagged( L, 4, 
						utilities::c_connection::get_connection_tag( ) );
					
				new( data ) const void* ( 
					*reinterpret_cast< void** >( obj[ "Object" ].get< void* >( ) ) );

				luaL_getmetatable( L, "RBXScriptConnection" );
				lua_setmetatable( L, -2 );
					
				const auto connection 
					= sol::stack::get< sol::userdata >( L, -1 );

				connection[ "Disconnect" ]( connection );
			} ),

			"Disable", sol::as_function( [ ]( sol::table obj, sol::this_state L )
			{
				utilities::g_blocked.toggle(
					*reinterpret_cast< void** >( obj[ "Object" ].get< void* >( ) ), false );
			} ),

			"Enable", sol::as_function( [ ]( sol::table obj, sol::this_state L )
			{
				utilities::g_blocked.toggle(
					*reinterpret_cast< void** >( obj[ "Object" ].get< void* >( ) ), true );
			} ),

			"Fire", sol::as_function( [ ]( sol::table obj, sol::variadic_args va )
			{
				obj[ "Function" ]( va );
			} ),

			"Defer", sol::as_function( [ ]( sol::table obj, sol::variadic_args va, sol::this_state L )
			{
				sol::state_view sv{ L };

				sv[ "task" ][ "defer" ]( 
					obj["Function"].get< sol::function >( ), va );
			} )
		);

		connections.push_back( tbl );
	}

	temp_connection[ "Disconnect" ]( temp_connection );

	return sol::make_object( L, sol::as_table( connections ) );
}

sol::object rbx::environment::c_env::firesignal( script_signal_t obj, sol::variadic_args va, sol::this_state L )
{
	sol::state_view sv{ L };

	auto container = sv[ "getconnections" ].call
		< sol::as_table_t< std::vector< sol::table > > >( obj );

	for(const auto& connection : container.value( ))
	{
		if ( connection[ "Enabled" ].get< bool >( ) )
			connection[ "Fire" ]( connection, va );
	}

	return sol::nil;
}

sol::object rbx::environment::c_env::defersignal( script_signal_t obj, sol::variadic_args va, sol::this_state L )
{
	sol::state_view sv{ L };

	auto container = sv[ "getconnections" ].call
		< sol::as_table_t< std::vector< sol::table > > >( obj );

	for ( const auto& connection : container.value( ) )
	{
		if ( connection[ "Enabled" ].get< bool >( ) )
			connection[ "Defer" ]( connection, va );
	}

	return sol::nil;
}

// TODO : other click types
void rbx::environment::c_env::fireclickdetector( click_detector_t obj, sol::optional< double > duration, sol::this_state L )
{
	sol::state_view sv{ L };

	const auto local_player = c_utilities::get_service< sol::userdata >( sv, "Players" )
		[ "LocalPlayer" ].get< instance_t >( );

	click_detector::offs
		::fire_shit( obj.get_internal( ), local_player.get_internal( ) );
}

float rbx::environment::c_env::getproximitypromptduration( proximity_prompt_t prompt, sol::this_state L )
{
	return *reinterpret_cast< float* >(
		prompt.get_internal( ) + proximity_prompt::offs::duration );
}

void rbx::environment::c_env::setproximitypromptduration( proximity_prompt_t prompt, double duration, sol::this_state L )
{
	*reinterpret_cast< float* >(
		prompt.get_internal( ) + proximity_prompt::offs::duration ) = duration;
}

void rbx::environment::c_env::fireproximityprompt( proximity_prompt_t prompt, sol::this_state L )
{
	sol::state_view sv{ L };

	const auto old_duration
		= sv[ "getproximitypromptduration" ]( prompt ).get< double >( );

	sv[ "setproximitypromptduration" ]( prompt, 0.0f );

	std::uint8_t* p
		= prompt.get_internal( );

	proximity_prompt::offs
		::fire_proximity_prompt( p );

	sv[ "setproximitypromptduration" ]( prompt, old_duration );
}

void rbx::environment::c_env::firetouchinterest( base_part_t part, base_part_t to_touch, std::uint32_t toggle, sol::this_state L )
{
	const auto part_primitive
		= *reinterpret_cast< std::uint8_t** >( part.get_internal( ) + instance::offs::primitive );

	const auto to_touch_primitive
		= *reinterpret_cast< std::uint8_t** >( to_touch.get_internal( ) + instance::offs::primitive );

	if ( toggle )
		touch_interest::offs
			::touch_ended( part_primitive, to_touch_primitive );
	else
		touch_interest::offs
			::touch_started( part_primitive, to_touch_primitive );
}

bool rbx::environment::c_env::isnetworkowner( base_part_t part, sol::this_state L )
{
	return !instance::offs
		::compute_network_owner_is_someone_else( part.get_internal( ) );
}

bool rbx::environment::c_env::setscriptable( instance_t obj, std::string prop, bool state, sol::this_state L )
{
	if ( !obj.get_property_descriptor( L, prop ) )
		luaL_error( L, std::format( "Invalid Property: {}", prop ).c_str( ) );

	const auto was_scriptable 
		= utilities::g_instance.is_scriptable( obj.get_internal( ), prop );

	utilities::g_instance.toggle( obj.get_internal( ), prop, state );

	return was_scriptable.value_or( *obj.get_scriptable( L, prop ) );
}

bool rbx::environment::c_env::isscriptable( instance_t obj, std::string prop, sol::this_state L )
{
	if ( !obj.get_property_descriptor( L, prop ) )
		luaL_error( L, std::format( "Invalid Property: {}", prop ).c_str( ) );

	const auto was_scriptable
		= utilities::g_instance.is_scriptable( obj.get_internal( ), prop );

	return was_scriptable.value_or( *obj.get_scriptable( L, prop ) );
}

std::pair< sol::object, bool > rbx::environment::c_env::gethiddenproperty( instance_t obj, std::string_view prop, sol::this_state L )
{
	sol::state_view sv{ L };

	const auto was = sv[ "setscriptable" ]( obj, prop, true );
	const auto ret = obj[ prop ].get< sol::object >( );
	sv[ "setscriptable" ]( obj, prop, was );

	return { ret, !was };
}

bool rbx::environment::c_env::sethiddenproperty( instance_t obj, std::string_view prop, sol::object value, sol::this_state L )
{
	sol::state_view sv{ L };

	const auto was = sv[ "setscriptable" ]( obj, prop, true ).get< bool >( );
	obj[ prop ].set( value );
	sv[ "setscriptable" ]( obj, prop, was );

	return !was;
}

void rbx::environment::c_env::setsimulationradius( double value, sol::optional< double > max_value, sol::this_state L ) 
{
	sol::state_view sv{ L };

	const auto local_player = c_utilities::get_service< sol::userdata >( sv, "Players" )
		[ "LocalPlayer" ].get< sol::userdata >( );

	if ( max_value )
		sv[ "sethiddenproperty" ]( local_player, "MaximumSimulationRadius", max_value.value( ) );

	sv[ "sethiddenproperty" ]( local_player, "SimulationRadius", value );
}

std::string rbx::environment::c_env::getcustomasset( std::string path, sol::this_state L )
{
	std::printf( "%s\n", path.c_str( ) );

	if ( utilities::c_file::is_malicious( path ) )
		luaL_error( L, "Malicious attempt" );

	sol::state_view sv{ L };

	const auto content = std::filesystem::current_path( ) / "content" / path;
	const auto to_copy = std::filesystem::current_path( ) / "Workspace" / path;

	if ( !std::filesystem::exists( to_copy ) )
		luaL_error( L, "File does not exist" );

	if ( !std::filesystem::exists( content.parent_path( ) ) )
		std::filesystem::create_directories( content.parent_path( ) );

	if ( std::filesystem::exists( content ) )
		std::filesystem::remove( content );

	std::filesystem::copy( to_copy, content, std::filesystem::copy_options::recursive );

	return "rbxasset://" + path;
}

// TODO: proper semver version
std::tuple< std::string, std::string > rbx::environment::c_env::identifyexecutor( sol::this_state L )
{
	return std::make_tuple( "Hydrogen", "ALPHA" );
}

std::int32_t rbx::environment::c_env::gethui( lua_State* L )
{
	sol::state_view sv{ L };

	c_utilities::get_service< sol::object >( L, "CoreGui" ).push( );

	return 1;
}

std::uint32_t rbx::environment::c_env::getthreadidentity( sol::this_state L )
{
	return rbx::environment::utilities
		::c_thread::get_thread_identity( L );
}

void rbx::environment::c_env::setthreadidentity( std::uint32_t n, sol::this_state L )
{
	rbx::environment::utilities
		::c_thread::set_thread_identity( L, n );
}

sol::object rbx::environment::c_env::require( sol::object arg, sol::this_state L )
{
	sol::state_view sv{ g_environment.get_roblox_state( ) };

	const auto old_identity = rbx::environment::utilities
		::c_thread::get_thread_identity( L );

	rbx::environment::utilities
		::c_thread::set_thread_identity( L, 2 );

	const auto res = sv[ "require" ]( arg ).get< sol::object >( );

	rbx::environment::utilities
		::c_thread::set_thread_identity( L, old_identity );

	return res;
}

std::int32_t rbx::environment::c_env::httpget( lua_State* L )
{
	sol::stack::get< instance_t >( L, 1 ).e_is_a( "DataModel" );
	
	const auto url = sol::stack::get< std::string >( L, 2 );

	if ( url.find( "http://" ) == std::string::npos && url.find( "https://" ) == std::string::npos )
		luaL_error( L, "Invalid protocol" );

	const auto async = sol::stack::get< sol::optional< bool > >( L, 3 );

	//if ( async.value_or( true ) )
	//{
	//	const auto identity
	//		= utilities::c_thread::get_thread_identity( L );

	//	lua_pushthread( L );
	//	const auto refid = lua_ref( L, -1 );

	//	struct c_storage
	//	{
	//		std::uint32_t identity;
	//		lua_State* L;
	//		int refid;
	//		std::string url;
	//		std::string res;
	//	};

	//	c_storage* storage =
	//		new c_storage{ .identity = identity,
	//			.L = L, .refid = refid, .url = url }; // Prevent destruction of any objects due to exiting scope

	//	std::thread{
	//		[ storage ]( )
	//		{
	//			cpr::Response r = cpr::Get( cpr::Url{ storage->url } );

	//			storage->res = r.text;

	//			g_environment.m_yields.insert(
	//			[ storage ]( )
	//			{
	//				utilities::c_thread
	//					::set_thread_identity( storage->L, storage->identity );

	//				lua_pushlstring( storage->L, storage->res.c_str( ), storage->res.size( ) );

	//				g_environment.resume( storage->L, 1 );

	//				lua_unref( storage->L, storage->refid );

	//				delete storage;
	//			} );
	//		}
	//	}.detach( );

	//	return lua_yield( L, 1 );
	//}
	//else
	//{
		cpr::Response r = cpr::Get( cpr::Url{ url } );

		lua_pushlstring( L, r.text.c_str( ), r.text.size( ) );
		return 1;
	//}
}

std::int32_t rbx::environment::c_env::httpgetasync( lua_State* L )
{
	lua_settop( L, 2 );

	lua_pushboolean( L, true );

	return httpget( L );
}

std::int32_t rbx::environment::c_env::request( lua_State* L ) // TODO: tolower stuff
{
	c_utilities::e_is_types( L, { { 1, { "table" } } } );

	const auto options = sol::stack::get< sol::table >( L, 1 );

	const auto url_obj = options.get< sol::object >( "Url" );

	if ( url_obj == sol::nil )
		luaL_error( L, "No url provided to request" );

	const auto url = url_obj.as< std::string >( );

	if ( url.find( "http://" ) == std::string::npos && url.find( "https://" ) == std::string::npos )
		luaL_error( L, "Invalid protocol" );

	auto method = 
		options.get_or< std::string >( "Method", "get" );

	STRTOLOWER( method );

	const auto option_headers 
		= options.get_or< std::map< 
			std::string, std::string > >( "Headers", { } );

	bool header_provided = false;

	cpr::Header headers;
	for ( const auto& [key, value] : option_headers )
	{
		auto temp_string = key;
		STRTOLOWER( temp_string );

		if ( temp_string == "user-agent" )
			header_provided = true;

		headers.insert( { key, value } );
	}

	//TODO: hwid here

	const auto id = *player::offs::player_id;

	const auto str_id = id ? std::to_string( id ) : "wtfskidfailed";

	std::printf( "%s\n", str_id.c_str( ) );

	headers.insert( { "User-Identifier", str_id } );
	headers.insert( { "Fingerprint", str_id } );

	if ( !header_provided ) // TODO: semver version
		headers.insert( { "User-Agent", std::format( "Hydrogen Windows {}", "ALPHA" ) } );

	std::map< std::string, std::string > option_cookies
		= options.get_or< std::map<
			std::string, std::string > >( "Cookies", { } );

	cpr::Cookies cookies;
	for ( const auto& [key, value] : option_cookies )
		cookies.push_back( cpr::Cookie{ key, value } );

	//TODO: check method cuz some shouldnt have body
	const auto body =
		options.get_or< std::string >( "Body", "" );

	const auto identity
		= utilities::c_thread::get_thread_identity( L );

	lua_pushthread( L );
	const auto refid = lua_ref( L, -1 );

	struct c_storage
	{
		std::uint32_t identity;
		lua_State* L;
		int refid;
		std::string url;
		std::string method;
		cpr::Header headers;
		cpr::Cookies cookies;
		std::string body;
		cpr::Response res;
	};

	c_storage* storage = new c_storage{ 
		.identity = identity,
		.L = L, 
		.refid = refid, 
		.url = url, 
		.method = method, 
		.headers = headers, 
		.cookies = cookies, 
		.body = body 
	}; // Prevent destruction of any objects due to exiting scope

	std::thread{
		[ storage ]( )
		{
			if ( storage->method == "get" )
				storage->res = cpr::Get( cpr::Url{ storage->url }, storage->cookies, storage->headers );
			else if ( storage->method == "post" )
				storage->res = cpr::Post( cpr::Url{ storage->url }, storage->cookies, storage->headers, cpr::Body{ storage->body } );
			else if ( storage->method == "put" )
				storage->res = cpr::Put( cpr::Url{ storage->url }, storage->cookies, storage->headers, cpr::Body{ storage->body } );
			else if ( storage->method == "delete" )
				storage->res = cpr::Delete( cpr::Url{ storage->url }, storage->cookies, storage->headers, cpr::Body{ storage->body } );
			else if ( storage->method == "options" )
				storage->res = cpr::Options( cpr::Url{ storage->url }, storage->cookies, storage->headers, cpr::Body{ storage->body } );
			else if ( storage->method == "head" )
				storage->res = cpr::Head( cpr::Url{ storage->url }, storage->cookies, storage->headers );

			g_environment.m_yields.insert(
			[ storage ]( )
			{
				utilities::c_thread
					::set_thread_identity( storage->L, storage->identity );

				sol::state_view sv{ storage->L };

				sol::table table = sv.create_table( );
				table[ "Success" ] = storage->res.status_code >= 200 && storage->res.status_code <= 299;
				table[ "StatusCode" ] = storage->res.status_code;
				table[ "StatusMessage" ] = storage->res.status_line;
				
				auto headers = sv.create_table( );
				for ( const auto& [key, value] : storage->res.header )
					headers[ key ] = value;
				
				table[ "Headers" ] = headers;

				auto cookies = sv.create_table( );
				for ( const auto cookie : storage->res.cookies )
					cookies[ cookie.GetName( ) ] = cookie.GetValue( );

				table[ "Cookies" ] = cookies;

				table[ "Body" ] = storage->res.text;

				table.push( );

				g_environment.resume( storage->L, 1 );

				lua_unref( storage->L, storage->refid );

				delete storage;
			} );
		}
	}.detach( );

	return lua_yield( L, 1 );
}

void __fastcall rbx::environment::c_env::on_function_call( std::uint8_t* function_slot_wrapper, void* edx, void* args )
{
	const auto connection
		= *reinterpret_cast< void** >( function_slot_wrapper );

	if ( utilities::g_blocked.should_block( connection ) )
		return;

	return m_on_function_call( function_slot_wrapper, edx, args );
}

void __fastcall rbx::environment::c_env::set_generic_callback_helper( void* callback_descriptor, void* edx, void* object, void* function )
{
	std::printf( "%p %p %p\n", callback_descriptor, object, function );
	m_set_generic_callback_helper( callback_descriptor, edx, object, function );
}

std::int32_t rbx::environment::c_env::on_index_bridge( lua_State* L ) // Kinda an issue i thought of, if an instance insta destructs after script then another instance is likely to be allocated at same address.
{
	if ( !utilities::c_thread::get_threads_script< bool >( L ) ) 
	{
		instance_t object = sol::stack::get< instance_t >( L, 1 );

		std::string_view key = lua_tostring( L, 2 );

		if ( key == "GetObjects" )
		{
			lua_getglobal( L, key.data( ) );
			return 1;
		}

		if ( key == "HttpGet" )
		{
			lua_pushcfunction( L, httpget, nullptr );
			return 1;
		}

		if ( key == "HttpGetAsync" )
		{
			lua_pushcfunction( L, httpgetasync, nullptr );
			return 1;
		}
	
		if ( const auto cache_scriptable = utilities::g_instance.is_scriptable( object.get_internal( ), key.data( ) ) )
		{
			if ( const auto scriptable = object.get_scriptable( L, key.data( ) ) )
			{
				object.set_scriptable( L, key.data( ), *cache_scriptable );

				const auto res = m_on_index_bridge( L );

				object.set_scriptable( L, key.data( ), *scriptable );

				return res;
			}
		}
	}

	return m_on_index_bridge( L );
}

std::int32_t rbx::environment::c_env::on_namecall_bridge( lua_State* L )
{
	if ( !utilities::c_thread::get_threads_script< bool >( L ) )
	{
		if ( !strcmp( getstr( L->namecall ), "GetObjects" ) )
		{
			lua_getglobal( L, "GetObjects" );
			lua_insert( L, -3 );
			lua_call( L, 2, 1 );

			return 1;
		}

		if ( !strcmp( getstr( L->namecall ), "HttpGet" ) )
		{
			return httpget( L );
		}

		if ( !strcmp( getstr( L->namecall ), "HttpGetAsync" ) )
		{

			return httpgetasync( L );
		}
	}

	return m_on_namecall_bridge( L );
}

std::int32_t rbx::environment::c_env::on_newindex_bridge( lua_State* L )
{
	if ( !utilities::c_thread::get_threads_script< bool >( L ) )
	{
		instance_t object = sol::stack::get< instance_t >( L, 1 );

		const std::string key = sol::stack::get< std::string >( L, 2 );

		if ( const auto cache_scriptable = utilities::g_instance.is_scriptable( object.get_internal( ), key ) )
		{
			if ( const auto scriptable = object.get_scriptable( L, key ) )
			{
				object.set_scriptable( L, key, *cache_scriptable );

				const auto res = m_on_newindex_bridge( L );

				object.set_scriptable( L, key, *scriptable );

				return res;
			}
		}
	}

	return m_on_newindex_bridge( L );
}

void rbx::environment::c_env::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	utilities::c_connection::set_connection_tag( L );

	sv.set_function( "GetObjects", [  ]( sol::userdata game, sol::object arg, sol::this_state L ) -> sol::table
	{
		sol::state_view sv{ L };

		auto insert_service = c_utilities::get_service< sol::userdata >( L, "InsertService" );
		auto load_local_asset = insert_service[ "LoadLocalAsset" ];

		const auto res = load_local_asset( insert_service, arg );

		return sv.create_table_with( 1, res );
	} );

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	register_fn( getgenv, { "getgenv" } );
	register_fn( getrenv, { "getrenv" } );
	register_fn( getreg, { "getreg" } );
	register_fn( getgc, { "getgc" } );
	register_fn( getinstances, { "getinstances" } );
	register_fn( getnilinstances, { "getnilinstances" } );
	register_fn( getscripts, { "getscripts" } );
	register_fn( getloadedmodules, { "getloadedmodules" } );
	register_fn( getrunningscripts, { "getrunningscripts" } );
	register_fn( getconnections, { "getconnections" } );
	register_fn( firesignal, { "firesignal" } );
	register_fn( defersignal, { "defersignal" } );
	//register_fn( fireclickdetector, { "fireclickdetector" } );
	register_fn( getproximitypromptduration, { "getproximitypromptduration" } );
	register_fn( setproximitypromptduration, { "setproximitypromptduration" } );
	register_fn( fireproximityprompt, { "fireproximityprompt" } );
	//register_fn( firetouchinterest, { "firetouchinterest" } );
	register_fn( isnetworkowner, { "isnetworkowner" } );
	register_fn( isscriptable, { "isscriptable" } );
	register_fn( setscriptable, { "setscriptable" } );
	register_fn( gethiddenproperty, { "gethiddenproperty" } );
	register_fn( sethiddenproperty, { "sethiddenproperty" } );
	register_fn( setsimulationradius, { "setsimulationradius" } );
	register_fn( getcustomasset, { "getcustomasset" } );
	register_fn( identifyexecutor, { "identifyexecutor", "getexecutorname" } );
	register_fn( gethui, { "gethui" } );
	register_fn( getthreadidentity, { "getthreadidentity", "getidentity", "getthreadcontext" } );
	register_fn( setthreadidentity, { "setthreadidentity", "setidentity", "setthreadcontext" } );
	register_fn( require, { "require" } );
	register_fn( request, { "request", "http_request" } );

	sv.create_named_table( "http" )[ "request" ] = sol::as_function( request );

	MH_CreateHook( rbx::instance
		::offs::function_script_slot_call_operator,
			&on_function_call, reinterpret_cast< void** >( &m_on_function_call ) );

	MH_EnableHook( rbx::instance
		::offs::function_script_slot_call_operator );

	//MH_CreateHook( rbx::instance
	//	::offs::set_generic_callback_helper,
	//	&set_generic_callback_helper, reinterpret_cast< void** >( &m_set_generic_callback_helper ) );

	//MH_EnableHook( rbx::instance
	//	::offs::set_generic_callback_helper );
}
