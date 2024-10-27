#include "cache.hpp"

#include <fstream>
#include <set>

#include <managers/managers.hpp>
#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>
#include <Luau/lmem.h>

std::int32_t roblox::environment::cache::invalidate( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TUSERDATA );

	lua_pushlightuserdata( state, reinterpret_cast< void* >(managers::mgrs.offset_mgr.roblox.push_instance.function) );
	lua_rawget( state, LUA_REGISTRYINDEX );

	lua_pushlightuserdata( state, *reinterpret_cast< void** >( lua_touserdata( state, 1 ) ) );
	lua_pushnil( state );
	lua_rawset( state, -3 );
	
	return 0;
}

std::int32_t roblox::environment::cache::is_cached( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TUSERDATA );

	lua_pushlightuserdata( state, reinterpret_cast< void* >( managers::mgrs.offset_mgr.roblox.push_instance.function ) );
	lua_rawget( state, LUA_REGISTRYINDEX );
	
	lua_pushlightuserdata( state, *reinterpret_cast< void** >( lua_touserdata( state, 1 ) ) );
	lua_rawget( state, -2 );

	lua_pushboolean( state, lua_isuserdata( state, -1 ) );

	return 1;
}

std::int32_t roblox::environment::cache::replace( lua_State* state )
{
	l( );

	luaL_checktype( state, 1, LUA_TUSERDATA );
	luaL_checktype( state, 2, LUA_TUSERDATA );

	lua_pushlightuserdata( state, reinterpret_cast< void* >( managers::mgrs.offset_mgr.roblox.push_instance.function ) );
	lua_rawget( state, LUA_REGISTRYINDEX );

	lua_pushlightuserdata( state, *reinterpret_cast< void** >( lua_touserdata( state, 1 ) ) );
	lua_pushvalue( state, 2 );
	lua_rawset( state, -3 );

	return 0;
}