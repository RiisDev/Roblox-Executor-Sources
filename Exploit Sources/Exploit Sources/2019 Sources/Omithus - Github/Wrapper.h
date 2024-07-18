#pragma once
//written by customality
//snipped system48 compatibility (it has nothing to do with the wrapper)
//pls dont blame me for the SHITTY code

#include "retcheck.h"
#include "scanner.hpp"
#include "HttpGet.h"
#include <Windows.h>
#include <sstream>
#include <TlHelp32.h>

#define VERSION "1.0"
#define DbgPrint __noop
#define Error MessageBoxA

#define GetGlobalStateIndex(SC, idx) (int*)(SC[14 * idx + 41] ^ (DWORD)&SC[14 * idx + 41])
#define ASLR(x) (x - 0x400000 + (int)GetModuleHandle(0))

#define R_LUA_REGISTRYINDEX 0xFFFFD8F0

#define NEWTHREAD 0x007F24F0//
#define PUSHSTRING 0x007F2DC0//
#define PUSHNUMBER 0x007F2D30//
#define PCALL 0x007F2800//
#define SETMETATABLE 0x007F3990//
#define NEWUSERDATA 0x007F25D0//
#define TOLSTRING 0x007F3F40//
#define TOBOOLEAN 0x007F3D50//
#define TOUSERDATA 0x007F4220//
#define SETFIELD 0x007F37E0//
#define PUSHVALUE 0x007F2E80//
#define PUSHCCLOSURE 0x007F2950//
#define TONUMBER 0x007F40F0//
#define GETMETAFIELD 0x007EC630//
#define TYPE 0x007F4250//
#define SETTOP 0x007F3BD0//
#define RAWGETI 0x007F3130//
#define REF 0x007ECA80//
#define NEXT 0x007F2680
#define GETTABLE 0x007F1EC0//
#define REMOVE 0x007F33E0//
#define SETTHREADIDENTITY 0x7D9590

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

namespace LuaWrapper {
	void ExecuteScript(std::string Source);
}

typedef void(__cdecl* rsetthreadidentity)(int, int, int);
rsetthreadidentity r_setthreadidentity = (rsetthreadidentity)ASLR(SETTHREADIDENTITY);

Ret ret;

typedef void*(__cdecl* rgetfield)(int, int, const char*);
rgetfield r_getfield = (rgetfield)Scanner::ReturnCheck::RemoveProtection(ASLR(0x007F1A50));

typedef void(__cdecl *rcreatetable)(int, int, int);
rcreatetable r_createtable = (rcreatetable)Scanner::ReturnCheck::RemoveProtection(ASLR(0x007F1700));

typedef int(__cdecl* rnewthread)(int);
rnewthread r_newthread = (rnewthread)Scanner::ReturnCheck::RemoveProtection(ASLR(0x7F24F0));

typedef void(__stdcall* rpushstring)(int, const char*);
rpushstring r_pushstring = (rpushstring)ASLR(PUSHSTRING);

typedef void(__cdecl* rpushnumber)(int, double);
rpushnumber r_pushnumber = (rpushnumber)Scanner::ReturnCheck::RemoveProtection(ASLR(PUSHNUMBER));

typedef int(__cdecl* rpcall)(int, int, int, int*);
rpcall r_pcall = (rpcall)Scanner::ReturnCheck::RemoveProtection(ASLR(PCALL));

typedef void(__cdecl* rsetmetatable)(int, int);
rsetmetatable r_setmetatable = (rsetmetatable)Scanner::ReturnCheck::RemoveProtection(ASLR(SETMETATABLE));

typedef void*(__cdecl* rnewuserdata)(int, int);
rnewuserdata r_newuserdata = (rnewuserdata)Scanner::ReturnCheck::RemoveProtection(ASLR(NEWUSERDATA));

typedef const char*(__stdcall* rtolstring)(int, int, int);
rtolstring r_tolstring = (rtolstring)Scanner::ReturnCheck::RemoveProtection(ASLR(TOLSTRING));

