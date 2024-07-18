#pragma once
#include <Windows.h>
#include <string>
#include <iostream>

#include "Functions.h"
#include <unordered_map>

int RobloxRegistry;
#define BASEWINAPI __stdcall


int FuncHandler(lua_State* L); int RResume(RbxState* thread);
LONG BASEWINAPI MainExceptionHandler(PEXCEPTION_POINTERS ex);
void UnWrap(RbxState* RLS, lua_State* LS, int Index);
void Wrap(lua_State* LS, RbxState* RLS, int Index);
int Handler(RbxState* rL);
std::unordered_map<void*, void*> robloxInstanceMap;
std::unordered_map<void*, void*> vanillaInstanceMap;

struct Userdata
{
	int32_t reference;
};
void cacheObject(RbxState *rL, lua_State* L, int index)
{
	uintptr_t rawInstancePtr = (uintptr_t)LuaC.r_lua_touserdata(rL, index);

	lua_rawgeti(L, LUA_REGISTRYINDEX, rawInstancePtr);

	if (!lua_type(L, -1))
	{
		lua_settop(L, -2);

		LuaC.r_lua_pushvalue(rL, index);
		reinterpret_cast<Userdata*>(lua_newuserdata(L, sizeof(Userdata)))->reference = LuaC.r_luaL_ref(rL, -10000);

		LuaC.r_lua_getmetatable(rL, index);
		UnWrap(rL, L, -1);
		lua_setmetatable(L, -2);

		lua_pushvalue(L, -1);
		lua_rawseti(L, -10000, rawInstancePtr);

		LuaC.r_lua_settop(rL, -2);
	}
}

DWORD locateINT3() {
	DWORD _s = ASLR(0x400000);
	const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
	for (int i = 0; i < MAX_INT; i++) {
		if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {
			return (_s + i);
		}
	}
	return NULL;
}

void UnWrap(RbxState *RLS, lua_State* LS, int Index) {

	switch (LuaC.r_lua_type(RLS, Index))
	{
	case R_LUA_TNIL: {
		lua_pushnil(LS);
		break;
	}
	case R_LUA_TLIGHTUSERDATA: {
		lua_pushlightuserdata(LS, LuaC.r_lua_touserdata(RLS, Index));
		break;
	}
	case R_LUA_TNUMBER: {
		lua_pushnumber(LS, LuaC.r_lua_tonumber(RLS, Index));
		break;
	}
	case R_LUA_TBOOLEAN: {
		lua_pushboolean(LS, LuaC.r_lua_toboolean(RLS, Index));
		break;
	}
	case R_LUA_TSTRING: {
		lua_pushstring(LS, r_lua_tostring(RLS, Index));
		break;
	}
	case R_LUA_TTHREAD: {
		lua_newthread(LS);
		break;
	}
	case R_LUA_TFUNCTION: {
		LuaC.r_lua_pushvalue(RLS, Index);
		lua_pushnumber(LS, LuaC.r_luaL_ref(RLS, LUA_REGISTRYINDEX));
		lua_pushcclosure(LS, FuncHandler, 1);
		break;
	}
	case R_LUA_TTABLE: {
		LuaC.r_lua_pushvalue(RLS, Index);
		lua_newtable(LS);
		LuaC.r_lua_pushnil(RLS);
		while (LuaC.r_lua_next(RLS, -2) != R_LUA_TNIL)
		{
			UnWrap(RLS, LS, -2);
			UnWrap(RLS, LS, -1);
			lua_settable(LS, -3);
			r_lua_pop(RLS, 1);
		}
		r_lua_pop(RLS, 1);
		break;
	}
	case R_LUA_TUSERDATA: {
		cacheObject(RLS, LS, Index);
		break;
	}
	case R_LUA_TNONE: {
		lua_pushnil(LS);
		break;
	}
	default: break;
	}
}

int RResume(RbxState * thread)
{
	lua_State* L = (lua_State*)LuaC.r_lua_touserdata(thread, lua_upvalueindex(1));
	const int nargs = LuaC.r_lua_gettop(thread);
	for (int arg = 1; arg <= nargs; ++arg)
		UnWrap(thread, L, arg);

	return lua_resume(L, nargs);
}


