#pragma once
#include "RetCheck.h"
#include <Windows.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <string>
#include <windows.h> 
#include <iostream> 
#include <process.h>
#include <Shlwapi.h>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <unordered_map>

std::string DATASTREAM = "DataStream/WrapperInterval/DataHandler";

#define ASLR(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))
#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(unprotect(ASLR(address)))
#define x(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))
#define r_l_gettop(rL) ((*(DWORD *)(rL + 20) - *(DWORD *)(rL + 24)) >> 4)
#define r_l_pop(rL, n) r_l_settop(rL, -(n) - 1)
#define r_l_getglobal(rL, k) r_l_getfield(rL, -10002, k)
#define R_LUA_TNIL						 0
#define R_LUA_TLIGHTUSERDATA              1
#define R_LUA_TNUMBER					 2
#define R_LUA_TBOOLEAN					 3
#define R_LUA_TSTRING					 4
#define R_LUA_TTHREAD                     5
#define R_LUA_TFUNCTION					 6
#define R_LUA_TTABLE						 7
#define R_LUA_TUSERDATA					 8
#define R_LUA_TPROTO					     9
#define R_LUA_TUPVAL					     10

extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua/ldo.h"
}

#include <vector>

DWORD RobloxState;
lua_State* VanillaState;
std::vector<int> int3bp;

union r_Value {
	PVOID gc;
	PVOID p;
	double n;
	int b;
};


struct r_TValue {
	r_Value value;
	int tt;
};


// -- Lua C Functions -- \\

typedef DWORD(__cdecl *rref)(DWORD, DWORD);
rref r_l_lref = (rref)(Ret::unprotect<DWORD>((byte*)(x(0x80a310))));

auto r_l_index2adr = Declare(0x8068d0, r_TValue*, __stdcall, DWORD a1, int idx);
auto r_l_getfield = Declare(0x80f7f0, void, __stdcall, int a1, int idx, const char *key);
auto r_l_gettable = Declare(0x80fc70, void, __cdecl, int a1, int idx);
auto r_l_createtable = Declare(0x80f480, void, __cdecl, int a1, int narray, int nrec);
auto r_l_tonumber = Declare(0x811d90, double, __stdcall, int a1, signed int a2);
auto r_l_touserdata = Declare(0x811ed0, void*, __cdecl, int a1, int idx);
auto r_l_newuserdata = Declare(0x810350, void*, __cdecl, int a1, int size);
auto r_l_getmetatable = Declare(0x80fac0, int, __cdecl, int a1, signed int a2);
auto r_l_toboolean = Declare(0x811b90, int, __cdecl, int a1, signed int a2);
auto r_l_pushcclosure = Declare(0x8106e0, void, __fastcall, DWORD a1, int a2, int a3);
auto r_l_pushlightuserdata = Declare(0x810950, void, __cdecl, DWORD a1, void *p);
auto r_l_pushstring = Declare(0x810b70, int, __cdecl, DWORD a1, const char* a2);
auto r_l_pushvalue = Declare(0x810c30, int, __cdecl, DWORD a1, int idx);
auto r_l_pushnumber = Declare(0x810ae0, void, __stdcall, DWORD a1, double a2);
auto r_l_pushboolean = Declare(0x810660, void, __cdecl, DWORD a1, int a2);
auto r_l_pushnil = Declare(0x810a70, void, __cdecl, DWORD a1);
auto r_l_next = Declare(0x810410, int, __cdecl, DWORD a1, int idx);
auto r_l_type = Declare(0x811f00, int, __cdecl, DWORD a1, int idx);
auto r_l_setfield = Declare(0x8115c0, void, __fastcall, DWORD a1, int a2, const char* a3);
auto r_l_settable = Declare(0x811980, void, __cdecl, DWORD a1, int idx);
auto r_l_tolstring = Declare(0x811c50, const char*, __fastcall, DWORD a1, int a2, std::size_t* a3);
auto r_l_setreadonly = Declare(0x8118a0, void, __cdecl, DWORD a1, int a2, int a3);
auto r_l_setmetatable = Declare(0x811770, int, __cdecl, DWORD a1, int a2);
auto r_l_newthread = Declare(0x810260, int, __cdecl, DWORD a1);
auto r_l_rawgeti = Declare(0x810ee0, void, __cdecl, DWORD a1, int idx, int n);
auto r_l_rawseti = Declare(0x811060, void, __cdecl, DWORD a1, int idx, int n);
auto r_l_settop = Declare(0x811a10, void, __cdecl, DWORD a1, int idx);
auto r_l_getfenv = Declare(0x80f700, void, __cdecl, DWORD a1, int idx);
auto r_l_pcall = Declare(0x810590, int, __cdecl, DWORD a1, int a2, int a3, int a4);
auto r_lL_error = Declare(0x809cf0, int, __cdecl, DWORD a1, const char *lol, ...);
auto writerf = Declare(0x542240, int, __cdecl, int idx, const char *lol, ...);
auto Spawn = Declare(0x7f03e0, int, __cdecl, DWORD a1);

#define r_l_isnil(L,n)		(r_l_type(L, (n)) == LUA_TNIL)



#define r_l_pushcfunction(L,f)	r_l_pushcclosure(L, (f), 0)

void r_l_pushobject(int rL, r_TValue *Value) {
	r_TValue *Top = (r_TValue*)*(DWORD *)(rL + 32);
	Top->tt = Value->tt;
	Top->value = Value->value;
	*(DWORD *)(rL + 32) += 16;
}


