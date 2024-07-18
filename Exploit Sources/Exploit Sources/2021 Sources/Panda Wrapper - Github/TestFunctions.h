#pragma once
#include "Wrapper.h"

std::map<const char*, lua_CFunction> MapFunctionClosures{
	std::make_pair("getgenv", [](lua_State* L) {
		lua_pushvalue(L, LUA_GLOBALSINDEX);
		return 1;
	}),
	std::make_pair("getrenv", [](lua_State* L) {
		int State = GetThreadPointer(L);
		wrap(L, State, LUA_GLOBALSINDEX);
		return 1;
	}),
	std::make_pair("getreg", [](lua_State* L) {
		int State = GetThreadPointer(L);
		wrap(L, State, LUA_REGISTRYINDEX);
		return 1;
	}),
	std::make_pair("getrawmetatable", [](lua_State* L) {
		int State = GetThreadPointer(L);
		if (lua_type(L, -1) != LUA_TUSERDATA) {
			luaL_error(L, "getrawmetatable only accepts a userdata");
			return 0;
		}
		unwrap(L, State, 1);
		if (!rlua_getmetatable(State, -1)) {
			lua_pushnil(L);
			return 1;
		}
		int ReferenceKey = rluaL_ref(State, LUA_REGISTRYINDEX);
		rlua_settop(State, -2);
		lua_createtable(L, 0, 0);
		lua_pushinteger(L, ReferenceKey);
		lua_setfield(L, -2, "__key");
		lua_createtable(L, 0, 0);
		lua_pushinteger(L, ReferenceKey);
		lua_pushcclosure(L, CustomMTIndex, 1);
		lua_setfield(L, -2, "__index");
		lua_pushinteger(L, ReferenceKey);
		lua_pushcclosure(L, CustomMTNewIndex, 1);
		lua_setfield(L, -2, "__newindex");
		lua_setmetatable(L, -2);
		return 1;
	}),
	std::make_pair("checkcaller", [](lua_State* L) {
		int State = GetThreadPointer(L);
		rlua_pushboolean(State, (*reinterpret_cast<int*>(State - 40) & 0x1F) == 6);
		return 1;
	}),
	std::make_pair("setreadonly", [](lua_State* L) {
		if (lua_gettop(L) < 0) return luaL_error(L, "setreadonly require 2 arguments!");
		int State = GetThreadPointer(L);
		int TrueORFalse = lua_toboolean(L, 2);
		if (lua_type(L, 1) == LUA_TTABLE) {
			lua_getfield(L, 1, "__key");
			if (lua_type(L, -1) > LUA_TNIL) {
				rlua_rawgeti(State, LUA_REGISTRYINDEX, lua_tointeger(L, -1));
				if (TrueORFalse) rlua_setreadonly(State, -1, 1); else rlua_setreadonly(State, -1, 0);
				r_lua_pop(State, 1);
			}
			lua_pop(L, 1);
			return 1;
		}
		else {
			unwrap(L, State, 1);
			if (TrueORFalse) rlua_setreadonly(State, -1, 1); else rlua_setreadonly(State, -1, 0);
			return 1;
		}
		return 1;
	}),
};

void TestFunctions(lua_State* L) {
	lua_State* Thread = lua_newthread(L)
;	for (auto closure : MapFunctionClosures)
		lua_register(L, closure.first, closure.second);

	lua_register(Thread, "HttpGet", DownloadString);
}
