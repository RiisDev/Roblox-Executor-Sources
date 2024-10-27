#pragma once

#include <dependencies/common_includes.hpp>

struct lua_State;

namespace roblox::environment::console
{
	std::int32_t rconsoleprint(lua_State* state);
	std::int32_t rconsoleerr(lua_State* state);
	std::int32_t rconsoleinfo(lua_State* state);
	std::int32_t rconsolewarn(lua_State* state);
	std::int32_t rconsolename(lua_State* state);
	std::int32_t rconsoleclear(lua_State* state);
	std::int32_t rconsoleshow(lua_State* state);
	std::int32_t rconsolehide(lua_State* state);
}