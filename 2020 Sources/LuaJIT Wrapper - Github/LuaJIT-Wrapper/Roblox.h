#pragma once
#include <Windows.h>
#include "Retcheck.h"
#include <string>
#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)
#define x(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))

#define R_LUA_TNIL 0
#define R_LUA_TBOOLEAN 1
#define R_LUA_VECTOR 2
#define R_LUA_TLIGHTUSERDATA 4
#define R_LUA_TNUMBER 3
#define R_LUA_TSTRING 5
#define R_LUA_TUSERDATA 6
#define R_LUA_TFUNCTION 9
#define R_LUA_TTHREAD 8
#define R_LUA_TTABLE 7
#define R_LUA_TPROTO 10
#define R_LUA_TUPVALUE 11

#include "Lua/lua.hpp"

DWORD DataModel;

DWORD TOP = 12;
DWORD BASE = 24;

typedef DWORD(__cdecl* getdatamodel2ok)();
getdatamodel2ok   getdatamodel2 = (getdatamodel2ok)(x(0xE790D0));

typedef DWORD(__thiscall* getdatamodelok)(DWORD, DWORD);
getdatamodelok   r_getdatamodel = (getdatamodelok)(x(0xE79220));

typedef int(__cdecl* printok)(int, const char*);
printok   print = (printok)(x(0x60AAA0));

typedef int(__cdecl* luaL_errorok)(int e, const char* n, ...);
luaL_errorok   r_luaL_error = (luaL_errorok)retcheckBypass((x(0x12216A0)));

typedef void(__fastcall* lua_getfieldok)(DWORD RobloxState, int idx, const char* k);
lua_getfieldok   r_lua_getfield = (lua_getfieldok)retcheckBypass((x(0x121F2D0)));

typedef int(__cdecl* lua_pushbooleanok)(unsigned int, int);
lua_pushbooleanok   r_lua_pushboolean = (lua_pushbooleanok)retcheckBypass((x(0x121FB00)));

typedef char* (__cdecl* lua_tolstringok)(DWORD RobloxState, int idx, size_t* size);
lua_tolstringok   r_lua_tolstring = (lua_tolstringok)retcheckBypass((x(0x1220C60)));

typedef bool(__cdecl* lua_tobooleanok)(DWORD RobloxState, bool idx);
lua_tobooleanok   r_lua_toboolean = (lua_tobooleanok)retcheckBypass((x(0x1220B90)));

typedef void(__stdcall* lua_pushvalueok)(DWORD RobloxState, DWORD idx);
lua_pushvalueok   r_lua_pushvalue = (lua_pushvalueok)retcheckBypass((x(0x1220040)));

typedef double(__cdecl* lua_pushnumberok)(DWORD RobloxState, double idx);
lua_pushnumberok   r_lua_pushnumber = (lua_pushnumberok)retcheckBypass((x(0x121FE20)));

typedef void(__stdcall* lua_pushstringok)(DWORD RobloxState, const char*);
lua_pushstringok   r_lua_pushstring = (lua_pushstringok)retcheckBypass((x(0x121FE80)));

typedef DWORD(__cdecl* lua_nextok)(DWORD RobloxState, int idx);
lua_nextok   r_lua_next = (lua_nextok)retcheckBypass((x(0x121F8E0)));

typedef double(__cdecl* lua_tonumberok)(DWORD ok, int idx, int bruh);
lua_tonumberok   r_lua_tonumber = (lua_tonumberok)retcheckBypass((x(0x1220D90)));

typedef int(__stdcall* lua_pushcclosureok)(DWORD RobloxState, int fn, int non, int a1, int xd);
lua_pushcclosureok   r_lua_pushcclosure = (lua_pushcclosureok)retcheckBypass((x(0x121FB50)));

typedef void(__cdecl* lua_createtableok)(DWORD RobloxState, int num, int fix);
lua_createtableok   r_lua_createtable = (lua_createtableok)retcheckBypass((x(0x121F080)));

typedef DWORD(__cdecl* lua_newthreadok)(DWORD ok);
lua_newthreadok   r_lua_newthread = (lua_newthreadok)retcheckBypass((x(0x121F7D0)));

typedef void* (__cdecl* lua_newuserdataok)(DWORD, size_t, int);
lua_newuserdataok   r_lua_newuserdata = (lua_newuserdataok)retcheckBypass((x(0x121F860)));

typedef void(__cdecl* lua_rawgetiok)(DWORD, DWORD, DWORD);
lua_rawgetiok   r_lua_rawgeti = (lua_rawgetiok)retcheckBypass((x(0x12202C0)));

typedef void* (__cdecl* lua_getmetatableok)(DWORD RobloxState, int idx);
lua_getmetatableok   r_lua_getmetatable = (lua_getmetatableok)retcheckBypass((x(0x121F370)));

typedef int(__cdecl* lua_touserdataok)(DWORD, int);
lua_touserdataok   r_lua_touserdata = (lua_touserdataok)retcheckBypass((x(0x1221020)));

typedef DWORD(__cdecl* lua_tothreadok)(DWORD, DWORD);
lua_tothreadok   r_lua_tothread = (lua_tothreadok)retcheckBypass((x(0x1220F50)));

typedef void(__cdecl* lua_rawsetok)(DWORD, DWORD, DWORD);
lua_rawsetok   r_lua_rawset = (lua_rawsetok)retcheckBypass((x(0x1220340)));

typedef void(__cdecl* lua_rawgetok)(DWORD, DWORD);
lua_rawgetok   r_lua_rawget = (lua_rawgetok)retcheckBypass((x(0x12201A0)));

typedef void(__cdecl* lua_callok)(DWORD, DWORD, DWORD);
lua_callok   r_lua_call = (lua_callok)retcheckBypass((x(0x121EEC0)));

