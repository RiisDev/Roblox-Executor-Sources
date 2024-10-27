#pragma once

#include <Luau/lfunc.h>
#include <Luau/lua.h>

#include <dependencies/common_includes.hpp>

struct lua_State;

namespace roblox::environment::crypt
{
	std::int32_t base_64_encode(lua_State* state);
	std::int32_t base_64_decode(lua_State* state);

	std::int32_t encrypt(lua_State* state);
	std::int32_t decrypt(lua_State* state);

	std::int32_t generatekey(lua_State* state);
	std::int32_t generatebytes(lua_State* state);

	std::int32_t hash(lua_State* state);
}