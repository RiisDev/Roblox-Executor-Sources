#pragma once
//#include "Main.h"
#include "Globals.hpp"
#include "Retcheck.h"

#include <iostream>
#include <vector>

#define rL_def ((ScriptContext + 220) ^ (*(DWORD*)(ScriptContext + 220)))

DWORD alua_getfield = offset(0x73C460); // 1
DWORD alua_setfield = offset(0x73E230);  // 2
DWORD alua_pushvalue = offset(0x73D8F0); // 3
DWORD alua_pushcclosure = offset(0x73D3A0); // 4
DWORD alua_pcall = offset(0x73d250); // 5
DWORD alua_tolstring = offset(0x73E850); // 6
DWORD alua_tonumber = offset(0x73EB40); // 7
DWORD alua_toboolean = offset(0x73E790); // 8
DWORD alua_call = offset(0x73be50); // 9
DWORD alua_settop = offset(0x73E610); // 10
DWORD alua_pushstring = offset(0x73D830); // 11
DWORD alua_pushnil = offset(0x73d730); // 12
DWORD alua_pushnumber = offset(0x73d7a0); // 13
DWORD alua_pushboolean = offset(0x73d320); // 14
DWORD alua_getmetatable = offset(0x73C730); // 15
DWORD alua_setmetatable = offset(0x73e3e0); // 16
DWORD alua_type = offset(0x73ECA0); // 17
DWORD alua_next = offset(0x73d0d0); // 18
DWORD aluaL_ref = offset(0x737470); //19
DWORD alua_rawgeti = offset(0x73dbb0); // 20
DWORD alua_createtable = offset(0x73c110); // 21
DWORD aScriptContext = offset(0x6ba66e); // 22
DWORD Context = offset(0x59c300); // 23
DWORD alua_touserdata = offset(0x73EC70); // 24
DWORD what = offset(0x7355d7); // 25
DWORD anewuserdata = offset(0x73d010); // 26
DWORD alua_settable = offset(0x73e580); // 27

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

std::string ConvertToString(int i) {
	std::string returnval;
	if (i) {
		returnval = std::to_string(i);
	}
	return returnval;
}

std::vector<std::string> gay(std::string str, char Delim) {
	std::vector<std::string> Args;
	std::stringstream ss(str);
	std::string Arg;
	while (getline(ss, Arg, Delim))
		Args.push_back(Arg);
	return Args;
}

std::string GetString() {
	std::string input;
	getline(std::cin, input);
	return input;
}

std::string tostring(std::string str)
{
	std::string retn("");
	for (int i = 0; i < str.length(); i++)
	{
		int ascii = (int)str.at(i);
		if (ascii >= (int)'a' && ascii <= (int)'z')
			retn += (char)ascii;
		else
			retn += (char)(ascii + ((int)'a' - (int)'A'));
	}
	return retn;
}

std::string GetInput() {
	std::string input;
	getline(std::cin, input);
	return input;
}

//update calling conventions, I got lazy lol
// will send u them
// update luastate in memory.h

typedef void*(__cdecl* tlua_newuserdata)(DWORD, std::size_t); //done
typedef void*(__cdecl* tlua_touserdata)(DWORD rL, int);

typedef void(__cdecl* tlua_settable)(DWORD rL, int idx); //same

