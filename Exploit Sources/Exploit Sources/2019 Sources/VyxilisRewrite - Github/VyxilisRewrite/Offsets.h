#pragma once
#include <stdexcept>
#include <tuple>
#include <typeinfo>
#include <windows.h>
#include <strsafe.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

#include "RetCheck.h"

extern "C" {
#include "Lua\lua.h"
#include "Lua\lapi.h"
#include "Lua\lualib.h"
#include "Lua\lstate.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lapi.h"
#include "Lua\lfunc.h"
#include "Lua\lobject.h"
}

#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, -10002, (s))
#define r_lua_setglobal(rL,s)	r_lua_setfield(rL, -10002, (s))
#define r_lua_isnil(L,n)		(r_lua_type(L, (n)) == R_LUA_TNIL)
#define r_lua_createnewtable(rL) r_lua_createtable(rL, 0, 0)
#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)

#define R_LUA_TNONE (-1)
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

/*
Eternals retcheck
Eternals retcheck is fine but it skips some parts...
*/
namespace Retcheck
{
	DWORD Unprotect(DWORD addr, bool mode)
	{
		if (mode)
			return Ret::unprotect<DWORD>((BYTE*)addr);

		BYTE* tAddr = (BYTE*)addr;
		do
		{
			tAddr += 16;
		} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

		DWORD funcSz = tAddr - (BYTE*)addr;

		PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (nFunc == NULL)
			return addr;

		memcpy(nFunc, (void*)addr, funcSz);

		BYTE* pos = (BYTE*)nFunc;
		BOOL valid = false;
		do
		{
			if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
				*(BYTE*)pos = 0xEB;

				DWORD cByte = (DWORD)nFunc;
				do
				{
					if (*(BYTE*)cByte == 0xE8)
					{
						DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
						DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

						if (oFuncAddr % 16 == 0)
						{
							DWORD relativeAddr = oFuncAddr - cByte - 5;
							*(DWORD*)(cByte + 1) = relativeAddr;

							cByte += 4;
						}
					}

					cByte += 1;
				} while (cByte - (DWORD)nFunc < funcSz);

				valid = true;
			}
			pos += 1;
		} while ((DWORD)pos < (DWORD)nFunc + funcSz);

		if (!valid)
		{
			VirtualFree(nFunc, funcSz, MEM_RELEASE);
			return addr;
		}

		return (DWORD)nFunc;
	}
}



