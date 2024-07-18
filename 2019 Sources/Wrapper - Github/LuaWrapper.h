#pragma once
#include <Windows.h>
#include <string>
#include <iostream>

#include "Offsets.h"
#include "ConsoleCol.h"
#include "Authentication.h"


int RobloxRegistry;

int FunctionHandler(lua_State* L);

void Wrap(DWORD RLS, lua_State* LS, int Index) {
	//NOTE: remove the cout for speed
	//std::cout << white << "Vanilla Type Used: " << r_l_type(RLS, Index) << "\n";
	switch (r_l_type(RLS, Index))
	{
	case R_LUA_TNIL: {
		lua_pushnil(LS);
		break;
	}
	case R_LUA_TLIGHTUSERDATA: {
		lua_pushlightuserdata(LS, r_l_touserdata(RLS, Index));
		break;
	}
	case R_LUA_TNUMBER: {
		lua_pushnumber(LS, r_l_tonumber(RLS, Index));
		break;
	}
	case R_LUA_TBOOLEAN: {
		lua_pushboolean(LS, r_l_toboolean(RLS, Index));
		break;
	}
	case R_LUA_TSTRING: {
		lua_pushstring(LS, r_l_tostring(RLS, Index));
		break;
	}
	case R_LUA_TTHREAD: {
		lua_newthread(LS);
		break;
	}
	case R_LUA_TFUNCTION: {
		r_l_pushvalue(RLS, Index);
		lua_pushnumber(LS, r_l_lref(RLS, LUA_REGISTRYINDEX));
		lua_pushcclosure(LS, FunctionHandler, 1);
		break;
	}
	case R_LUA_TTABLE: {
		r_l_pushvalue(RLS, Index);
		r_l_gettable(RLS, LUA_REGISTRYINDEX);
		if (!r_l_isnil(RLS, -1)) {
			lua_getfield(LS, LUA_REGISTRYINDEX, r_l_tostring(RLS, -1));
		}
		else {
			r_l_settop(RLS, -2);
			/*table caching*/
			r_l_pushvalue(RLS, Index);
			r_l_pushstring(RLS, std::to_string(++RobloxRegistry).c_str());
			r_l_pushvalue(RLS, -2);
			r_l_settable(RLS, LUA_REGISTRYINDEX);
			/*table caching*/
			lua_newtable(LS);
			r_l_pushnil(RLS);
			while (r_l_next(RLS, -2) != R_LUA_TNIL)
			{
				Wrap(RLS, LS, -2);
				Wrap(RLS, LS, -1);
				lua_settable(LS, -3);
				r_l_pop(RLS, 1);
			}
		}
		r_l_pop(RLS, 1);
		break;
	}
	case R_LUA_TUSERDATA: {
		r_l_pushvalue(RLS, Index);
		r_l_gettable(RLS, LUA_REGISTRYINDEX);
		if (!(r_l_type(RLS, -1) == 0)) {
			lua_getfield(LS, LUA_REGISTRYINDEX, r_l_tostring(RLS, -1));
			r_l_settop(RLS, -3);
			break;
		}
		else {
			r_l_settop(RLS, -2);
			r_l_pushvalue(RLS, Index);
			r_l_pushstring(RLS, std::to_string(++RobloxRegistry).c_str());
			r_l_pushvalue(RLS, -2);
			r_l_settable(RLS, LUA_REGISTRYINDEX);
			r_l_pop(RLS, 1);
			lua_newuserdata(LS, 0);
			lua_pushvalue(LS, -1);
			lua_pushstring(LS, std::to_string(RobloxRegistry).c_str());
			lua_settable(LS, LUA_REGISTRYINDEX);
			r_l_getmetatable(RLS, Index);
			Wrap(RLS, LS, -1);
			r_l_pop(RLS, 1);
			lua_setmetatable(LS, -2);
		}
		break;
	}
	case R_LUA_TPROTO: {
		luaF_newproto(LS);
		break;
	}
	case R_LUA_TUPVALUE: {
		luaF_newupval(LS);
		break;
	}
	case R_LUA_TNONE: {
		lua_pushnil(LS);
		break;
	}
	default: break;
	}
}


