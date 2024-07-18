
#pragma once
#include <Windows.h>
#include <stdio.h>
#include <sys/types.h>
//#include "VMProtectSDK.h"
namespace RLua {
	int lua_State = 0;
	//LUA_ENVIRONINDEX; a closure environment table (for either a C closure or a Lua closure) 
	//#define LUA_MULTRET	            (-1)
#define LUA_REGISTRYINDEX       (-10000)
#define LUA_ENVIRONINDEX        (-10001)
#define LUA_GLOBALSINDEX        (-10002)
#define lua_upvalueindex(i)     (LUA_GLOBALSINDEX-(i))

#define RLUA_TNONE               (-1)
#define RLUA_TNIL                0
#define RLUA_TBOOLEAN            1
#define RLUA_TLIGHTUSERDATA      2
#define RLUA_TNUMBER             3
#define RLUA_TSTRING             4
#define RLUA_TTABLE              5
#define RLUA_TFUNCTION           6
#define RLUA_TUSERDATA           7
#define RLUA_TTHREAD             8

#define lua_pop(L,n)            RLua::lua_settop(L, -(n)-1)
#define lua_emptystack(L)       RLua::lua_settop(L, 0)
#define lua_isfunction(L,n)     (lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)        (lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)        (lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define rlua_isnil(L,n)          (lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)      (lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)       (lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)         (lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)   (RLua::lua_type(L, (n)) <= 0)

#define lua_setglobal(L,s)      lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define rlua_getglobal(L,s)      RLua::lua_getfield(L, LUA_GLOBALSINDEX, (s))
#define rlua_tostring(L,i)       RLua::lua_tolstring(L, (i), NULL)
	//int (rluaL_loadstring)(int lstate, const char *s);



	typedef void* (__cdecl *lua_GetField)(int lua_State, int idx, const char *k);
	typedef int(__cdecl *lua_PCall)(int lua_State, int nargs, int nresults, int errfunc);
	typedef void(__cdecl *lua_PushString)(int lua_State, const char *s);
	typedef void(__cdecl *lua_PushNumber)(int lua_State, double Number);
	typedef const char *(__cdecl *lua_ToLString)(int lua_State, int idx, size_t *len);
	typedef void* (__cdecl *lua_PushValue)(int lua_State, int idx);
	typedef int(__cdecl *lua_GetTop)(int lua_State);
	typedef int(__cdecl *lua_GetVM)(int lua_State);
	typedef void* (__cdecl *lua_SetTop)(int lua_State, int n);
	typedef void* (__cdecl *lua_ToUserData)(int lua_State, int idx);
	typedef void* (__cdecl *lua_SetField)(int lua_State, int idx, const char *k);
	typedef void* (__cdecl *lua_PushCClosure)(int lua_State, int fn(), int n);
	typedef BOOL(__cdecl *lua_ToBoolean)(int lua_State, int idx);
	typedef int(__cdecl *lua_Next)(int lua_State, int idx);
	typedef int(__cdecl *lua_Type)(int lua_State, int idx);
	typedef int(__cdecl *lua_Call)(int lua_State, int idx, int idx2);
	typedef void(__cdecl *lua_Loadstring)(int lua_State, const char *s);
	typedef int(__cdecl *lua_Setmetatable)(DWORD rbxlua_State, int index);
	typedef void*(__cdecl *lua_Newuserdata)(DWORD lua_State, size_t size);



	/*
	memcheck: 005399c5
	tolstring: 00534cc0
	retcheck: 00537de0
	remove: 00534610
	newuserdata: 00533b10
	touserdata: 00535000
	gettop: 00533740
	toboolean: 00534c40
	type: 00535030
	setmetatable: 00534910
	setfield: 00534850
	pushlightuserdata: 00533fe0
	getinfo: 007447d0
	pushnil: 00534ae0
	tonumber: 00534110
	getstack: 00744950
	pushboolean: 00533d90
	insert: 005337f0
	settop: 00534ae0
	next: 00533bb0
	pushcclosure: 00533df0
	load: 005339d0
	*/
	int aslr(DWORD addr){
		//VMProtectBeginUltra("dasdastgttw");
		DWORD dynamicbase = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");
		DWORD rekt = (addr - 0x401000 + dynamicbase + 0x001000);
		return rekt;
	//	VMProtectEnd();
	}



	//tonumber = 0x536DA0

	DWORD ps = aslr(0x536E10); //pushstring upd

	DWORD sf = aslr(0x5374E0); //setfield upd

	DWORD pn = aslr(0x536DA0); //pushnumber upd

	DWORD pcall = aslr(0x536970); //updated

	DWORD tls = aslr(0x537950); // tolstring upd

	DWORD st = aslr(0x537770); //settop  upd

	DWORD tb = aslr(0x5378D0); //toboolen upd

	DWORD t = aslr(0x537CC0); //lua_type upd

	DWORD gf = aslr(0x536230); // getfield upd

	DWORD pv = aslr(0x536F00); // pushvalue upd

	DWORD os = aslr(0x52A4C0); // openstate

	DWORD rc = aslr(0x537ED0);

	DWORD gt = aslr(0x533810);
	DWORD ls = aslr(0x52A680);

	DWORD smt = aslr(0x5375A0);
	DWORD nud = aslr(0x5367A0);

	DWORD tud = aslr(0x537C90); 

	//  int ScriptContextVFTable = aslr(0x1008904);
	lua_Call lua_call = (lua_Call)0x517060;
	lua_PushString lua_pushstring = (lua_PushString)ps; //updated
	lua_SetField lua_setfield = (lua_SetField)sf; //updated
	lua_PushNumber lua_pushnumber = (lua_PushNumber)pn; //updated
	lua_PCall lua_pcall = (lua_PCall)pcall; //updated 
	lua_ToLString lua_tolstring = (lua_ToLString)tls; //updated
	lua_SetTop lua_settop = (lua_SetTop)st; //updated
	lua_ToBoolean lua_toboolean = (lua_ToBoolean)tb;
	lua_Type lua_type = (lua_Type)t;
	lua_GetField lua_getfield = (lua_GetField)gf; //updated
	lua_PushValue lua_pushvalue = (lua_PushValue)pv; //updated
	lua_GetTop lua_gettop = (lua_GetTop)gt; //updated
	lua_Setmetatable lua_setmetatable = (lua_Setmetatable)smt;
	lua_Newuserdata lua_newuserdata = (lua_Newuserdata)nud;
	lua_GetVM lua_getvm = (lua_GetVM)0x75F2E0;
	lua_ToUserData lua_touserdata = (lua_ToUserData)tud;
	lua_Next lua_next = (lua_Next)0x5401F0;


	void lua_pushnil(int lua_State) {
		*(DWORD *)(*(DWORD *)(lua_State + 16) + 8) = 0;
		*(DWORD *)(lua_State + 16) += 16;
	}

	DWORD PatchedGetTop(int state) {
		if (lua_State == 0) {
			lua_State = state;
		}
		return (*(DWORD *)(state + 16) - *(DWORD *)(state + 28)) >> 4;
	}

	DWORD* PatchedIdentity() {

		return (DWORD*)0x163A418;
	}

	DWORD* Patchedvm(int bytes){
		return (DWORD *)((int)13 + 24) - 15501658;

	}
}