typedef void(__fastcall* tlua_getfield)(DWORD L, int idx, const char* k);
typedef void(__cdecl* tlua_setfield)(DWORD rL, int idx, const char* k);
typedef void(__stdcall* tlua_pushvalue)(DWORD rL, int idx);
typedef void(__cdecl* tlua_pushcclosure)(DWORD rL, const void* fn, int n);
typedef int(__cdecl* tlua_pcall)(DWORD rL, int nargs, int nresults, int errfunc);
typedef const char*(__stdcall* tlua_tolstring)(DWORD rL, int idx, size_t s);
typedef double(__stdcall* tlua_tonumber)(DWORD rL, int idx);
typedef bool(__cdecl* tlua_toboolean)(DWORD rL, int idx);
typedef int(__cdecl* tlua_call)(DWORD rL, int args, int results);
typedef void(__fastcall* tlua_settop)(DWORD rL, int top);
typedef void(__cdecl* tlua_pushstring)(DWORD rL, const char* k);
typedef void(__cdecl* tlua_pushnil)(DWORD rL);
typedef void(__thiscall* tlua_pushnumber)(DWORD rL, double n);
typedef void(__cdecl* tlua_pushboolean)(DWORD rL, bool b);
typedef int(__cdecl* tlua_getmetatable)(DWORD rL, int idx);
typedef int(__cdecl* tlua_type)(DWORD rL, int idx);
typedef int(__cdecl* tlua_next)(DWORD rL, int idx);
typedef void(__cdecl* tlua_rawseti)(DWORD rL, int idx, int n);
typedef int(__cdecl* tluaL_ref)(DWORD rL, int t);
typedef void(__cdecl* tlua_rawgeti)(DWORD rL, int idx, int n);
typedef void(__cdecl* tlua_createtable)(DWORD rL, int narr, int nrec);
typedef void(__cdecl* tlua_pushcclosure)(DWORD rL, const void* fn, int n);

tlua_pushcclosure rlua_pushcclosure = (tlua_pushcclosure)Retcheck::unprotect((byte*)(alua_pushcclosure));
tlua_getfield rlua_getfield = (tlua_getfield)Retcheck::unprotect((byte*)(alua_getfield));
tlua_setfield arlua_setfield = (tlua_setfield)Retcheck::unprotect((byte*)(alua_setfield));
tlua_pushvalue rlua_pushvalue = (tlua_pushvalue)Retcheck::unprotect((byte*)(alua_pushvalue));
tlua_pcall arlua_pcall = (tlua_pcall)Retcheck::unprotect((byte*)(alua_pcall));
tlua_tolstring rlua_tolstring = (tlua_tolstring)Retcheck::unprotect((byte*)(alua_tolstring));
tlua_tonumber rlua_tonumber = (tlua_tonumber)Retcheck::unprotect((byte*)(alua_tonumber));
tlua_toboolean rlua_toboolean = (tlua_toboolean)Retcheck::unprotect((byte*)(alua_toboolean));
tlua_call rlua_call = (tlua_call)Retcheck::unprotect((byte*)(alua_call));
tlua_settop rlua_settop = (tlua_settop)Retcheck::unprotect((byte*)(alua_settop));
tlua_pushstring rlua_pushstring = (tlua_pushstring)Retcheck::unprotect((byte*)(alua_pushstring));
tlua_pushnil rlua_pushnil = (tlua_pushnil)Retcheck::unprotect((byte*)(alua_pushnil));
tlua_pushnumber rlua_pushnumber = (tlua_pushnumber)Retcheck::unprotect((byte*)(alua_pushnumber));
tlua_pushboolean rlua_pushboolean = (tlua_pushboolean)Retcheck::unprotect((byte*)(alua_pushboolean));
tlua_getmetatable rlua_getmetatable = (tlua_getmetatable)Retcheck::unprotect((byte*)(alua_getmetatable));
tlua_type rlua_type = (tlua_type)Retcheck::unprotect((byte*)(alua_type));
tlua_next rlua_next = (tlua_next)Retcheck::unprotect((byte*)(alua_next));
tluaL_ref rluaL_ref = (tluaL_ref)Retcheck::unprotect((byte*)(aluaL_ref));
tlua_newuserdata rlua_newuserdata = (tlua_newuserdata)Retcheck::unprotect((byte*)(anewuserdata));
tlua_rawgeti rlua_rawgeti = (tlua_rawgeti)Retcheck::unprotect((byte*)(alua_rawgeti));
tlua_createtable rlua_createtable = (tlua_createtable)Retcheck::unprotect((byte*)(alua_createtable));
tlua_touserdata rlua_touserdata = (tlua_touserdata)Retcheck::unprotect((byte*)(alua_touserdata));
tlua_settable rlua_settable = (tlua_settable)Retcheck::unprotect((byte*)(alua_settable));


