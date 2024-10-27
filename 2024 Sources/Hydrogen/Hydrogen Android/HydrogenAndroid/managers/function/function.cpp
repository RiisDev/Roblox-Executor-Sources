#include "function.hpp"

#include <managers/roblox/roblox.hpp>

void managers::function_manager_t::lua_pushcclosurek(lua_State* L, lua_CFunction fn, const char* debugname, int nup, lua_Continuation cont)
{
	::lua_pushcclosurek( L, fn, debugname, nup, cont );

	const auto closure = clvalue(managers::function_mgr.index2addr(L, -1));
	managers::roblox_mgr.function_map.insert({ closure, nullptr });
}

