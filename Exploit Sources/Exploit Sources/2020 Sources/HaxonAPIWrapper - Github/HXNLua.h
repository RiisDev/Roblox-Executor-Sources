#pragma once
#include <Windows.h>

#include "HXNRetcheck.h"
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


/* Eternals Retcheck*/
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
/* End Off the RetCheck!*/

/* aslr, and etc.*/
#define HXN_ASLR(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))
#define Declare(address, returnValue, callingConvention, ...) (returnValue(callingConvention*)(__VA_ARGS__))(unprotect(HXN_ASLR(address)))
/* Declare Roblox Functions!*/


#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_newtable(rL) r_lua_createtable(rL, 0, 0)
#define r_lua_isnil(L,n)		(r_lua_type(L, (n)) == 0)
#define abs_index(L, i)		((i) > 0 || (i) <= -10000 ? (i) : \
					r_lua_gettop(L) + (i) + 1)


//#define R_LUA_TUSERDATA 7
//#define R_LUA_TTABLE 5
//#define R_LUA_TFUNCTION 6
//#define R_LUA_TSTRING 4
//#define R_LUA_TBOOLEAN 1
//#define R_LUA_TNIL  0
//#define R_LUA_TTHREAD 8
//#define R_LUA_TNUMBER 3
//#define R_LUA_TLIGHTUSERDATA 2

int R_LUA_TNIL = 0;
int R_LUA_TBOOLEAN = 0;
int R_LUA_TNUMBER = 0;
int R_LUA_TSTRING = 0;
int R_LUA_TUSERDATA = 0;
int R_LUA_TTABLE = 0;
int R_LUA_TTHREAD = 0;
int R_LUA_TFUNCTION = 0;


DWORD HXN_STATE;
lua_State* HXN_LSTATE;

#define off_top 28
#define off_base 24


auto r_luaH_new = Declare(0x75f540, int, __cdecl, int, int, int); // needed for the hook, you tards if you even get the src (lol)

typedef int(__cdecl * sandboxthread)(int, int, int);
sandboxthread SandBoxThread = (sandboxthread)HXN_ASLR(0x6d5d80);

typedef void(__cdecl* rgetfield)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)unprotect(HXN_ASLR(0x764b00));

typedef void(__fastcall* rsetfield)(DWORD rL, int idx, const char* k);
rsetfield r_lua_setfield = (rsetfield)unprotect(HXN_ASLR(0x766c10));//Updated

typedef char* (__fastcall* rtolstring)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(HXN_ASLR(0x7673a0)));//Updated

