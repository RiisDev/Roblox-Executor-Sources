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
#include <Windows.h>
#include <ios>
#include <vector>

extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\ldo.h"
}

#include "Ret.h"

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

#define R_LUA_REGISTRYINDEX 0xFFFFD8F0
#define ASLR(address) (address - 0x400000 + (DWORD)GetModuleHandle(NULL))
#define lua_register(L,n,f) (lua_pushcfunction(LuaC.VanillaState, (f)), lua_setglobal(LuaC.VanillaState, (n)))
#define SetIdentity(rL, i) DWORD* identity  = (DWORD*)(rL - 40); *identity ^= (i ^ (unsigned __int8)*identity) & 0x1F;
#define _DWORD DWORD
std::vector<int> int3bp;

#define r_lua_tostring(rL,i)	LuaC.r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		LuaC.r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	LuaC.r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) LuaC.r_lua_createtable(rL, 0, 0)

struct RbxState
{
	unsigned long rawState;
	int const top = 32;
	int const base = 8;
	int const env = 64;
	int const cdt = 48;

	unsigned long ScriptContextVFTable = ASLR(0x1CA904C);
	unsigned long ScriptContext;
	unsigned int CoreScript = 0;
	std::vector<std::string> Globals{
		"printidentity","game","Game","workspace","Workspace","Axes","BrickColor","CFrame","Color3","ColorSequence","ColorSequenceKeypoint","NumberRange","NumberSequence","NumberSequenceKeypoint","PhysicalProperties","Ray","Rect","Region3","Region3int16","TweenInfo","UDim","UDim2","Vector2", "Vector2int16","Vector3","Vector3int16","Enum","Faces","Instance","math","warn","typeof","type","print","printidentity","ypcall","Wait","wait","delay","Delay","tick","LoadLibrary","spawn","Spawn"
	};
};

struct rbxLua
{
	RbxState *RobloxState;
	lua_State *VanillaState;

	typedef int(__cdecl*rLuaS_newlstr)(RbxState *RL, const char *str, size_t l);
	rLuaS_newlstr r_luaS_newlstr = (rLuaS_newlstr)ASLR(0x80c4e0);

	typedef TValue*(__fastcall*rlua_index2adr)(RbxState *lst, int idx);
	rlua_index2adr r_lua_index2adr = (rlua_index2adr)ASLR(0x808030);

	typedef int(__cdecl*rlua_pcall)(RbxState*rs, int a1, int a2, int a3);
	rlua_pcall r_lua_pcall = (rlua_pcall)Retcheck::Unprotect(0x811c70, true);

	typedef void*(__cdecl*rlua_touserdata)(RbxState *rs, int a1);
	rlua_touserdata r_lua_touserdata = (rlua_touserdata)Retcheck::Unprotect(0x8135a0, true);

	typedef DWORD(__cdecl* setfield)(RbxState * rL, int idx, const char* k);
	setfield r_lua_setfield = (setfield)(unprotect(ASLR(0x812cb0)));

	typedef void(__stdcall* rgetfield)(RbxState * rL, int idx, const char *k);
	rgetfield r_lua_getfield = (rgetfield)unprotect(ASLR(0x810ed0));

	typedef char*(__fastcall* rtolstring)(RbxState * rL, int idx, size_t *size);
	rtolstring r_lua_tolstring = (rtolstring)(unprotect(ASLR(0x813330)));

	typedef bool(__cdecl* rtoboolean)(RbxState * rL, bool idx);
	rtoboolean r_lua_toboolean = (rtoboolean)(ASLR(0x813270));

	typedef double(__thiscall* rpushnumber)(RbxState * rL, double idx);
	rpushnumber r_lua_pushnumber = (rpushnumber)(unprotect(ASLR(0x8121c0)));

	typedef DWORD(__cdecl* next2)(RbxState * rL, int idx);
	next2 r_lua_next = (next2)(unprotect(ASLR(0x811af0)));

	typedef double(__stdcall * rtonumber)(RbxState *, int);
	rtonumber r_lua_tonumber = (rtonumber)(ASLR(0x813460));

	typedef void(__stdcall* rpushcclosure)(RbxState * rL, int fn, int non);
	rpushcclosure r_lua_pushcclosure = (rpushcclosure)(unprotect(ASLR(0x811dc0)));