#define r_tostring(L, idx) r_tolstring(L, idx, 0)

typedef bool(__cdecl* rtoboolean)(int, int);
rtoboolean r_toboolean = (rtoboolean)ASLR(TOBOOLEAN);

typedef int(__cdecl* rtouserdata)(int, int);
rtouserdata r_touserdata = (rtouserdata)ASLR(TOUSERDATA);

typedef void(__fastcall* rsetfield)(int, int, const char*);
rsetfield r_setfield = (rsetfield)Scanner::ReturnCheck::RemoveProtection(ASLR(SETFIELD));

typedef void(__stdcall* rpushvalue)(int, int);
rpushvalue r_pushvalue = (rpushvalue)Scanner::ReturnCheck::RemoveProtection(ASLR(PUSHVALUE));

typedef void(__stdcall* rpushcclosure)(int, int, int);
rpushcclosure r_pushcclosure = (rpushcclosure)ret.unprotect<DWORD>((BYTE*)ASLR(PUSHCCLOSURE));

#define r_pushcfunction(L, f) r_pushcclosure(L, f, 0);

typedef double(__fastcall* rtonumber)(int, int);
rtonumber r_tonumber = (rtonumber)ret.unprotect<DWORD>((BYTE*)ASLR(TONUMBER));

typedef void(__cdecl* rgetmetafield)(int, int, const char*);
rgetmetafield r_getmetafield = (rgetmetafield)ASLR(GETMETAFIELD);

typedef int(__cdecl* rtype)(int, int);
rtype r_type = (rtype)ASLR(TYPE);

typedef void(__cdecl* rsettop)(int, int);
rsettop r_settop = (rsettop)Scanner::ReturnCheck::RemoveProtection(ASLR(SETTOP));

typedef int(__cdecl* rref)(int, int);
rref r_ref = (rref)ret.unprotect<DWORD>((BYTE*)ASLR(REF));

typedef void(__cdecl* rrawgeti)(int, int, int);
rrawgeti r_rawgeti = (rrawgeti)Scanner::ReturnCheck::RemoveProtection(ASLR(RAWGETI));

typedef int(__cdecl* rnext)(int, int);
rnext r_next = (rnext)Scanner::ReturnCheck::RemoveProtection(ASLR(NEXT));

typedef int(__cdecl* rgettable)(int*, int);
rgettable r_gettable = (rgettable)ASLR(GETTABLE);

typedef void(__cdecl* rremove)(int, int);
rremove r_remove = (rremove)Scanner::ReturnCheck::RemoveProtection(ASLR(REMOVE));

#define r_pop(L, n) r_settop(L, -(n)-1)

DWORD int3breakpoint = ASLR(0x401016);

void _WriteMem(DWORD Address, char Byte) {
	DWORD Protection;
	VirtualProtect((LPVOID)Address, 0x5, PAGE_READWRITE, &Protection);
	*(char*)Address = Byte;
	VirtualProtect((LPVOID)Address, 0x5, Protection, &Protection);
}

int r_gettop(int _State) {
	int State = (int)_State;
	return *(DWORD*)(State + 24) - *(DWORD*)(State + 12) >> 4;
}

void r_pushboolean(int _State, bool boolean) {
	int State = (int)_State;
	int v2 = *(DWORD*)(State + 24);
	*(DWORD*)v2 = boolean;
	*(DWORD*)(v2 + 8) = 3;
	*(DWORD*)(State + 24) += 16;
}

void r_pushnil(int _State) {
	int State = (int)_State;
	int v2 = *(DWORD*)(State + 24);
	*(DWORD*)(v2 + 8) = 0;
	*(DWORD*)(State + 24) += 16;
}

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lstate.h"
#include "Lua/lobject.h"
}

#pragma comment(lib, "legacy_stdio_definitions.lib")

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

