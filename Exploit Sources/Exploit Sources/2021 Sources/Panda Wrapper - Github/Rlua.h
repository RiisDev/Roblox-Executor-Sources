#pragma once
#include "Scanner.h"
#include "RetCheck.h"
#include "Address.h"
extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\lapi.h"
#include "Lua\lualib.h"
#include "Lua\lobject.h"
#include "Lua\lstate.h"
#include "Lua\lgc.h"
#include "Lua\ldo.h"
}

DWORD rL;

typedef int (*rbx_CFunction)(int ls);

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

#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(unprotect(ScanAddress(address)))
#define DeclareX(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(ReturnCheck::UnProtect((byte*)ScanAddress(address)))
#define r_lua_gettop(rL) ((*(DWORD *)(rL + 12) - *(DWORD *)(rL + 24)) >> 4)
#define r_lua_pop(rL, n) rlua_settop(rL, -(n) - 1)
#define r_lua_getglobal(rL, k) rlua_getfield(rL, -10002, k)
#define r_lua_newtable(rL) rlua_createtable(rL, 0, 0)
#define r_lua_isnil(L,n) (rlua_type(L, (n)) == RLUA_TNIL)
#define RBX_TOP Adresses::ls_top
#define RBX_BASE Adresses::ls_base

#define TOP(L) (TValue*)*(DWORD*)(L + RBX_TOP)
#define _TOP(L) *(DWORD*)(L + RBX_TOP)
#define _BASE(L) *(DWORD*)(L + RBX_BASE)
#define incr_top(L) *(DWORD*)(L + RBX_TOP) += 16

//#define RLUA_TNIL 0
//#define RLUA_TLIGHTUSERDATA 4
//#define RLUA_TNUMBER 3
//#define RLUA_TBOOLEAN 1
//#define RLUA_TSTRING 5
//#define RLUA_TTHREAD 8
//#define RLUA_TFUNCTION 9
//#define RLUA_TTABLE 6
//#define RLUA_TUSERDATA 7

#define RLUA_TNONE   -1
#define RLUA_TUSERDATA   8
#define RLUA_TFUNCTION   6
#define RLUA_TSTRING   5
#define RLUA_TBOOLEAN   1
#define RLUA_TNUMBER   3
#define RLUA_TTABLE   9
#define RLUA_TNIL   0
#define RLUA_TTHREAD   7
#define RLUA_TVECTOR   4
#define RLUA_TLIGHTUSERDATA   2
#define RLUA_TPROTO   10
#define RLUA_TUPVALUE   11

typedef DWORD _DWORD;

int State(int v40, int v47) {
	return *(uintptr_t*)(v40 + 0xA4) - (v40 + 0xA4);
}

int int3breakpoint, int3breakpoint1, luaState, RobloxThread, handler;

DWORD int3faker;

union r_Value {
	PVOID gc;
	PVOID p;
	double n;
	int b;
};

struct r_TValue {
	r_Value value;
	int tt;
};

struct Userdata {
	int32_t reference;
	void* UD;
	int Key;
	int Key2;
};

typedef void(lua_getfield_CCV* rgetfield)(DWORD rL, int idx, const char* k);
rgetfield rlua_getfield = (rgetfield)unprotect(ScanAddress(Adresses::lua_getfield_addr)); //

typedef void(lua_setfield_CCV* rsetfield)(DWORD rL, int idx, const char* k);
rsetfield rlua_setfield = (rsetfield)unprotect(ScanAddress(Adresses::lua_setfield_addr)); //

typedef void(lua_pushcclosure_CCV* rpushcclosure)(DWORD rL, int fn, int non, int a3, int a);
rpushcclosure rlua_pushcclosure = (rpushcclosure)(unprotect(ScanAddress(Adresses::lua_pushcclosure_addr)));

typedef char* (lua_pushlstring_CCV* pushlstring)(int a1, const char* a2, int a3);
pushlstring rlua_pushlstring = (pushlstring)(ReturnCheck::UnProtect((byte*)(ScanAddress(Adresses::lua_pushlstring_addr))));

typedef int(lua_pcall_CCV* rrr)(DWORD a1, int a2, int a3, int a4);
rrr rlua_pcall = (rrr)(ReturnCheck::UnProtect((byte*)(ScanAddress(Adresses::lua_pcall_addr))));

typedef void* (lua_touserdata_CCV* touserdata)(DWORD, int);
touserdata rlua_touserdata = (touserdata)(ReturnCheck::UnProtect((byte*)(ScanAddress(Adresses::lua_touserdata_addr))));

typedef int(lua_touserdata_CCV* Lua_touserdata)(int rL, int idx);
Lua_touserdata rxlua_touserdata = (Lua_touserdata)(ReturnCheck::UnProtect((byte*)(ScanAddress(Adresses::lua_touserdata_addr)))); // this one idk seems fuck

