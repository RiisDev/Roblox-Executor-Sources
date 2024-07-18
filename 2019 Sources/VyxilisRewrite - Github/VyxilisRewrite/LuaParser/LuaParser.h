#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <istream>
#include <iterator>
#include <sstream>

#include "../Offsets.h"


extern "C" {
#include "../Lua\lua.h"
#include "../Lua\lapi.h"
#include "../Lua\lualib.h"
#include "../Lua\lstate.h"
#include "../Lua\lauxlib.h"
#include "../Lua\luaconf.h"
#include "../Lua\llimits.h"
#include "../Lua\lapi.h"
#include "../Lua\lfunc.h"
#include "../Lua\lobject.h"
}

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

int UDim2_new(lua_State* L) {
	typeshit = "UDim2";
	xx = lua_tonumber(L, 1);
	yy = lua_tonumber(L, 2);
	zz = lua_tonumber(L, 3);
	return 0;
}


int Instance_new(lua_State* L) {
	game_didGlobal = false;
	workspace_didGlobal = false;
	int a = lua_gettop(L);
	r_lua_getglobal(RLS, "Instance");
	r_lua_getfield(RLS, -1, "new");
	if (lua_isstring(L, 1)) {
		r_lua_pushstring(RLS, lua_tostring(L, 1));
		if (a == 2) r_lua_pushvalue(RLS, -4);
	}
	r_lua_pcall(RLS, a, 1, 0);
	return 1;
}



int GameIndex(lua_State* L) {
	if (game_didGlobal == false) {
		r_lua_getglobal(RLS, "game");
		game_didGlobal = true;
	}
	lua_getglobal(L, "game");
	r_lua_getfield(RLS, -1, lua_tostring(L, -2));
	return 1;
}


