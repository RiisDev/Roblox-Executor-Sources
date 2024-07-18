#pragma once
#include <Windows.h>

#include "retcheck.h"
#pragma once
extern "C" {
#include "Lua\lua.h"
#include "Lua\ldo.h"
#include "Lua\lapi.h"
#include "Lua\lualib.h"
#include "Lua\lstate.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lapi.h"
#include "Lua\lfunc.h"
#include "Lua/lopcodes.h"
#include "Lua\lobject.h"
}
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

#define x(Address) (Address - 0x400000 + (DWORD)GetModuleHandle(NULL))
#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(unprotect(x(address)))

#define R_LUA_TNIL 0
#define R_LUA_TLIGHTUSERDATA 2
#define R_LUA_TNUMBER 3
#define R_LUA_TBOOLEAN 1
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 7
#define R_LUA_TFUNCTION 8
#define R_LUA_TTABLE 6
#define R_LUA_TUSERDATA 5
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10

#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)
#define r_lua_isnil(L,n)		(r_lua_type(L, (n)) == 0)
#define abs_index(L, i)		((i) > 0 || (i) <= -10000 ? (i) : \
					r_lua_gettop(L) + (i) + 1)

union r_lua_Value {
	int b;
	double n;
	void* p;
	void* gc;
};

struct r_lua_TValue {
	r_lua_Value value;
	int tt;
};

DWORD RLS;
lua_State* LS;

int top = 24; int base = 16;

typedef int(__cdecl* rError) (int a1, const char* a2);
rError Print = (rError)unprotect(x(0x5ed910));

typedef r_lua_TValue* (__cdecl* rrindex2)(int lst, int idx);
rrindex2 r_lua_index2 = (rrindex2)(x(0x7b0e80));

typedef DWORD(__cdecl* rref)(DWORD, DWORD);
rref r_luaL_ref = (rref)(Ret::unprotect<DWORD>((byte*)(x(0x7b3bb0))));

typedef int(__cdecl* sandboxthread)(int, int, int);
sandboxthread SandBoxThread = (sandboxthread)x(0x71ae60);

typedef void(__fastcall* rgetfield)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)unprotect(x(0x7b8BF0));

typedef void(__stdcall* rsetfield)(DWORD rL, int idx, const char* k);
rsetfield r_lua_setfield = (rsetfield)unprotect(x(0x7bac00));//Updated

typedef char* (__stdcall* rtolstring)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(x(0x7bB3A0)));//Updated

typedef bool(__cdecl* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(x(0x7bb2e0));//Updated
/////////////////////////////////////////////////////////////////////////////
typedef void(__cdecl* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(x(0x7bA250)));//Updated

typedef double(__stdcall* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(x(0x7ba060)));//Updated

typedef void(__fastcall* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(x(0x7bA0F0));//

typedef int(__cdecl* rrr)(DWORD a1, int a2, int a3, int a4);
rrr r_lua_pcall = (rrr)(Ret::unprotect<DWORD>((byte*)(x(0x7b9ab0))));

typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(Ret::unprotect<DWORD>((byte*)(x(0x7b9820))));//Updated

double r_lua_tonumber(DWORD rL, int idx)
{
	return ((double(__cdecl*)(DWORD, int, int))x(0x7bb4e0))(rL, idx, 0);//Updated
}

typedef void(__cdecl* rpushcclosure)(DWORD rL, int fn, int non);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(Ret::unprotect<DWORD>((byte*)(x(0x7b9C00))));//Updated

typedef void(__cdecl* rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(x(0x7b87B0)));//Updated

typedef DWORD(__cdecl* rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)unprotect(x(0x7b95C0));//Updated

void* r_lua_newuserdata(DWORD rL, size_t size)//Updated
{
	return ((void* (__cdecl*)(DWORD rL, size_t size, int))unprotect(x(0x7b9700)))(rL, size, 0);
}

typedef void(__cdecl* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(x(0x7ba510));//Updated

typedef void(__cdecl* rrawseti)(DWORD, DWORD, DWORD);
rrawseti r_lua_rawseti = (rrawseti)unprotect(x(0x7bA6A0));//Updated

typedef void* (__cdecl* rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(x(0x7b8f80)));//Updated

typedef void* (__cdecl* rsetmetatable)(DWORD rL, int idx);
rsetmetatable r_lua_setmetatable = (rsetmetatable)(unprotect(x(0x7bAE80)));//Updated

auto r_lua_touserdata = (int(__cdecl*)(DWORD, int))x(0x7bb620);//Updated

typedef DWORD(__cdecl* rtype)(DWORD, int);
rtype r_lua_type = (rtype)(x(0x7bb680));//Updated

auto r_lua_gettable = Declare(0x7b9130, void, __cdecl, int a1, int idx);

typedef void* (__cdecl* rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(x(0x7bB0D0)));//Updated

typedef void(__cdecl* rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)(unprotect(x(0x7b9ED0)));//Updated

typedef void(__cdecl* rtopointer)(int);
rtopointer r_lua_topointer = (rtopointer)(unprotect(x(0x7bb570)));//Updated


void SetIdentity(int rls, int identity) {
	int unk[] = { NULL, NULL };
	SandBoxThread(rls, identity, (int)unk);
}

int r_lua_gettop(int a1)
{
	return (*(DWORD*)(a1 + top) - *(DWORD*)(a1 + base)) >> 4;
}

static TValue* lua_index2adr(lua_State* L, int idx) {
	if (idx > 0) {
		TValue* o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue*, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure* func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure* func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue*, luaO_nilobject);
	}
	}
}

void r_lua_settop(int a1, int a2) {
	int i;
	if (a2 < 0)
	{
		*(DWORD*)(a1 + top) += 16 * a2 + 16;
	}
	else
	{
		for (i = 16 * a2; *(DWORD*)(a1 + top) < (unsigned int)(i + *(DWORD*)(a1 + base)); *(DWORD*)(a1 + top) += 16)
			* (DWORD*)(*(DWORD*)(a1 + top) + 8) = 0;
		*(DWORD*)(a1 + top) = i + *(DWORD*)(a1 + base);
	}
}

//void r_lua_pushnil(DWORD a1) {
//	*(DWORD*)(*(DWORD*)(a1 + top) + 8) = 0;
//	*(DWORD*)(a1 + top) += 16;
//}

auto r_lua_pushnil = Declare(0x7b9FF0, const char*, __cdecl, int);

//void r_lua_pushboolean(DWORD a1, int a2) {
//	DWORD* v2;
//	v2 = *(DWORD * *)(a1 + top);
//	*v2 = a2 != 0;
//	v2[2] = 3;
//	*(DWORD*)(a1 + top) += 16;
//}

auto r_lua_pushboolean = Declare(0x7b9b80, const char*, __cdecl, int, int);