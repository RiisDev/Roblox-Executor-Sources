#pragma once

#include <windows.h>
#include <string>

#include "LuaCParser.h"
#include "LuaCFunctions.h"

extern "C" {
#include "Lua-5.1.5\lua.h"
#include "Lua-5.1.5\lua.hpp"
#include "Lua-5.1.5\lualib.h"
#include "Lua-5.1.5\lauxlib.h"
#include "Lua-5.1.5\luaconf.h"
}

int LuaCAPI(lua_State* LuaS) {
	DWORD a = lua_gettop(LuaS);
	std::string cmd;
	for (int i = 1; i <= a; ++i) cmd = lua_tostring(LuaS, i);
		LuaCExecute(cmd);
	return 0;
}

int PrintIdentity(lua_State* Lua51) {
	int arg = lua_gettop(Lua51);
	std::string print;
	for (int i = 1; i <= arg; ++i) {
		if (lua_isstring(Lua51, -(i))) {
			print = lua_tostring(Lua51, i);
			LuaC::rbx_getglobal(State, "printidentity");
			LuaC::rbx_pushstring(State, print.c_str());
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
		else if (lua_isboolean(Lua51, -(i))) {
			print = lua_toboolean(Lua51, i);
			LuaC::rbx_getglobal(State, "printidentity");
			LuaC::rbx_pushstring(State, print.c_str());
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
		else if (lua_isnil(Lua51, -(i))) {
			LuaC::rbx_getglobal(State, "printidentity");
			LuaC::rbx_pushnil(State);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}
	if (arg == 0) {
		LuaC::rbx_getglobal(State, "printidentity");
		LuaC::rbx_pcall(State, 0, 0, 0);
		LuaC::rbx_settop(State, 0);
	}
	return 0;
}

int Print(lua_State* L) {
	std::string message;

	int n = lua_gettop(L);  /* number of arguments */
	int i;

	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; ++i) {
		if (i > 1)
		{
			message += ' ';
		}

		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		const char* s = lua_tostring(L, -1);  /* get result */
		message += s;
		if (i > 1) fputs("\t", stdout);
		lua_pop(L, 1);  /* pop result */
	}
	LuaC::rbx_getglobal(State, "print");
	LuaC::rbx_pushstring(State, message);
	LuaC::rbx_pcall(State, n, 0, 0);
	return 0;
}

int Warn(lua_State* L) {
	std::string message;

	int n = lua_gettop(L);  /* number of arguments */
	int i;

	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; ++i) {
		if (i > 1)
		{
			message += ' ';
		}

		lua_pushvalue(L, -1);  /* function to be called */
		lua_pushvalue(L, i);   /* value to print */
		lua_call(L, 1, 1);
		const char* s = lua_tostring(L, -1);  /* get result */
		message += s;
		if (i > 1) fputs("\t", stdout);
		lua_pop(L, 1);  /* pop result */
	}
	LuaC::rbx_getglobal(State, "warn");
	LuaC::rbx_pushstring(State, message);
	LuaC::rbx_pcall(State, n, 0, 0);
	return 0;
}

//YALL KNOW DAMN WELL WHERE THAT SHIT CAME FROM ^^

int Wait(lua_State* Lua51) {
	DWORD arg = lua_gettop(Lua51);
	float time;
	for (int i = 1; i <= arg; ++i) {
		time = lua_tonumber(Lua51, i);
	}
	Sleep(time * 1000);
	// DISGUSTING ^
	return 0;
}