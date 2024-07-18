#pragma once

#include <windows.h>
#include <string>

#include "LuaCFunctions.h"

//ABSOLUTELY DISGUSTING. IF YOU'RE GONNA SKID OFF OF THIS SOURCE, I BEG THAT YOU DON'T USE THIS MESS.

extern "C" {
#include "Lua-5.1.5\lua.h"
#include "Lua-5.1.5\lua.hpp"
#include "Lua-5.1.5\lualib.h"
#include "Lua-5.1.5\lauxlib.h"
#include "Lua-5.1.5\luaconf.h"
}

extern unsigned long State;

bool getfield_index = true;
bool game_didGlobal = false;
bool workspace_didGlobal = false;
bool instance_didGlobal = false;
bool vector3_didGlobal = false;
bool call_triggered = false;
bool call_return = false;
int __indexstack1 = 0;
int __indexstack2 = 0;
int arg_size = 1;
std::string key;
std::string typeshit;
int xx, yy, zz;

#define memehax "Memehax"

int Vector3_new(lua_State* L) {
	typeshit = "Vector3";
	xx = lua_tonumber(L, 1);
	yy = lua_tonumber(L, 2);
	zz = lua_tonumber(L, 3);
	return 0;
}

int Color3_new(lua_State* L) {
	typeshit = "Color3";
	xx = lua_tonumber(L, 1);
	yy = lua_tonumber(L, 2);
	zz = lua_tonumber(L, 3);
	return 0;
}

int GameIndex(lua_State* L) {
	if (game_didGlobal == false) {
		LuaC::rbx_getglobal(State, "game");
		game_didGlobal = true;
	}
	lua_getglobal(L, "game");
	LuaC::rbx_getfield(State, -1, lua_tostring(L, -2));
	return 1;
}

