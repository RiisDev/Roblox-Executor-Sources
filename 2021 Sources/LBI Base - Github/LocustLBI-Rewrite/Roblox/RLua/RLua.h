#pragma once
#include <Windows.h>
#include "../Bypasses/Retcheck.h"
#include "../Bypasses/Callcheck.h"
#include "Addresses.h"

extern "C" {
#include "../../Libraries/Lua/lua.h"
#include "../../Libraries/Lua/lauxlib.h"
#include "../../Libraries/Lua/lualib.h"
#include "../../Libraries/Lua/lopcodes.h"
#include "../../Libraries/Lua/lobject.h"
#include "../../Libraries/Lua/ldo.h"
#include "../../Libraries/Lua/lstate.h"
}

using namespace Adresses;

int aslr(int x) { return(x - 0x400000 + reinterpret_cast<int>(GetModuleHandleA(0))); }
DWORD rL;

using def_getdatamodel = void(__thiscall*)(DWORD, DWORD); // these calling conventions usually stay the same
using def_getdatamodel2 = DWORD(__cdecl*)(); // these calling conventions usually stay the same

typedef int(luau_deserialize_CCV* def_deserialize)(DWORD, const char*, const char*, int, int);
typedef int(spawn_CCV* def_spawn)(DWORD);
typedef DWORD(lua_newthread_CCV* def_newthread)(DWORD);
typedef int(lua_getfield_CCV* def_getfield)(DWORD, int, const char*);
typedef int(lua_setfield_CCV* def_setfield)(DWORD, int, const char*);
typedef void(lua_pushlstring_CCV* def_pushlstring)(DWORD, const char*, size_t);
typedef int(lua_pushvalue_CCV* def_pushvalue)(DWORD, int);
typedef int(lua_pushcclosure_CCV* def_pushcclosure)(DWORD, int, int, int, int);
typedef int(lua_pcall_CCV* def_pcall)(DWORD, int, int, int);
typedef const char* (lual_checklstring_CCV* def_checklstring)(DWORD, int, size_t*);
typedef int(lua_getmetatable_CCV* def_getmetatable)(DWORD, int);
typedef int(lua_setmetatable_CCV* def_setmetatable)(DWORD, int);

def_deserialize r_deserialize = reinterpret_cast<def_deserialize>(aslr(luau_deserialize_addr));
def_spawn r_spawn = reinterpret_cast<def_spawn>(aslr(spawn_addr));
def_newthread r_newthread = reinterpret_cast<def_newthread>(unprotect(aslr(lua_newthread_addr)));
def_getfield r_getfield = reinterpret_cast<def_getfield>(unprotect(aslr(lua_getfield_addr)));
def_setfield r_setfield = reinterpret_cast<def_setfield>(unprotect(aslr(lua_setfield_addr)));
def_pushlstring r_pushlstring = reinterpret_cast<def_pushlstring>(unprotect(aslr(lua_pushlstring_addr)));
def_pushvalue r_pushvalue = reinterpret_cast<def_pushvalue>(unprotect(aslr(lua_pushvalue_addr)));
def_pushcclosure r_pushcclosure = reinterpret_cast<def_pushcclosure>(unprotect(aslr(lua_pushcclosure_addr)));
def_pcall r_pcall = reinterpret_cast<def_pcall>(unprotect(aslr(lua_pcall_addr)));
def_checklstring r_checklstring = reinterpret_cast<def_checklstring>(unprotect(aslr(lual_checklstring_addr)));
def_getmetatable r_getmetatable = reinterpret_cast<def_getmetatable>(unprotect(aslr(lua_getmetatable_addr)));
def_setmetatable r_setmetatable = reinterpret_cast<def_setmetatable>(unprotect(aslr(lua_setmetatable_addr)));

void registerfunc(DWORD rL, int funcaddr, const char* name, int upvalues = 0)
{
	r_pushcclosure(rL, ReturnSafeAddr(funcaddr), 0, upvalues, 0);
	r_setfield(rL, -10002, name);
}

DWORD GetState() // Datamodel shit that isn't mine
{
	DWORD ScriptContext;
	DWORD DMPad[8];
	DWORD gdm_addr = unprotect(aslr(getdatamodel_addr));
	DWORD gdm2_addr = unprotect(aslr(getdatamodel2_addr));
	reinterpret_cast<def_getdatamodel>(gdm_addr)(reinterpret_cast<def_getdatamodel2>(gdm2_addr)(), (DWORD)DMPad);
	DWORD startofc = *(DWORD*)(DMPad[0] + 0xC + 44);
	DWORD endofc = *(DWORD*)(startofc + 4);
	for (int i = *(int*)startofc; i != endofc; i += 8)
	{
		if (memcmp((const char*)(*(int(**)(void))(*(int*)*(int*)i + 16))(), "ScriptContext", strlen("ScriptContext")) == 0)
		{
			ScriptContext = *(int*)i;
		}
	}

	return RBX_LuaState(ScriptContext);
}

void SetLevel(DWORD rL, int identity) { *reinterpret_cast<DWORD*>(*reinterpret_cast<DWORD*>(rL + Identity2) + Identity1) = identity; }