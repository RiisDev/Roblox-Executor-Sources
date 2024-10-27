#include "console.hpp"

#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>

#include <managers/managers.hpp>

std::int32_t roblox::environment::console::rconsoleprint(lua_State* state)
{
	managers::utility_mgr.log(luaL_checkstring(state, 1));

	return 0;
}

std::int32_t roblox::environment::console::rconsoleerr(lua_State* state)
{
	managers::utility_mgr.log<ANDROID_LOG_ERROR>(luaL_checkstring(state, 1));

	return 0;
}

std::int32_t roblox::environment::console::rconsoleinfo(lua_State* state)
{
	managers::utility_mgr.log<ANDROID_LOG_DEBUG>(luaL_checkstring(state, 1));

	return 0;
}

std::int32_t roblox::environment::console::rconsolewarn(lua_State* state)
{
	managers::utility_mgr.log<ANDROID_LOG_WARN>(luaL_checkstring(state, 1));

	return 0;
}

std::int32_t roblox::environment::console::rconsolename(lua_State* state)
{
	return 0;
}

std::int32_t roblox::environment::console::rconsoleclear(lua_State* state)
{
	return 0;
}

std::int32_t roblox::environment::console::rconsoleshow(lua_State* state)
{
	return 0;
}

std::int32_t roblox::environment::console::rconsolehide(lua_State* state)
{
	return 0;
}
