#pragma once
#include "SDK.h"
#include "Memory.h"

DWORD unprotect(DWORD addr) //this retcheck is outdated look for eternals latest retcheck
{
	BYTE* tAddr = (BYTE *)addr;

	do {
		tAddr += 0x10;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	memcpy(nFunc, (BYTE*)addr, funcSz);

	DWORD pos = (DWORD)nFunc;
	BOOL valid = false;
	do {
		if (*(BYTE*)pos == 0x72 && *(BYTE*)(pos + 0x2) == 0xA1 && *(BYTE*)(pos + 0x7) == 0x8B) {
			memcpy((void*)pos, "\xEB", 1);

			DWORD cNFunc = (DWORD)nFunc;
			do {
				if (*(BYTE*)cNFunc == 0xE8)
				{
					DWORD tFunc = addr + (cNFunc - (DWORD)nFunc);
					DWORD oFunc = (tFunc + *(DWORD*)(tFunc + 1)) + 5;

					if (oFunc % 16 == 0)
					{
						DWORD rebFAddr = oFunc - cNFunc - 5;
						*(DWORD*)(cNFunc + 1) = rebFAddr;
					}
					cNFunc += 5;
				}
				else
					cNFunc += 1;
			} while (cNFunc - (DWORD)nFunc < funcSz);

			valid = true;
		}
		pos += 1;
	} while (pos < (DWORD)nFunc + funcSz);

	if (!valid) {
		VirtualFree(nFunc, funcSz, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}

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

int L;

typedef void(__cdecl *Lua_getfield)(int lua_State, int idx, const char *k);
Lua_getfield lua_getfield = (Lua_getfield)unprotect(Memory::SigScan("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x83\xC4\x08\x8B\xCA\x8B\xF8\x8D\x59\x01\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\xFF\x76\x0C", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx????xxx"));
typedef int(__cdecl *Lua_pcall)(int lua_State, int naArgs, int nresults, int errfunc);
Lua_pcall lua_pcall = (Lua_pcall)unprotect(Memory::SigScan("\x55\x8B\xEC\x8B\x45\x14\x83\xEC\x08\x53\x56", "xxxxxxxxxxx"));
typedef void(__cdecl *Lua_setfield)(int lua_State, int idx, const char *k);
Lua_setfield lua_setfield = (Lua_setfield)unprotect(Memory::SigScan("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x83\xC4\x08\x8B\xCA\x8B\xF8\x8D\x59\x01\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\x89\x45\xF0", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx????xxx"));
typedef void(__cdecl *Lua_pushstring)(int lua_State, const char *s);
Lua_pushstring lua_pushstring = (Lua_pushstring)unprotect(Memory::SigScan("\x55\x8B\xEC\x8B\x55\x0C\x85\xD2\x75\x0D", "xxxxxxxxxx"));
typedef void(__cdecl *Lua_pushvalue)(int lua_State, int idx);
Lua_pushvalue lua_pushvalue = (Lua_pushvalue)unprotect(Memory::SigScan("\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\x75\x08\x56\xE8\x00\x00\x00\x00\x8B\x56\x0C\x83\xC4\x08", "xxxxxxxxxxxx????xxxxxx"));
typedef void(__cdecl *Lua_settop)(int lua_State, int n);
Lua_settop lua_settop = (Lua_settop)unprotect(Memory::SigScan("\x55\x8B\xEC\x8B\x4D\x0C\x8B\x55\x08\x85\xC9\x78\x35", "xxxxxxxxxxxxx"));
typedef void(__cdecl *Lua_pushnumber)(int lua_State, double number);
Lua_pushnumber lua_pushnumber = (Lua_pushnumber)unprotect(Memory::SigScan("\x55\x8B\xEC\x8B\x55\x08\x0F\x28\x15", "xxxxxxxxx"));
//ypedef int(__cdecl *Lua_NewThread)(int State);
//Lua_NewThread lua_newthread = (Lua_NewThread)unprotect(Memory::SigScan("\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x51\x56\x8B\x75\x08\x57\x8B\x4E\x20", "xxxxxx????xx????xxxx????xxxxxxxxx"));
typedef BOOL(__cdecl *lua_ToBoolean)(int lua_State, int idx);
lua_ToBoolean lua_toboolean = (lua_ToBoolean)unprotect(Memory::SigScan("\x55\x8B\xEC\xFF\x75\x0C\xFF\x75\x08\xE8\x00\x00\x00\x00\x83\xC4\x08\x8B\x48\x08\x85\xC9", "xxxxxxxxxx????xxxxxxxx"));
typedef const char*(__cdecl *Lua_ToLString)(int lua_State, int idx, size_t *len);
//Lua_ToLString lua_tolstring = (Lua_ToLString)unprotect(Memory::SigScan("\x55\x8B\xEC\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\xF8", "xxxxxxxxxxxxx????xx"));
//typedef int(__cdecl *Lua_next)(int lua_State, int idx);
//Lua_next lua_next = (Lua_next)unprotect(Memory::SigScan("\x55\x8B\xEC\x83\xEC\x18\x56\x8B\x75\x08\x57\xFF\x75\x0C", "xxxxxxxxxxxxxx"));
//typedef int(__cdecl *Lua_Type)(int State, int idx);
//Lua_Type lua_type = (Lua_Type)unprotect(Memory::SigScan("\x55\x8B\xEC\xFF\x75\x0C\xFF\x75\x08\xE8\x00\x00\x00\x00\x83\xC4\x08\x8B\x48\x08\x49", "xxxxxxxxxx????xxxxxxx"));
//typedef int(__cdecl *Lua_PushBoolean)(int State, bool Boolean);
//Lua_PushBoolean lua_pushboolean = (Lua_PushBoolean)unprotect(Memory::SigScan("\x55\x8B\xEC\x8B\x55\x08\x33\xC0\x39\x45\x0C", "xxxxxxxxxxx"));
//typedef int(__cdecl *Lua_Remove)(int lua_State, int idx);
//Lua_Remove lua_remove = (Lua_Remove)unprotect(Memory::SigScan("\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\x75\x08\x56\xE8\x00\x00\x00\x00\x83\xC4\x08\x8D\x48\x10", "xxxxxxxxxxxx????xxxxxx"));

void lua_getglobal(int L, const char *k) {
	lua_getfield(L, -10002, k);
}

void lua_setglobal(int L, const char *k) {
	lua_setfield(L, -10002, k);
}

void lua_emptystack(int L) {
	lua_settop(L, 0);
}

#define lua_isnil(L,n)          (lua_type(L, (n)) == LUA_TNIL)