#include "cache.hpp"

void rbx::environment::c_cache::replace( instance_t obj, instance_t t_obj, sol::this_state L )
{
	sol::state_view sv{ L };

	auto dictionary = sv.lua_registry( ).get
		< sol::table >( sol::make_light( instance::offs::push_instance ) );

	const auto address = obj.get_internal( );

	const auto cached = dictionary.get< sol::object >( sol::make_light( address ) );
	if ( !cached.valid( ) || cached == sol::nil )
		return;

	dictionary[ sol::make_light( address ) ] = t_obj;
}

void rbx::environment::c_cache::invalidate( instance_t obj, sol::this_state L )
{
	sol::state_view sv{ L };

	auto dictionary = sv.lua_registry( ).get
		< sol::table >( sol::make_light( instance::offs::push_instance ) );

	const auto address = obj.get_internal( );

	const auto cached = dictionary.get< sol::object >( sol::make_light( address ) );

	if ( !cached.valid( ) || cached == sol::nil )
		return;

	dictionary[ sol::make_light( address ) ] = sol::nil;
}

bool rbx::environment::c_cache::iscached( instance_t obj, sol::this_state L )
{
	sol::state_view sv{ L };

	auto dictionary = sv.lua_registry( ).get
		< sol::table >( sol::make_light( instance::offs::push_instance ) );

	const auto address = obj.get_internal( );

	const auto cached = dictionary.get< sol::object >( sol::make_light( address ) );

	return cached.valid( ) && cached != sol::nil;
}

sol::object rbx::environment::c_cache::cloneref( instance_t obj, sol::this_state L )
{
	sol::state_view sv{ L };

	auto dictionary = sv.lua_registry( ).get
		< sol::table >( sol::make_light( instance::offs::push_instance ) );

	const auto address = obj.get_internal( );

	const auto cached = dictionary.get< sol::object >( sol::make_light( address ) );

	if ( !cached.valid( ) || cached == sol::nil )
		return sol::nil;

	// Invalidate the cache
	dictionary[ sol::make_light( address ) ] = sol::nil;

	// New cache
	rbx::instance::offs
		::push_instance_u( L, cached.pointer( ) );

	const auto new_obj = sol::stack::get< sol::object >( L, -1 );

	// Restore cache
	dictionary[ sol::make_light( address ) ] = cached;

	return new_obj;
}

bool rbx::environment::c_cache::compareinstances( instance_t obj, instance_t t_obj, sol::this_state L )
{
	return obj.get_internal( ) == t_obj.get_internal( );
}

void rbx::environment::c_cache::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	sol::table cache = sv.create_named_table( "cache" );

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	const auto register_cache_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			cache.set_function( alias, function );
	};

	register_cache_fn( replace, { "replace" } );
	register_cache_fn( invalidate, { "invalidate" } );
	register_cache_fn( iscached, { "iscached" } );

	register_fn( cloneref, { "cloneref" } );
	register_fn( compareinstances, { "compareinstances" } );
}