int GetThreadPointer(lua_State* L) {
	lua_getfield(L, -10000, "__thread");
	int T = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return (int)T;
}

static int LuaErrorNoPermission(lua_State* L) {
	luaL_error(L, "You are not allowed to do that!");
	return 0;
}

static int UserdataToString(lua_State* L) {
	int T = GetThreadPointer(L);
	r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -1));
	r_getfield(T, -10002, "tostring");
	r_pushvalue(T, -2);
	r_pcall(T, 1, 1, 0);
	lua_pushstring(L, r_tostring(T, -1));
	r_pop(T, 2);
	return 1;
}

int R_GlobalState;
int R_DataModel;

void UnWrap(lua_State* L, int T, int idx);
void Wrap(int T, lua_State* L, int idx);

static int LuaFunction(int T) {
	int Key = r_tonumber(T, lua_upvalueindex(1));
	lua_State* L = (lua_State*)(int)r_tonumber(T, lua_upvalueindex(2));
	lua_rawgeti(L, -10000, Key);
	for (int i = r_gettop(T); i != 0; i--) {
		Wrap(T, L, 1);
	}
	lua_pcall(L, r_gettop(T), -1, 0);
	for (int i = lua_gettop(L); i != 0; i--)
	{
		UnWrap(L, T, 1);
	}

	return 0;
}

void UnWrap(lua_State* L, int T, int idx) {
	switch (lua_type(L, idx)) {
	case LUA_TNIL:
	{
		r_pushnil(T);
		break;
	}
	case LUA_TSTRING:
	{
		r_pushstring(T, lua_tostring(L, idx));
		break;
	}
	case LUA_TNUMBER:
	{
		r_pushnumber(T, lua_tonumber(L, idx));
		break;
	}
	case LUA_TBOOLEAN:
	{
		r_pushboolean(T, lua_toboolean(L, idx));
		break;
	}
	case LUA_TTHREAD:
	{
		r_newthread(T);
		break;
	}
	case LUA_TUSERDATA:
	{
		int* U = (int*)lua_touserdata(L, idx);
		int Key = *U;
		int IsRU = (*(int*)(U + 1) == 1);
		if (IsRU) {
			r_rawgeti(T, R_LUA_REGISTRYINDEX, Key);
		}
		else {
			int* U = (int*)r_newuserdata(T, lua_objlen(L, idx));
			*U = *(int*)lua_touserdata(L, idx);
		}
		break;
	}
	case LUA_TTABLE:
	{
		printf("TTABLE\n");
		r_createtable(T, 0, 0);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			lua_pushvalue(L, -2);
			const char* key = lua_tostring(L, -1);
			lua_pushvalue(L, -2);
			UnWrap(L, T, -1);
			r_setfield(T, -2, key);
			lua_pop(L, 2);
		}
		lua_pop(L, 1);
		break;
	}
	case LUA_TFUNCTION:
	{
		r_pushnumber(T, lua_ref(L, -10000));
		r_pushnumber(T, (int)(int*)L);
		r_pushcclosure(T, (int)int3breakpoint, 2);
		break;
	}
	}
}

static int RobloxFunction(lua_State* L) {
	int T = GetThreadPointer(L);
	int Key = lua_tonumber(L, lua_upvalueindex(1));
	int ArgC = lua_gettop(L);
	r_rawgeti(T, R_LUA_REGISTRYINDEX, Key);
	for (int i = ArgC; i != 0; i--) {
		UnWrap(L, T, i * -1);
	}
	r_pcall(T, ArgC, LUA_MULTRET, 0);
	ArgC = r_gettop(T);
	for (int i = ArgC; i != 0; i--) {
		Wrap(T, L, i * -1);
	}
	return ArgC;
}