typedef DWORD(lual_ref_CCV* rref)(DWORD, DWORD);
rref rluaL_ref = (rref)(ReturnCheck::UnProtect((BYTE*)(ScanAddress(Adresses::lual_ref_addr))));

typedef void(lual_unref_CCV* runref)(DWORD, DWORD, int);
runref rluaL_unref = (runref)(unprotect(ScanAddress(Adresses::lual_unref_addr)));

typedef DWORD(lua_next_CCV* next2)(DWORD rL, int idx);
next2 rlua_next = (next2)(ReturnCheck::UnProtect((BYTE*)(ScanAddress(Adresses::lua_next_addr))));

typedef int(lua_iscfunction_CCV* riscfunction)(int a1, int a2);
riscfunction rlua_iscfunction = (riscfunction)(unprotect(ScanAddress(Adresses::lua_iscfunction_addr)));

typedef int(spawn_CCV* spawnuwu)(int);
spawnuwu Spawn = (spawnuwu)unprotect(ScanAddress(Adresses::spawn_addr));

DWORD sandboxthreadoffset = ScanAddress(Adresses::sandboxthread_addr);
auto r_lua_sandboxthread = (void(sandboxthread_CCV*)(DWORD, int, int))unprotect(sandboxthreadoffset);

typedef int(sandboxthread_CCV* sandboxthread)(int, int*, int);
sandboxthread SandBoxThread = (sandboxthread)unprotect(ScanAddress(Adresses::sandboxthread_addr));

typedef void(lua_remove_CCV* rremove)(DWORD, DWORD);
rremove rlua_remove = (rremove)(unprotect(ScanAddress(Adresses::lua_remove_addr)));

auto rlua_index2adr = Declare(Adresses::index2adr_addr, TValue*, index2adr_CCV, DWORD a1, int idx);
auto rlua_gettable = Declare(Adresses::lua_gettable_addr, void, lua_gettable_CCV, int a1, int idx);
auto rlua_getmetatable = Declare(Adresses::lua_getmetatable_addr, int, lua_getmetatable_CCV, int a1, signed int a2);
auto rlua_createtable = Declare(Adresses::lua_createtable_addr, void, lua_createtable_CCV, int a1, int narray, int nrec);
auto rlua_toboolean = Declare(Adresses::lua_toboolean_addr, int, lua_toboolean_CCV, int a1, signed int a2);
auto rlua_tointeger = Declare(Adresses::lua_tointeger_addr, signed int, lua_tointeger_CCV, int a1, int a2, DWORD a3);
auto rlua_topointer = Declare(Adresses::lua_topointer_addr, int, lua_topointer_CCV, int a1, int a2);
auto rlua_pushlightuserdata = Declare(Adresses::lua_pushlightuserdata_addr, void, lua_pushlightuserdata_CCV, DWORD a1, void* p);
auto rlua_pushvalue = Declare(Adresses::lua_pushvalue_addr, int, lua_pushvalue_CCV, DWORD a1, int idx);
auto rlua_pushnumber = Declare(Adresses::lua_pushnumber_addr, void, lua_pushnumber_CCV, DWORD a1, double a2);
auto rlua_pushboolean = Declare(Adresses::lua_pushboolean_addr, void, lua_pushboolean_CCV, DWORD a1, int a2);
auto rlua_pushnil = Declare(Adresses::lua_pushnil_addr, void, lua_pushnil_CCV, DWORD a1);
auto rlua_type = Declare(Adresses::lua_type_addr, int, lua_type_CCV, DWORD a1, int idx);
auto rlua_typename = Declare(Adresses::lua_typename_addr, const char*, lua_typename_CCV, DWORD, int);


