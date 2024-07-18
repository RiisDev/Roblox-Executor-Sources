#pragma once
#include <stdexcept>
#include <tuple>
#include <typeinfo>
#include <windows.h>
#include <strsafe.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>

#include "Memory.h"
#include "RetCheck.h"

extern "C" {
#include "Lua\lua.h"
#include "Lua\lapi.h"
#include "Lua\lualib.h"
#include "Lua\lstate.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lapi.h"
#include "Lua\lfunc.h"
#include "Lua\lobject.h"
}


#define R_LUA_TNONE	(-1)
#define R_LUA_TNIL 0 
#define R_LUA_TLIGHTUSERDATA 1
#define R_LUA_TNUMBER 2
#define R_LUA_TBOOLEAN 3
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 5
#define R_LUA_TFUNCTION 6
#define R_LUA_TTABLE 7
#define R_LUA_TUSERDATA 8
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10


typedef DWORD _DWORD;


union r_l_Value
{
	int b;
	double n;
	void* p;
	void* gc;
};

struct r_l_TValue
{
	r_l_Value value;
	int tt;
};

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

#define R_LUA_REGISTRYINDEX 0xFFFFD8F0
#define ASLR(OFFSET) (OFFSET - 0x400000 + (DWORD)GetModuleHandle(NULL))

DWORD ScriptContext;
lua_State* LS;
DWORD RLS;
int ContentProvider; //used in GetObjects
std::vector<DWORD> bpint3;
int top = 12, base = 16;


//lua_settable - 0x77ed60 __cdecl
//lua_gettable - 0x77d0f0 __cdecl


DWORD ScriptContextVFT_Addy = ASLR(0x1868EF0);  //updated 24/01/2019

DWORD GetField_Addy = (0x7D4890); //updated 24/01/2019
DWORD SetField_Addy = (0x7D6680); //updated 24/01/2019
DWORD Gettable_Addy = (0x7D4D10); // updated  this is lua_gettable 24/01/2019
DWORD Settable_Addy = (0x7D69C0); //updated   this is lua_settable 24/01/2019

DWORD CreateTable_Addy = (0x7d4550); //updated 24/01/2019
DWORD GetMetaTable_Addy = (0x7D4B60); //updated 24/01/2019
DWORD SetMetaTable_Addy = (0x7D6830); //updated 24/01/2019
DWORD ToLString_Addy = (0x7D6C90); //updated 24/01/2019

//:thonk: kEk     

DWORD GetMetaField_Addy = (0x7CF390); // updated 24/01/2019
DWORD Remove_Addy = (0x7D6270);// updated 24/01/2019
//DWORD ToPointer_Addy = (0x7820b0); gay

DWORD PushNumber_Addy = (0x7D5BC0); //updated 24/01/2019
DWORD PushLightUserData_Addy = (0x7d5a30); //updated 24/01/2019
DWORD PushCClosure_Addy = (0x7d57e0); //updated 24/01/2019

DWORD ToNumber_Addy = (0x7D6F80); //updated 24/01/2019
DWORD ToBoolean_Addy = (0x7D6BD0); //updated 24/01/2019
DWORD ToUserData_Addy = (0x7D70C0); //updated 24/01/2019

DWORD NewUserData_Addy = (0x7d5450); //updated 24/01/2019
DWORD RawGetI_Addy = (0x7D5FC0); //updated 24/01/2019
DWORD RawSetI_Addy = (0x7D6140); //updated 24/01/2019
DWORD PCall_Addy = (0x7D5690); //updated 24/01/2019
DWORD NewThread_Addy = (0x7d5360); //updated 24/01/2019
DWORD LRef_Addy = (0x7cf7e0); //updated 24/01/2019
DWORD Next_Addy = (0x7D5510); //updated 24/01/2019
DWORD Type_Addy = (0x7D70F0); //updated 24/01/2019
DWORD NewLStr_Addy = (0x7d0280);// updated 24/01/2019
DWORD Index2_Addy = (0x7CC0A0); //updated 24/01/2019


typedef void(__fastcall* XR_GetField)(DWORD RLS, int idx, const char *k);
typedef int(__fastcall*XR_SetField)(DWORD RLS, int idx, const char *k);
typedef void*(__cdecl* XR_GetTable)(DWORD RLS, int index);
typedef void*(__cdecl *XR_SetTable)(DWORD RLS, int idx);

typedef void(__cdecl *XR_CreateTable)(DWORD RLS, int num, int fix);
typedef void*(__cdecl *XR_GetMetaTable)(DWORD RLS, int idx);
typedef int(__cdecl *XR_SetMetaTable)(DWORD RLS, int index);
typedef char*(__fastcall* XR_TolString)(DWORD RLS, int idx, size_t *size);
typedef int(__cdecl* XR_GetMetaField)(DWORD RLS, int obj, const char *e);
typedef void*(__cdecl* XR_Remove)(DWORD RLS, int idx);
//typedef int*(__cdecl* XR_ToPointer)(DWORD RLS, int index);