void Wrap(int T, lua_State* L, int idx) {
	switch (r_type(T, idx)) {
	case R_LUA_TNIL:
	{
		lua_pushnil(L);
		r_remove(T, idx);
		break;
	}
	case R_LUA_TTHREAD:
	{
		lua_newthread(L);
		break;
	}
	case R_LUA_TSTRING:
	{
		lua_pushstring(L, r_tostring(T, idx));
		r_remove(T, idx);
		break;
	}
	case R_LUA_TNUMBER:
	{
		lua_pushnumber(L, r_tonumber(T, idx));
		r_remove(T, idx);
		break;
	}
	case R_LUA_TBOOLEAN:
	{
		lua_pushboolean(L, r_toboolean(T, idx));
		r_remove(T, idx);
		break;
	}
	case R_LUA_TFUNCTION:
	{
		lua_pushnumber(L, r_ref(T, R_LUA_REGISTRYINDEX));
		lua_pushcclosure(L, RobloxFunction, 1);
		break;
	}
	case R_LUA_TUSERDATA:
	{

		int Key = r_ref(T, R_LUA_REGISTRYINDEX);
		int* U = (int*)lua_newuserdata(L, 8);
		*U = Key;
		*(U + 1) = 1;
		lua_getfield(L, -10000, "Object");
		lua_setmetatable(L, -2);
		break;
	}
	case R_LUA_TTABLE:
	{
		lua_newtable(L);
		r_pushnil(T);
		while (r_next(T, -2)) {
			r_pushvalue(T, -2);

			const char* key = r_tostring(T, -1);

			r_pushvalue(T, -2);

			Wrap(T, L, -1);
			lua_setfield(L, -2, key);

			r_pop(T, 2);
		}
		r_pop(T, 1);
		break;
	}
	}
}

static int HttpGet(lua_State* l)
{
	const std::string url = download_url(lua_tostring(l, -1));
	LuaWrapper::ExecuteScript(url.c_str());
	return 1;
}

void PushGlobal(int T, lua_State* L, const char* Name) {
	r_getfield(T, -10002, Name);
	Wrap(T, L, -1);
	lua_setfield(L, -10002, Name);
}

static int UserdataIndex(lua_State* L) {
	try {
		DbgPrint("[Index] Start\n");
		int T = GetThreadPointer(L);

		const char* Key = lua_tostring(L, -1);
		lua_pop(L, 1);
		DbgPrint("[Index] Key: %s\n", Key);

		UnWrap(L, T, 1);
		r_getfield(T, -1, Key);
		Wrap(T, L, -1);
		r_pop(T, 1);
		DbgPrint("[Index] End\n");
	}
	catch (std::exception e) {
		Error(0, e.what(), "Error", 0);
		luaL_error(L, e.what());
	}
	return 1;
}

static int UserdataNewIndex(lua_State* L) {
	try {
		int T = GetThreadPointer(L);
		int Type = lua_type(L, -1);
		const char* Key = lua_tostring(L, -2);
		UnWrap(L, T, -3);
		UnWrap(L, T, -1);
		r_setfield(T, -2, Key);
		r_pop(T, 1);
	}
	catch (std::exception e) {
		Error(0, e.what(), "Error", 0);
		luaL_error(L, e.what());
	}
	return 0;
}

static int Sleep(lua_State* L) {

	return 0;
}

static int UserdataMul(lua_State* L) {
	int T = GetThreadPointer(L);

	r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -2));
	r_getmetafield(T, -1, "__mul");
	r_pushvalue(T, -2);
	r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -1));
	r_pcall(T, 2, 1, 0);
	Wrap(T, L, -1);
	return 1;
}

static int UserdataAdd(lua_State* L) {
	int T = GetThreadPointer(L);

	r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -2));
	r_getmetafield(T, -1, "__add");
	r_pushvalue(T, -2);
	r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -1));
	r_pcall(T, 2, 1, 0);
	Wrap(T, L, -1);
	return 1;
}

