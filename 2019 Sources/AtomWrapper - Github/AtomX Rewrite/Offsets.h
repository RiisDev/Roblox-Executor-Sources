#pragma once
#include <Windows.h>
#include "Includes.h"
#include "memory.h"

#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
//#define r_lua_gettop(rL) (*(DWORD *)(rL + 32) - *(DWORD *)(rL + 8)) >> 4
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)

/*
Aobscan part
Credits to austin for this scanning
making our own soon
*/
DWORD ADDRESS_PCALL = x(0x811C70);//
/*DWORD ADDRESS_TYPE = x(0x73CDD0);//
DWORD ADDRESS_GETFIELD = x(0x73A5C0);//
DWORD ADDRESS_SETFIELD = x(0x73C380);//
DWORD ADDRESS_SETTABLE = x(0x73C6C0);//(uintptr_t)sigscan::scan("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x8B\xF8\x8B\xCA\x8D\x59\x01\x90\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\x6A\x01", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxx????xx");
DWORD ADDRESS_GETTABLE = x(0x73AA30);//(uintptr_t)sigscan::scan("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\x75\x08\x56\xE8\x00\x00\x00\x00\x8B\x4E\x20\x6A\x01", "xxxxxxxxxxxx????xxxxx");
DWORD ADDRESS_SETTOP = x(0x73c750);//
DWORD ADDRESS_CREATETABLE = x(0x73A210);//
DWORD ADDRESS_NEXT = x(0x73B200);//
DWORD ADDRESS_NEWTHREAD = x(0x73B050);//(uintptr_t)sigscan::scan("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x51\x56\x8B\x75\x08\x8B\x46\x1C", "xxxxxx????xx????xxxx????xxxxxxxx");
DWORD ADDRESS_TOBOOLEAN = x(0x73C8D0);//
DWORD ADDRESS_TOLSTRING = x(0x73C990);//
DWORD ADDRESS_TONUMBER = x(0x73CC60);//
DWORD ADDRESS_PUSHVALUE = x(0x73BA10);//
DWORD ADDRESS_PUSHSTRING = x(0x73B950);//
DWORD ADDRESS_PUSHCCLOSURE = x(0x73B4D0);//
DWORD ADDRESS_PUSHBOOLEAN = x(0x73b450);//
DWORD ADDRESS_PUSHNIL = x(0x73b850);//
DWORD ADDRESS_PUSHLIGHTUSERDATA = x(0x73b730);// (uintptr_t)sigscan::scan("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x8B\x55\x08\x8B\x45\x0C\x8B\x4A\x20", "xxxxxxxxxxxx");
//DWORD ADDRESS_TOPOINTER = x();
DWORD ADDRESS_NEWUSERDATA = x(0x73B140); //
DWORD ADDRESS_GETMETATABLE = x(0x73A890); //
//DWORD ADDRESS_SINGLETON = x();
DWORD ADDRESS_REF = x(0x735490); //
DWORD ADDRESS_RAWGETI = x(0x73BCD0);
DWORD ADDRESS_UNREF = x(0x735770);*/
DWORD ADDRESS_TOUSERDATA = x(0x8135A0);
//DWORD ADDRESS_PUSHNUMBER = x(0x73B8C0);


