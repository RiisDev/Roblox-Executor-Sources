#pragma once
#include "WrapRBX.h"
#include "Address.hpp"
#include "Console.h"
int PreventCracking;//Inside joke
int ImperiousRetardedHandler(lua_State* LS);

	struct DataModelUserData {
		int ref;
	};


void Wrap(DWORD RLS, lua_State* LS, int Index)
{
	int WrapType = r_lua_type(RLS, Index);
	if (WrapType == R_LUA_TLIGHTUSERDATA) {
	//	std::cout << ired << "[Wrap Type]";
		lua_pushlightuserdata(LS, nullptr);
		//std::cout << ipurple << " RLUA_TLIGHTUSERDATA" << endl;
	}
	else if (WrapType == R_LUA_TNIL) {
		//std::cout << ired << "[Wrap Type]";
		lua_pushnil(LS);
		//std::cout << ipurple << " RLUA_TNIL" << endl;
	}
	else if (WrapType == R_LUA_TNUMBER) {
		//std::cout << ired << "[Wrap Type]";
		lua_pushnumber(LS, r_lua_tonumber(RLS, Index));
		//std::cout << ipurple << " R_LUA_TNUMBER" << endl;
	}
	else if (WrapType == R_LUA_TBOOLEAN) {
		//std::cout << ired << "[Wrap Type]";
		lua_pushboolean(LS, r_lua_toboolean(RLS, Index));
		//std::cout << ipurple << " R_LUA_TBOOLEAN" << endl;
	}
	else if (WrapType == R_LUA_TSTRING) {
	//	std::cout << ired << "[Wrap Type]";
		lua_pushstring(LS, r_lua_tostring(RLS, Index));
		//std::cout << ipurple << " R_LUA_TSTRING" << endl;
	}
	else if (WrapType == R_LUA_TTHREAD) {
	//	std::cout << ired << "[Wrap Type]";
		lua_newthread(LS);
		//std::cout << ipurple << " R_LUA_TTHREAD" << endl;
	}
	else if (WrapType == R_LUA_TFUNCTION) {
	//	std::cout << ired << "[Wrap Type]";
		r_lua_pushvalue(RLS, Index);
		lua_pushnumber(LS, r_luaL_ref(RLS, LUA_REGISTRYINDEX));
		lua_pushcclosure(LS, ImperiousRetardedHandler, 1);
		//std::cout << ipurple << " R_LUA_TFUNCTION" << endl;
	}
	else if (WrapType == R_LUA_TTABLE) {
	//	std::cout << ired << "[Wrap Type]";
		r_lua_pushvalue(RLS, Index);
		lua_newtable(LS);
		r_lua_pushnil(RLS);
		//std::cout << ipurple << " R_LUA_TTABLE" << endl;
		while (r_lua_next(RLS, -2) != R_LUA_TNIL)
		{
			Wrap(RLS, LS, -2);
			Wrap(RLS, LS, -1);
			lua_settable(LS, -3);
			r_lua_pop(RLS, 1);
		}
		r_lua_pop(RLS, 1);
	}
	else if (WrapType == R_LUA_TUSERDATA) {
		int passedUD = (int)r_lua_touserdata(RLS, Index);
		lua_rawgeti(LS, LUA_REGISTRYINDEX, passedUD);

		if (!lua_type(LS, -1))
		{
			lua_settop(LS, -2);
			r_lua_pushvalue(RLS, Index);
			reinterpret_cast<DataModelUserData*>(lua_newuserdata(LS, sizeof(DataModelUserData)))->ref = r_luaL_ref(RLS, LUA_REGISTRYINDEX);
			r_lua_getmetatable(RLS, Index);
			Wrap(RLS, LS, -1);
			lua_setmetatable(LS, -2);
			lua_pushvalue(LS, -1);
			lua_rawseti(LS, -10000, passedUD);
			r_lua_settop(RLS, -2);
		}
	}
	else if (WrapType == R_LUA_TPROTO) {
	//	std::cout << ired << "[Wrap Type]";
		luaF_newproto(LS);
		//std::cout << ipurple << " R_LUA_TPROTO" << endl;
	}
	else if (WrapType == R_LUA_TUPVALUE) {
	//	std::cout << ired << "[Wrap Type]";
		luaF_newupval(LS);
	//	std::cout << ipurple << " R_LUA_TUPVALUE" << endl;
	}
	else if (WrapType == R_LUA_TNONE) {
	//	std::cout << ired << "[Wrap Type]";
		lua_pushnil(LS);
		//std::cout << ipurple << " R_LUA_TNONE" << endl;
	}
}