auto rlua_settable = Declare(Adresses::lua_settable_addr, void, lua_settable_CCV, DWORD a1, int idx);
auto rlua_tolstring = Declare(Adresses::lua_tolstring_addr, const char*, lua_tolstring_CCV, DWORD a1, int a2, std::size_t * a3);
auto rlua_tostring = Declare(Adresses::lua_tostring_addr, const char*, lua_tostring_CCV, int a1, int a2);
auto rlua_setreadonly = Declare(Adresses::lua_setreadonly_addr, void, lua_setreadonly_CCV, DWORD a1, int a2, int a3);
auto rlua_setmetatable = Declare(Adresses::lua_setmetatable_addr, int, lua_setmetatable_CCV, DWORD a1, int a2);
auto rlua_newthread = Declare(Adresses::lua_newthread_addr, int, lua_newthread_CCV, DWORD a1);
auto rlua_rawget = Declare(Adresses::lua_rawget_addr, void, lua_rawget_CCV, DWORD a1, int idx);
auto rlua_rawgeti = Declare(Adresses::lua_rawgeti_addr, void, lua_rawgeti_CCV, DWORD a1, int idx, int n);
auto rlua_rawset = Declare(Adresses::lua_rawset_addr, void, lua_rawget_CCV, DWORD a1, int idx);
auto rlua_rawseti = Declare(Adresses::lua_rawseti_addr, void, lua_rawgeti_CCV, DWORD a1, int idx, int n);
auto rlua_settop = Declare(Adresses::lua_settop_addr, void, lua_settop_CCV, DWORD a1, int idx);
auto rlua_objlen = Declare(Adresses::lua_objlen_addr, size_t, lua_objlen_CCV, DWORD a1, DWORD a2);
auto rluaL_error = Declare(Adresses::lual_error_addr, int, lual_error_CCV, DWORD a1, const char* lol, ...);
auto writerf = Declare(Adresses::print_addr, int, print_CCV, int idx, const char* lol, ...);

void* rlua_newuserdata(DWORD rL, size_t size)
{
	return ((void* (lua_newuserdata_CCV*)(DWORD rL, size_t size, int))unprotect(ScanAddress(Adresses::lua_newuserdata_addr)))(rL, size, 0);
}

double rlua_tonumber(DWORD rL, int idx)
{
	return ((double(lua_tonumber_CCV*)(DWORD, int, int))ScanAddress(Adresses::lua_tonumber_addr))(rL, idx, 0);
}

void SetContext(int L, int Context)
{
	*(DWORD*)(*(DWORD*)(L + Adresses::Identity1) + Adresses::Identity2) = Context;
}

int GetContext(int L)
{
	DWORD a1 = L;
	int v3 = *(DWORD*)(a1 + Adresses::Identity1);
	int final = *(DWORD*)(v3 + Adresses::Identity2);
	return final;
}

void rlua_pushstring(int a1, const char* a2)
{
	rlua_pushlstring(a1, (const char*)a2, strlen(a2));
}

lua_Number rluaL_checknumber(int ls, int numarg)
{
	return ((lua_Number(__fastcall*)(int, int))(0x11F6830))(ls, numarg);
}

const char* rluaL_checklstring(int ls, int a, unsigned int* b)
{
	return ((const char* (__cdecl*)(int, int, unsigned int*))(0x11F6800))(ls, a, b);
}

int rluaL_getmetafield(DWORD RL, int obj, const char* event)
{
	if (!rlua_getmetatable(RL, obj))  /* no metatable? */
		return 0;
	rlua_pushstring(RL, event);
	rlua_rawget(RL, -2);
	if (rlua_type(RL, -1) == RLUA_TNIL) {
		r_lua_pop(RL, 2);  /* remove metatable and metafield */
		return 0;
	}
	else {
		rlua_remove(RL, -2);  /* remove only metatable */
		return 1;
	}
}

void PushValue(int L, int _idx) {
	TValue* top = TOP(L);
	TValue* ok = rlua_index2adr(L, _idx);
	top->tt = ok->tt;
	top->value = ok->value;
	incr_top(L);
}

int rluaL_getmetatable(int ls, const char* tname)
{
	rlua_getfield(ls, -10000, tname);
}
void rluaL_setmetatable(int ls, const char* tname)
{
	rluaL_getmetatable(ls, tname);
	rlua_setmetatable(ls, -2);
}


void rlua_pushccfunction(int rL, int fn, int n, bool lol = false) {
	handler = lol;
	if (lol) {
		rlua_pushlightuserdata(rL, (void*)fn);
		rlua_pushcclosure(rL, int3breakpoint1, 0, n, 0);
	}
	else {
		rlua_pushlightuserdata(rL, (void*)fn);
		rlua_pushcclosure(rL, int3breakpoint1, 0, n + 1, 0);
	}
}

static int int3jumphandler(int CurrentState)
{
	if (handler) {
		typedef int(*_handle)(DWORD);
		_handle handle = (_handle)rlua_touserdata(CurrentState, -10003);
		return handle(CurrentState);
	}
	else {
		typedef int(*_handle)(DWORD);
		_handle handle = (_handle)rlua_touserdata(CurrentState, -10004);
		return handle(CurrentState);
	}
	return 0;
}

LONG WINAPI int3cbreakpoint(PEXCEPTION_POINTERS ex)
{
	if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		if (ex->ContextRecord->Eip == int3breakpoint) {
			ex->ContextRecord->Eip = int3faker;
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (ex->ContextRecord->Eip == int3breakpoint1) {
			ex->ContextRecord->Eip = (DWORD)(int3jumphandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
