#pragma once
#include <Windows.h>
#include "globals.h"
#include "memory.h"

#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)

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

typedef void(__stdcall* rgetfield)(DWORD rL, int idx, const char *k);
rgetfield r_lua_getfield = (rgetfield)unprotect(x(0x74BE20));
typedef char*(__stdcall* rtolstring)(DWORD rL, int idx, size_t *size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(x(0x74E210)));
typedef void(__fastcall* rsettop)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)(unprotect(x(0x74DFD0)));
typedef bool(__cdecl* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(x(0x74E150));
typedef void(__fastcall* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(x(0x74D2B0)));
typedef double(__thiscall* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(x(0x74d160)));
typedef void(__fastcall* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(x(0x74D1F0));
typedef int(__cdecl* pcall)(DWORD rL, DWORD, DWORD, DWORD);
pcall r_luad_pcall = (pcall)(unprotect(x(0x74cc10)));
typedef void(__cdecl* pushnil)(DWORD);
pushnil r_lua_pushnil = (pushnil)(unprotect(x(0x74d0f0)));
typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(unprotect(x(0x74ca90)));
typedef bool(__cdecl* rboolean)(unsigned int, bool);
rboolean r_lua_pushboolean = (rboolean)(unprotect(x(0x74cce0)));
typedef double(__cdecl* rtonumber)(DWORD, int);
rtonumber r_lua_tonumber = (rtonumber)(x(0x74e500));
typedef void(__stdcall* rpushcclosure)(DWORD rL, int fn, int non);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(unprotect(x(0x74cd60)));
typedef void(__cdecl *rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(x(0x74BAD0)));
typedef void*(__cdecl *rnewuserdata)(DWORD rL, int idx);
rnewuserdata r_lua_newuserdata = (rnewuserdata)(unprotect(x(0x74C9D0)));
typedef void*(__stdcall *rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(x(0x74C0F0)));
typedef void*(__cdecl *rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(x(0x74df40)));
typedef DWORD(__cdecl *rtype)(DWORD, int);
rtype r_lua_type = (rtype)(x(0x74E660));
typedef DWORD(__cdecl *rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)unprotect(x(0x74c8e0));
typedef DWORD(__cdecl *rref)(DWORD, DWORD);
#include "retcheck.h"
rref r_luaL_ref = (rref)(Ret::unprotect<DWORD>((byte*)(x(0x746E40))));
typedef void(__cdecl *rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(x(0x74d570));
typedef int(__cdecl *gettop)(DWORD);
gettop r_lua_gettop = (gettop)(x(0x74C320));
typedef void(__cdecl *rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)(unprotect(x(0x74cfd0)));
typedef void*(__cdecl *touserdata)(DWORD, int);
touserdata r_lua_touserdata =(touserdata)Retcheck::Unprotect(x(0x74E630), true);

/*
Sehchainfaker
Eternals sehchain fixed for windows7, windows 8,...
*/
void fakeChain(DWORD* chain)
{
	chain[1] = 0x1555555;
	if ((((DWORD*)chain[0])[1]) != NULL) {
		((DWORD*)chain[0])[1] = 0x1555555;
	}
}
void restoreChain(DWORD* chain, DWORD unk, DWORD nextUnk)
{
	chain[1] = unk;
	if ((((DWORD*)chain[0])[1]) != NULL) {
		((DWORD*)chain[0])[1] = nextUnk;
	}
}

/*
r_lua_pcall
r_lua_pcall we use this to see what error roblox returns if it returns the error we want we put a breakpoint on it
and rewrite it to our own functions sadly r_lua_pcall has a check sehchaincheck which i already bypassed!
*/

int r_lua_pcall(DWORD rL, int nargs, int nresults, int errfunc)
{
	DWORD* exceptionChain = (DWORD*)__readfsdword(0);
	DWORD unk = exceptionChain[1];
	((((DWORD*)exceptionChain[0])[1]) != NULL );
	{
		DWORD nextUnk = ((DWORD*)exceptionChain[0])[1];
		fakeChain(exceptionChain);
		int ret = r_luad_pcall(rL, nargs, nresults, errfunc);
		restoreChain(exceptionChain, unk, nextUnk);
		return ret;
	}

	fakeChain(exceptionChain);
	int ret = r_luad_pcall(rL, nargs, nresults, errfunc);
	restoreChain(exceptionChain, unk, 0);
	return ret;
}