#define rlua_pushcfunction(rL, fn)		 (rlua_pushcclosure(rL,fn,0))
#define rlua_newtable(rL)				 (rlua_createtable(rL, 0, 0))
#define rlua_clearstack(rL)				 (rlua_settop(rL, 0))
#define rlua_getglobal(rL, k)			 (rlua_getfield(rL, -10002, k))
#define rlua_setglobal(rL, k)			 (rlua_setfield(rL, -10002, k))
#define rlua_pop(L,n)					 (rlua_settop(L, -(n)-1))
#define rlua_isnil(L,n)					 (rlua_type(L, (n)) == 0)
#define rlua_isnone(L,n)			     (rlua_type(L, (n)) == (-1))
#define rlua_isnumber(L,n)				 (rlua_type(L, (n)) == 2)
#define rlua_isbool(L,n)			     (rlua_type(L, (n)) == 3)
#define rlua_isstring(L,n)				 (rlua_type(L, (n)) == 4)
#define rlua_islightuserdata(L,n)		 (rlua_type(L, (n)) == 1)
#define rlua_istable(L,n)				 (rlua_type(L, (n)) == 7)
#define rlua_isuserdata(L,n)		     (rlua_type(L, (n)) == 8)
#define rlua_isfunction(L,n)		     (rlua_type(L, (n)) == 6)
#define rlua_isproto(L,n)				 (rlua_type(L, (n)) == 9)
#define rlua_isthread(L,n)				 (rlua_type(L, (n)) == 5)
#define rlua_isupvalue(L,n)				 (rlua_type(L, (n)) == 10)
#define RLUA_TNONE						 -1
#define RLUA_TNIL						 0
#define RLUA_TLIGHTUSERDATA				 1
#define RLUA_TNUMBER					 2
#define RLUA_TBOOLEAN					 3
#define RLUA_TSTRING					 4
#define RLUA_TTHREAD					 5
#define RLUA_TFUNCTION					 6
#define RLUA_TTABLE						 7
#define RLUA_TUSERDATA					 8
#define RLUA_TPROTO						 9
#define RLUA_TUPVAL						 10


DWORD rlua_gettop(DWORD a1) {
	return ((*(DWORD *)(a1 + 20) - *(DWORD *)(a1 + 8)) >> 4);
}


int rlua_pcall(int LuaState, int NumberOfArguments, int NumberOfResults, int ErrorCallback) {

	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(what), "\xEB", 1, 0);
	arlua_pcall(LuaState, NumberOfArguments, NumberOfResults, ErrorCallback);
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(what), "\x74", 1, 0);
	return 0;
}


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
	}
}

std::vector<DWORD> breakpointaddys;

auto index2adr = (r_TValue*(__stdcall *)(DWORD lst, signed int idx))offset(0x736D70);//aobscan::scan("\x55\x8B\xEC\x56\x8B\x75\x0C\x85\xF6\x7E\x1C", "xxxxxxxxxxx");

//int int3fnhandler(DWORD rL);

int int3fnhandlerr(DWORD rL)
{
	typedef int(*_handle)(DWORD);
	_handle handle = (_handle)(index2adr(rL, -10003)->value.p);
	return handle(rL);
}

LONG WINAPI breakpointHandle(PEXCEPTION_POINTERS ex)
{
	switch (ex->ExceptionRecord->ExceptionCode)
	{
	case (DWORD)0x80000003L:
	{
		for (int i = 0; i < breakpointaddys.size(); i++) {
			if (ex->ContextRecord->Eip == breakpointaddys[i])
				ex->ContextRecord->Eip = (DWORD)(int3fnhandlerr);
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




