#pragma once
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

#include "Rlua.h"
#include "Converter.h"
#include "Stuffs.h"
#include "Utils.h"

extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\lapi.h"
#include "Lua\lualib.h"
#include "Lua\lobject.h"
#include "Lua\lstate.h"
}

using namespace std;

void SetThreadPointer(lua_State *L, int State) {
	lua_pushinteger(L, State);
	lua_setfield(L, LUA_REGISTRYINDEX, "__thread");
	lua_settop(L, 0);
}

static int GlobalIndexTable(lua_State *L) {
	if (lua_type(L, 2) != LUA_TSTRING) return luaL_error(L, "invalid string index");
	rlua_getfield(luaState, -10002, lua_tostring(L, 2));
	if (rlua_type(luaState, -1) > LUA_TNIL) {
		wrap(Vanilla, luaState, -1);
		r_lua_pop(luaState, 1);
		return 1;
	}
	return 0;
}

static TValue *index2adr(lua_State *L, int idx) {
	if (idx > 0) {
		TValue *o = L->base + (idx - 1);
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

static TValue* wrapidx2adr(lua_State* L, int idx) {
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


void GetGlobals() {
	std::vector<std::string> rbxfuncs{
	"printidentity","game","Game","workspace","Workspace","PluginDrag","DateTime",
	"Axes","BrickColor","CFrame","Color3","ColorSequence","ColorSequenceKeypoint","EnumItem",
	"NumberRange","NumberSequence","NumberSequenceKeypoint","PhysicalProperties","Ray",
	"Rect","Region3","Region3int16","TweenInfo","UDim","UDim2","Vector2","RBXScriptSignal","shared",
	"Vector2int16","Vector3","Vector3int16","Enum","Faces","RaycastParams","Object","GenericFunction",
	"Instance","math","warn","typeof","type","spawn","Spawn","print","Random","bit",
	"ypcall","pcall","Wait","wait","delay","Delay","tick","LoadLibrary","Enums","CellId","utf8"
	};

	for (auto func : rbxfuncs) {
		r_lua_getglobal(luaState, func.c_str());
		wrap(Vanilla, luaState, -1);
		lua_setfield(Vanilla, LUA_GLOBALSINDEX, func.c_str());
		r_lua_pop(luaState, 1);
	}
}

int SpawnHandler(int CurrentState) {
	SetThreadPointer(Vanilla, CurrentState);
	const char* Script = rlua_tolstring(CurrentState, lua_upvalueindex(1), 0);
	r_lua_pop(CurrentState, 1);
	std::string ExecScript;
	ExecScript = "spawn(function() script = Instance.new'LocalScript' script.Disabled = true script.Parent = nil\r\n";
	ExecScript += Script;
	ExecScript += "\r\nend)";
	if (luaL_loadbuffer(Vanilla, ExecScript.c_str(), ExecScript.length(), "@Eclipse") || lua_pcall(Vanilla, 0, LUA_MULTRET, 0)) {
		writerf(3, lua_tostring(Vanilla, -1));
		lua_pop(Vanilla, 1);
	}
	return 1;
}

void ExecuteScript(const char* Script) {
	int NewThread = rlua_newthread(luaState);
	rlua_pushstring(NewThread, Script);
	rlua_pushccfunction(NewThread, (int)SpawnHandler, 1);
	Spawn(NewThread);
	rlua_settop(NewThread, 0);
}