typedef double(__cdecl * XR_PushNumber)(DWORD RLS, double idx);
typedef void(__cdecl *XR_PushLightUserData)(DWORD RLS, void* p);
typedef void(__cdecl* XR_PushCClosure)(DWORD RLS, int fn, int non);

typedef double(__stdcall* XR_ToNumber)(DWORD RLS, int idx);
typedef bool(__cdecl* XR_ToBoolean)(DWORD RLS, bool idx);
typedef void*(__cdecl *XR_ToUserData)(DWORD, int idx);

typedef void*(__cdecl *XR_NewUserData)(DWORD RLS, int idx);
typedef void(__cdecl *XR_RawGetI)(DWORD RLS, DWORD index, DWORD n);
typedef void(__cdecl* XR_RawSetI)(DWORD RLS, int index, int n);
typedef int(__cdecl* XR_PCall)(DWORD RLS, DWORD nargs, DWORD nresults, DWORD errfunc);
typedef DWORD(__cdecl *XR_NewThread)(DWORD RLS);
typedef DWORD(__cdecl *XR_LRef)(DWORD RLS, DWORD T);
typedef DWORD(__cdecl* XR_Next)(DWORD RLS, int idx);
typedef DWORD(__cdecl *XR_Type)(DWORD RLS, int idx);
typedef int(__cdecl *XR_NewLStr)(DWORD RLS, const char *str, size_t l);
typedef r_l_TValue*(__fastcall *XR_Index2)(int lst, int idx);


XR_GetField r_l_getfield = (XR_GetField)(unprotect(ASLR(GetField_Addy)));
XR_SetField rX_l_setfield = (XR_SetField)(unprotect(ASLR(SetField_Addy)));
XR_GetTable r_l_gettable = (XR_GetTable)(unprotect(ASLR(Gettable_Addy)));
XR_SetTable r_l_settable = (XR_SetTable)(unprotect(ASLR(Settable_Addy)));

XR_CreateTable r_l_createtable = (XR_CreateTable)(unprotect(ASLR(CreateTable_Addy)));
XR_GetMetaTable r_l_getmetatable = (XR_GetMetaTable)(unprotect(ASLR(GetMetaTable_Addy)));
XR_SetMetaTable r_l_setmetatable = (XR_SetMetaTable)(unprotect(ASLR(SetMetaTable_Addy)));
XR_TolString r_l_tolstring = (XR_TolString)(unprotect(ASLR(ToLString_Addy)));
XR_GetMetaField r_l_getmetafield = (XR_GetMetaField)(unprotect(ASLR(GetMetaField_Addy)));
XR_Remove r_l_remove = (XR_Remove)(unprotect(ASLR(Remove_Addy)));
//XR_ToPointer r_l_topointer = (XR_ToPointer)(unprotect(ASLR(ToPointer_Addy)));

XR_PushNumber r_l_pushnumber = (XR_PushNumber)(unprotect(ASLR(PushNumber_Addy)));
XR_PushLightUserData r_l_pushlightuserdata = (XR_PushLightUserData)(unprotect(ASLR(PushLightUserData_Addy)));
XR_PushCClosure r_l_pushcclosure = (XR_PushCClosure)(unprotect(ASLR(PushCClosure_Addy)));

XR_ToNumber r_l_tonumber = (XR_ToNumber)(ASLR(ToNumber_Addy));
XR_ToBoolean r_l_toboolean = (XR_ToBoolean)(ASLR(ToBoolean_Addy));
XR_ToUserData r_l_touserdata = (XR_ToUserData)Retcheck::Unprotect(ASLR(ToUserData_Addy), true);

XR_NewUserData r_l_newuserdata = (XR_NewUserData)(unprotect(ASLR(NewUserData_Addy)));
XR_RawGetI r_l_rawgeti = (XR_RawGetI)(unprotect(ASLR(RawGetI_Addy)));
XR_RawSetI r_l_rawseti = (XR_RawSetI)(unprotect(ASLR(RawSetI_Addy)));
XR_PCall rX_l_pcall = (XR_PCall)Retcheck::Unprotect(ASLR(PCall_Addy), true);
XR_NewThread r_l_newthread = (XR_NewThread)(unprotect(ASLR(NewThread_Addy)));
XR_LRef r_l_lref = (XR_LRef)(Ret::unprotect<DWORD>((byte*)(ASLR(LRef_Addy))));
XR_Next r_l_next = (XR_Next)(unprotect(ASLR(Next_Addy)));
XR_Type r_l_type = (XR_Type)(unprotect(ASLR(Type_Addy)));
XR_NewLStr r_l_newlstr = (XR_NewLStr)ASLR(NewLStr_Addy);
XR_Index2 r_l_index2 = (XR_Index2)ASLR(Index2_Addy);

/*Lua Macros In RLua*/

