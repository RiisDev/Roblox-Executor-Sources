#pragma once
#include <windows.h>
#include <string>
#include "aobscan.h"
//#include "VMProtectSDK.h"
//#include "../lua-5.1/src/lua.hpp"

typedef int(*lua_CFunction)(int lua_State2);

/*
extern "C" {
#include "../lua-5.1/src/lua.h"
#include "../lua-5.1/src/lualib.h"
#include "../lua-5.1/src/lauxlib.h"
}
*/



DWORD Base3 = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");

DWORD GetAddress2(int address) {
	try {
		return{ address - 0x400000 + Base3 };
	}
	catch (...) {};
}

int lua_State2 = 0;

#define LUA_REGISTRYINDEX       (-10000)
#define LUA_ENVIRONINDEX        (-10001)
#define LUA_GLOBALSINDEX        (-10002)
#define lua_upvalueindex(i)     (LUA_GLOBALSINDEX-(i))

#define LUA_TNONE               (-1)
#define LUA_TNIL                0
#define LUA_TBOOLEAN            1
#define LUA_TLIGHTUSERDATA      2
#define LUA_TNUMBER             3
#define LUA_TSTRING             4
#define LUA_TTABLE              5
#define LUA_TFUNCTION           6
#define LUA_TUSERDATA           7
#define LUA_TTHREAD             8

//#define lua_pushcfunction(L,f)	lua_pushcclosure(L, (f), 0)
#define lua_pushcfunction66(L,f)	lua_pushcclosure(L, (f), 0)
#define lua_pop(L,n)            lua_settop(L, -(n)-1)
#define lua_isfunction(L,n)     (lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)        (lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)        (lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)          (lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)      (lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)       (lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)         (lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)   (lua_type(L, (n)) <= 0)

#define lua_setglobal(L,s)      lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L,s)      lua_getfield(L, LUA_GLOBALSINDEX, (s))
#define lua_tostring(L,i)       lua_tolstring(L, (i), NULL)
#define lua_emptystack(L)       lua_settop(L, 0)

typedef void* (__cdecl *lua_GetField)(int lua_State2, int idx, const char *k);
typedef int(__cdecl *lua_PCall)(int lua_State2, int nargs, int nresults, int errfunc);
typedef void(__cdecl *lua_PushString)(int lua_State2, const char *s);
typedef void(__cdecl *lua_PushNumber)(int lua_State2, double Number);
typedef void(__cdecl *lua_PushBoolean)(int lua_State2, bool thevalue);
typedef const char *(__cdecl *lua_ToLString)(int lua_State2, int idx, size_t *len);
typedef void* (__cdecl *lua_PushValue)(int lua_State2, int idx);
typedef int(__cdecl *lua_GetTop)(int lua_State2);
typedef void* (__cdecl *lua_SetTop)(int lua_State2, int n);
typedef void* (__cdecl *lua_SetField)(int lua_State2, int idx, const char *k);

//typedef int (*lua_CFunction)(int lua_State2);
typedef void* (__cdecl *lua_PushCclosure)(int lua_State2, lua_CFunction a2, int a3);
typedef BOOL(__cdecl *lua_ToBoolean)(int lua_State2, int idx);
typedef int(__cdecl *lua_Type)(int lua_State2, int idx);
typedef int(__cdecl *lua_SetMetatable)(int lua_State2, int n);
typedef int(__cdecl *lua_NewLuaState)();
typedef int(__cdecl *lua_NewUserdata)(int r_lua_State, int sz);

//typedef int(__cdecl *lua_PushCclosure)(int lua_State2, int(*a2)(), int a3);


