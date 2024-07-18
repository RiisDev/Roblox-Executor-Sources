#pragma once
#include "Includes.h"
#include "retcheck.h"


#define Unformat(a)(a + 0x400000 - (DWORD)GetModuleHandleA(0))
#define format(x)(x - 0x400000 + (DWORD)GetModuleHandle(NULL))

#define DEBUG

#if true
#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(Retcheck::unprotect(format(address)))
#else
#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(address)
#endif 

#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 1
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 3
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 5
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 8
#define RLUA_TPROTO 9


namespace RBX {
	const auto lua_gettable = Declare(0x00801D60, void, __cdecl, int, int);
	const auto lua_createtable = Declare(0x00801590, void, __cdecl, int, int, int);
	const auto lua_getgenv = Declare(0x00801810, void, __cdecl, int, int);
	const auto luaL_ref = Declare(0x007FC8F0, int, __cdecl, int, int);
	const auto lua_pushstring = Declare(0x00802C70, void, __fastcall, int, const char*);
	const auto lua_pushvalue = Declare(0x00802D20, void, __stdcall, int, int);
	const auto lua_pushnumber = Declare(0x00802BE0, void, __cdecl, int, int);
	const auto lua_pushnil = Declare(0x00802B70, void, __cdecl, int);
	const auto lua_pushcclosure = Declare(0x00802800, void, __cdecl, int, int, int);
	const auto lua_pushboolean = Declare(0x00802780, void, __cdecl, int, bool);
	const auto lua_pcall = Declare(0x008026B0, int, __cdecl, int, int, int, int);
	const auto lua_next = Declare(0x00802530, int, __cdecl, int, int);
	const auto lua_newuserdata = Declare(0x00802480, void*, __cdecl, int, int);
	const auto lua_setfenv = Declare(0x00803590, void, __cdecl, int, int);
	const auto lua_setfield = Declare(0x00803680, void, __stdcall, int, int, const char*);
	const auto lua_getfield = Declare(0x008018E0, void, __cdecl, int, int, const char*);
	const auto lua_rawgeti = Declare(0x00802FD0, void, __cdecl, int, int, int);
	const auto lua_gettop = Declare(0x00801DE0, int, __cdecl, int);
	const auto lua_toboolean = Declare(0x00803BF0, bool, __cdecl, int, bool);
	const auto lua_tolstring = Declare(0x00803CB0, const char*, __stdcall, int, int, int);
	const auto lua_touserdata = Declare(0x008040D0, int, __cdecl, int, int);
	const auto lua_settable = Declare(0x008039E0, void, __cdecl, int, int);
	const auto lua_settop = Declare(0x00803A70, void, __cdecl, int, int);
	const auto lua_newthread = Declare(0x008023A0, int, __cdecl, int);
	const auto lua_getmetatable = Declare(0x00801BB0, int, __stdcall, int, int);
	const auto luaE_newthread = Declare(0x007FAD20, int, __cdecl, int);
	const auto lua_type = Declare(0x00804100, int, __cdecl, int, int);
	const auto lua_pushlightuserdata = Declare(0x00802A50, void*, __cdecl, int, void*);
}

typedef double(__stdcall* rtonumber)(int, int);
rtonumber r_tonumber = (rtonumber)format(0x00803F90);