typedef void(__cdecl* lua_removeok)(DWORD, DWORD);
lua_removeok   r_lua_remove = (lua_removeok)retcheckBypass((x(0x12204E0)));

typedef const void* (__cdecl* lua_topointerok)(DWORD, DWORD);
lua_topointerok   r_lua_topointer = (lua_topointerok)retcheckBypass((x(0x1220E10)));

typedef bool(__cdecl* lua_iscfunctionok)(DWORD, DWORD);
lua_iscfunctionok   r_lua_iscfunction = (lua_iscfunctionok)retcheckBypass((x(0x121F620)));

typedef DWORD(__cdecl* lua_typeok)(DWORD, int);
lua_typeok   r_lua_type = (lua_typeok)retcheckBypass((x(0x1221110)));

typedef void* (__cdecl* lua_settableok)(DWORD RobloxState, int);
lua_settableok   r_lua_settable = (lua_settableok)retcheckBypass((x(0x12209E0)));

typedef void(__cdecl* luaL_unrefok)(DWORD, DWORD, int);
luaL_unrefok   r_luaL_unref = (luaL_unrefok)retcheckBypass((x(0x1221C90)));

typedef void(__cdecl* lua_xmoveok)(DWORD, int, int);
lua_xmoveok   r_lua_xmove = (lua_xmoveok)retcheckBypass((x(0x12211C0)));

typedef DWORD(__cdecl* lua_pushthreadok)(DWORD);
lua_pushthreadok   r_lua_pushthread = (lua_pushthreadok)retcheckBypass((x(0x121FF60)));

typedef void(__cdecl* lua_gettableok)(DWORD, int);
lua_gettableok   r_lua_gettable = (lua_gettableok)retcheckBypass((x(0x121F430)));

typedef void(__cdecl* lua_setfieldok)(DWORD, signed int, const char*);
lua_setfieldok   r_lua_setfield = (lua_setfieldok)retcheckBypass((x(0x1220740)));

typedef int(__cdecl* lua_setmetatableok)(DWORD, int);
lua_setmetatableok   r_lua_setmetatable = (lua_setmetatableok)retcheckBypass((x(0x12207F0)));

typedef void(__cdecl* lua_setreadonlyok)(DWORD, int, int);
lua_setreadonlyok   r_lua_setreadonly = (lua_setreadonlyok)retcheckBypass((x(0x1220900)));

typedef DWORD(__cdecl* luaL_refok)(DWORD, DWORD);
luaL_refok   r_luaL_ref = (luaL_refok)retcheckBypass((x(0x1221A40)));

typedef int(__cdecl* lua_pcallok)(DWORD, int, int, int);
lua_pcallok   r_lua_pcall = (lua_pcallok)retcheckBypass((x(0x121FA40)));

typedef void(__cdecl* lua_rawsetiok)(DWORD, DWORD, DWORD);
lua_rawsetiok   r_lua_rawseti = (lua_rawsetiok)retcheckBypass((x(0x12204102)));


int r_lua_gettop(int RobloxState) { return *(DWORD*)(RobloxState + TOP) - *(DWORD*)(RobloxState + BASE) >> 4; }

typedef void(__fastcall* lua_settopok)(DWORD RobloxState, int idx);
lua_settopok   r_lua_settop = (lua_settopok)retcheckBypass((x(0x1220A60)));

typedef void(__cdecl* lua_pushnilok)(DWORD ok);
lua_pushnilok   r_lua_pushnil = (lua_pushnilok)retcheckBypass((x(0x121FDD0)));

typedef void(__cdecl* lua_pushlightuserdataok)(DWORD, void*);
lua_pushlightuserdataok   r_lua_pushlightuserdata = (lua_pushlightuserdataok)retcheckBypass((x(0x121FD00)));

const char* GetClass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

int FindFirstClass(int Instance, const char* Name)
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 44);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		{			
			return *(int*)i;
		}
	}
}

int ReturnDataModel()
{
	static DWORD DMPad[4]{};
	r_getdatamodel(getdatamodel2(), (DWORD)DMPad);
	DWORD DM = DMPad[0];
	return DM + 12;
}

typedef void(__thiscall* fireclick)(DWORD, float, DWORD);
fireclick FireClick = (fireclick)x(0xBD7DF0);

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

typedef r_lua_TValue* (__stdcall* rrindex2)(int lst, int idx);
rrindex2 r_lua_index2 = (rrindex2)(x(0x121ED90));

/* synapse's instance xd */
DWORD instance_ptr;

#define SynException(x) (x)
#define ERR_INVALID_INST		1

#define DereferenceSmartPointerInstance(Ptr) (*(DWORD*) (Ptr))
#define BoundsCheckInstance() if (!instance_ptr) throw SynException(ERR_INVALID_INST)


std::string Synx_GetInstanceClassName(DWORD Inst)
{
	if (Inst == 0)
	{
		BoundsCheckInstance();
#ifdef _DEBUG
		return std::string(*(const char**)(*(DWORD*)(instance_ptr + 0xC) + 4));
#else
		return **(std::string**)(*(DWORD*)(instance_ptr + 0xC) + 4);
#endif
	}
#ifdef _DEBUG
	return std::string(*(const char**)(*(DWORD*)(Inst + 0xC) + 4));
#else
	return **(std::string**)(*(DWORD*)(Inst + 0xC) + 4);
#endif
}

/* my instance check implementation for FCD */
const char* GetInstanceClassName(int instancename) {

	if (instancename == 0)
	{
		BoundsCheckInstance();
		return (*(const char**)(*(DWORD*)(instance_ptr + 16) + 4));
	}
}

