#pragma once

#include <dependencies/common_includes.hpp>

struct lua_State;

namespace roblox::environment::debug
{
	std::int32_t get_upvalue(lua_State* state);
	std::int32_t get_upvalues(lua_State* state);
	std::int32_t set_upvalue(lua_State* state);

	std::int32_t get_constant(lua_State* state);
	std::int32_t get_constants(lua_State* state);
	std::int32_t set_constant(lua_State* state);

	std::int32_t get_proto(lua_State* state);
	std::int32_t get_protos(lua_State* state);

	std::int32_t get_info(lua_State* state);

	std::int32_t get_stack(lua_State* state);
	std::int32_t set_stack(lua_State* state);
}