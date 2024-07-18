#pragma once
#include <Windows.h>
#include "retcheck.h"
#include <fstream>

extern "C" {
#include "Lua\lua.h"
#include "Lua\ldo.h"
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

lua_State* L;
DWORD rL;

#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)
#define x(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))

#define SetIdentity() *(DWORD*)(*(DWORD*)(m_rL + 112) + 24) = 7
#define StateOffset 164


int GDM;

DWORD top_off = 24;
DWORD base_off = 20;
DWORD OFF_FFC = 0x60CEB0;

typedef DWORD(__cdecl* gdm2)();
gdm2 getdatamodel2 = (gdm2)(x(0xE4D090));


typedef DWORD(__thiscall* getdatamodel)(DWORD, DWORD);
getdatamodel r_getdatamodel = (getdatamodel)(x(0xE4D1E0));

typedef int(__cdecl* ee)(int e, const char* n, ...);
static ee r_luaL_error = (ee)x(0x11E24D0);

typedef int(__cdecl* printlua)(int, const char*);
printlua  print = (printlua)(x(0x5F2D70));

typedef void(__cdecl* rgetfield)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)retcheckBypass(x(0x11E0080));

typedef int(__cdecl* rboolean)(unsigned int, int);
rboolean r_lua_pushboolean = (rboolean)(retcheckBypass(x(0x11E08F0)));

typedef char* (__fastcall* rtolstring)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)(retcheckBypass(x(0x11E1A60)));

typedef bool(__cdecl* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(x(0x11E1980));

typedef void(__cdecl* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(retcheckBypass(x(0x11E0E00)));

typedef double(__cdecl* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(retcheckBypass(x(0x11E0BF0)));

typedef void(__stdcall* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(retcheckBypass(x(0x11E0C50)));

typedef int(__cdecl* rLua_pcall)(DWORD lst, int nargs, int nresults, int errfunc);
rLua_pcall r_lua_pcall = (rLua_pcall)retcheckBypass(x(0x11E0830));

typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(retcheckBypass(x(0x11E06C0)));

typedef double(__cdecl* rtonumber)(DWORD, int, int);
rtonumber r_lua_tonumber = (rtonumber)(x(0x11E1B90));

typedef void(__stdcall* rpushcclosure)(DWORD rL, int fn, int non, int a1, int xd);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(retcheckBypass(x(0x11E0940)));

typedef void(__cdecl* rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(retcheckBypass(x(0x11DFE40)));

typedef DWORD(__cdecl* rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)retcheckBypass(x(0x11E05B0));

typedef void* (__cdecl* rnewuserdata)(DWORD, size_t, int);
rnewuserdata r_lua_newuserdata = (rnewuserdata)(retcheckBypass(x(0x11E0640)));

typedef void(__cdecl* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)retcheckBypass(x(0x11E1090));

typedef void* (__fastcall* rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(retcheckBypass(x(0x11E0130)));

typedef void* (__cdecl* rsetmetatable)(DWORD rL, int idx);
rsetmetatable r_lua_setmetatable = (rsetmetatable)(retcheckBypass(x(0x11E15D0)));

typedef int(__cdecl* rtouserdata)(DWORD, int);
rtouserdata r_lua_touserdata = (rtouserdata)(retcheckBypass(x(0x11E1E60)));

typedef DWORD(__cdecl* rtype)(DWORD, int);
rtype r_lua_type = (rtype)(x(0x11E1F50));

typedef void* (__cdecl* rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(retcheckBypass(x(0x11E17C0)));

typedef DWORD(__cdecl* rref)(DWORD, DWORD);
rref r_luaL_ref = (rref)(retcheckBypass(x(0x11E2850)));

int r_lua_gettop(int RobloxState)
{
	{ return *(DWORD*)(RobloxState + top_off) - *(DWORD*)(RobloxState + base_off) >> 4; }
}

typedef void(__cdecl* rawseti)(DWORD, DWORD, DWORD);
rawseti r_lua_rawseti = (rawseti)(retcheckBypass(x(0x11E11E0)));

typedef void(__stdcall* rsettop)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)(retcheckBypass(x(0x11E1840)));

typedef void(__cdecl* pushnil)(DWORD);
pushnil r_lua_pushnil = (pushnil)(retcheckBypass(x(0x11E0BA0)));

typedef void(__cdecl* rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)(retcheckBypass(x(0x11E0AE0)));

typedef int(__thiscall* findfirstchild)(DWORD, DWORD, const std::string&);
findfirstchild FindFirstChild = (findfirstchild)x(OFF_FFC);

union r_lua_Value {
	int b;
	double n;
	void* p;
	void* gc;
};

struct r_lua_TValue {
	r_lua_Value value;
	int tt;
};

static TValue* lua_index2adr(lua_State* L, int idx) {
	if (idx > 0) {
		TValue* o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue*, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure* func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure* func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue*, luaO_nilobject);
	}
	}
}