void UnWrap(lua_State* LS, DWORD RLS, int Index) {
	//NOTE: remove the cout for speed
	//std::cout << white << "Roblox Type Used: " << lua_type(LS, Index) << "\n";
	switch (lua_type(LS, Index))
	{
	case LUA_TNIL: {
		r_l_pushnil(RLS);
		break;
	}
	case LUA_TLIGHTUSERDATA: {
		r_l_pushlightuserdata(RLS, lua_touserdata(LS, Index));
		break;
	}
	case LUA_TNUMBER: {
		r_l_pushnumber(RLS, lua_tonumber(LS, Index));
		break;
	}
	case LUA_TBOOLEAN: {
		r_l_pushboolean(RLS, lua_toboolean(LS, Index));
		break;
	}
	case LUA_TSTRING: {
		r_l_pushstring(RLS, lua_tostring(LS, Index));
		break;
	}
	case LUA_TTHREAD: {
		r_l_newthread(RLS);
		break;
	}
	case LUA_TFUNCTION: {
		lua_pushvalue(LS, Index);
		r_l_pushnumber(RLS, luaL_ref(LS, LUA_REGISTRYINDEX));
		r_l_pushcclosure(RLS, bpint3[0], 1);
		break;
	}
	case LUA_TTABLE: {
		lua_pushvalue(LS, Index);
		lua_gettable(LS, LUA_REGISTRYINDEX);
		if (!lua_isnil(LS, -1)) {
			r_l_getfield(RLS, LUA_REGISTRYINDEX, lua_tostring(LS, -1));
		}
		else {
			lua_settop(LS, -2);
			/*table caching*/
			lua_pushvalue(LS, Index);
			lua_pushstring(LS, std::to_string(++RobloxRegistry).c_str());
			lua_pushvalue(LS, -2);
			lua_settable(LS, LUA_REGISTRYINDEX);
			/*table caching*/
			r_l_createnewtable(RLS);
			lua_pushnil(LS);
			while (lua_next(LS, -2) != LUA_TNIL)
			{
				UnWrap(LS, RLS, -2);
				UnWrap(LS, RLS, -1);
				r_l_settable(RLS, -3);
				lua_pop(LS, 1);
			}
		}
		lua_pop(LS, 1);
		break;
	}
	case LUA_TUSERDATA: {
		lua_pushvalue(LS, Index);
		lua_gettable(LS, LUA_REGISTRYINDEX);
		if (lua_type(LS, -1) != 0) {
			r_l_getfield(RLS, LUA_REGISTRYINDEX, lua_tostring(LS, -1));
			lua_pop(LS, 1);
			break;
		}
		else {
			lua_settop(LS, -2);
			lua_pushvalue(LS, Index);
			lua_pushstring(LS, std::to_string(++RobloxRegistry).c_str());
			lua_pushvalue(LS, -2);
			lua_settable(LS, LUA_REGISTRYINDEX);
			lua_pop(LS, 1);
			r_l_newuserdata(RLS, 0);
			r_l_pushvalue(RLS, -1);
			r_l_pushstring(RLS, std::to_string(RobloxRegistry).c_str());
			r_l_settable(RLS, LUA_REGISTRYINDEX);
			lua_getmetatable(LS, Index);
			UnWrap(LS, RLS, -1);
			lua_pop(LS, 1);
			r_l_setmetatable(RLS, -2);
		}
		break;
	}
	//tproto
	//tupvalue
	case LUA_TNONE: {
		r_l_pushnil(RLS);
		break;
	}
	default: break;
	}
}

static int LUAYield(lua_State* thread)
{
	lua_State* L = lua_tothread(thread, lua_upvalueindex(1));
	const int nargs = lua_gettop(thread);
	lua_xmove(thread, L, nargs);
	return lua_resume(L, nargs);
}