int GameNewIndex(lua_State* L) {
	game_didGlobal = true;
	lua_getglobal(L, "game");
	if (typeshit == "Vector3") {
		/*	MsgBox("Vector3", "getglobal");*/
		LuaC::rbx_getglobal(State, "Vector3");
		/*MsgBox("new", "getfield -1");*/
		LuaC::rbx_getfield(State, -1, "new");
		/*MsgBox(std::to_string(xx), "pushnumber");*/
		LuaC::rbx_pushnumber(State, xx);
		/*MsgBox(std::to_string(yy), "pushnumber");*/
		LuaC::rbx_pushnumber(State, yy);
		/*MsgBox(std::to_string(zz), "pushnumber");*/
		LuaC::rbx_pushnumber(State, zz);
		/*MsgBox("3 1 0", "pcall");*/
		LuaC::rbx_pcall(State, 3, 1, 0);
		/*	MsgBox(lua_tostring(L, -3), "setfield -3");*/
		LuaC::rbx_setfield(State, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	else if (typeshit == "Color3") {
		/*	MsgBox("Color3", "getglobal");*/
		LuaC::rbx_getglobal(State, "Color3");
		/*MsgBox("-1 new", "getfield");*/
		LuaC::rbx_getfield(State, -1, "new");
		/*MsgBox(std::to_string(xx), "pushnumber");*/
		LuaC::rbx_pushnumber(State, xx);
		/*MsgBox(std::to_string(yy), "pushnumber");*/
		LuaC::rbx_pushnumber(State, yy);
		/*MsgBox(std::to_string(zz), "pushnumber");*/
		LuaC::rbx_pushnumber(State, zz);
		/*MsgBox("3 1 0", "pcall");*/
		LuaC::rbx_pcall(State, 3, 1, 0);
		/*MsgBox(lua_tostring(L, -3), "setfield -3");*/
		LuaC::rbx_setfield(State, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	else {
		if (lua_isnumber(L, -2)) {
			LuaC::rbx_pushnumber(State, lua_tonumber(L, -2));
			/*MsgBox(std::to_string(lua_tonumber(L, -2)), "pushnumber");*/
			LuaC::rbx_setfield(State, -2, lua_tostring(L, -3));
			/*MsgBox(lua_tostring(L, -3), "setfield -2");*/
		}
		else if (lua_isboolean(L, -2)) {
			LuaC::rbx_pushboolean(State, lua_toboolean(L, -2));
			/*MsgBox(std::to_string(lua_toboolean(L, -2)), "pushboolean");*/
			LuaC::rbx_setfield(State, -2, lua_tostring(L, -3));
			/*MsgBox(lua_tostring(L, -3), "setfield -2");*/
		}
		else if (lua_isstring(L, -2)) {
			LuaC::rbx_pushstring(State, lua_tostring(L, -2));
			/*	MsgBox(lua_tostring(L, -2), "pushstring");*/
			LuaC::rbx_setfield(State, -2, lua_tostring(L, -3));
			/*MsgBox(lua_tostring(L, -3), "setfield -2");*/
		}
		else if (lua_istable(L, -2)) {

		}
		else if (lua_isnil(L, -2)) {
			LuaC::rbx_pushnil(State);
			/*	MsgBox("nil", "pushnil");*/
			LuaC::rbx_setfield(State, -2, lua_tostring(L, -3));
			/*MsgBox(lua_tostring(L, -3), "setfield -2");*/
		}
	}
	return 1;
}

int WorkspaceIndex(lua_State* L) {
	if (workspace_didGlobal == false) {
		/*MsgBox("game", "getglobal");*/
		LuaC::rbx_getglobal(State, "workspace");
		workspace_didGlobal = true;
	}
	lua_getglobal(L, "workspace");
	/*MsgBox(lua_tostring(L, -2), "getfield -1");*/
	LuaC::rbx_getfield(State, -1, lua_tostring(L, -2));
	return 1;
}

int WorkspaceNewIndex(lua_State* L) {
	workspace_didGlobal = false;
	lua_getglobal(L, "workspace");
	if (typeshit == "Vector3") {
		/*	MsgBox("Vector3", "getglobal");*/
		LuaC::rbx_getglobal(State, "Vector3");
		/*MsgBox("new", "getfield -1");*/
		LuaC::rbx_getfield(State, -1, "new");
		/*MsgBox(std::to_string(xx), "pushnumber");*/
		LuaC::rbx_pushnumber(State, xx);
		/*MsgBox(std::to_string(yy), "pushnumber");*/
		LuaC::rbx_pushnumber(State, yy);
		/*MsgBox(std::to_string(zz), "pushnumber");*/
		LuaC::rbx_pushnumber(State, zz);
		/*MsgBox("3 1 0", "pcall");*/
		LuaC::rbx_pcall(State, 3, 1, 0);
		/*	MsgBox(lua_tostring(L, -3), "setfield -3");*/
		LuaC::rbx_setfield(State, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	else if (typeshit == "Color3.new") {
		/*	MsgBox("Color3", "getglobal");*/
		LuaC::rbx_getglobal(State, "Color3");
		/*MsgBox("-1 new", "getfield");*/
		LuaC::rbx_getfield(State, -1, "new");
		/*MsgBox(std::to_string(xx), "pushnumber");*/
		LuaC::rbx_pushnumber(State, xx);
		/*MsgBox(std::to_string(yy), "pushnumber");*/
		LuaC::rbx_pushnumber(State, yy);
		/*MsgBox(std::to_string(zz), "pushnumber");*/
		LuaC::rbx_pushnumber(State, zz);
		/*MsgBox("3 1 0", "pcall");*/
		LuaC::rbx_pcall(State, 3, 1, 0);
		/*MsgBox(lua_tostring(L, -3), "setfield -3");*/
		LuaC::rbx_setfield(State, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	else {
		if (lua_isnumber(L, -2)) {
			LuaC::rbx_pushnumber(State, lua_tonumber(L, -2));
			/*MsgBox(std::to_string(lua_tonumber(L, -2)), "pushnumber");*/
			LuaC::rbx_setfield(State, -2, lua_tostring(L, -3));
			/*MsgBox(lua_tostring(L, -3), "setfield -2");*/
		}
		else if (lua_isboolean(L, -2)) {
			LuaC::rbx_pushboolean(State, lua_toboolean(L, -2));
			/*MsgBox(std::to_string(lua_toboolean(L, -2)), "pushboolean");*/
			LuaC::rbx_setfield(State, -2, lua_tostring(L, -3));
			/*MsgBox(lua_tostring(L, -3), "setfield -2");*/
		}
		else if (lua_isstring(L, -2)) {
			LuaC::rbx_pushstring(State, lua_tostring(L, -2));
			/*	MsgBox(lua_tostring(L, -2), "pushstring");*/
			LuaC::rbx_setfield(State, -2, lua_tostring(L, -3));
			/*MsgBox(lua_tostring(L, -3), "setfield -2");*/
		}
		else if (lua_isnil(L, -2)) {
			LuaC::rbx_pushnil(State);
			/*	MsgBox("nil", "pushnil");*/
			LuaC::rbx_setfield(State, -2, lua_tostring(L, -3));
			/*MsgBox(lua_tostring(L, -3), "setfield -2");*/
		}
	}
	return 1;
}

int GameCall(lua_State* L) {
	game_didGlobal = false;
	arg_size = 1;
	LuaC::rbx_pushvalue(State, -2);
	/*MsgBox("-2", "pushvalue");*/
	int i = 2;
	while (!lua_isnil(L, -i)) {
		if (lua_isnumber(L, -i)) {
			LuaC::rbx_pushnumber(State, lua_tonumber(L, -i));
			/*MsgBox(std::to_string(lua_tonumber(L, -i)), "pushnumber");*/
			arg_size++;
		}
		if (lua_isstring(L, -i)) {
			LuaC::rbx_pushstring(State, lua_tostring(L, -i));
			/*MsgBox(lua_tostring(L, -i), "pushstring");*/
			arg_size++;
		}
		if (lua_isboolean(L, -i)) {
			LuaC::rbx_pushboolean(State, lua_toboolean(L, -i));
			/*MsgBox(std::to_string(lua_toboolean(L, -i)), "pushboolean");*/
			arg_size++;
		}
		i++;
	}
	LuaC::rbx_pcall(State, arg_size, 1, 0);
	/*MsgBox(std::to_string(arg_size) + " 1 0", "pcall");*/
	arg_size = 1;
	return 1;
}

int WorkspaceCall(lua_State* L) {
	workspace_didGlobal = false;
	arg_size = 1;
	LuaC::rbx_pushvalue(State, -2);
	/*MsgBox("-2", "pushvalue");*/
	int i = 2;
	while (!lua_isnil(L, -i)) {
		if (lua_isnumber(L, -i)) {
			LuaC::rbx_pushnumber(State, lua_tonumber(L, -i));
			/*MsgBox(std::to_string(lua_tonumber(L, -i)), "pushnumber");*/
			arg_size++;
		}
		if (lua_isstring(L, -i)) {
			LuaC::rbx_pushstring(State, lua_tostring(L, -i));
			/*MsgBox(lua_tostring(L, -i), "pushstring");*/
			arg_size++;
		}
		if (lua_isboolean(L, -i)) {
			LuaC::rbx_pushboolean(State, lua_toboolean(L, -i));
			/*MsgBox(std::to_string(lua_toboolean(L, -i)), "pushboolean");*/
			arg_size++;
		}
		i++;
	}
	LuaC::rbx_pcall(State, arg_size, 1, 0);
	/*MsgBox(std::to_string(arg_size) + " 1 0", "pcall");*/
	arg_size = 1;
	return 1;
}

int Instance_new(lua_State* L) {
	game_didGlobal = false;
	workspace_didGlobal = false;
	int a = lua_gettop(L);
	/*MsgBox("Instance", "getglobal");*/
	LuaC::rbx_getglobal(State, "Instance");
	/*MsgBox("new", "getfield -1");*/
	LuaC::rbx_getfield(State, -1, "new");
	if (lua_isstring(L, 1)) {
		/*MsgBox(lua_tostring(L, 1), "pushstring");*/
		LuaC::rbx_pushstring(State, lua_tostring(L, 1));
		if (a == 2) LuaC::rbx_pushvalue(State, -4);/* MsgBox("-4", "pushvalue");*/
	}
	/*MsgBox(std::to_string(a) + " 1 0", "pcall");*/
	LuaC::rbx_pcall(State, a, 1, 0);
	return 1;
}
