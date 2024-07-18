#include <vector>
#include <sstream>
#include <fstream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <string>
#include <windows.h> 
#include <iostream> 
#include <process.h>
#include <Shlwapi.h>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <unordered_map>
#include "Stuffs.h"
#include "Rlua.h"

extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\lapi.h"
#include "Lua\lualib.h"
#include "Lua\lobject.h"
#include "Lua\lstate.h"
#include "Lua\lgc.h"
#include "Tools.h"
}

std::vector<int> Breakpoints;
std::unordered_map<void*, void*> robloxInstanceMap;
std::unordered_map<void*, void*> vanillaInstanceMap;
static int WrapCallHandler(lua_State* L);
static int UnWrapCallHandler(int rL);
int Registry;
lua_State* Vanilla;
void wrap(lua_State* L, int rL, int idx);
void unwrap(lua_State* L, int rL, int idx);

struct UserDataCachingStruct {
	int ReferenceToUserData;
};


typedef int UserDataHandle;

int DataPointer;
struct UserDataCache {
	int* UserDataSize;
};


static TValue* idx2adr(lua_State* L, int idx) {
	if (idx > 0) {
		TValue* o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue*, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure* func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure* func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue*, luaO_nilobject);
	}
	}
}

static int GarbageCollector(lua_State* L) {
	void* NUD = (void*)rlua_touserdata(luaState, -1);
	lua_pushvalue(L, 1);
	lua_gettable(L, LUA_REGISTRYINDEX);
	if (!lua_isnil(L, -1)) {
		int Reference = lua_tointeger(L, -1);
		rlua_pushnil(luaState);
		rlua_rawseti(luaState, LUA_REGISTRYINDEX, Reference);
		rlua_settop(luaState, 0);
		//writerf(1, "Garbage Collected!");
		return 1;
	}
	lua_pop(L, 1);
	return 0;
}


static int GetThreadPointer(lua_State* L) {
	lua_getfield(L, -10000, "__thread");
	int Thread = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return Thread;
}

static int DownloadString(lua_State* L) {
	if (lua_gettop(L) == 2) {
		lua_pushstring(L, DownloadURL(lua_tostring(L, -2)).c_str());
	}
	else {
		lua_pushstring(L, DownloadURL(lua_tostring(L, -1)).c_str());
	}
	return 1;
}

static int GetObjects(lua_State* L) {
	int State = GetThreadPointer(L);
	const char* id = lua_tostring(L, 1);
	lua_newtable(L);
	lua_pushnumber(L, 1);
	r_lua_getglobal(State, "game");
	rlua_getfield(State, -1, "GetService");
	rlua_pushvalue(State, -2);
	rlua_pushstring(State, "InsertService");
	rlua_pcall(State, 2, 1, 0);
	rlua_getfield(State, -1, "LoadLocalAsset");
	rlua_pushvalue(State, -2);
	rlua_pushstring(State, id);
	rlua_pcall(State, 2, 1, 0);
	wrap(L, State, -1);
	lua_settable(L, -3);
	return 1;
}

int Index(lua_State* L) {
	const char* key = lua_tostring(L, -1);
	unwrap(L, luaState, 1);
	rlua_getfield(luaState, -1, key);
	wrap(L, luaState, -1);
	return 1;
}

const char* typestr(int type) {
	if (type == RLUA_TBOOLEAN) {
		return "boolean";
	}
	else if (type == RLUA_TFUNCTION) {
		return "function";
	}
	else if (type == RLUA_TLIGHTUSERDATA) {
		return "lightuserdata";
	}
	else if (type == RLUA_TNIL) {
		return "nil";
	}
	else if (type == -1) {
		return "none";
	}
	else if (type == RLUA_TNUMBER) {
		return "number";
	}
	else if (type == RLUA_TSTRING) {
		return "string";
	}
	else if (type == RLUA_TTABLE) {
		return "table";
	}
	else if (type == RLUA_TTHREAD) {
		return "thread";
	}
	else if (type == RLUA_TUSERDATA) {
		return "userdata";
	}
	else
	{
		return "Bruh fucking instance please.";
	}
}