int RLUAYield(DWORD thread)
{
	lua_State* L = (lua_State*)r_l_touserdata(thread, lua_upvalueindex(1));
	const int nargs = r_l_gettop(thread);
	for (int arg = 1; arg <= nargs; ++arg) {
		Wrap(thread, L, arg);
	}
	return lua_resume(L, nargs);
	lua_close(L);
}


int FunctionHandler(lua_State* L) {
	r_l_pushstring(RLS, std::to_string(++RobloxRegistry).c_str());
	DWORD rL = r_l_newthread(RLS);
	r_l_settable(RLS, LUA_REGISTRYINDEX);
	int key = lua_tonumber(L, lua_upvalueindex(1));
	r_l_rawgeti(rL, LUA_REGISTRYINDEX, key);
	for (int arg = 1; arg <= lua_gettop(L); ++arg) {
		UnWrap(L, rL, arg);
	}
	if (!r_l_pcall(rL, lua_gettop(L), LUA_MULTRET, 0)) {
		int args = 0;
		for (int arg = 1; arg <= r_l_gettop(rL); ++arg, ++args) {
			Wrap(rL, L, arg);
		}
		r_l_settop(rL, 0);
		return args;
	}
	else {
		std::string err = r_l_tostring(rL, -1);
		if (err == (std::string)"attempt to yield across metamethod/C-call boundary")
		{
			r_l_pop(rL, 1);
			lua_pushthread(L);
			lua_pushcclosure(L, &LUAYield, 1);
			UnWrap(L, rL, -1);
			return lua_yield(L, 0);
		}
		else
		{
			std::cout << ired << "Lua API Error: " << err << "\n";
			return 0;
		}
		std::cout << ired << "Lua API Error: " << "No Error Supplied!" << "\n";
		return 0;
	}
}

int int3fnhandler(DWORD rL) {
	lua_pushstring(LS, std::to_string(++RobloxRegistry).c_str());
	lua_State* L = lua_newthread(LS);
	lua_settable(LS, LUA_REGISTRYINDEX);
	int key = r_l_tonumber(rL, lua_upvalueindex(1));
	lua_rawgeti(L, LUA_REGISTRYINDEX, key);
	for (int arg = 1; arg <= r_l_gettop(rL); ++arg) {
		Wrap(rL, L, arg);
	}
	int Err = lua_pcall(L, r_l_gettop(rL), LUA_MULTRET, 0);
	if (Err == LUA_ERRRUN) {
		std::cout << ired << "Roblox API Error: " << lua_tostring(L, -1) << "\n";
	}
	if (Err == LUA_YIELD) {
		r_l_pushlightuserdata(RLS, (void*)L);
		r_l_pushcclosure(RLS, bpint3[2], 1);
	}
	int args = 0;
	for (int arg = 1; arg <= lua_gettop(L); ++arg, ++args) {
		UnWrap(L, rL, arg);
	}
	lua_settop(L, 0);
	return args;
}



static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

char genRandom()
{
	return alphanum[rand() % stringLength];
}

static int GlobalsIndex(lua_State* Thread) {
	int RThread = r_l_newthread(RLS);
	r_l_getglobal(RThread, lua_tostring(Thread, 2));
	if (r_l_type(RThread, -1) > R_LUA_TNIL) {
		Wrap(RThread, Thread, -1);
		return 1;
	}
	return 0;
}


using namespace std;




namespace Wrapper {
	void Execute_Script(std::string Script) {

		//Random ScriptName Generator
		srand(time(0));
		std::string Str;
		for (unsigned int i = 0; i < 20; ++i)
		{
			Str += genRandom();
		}

		Script = "spawn(function()script=Instance.new(\"LocalScript\") script.Name = \"" + Str + "\" \r\n" + Script + "\r\nend)";

		Script = replaceAll(Script, "game:HttpGetAsync", "HttpGet");
		Script = replaceAll(Script, "game:HttpGet", "HttpGet");

		Script = replaceAll(Script, "game:GetObjects", "GetObjects");

		//Run The Script
		if (luaL_loadstring(LS, Script.c_str())) {
			return;
		}
		else {
			lua_pcall(LS, 0, 0, 0);
		}

		GC(LS);
	}
}



