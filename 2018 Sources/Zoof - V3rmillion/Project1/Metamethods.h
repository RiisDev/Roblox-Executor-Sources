#pragma once
#include "Definitions.h"
#include "Functions.h"
//Metamethods

bool gamecalled = false;

int game__index(lua_State* L) {
	if (gamecalled == false) {
		rlua_getglobal(rL, "game");
		gamecalled = true;
	}
	lua_getglobal(L, "game");
	rlua_getfield(rL, -1, lua_tostring(L, -2));
	wrap(rL, L, -1);
	return 1;
}

int game__newindex(lua_State *L) {
	gamecalled = false;
	switch (lua_type(L, -1)) {
	case LUA_TNIL:
		rlua_pushnil(rL);
		rlua_setfield(rL, -2, lua_tostring(L, 2));
		break;
	case LUA_TNONE:
		rlua_pushnil(rL);
		rlua_setfield(rL, -2, lua_tostring(L, 2));
	case LUA_TBOOLEAN:
		rlua_pushboolean(rL, lua_toboolean(L, -1));
		rlua_setfield(rL, -2, lua_tostring(L, 2));
		break;
	case LUA_TNUMBER:
		rlua_pushnumber(rL, lua_tonumber(L, -1));
		rlua_setfield(rL, -2, lua_tostring(L, 2));
		break;
	case LUA_TSTRING:
		rlua_pushstring(rL, lua_tostring(L, -1));
		rlua_setfield(rL, -2, lua_tostring(L, 2));
		break;
	default:
		rlua_setfield(rL, -3, lua_tostring(L, 2));
	}
	return 1;
}

int game__call(lua_State *L) {
	gamecalled = false;
	int args = 1;
	rlua_pushvalue(rL, -2);
	int i = 2;
	while (!lua_isnil(L, -i)) {
		unwrap(L, rL, -1);
		i++;
	}
	rlua_call(rL, args, 1);
	if (rlua_isnil(rL, -1)) {
		return 1;
	}
	else {
		wrap(rL, L, -1);
		return 1;
	}
}

void createmetas() {
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushcfunction(L, game__index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, game__call);
	lua_setfield(L, -2, "__call");
	lua_pushcfunction(L, game__newindex);
	lua_setfield(L, -2, "__newindex");
	lua_setmetatable(L, -2);
	lua_setglobal(L, "game");
	lua_settop(L, 0);
}