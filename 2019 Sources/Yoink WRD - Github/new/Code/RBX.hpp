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
//Data Types
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

struct Userdata
{
	int32_t reference;
};

DWORD RLS;
lua_State* LS;
int top = 24, base = 28;
std::vector<int> BreakPointShit;

#define Rebase(OFFSET) (OFFSET - 0x400000 + (DWORD)GetModuleHandle(NULL))
#define Level(rL, i) DWORD* identity  = (DWORD*)(rL - 40); *identity ^= (i ^ (unsigned __int8)*identity) & 0x1F;

DWORD Address_GetField = 0x853770;//1
#define Convention_GetField __stdcall//Updated

DWORD Address_Tolstring = 0x8561f0;//2
#define Convention_TolString __cdecl//Updated

DWORD Address_Toboolean = 0x8560E0;//3
#define Convention_ToBoolean __cdecl//Updated

DWORD Address_PushValue = 0x854EA0;//4
#define Convention_PushValue __cdecl//Updated

DWORD Address_PushNumber = 0x854ca0;//5
#define Convention_PushNumber __cdecl

DWORD Address_PushString = 0x854D30;//6
#define Convention_PushString __cdecl 

DWORD Address_Pcall = 0x854740;//7
#define Convention_Pcall __cdecl

DWORD Address_Next = 0x854580;//8
#define Convention_Next __cdecl

DWORD Address_ToNumber = 0x856360;//9
#define Convention_ToNumber __cdecl

DWORD Address_PushCClosure = 0x8548a0;//10
#define Convention_PushCClosure __fastcall

DWORD Address_CreateTable = 0x853400;//11
#define Convention_CreateTable __cdecl

DWORD Address_NewThread = 0x8543e0;//12
#define Convention_NewThread __cdecl

DWORD Address_NewUserData = 0x8544d0;//13
#define Convention_NewUserData __cdecl

DWORD Address_RawGeti = 0x855200;//14
#define Convention_RawGeti __cdecl

DWORD Address_GetMetaTable = 0x853B20;//15
#define Convention_GetMetaTable __fastcall 

DWORD Address_SetMetaTable = 0x855C20;//16
#define Convention_SetMetaTable __cdecl

DWORD Address_ToUserData = 0x8565E0;//17
#define Convention_ToUserData __cdecl

DWORD Address_Type = 0x856680;//18
#define Convention_Type __cdecl

DWORD Address_Settable = 0x855E90;//19
#define Convention_Settable __cdecl

DWORD Address_Index2Adr = 0x84a420;//20
#define Convention_Index2Adr __stdcall

DWORD Address_Ref = 0x84e2b0;//21
#define Convention_Ref __cdecl

DWORD Address_Gettop = 0x853d80; //23  //This is used for the STABLE gettop hook!
#define Convention_Gettop __cdecl

DWORD Address_SetReadOnly = 0x855D70;//24
#define Convention_SetReadOnly __cdecl



