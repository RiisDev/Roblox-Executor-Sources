#ifndef BLACKOUTLUA_H
#define BLACKOUTLUA_H
#include <Windows.h>

#include "util.h"
#include "memory.h"
#include "retcheck.h"

RetcheckBypass func;

/* - ROBLOX Lua C API - */

typedef void(__cdecl* rbx_lua_getfield_def)(INT, INT, const char*);
typedef void(__cdecl* rbx_lua_settop_def)(INT, INT);
typedef void(__cdecl* rbx_lua_pushstring_def)(INT, const char*);
typedef void(__cdecl* rbx_lua_pushvalue_def)(INT, INT);
typedef INT(__cdecl* rbx_lua_pcall_def)(INT, INT, INT, INT);
typedef INT(__cdecl* rbx_lua_toboolean_def)(INT, INT);
typedef void(__cdecl* rbx_lua_pushnumber_def)(INT, double);
typedef void(__cdecl* rbx_lua_setfield_def)(INT, INT, const char*);
typedef INT(__cdecl* rbx_lua_newthread_def)(INT);

rbx_lua_getfield_def rbx_lua_getfield = (rbx_lua_getfield_def)func.cache((PBYTE)(rbx_lua_getfield_addr));
rbx_lua_settop_def rbx_lua_settop = (rbx_lua_settop_def)func.cache((PBYTE)(rbx_lua_settop_addr));
rbx_lua_pushstring_def rbx_lua_pushstring = (rbx_lua_pushstring_def)func.cache((PBYTE)(rbx_lua_pushstring_addr));
rbx_lua_pushvalue_def rbx_lua_pushvalue = (rbx_lua_pushvalue_def)func.cache((PBYTE)(rbx_lua_pushvalue_addr));
rbx_lua_pcall_def rbx_lua_pcall = (rbx_lua_pcall_def)func.cache((PBYTE)(rbx_lua_pcall_addr));
rbx_lua_toboolean_def rbx_lua_toboolean = (rbx_lua_toboolean_def)(rbx_lua_toboolean_addr);
rbx_lua_pushnumber_def rbx_lua_pushnumber = (rbx_lua_pushnumber_def)func.cache((PBYTE)(rbx_lua_pushnumber_addr));
rbx_lua_setfield_def rbx_lua_setfield = (rbx_lua_setfield_def)func.cache((PBYTE)(rbx_lua_setfield_addr));
rbx_lua_newthread_def rbx_lua_newthread = (rbx_lua_newthread_def)func.cache((PBYTE)(rbx_lua_newthread_addr));

INT* rbx_index2adr(INT rbx_L, INT idx) {
	// I'll partially copy IDA pseudocode LOL !!!!!
	INT result;
	INT v3;
	const char rbx_luaO_nilobject[] = { 0, 0, 0, 0 };

	if (idx <= 0)
	{
		if (idx <= -10000)
		{
			switch (idx)
			{
			case -10002: // LUA_GLOBALSINDEX
				result = rbx_L + 104;
				break;
			case -10001: // LUA_ENVIRONINDEX
				result = rbx_L + 72;
				*(INT*)result = *(INT*)(**(INT**)(*(INT*)(rbx_L + 12) + 16) + 12);
				*(INT*)(result + 8) = 7;
				break;
			case -10000: // LUA_REGISTRYINDEX
				result = rbx_L + 8 + *(INT*)(rbx_L + 8) + 168;
				break;
			default:
				v3 = **(INT**)(*(INT*)(rbx_L + 12) + 16);
				if (-10002 - idx > *(unsigned char*)(v3 + 7))
					result = (INT)&rbx_luaO_nilobject;
				else
					result = v3 + 16 * (-10002 - idx) + 8;
				break;
			}
		}
		else
		{
			result = *(INT*)(rbx_L + 16) + 16 * idx;
		}
	}
	else
	{
		result = (INT)&rbx_luaO_nilobject;
		if ((unsigned int)(16 * idx + *(INT*)(rbx_L + 28) - 16) < *(INT*)(rbx_L + 16))
			result = 16 * idx + *(INT*)(rbx_L + 28) - 16;
	}
	return (INT*)result;
}

