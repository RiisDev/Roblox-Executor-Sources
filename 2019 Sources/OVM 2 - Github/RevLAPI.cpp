
#include "RevLAPI.hpp"
#include "../Lua/lobject.h"

void rev_lua_settop(DWORD rL, signed int idx)
{
	*(DWORD*)(rL + 20) += 16 * idx + 16;
}

void rev_lua_pushlightuserdata(DWORD rL, void * ud)
{
	TValue* upv = (TValue*)(*(DWORD*)(rL + 20));
	upv->tt = R_LUA_TLIGHTUSERDATA;
	upv->value.p = ud;
	*(DWORD*)(rL + 20) += 16;
}
