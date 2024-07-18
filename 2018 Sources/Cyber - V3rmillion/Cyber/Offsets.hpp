#pragma once

#include <Windows.h>

#include "Offsets.hpp"
#include "RCBypass.hpp"

#include <iostream>
#include <vector>

#define gettop_def ((*(DWORD *)(a1 + 20) - *(DWORD *)(a1 + 24)) >> 4)
#define rL_def ((ScriptContext + 220) ^ (*(DWORD*)(ScriptContext + 220)))

DWORD alua_getfield = offset(0x72C120);
DWORD alua_setfield = offset(0x72DE80);
DWORD alua_pushvalue = offset(0x72D5A0);
DWORD alua_pcall = offset(0x72CF20);
DWORD alua_tolstring = offset(0x72E4A0);
DWORD alua_tonumber = offset(0x72E770);
DWORD alua_toboolean = offset(0x72E3E0); 
DWORD alua_call = offset(0x72BAC0);
DWORD alua_settop = offset(0x72E260);
DWORD alua_pushstring = offset(0x72D4E0);
DWORD what = offset(0x725717); 
DWORD alua_pushnil = offset(0x72D3E0); 
DWORD alua_pushnumber = offset(0x72D450);
DWORD alua_pushboolean = offset(0x72CFF0);
DWORD alua_getmetatable = offset(0x72C410);
DWORD alua_type = offset(0x72E8D0); 
DWORD alua_next = offset(0x72CDA0); 
DWORD aluaL_ref = offset(0x7277F0); 
DWORD alua_rawgeti = offset(0x72D7B0); 
DWORD alua_createtable = offset(0x72BD70);
DWORD alua_settable = offset(0x73e580);
DWORD anewuserdata = offset(0x73d010);
DWORD aScriptContext = offset(0x1011748);
DWORD Context = offset(0x15A27F0);
DWORD APushCCLosure = offset(0x72D070);

typedef void*(__cdecl* tlua_newuserdata)(DWORD, std::size_t);
typedef void(__cdecl* tlua_settable)(DWORD rL, int idx);

typedef void(__cdecl* tlua_getfield)(DWORD L, int idx, const char* k);
typedef void(__cdecl* tlua_setfield)(DWORD rL, int idx, const char* k);
typedef void(__cdecl* tlua_pushvalue)(DWORD rL, int idx);
typedef void(__cdecl* tlua_pushcclosure)(DWORD rL, const void* fn, int n);
typedef int(__cdecl* tlua_pcall)(DWORD rL, int nargs, int nresults, int errfunc);
typedef const char*(__cdecl* tlua_tolstring)(DWORD rL, int idx, size_t s);
typedef double(__cdecl* tlua_tonumber)(DWORD rL, int idx);
typedef bool(__cdecl* tlua_toboolean)(DWORD rL, int idx);
typedef int(__cdecl* tlua_call)(DWORD rL, int args, int results);
typedef void(__cdecl* tlua_settop)(DWORD rL, int top);
typedef void(__cdecl* tlua_pushstring)(DWORD rL, const char* k);
typedef void(__cdecl* tlua_pushnil)(DWORD rL);
typedef void(__cdecl* tlua_pushnumber)(DWORD rL, double n);
typedef void(__cdecl* tlua_pushboolean)(DWORD rL, bool b);
typedef int(__cdecl* tlua_getmetatable)(DWORD rL, int idx);
typedef int(__cdecl* tlua_type)(DWORD rL, int idx);
typedef int(__cdecl* tlua_next)(DWORD rL, int idx);
typedef void(__cdecl* tlua_rawseti)(DWORD rL, int idx, int n);
typedef int(__cdecl* tluaL_ref)(DWORD rL, int t);
typedef void(__cdecl* tlua_rawgeti)(DWORD rL, int idx, int n);
typedef void(__cdecl* tlua_createtable)(DWORD rL, int narr, int nrec);
typedef void(__cdecl* tlua_pushcclosure)(DWORD rL, const void* fn, int n);