#define r_l_pop(rL,n)		r_l_settop(rL, -(n)-1)
#define r_l_getglobal(rL,s)	r_l_getfield(rL, -10002, (s))
#define r_l_createnewtable(rL) r_l_createtable(rL, 0, 0)
#define r_l_tostring(rL,i)	r_l_tolstring(rL, (i), NULL)
#define r_l_pushcfunction(rL, fn)		 (r_l_pushcclosure(rL,fn,0))
#define r_l_isnil(L,n)		(r_l_type(L, (n)) == R_LUA_TNIL)
#define r_lua_getglobal(rL,s)	r_l_getfield(rL, LUA_GLOBALSINDEX, (s))


/*Inlined Functions*/

void r_l_pushnil(DWORD a1) {
	*(_DWORD *)(*(_DWORD *)(a1 + top) + 8) = 0;
	*(_DWORD *)(a1 + top) += 16;
}

void r_l_pushboolean(DWORD a1, int a2) {
	_DWORD *v2;
	v2 = *(_DWORD **)(a1 + top);
	*v2 = a2 != 0;
	v2[2] = 3;
	*(_DWORD *)(a1 + top) += 16;
}

void r_l_pushstring(int rState, const char *s) {
	int v3 = *(DWORD *)(rState + top);
	*(DWORD *)v3 = r_l_newlstr(rState, s, strlen(s));
	*(DWORD *)(v3 + 8) = R_LUA_TSTRING;
	*(DWORD *)(rState + top) += 16;
}

void r_l_settop(int a1, int a2) {
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

void r_l_pushobject(DWORD pRobloxState, TValue *value) {
	auto &topa = *(TValue**)(pRobloxState + top);
	*topa = *value;
	++topa;
}


void r_l_pushvalue(DWORD a1, int a2)
{
	r_l_TValue *p = r_l_index2(a1, a2);
	int v3 = *(DWORD *)(a1 + top);
	*(DWORD *)v3 = *(DWORD *)p;
	*(DWORD *)(v3 + 4) = *(DWORD *)(p + 4);
	*(DWORD *)(v3 + 8) = p->tt;
	*(DWORD *)(a1 + top) += 16;
}


static TValue *lua_index2adr(lua_State *L, int idx) {
	if (idx > 0) {
		TValue *o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue *, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure *func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure *func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue *, luaO_nilobject);
	}
	}
}


int r_l_gettop(DWORD a1)
{
	return (*(_DWORD *)(a1 + top) - *(_DWORD *)(a1 + base)) >> 4;
}

/*Working PCall Bypass Impl Using Eternals SEHChainFake Bypass, No RawJZ Needed*/

int r_l_pcall(DWORD rL, int nargs, int nresults, int errfunc)
{
	DWORD* exceptionChain = (DWORD*)__readfsdword(0);
	DWORD unk = exceptionChain[1];
	((((DWORD*)exceptionChain[0])[1]) != NULL);
	{
		DWORD nextUnk = ((DWORD*)exceptionChain[0])[1];
		fakeChain(exceptionChain);
		int ret = rX_l_pcall(rL, nargs, nresults, errfunc);
		restoreChain(exceptionChain, unk, nextUnk);
		return ret;
	}

	fakeChain(exceptionChain);
	int ret = rX_l_pcall(rL, nargs, nresults, errfunc);
	restoreChain(exceptionChain, unk, 0);
	return ret;
}

inline void r_l_getservice(DWORD state, const char* s)
{
	r_l_getfield(RLS, -10002, "game");
	r_l_getfield(RLS, -1, "GetService");
	r_l_pushvalue(RLS, -2);
	r_l_pushstring(RLS, s);
	r_l_pcall(RLS, 2, 1, 0);
}


void r_l_setfield(DWORD L, int idx, const char *k) {
	r_l_pushvalue(L, idx);
	if (r_l_getmetatable(L, -1)) {
		r_l_getfield(L, -1, "__newindex");
		r_l_pushvalue(L, -3);
		r_l_pushstring(L, k);
		r_l_pushvalue(L, -6);
		r_l_pcall(L, 3, 1, 0);
		r_l_pop(L, 3);
	}
	else {
		r_l_pop(L, 1);
		rX_l_setfield(L, idx, k);
	}
}

static int GC(lua_State *Thread) {
	void *UD = lua_touserdata(Thread, 1);
	if (RLS) {
		r_l_rawgeti(RLS, LUA_REGISTRYINDEX, (int)UD);
		if (r_l_type(RLS, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
		}
	}
	return 0;
}

static int EQ(lua_State *Thread) {
	lua_pushboolean(Thread, (*(DWORD *)lua_touserdata(Thread, 1) == *(DWORD *)lua_touserdata(Thread, 2)));
	return 1;
}

void r_l_emptystack(DWORD luaState)
{
	r_l_settop(luaState, 0);
};

void PushCClosure(const void* func, DWORD n) {
	try {
		r_l_pushcclosure(RLS, (int)func, n);
	}
	catch (...) {};
}


VOID PushCClosureBP(void* fn, DWORD n)
{
	r_l_pushlightuserdata(RLS, fn);
	PushCClosure((const void*)(bpint3[1]), n);
}