int GameNewIndex(lua_State* L) {
	game_didGlobal = true;
	lua_getglobal(L, "game");
	if (typeshit == "Vector3") {
		r_lua_getglobal(RLS, "Vector3");
		r_lua_getfield(RLS, -1, "new");
		r_lua_pushnumber(RLS, xx);
		r_lua_pushnumber(RLS, yy);
		r_lua_pushnumber(RLS, zz);
		r_lua_pcall(RLS, 3, 1, 0);
		r_lua_setfield(RLS, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	if (typeshit == "UDim2") {
		r_lua_getglobal(RLS, "UDim2");
		r_lua_getfield(RLS, -1, "new");
		r_lua_pushnumber(RLS, xx);
		r_lua_pushnumber(RLS, yy);
		r_lua_pushnumber(RLS, zz);
		r_lua_pcall(RLS, 3, 1, 0);
		r_lua_setfield(RLS, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	else if (typeshit == "Color3") {
		r_lua_getglobal(RLS, "Color3");
		r_lua_getfield(RLS, -1, "new");
		r_lua_pushnumber(RLS, xx);
		r_lua_pushnumber(RLS, yy);
		r_lua_pushnumber(RLS, zz);
		r_lua_pcall(RLS, 3, 1, 0);
		r_lua_setfield(RLS, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	else {
		if (lua_isnumber(L, -2)) {
			r_lua_pushnumber(RLS, lua_tonumber(L, -2));
			r_lua_setfield(RLS, -2, lua_tostring(L, -3));
		}
		else if (lua_isboolean(L, -2)) {
			r_lua_pushboolean(RLS, lua_toboolean(L, -2));
			r_lua_setfield(RLS, -2, lua_tostring(L, -3));
		}
		else if (lua_isstring(L, -2)) {
			r_lua_pushstring(RLS, lua_tostring(L, -2));
			r_lua_setfield(RLS, -2, lua_tostring(L, -3));
		}
		else if (lua_isnil(L, -2)) {
			r_lua_pushnil(RLS);
			r_lua_setfield(RLS, -2, lua_tostring(L, -3));
		}
	}
	return 1;
}



int WorkspaceIndex(lua_State* L) {
	if (workspace_didGlobal == false) {
		r_lua_getglobal(RLS, "workspace");
		workspace_didGlobal = true;
	}
	lua_getglobal(L, "workspace");
	r_lua_getfield(RLS, -1, lua_tostring(L, -2));
	return 1;
}


int WorkspaceNewIndex(lua_State* L) {
	workspace_didGlobal = false;
	lua_getglobal(L, "workspace");
	if (typeshit == "Vector3") {
		r_lua_getglobal(RLS, "Vector3");
		r_lua_getfield(RLS, -1, "new");
		r_lua_pushnumber(RLS, xx);
		r_lua_pushnumber(RLS, yy);
		r_lua_pushnumber(RLS, zz);
		r_lua_pcall(RLS, 3, 1, 0);
		r_lua_setfield(RLS, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	if (typeshit == "UDim2") {
		r_lua_getglobal(RLS, "UDim2");
		r_lua_getfield(RLS, -1, "new");
		r_lua_pushnumber(RLS, xx);
		r_lua_pushnumber(RLS, yy);
		r_lua_pushnumber(RLS, zz);
		r_lua_pcall(RLS, 3, 1, 0);
		r_lua_setfield(RLS, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	else if (typeshit == "Color3") {
		r_lua_getglobal(RLS, "Color3");
		r_lua_getfield(RLS, -1, "new");
		r_lua_pushnumber(RLS, xx);
		r_lua_pushnumber(RLS, yy);
		r_lua_pushnumber(RLS, zz);
		r_lua_pcall(RLS, 3, 1, 0);
		r_lua_setfield(RLS, -3, lua_tostring(L, -3));
		xx, yy, zz = 0;
		typeshit = "";
	}
	else {
		if (lua_isnumber(L, -2)) {
			r_lua_pushnumber(RLS, lua_tonumber(L, -2));
			r_lua_setfield(RLS, -2, lua_tostring(L, -3));
		}
		else if (lua_isboolean(L, -2)) {
			r_lua_pushboolean(RLS, lua_toboolean(L, -2));
			r_lua_setfield(RLS, -2, lua_tostring(L, -3));
		}
		else if (lua_isstring(L, -2)) {
			r_lua_pushstring(RLS, lua_tostring(L, -2));
			r_lua_setfield(RLS, -2, lua_tostring(L, -3));
		}
		else if (lua_isnil(L, -2)) {
			r_lua_pushnil(RLS);
			r_lua_setfield(RLS, -2, lua_tostring(L, -3));
		}
	}
	return 1;
}


int GameCall(lua_State* L) {
	game_didGlobal = false;
	arg_size = 1;
	r_lua_pushvalue(RLS, -2);
	int i = 2;
	while (!lua_isnil(L, -i)) {
		if (lua_isnumber(L, -i)) {
			r_lua_pushnumber(RLS, lua_tonumber(L, -i));
			arg_size++;
		}
		if (lua_isstring(L, -i)) {
			r_lua_pushstring(RLS, lua_tostring(L, -i));
			arg_size++;
		}
		if (lua_isboolean(L, -i)) {
			r_lua_pushboolean(RLS, lua_toboolean(L, -i));
			arg_size++;
		}
		i++;
	}
	r_lua_pcall(RLS, arg_size, 1, 0);
	arg_size = 1;
	return 1;
}


int WorkspaceCall(lua_State* L) {
	workspace_didGlobal = false;
	arg_size = 1;
	r_lua_pushvalue(RLS, -2);
	int i = 2;
	while (!lua_isnil(L, -i)) {
		if (lua_isnumber(L, -i)) {
			r_lua_pushnumber(RLS, lua_tonumber(L, -i));
			arg_size++;
		}
		if (lua_isstring(L, -i)) {
			r_lua_pushstring(RLS, lua_tostring(L, -i));
			arg_size++;
		}
		if (lua_isboolean(L, -i)) {
			r_lua_pushboolean(RLS, lua_toboolean(L, -i));
			arg_size++;
		}
		i++;
	}
	r_lua_pcall(RLS, arg_size, 1, 0);
	arg_size = 1;
	return 1;
}



int Print(lua_State* L) {
	std::string message;
	int n = lua_gettop(L); 
	int i;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; ++i) {
		if (i > 1)
		{
			message += ' ';
		}

		lua_pushvalue(L, -1);  
		lua_pushvalue(L, i);   
		lua_call(L, 1, 1);
		const char* s = lua_tostring(L, -1);  
		message += s;
		if (i > 1) fputs("\t", stdout);
		lua_pop(L, 1);  
	}
	r_lua_getglobal(RLS, "print");
	r_lua_pushstring(RLS, message.c_str());
	r_lua_pcall(RLS, n, 0, 0);
	return 0;
}


int Warn(lua_State* L) {
	std::string message;
	int n = lua_gettop(L);
	int i;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; ++i) {
		if (i > 1)
		{
			message += ' ';
		}

		lua_pushvalue(L, -1);
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		const char* s = lua_tostring(L, -1);
		message += s;
		if (i > 1) fputs("\t", stdout);
		lua_pop(L, 1);
	}
	r_lua_getglobal(RLS, "warn");
	r_lua_pushstring(RLS, message.c_str());
	r_lua_pcall(RLS, n, 0, 0);
	return 0;
}

int Wait(lua_State* Lua51) {
	DWORD arg = lua_gettop(Lua51);
	float time;
	for (int i = 1; i <= arg; ++i) {
		time = lua_tonumber(Lua51, i);
	}
	Sleep(time * 1000);
	return 0;
}


int StartParser(lua_State* L) {
	lua_newtable(L);
	lua_pushcfunction(L, Instance_new);
	lua_setfield(L, -2, "new");
	lua_setglobal(L, "Instance");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_pushcfunction(L, UDim2_new);
	lua_setfield(L, -2, "new");
	lua_setglobal(L, "UDim2");
	lua_settop(L, 0);
	lua_getglobal(L, "UDim2");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_pushcfunction(L, Vector3_new);
	lua_setfield(L, -2, "new");
	lua_setglobal(L, "Vector3");
	lua_settop(L, 0);
	lua_getglobal(L, "Vector3");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_pushcfunction(L, Color3_new);
	lua_setfield(L, -2, "new");
	lua_setglobal(L, "Color3");
	lua_settop(L, 0);
	lua_getglobal(L, "Color3");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_pushstring(L, "This metatable is locked");
	lua_setfield(L, -2, "__metatable");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushcfunction(L, GameIndex);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, GameNewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, GameCall);
	lua_setfield(L, -2, "__call");
	lua_setmetatable(L, -2);
	lua_setglobal(L, "game");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushcfunction(L, WorkspaceIndex);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, WorkspaceNewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, WorkspaceCall);
	lua_setfield(L, -2, "__call");
	lua_setmetatable(L, -2);
	lua_setglobal(L, "workspace");
	lua_settop(L, 0);
	lua_pushcfunction(L, Warn);
	lua_setglobal(L, "warn");
	lua_settop(L, 0);
	lua_pushcfunction(L, Print);
	lua_setglobal(L, "print");
	lua_settop(L, 0);
	lua_pushcfunction(L, Wait);
	lua_setglobal(L, "wait");
	lua_settop(L, 0);
	lua_pushcfunction(L, Wait);
	lua_setglobal(L, "sleep");
	lua_settop(L, 0);
	return 1;
}


void ExecuteParserScript(std::string Script) {
	lua_State* luaS = luaL_newstate();
	lua_State* LuaS = lua_newthread(luaS);
	luaL_openlibs(LuaS);
	StartParser(LuaS);
	std::string total;
	luaL_dostring(LuaS, Script.c_str());
	lua_settop(LuaS, 0);
	lua_close(LuaS);
}