typedef void(Convention_GetField* rgetfield)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)unprotect(Rebase(Address_GetField));
typedef char* (Convention_TolString* rtolstring)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(Rebase(Address_Tolstring)));
typedef bool(Convention_ToBoolean* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(Rebase(Address_Toboolean));
typedef void(Convention_PushValue* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(Rebase(Address_PushValue)));
typedef double(Convention_PushNumber* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(Rebase(Address_PushNumber)));
typedef void(Convention_PushString* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(Rebase(Address_PushString));
auto r_luad_pcall = (int(Convention_Pcall*)(DWORD, int, int, int))unprotect(Rebase(Address_Pcall));
typedef DWORD(Convention_Next* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(unprotect(Rebase(Address_Next)));
double r_lua_tonumber(DWORD rL, int idx)
{
	return ((double(Convention_ToNumber*)(DWORD, int, int))Rebase(Address_ToNumber))(rL, idx, 0);
}
typedef void(Convention_PushCClosure* rpushcclosure)(DWORD rL, int fn, int non);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(unprotect(Rebase(Address_PushCClosure)));
typedef void(Convention_CreateTable* rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(Rebase(Address_CreateTable)));
typedef DWORD(Convention_NewThread* rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)unprotect(Rebase(Address_NewThread));
void* r_lua_newuserdata(DWORD rL, size_t size)
{
	return ((void* (Convention_NewUserData*)(DWORD rL, size_t size, int))unprotect(Rebase(Address_NewUserData)))(rL, size, 0);
}
typedef void(Convention_RawGeti* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(Rebase(Address_RawGeti));

typedef void* (Convention_GetMetaTable* rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(Rebase(Address_GetMetaTable)));
auto r_lua_touserdata = (int(__cdecl*)(DWORD, int))Rebase(Address_ToUserData);
typedef DWORD(Convention_Type* rtype)(DWORD, int);
rtype r_lua_type = (rtype)(Rebase(Address_Type));
typedef void* (Convention_Settable* rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(Rebase(Address_Settable)));
typedef int(Convention_Gettop* gettop)(DWORD);
gettop r_lua_gettop = (gettop)(Rebase(Address_Gettop));
typedef r_lua_TValue* (Convention_Index2Adr* rrindex2)(int lst, int idx);
rrindex2 r_lua_index2 = (rrindex2)Rebase(Address_Index2Adr);
typedef DWORD(Convention_Ref* rref)(DWORD, DWORD);
rref r_luaL_ref = (rref)(unprotect((byte*)(Rebase(Address_Ref))));
typedef void* (Convention_SetReadOnly* rrsetreadonly)(DWORD RLS, int idx, int k);
rrsetreadonly r_lua_setreadonly = (rrsetreadonly)(unprotect(Rebase(Address_SetReadOnly)));
typedef int(Convention_ToUserData* touserrrdata)(DWORD, int);
touserrrdata rlua_touserdata = (touserrrdata)unprotect(Rebase(Address_ToUserData));
typedef void* (Convention_ToUserData* rrtouser)(DWORD RLS, int idx);
rrtouser r_luax_touserdata = (rrtouser)unprotect(Rebase(Address_ToUserData));

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

//r_TValue* rlua_index2adr(unsigned int rL, int idx)
//{
//	return ((r_TValue*(__fastcall*)(unsigned int, int))(Rebase(0x844210)))(rL, idx);
//}

typedef int(Convention_SetMetaTable* rrrsetmeta)(DWORD RLS, int index);//
rrrsetmeta r_lua_setmetatable = (rrrsetmeta)(unprotect(Rebase(Address_SetMetaTable)));


//Lua Macro

#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)


//Inlined Functions
int r_lua_pcall(DWORD rL, int nargs, int nresults, int errfunc)
{
	int ret = r_luad_pcall(rL, nargs, nresults, errfunc);
	return ret;
}

void r_lua_settop(int a1, int a2) {
	int i;
	if (a2 < 0)
	{
		*(_DWORD*)(a1 + top) += 16 * a2 + 16;
	}
	else
	{
		for (i = 16 * a2; *(_DWORD*)(a1 + top) < (unsigned int)(i + *(_DWORD*)(a1 + base)); *(_DWORD*)(a1 + top) += 16)
			* (_DWORD*)(*(_DWORD*)(a1 + top) + 8) = 0;
		*(_DWORD*)(a1 + top) = i + *(_DWORD*)(a1 + base);
	}
}

void r_lua_emptystack(DWORD LS)
{
	r_lua_settop(LS, 0);
};

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

void r_lua_pushlightuserdata(DWORD a1, void* a2)
{
	auto& toxp = *(TValue * *)(a1 + top);
	toxp->tt = R_LUA_TLIGHTUSERDATA;
	toxp->value.p = a2;
	toxp++;
}

void r_lua_setfield(DWORD L, int idx, const char* k) {
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
