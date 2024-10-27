#include "console.hpp"

#include <utilities/io.hpp>

#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>

#include <managers/managers.hpp>

std::int32_t roblox::environment::console::rconsoleprint( lua_State* state )
{
	utilities::io::open( );

	utilities::io::log( utilities::io::log_options_t::no_tag, luaL_checkstring( state, 1 ) );

	return 0;
}

std::int32_t roblox::environment::console::rconsoleerr( lua_State* state )
{
	utilities::io::open( );

	utilities::io::log( utilities::io::log_options_t::error, luaL_checkstring( state, 1 ) );

	return 0;
}

std::int32_t roblox::environment::console::rconsoleinfo( lua_State* state )
{
	utilities::io::open( );

	utilities::io::log( luaL_checkstring( state, 1 ) );

	return 0;
}

std::int32_t roblox::environment::console::rconsolewarn( lua_State* state )
{
	utilities::io::open( );

	utilities::io::log( utilities::io::log_options_t::warning, luaL_checkstring( state, 1 ) );

	return 0;
}

std::int32_t roblox::environment::console::rconsolename( lua_State* state )
{
	utilities::io::open( );

	SetConsoleTitleA( luaL_checkstring( state, 1 ) );

	return 0;
}

std::int32_t roblox::environment::console::rconsoleclear( lua_State* state )
{
	std::system( "cls" );

	return 0;
}

std::int32_t roblox::environment::console::rconsoleinput( lua_State* state )
{
	utilities::io::open( );

	lua_pushthread( state );
	lua_ref( state, -1 );

	std::thread { 
		[ state ] ( )
		{
			std::string res;

			std::getline( std::cin, res );

			managers::mgrs.roblox_mgr.queue_yield_callback(
			[ state, res ] ( ) /* todo: impersonator of thread */
			{
				lua_pushstring( state, res.c_str( ) );

				managers::mgrs.roblox_mgr.resume( state, 1 );
			} );
		}
	}.detach( );

	return lua_yield( state, 1 );
}

std::int32_t roblox::environment::console::rconsoleshow( lua_State* state )
{
	utilities::io::open( );

	return 0;
}

std::int32_t roblox::environment::console::rconsolehide( lua_State* state )
{
	utilities::io::close( );

	return 0;
}