INT rbx_lua_touserdata(INT rbx_L, signed int idx) {
	INT result;

	INT* o = rbx_index2adr(rbx_L, idx);
	if (o[2] == 1)
	{
		result = *o;
	}
	else if (o[2] == 8)
	{
		result = *o + 24;
	}
	else
	{
		result = NULL;
	}
	return result;
}

void rbx_lua_pushboolean(INT rbx_L, bool boolean) {
	INT v2 = *(INT*)(rbx_L + 16);
	*(INT*)v2 = boolean;
	*(INT*)(v2 + 8) = 3;
	*(INT*)(rbx_L + 16) += 16;
}

/*void rbx_lua_pushvalue(INT rbx_L, INT idx) { // fuck you
	INT* v2;
	INT v3;

	v2 = rbx_index2adr(rbx_L, idx);
	v3 = *(INT*)(rbx_L + 16);
	*(INT*)v3 = *(INT*)v2;
	*(INT*)(v3 + 4) = *(INT*)(v2 + 4);
	*(INT*)(v3 + 8) = *(INT*)(v2 + 8);
	*(INT*)(rbx_L + 16) += 16;
}*/

/* - ROBLOX Lua C API macro definitions  - */

#define rbx_lua_getglobal(rbx_L, k) rbx_lua_getfield(rbx_L, -10002, k)
#define rbx_lua_emptystack(rbx_L) rbx_lua_settop(rbx_L, 0)
#define rbx_lua_pop(rbx_L, n) rbx_lua_settop(rbx_L, -(n)-1)
#endif

// some old attempts at calling getfield i tested but they didn't work, kept for memes,.,..,,

// | attempt 1: | \\

/*#define rbx_index2adr_addr ScanAddress(0x489750)

typedef INT*(__cdecl* index2adr_def)(INT, INT);
index2adr_def index2adr = (index2adr_def)rbx_index2adr_addr;

#define rbx_luaV_gettable_addr ScanAddress(0x48FF70)

typedef INT(__cdecl* rbx_luaV_gettable_def)(INT, INT*, INT, INT);
rbx_luaV_gettable_def rbx_luaV_gettable = (rbx_luaV_gettable_def)rbx_luaV_gettable_addr;

void rbx_lua_gettable(INT rbx_L, INT idx) {
	INT* v2;
	v2 = index2adr(rbx_L, idx);
	rbx_luaV_gettable(rbx_L, v2, *(DWORD *)(rbx_L + 16) - 16, *(DWORD *)(rbx_L + 16) - 16);
}

void rbx_lua_getfield2(INT rbx_L, INT idx, const char* k) {
	rbx_lua_pushstring(rbx_L, k);
	rbx_lua_gettable(rbx_L, idx);
}*/

// | attempt 2: | \\

/*#define rbx_luaS_newlstr_addr ScanAddress(0x48F0E0)

DWORD rbx_luaS_newlstr(INT rbx_L, INT s, size_t l) {
	typedef DWORD(__cdecl* rbx_luaS_newlstr_def)(INT, INT, size_t);
	return ((rbx_luaS_newlstr_def)(rbx_luaS_newlstr_addr))(rbx_L, s, l);
}

typedef void(__cdecl* rbx_luaV_gettable_def)(INT, DWORD, DWORD, DWORD);
rbx_luaV_gettable_def rbx_luaV_gettable = (rbx_luaV_gettable_def)ScanAddress(0x491710);

void rbx_lua_getfield(INT rbx_L, DWORD idx, const char* k) {
	rbx_luaV_gettable(rbx_L, (DWORD)rbx_index2adr(rbx_L, idx), rbx_luaS_newlstr(rbx_L, (INT)k, strlen(k)), *(DWORD*)(rbx_L + 16));
	*(DWORD*)(rbx_L + 16) += 16;
}*/