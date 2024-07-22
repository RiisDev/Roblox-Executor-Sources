#pragma once
#include "Vitals.h"
#include <string>
#include <vector>
#include "Signatures.h"
#include <signal.h>
extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lfunc.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
}

//sry to yeet
//not the real s r c
int library;
static int DataModel;

union globalValue {
	PVOID gc;
	PVOID p;
	double n;
	int b;
};

struct r_TValue {
	globalValue value;
	int tt;
};

class WrapperInternals
{
public:
	static void execute(const char *script);
	static int RobloxGlobalIndex(lua_State *L);
private:
	struct rData;
	static void pushToRoblox(lua_State *L, int rState, int idx);
	static void pushToLua(lua_State *L, int rState, int idx);
	static int functionhandler(lua_State* L);
	static inline void lua_pushfclosure(lua_State* L, LClosure* fc);
	static int rbxFunctionBridge(DWORD rL);
	static inline int Ryse_loadstring(lua_State *lState);
	static inline int Ryse_FetchClipboard(lua_State *lState);
protected:
	static TValue *index2adr(lua_State *L, int idx);
};

inline TValue *WrapperInternals::index2adr(lua_State *L, int idx) {
	if (idx > 0) {
		auto o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue *, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure *func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure *func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue *, luaO_nilobject);
	}
	}
}

struct WrapperInternals::rData {
	int *size;
};

//mLint callcheckHandler(int rL);
inline void WrapperInternals::pushToRoblox(lua_State *L, int rState, int idx) {
	int start, p;
	double pl;
	auto cd = new calldata;
	switch (lua_type(L, idx)) {
	case LUA_TNIL:
		rlua_pushnil(rState);
		break;
	case LUA_TBOOLEAN:
		rlua_pushboolean(rState, lua_toboolean(L, idx));
		break;
	case LUA_TLIGHTUSERDATA:
		rlua_pushlightuserdata(rState, nullptr);
		break;
	case LUA_TNUMBER:
		rlua_pushnumber(rState, index2adr(L, idx)->value.n);
		break;
	case LUA_TSTRING:
		rlua_pushstring(rState, lua_tostring(L, idx));
		break;
	case LUA_TTABLE:
		lua_pushvalue(L, idx);
		rlua_newtable(rState);
		lua_pushnil(L);
		start = lua_gettop(L);
		while (lua_next(L, idx - 1) != LUA_TNIL)
		{
			pushToRoblox(L, rState, -2);
			pushToRoblox(L, rState, -1);
			rlua_settable(rState, -3);
			lua_pop(L, lua_gettop(L) - start);
		}
		lua_pop(L, -1);
		//rlua_rawseti(rState, -1, *(int*)rlua_topointer(rState, -1));
		break;
	case LUA_TFUNCTION:
		lua_pushvalue(L, idx);
		rlua_pushnumber(rState, luaL_ref(L, LUA_REGISTRYINDEX));
		rlua_pushcclosure(rState, int3bpt, 1);
		break;
	case LUA_TUSERDATA:
		rlua_rawgeti(rState, -10000, *(int*)lua_topointer(L, idx));
		break;
	}
}

inline int WrapperInternals::functionhandler(lua_State* L) {
	int key = lua_tonumber(lua_state, lua_upvalueindex(1));
	auto args = lua_gettop(lua_state);
	rlua_rawgeti(rState, -10000, key);
	for (int i = args; i != 0; i--) {
		pushToRoblox(lua_state, rState, i * -1);
	}
	rlua_pcall(rState, args, 1, 0);
	pushToLua(lua_state, rState, -1);
	return 1;
}

