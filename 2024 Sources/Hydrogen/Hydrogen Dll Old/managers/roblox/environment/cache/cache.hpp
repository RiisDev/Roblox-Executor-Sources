#pragma once

#include <Luau/lfunc.h>
#include <Luau/lua.h>

#include <dependencies/common_includes.hpp>

struct lua_State;

namespace roblox::environment::cache
{
	std::int32_t invalidate( lua_State* state );
	std::int32_t is_cached( lua_State* state );
	std::int32_t replace( lua_State* state );
	std::int32_t test( lua_State* state );
}