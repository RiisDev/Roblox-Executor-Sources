#pragma once
#include <Windows.h>
#include "globals.h"
#include "memory.h"

#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)
#define r_lua_pushcfunction(rL, fn)	(r_lua_pushcclosure(rL,fn,0))


DWORD ADDRESS_PCALL = x(0x77A940);//find
DWORD ADDRESS_TOUSERDATA = x(0x77C3F0);//find
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

int top = 24, base = 16;
typedef DWORD _DWORD;

typedef int(__stdcall *rLuaS_newlstr)(int RL, const char *str, size_t l);
rLuaS_newlstr r_luaS_newlstr = (rLuaS_newlstr)x(0x775590);

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

typedef r_TValue*(__cdecl *rlua_index2adr)(int lst, int idx);
rlua_index2adr r_lua_index2adr = (rlua_index2adr)x(0x7713B0);

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


/*typedef int(__cdecl *gettop)(DWORD);
gettop r_lua_gettop = (gettop)(x(0x77F740));
typedef void(__cdecl* pushnil)(DWORD);
pushnil r_lua_pushnil = (pushnil)(unprotect(x(0x7804F0)));
typedef void(__cdecl* rsettop)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)(unprotect(x(0x7813E0)));
typedef bool(__cdecl* rboolean)(unsigned int, bool);
rboolean r_lua_pushboolean = (rboolean)(unprotect(x(0x7800E0)));
typedef void(__fastcall* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(x(0x7806B0)));
typedef void(__stdcall* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(x(0x7805F0));*/
typedef int(__fastcall* rgetfield)(DWORD rL, int idx, const char *k);
rgetfield r_lua_getfield = (rgetfield)unprotect(x(0x779B70));
typedef char*(__stdcall* rtolstring)(DWORD rL, int idx, size_t *size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(x(0x77BFC0)));
typedef bool(__cdecl* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(x(0x77BEC0));
typedef double(__cdecl* pushnumber)(DWORD rL, double idx);//warning
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(x(0x77AE80)));
typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(unprotect(x(0x77A7C0)));
typedef double(__cdecl* rtonumber)(DWORD, int);
rtonumber r_lua_tonumber = (rtonumber)(x(0x77C2C0));
typedef void(__fastcall* rpushcclosure)(DWORD rL, int fn, int non);//stdcall?
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(unprotect(x(0x77AA90)));
typedef void(__cdecl *rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(x(0x779820)));
typedef int*(__cdecl *rnewuserdata)(DWORD rL, int idx);
rnewuserdata r_lua_newuserdata = (rnewuserdata)(unprotect(x(0x77A700)));
typedef void*(__fastcall *rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(x(0x779E40)));
typedef void*(__cdecl *rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(x(0x77BCB0)));
typedef DWORD(__cdecl *rtype)(DWORD, int);
rtype r_lua_type = (rtype)(x(0x77C420));
typedef DWORD(__cdecl *rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)unprotect(x(0x77A610));
typedef DWORD(__cdecl *rref)(DWORD, DWORD);
#include "retcheck.h"
rref r_luaL_ref = (rref)(Ret::unprotect<DWORD>((byte*)(x(0x774BA0))));
typedef void(__cdecl *rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(x(0x77B280));
typedef void(__cdecl *rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)(unprotect(x(0x77ACF0)));
//typedef int(__cdecl *rsetfield)(DWORD, int, const char *k);
//rsetfield r_lua_setfield = (rsetfield)(unprotect(x(0x77FDF0)));
//typedef void(__cdecl *rawseti)(DWORD rL, DWORD, DWORD);
//rawseti r_lua_rawseti = (rawseti)(unprotect(x(0x780AD0)));
//typedef int(__cdecl *rgetmetafield)(DWORD rL, DWORD, const char*);
//rgetmetafield r_luaL_getmetafield = (rgetmetafield)(x(0x779DE0));
/*typedef void*(__cdecl *rsetmetatable)(DWORD rL, int idx);
rsetmetatable r_lua_setmetatable = (rsetmetatable)(unprotect(x(0x77A5F0)));
typedef void*(__cdecl *rremove)(DWORD rL, DWORD);
rremove r_lua_remove = (rremove)(unprotect(x(0x77A040)));
typedef void*(__cdecl *rgettable)(DWORD rL, DWORD);
rgettable r_lua_gettable = (rgettable)(unprotect(x(0x778B00)));
typedef int(__cdecl *rtopointer)(DWORD, DWORD);
rtopointer r_lua_topointer = (rtopointer)(x(0x77ADD0));*/

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