	typedef void(__cdecl *rcreatetable)(RbxState * rL, int num, int fix);
	rcreatetable r_lua_createtable = (rcreatetable)(unprotect(ASLR(0x4e4a70)));

	typedef void*(__cdecl *rnewuserdata)(RbxState * rL, int idx);
	rnewuserdata r_lua_newuserdata = (rnewuserdata)(unprotect(ASLR(0x811a30)));

	typedef void*(__cdecl*rgetmetatable)(RbxState * rL, int idx);
	rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(ASLR(0x8111a0)));

	typedef void*(__cdecl *rsettable)(RbxState * rL, int);
	rsettable r_lua_settable = (rsettable)(unprotect(ASLR(0x813060)));

	typedef DWORD(__cdecl *rtype)(RbxState *, int);
	rtype r_lua_type = (rtype)(ASLR(0x8135d0));

	typedef RbxState*(__cdecl *rnewthread)(RbxState *);
	rnewthread r_lua_newthread = (rnewthread)unprotect(ASLR(0x811940));

	typedef DWORD(__cdecl *rref)(RbxState *, DWORD);
	rref r_luaL_ref = (rref)(Retcheck::Unprotect((unsigned long)(ASLR(0x80ba40)), true));

	typedef void(__cdecl *rrawgeti)(RbxState *, DWORD, DWORD);
	rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(ASLR(0x8125b0));

	typedef void(__cdecl *rpushlight)(RbxState *, void*);
	rpushlight r_lua_pushlightuserdata = (rpushlight)(unprotect(ASLR(0x812030)));

	typedef void(__cdecl*rLsandbox)(RbxState *, int, int);
	rLsandbox r_lua_sandbox = (rLsandbox)(ASLR(0x7f17e0));

	void r_lua_pushstring(RbxState *roblox_state, const char* s) {
		int v3 = *(DWORD *)(roblox_state + roblox_state->top); 
		*(DWORD *)v3 = r_luaS_newlstr(roblox_state, s, strlen(s));
		*(DWORD *)(v3 + 8) = R_LUA_TSTRING;
		*(DWORD *)(roblox_state + roblox_state->top) += 16;
	}

	void r_lua_pushboolean(RbxState *a1, int a2) {
		_DWORD *v2;
		v2 = *(_DWORD **)(a1 + a1->top);
		*v2 = a2 != 0;
		v2[2] = 3;
		*(_DWORD *)(a1 + a1->top) += 16;
	}

	void r_lua_settop(RbxState *a1, int a2) {
		int i;
		if (a2 < 0)
		{
			*(_DWORD *)(a1 + a1->top) += 16 * a2 + 16;
		}
		else
		{
			for (i = 16 * a2; *(_DWORD *)(a1 + a1->top) < (unsigned int)(i + *(_DWORD *)(a1 + a1->base)); *(_DWORD *)(a1 + a1->top) += 16)
				*(_DWORD *)(*(_DWORD *)(a1 + a1->top) + 8) = 0;
			*(_DWORD *)(a1 + a1->top) = i + *(_DWORD *)(a1 + a1->base);
		}
	}

	void r_lua_pushobject(RbxState *roblox_state, TValue *value) {
		auto &topa = *(TValue**)(roblox_state + roblox_state->top);
		*topa = *value;
		++topa;
	}

	void r_lua_pushvalue(RbxState *roblox_state, int a2)
	{
		TValue *p = r_lua_index2adr(roblox_state, a2);
		int v3 = *(DWORD *)(roblox_state + roblox_state->top);
		*(DWORD *)v3 = *(DWORD *)p;
		*(DWORD *)(v3 + 4) = *(DWORD *)(p + 4);
		*(DWORD *)(v3 + 8) = p->tt;
		*(DWORD *)(roblox_state + roblox_state->top) += 16;
	}

	int r_lua_gettop(RbxState *a1)
	{
		return (*(_DWORD *)(a1 + a1->top) - *(_DWORD *)(a1 + a1->base)) >> 4;
	}

	void r_lua_pushnil(RbxState *a1) {
		*(_DWORD *)(*(_DWORD *)(a1 + a1->top) + 8) = 0;
		*(_DWORD *)(a1 + a1->top) += 16;
	}

} LuaC; /* Lua Functions */ 
