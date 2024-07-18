#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include <istream>
#include <iterator>
#include "ret.h"
#define _CRT_SECURE_NO_WARNINGS
//addr = penvcheck addy
void set_jnz(DWORD addr)
{
	DWORD o_buff;
	VirtualProtect((void*)addr, 5, PAGE_EXECUTE_READWRITE, &o_buff);
	*(char*)addr = 0x75;
	VirtualProtect((void*)addr, 5, o_buff, &o_buff);
}


void set_jz(DWORD addr)
{
	DWORD o_buff;
	VirtualProtect((void*)addr, 5, PAGE_EXECUTE_READWRITE, &o_buff);
	*(char*)addr = 0x74;
	VirtualProtect((void*)addr, 5, o_buff, &o_buff);
}
DWORD aslr(int address) {
	return (address - 0x400000 + (DWORD)GetModuleHandle("RobloxPlayerBeta.exe"));
}
typedef void (_stdcall *lua_getfield)(int L, int idx, const char *k);
lua_getfield LuaCGetfield = (lua_getfield)unprotect(aslr(0x73F8D0)); //getfield
typedef void(_stdcall *lua_pushstring)(int L, const char *k);
lua_pushstring LuaCPushString = (lua_pushstring)unprotect(aslr(0x740C70)); //pushstring
typedef int(_cdecl *lua_pcall)(int L, int args1, int args2, int args3);
lua_pcall LuaCPcall = (lua_pcall)unprotect(aslr(0x7406B0)); //pcall
class LuaCFunctions {
public:
	static void getglobal(int L, const char *k)
	{
		LuaCFunctions::getfield(L, -10002, k);
	}
	static void getfield(int L, int idx, const char *k)
	{
		set_jnz(0x738A07); //penv
		LuaCGetfield(L, idx, k);
		set_jz(0x738A07); //penv
	}
	static void pushstring(int L, const char *k)
	{
		set_jnz(0x738A07);
		LuaCPushString(L, k);
		set_jz(0x738A07);
	}
	static void pcall(int L, int args1, int args2, int args3)
	{
		set_jnz(0x738A07);
		LuaCPcall(L, args1, args2, args3);
		set_jz(0x738A07);
	}
};