DWORD unprotect(DWORD addr)
{
	BYTE* tAddr = (BYTE*)addr;
	do
	{
		tAddr += 16;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	memcpy(nFunc, (void*)addr, funcSz);

	BYTE* pos = (BYTE*)nFunc;
	BOOL valid = false;
	do
	{
		if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
			*(BYTE*)pos = 0xEB;

			DWORD cByte = (DWORD)nFunc;
			do
			{
				if (*(BYTE*)cByte == 0xE8)
				{
					DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
					DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

					if (oFuncAddr % 16 == 0)
					{
						DWORD relativeAddr = oFuncAddr - cByte - 5;
						*(DWORD*)(cByte + 1) = relativeAddr;

						cByte += 4;
					}
				}

				cByte += 1;
			} while (cByte - (DWORD)nFunc < funcSz);

			valid = true;
		}
		pos += 1;
	} while ((DWORD)pos < (DWORD)nFunc + funcSz);

	if (!valid)
	{
		VirtualFree(nFunc, funcSz, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}

#define R_LUA_REGISTRYINDEX 0xFFFFD8F0
#define ASLR(OFFSET) (OFFSET - 0x400000 + (DWORD)GetModuleHandle(NULL))

typedef DWORD _DWORD;

union r_Value
{
	int b;
	double n;
	void* p;
	void* gc;
};

struct r_TValue
{
	r_Value value;
	int tt;
};

DWORD RLS;
lua_State* LS;
int top = 24, base = 12;

DWORD Identity = ASLR(0x1EEC250);

typedef int(__cdecl* rgetfield)(DWORD rL, int idx, const char *k);
rgetfield r_lua_getfield = (rgetfield)unprotect(ASLR(0x7F47F0));

typedef char*(__fastcall* rtolstring)(DWORD rL, int idx, size_t *size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(ASLR(0x7F6BA0)));

typedef void(__cdecl * rgettable)(DWORD rL, int idx);
rgettable r_lua_gettable = (rgettable)(unprotect(ASLR(0x7F4C50))); /*this is lua_gettable*/

typedef int(__cdecl* pcall)(DWORD rL, DWORD, DWORD, DWORD);
pcall r_lua_pcall = (pcall)(unprotect(ASLR(0x7F55A0)));

typedef double(__stdcall *pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(ASLR(0x7F5AE0)));

typedef DWORD(__cdecl * next)(DWORD RLS, int idx);
next r_lua_next = (next)(unprotect(ASLR(0x7F5420)));

typedef r_TValue*(__stdcall *rlua_index2)(int lst, int idx);
rlua_index2 r_lua_index2 = (rlua_index2)ASLR(0x7EC000);

typedef void*(__cdecl *touserdata)(DWORD, int);
touserdata r_lua_touserdata = (touserdata)Retcheck::Unprotect(ASLR(0x7F6FB0), true);

typedef void(__cdecl *rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(ASLR(0x7F5ED0));

typedef void*(__fastcall  *rgetmetatable)(DWORD RLS, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(ASLR(0x7F4AB0)));

typedef void*(__cdecl *rsettable)(DWORD RLS, int idx);
rsettable r_lua_settable = (rsettable)(unprotect(ASLR(0x7F4C50))); /*this is lua_settable*/

typedef int(__cdecl *rLuaS_newlstr)(int RL, const char *str, size_t l);
rLuaS_newlstr r_luaS_newlstr = (rLuaS_newlstr)ASLR(0x7f01e0);

void r_lua_pushstring(int rState, const char *s) {
	int v3 = *(DWORD *)(rState + top);
	*(DWORD *)v3 = r_luaS_newlstr(rState, s, strlen(s));
	*(DWORD *)(v3 + 8) = R_LUA_TSTRING;
	*(DWORD *)(rState + top) += 16;
}

void r_lua_settop(int a1, int a2) {
	int i;
	if (a2 < 0)
	{
		*(_DWORD *)(a1 + top) += 16 * a2 + 16;
	}
	else
	{
		for (i = 16 * a2; *(_DWORD *)(a1 + top) < (unsigned int)(i + *(_DWORD *)(a1 + base)); *(_DWORD *)(a1 + top) += 16)
			*(_DWORD *)(*(_DWORD *)(a1 + top) + 8) = 0;
		*(_DWORD *)(a1 + top) = i + *(_DWORD *)(a1 + base);
	}
}

void r_lua_pushvalue(DWORD a1, int a2)
{
	r_TValue *p = r_lua_index2(a1, a2);
	int v3 = *(DWORD *)(a1 + top);
	*(DWORD *)v3 = *(DWORD *)p;
	*(DWORD *)(v3 + 4) = *(DWORD *)(p + 4);
	*(DWORD *)(v3 + 8) = p->tt;
	*(DWORD *)(a1 + top) += 16;
}

void r_lua_setfield(DWORD L, int idx, const char *k) {
	r_lua_pushvalue(L, idx);
	if (r_lua_getmetatable(L, -1)) {
		r_lua_getfield(L, -1, "__newindex");
		r_lua_pushvalue(L, -3);
		r_lua_pushstring(L, k);
		r_lua_pushvalue(L, -6);
		r_lua_pcall(L, 3, 1, 0);
		r_lua_pop(L, 3);
	}
	else {
		r_lua_pop(L, 1);
		r_lua_setfield(L, idx, k);
	}
}

void r_lua_pushnil(DWORD a1) {
	*(_DWORD *)(*(_DWORD *)(a1 + top) + 8) = 0;
	*(_DWORD *)(a1 + top) += 16;
}

void r_lua_pushboolean(DWORD a1, int a2) {
	_DWORD *v2;
	v2 = *(_DWORD **)(a1 + top);
	*v2 = a2 != 0;
	v2[2] = 3;
	*(_DWORD *)(a1 + top) += 16;
}

inline void r_lua_getService(DWORD RLS, const char* s)
{
	r_lua_getfield(RLS, -10002, "game");
	r_lua_getfield(RLS, -1, "GetService");
	r_lua_pushvalue(RLS, -2);
	r_lua_pushstring(RLS, s);
	r_lua_pcall(RLS, 2, 1, 0);
}