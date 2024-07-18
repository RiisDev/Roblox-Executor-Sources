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
#define R_LUA_TLIGHTUSERDATA 3
#define R_LUA_TNUMBER 2
#define R_LUA_TBOOLEAN 1
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 5
#define R_LUA_TFUNCTION 8
#define R_LUA_TTABLE 7
#define R_LUA_TUSERDATA 6
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
int top = 28, base = 16;
std::vector<int> BreakPointShit;

#define Rebase(OFFSET) (OFFSET - 0x400000 + (DWORD)GetModuleHandle(NULL))
#define Level(rL, i) DWORD* identity  = (DWORD*)(rL - 40); *identity ^= (i ^ (unsigned __int8)*identity) & 0x1F;

DWORD Address_GetField = 0x765D40;//1
#define Convention_GetField __stdcall //Updated

DWORD Address_Tolstring = 0x7685F0;//2
#define Convention_TolString __cdecl   //Updated

DWORD Address_Toboolean = 0x768530;//3
#define Convention_ToBoolean __cdecl//Updated

DWORD Address_PushValue = 0x7674D0;//4
#define Convention_PushValue __fastcall  //Updated

//typedef int(__cdecl* AEONIANX_LUA_GETMETAFIELD)(DWORD RLS, int obj, const char* e);
//AEONIANX_LUA_GETMETAFIELD r_luaL_getmetafield = (AEONIANX_LUA_GETMETAFIELD)unprotect((byte*)(Rebase(0x84bd10)));

DWORD Address_PushNumber = 0x7672d0;//5
#define Convention_PushNumber __stdcall  

DWORD Address_PushString = 0x767360;//6
#define Convention_PushString __cdecl    

DWORD Address_Pcall = 0x766D20;//7
#define Convention_Pcall __cdecl

DWORD Address_Next = 0x766A80;//8
#define Convention_Next __cdecl

DWORD Address_ToNumber = 0x768720;//9
#define Convention_ToNumber __cdecl

DWORD Address_CreateTable = 0x765A30;//11
#define Convention_CreateTable __cdecl

DWORD Address_NewThread = 0x766820;//12
#define Convention_NewThread __cdecl

DWORD Address_NewUserData = 0x766960;//13
#define Convention_NewUserData __cdecl

DWORD Address_RawGeti = 0x767790;//14
#define Convention_RawGeti __cdecl

DWORD Address_GetMetaTable = 0x7661E0;//15
#define Convention_GetMetaTable __fastcall   

DWORD Address_SetMetaTable = 0x7680F0;//16
#define Convention_SetMetaTable __cdecl

DWORD Address_ToUserData = 0x768860;//17
#define Convention_ToUserData __cdecl

DWORD Address_Type = 0x7688C0;//18
#define Convention_Type __cdecl

DWORD Address_Settable = 0x768320;//19
#define Convention_Settable __cdecl

DWORD Address_Index2Adr = 0x75E340;//20
#define Convention_Index2Adr __cdecl  

DWORD Address_Ref = 0x760E70;//21
#define Convention_Ref __cdecl

DWORD Address_Gettop = 0x766410; //23  //This is used for the STABLE gettop hook!
#define Convention_Gettop __cdecl

int Error = 3;
typedef int(__cdecl* rprint)(int a1, const char* a2);
rprint rlua_print = (rprint)Rebase(0x5A36C0);

DWORD Address_SetReadOnly = 0x768220;//24
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
using _DWORD = DWORD;
using _BYTE = BYTE;

using unknown0 = int(__cdecl*)(int);
unknown0 unknown = reinterpret_cast<unknown0>(unprotect(Rebase(0x75EBD0)));

using unknown01 = void(__cdecl*)(int, int);
unknown01 unknown1 = reinterpret_cast<unknown01>(unprotect(Rebase(0x75F7A0)));

int r_lua_pushcclosure(int a1, int a2, int a3)
{
	std::cout << "retarded fix\n";
	int v3; // esi
	_DWORD* v4; // edi
	int v5; // eax
	int v6; // eax
	int v7; // ebx
	int v8; // eax
	_DWORD* v9; // eax
	int v10; // edx
	int v11; // ecx
	char v12; // al
	_DWORD* v13; // eax
	int v14; // edi
	int v15; // edx
	int v16; // eax
	signed int result; // eax
	int v18; // [esp+Ch] [ebp-Ch]
	int v19; // [esp+10h] [ebp-8h]
	int v20; // [esp+14h] [ebp-4h]
	char* retaddr; // [esp+1Ch] [ebp+4h]
	int v22; // [esp+20h] [ebp+8h]
	int v23; // [esp+28h] [ebp+10h]

	v3 = a1;
	v4 = (_DWORD*)(a1 + 32);
	if (*(_DWORD*)(*(_DWORD*)(a1 + 32) - a1 + 44) >= *(_DWORD*)(*(_DWORD*)(a1 + 32) - (a1 + 32) + 80))
		unknown(a1);
	v5 = *(_DWORD*)(a1 + 12);
	if (v5 == *(_DWORD*)(a1 + 48))
		v6 = *(_DWORD*)(a1 + 72);
	else
		v6 = *(_DWORD*)(**(_DWORD**)(v5 + 12) + 16);
	v7 = a3;
	v22 = v6;
	v8 = 16 * a3;
	v23 = v8;
	v18 = v8 + 24;
	v19 = *v4 - (_DWORD)v4;
	v9 = (_DWORD*)(*(int(__cdecl**)(int, _DWORD, _DWORD, _DWORD, int, signed int))(v19 + 16))(
		v3,
		*(_DWORD*)(v19 + 20),
		0,
		0,
		v8 + 24,
		3);
	v10 = (int)v9;
	v20 = (int)v9;
	if (!v9)
		unknown1(v3, 4);
	*(_DWORD*)(v19 + 76) += v18;
	v11 = *v4 - (_DWORD)v4;
	*v9 = *(_DWORD*)(v11 + 44);
	v12 = *(_BYTE*)(v11 + 24);
	*(_DWORD*)(v11 + 44) = v10;
	*(_BYTE*)(v10 + 5) = v12 & 3;
	*(_DWORD*)(v10 + 16) = v22;
	*(_BYTE*)(v10 + 4) = 8;
	*(_BYTE*)(v10 + 6) = 1;
	*(_BYTE*)(v10 + 7) = v7;
	*(_BYTE*)(v10 + 8) = 20;
	*(_DWORD*)(v10 + 20) = v10 + a2 + 20;
	*(_DWORD*)(v3 + 28) -= v23;
	if (v7)
	{
		v13 = (_DWORD*)(v10 + v23 + 24);
		v14 = -24 - v10;
		do
		{
			std::cout << "upval\n";
			v13 -= 4;
			v15 = v14 + *(_DWORD*)(v3 + 28);
			*v13 = *(_DWORD*)((char*)v13 + v15);
			v13[1] = *(_DWORD*)((char*)v13 + v15 + 4);
			v13[2] = *(_DWORD*)((char*)v13 + v15 + 8);
			--v7;
		} while (v7);
	}
	v16 = *(_DWORD*)(v3 + 28);
	*(_DWORD*)v16 = v20;
	*(_DWORD*)(v16 + 8) = 8;
	*(_DWORD*)(v3 + 28) += 16;
	return 1;
}
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
