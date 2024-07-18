#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <TlHelp32.h>
#include <sstream>
#include <iosfwd>
#include "Retcheck.h"

extern "C" {
#include "Lua/lua.h"
#include "Lua/luaconf.h"
#include "Lua/lauxlib.h"
#include "Lua/ldo.h"
#include "Lua/lobject.h"
#include "Lua/lopcodes.h"
#include "Lua/lstate.h"
}

#define R_LUA_TNIL 0
#define R_LUA_TLIGHTUSERDATA 1
#define R_LUA_TNUMBER 2
#define R_LUA_TBOOLEAN 3
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 5
#define R_LUA_TFUNCTION 6
#define R_LUA_TTABLE 7
#define R_LUA_TUSERDATA 8
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10

#define runtime_check(L, c)	{ if (!(c)) break; }

#define RA(i)	(base+GETARG_A(i))
/* to be used after possible stack reallocation */
#define RB(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgR, base+GETARG_B(i))
#define RC(i)	check_exp(getCMode(GET_OPCODE(i)) == OpArgR, base+GETARG_C(i))
#define RKB(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgK, \
	ISK(GETARG_B(i)) ? k+INDEXK(GETARG_B(i)) : base+GETARG_B(i))
#define RKC(i)	check_exp(getCMode(GET_OPCODE(i)) == OpArgK, \
	ISK(GETARG_C(i)) ? k+INDEXK(GETARG_C(i)) : base+GETARG_C(i))
#define KBx(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgK, k+GETARG_Bx(i))


#define dojump(L,pc,i)	{(pc) += (i); luai_threadyield(L);}


#define Protect(x)	{ L->savedpc = pc; {x;}; base = L->base; }


#define RBase(x)(x - 0x400000 + (DWORD)GetModuleHandleA(nullptr))

typedef void(__cdecl* GettableV)(uintptr_t, TValue*, TValue*, TValue*, char);
typedef int(__cdecl* _luaD_precall)(uintptr_t, StkId, size_t);
typedef TValue* (__fastcall* _luaS_newlstr)(uintptr_t rL, const char*, size_t);
typedef void* (__cdecl* _luaM_realloc_)(uintptr_t, int, size_t, size_t, size_t);
typedef TValue* (__cdecl* rindex2adr)(DWORD, int);
typedef int(__cdecl* rnewthread)(int);

namespace RLua
{
	rnewthread lua_newthread = reinterpret_cast<rnewthread>(Retcheck::unprotect(RBase(0x849810)));
	rindex2adr Index2Adr = reinterpret_cast<rindex2adr>(RBase(0x83f9d0));
	_luaM_realloc_ luaM_realloc_ = reinterpret_cast<_luaM_realloc_>(RBase(0x843ab0));
	_luaD_precall luaD_precall = reinterpret_cast<_luaD_precall>(Retcheck::unprotect(RBase(0x841090)));
	_luaS_newlstr luaS_newlstr = reinterpret_cast<_luaS_newlstr>(RBase(0x844140));
	GettableV luaV_gettable = reinterpret_cast<GettableV>(RBase(0x847d50));
}