void UnWrap(lua_State* LS, DWORD RLS, int Index)
{
	int UnWrapType = lua_type(LS, Index);
	if (UnWrapType == LUA_TLIGHTUSERDATA) {
		r_lua_pushlightuserdata(RLS, nullptr);
	}
	else if (UnWrapType == LUA_TNIL) {
		r_lua_pushnil(RLS);
	}
	else if (UnWrapType == LUA_TNUMBER) {
		r_lua_pushnumber(RLS, lua_tonumber(LS, Index));
	}
	else if (UnWrapType == LUA_TBOOLEAN) {
		r_lua_pushboolean(RLS, lua_toboolean(LS, Index));
	}
	else if (UnWrapType == LUA_TSTRING) {
		r_lua_pushstring(RLS, lua_tostring(LS, Index));
	}
	else if (UnWrapType == LUA_TTHREAD) {
		r_lua_newthread(RLS);
	}
	else if (UnWrapType == LUA_TFUNCTION) {
		lua_pushvalue(LS, Index);
		r_lua_pushnumber(RLS, luaL_ref(LS, LUA_REGISTRYINDEX));
		r_lua_pushcclosure(RLS, BreakPointShit[0], 1);
	}
	else if (UnWrapType == LUA_TTABLE) {
		lua_pushvalue(LS, Index);
		lua_getfield(LS, LUA_GLOBALSINDEX, "tostring");
		lua_pushvalue(LS, -2);
		lua_pcall(LS, 1, 1, 0);
		const char* a = lua_tolstring(LS, -1, NULL);
		lua_settop(LS, -2);
		r_lua_createtable(RLS, 0, 0);
		lua_pushnil(LS);
		while (lua_next(LS, -2)) {
			lua_pushvalue(LS, -1);
			lua_getfield(LS, LUA_GLOBALSINDEX, "tostring");
			lua_pushvalue(LS, -2);
			lua_pcall(LS, 1, 1, 0);
			const char* b = lua_tolstring(LS, -1, NULL);
			lua_settop(LS, -3);
			if (strcmp(a, b) == 0) {
				lua_settop(LS, -2);
				continue;
			}
			UnWrap(LS, RLS, -2);
			UnWrap(LS, RLS, -1);
			r_lua_settable(RLS, -3);
			lua_settop(LS, -2);
		}
		lua_settop(LS, -2);
	}
	else if (UnWrapType == LUA_TUSERDATA) {
		r_lua_rawgeti(RLS, LUA_REGISTRYINDEX, reinterpret_cast<Userdata*>(lua_touserdata(LS, Index))->reference);
	}
	else if (UnWrapType == LUA_TNONE) {
		r_lua_pushnil(RLS);
	}
}

static int LuaYeild(lua_State* thread)
{
	lua_State* L = lua_tothread(thread, lua_upvalueindex(1));
	const int nargs = lua_gettop(thread);
	lua_xmove(thread, L, nargs);
	return lua_resume(L, nargs);
	lua_newtable(L);
	lua_pushstring(L, "This metatable is locked");
	lua_setfield(L, -2, "__metatable");
	lua_close(L);

}
int RBXYeild(DWORD thread)
{
	lua_State* L = (lua_State*)r_lua_touserdata(thread, lua_upvalueindex(1));
	const int nargs = r_lua_gettop(thread);
	for (int arg = 1; arg <= nargs; ++arg)
		Wrap(thread, L, arg);
	return lua_resume(L, nargs);
}

int ImperiousRetardedHandler(lua_State* LS)
{
	r_lua_pushstring(RLS, std::to_string(++PreventCracking).c_str());
	DWORD rL = r_lua_newthread(RLS);
	r_lua_settable(RLS, LUA_REGISTRYINDEX);
	int key = lua_tonumber(LS, lua_upvalueindex(1));
	r_lua_rawgeti(rL, LUA_REGISTRYINDEX, key);
	for (int arg = 1; arg <= lua_gettop(LS); ++arg)
		UnWrap(LS, rL, arg);
	if (r_lua_pcall(rL, lua_gettop(LS), LUA_MULTRET, 0))
	{
		const char* errormessage = r_lua_tostring(rL, -1, 0);
		if (!errormessage || strlen(errormessage) == 0)
			errormessage = "Error occourred, no output from Lua\n";
		if (strcmp(errormessage, "attempt to yield across metamethod/C-call boundary") == 0)
		{
			r_lua_pop(rL, 1);
			lua_pushthread(LS);
			lua_pushcclosure(LS, &LuaYeild, 1);
			UnWrap(LS, rL, -1);
			return lua_yield(LS, 0);
		}
		return 0;
		delete[] errormessage;
	}
	int args = 0;
	for (int arg = 1; arg <= r_lua_gettop(rL); ++arg, ++args)
		Wrap(rL, LS, arg);
	r_lua_settop(rL, 0);
	return args;
	lua_close(LS);
}

int RBXImperiousRetardedHandler(DWORD RLS)
{
	lua_pushstring(LS, std::to_string(++PreventCracking).c_str());
	lua_State* L = lua_newthread(LS);
	lua_settable(LS, LUA_REGISTRYINDEX);
	int key = r_lua_tonumber(RLS, lua_upvalueindex(1));
	lua_rawgeti(L, LUA_REGISTRYINDEX, key);
	for (int arg = 1; arg <= r_lua_gettop(RLS); ++arg)
		Wrap(RLS, L, arg);
	int Error = lua_pcall(L, r_lua_gettop(RLS), LUA_MULTRET, 0);
	if (Error == LUA_YIELD) {
		r_lua_pushlightuserdata(RLS, (void*)L);
		r_lua_pushcclosure(RLS, BreakPointShit[1], 1);
	}
	int args = 0;
	for (int arg = 1; arg <= lua_gettop(L); ++arg, ++args)
		UnWrap(L, RLS, arg);
	lua_settop(L, 0);
	return args;
	lua_close(L);
}

LONG WINAPI MainExpectionHandler(PEXCEPTION_POINTERS ex)
{
	switch (ex->ExceptionRecord->ExceptionCode)
	{
	case (DWORD)0x80000003L:
	{
		if (ex->ContextRecord->Eip == BreakPointShit[0])
		{
			ex->ContextRecord->Eip = (DWORD)(RBXImperiousRetardedHandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (ex->ContextRecord->Eip == BreakPointShit[1])
		{
			ex->ContextRecord->Eip = (DWORD)(RBXYeild);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return -1;
	}
	default: return 0;
	}
	return 0;
}
DWORD locateINT3() {
	DWORD _s = Rebase(0x400000);
	const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
	for (int i = 0; i < MAX_INT; i++) {
		if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {
			return (_s + i);
		}
	}
	return NULL;
}
VOID BypassBreakpoints()
{
	BreakPointShit.push_back(locateINT3());
	BreakPointShit.push_back(locateINT3());
	AddVectoredExceptionHandler(1, MainExpectionHandler);
}