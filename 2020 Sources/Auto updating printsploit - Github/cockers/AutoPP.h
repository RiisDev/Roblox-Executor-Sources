#pragma once
#include <windows.h>



typedef uint32_t(__cdecl* T_index2adr)(uint32_t, uint32_t); // Auto updating convention
T_index2adr r_index2adr;

typedef int(__cdecl* T_getfield)(uint32_t, uint32_t, const char* c);
T_getfield r_lua_getfield;

typedef void(__cdecl* T_setfield)(uint32_t, uint32_t, const char* c);
T_setfield r_lua_setfield;


typedef int(__cdecl* rLua_pcall)(DWORD lst, int nargs, int nresults, int errfunc);
rLua_pcall r_lua_pcall;

typedef void(__cdecl* T_pushstring)(DWORD rL, const char* you);
T_pushstring r_lua_pushstring;

typedef void(__cdecl* rsettop)(DWORD rL, int idx);
rsettop r_lua_settop;

typedef int(__cdecl* rtouserdata)(DWORD, int);
rtouserdata r_lua_touserdata;

void r_lua_getglobal(int L, const char* k)
{
	r_lua_getfield(L, -10002, k);
}