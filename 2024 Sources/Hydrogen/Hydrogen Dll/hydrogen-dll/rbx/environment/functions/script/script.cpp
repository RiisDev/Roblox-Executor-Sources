#include "script.hpp"

#include <rbx/environment/utilities/utilities.hpp>
#include <rbx/environment/utilities/thread/thread.hpp>
#include <rbx/environment/utilities/protected_string/protected_string.hpp>
#include <rbx/environment/environment.hpp>
#include <dependencies/sha384/sha384.h>

#include <set>

sol::table rbx::environment::c_script::getsenv( instance_t script, sol::this_state L )
{
	script.e_is_a( "ModuleScript", "LocalScript" );

	if ( !script.valid( ) || !script.pointer( ) )
		luaL_error( L, "Script is not loaded" );

	sol::state_view sv{ L };

	const auto addr = script.get_internal( );

	if ( !addr )
		luaL_error( L, "Script is not loaded" );

	const auto scripts = utilities::c_thread::get_scripts_with_thread( L );

	for ( const auto [key, value] : scripts )
	{
		if ( reinterpret_cast< std::uint8_t* >( key.get( ) ) == addr )
			return value;
	}

	luaL_error( L, "Script is not loaded" );
}

std::int32_t rbx::environment::c_script::getcallingscript( lua_State* L )
{
	const auto threads_script = utilities::
		c_thread::get_threads_script< std::shared_ptr< void* > >( L );

	luaD_checkstack( L, 1 );

	rbx::instance::offs
		::push_instance( L, threads_script );

	return 1;
}

// TODO: random chunkname
sol::function rbx::environment::c_script::getscriptclosure( instance_t script, sol::this_state L )
{
	script.e_is_a( "ModuleScript", "LocalScript" );

	if ( const auto bytecode = utilities::c_protected_string::get_script_bytecode( L, script.get_internal( ) ) )
	{
		const auto value = *bytecode;

		if ( value.front( ) )
		{
			if ( !luau_load( L, "@", value.c_str( ), value.size( ), 0 ) )
				return sol::stack::get< sol::function >( L, -1 );
		}
	}

	return sol::nil;
}

std::string rbx::environment::c_script::getscriptbytecode( instance_t script, sol::this_state L )
{
	script.e_is_a( "ModuleScript", "LocalScript" );

	if ( const auto bytecode = utilities::c_protected_string::get_script_bytecode( L, script.get_internal( ) ) )
		return *bytecode;

	return "Script has no bytecode";
}

//TODO: switch this to cryptopp because it seems to return a different hash than others.
std::string rbx::environment::c_script::getscripthash( instance_t script, sol::this_state L )
{
	script.e_is_a( "ModuleScript", "LocalScript" );

	if ( const auto bytecode = utilities::c_protected_string::get_script_bytecode( L, script.get_internal( ) ) )
	{
		return sha384( *bytecode );
	}

	luaL_error( L, "Invalid bytecode gotten from script" );

	return { };
}

void rbx::environment::c_script::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};
	 
	register_fn( getsenv, { "getsenv", "getmenv" } );
	register_fn( getcallingscript, { "getcallingscript" } );
	register_fn( getscriptclosure, { "getscriptclosure", "getscriptfunction" } );
	register_fn( getscriptbytecode, { "getscriptbytecode", "dumpstring" } );
	register_fn( getscripthash, { "getscripthash" } );
}