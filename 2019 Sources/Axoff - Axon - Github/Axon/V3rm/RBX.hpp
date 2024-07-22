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
#include "RetCheck.hpp"

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

#define R_LUA_TNONE	(-1)
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
#define R_LUA_REFNIL (-1)
typedef DWORD _DWORD;

union r_lua_Value
{
	int b;
	double n;
	void* p;
	void* gc;
};

struct r_lua_TValue
{
	r_lua_Value value;
	int tt;
};

DWORD RLS;
lua_State* LS;
DWORD Address_ScriptContext = 0x19C5D7C;
int top = 24, rbx_base = 28;
std::vector<int> int3breakpoints;
#define x(Address) (Address - 0x400000 + (DWORD)GetModuleHandle(NULL))
#define Level(rL, i) DWORD* identity  = (DWORD*)(rL - 40); *identity ^= (i ^ (unsigned __int8)*identity) & 0x1F;

DWORD Address_GetField = 0x851FB0;//1
DWORD Address_Tolstring = 0x854a80;//2
DWORD Address_Toboolean = 0x854970;//3
DWORD Address_PushValue = 0x853700;//4
DWORD Address_PushNumber = 0x853510;//5
DWORD Address_PushString = 0x8535A0;//6
DWORD Address_Pcall = 0x852fb0;//7
DWORD Address_Next = 0x852df0;//8
DWORD Address_ToNumber = 0x854bf0;//9
DWORD Address_PushCClosure = 0x853110;//10
DWORD Address_CreateTable = 0x851C40;//11
DWORD Address_NewThread = 0x852c50;//12
DWORD Address_NewUserData = 0x852D40;//13
DWORD Address_RawGeti = 0x853a50;//14
DWORD Address_RawSeti = 0x853c10;//15
DWORD Address_GetMetaTable = 0x852370;//16
DWORD Address_ToUserData = 0x854ea0;//17
DWORD Address_Type = 0x854f40;//18
DWORD Address_Settable = 0x854710;//19
DWORD Address_PushLight = 0x853380; //20
DWORD Address_Gettop = 0x8525e0;//21
DWORD Address_Tointeger = 0x8549c0;//22
DWORD Address_Objlen = 0x852ea0;//23

typedef void(__fastcall* rgetfield)(DWORD rL, int idx, const char *k);
rgetfield r_lua_getfield = (rgetfield)unprotect(x(Address_GetField));//Updated

typedef char*(__stdcall* rtolstring)(DWORD rL, int idx, size_t *size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(x(Address_Tolstring)));//Updated

typedef bool(__cdecl* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(x(Address_Toboolean));//Updated

typedef void(__fastcall* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(x(Address_PushValue)));//Updated

typedef double(__stdcall* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(x(Address_PushNumber)));//Updated

typedef void(__fastcall* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(x(Address_PushString));//

auto r_lua_pcall = (int(__cdecl*)(DWORD, int, int, int))unprotect(x(Address_Pcall));//Updated

typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(unprotect(x(Address_Next)));//Updated

double r_lua_tonumber(DWORD rL, int idx)
{
	return ((double(__cdecl*)(DWORD, int, int))x(Address_ToNumber))(rL, idx, 0);//Updated
}

typedef void(__fastcall* rpushcclosure)(DWORD rL, int fn, int non);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(unprotect(x(Address_PushCClosure)));//Updated

typedef void(__cdecl *rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(x(Address_CreateTable)));//Updated

typedef DWORD(__cdecl* rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)unprotect(x(Address_NewThread));//Updated

void* r_lua_newuserdata(DWORD rL, size_t size)//Updated
{
	return ((void* (__cdecl*)(DWORD rL, size_t size, int))unprotect(x(Address_NewUserData)))(rL, size, 0);
}

typedef void(__cdecl* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(x(Address_RawGeti));//Updated

typedef void(__cdecl* rrawseti)(DWORD, DWORD, DWORD);
rrawseti r_lua_rawseti = (rrawseti)unprotect(x(Address_RawSeti));//Updated

typedef void*(__cdecl*rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(x(Address_GetMetaTable)));//Updated

auto r_lua_touserdata = (int(__cdecl*)(DWORD, int))x(Address_ToUserData);//Updated

typedef DWORD(__cdecl* rtype)(DWORD, int);
rtype r_lua_type = (rtype)(x(Address_Type));//Updated

typedef void*(__cdecl *rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(x(Address_Settable)));//Updated

typedef void(__cdecl* rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)(unprotect(x(Address_PushLight)));//Updated

typedef int(__cdecl *gettop)(DWORD);
gettop r_lua_gettop = (gettop)(x(Address_Gettop));//Updated

typedef int(__cdecl* rtointeger)(int, int, int);
rtointeger r_lua_tointeger = (rtointeger)(x(Address_Tointeger));//Updated

typedef int(__cdecl* robjlen)(int, int);
robjlen r_lua_objlen = (robjlen)unprotect(x(Address_Objlen));//Updated

#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)
#define r_lua_isnil(L,n)		(r_lua_type(L, (n)) == 0)
#define abs_index(L, i)		((i) > 0 || (i) <= -10000 ? (i) : \
					r_lua_gettop(L) + (i) + 1)

void r_lua_settop(int a1, int a2) {
	int i;
	if (a2 < 0)
	{
		*(_DWORD*)(a1 + top) += 16 * a2 + 16;
	}
	else
	{
		for (i = 16 * a2; *(_DWORD*)(a1 + top) < (unsigned int)(i + *(_DWORD*)(a1 + rbx_base)); *(_DWORD*)(a1 + top) += 16)
			* (_DWORD*)(*(_DWORD*)(a1 + top) + 8) = 0;
		*(_DWORD*)(a1 + top) = i + *(_DWORD*)(a1 + rbx_base);
	}
}

void r_lua_pushnil(DWORD a1) {
	*(_DWORD*)(*(_DWORD*)(a1 + top) + 8) = 0;
	*(_DWORD*)(a1 + top) += 16;
}

void r_lua_pushboolean(DWORD a1, int a2) {
	_DWORD* v2;
	v2 = *(_DWORD * *)(a1 + top);
	*v2 = a2 != 0;
	v2[2] = 3;
	*(_DWORD*)(a1 + top) += 16;
}

int r_luaL_ref(int L, int t) {
	int ref;
	t = abs_index(L, t);
	if (r_lua_isnil(L, -1)) {
		r_lua_pop(L, 1);
		return R_LUA_REFNIL; 
	}
	r_lua_rawgeti(L, t, 0);  
	ref = (int)r_lua_tointeger(L, -1, 0);  
	r_lua_pop(L, 1); 
	if (ref != 0) {  
		r_lua_rawgeti(L, t, ref);  
		r_lua_rawseti(L, t, 0); 
	}
	else {  
		ref = (int)r_lua_objlen(L, t);
		ref++; 
	}
	r_lua_rawseti(L, t, ref);
	return ref;
}

