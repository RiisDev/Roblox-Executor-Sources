#pragma once
#include "UnmanagedPointer.hpp"
#include "GeneralUpdates.hpp"

namespace
{
	uintptr_t fixOffset(uintptr_t address)
	{
		return address - 0x400000 + reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));
	}
}

#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)
#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))


/* thread status; 0 is OK */
#define LUA_YIELD	1
#define LUA_ERRRUN	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define LUA_ERRERR	5


typedef int (*rlua_CFunction) (uintptr_t L);