typedef bool(__cdecl* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(HXN_ASLR(0x7672e0));//Updated

typedef void(__fastcall* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(HXN_ASLR(0x766280)));//Updated

typedef double(__cdecl* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(HXN_ASLR(0x7660b0)));//Updated

typedef void(__fastcall* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(HXN_ASLR(0x766130));//

typedef int(__cdecl* rrr)(DWORD a1, int a2, int a3, int a4);
rrr r_lua_pcall = (rrr)(Retcheck::unprotect((byte*)(HXN_ASLR(0x765af0))));

//typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
//next2 r_lua_next = (next2)(Retcheck::unprotect((byte*)(x(0x766A80))));//Updated

double r_lua_tonumber(DWORD rL, int idx)
{
	return ((double(__cdecl*)(DWORD, int, int))HXN_ASLR(0x7674e0))(rL, idx, 0);//Updated
}

typedef void(__stdcall* rpushcclosure)(DWORD rL, int fn, int non);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(Retcheck::unprotect((byte*)(HXN_ASLR(0x765c40))));//Updated

typedef void(__cdecl* rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(HXN_ASLR(0x764800)));//Updated

typedef DWORD(__cdecl* rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)unprotect(HXN_ASLR(0x7655f0));//Updated

void* r_lua_newuserdata(DWORD rL, size_t size)//Updated
{
	return ((void* (__cdecl*)(DWORD rL, size_t size, int))unprotect(HXN_ASLR(0x765730)))(rL, size, 0);
}

typedef void(__cdecl* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(HXN_ASLR(0x766540));//Updated

typedef void(__cdecl* rrawseti)(DWORD, DWORD, DWORD);
rrawseti r_lua_rawseti = (rrawseti)unprotect(HXN_ASLR(0x7666d0));//Updated

typedef void* (__cdecl* rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(HXN_ASLR(0x764f80)));//Updated

typedef void* (__cdecl* rsetmetatable)(DWORD rL, int idx);
rsetmetatable r_lua_setmetatable = (rsetmetatable)(unprotect(HXN_ASLR(0x766e90)));//Updated

auto r_lua_touserdata = (int(__cdecl*)(DWORD, int))HXN_ASLR(0x767620);//Updated;//(int(__cdecl*)(DWORD, int))x(0x768860);//Updated

typedef DWORD(__cdecl* rtype)(DWORD, int);
rtype r_lua_type = (rtype)(HXN_ASLR(0x767680));//Updated

auto r_lua_gettable = Declare(0x765130, void, __cdecl, int a1, int idx);

typedef void* (__cdecl* rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(HXN_ASLR(0x7670d0)));//Updated

typedef void(__cdecl* rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)(unprotect(HXN_ASLR(0x765f10)));//Updated

typedef int(__cdecl* rprint)(int a, const char* e, ...);
rprint SingletonPrint = (rprint)(HXN_ASLR(0x5a2ec0));



typedef union
{
	//r_GCObject* gc;
	void* p;
	double n;
	int b;
} r_Value;

#define r_TValuefields	r_Value value; int tt

typedef struct r_TValue
{
	r_TValuefields;
} r_TValue;

typedef r_TValue* r_StkId;

const r_TValue r_luaO_nilobject_;

auto r_lua_index2adr = Declare(0x75d260, r_TValue*, __cdecl, DWORD a1, int idx);


void r_lua_setreadonly(DWORD rL, int idx, byte v)
{
	r_TValue* obj = r_lua_index2adr(rL, idx);
	*(byte*)(obj + 6) = v;
}

byte r_lua_isreadonly(DWORD rL, int idx)
{
	r_TValue* obj = r_lua_index2adr(rL, idx);
	return *(byte*)(obj + 6);
}


void SetIdentity(int rls, int identity) {
	int unk[] = { NULL, NULL };
	SandBoxThread(rls, identity, (int)unk);
}

int r_lua_gettop(int a1)
{
	return (*(DWORD*)(a1 + off_top) - *(DWORD*)(a1 + off_base)) >> 4;
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

typedef void(__fastcall* rsettop)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)(unprotect(HXN_ASLR(0x767160)));

typedef void(__cdecl* pushnil)(DWORD);
pushnil r_lua_pushnil = (pushnil)(unprotect(HXN_ASLR(0x766040)));

typedef int(__cdecl* rboolean)(unsigned int, int);
rboolean r_lua_pushboolean = (rboolean)(unprotect(HXN_ASLR(0x765bc0)));


//DWORD RetCheck1 = 0x1F878B8;
//DWORD RetCheck2 = 0x1F8201C;
//DWORD RetcheckFunctionPointer = 0x1F87888;
//DWORD Flag1, Flag2, Func, OldProtect;
//
//void RetCheckON() {
//	Flag1 = *(long*)HXN_ASLR(RetCheck1);
//	Flag2 = *(long*)HXN_ASLR(RetCheck2);
//	Func = *(long*)HXN_ASLR(RetcheckFunctionPointer);
//	VirtualProtect((void*)HXN_ASLR(RetcheckFunctionPointer), 1, PAGE_EXECUTE_READWRITE, &OldProtect);
//	memcpy((void*)HXN_ASLR(RetcheckFunctionPointer), "\xC2", 1);
//}
//
//void RetCheckOFF() {
//	*(long*)HXN_ASLR(RetCheck1) = Flag1;
//	*(long*)HXN_ASLR(RetCheck2) = Flag2;
//	*(long*)HXN_ASLR(RetcheckFunctionPointer) = Func;
//	VirtualProtect((void*)HXN_ASLR(RetcheckFunctionPointer), 1, OldProtect, &OldProtect);
//}

typedef DWORD(__cdecl* rref)(DWORD, DWORD);
rref r_luaL_ref = (rref)(Retcheck::unprotect((BYTE*)(HXN_ASLR(0x75fe50)))); // luaL_ref offset

typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(Retcheck::unprotect((BYTE*)(HXN_ASLR(0x765850))));



//typedef int(__cdecl* rtointeger)(int, int, int);
//rtointeger r_lua_tointeger = (rtointeger)(HXN_ASLR(0x767560));
//typedef int(__cdecl* robjlen)(int, int);
//robjlen r_lua_objlen = (robjlen)unprotect(HXN_ASLR(0x765c70));
//
//#define abs_index(L, i)		((i) > 0 || (i) <= -10000 ? (i) : \
//					r_lua_gettop(L) + (i) + 1)
//#define r_lua_isnil(L,n)		(r_lua_type(L, (n)) == 0)
//#define r_LUA_REFNIL      (-1)
//
//int r_luaL_ref(int L, int t) {//just like vanilla lual_ref
//	int ref;
//	t = abs_index(L, t);
//	if (r_lua_isnil(L, -1)) {
//		r_lua_pop(L, 1);  /* remove from stack */
//		return r_LUA_REFNIL;  /* `nil' has a unique fixed reference */
//	}
//	r_lua_rawgeti(L, t, 0);  /* get first free element */
//	ref = (int)r_lua_tointeger(L, -1, 0);  /* ref = t[FREELIST_REF] */
//	r_lua_pop(L, 1);  /* remove it from stack */
//	if (ref != 0) {  /* any free element? */
//		r_lua_rawgeti(L, t, ref);  /* remove it from list */
//		r_lua_rawseti(L, t, 0);  /* (t[FREELIST_REF] = t[ref]) */
//	}
//	else {  /* no free elements */
//		ref = (int)r_lua_objlen(L, t);
//		ref++;  /* create new reference */
//	}
//	r_lua_rawseti(L, t, ref);
//	return ref;
//}


//int r_luaL_ref(DWORD r_l, int idx)
//{
//	RetCheckON();
//	int ReturnUnprotected = RefProtected(r_l, idx);
//	RetCheckOFF();
//	return ReturnUnprotected;
//}
//
//int r_lua_next(DWORD a1, signed int a2) {
//	RetCheckON();
//	int ReturnUnprotected = NextProtected(a1, a2);
//	RetCheckOFF();
//	return ReturnUnprotected;
//}