inline void WrapperInternals::pushToLua(lua_State *L, int rState, int idx) {
	int adr, start;
	std::cout << rlua_type(rState, -1) << std::endl;
	switch (rlua_type(rState, idx)) {
	case RLUA_TNIL:
		lua_pushnil(L);
		break;
	case RLUA_TBOOLEAN:
		lua_pushboolean(L, rlua_toboolean(rState, idx));
		break;
	case RLUA_TLIGHTUSERDATA:
		lua_pushlightuserdata(L, nullptr);
		break;
	case RLUA_TNUMBER:
		lua_pushnumber(L, rlua_tonumber(rState, idx));
		break;
	case RLUA_TSTRING:
		lua_pushstring(L, rlua_tostring(rState, idx));
		break;
	case RLUA_TTABLE:
		std::cout << "ugh" << std::endl;
		rlua_pushvalue(rState, idx);
		std::cout << "ugh" << std::endl;
		lua_newtable(L);
		std::cout << "ugh" << std::endl;
		rlua_pushnil(rState);
		std::cout << "ugh" << std::endl;
		start = rlua_gettop(rState);
		std::cout << "ugh" << std::endl;
		while (rlua_next(rState, idx - 1) != RLUA_TNIL)
		{
			std::cout << "ugh " << rlua_type(rState, -2) << std::endl;
			pushToLua(L, rState, -2);
			std::cout << "ugh " << rlua_type(rState, -1) << std::endl;
			pushToLua(L, rState, -1);
			std::cout << "ugh" << std::endl;
			lua_settable(L, -3);
			rlua_pop(rState, rlua_gettop(rState) - start);
		}

		rlua_pop(rState, 1);
		break;
	case RLUA_TFUNCTION:
		std::cout << "oooogh" << std::endl;
		rlua_pushvalue(rState, idx);
		adr = *static_cast<int*>(rlua_topointer(rState, idx));
		rlua_rawseti(rState, -10000, adr);
		lua_pushnumber(L, adr);
		lua_pushcclosure(L, static_cast<lua_CFunction>(functionhandler), 1);
		break;
	case RLUA_TUSERDATA:
		rlua_pushvalue(rState, idx);
		adr = *static_cast<int*>(rlua_topointer(rState, idx));
		rData *newData = static_cast<rData*>(lua_newuserdata(L, sizeof(int)));
		newData->size = reinterpret_cast<int*>(adr);
		rlua_rawseti(rState, -10000, adr);
		rlua_getmetatable(rState, idx);
		pushToLua(L, rState, -1);
		rlua_pop(rState, 1);
		lua_setmetatable(L, -2);
		break;
	}
}

inline void WrapperInternals::lua_pushfclosure(lua_State* L, LClosure* fc)
{
	TValue* top = L->top;
	top->tt = LUA_TFUNCTION;
	top->value.gc = cast(GCObject*, fc);
	L->top++;
}

inline int WrapperInternals::rbxFunctionBridge(DWORD rL)
{
	auto key = rlua_tonumber(rL, lua_upvalueindex(1));
	lua_rawgeti(lua_state, LUA_REGISTRYINDEX, key);

	for (int arg = 1; arg <= rlua_gettop(rL); ++arg)
		pushToLua(lua_state, rState, arg);

	switch (lua_pcall(lua_state, rlua_gettop(rL), LUA_MULTRET, 0))
	{
	case LUA_YIELD:
		rlua_pushlightuserdata(rState, static_cast<void*>(lua_state));
		rlua_pushcclosure(rState, int3bpt, 1);
		return -1;
	case LUA_ERRRUN:
		printf("Ryse error: %s\n", lua_tostring(lua_state, -1));
		return -1;
	default: break;
	}

	auto args = 0;
	for (auto arg = 1; arg <= lua_gettop(lua_state); ++arg, ++args)
		pushToRoblox(lua_state, rL, arg);

	lua_settop(lua_state, 0);
	return args;
}

inline int WrapperInternals::RobloxGlobalIndex(lua_State *L) {
	auto name = lua_tostring(lua_state, -1);
	std::cout << "gindex: " << name << std::endl;
	rlua_getglobal(rState, name);
	pushToLua(lua_state, rState, -1);
	return 1;
}

inline int WrapperInternals::Ryse_loadstring(lua_State *lState) {
	if (!luaL_checkstring(lua_state, 1)) {
		luaL_error(lua_state, "Argument #1 to 'loadstring' expected to be a string instead got %s", lua_typename(lua_state, -1));
		return 0;
	}
	auto text = lua_tostring(lua_state, 1);
	luaL_dostring(lua_state, text);
	return 1;
}

inline int WrapperInternals::Ryse_FetchClipboard(lua_State *lState) {
	if (!OpenClipboard(nullptr))
		lua_pushstring(lua_state, "");

	auto h = GetClipboardData(CF_TEXT);

	lua_pushstring(lua_state, reinterpret_cast<char* >(h));
	CloseClipboard();
	return 1;
}


inline void WrapperInternals::execute(const char *script)
{
	lua_getmetatable(lua_state, -10002);
	if (lua_type(lua_state, -1) == LUA_TNIL) {
		lua_newtable(lua_state);
		lua_setmetatable(lua_state, -10002);
		lua_getmetatable(lua_state, -10002);
		lua_pushcfunction(lua_state, RobloxGlobalIndex);
		lua_setfield(lua_state, -2, "__index");
	}
	lua_register(lua_state, "getclipboard", Ryse_FetchClipboard);
	lua_register(lua_state, "loadstring", Ryse_loadstring);
	if (luaL_loadbuffer(lua_state, script, strlen(script), "Ryse") != 0) {
		std::cout << lua_tostring(lua_state, -1) << std::endl;
		//rlua_error(3, lua_tostring(lua_state, -1));
	}
	else {
		if (lua_pcall(lua_state, 0, 0, 0) != 0) {
			std::cout << lua_tostring(lua_state, -1) << std::endl;
			//rlua_error(3, lua_tostring(lua_state, -1));
		}
	}
}