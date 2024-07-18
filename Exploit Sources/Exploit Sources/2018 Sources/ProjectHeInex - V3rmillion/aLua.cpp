#include "aLua.h"

void CLua::Init() {

	getfield = (lua_GetField)0x515ED0;
	setfield = (lua_SetField)0x516930;
	gettop = (lua_GetTop)0x516ea0;
	pcall = (lua_PCall)0x517340;
	pushvalue = (lua_PushValue)0x517740;
	pushstring = (lua_PushString)0x5176b0;
	tolstring = (lua_ToLString)0x517f70;
	setfield = (lua_SetField)0x517be0;
	type = (lua_Type)0x518250;
	pushboolean = (lua_pushboolean)0x517410;



	/*
	if you need anymore added :
	tolstring: 00517f70
	getfield: 00516d70
	openstate: 0068a700
	getstack: 00728430
	pushstring: 005176b0
	pcall: 00517340
	gettop: 00516ea0
	type: 00518250
	setfield: 00517be0
	pushnil: 00517630
	getinfo: 007282f0
	pushboolean: 005173d0
	insert: 00516f30
	settop: 00517de0
	next: 00517250
	pushcclosure: 00517410
	load: 005170d0
	pushvalue: 00517740
	*/
}


