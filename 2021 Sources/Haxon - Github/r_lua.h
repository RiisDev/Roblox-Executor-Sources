#include <Windows.h> 
#include "Bridge.h"
#include<cstdint> 
/*

  _    _           _       _              _               _
 | |  | |         | |     | |            | |             | |
 | |  | |_ __   __| | __ _| |_ ___   _ __| |_   _  __ _  | |__
 | |  | | '_ \ / _` |/ _` | __/ _ \ | '__| | | | |/ _` | | '_ \
 | |__| | |_) | (_| | (_| | ||  __/ | |  | | |_| | (_| |_| | | |
  \____/| .__/ \__,_|\__,_|\__\___| |_|  |_|\__,_|\__,_(_)_| |_|
		| |                          ______
		|_|                         |______|
  Welcome to r_lua.h
  To get started, update the 0x0 and the __fastcall.
  Remember not all are fastcall.

  And Update EVERYTHING.
*/

#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))( (x(address)))
#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)
#define x(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))

int GDM;

typedef DWORD(__cdecl* gdm2)();
gdm2 getdatamodel2 = (gdm2)(x(0x00F9D7E0));

typedef DWORD(__thiscall* getdatamodel)(DWORD, DWORD);
getdatamodel r_getdatamodel = (getdatamodel)(x(0x005EBDE0));

typedef int(__cdecl* ee)(int e, const char* n, ...);
static ee r_luaL_error = (ee)x(0x0138E0C0);

typedef void(__cdecl* rgetfield)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)(x(0x0138BBA0));

typedef int(__cdecl* rboolean)(unsigned int, int);
rboolean r_lua_pushboolean = (rboolean)((x(0x0138C620)));

typedef char* (__fastcall* rtolstring)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)((x(0x0138D650)));

typedef bool(__cdecl* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(x(0x0138D570));

typedef void(__fastcall* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)((x(0x0138C950)));

typedef double(__stdcall* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)((x(0x0138C7A0)));

typedef void(__fastcall* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)((x(0x0138C7A0)));

typedef int(__cdecl* rLua_pcall)(DWORD lst, int nargs, int nresults, int errfunc);
rLua_pcall r_lua_pcall = (rLua_pcall)(x(0x0138C360));

typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)((x(0x0138C1F0)));

typedef double(__cdecl* rtonumber)(DWORD, int, int);
rtonumber r_lua_tonumber = (rtonumber)(x(0x0138D780));

typedef void(__fastcall* rpushcclosure)(DWORD rL, int fn, int non, int a1, int xd);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)((x(0x0138C470)));

typedef void(__cdecl* rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)((x(0x0138B950)));

typedef DWORD(__cdecl* rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)(x(0x0138C170));

typedef void* (__cdecl* rnewuserdata)(DWORD, size_t, int);
rnewuserdata r_lua_newuserdata = (rnewuserdata)((x(0x0138C1F0)));

typedef void(__cdecl* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)(x(0x0138CCE0));

typedef void(__cdecl* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawseti = (rrawgeti)(x(0x0138CDB0));

typedef void* (__stdcall* rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)((x(0x0138BD10)));

typedef int (__cdecl* rsetmetatable)(DWORD rL, int idx);
rsetmetatable r_lua_setmetatable = (rsetmetatable)((x(0x0138D1C0)));

typedef int(__cdecl* rtouserdata)(DWORD, int);
rtouserdata r_lua_touserdata = (rtouserdata)((x(0x0138DA50)));

//typedef void(__fastcall* fireclick)(DWORD, float, DWORD);
//fireclick FireClick = (fireclick)x(0x0); // Ew who uses this in Axon Lmao.

typedef DWORD(__cdecl* rtype)(DWORD, int);
rtype r_lua_type = (rtype)(x(0x0138DB40));

typedef void* (__cdecl* rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)((x(0x0138D3B0)));

typedef DWORD(__cdecl* rref)(DWORD, DWORD);
rref r_luaL_ref = (rref)((x(0x0138E450)));

int r_lua_gettop(int rL)
{
	DWORD TOP = 28;
	DWORD BASE = 28;
	return *(DWORD*)(rL + TOP) - *(DWORD*)(rL + BASE) >> 4;
}

typedef void(__fastcall* rsettop)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)((x(0x0138D430)));

typedef void(__cdecl* pushnil)(DWORD);
pushnil r_lua_pushnil = (pushnil)((x(0x0138C740)));

typedef void(__cdecl* rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)((x(0x0138C670)));

//typedef bool(__fastcall* FLog)(const std::string& name, std::string& value, bool alsoCheckUnknown); \\
FLog GetValue = reinterpret_cast<FLog>(x(0x0));

//typedef bool(__fastcall* SLog)(const std::string& name, const std::string& value, int Type, bool loadedFromServer); \\
SLog SetValue = reinterpret_cast<SLog>(x(0x0));

typedef uint32_t(__fastcall* T_index2adr)(uint32_t, uint32_t);
T_index2adr r_index2adr;

namespace AxonWonderful
{
	// Update RBX_LuaState.
	DWORD RBX_LuaState(DWORD ScriptContext) { return  *(DWORD*)(ScriptContext + 180) ^ (ScriptContext + 180); }
}