//void AddDataType(lua_State* L, const char* Type) {
//	lua_newtable(L);
//	lua_pushcfunction(L, UserdataToString);
//	lua_setfield(L, -2, "__tostring");
//	lua_pushcfunction(L, UserdataIndex);
//	lua_setfield(L, -2, "__index");
//	lua_pushcfunction(L, UserdataNewIndex);
//	lua_setfield(L, -2, "__newindex");
//	lua_pushstring(L, "The metatable is locked!");
//	lua_setfield(L, -2, "__metatable");
//	lua_pushcfunction(L, UserdataMul);
//	lua_setfield(L, -2, "__mul");
//	lua_pushcfunction(L, UserdataAdd);
//	lua_setfield(L, -2, "__add");
//	lua_setfield(L, -10000, "Object");//(std::string("R") + (std::string)Type).c_str());
//}

static int __mathmf(int state, lua_State * L, std::string m, int idxa, int idxb) {
	r_rawgeti(state, LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -2));
	r_getmetafield(state, -1, m.c_str());
	UnWrap(L, state, -2);
	UnWrap(L, state, -1);
	r_pcall(state, 2, 1, 0);
	Wrap(state, L, -1);
	return 0;
}

static int u_metaf(lua_State *L) {
	int rstate = lua_tonumber(L, -10003);
	std::string method = lua_tostring(L, -10004);
	__mathmf(rstate, L, method, -2, -1);
	return 1;
}

void pushMetaMeth(int rstate, lua_State *L, std::string method) {
	if (method == "__type") {
		lua_pushstring(L, "Object");
		lua_setfield(L, -2, method.c_str());
	}
	else if (method == "__metatable") {
		lua_pushstring(L, "This metatable is locked");
		lua_setfield(L, -2, method.c_str());
	}
	else {
		lua_pushnumber(L, rstate);
		lua_pushstring(L, method.c_str());
		lua_pushcclosure(L, u_metaf, 2);
		lua_setfield(L, -2, method.c_str());
	}
}