tlua_pushcclosure rlua_pushcclosure = (tlua_pushcclosure)ReturnCheck::UnProtect((byte*)(APushCCLosure));
tlua_getfield rlua_getfield = (tlua_getfield)ReturnCheck::UnProtect((byte*)(alua_getfield));
tlua_setfield arlua_setfield = (tlua_setfield)ReturnCheck::UnProtect((byte*)(alua_setfield));
tlua_pushvalue rlua_pushvalue = (tlua_pushvalue)ReturnCheck::UnProtect((byte*)(alua_pushvalue));
tlua_pcall arlua_pcall = (tlua_pcall)ReturnCheck::UnProtect((byte*)(alua_pcall));
tlua_tolstring rlua_tolstring = (tlua_tolstring)ReturnCheck::UnProtect((byte*)(alua_tolstring));
tlua_tonumber rlua_tonumber = (tlua_tonumber)ReturnCheck::UnProtect((byte*)(alua_tonumber));
tlua_toboolean rlua_toboolean = (tlua_toboolean)ReturnCheck::UnProtect((byte*)(alua_toboolean));
tlua_call rlua_call = (tlua_call)ReturnCheck::UnProtect((byte*)(alua_call));
tlua_settop rlua_settop = (tlua_settop)ReturnCheck::UnProtect((byte*)(alua_settop));
tlua_pushstring rlua_pushstring = (tlua_pushstring)ReturnCheck::UnProtect((byte*)(alua_pushstring));
tlua_pushnil rlua_pushnil = (tlua_pushnil)ReturnCheck::UnProtect((byte*)(alua_pushnil));
tlua_pushnumber rlua_pushnumber = (tlua_pushnumber)ReturnCheck::UnProtect((byte*)(alua_pushnumber));
tlua_pushboolean rlua_pushboolean = (tlua_pushboolean)ReturnCheck::UnProtect((byte*)(alua_pushboolean));
tlua_getmetatable rlua_getmetatable = (tlua_getmetatable)ReturnCheck::UnProtect((byte*)(alua_getmetatable));
tlua_type rlua_type = (tlua_type)ReturnCheck::UnProtect((byte*)(alua_type));
tlua_next rlua_next = (tlua_next)ReturnCheck::UnProtect((byte*)(alua_next));
tluaL_ref rluaL_ref = (tluaL_ref)ReturnCheck::UnProtect((byte*)(aluaL_ref));
tlua_rawgeti rlua_rawgeti = (tlua_rawgeti)ReturnCheck::UnProtect((byte*)(alua_rawgeti));
tlua_createtable rlua_createtable = (tlua_createtable)ReturnCheck::UnProtect((byte*)(alua_createtable));
tlua_newuserdata rlua_newuserdata = (tlua_newuserdata)ReturnCheck::UnProtect((byte*)(anewuserdata));
tlua_settable rlua_settable = (tlua_settable)ReturnCheck::UnProtect((byte*)(alua_settable));

#define rlua_pop(L,n)					 (rlua_settop(L, -(n)-1))
#define rlua_getglobal(rL, k)			 (rlua_getfield(rL, -10002, k))

#define RLUA_TNONE						 -1
#define RLUA_TNIL						 0
#define RLUA_TNUMBER					 2
#define RLUA_TBOOLEAN					 3
#define RLUA_TSTRING					 4
#define RLUA_TFUNCTION					 6
#define RLUA_TTABLE						 7
#define RLUA_TUSERDATA					 8

DWORD rlua_gettop(DWORD a1) {
	return gettop_def;
};

void rlua_pcall(int LuaState, int NumberOfArguments, int NumberOfResults, int ErrorCallback) {
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(what), "\xEB", 1, 0);
	arlua_pcall(LuaState, NumberOfArguments, NumberOfResults, ErrorCallback);
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(what), "\x74", 1, 0);
};

void rlua_setfield(DWORD L, int idx, const char *k) {
	rlua_pushvalue(L, idx);
	if (rlua_getmetatable(L, -1)) {
		rlua_getfield(L, -1, "__newindex");
		rlua_pushvalue(L, -3);
		rlua_pushstring(L, k);
		rlua_pushvalue(L, -6);
		rlua_pcall(L, 3, 1, 0);
		rlua_pop(L, 3);
	}
	else {
		rlua_pop(L, 1);
		arlua_setfield(L, idx, k);
	};
};

union r_Value
{
	int b;
	double n;
	const void* p;
	void* gc;
};

struct r_TValue
{
	r_Value value;
	int tt;
};

std::vector<DWORD> breakpointaddys;

auto index2adr = (r_TValue*(__stdcall *)(DWORD lst, signed int idx))offset(0x736D70);//aobscan::scan("\x55\x8B\xEC\x56\x8B\x75\x0C\x85\xF6\x7E\x1C", "xxxxxxxxxxx");

int rkeyfn;

int int3fnhandler(DWORD rL);

LONG WINAPI breakpointHandle(PEXCEPTION_POINTERS ex)
{
	switch (ex->ExceptionRecord->ExceptionCode)
	{
	case (DWORD)0x80000003L:
	{
		for (int i = 0; i < breakpointaddys.size(); i++) {
			if (ex->ContextRecord->Eip == breakpointaddys[i])
				ex->ContextRecord->Eip = (DWORD)(int3fnhandler);
		}
		return -1;
	}
	default: return 0;
	}
	return 0;
}

VOID enable_exceptionHandler()
{
	breakpointaddys.push_back(offset(0x401018));//This is the main breakpoint where retcheck would call to crash your sonofabitch
	AddVectoredExceptionHandler(1, breakpointHandle);
}

void pushlightuserdata(const void* fn)
{
	r_TValue* top = *(r_TValue**)(rL + 20);
	top->value.p = fn;
	top->tt = 1;
	*(DWORD*)(rL + 20) += 16;
}

void pushcclosure(const void* func, DWORD n) {
	try {
		rlua_pushcclosure(rL, func, n);
	}
	catch (...) {};
}

VOID pushcclosurex(const void* fn, DWORD n)
{
	pushlightuserdata(fn);
	pushcclosure((const void*)(breakpointaddys[0]), n);
}

int pushcclosurex2(const void* fn, DWORD n)
{
	pushlightuserdata(fn);
	pushcclosure((const void*)(breakpointaddys[0]), n);
	return TRUE;
}

namespace PrintExploit {
	namespace Integers {
		int OutputType = 0; // 0 = Print, 1 = Info, 2 = Warn, 3 = Error
	}
}

enum ErrorColors
{
	Print,
	Info,
	Warn,
	Error
};

//https://gyazo.com/ad4f7e55ae7186140896b0ec7d150865
typedef int(__cdecl *SINGLETON)(int a1, const char *a2);
SINGLETON SingletonPrint = (SINGLETON)offset(0x4FC8D0);