void DisplayStack(int ls) {
	int size = r_lua_gettop(ls);
	printf(" -- Stack Start --\n");
	for (int at = -11; at < size; at++) {
		printf(" --- at %d - %s\n", at + 1, typestr(rlua_type(ls, at + 1)));
	}
	printf(" -- Stack End --\n");
}

void wrap(lua_State* L, int rL, int idx) {
	switch (rlua_type(rL, idx))
	{
	case RLUA_TLIGHTUSERDATA:
		lua_pushlightuserdata(L, (void*)rlua_touserdata(rL, idx));
		break;
	case RLUA_TNIL:
		lua_pushnil(L);
		break;
	case RLUA_TNUMBER:
		lua_pushnumber(L, rlua_tonumber(rL, idx));
		break;
	case RLUA_TBOOLEAN:
		lua_pushboolean(L, rlua_toboolean(rL, idx));
		break;
	case RLUA_TTHREAD:
		lua_newthread(L);
		break;
	case RLUA_TSTRING:
		lua_pushstring(L, rlua_tolstring(rL, idx, 0));
		break;
	case RLUA_TFUNCTION: 
	{
		rlua_pushvalue(rL, idx);
		lua_pushinteger(L, rluaL_ref(rL, LUA_REGISTRYINDEX));
		lua_pushcclosure(L, WrapCallHandler, 1);
		break;
	}
	case RLUA_TTABLE: 
	{
		rlua_pushvalue(rL, idx);
		lua_newtable(L);
		rlua_pushnil(rL);
		while (rlua_next(rL, -2))
		{
			wrap(L, rL, -2);
			wrap(L, rL, -1);
			lua_settable(L, -3);
			r_lua_pop(rL, 1);
		}
		r_lua_pop(rL, 1);
		break;
	}
	case RLUA_TUSERDATA: {
		const auto robloxPtr = (void*)rlua_touserdata(rL, idx);/*grabs a userdata from it's index*/
		const auto iterator = vanillaInstanceMap.find((void*)robloxPtr);/*find's the roblox's userdata from it's current mapped*/

		if (iterator == vanillaInstanceMap.cend()) { /*uses cend to cache all the used stuff*/
			rlua_pushvalue(rL, idx);
			reinterpret_cast<Userdata*>(lua_newuserdata(L, sizeof(Userdata)))->reference = rluaL_ref(rL, LUA_REGISTRYINDEX);/*calls roblox's ref by using newuserdata*/

			rlua_getmetatable(rL, idx);/*grabs the metatable to it's index*/

			wrap(L, rL, -1);/*push to vanilla*/

			lua_pushcfunction(L, GarbageCollector);
			lua_setfield(L, -2, "__gc");

			lua_pushcfunction(L, Index);
			lua_setfield(L, -2, "__index");

			lua_setmetatable(L, -2);/*set the metatable*/

			lua_pushvalue(L, -1);/*push the value and set it to -1 (index)*/
			lua_rawseti(L, LUA_REGISTRYINDEX, ++Registry);/*rawseti the registryindex aka LUA_REGISTRYINDEX (-10000) but it's set as a var*/

			vanillaInstanceMap[robloxPtr] = (void*)Registry;/*used vanilla cache map*/

			r_lua_pop(rL, 1);/*pop the stack lol*/

		}
		else
		{

			lua_rawgeti(L, LUA_REGISTRYINDEX, (int)iterator->second);
		}
		break;
	}
	default:	
		rlua_pushnil(rL);
	}
}