/*
Typedefs part
All our typedefs that rvx uses
*/
/*auto rr_lua_gettop = (int(__cdecl*)(DWORD))x(0x73AAB0);
auto r_lua_getmetatable = (int(__fastcall*)(DWORD, int))Retcheck::Unprotect(ADDRESS_GETMETATABLE, true);
auto r_lua_type = (int(__cdecl*)(DWORD, int))ADDRESS_TYPE;
auto r_lua_toboolean = (bool(__cdecl*)(DWORD, int))ADDRESS_TOBOOLEAN;
auto r_lua_tolstring = (char*(__cdecl*)(DWORD, int, std::size_t*))Retcheck::Unprotect(ADDRESS_TOLSTRING, true);
auto r_lua_tonumber = (double(__stdcall*)(DWORD, int))ADDRESS_TONUMBER;
//auto r_lua_topointer = (const void*(__cdecl*)(DWORD, int))ADDRESS_TOPOINTER;
auto r_lua_touserdata = (void*(__cdecl*)(DWORD, int))Retcheck::Unprotect(ADDRESS_TOUSERDATA, true);
auto r_lua_pushvalue = (void(__stdcall*)(DWORD, int))Retcheck::Unprotect(ADDRESS_PUSHVALUE, true);
auto r_lua_pushlightuserdata = (void(__cdecl*)(DWORD, void*))Retcheck::Unprotect(ADDRESS_PUSHLIGHTUSERDATA, true);
auto r_lua_pushboolean = (void(__cdecl*)(DWORD, int))Retcheck::Unprotect(ADDRESS_PUSHBOOLEAN, true);
auto r_lua_pushnumber = (void(__stdcall*)(DWORD, double))Retcheck::Unprotect(ADDRESS_PUSHNUMBER, true);
auto r_lua_pushnil = (void(__cdecl*)(DWORD))Retcheck::Unprotect(ADDRESS_PUSHNIL, true);
auto r_lua_pushstring = (void(__fastcall*)(DWORD, const char*))ADDRESS_PUSHSTRING;
auto r_lua_pushcclosure = (void(__stdcall*)(DWORD, int, int))Retcheck::Unprotect(ADDRESS_PUSHCCLOSURE, true);
auto r_lua_getfield = (void*(__stdcall*)(DWORD, int, const char*))Retcheck::Unprotect(ADDRESS_GETFIELD, true);
auto r_lua_setfield = (void(__cdecl*)(DWORD, int, const char*))Retcheck::Unprotect(ADDRESS_SETFIELD, true);
auto r_lua_settable = (void(__cdecl*)(DWORD, int))Retcheck::Unprotect(ADDRESS_SETTABLE, true);
auto r_lua_gettable = (void(*__cdecl)(DWORD, int))Retcheck::Unprotect(ADDRESS_GETTABLE, true);
auto r_lua_settop = (void(__cdecl*)(DWORD, int))Retcheck::Unprotect(ADDRESS_SETTOP, true);
auto r_lua_createtable = (void(__cdecl*)(DWORD, int, int))Retcheck::Unprotect(ADDRESS_CREATETABLE, true);
auto r_lua_next = (int(__cdecl*)(DWORD, int))Retcheck::Unprotect(ADDRESS_NEXT, true);
auto r_lua_newuserdata = (void*(__cdecl*)(DWORD, std::size_t))Retcheck::Unprotect(ADDRESS_NEWUSERDATA, true);
auto r_lua_newthread = (DWORD(__cdecl *)(DWORD))Retcheck::Unprotect(ADDRESS_NEWTHREAD, true);;
auto r_luaL_ref = (int(__cdecl*)(DWORD, int))ADDRESS_REF;
auto r_luaL_unref = (void(__cdecl*)(DWORD, int, int))ADDRESS_UNREF;
auto r_lua_rawgeti = (int(__cdecl*)(DWORD, int, int))Retcheck::Unprotect(ADDRESS_RAWGETI, true);
auto r_lua_rawseti = (int(__cdecl*)(DWORD, int, int))ADDRESS_RAWGETI;*/
auto r_luad_pcall = (int(__cdecl*)(DWORD, int, int, int))Retcheck::Unprotect(ADDRESS_PCALL, true);
auto r_lua_touserdata = (void*(__cdecl*)(DWORD, int))Retcheck::Unprotect(ADDRESS_TOUSERDATA, true);

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

int top = 32, base = 8;
typedef DWORD _DWORD;

typedef int(__cdecl*rLuaS_newlstr)(int RL, const char *str, size_t l);
rLuaS_newlstr r_luaS_newlstr = (rLuaS_newlstr)x(0x80C4E0);

union r_Value
{
	int b;
	double n;
	void* p;
	void* gc;
};

struct r_TValue
{
	r_Value value;
	int tt;
};

