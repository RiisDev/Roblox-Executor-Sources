#pragma once
#include <Windows.h>
#include "Retcheck.h"
#include "Addresses.h"

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lopcodes.h"
#include "Lua/lobject.h"
#include "Lua/ldo.h"
#include "Lua/lstate.h"
}

using namespace Addresses;

int aslr(int x) { return(x - 0x400000 + reinterpret_cast<int>(GetModuleHandleA(0))); }
DWORD rL;

using def_getdatamodel = void(__thiscall*)(DWORD, DWORD);
using def_getdatamodel2 = DWORD(__cdecl*)();

typedef int(luau_deserialize_CCV* def_deserialize)(DWORD, const char*, const char*, int, int);
typedef int(spawn_CCV* def_spawn)(DWORD);
typedef DWORD(lua_newthread_CCV* def_newthread)(DWORD);
typedef int(lua_getfield_CCV* def_getfield)(DWORD, int, const char*);
typedef int(lua_setfield_CCV* def_setfield)(DWORD, int, const char*);
typedef void(lua_pushlstring_CCV* def_pushlstring)(DWORD, const char*, size_t);
typedef int(lua_pushvalue_CCV* def_pushvalue)(DWORD, int);
typedef int(lua_pushcclosure_CCV* def_pushcclosure)(DWORD, int, int, int, int);
typedef int(lua_pcall_CCV* def_pcall)(DWORD, int, int, int);

def_deserialize r_deserialize = reinterpret_cast<def_deserialize>(aslr(luau_deserialize_addr));
def_spawn r_spawn = reinterpret_cast<def_spawn>(aslr(spawn_addr));
def_newthread r_newthread = reinterpret_cast<def_newthread>(unprotect(aslr(lua_newthread_addr)));
def_getfield r_getfield = reinterpret_cast<def_getfield>(unprotect(aslr(lua_getfield_addr)));
def_setfield r_setfield = reinterpret_cast<def_setfield>(unprotect(aslr(lua_setfield_addr)));
def_pushlstring r_pushlstring = reinterpret_cast<def_pushlstring>(unprotect(aslr(lua_pushlstring_addr)));
def_pushvalue r_pushvalue = reinterpret_cast<def_pushvalue>(unprotect(aslr(lua_pushvalue_addr)));
def_pushcclosure r_pushcclosure = reinterpret_cast<def_pushcclosure>(unprotect(aslr(lua_pushcclosure_addr)));
def_pcall r_pcall = reinterpret_cast<def_pcall>(unprotect(aslr(lua_pcall_addr)));

int ReturnDataModel()
{
	DWORD DMPad[8];
	DWORD GetDataModelAdr = unprotect(aslr(getdatamodel_addr));
	DWORD GetDataModelAdr2 = unprotect(aslr(getdatamodel2_addr));
	reinterpret_cast<def_getdatamodel>(GetDataModelAdr)(reinterpret_cast<def_getdatamodel2>(GetDataModelAdr2)(), (DWORD)DMPad);
	return DMPad[0] + 0xC;
}

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

int* GetLevel(int rL) { return reinterpret_cast<int*>(*reinterpret_cast<int*>(rL + Identity2) + Identity1); }