lua_PushCclosure lua_pushcclosure2;
lua_PushString lua_pushstring2;
lua_SetField lua_setfield2;
lua_PushNumber lua_pushnumber2;
lua_PCall lua_pcall2;
lua_ToLString lua_tolstring2;
lua_SetTop lua_settop2;
lua_ToBoolean lua_toboolean2;
lua_Type lua_type2;
lua_NewUserdata lua_newuserdata2;
lua_GetField lua_getfield2;
lua_PushValue lua_pushvalue2;
lua_PushBoolean lua_pushboolean2;
lua_SetMetatable lua_setmetatable2;
lua_GetTop lua_getop2;
lua_NewLuaState lua_newstate2;

void WriteDWORD(DWORD *adr, int value)
{
	DWORD OldProtection;
	VirtualProtect((LPVOID)adr, 4, PAGE_READWRITE, &OldProtection);
	*(DWORD*)(adr) = value;
	VirtualProtect((LPVOID)adr, 4, OldProtection, &OldProtection);
}

void lua_pushnil(int lua_State2) {
	*(DWORD *)(*(DWORD *)(lua_State2 + 16) + 8) = 0;
	*(DWORD *)(lua_State2 + 16) += 16;
}

int lua_gettop(int lua_State2) {
	return (*(DWORD *)(lua_State2 + 16) - *(DWORD *)(lua_State2 + 28)) >> 4;
}

int lua_atpanic(int a1, int a2) {
	char *v2;
	int v3;
	int v4;

	v2 = (char *)(a1 + *(DWORD *)(a1 + 8) + 92);
	v3 = *(DWORD *)v2;
	*(DWORD *)v2 = a2 - (DWORD)v2;
	v4 = (int)&v2[v3];
	return v4;
}

int lua_setlevel(int lua_State2, int a2) {
	int result;

	result = a2;
	*(WORD *)(a2 + 54) = *(WORD *)(lua_State2 + 54);
	return result;
}

bool DoRestoreFlags = true;

DWORD CallCheck = (0x1533DF0 - 0x400000 + (DWORD)GetModuleHandle(NULL)); //Search for 'PlaceFilter_LuaStrlibLimitMatchDepth' in Strings. Xref 'sub_XXXXXX+9' and click 'Ok'. Scroll down and look for 'lea eax, dword_AddressHere[eax]'
DWORD IdentityFlag = (0x15C8E50 - 0x400000 + (DWORD)GetModuleHandle(NULL));
int retcheck2 = GetAddress2(0x49B7B0); //get getfield, copy the address, control + f it in functions, view it in ida-view, scroll down to add esp, 4 and find (RETCHECK =  call sub_552430) (1st FLAG = or dword_14643E8, 200000h) (2nd FLAG = mov dword_1A1F424, 0)

									   //int returncheckcheck = GetAddress2(0x1A9F000);
									   //int unusedPaddingText = GetAddress2(0xA0EF44);

int* f1 = (int*)(0x153CE58 - 0x400000 + (DWORD)GetModuleHandle(NULL));
int* f2 = (int*)(0x1536BAC - 0x400000 + (DWORD)GetModuleHandle(NULL));


DWORD HackFlag1Old, HackFlag2Old;
DWORD hakerflag1 = GetAddress2(0x14703A8);
DWORD hakerflag2 = GetAddress2(0x15912DC);

void RestoreFlags() {
	try {
		DoRestoreFlags = false;
		DWORD* HackFlag1 = (DWORD*)hakerflag1;
		DWORD* HackFlag2 = (DWORD*)hakerflag2;
		*HackFlag1 = HackFlag1Old;
		*HackFlag2 = HackFlag2Old;
		DoRestoreFlags = true;
		return;
	}
	catch (...) {};
}

void RestoreFlags_Loop() {
	do {
		if (DoRestoreFlags) {
			try {
				DWORD* HackFlag1 = (DWORD*)hakerflag1;
				DWORD* HackFlag2 = (DWORD*)hakerflag2;
				HackFlag1Old = *HackFlag1;
				HackFlag2Old = *HackFlag2;
			}
			catch (...) {};
		}
		Sleep(65);
	} while (true);
}

void MakeJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen)
{
	try {
		DWORD dwOldProtect, dwBkup, dwRelAddr;
		VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);
		dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;
		*pAddress = 0xE9;
		(*(DWORD*)(pAddress + 0x1)) = dwRelAddr;
		for (DWORD x = 0x5; x < dwLen; x++) *(pAddress + x) = 0x90;
		VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);
		return;
	}
	catch (...) {};
}

void Retcheck_Disable() {
	try {
		DWORD* HackFlag1 = (DWORD*)hakerflag1;
		DWORD* HackFlag2 = (DWORD*)hakerflag2;
		HackFlag1Old = *HackFlag1;
		HackFlag2Old = *HackFlag2;
		MakeJMP((BYTE*)GetAddress2(0x504690), (DWORD)RestoreFlags, 0x6);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RestoreFlags_Loop, NULL, NULL, NULL);
	}
	catch (...) {};
}




//LUA FUNCTIONS
int lua_newuserdata(int lua_State2, int n) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	int state = lua_newuserdata2(lua_State2, n);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;

	return state;
}

int lua_setmetatable(int lua_State2, int n) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	int state = lua_setmetatable2(lua_State2, n);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;

	return state;
}

int lua_type(int lua_State2, int n) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	int state = lua_type2(lua_State2, n);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;

	return state;
}

void lua_getfield(int lua_State2, int idx, const char *k) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	DWORD returnold, returnfag;
	DWORD undold, oldfag;

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	lua_getfield2(lua_State2, idx, k);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
}


void lua_pushcclosure(int lua_State2, lua_CFunction a2, int a3) {
	int a = *f1;
	int b = *f2;

	DWORD old, fag;

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);


	lua_pushcclosure2(lua_State2, a2, a3);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	*f1 = a;
	*f2 = b;
}
void lua_pushstring(int lua_State2, const char *s) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	lua_pushstring2(lua_State2, s);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
}

void lua_pushnumber(int lua_State2, double Number) {
	int a = *f1;
	int b = *f2;

	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	lua_pushnumber2(lua_State2, Number);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
}

void lua_pushboolean(int lua_State2, bool thevalue) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	lua_pushboolean2(lua_State2, thevalue);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
}

const char* lua_tolstring(int lua_State2, int idx, size_t *len) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	const char* noobser = lua_tolstring2(lua_State2, idx, len);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
	return noobser;
}

void lua_pushvalue(int lua_State2, int idx) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	lua_pushvalue2(lua_State2, idx);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
}

int lua_getop(int lua_State2) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	int gettop = lua_getop2(lua_State2);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
	return gettop;
}

void lua_pcall(int lua_State2, int nargs, int nresults, int errfunc) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	lua_pcall2(lua_State2, nargs, nresults, errfunc);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
}

void lua_settop(int lua_State2, int n) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	lua_settop2(lua_State2, n);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
}

bool lua_toboolean(int lua_State2, int idx) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	bool asshurteasteregg = lua_toboolean2(lua_State2, idx);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
	return asshurteasteregg;
}

void lua_setfield(int lua_State2, int idx, const char *k) {
	int a = *f1;
	int b = *f2;
	DWORD old, fag;
	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0xC3;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

	lua_setfield2(lua_State2, idx, k);

	VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)retcheck2 = 0x55;
	VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
	*f1 = a;
	*f2 = b;
}

/*
void printidentitieslol()
{
int a = *f1;
int b = *f2;
DWORD old, fag;
VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
*(BYTE*)retcheck2= 0xC3;
VirtualProtect((LPVOID)retcheck2, 1, old, &fag);

lua_getfield(lua_State2, -10002, "printidentity");
lua_pcall(lua_State2, 0, 0, 0);

VirtualProtect((LPVOID)retcheck2, 1, PAGE_EXECUTE_READWRITE, &old);
*(BYTE*)retcheck2 = 0x55;
VirtualProtect((LPVOID)retcheck2, 1, old, &fag);
*f1 = a;
*f2 = b;
}
*/