typedef r_TValue*(__fastcall*rlua_index2adr)(int lst, int idx);
rlua_index2adr r_lua_index2adr = (rlua_index2adr)x(0x808030);

void r_lua_pushnil(DWORD a1) {
	*(_DWORD *)(*(_DWORD *)(a1 + top) + 8) = 0;
	*(_DWORD *)(a1 + top) += 16;
}

void r_lua_pushboolean(DWORD a1, int a2) {
	_DWORD *v2;
	v2 = *(_DWORD **)(a1 + top);
	*v2 = a2 != 0;
	v2[2] = 3;
	*(_DWORD *)(a1 + top) += 16;
}

void r_lua_pushstring(int rState, const char *s) {
	int v3 = *(DWORD *)(rState + top);
	*(DWORD *)v3 = r_luaS_newlstr(rState, s, strlen(s));
	*(DWORD *)(v3 + 8) = R_LUA_TSTRING;
	*(DWORD *)(rState + top) += 16;
}

void r_lua_settop(int a1, int a2) {
	int i;
	if (a2 < 0)
	{
		*(_DWORD *)(a1 + top) += 16 * a2 + 16;
	}
	else
	{
		for (i = 16 * a2; *(_DWORD *)(a1 + top) < (unsigned int)(i + *(_DWORD *)(a1 + base)); *(_DWORD *)(a1 + top) += 16)
			*(_DWORD *)(*(_DWORD *)(a1 + top) + 8) = 0;
		*(_DWORD *)(a1 + top) = i + *(_DWORD *)(a1 + base);
	}
}

void r_lua_pushvalue(DWORD a1, int a2)
{
	r_TValue *p = r_lua_index2adr(a1, a2);
	int v3 = *(DWORD *)(a1 + top);
	*(DWORD *)v3 = *(DWORD *)p;
	*(DWORD *)(v3 + 4) = *(DWORD *)(p + 4);
	*(DWORD *)(v3 + 8) = p->tt;
	*(DWORD *)(a1 + top) += 16;
}

int r_lua_gettop(DWORD a1)
{
	return (*(_DWORD *)(a1 + top) - *(_DWORD *)(a1 + base)) >> 4;
}

typedef void(__stdcall* rgetfield)(DWORD rL, int idx, const char *k);
rgetfield r_lua_getfield = (rgetfield)unprotect(x(0x810ED0));

typedef char*(__fastcall* rtolstring)(DWORD rL, int idx, size_t *size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(x(0x813330)));

typedef bool(__cdecl* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(x(0x813270));

typedef double(__thiscall* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(x(0x8121C0)));

typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(unprotect(x(0x811AF0)));

typedef double(__stdcall* rtonumber)(DWORD, int);
rtonumber r_lua_tonumber = (rtonumber)(x(0x813460));

typedef void(__stdcall* rpushcclosure)(DWORD rL, int fn, int non);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(unprotect(x(0x811DC0)));

typedef void(__cdecl *rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(x(0x810B60)));

typedef void*(__cdecl *rnewuserdata)(DWORD rL, int idx);
rnewuserdata r_lua_newuserdata = (rnewuserdata)(unprotect(x(0x811A30)));

typedef void*(__cdecl*rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(x(0x8111A0)));

typedef void*(__cdecl *rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(x(0x813060)));

typedef DWORD(__cdecl *rtype)(DWORD, int);
rtype r_lua_type = (rtype)(x(0x8135D0));

typedef DWORD(__cdecl *rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)unprotect(x(0x811940));

#include "retcheck.h"
typedef DWORD(__cdecl *rref)(DWORD, DWORD);
rref r_luaL_ref = (rref)(Ret::unprotect<DWORD>((byte*)(x(0x80BA40))));

typedef void(__cdecl *rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(x(0x8125B0));

typedef void(__cdecl *rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)(unprotect(x(0x812030)));


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

int r_lua_pcall(DWORD rL, int nargs, int nresults, int errfunc)
{
	DWORD* exceptionChain = (DWORD*)__readfsdword(0);
	DWORD unk = exceptionChain[1];
	((((DWORD*)exceptionChain[0])[1]) != NULL);
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