static int RawExecuteScript(lua_State* L, int T) {

	luaL_openlibs(L);

	lua_newtable(L);

	



	lua_register(L, "HttpGet", HttpGet);

	pushMetaMeth(T, L, "__add");
	pushMetaMeth(T, L, "__sub");
	pushMetaMeth(T, L, "__mul");
	pushMetaMeth(T, L, "__div");
	pushMetaMeth(T, L, "__pow");
	pushMetaMeth(T, L, "__unm");
	pushMetaMeth(T, L, "__mod");
	pushMetaMeth(T, L, "__concat");
	//lua_pushcfunction(L, __eq);
	//lua_setfield(L, -2, "__eq");
	pushMetaMeth(T, L, "__eq");
	pushMetaMeth(T, L, "__lt");
	pushMetaMeth(T, L, "__le");
	pushMetaMeth(T, L, "__type");
	pushMetaMeth(T, L, "__call");
	pushMetaMeth(T, L, "__metatable");
	lua_pushcfunction(L, UserdataIndex);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, UserdataNewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushstring(L, "The metatable is locked!");
	lua_setfield(L, -2, "__metatable");
	lua_setfield(L, -10000, "Object");

	int m_rL = T;
	lua_State* m_L = L;

	PushGlobal(m_rL, m_L, "game");
	PushGlobal(m_rL, m_L, "Game");
	PushGlobal(m_rL, m_L, "workspace");
	PushGlobal(m_rL, m_L, "Workspace");
	PushGlobal(m_rL, m_L, "Axes");
	PushGlobal(m_rL, m_L, "BrickColor");
	PushGlobal(m_rL, m_L, "CFrame");
	PushGlobal(m_rL, m_L, "Color3");
	PushGlobal(m_rL, m_L, "ColorSequence");
	PushGlobal(m_rL, m_L, "ColorSequenceKeypoint");
	PushGlobal(m_rL, m_L, "NumberRange");
	PushGlobal(m_rL, m_L, "NumberSequence");
	PushGlobal(m_rL, m_L, "NumberSequenceKeypoint");
	PushGlobal(m_rL, m_L, "PhysicalProperties");
	PushGlobal(m_rL, m_L, "Ray");
	PushGlobal(m_rL, m_L, "Rect");
	PushGlobal(m_rL, m_L, "Region3");
	PushGlobal(m_rL, m_L, "Region3int16");
	PushGlobal(m_rL, m_L, "TweenInfo");
	PushGlobal(m_rL, m_L, "UDim");
	PushGlobal(m_rL, m_L, "UDim2");
	PushGlobal(m_rL, m_L, "Vector2");
	PushGlobal(m_rL, m_L, "Vector2int16");
	PushGlobal(m_rL, m_L, "Vector3");
	PushGlobal(m_rL, m_L, "Vector3int16");
	PushGlobal(m_rL, m_L, "Enum");
	PushGlobal(m_rL, m_L, "Faces");
	PushGlobal(m_rL, m_L, "Instance");
	PushGlobal(m_rL, m_L, "math");
	PushGlobal(m_rL, m_L, "warn");
	PushGlobal(m_rL, m_L, "typeof");
	PushGlobal(m_rL, m_L, "type");
	PushGlobal(m_rL, m_L, "spawn");
	PushGlobal(m_rL, m_L, "Spawn");
	PushGlobal(m_rL, m_L, "print");
	PushGlobal(m_rL, m_L, "printidentity");
	PushGlobal(m_rL, m_L, "ypcall");
	PushGlobal(m_rL, m_L, "Wait");
	PushGlobal(m_rL, m_L, "wait");
	PushGlobal(m_rL, m_L, "delay");
	PushGlobal(m_rL, m_L, "Delay");
	PushGlobal(m_rL, m_L, "tick");
	PushGlobal(m_rL, m_L, "LoadLibrary");

	lua_pushnumber(L, (int)T);
	lua_setfield(L, -10000, "__thread");

	lua_newtable(L);
	lua_pushstring(L, VERSION);
	lua_setfield(L, -2, "Version");
	lua_setfield(L, -10002, "Omithus");

	lua_pushcfunction(L, Sleep);
	lua_setfield(L, -10002, "wait");

	lua_getfield(L, -10002, "os");
	lua_pushcfunction(L, LuaErrorNoPermission);
	lua_setfield(L, -2, "execute");
	lua_pop(L, 1);


	DbgPrint("Stack Size: %i\n", lua_gettop(L));
	DbgPrint("RStack Size: %i\n", r_gettop(T));
	return 0;
}

static int System48Execution(int* T) {
	//snip
	return 0;
}

static DWORD hookexec = ASLR(0x401016);

LONG WINAPI CallBack(PEXCEPTION_POINTERS ex)
{
	if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		if (ex->ContextRecord->Eip == int3breakpoint)
		{
			ex->ContextRecord->Eip = (DWORD)(LuaFunction);
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		else if (ex->ContextRecord->Eip == hookexec)
		{
			ex->ContextRecord->Eip = (DWORD)(RawExecuteScript);
			return EXCEPTION_CONTINUE_EXECUTION;
		}

	}
	return EXCEPTION_CONTINUE_SEARCH;
}


namespace LuaWrapper {
	void ExecuteScript(std::string Source) {
		printf("passing thread\n");
		int T = r_newthread(R_GlobalState);
		int asd;
		printf("thread T passed\n");
		//r_setthreadidentity((int)T, 6, (int)&asd);
		printf("pased pop\n");

		lua_State* L = luaL_newstate();
		printf("Passed L\n");
		RawExecuteScript(L, T);

		luaL_dostring(L, Source.c_str());
	}

	void Init(int SC) {

		printf("ScriptContext: %X\n", (int)SC);

		INT v39 = SC;
		int v51 = 0;
		R_GlobalState = (v39 + 56 * v51 + 164) ^ *(DWORD *)(v39 + 56 * v51 + 164);
		AddVectoredExceptionHandler(TRUE, CallBack);


		printf("LuaState: %X\n", R_GlobalState);

	}
}