static int LUAYield(lua_State* thread)
{
	lua_State* L = lua_tothread(thread, lua_upvalueindex(1));
	const int nargs = lua_gettop(thread);
	lua_xmove(thread, L, nargs);
	return lua_resume(L, nargs);
	lua_newtable(L);
}

LONG WINAPI MainExceptionHandler(PEXCEPTION_POINTERS ex)
{
	switch (ex->ExceptionRecord->ExceptionCode)
	{
	case (DWORD)0x80000003L:
	{
		if (ex->ContextRecord->Eip == int3bp[0])
		{
			ex->ContextRecord->Eip = (DWORD)(FuncHandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (ex->ContextRecord->Eip == int3bp[1])
		{
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (ex->ContextRecord->Eip == int3bp[2])
		{
			ex->ContextRecord->Eip = (DWORD)(RResume);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return -1;
	}
	default: return 0;
	}
	return 0;
}


VOID Int3BreakPoints() {
	int3bp.push_back(locateINT3());
	int3bp.push_back(locateINT3());
	AddVectoredExceptionHandler(1, MainExceptionHandler);
}


void Wrap(lua_State* LS, RbxState *RLS, int Index) {

	switch (LuaC.r_lua_type(RLS, Index))
	{
	case R_LUA_TNIL: {
		LuaC.r_lua_pushnil(RLS);
		break;
	}
	case R_LUA_TLIGHTUSERDATA: {
		LuaC.r_lua_pushlightuserdata(RLS, lua_touserdata(LS, Index));
		break;
	}
	case R_LUA_TNUMBER: {
		LuaC.r_lua_pushnumber(RLS, lua_tonumber(LS, Index));
		break;
	}
	case R_LUA_TBOOLEAN: {
		LuaC.r_lua_pushboolean(RLS, lua_toboolean(LS, Index));
		break;
	}
	case R_LUA_TSTRING: {
		LuaC.r_lua_pushstring(RLS, lua_tostring(LS, Index));
		break;
	}
	case R_LUA_TTHREAD: {
		LuaC.r_lua_newthread(RLS);
		break;
	}
	case R_LUA_TFUNCTION: {
		lua_pushvalue(LS, Index);
		LuaC.r_lua_pushnumber(RLS, luaL_ref(LS, LUA_REGISTRYINDEX));
		LuaC.r_lua_pushcclosure(RLS, int3bp[0], 1);
		break;
	}
	case R_LUA_TTABLE: {
		lua_pushvalue(LS, Index);
		r_lua_newtable(RLS);
		lua_pushnil(LS);
		while (lua_next(LS, -2) != LUA_TNIL)
		{
			Wrap(LS, RLS, -2);
			Wrap(LS, RLS, -1);
			LuaC.r_lua_settable(RLS, -3);
			lua_pop(LS, 1);
		}
		lua_pop(LS, 1);
		break;
	}
	case R_LUA_TUSERDATA: {
		LuaC.r_lua_rawgeti(RLS, -10000, reinterpret_cast<Userdata*>(lua_touserdata(LS, Index))->reference);
		break;
	}
	case R_LUA_TNONE: {
		LuaC.r_lua_pushnil(RLS);
		break;
	}
	default: break;
	}
}



int FuncHandler(lua_State* L) {
	RbxState *rL = LuaC.RobloxState;
	RbxState *RLS = LuaC.RobloxState;
	LuaC.r_lua_pushstring(RLS, std::to_string(++RobloxRegistry).c_str());
	LuaC.r_lua_settable(RLS, LUA_REGISTRYINDEX);
	int key = lua_tonumber(L, lua_upvalueindex(1));
	LuaC.r_lua_rawgeti(rL, LUA_REGISTRYINDEX, key);
	for (int arg = 1; arg <= lua_gettop(L); ++arg) {
		UnWrap(rL, L, arg);
	}
	if (!LuaC.r_lua_pcall(rL, lua_gettop(L), LUA_MULTRET, 0)) {
		int args = 0;
		for (int arg = 1; arg <= LuaC.r_lua_gettop(rL); ++arg, ++args) {
			Wrap(L, rL, arg);
		}
		LuaC.r_lua_settop(rL, 0);
		return args;
	}
	else {
		std::string err = r_lua_tostring(rL, -1);
		if (err == (std::string)"attempt to yield across metamethod/C-call boundary")
		{
			r_lua_pop(rL, 1);
			lua_pushthread(L);
			lua_pushcclosure(L, &LUAYield, 1);
			UnWrap(rL, L, -1);
			return lua_yield(L, 0);
			r_lua_pop(RLS, 1);
		}
		else
		{
			//std::cout << ired << "Lua API Error: " << err << "\n";
			return 0;
		}
		//std::cout << ired << "Lua API Error: No Error Message Supplied!\n";
		return 0;
	}
}

int Handler(RbxState *rL) {
	lua_State *LS = LuaC.VanillaState;
	lua_pushstring(LS, std::to_string(++RobloxRegistry).c_str());
	lua_State* L = lua_newthread(LS);
	lua_settable(LS, LUA_REGISTRYINDEX);
	int key = LuaC.r_lua_tonumber(rL, lua_upvalueindex(1));
	lua_rawgeti(L, LUA_REGISTRYINDEX, key);
	for (int arg = 1; arg <= LuaC.r_lua_gettop(rL); ++arg) {
		Wrap(L, rL, arg);
	}
	int Err = lua_pcall(L, LuaC.r_lua_gettop(rL), LUA_MULTRET, 0);
	if (Err == LUA_ERRRUN) {
		//std::cout << ired << "\nRoblox API Error: " << lua_tostring(L, -1) << "\n";
		return 0;
	}
	if (Err == LUA_ERRERR) {
		//std::cout << ired << "\nUnexpected Roblox Error: " << lua_tostring(L, -1) << "\n";
		return 0;
	}
	if (Err == LUA_ERRMEM) {
		//std::cout << ired << "\nRoblox Memory Error: " << lua_tostring(L, -1) << "\n";
		return 0;
	}
	if (Err == LUA_YIELD) {
		LuaC.r_lua_pushlightuserdata(rL, (void*)L);
		LuaC.r_lua_pushcclosure(rL, int3bp[2], 1);
		r_lua_pop(rL, 1);
	}
	int args = 0;
	for (int arg = 1; arg <= lua_gettop(L); ++arg, ++args) {
		UnWrap(rL, L, arg);
	}
	lua_settop(L, 0);
	return args;
}

/* 3 table caching, we do not need it but still it looks sexy! */

std::vector<int> Tables = {};

VOID Environment()
{
	lua_State *LS = LuaC.VanillaState;
	for (int table = 0; table < 3; table++)
	{
		lua_newtable(LS);
		lua_pushnil(LS);
		int UD = table;
		Tables.push_back(UD);
	}

	for (int i = 0; i < Tables.size(); i++) {
	}
	Tables.clear();
}

using namespace std;

string ReplaceAll(string subject, const string& search,
	const string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

namespace Wrapper {

	std::string CachedScript;

	void Execute() {
		std::string Script = CachedScript;
		Script = "spawn(function() script = Instance.new('CoreScript') Script.name = '  '\r\n" + Script + "\r\nend)";

		Script = ReplaceAll(Script, "savefile", "writefile");
		Script = ReplaceAll(Script, "loadfile", "runfile");

		Script = ReplaceAll(Script, "make_writeable", "setreadonly");
		Script = ReplaceAll(Script, "HttpGetAsync", "HttpGet");

		Script = ReplaceAll(Script, "decompile", "khemical.decompile");
		Script = ReplaceAll(Script, "saveinstance", "khemical.saveinstance");

		if (luaL_loadstring(LuaC.VanillaState, Script.c_str()))
		{
			return;
		}
		else 
		{
			lua_pcall(LuaC.VanillaState, 0, 0, 0);
		}
	}

#define CRASH EXCEPTION_EXECUTE_HANDLER

	void SecureRun() {
		__try 
		{
			Execute(); 
		}
		__except (CRASH) 
		{

		}
	}
	void Execute_Script(std::string Script) {
		CachedScript = Script;
		SecureRun();
	}

}