void unwrap(lua_State* L, int rL, int idx) {
	switch (lua_type(L, idx))
	{
	case LUA_TLIGHTUSERDATA:
		rlua_pushlightuserdata(rL, lua_touserdata(L, idx));
		break;
	case LUA_TNIL:
		rlua_pushnil(rL);
		break;
	case LUA_TNUMBER:
		rlua_pushnumber(rL, lua_tonumber(L, idx));
		break;
	case LUA_TBOOLEAN:
		rlua_pushboolean(rL, lua_toboolean(L, idx));
		break;
	case LUA_TTHREAD:
		rlua_newthread(rL);
		break;
	case LUA_TSTRING: 
		rlua_pushstring(rL, lua_tostring(L, idx));
		break;
	case LUA_TFUNCTION: 
	{
		lua_pushvalue(L, idx);
		rlua_pushnumber(rL, luaL_ref(L, LUA_REGISTRYINDEX));
		rlua_pushccfunction(rL, (DWORD)UnWrapCallHandler, 1);
		break;
	}
	case LUA_TTABLE: 
	{
		lua_pushvalue(L, idx);
		lua_getfield(L, LUA_GLOBALSINDEX, "tostring");
		lua_pushvalue(L, -2);
		lua_pcall(L, 1, 1, 0);
		const char* a = lua_tolstring(L, -1, NULL);
		lua_settop(L, -2);
		rlua_createtable(rL, 0, 0);
		lua_pushnil(L);
		while (lua_next(L, -2)) {
			lua_pushvalue(L, -1);
			lua_getfield(L, LUA_GLOBALSINDEX, "tostring");
			lua_pushvalue(L, -2);
			lua_pcall(L, 1, 1, 0);
			const char* b = lua_tolstring(L, -1, NULL);
			lua_settop(L, -3);
			if (strcmp(a, b) == 0) {
				lua_settop(L, -2);
				continue;
			}
			unwrap(L, rL, -2);
			unwrap(L, rL, -1);
			rlua_settable(rL, -3);
			lua_settop(L, -2);
		}
		lua_settop(L, -2);
		break;
	}
	case LUA_TUSERDATA:
	{
		rlua_rawgeti(rL, LUA_REGISTRYINDEX, reinterpret_cast<Userdata*>(lua_touserdata(L, idx))->reference);
		break;
	}
	default: 
		lua_pushnil(L);
	}
}

static int resume(lua_State* rL)
{
	lua_State* To = lua_tothread(rL, lua_upvalueindex(1));
	const int nargs = lua_gettop(rL);
	lua_xmove(rL, To, nargs);
	return lua_resume(To, nargs);
	lua_newtable(To);
	lua_pushstring(To, "This metatable is locked");
	lua_setfield(To, -2, "__metatable");
}

static int WrapCallHandler(lua_State* L) {
	DWORD rL = rlua_newthread(luaState);
	rlua_setfield(luaState, LUA_REGISTRYINDEX, to_string(++Registry).c_str());
	int Args = lua_gettop(L);
	rlua_rawgeti(rL, LUA_REGISTRYINDEX, lua_tointeger(L, lua_upvalueindex(1)));
	for (int i = 1; i <= Args; ++i)
		unwrap(L, rL, i);
	if (rlua_pcall(rL, Args, LUA_MULTRET, 0))
	{
		string Message = rlua_tolstring(rL, -1, 0);
		if (Message.empty()) Message = "Error occured, no output from Lua\n";
		if (Message == "attempt to yield across metamethod/C-call boundary")
		{
			r_lua_pop(rL, 1);
			lua_pushthread(L);
			lua_pushcclosure(L, &resume, 1);
			unwrap(L, rL, -1);
			return lua_yield(L, 0);
		}
		return luaL_error(L, Message.c_str());
	}
	int Return = r_lua_gettop(rL);
	for (int i = 1; i <= Return; ++i)
		wrap(L, rL, i);
	rlua_settop(rL, 0);
	rlua_pushnil(luaState);
	rlua_setfield(luaState, LUA_REGISTRYINDEX, to_string(Registry).c_str());
	return Return;
}

static int UnWrapCallHandler(int rL) {
	lua_State* L = lua_newthread(Vanilla);
	lua_setfield(Vanilla, LUA_REGISTRYINDEX, to_string(++Registry).c_str());
	int Args = r_lua_gettop(rL);
	lua_rawgeti(L, LUA_REGISTRYINDEX, cast_int(rlua_tonumber(rL, lua_upvalueindex(1))));
	for (int i = 1; i <= Args; ++i)
		wrap(L, rL, i);
	if (lua_pcall(L, Args, LUA_MULTRET, 0)) {

		return rluaL_error(rL, luaL_checkstring(L, -1));
	}
	int Return = lua_gettop(L);
	for (int i = 1; i <= Return; ++i)
		unwrap(L, rL, i);
	lua_settop(L, 0);
	lua_pushnil(Vanilla);
	lua_setfield(Vanilla, LUA_REGISTRYINDEX, to_string(Registry).c_str());
	return Return;
}