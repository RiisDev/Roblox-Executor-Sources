#pragma once
#include <Windows.h>
#include "r_lua.hpp"

extern "C" {
#include "Lua//lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lstate.h"
#include "Lua/lobject.h"
}
void InitLua(int rstate, lua_State *L, const char* src);
DWORD rL;

#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 1
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 3
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 5
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 8

int r_PushObject_id(int state, lua_State *L, int id) {
	rlua_pushvalue(state, id);
	int ref = rluaL_ref(state, LUA_REGISTRYINDEX);
	int l_u = (int)lua_newuserdata(L, 8);
	*(int*)(l_u) = ref;
	*(int*)(l_u + 1) = 1;

	lua_getglobal(L, "robject");
	lua_setmetatable(L, -2);
	return 0;
}



DWORD int3breakpoint = aslr_2(0x401006);
int wrap(lua_State *L, int rstate, int pos);
static int unwrap(lua_State *L, int rstate, int pos) { //case LUA_
	int res = 0;
	switch (lua_type(L, pos)) {
	case LUA_TNIL: {
		res += 1;
		rlua_pushnil(rstate);
		break;
	}
	case LUA_TLIGHTUSERDATA:#pragma once
#include <Windows.h>
#include "r_lua.hpp"
#include <cstdint>
#include <unordered_map>
extern "C" {
#include "Lua//lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lstate.h"
#include "Lua/lobject.h"
}
void InitLua(int rstate, lua_State *L, const char* src);
DWORD rL;

#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 1
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 3
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 5
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 8

int r_PushObject_id(int state, lua_State *L, int id) {
	rlua_pushvalue(state, id);
	int ref = rluaL_ref(state, LUA_REGISTRYINDEX);
	int l_u = (int)lua_newuserdata(L, 8);
	*(int*)(l_u) = ref;
	*(int*)(l_u + 1) = 1;

	lua_getglobal(L, "robject");
	lua_setmetatable(L, -2);
	return 0;
}
void pushObject(DWORD pRobloxState, TValue *value) {
	auto &top = *(TValue**)(pRobloxState + 24);

	*top = *value;
	++top;
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


std::unordered_map<void*, void*> robloxInstanceMap;
std::unordered_map<void*, void*> vanillaInstanceMap;
DWORD int3breakpoint = aslr_2(0x401006);
int wrap(lua_State *L, int rstate, int pos);
static int unwrap(lua_State *L, int rstate, int pos) { //case LUA_
	int res = 0;
	switch (lua_type(L, pos)) {
	case LUA_TNIL: {
		res += 1;
		rlua_pushnil(rstate);
		break;
	}
	case LUA_TLIGHTUSERDATA:
		printf("unsupported lua type\n");
		rlua_pushstring(rstate, "unsupported lua type");
		res += 1;
		break;
	case LUA_TTHREAD:
		res += 1;
		rlua_newthread(rstate);
		//printf("Thread type\n");
		break;
	case LUA_TNUMBER: {
		res += 1;
		r_pushnumber(rstate, lua_tonumber(L, pos));
		break;
	}
	case LUA_TBOOLEAN:
		res += 1;
		rlua_pushboolean(rstate, lua_toboolean(L, pos));
		break;
	case LUA_TSTRING:
		printf("wrapping string\n");
		res += 1;
		rlua_pushstring(rstate, lua_tolstring(L, pos, 0));
		break;
	case LUA_TFUNCTION: {
		res += 1;
		lua_pushvalue(L, pos);
		printf("pushed value\n");
		int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		r_pushnumber(rstate, ref);
		printf("pushed ref\n");
		r_pushnumber(rstate, (int)L);
		printf("pushed state\n");
		rlua_pushcclosure(rstate, int3breakpoint, 2);
		printf("passed pushcclosure\n");
		break;
	}
	case LUA_TTABLE: {
		lua_pushvalue(L, pos);
		rlua_createtable(rstate, 0, 0);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			unwrap(L, rstate, -2);
			unwrap(L, rstate, -1);
			rlua_settable(rstate, -3);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		res += 1;
		break;
	}
	case LUA_TUSERDATA:
		void *userdataInstance = index2adr(L, pos)->value.p;
		const auto iterator = robloxInstanceMap.find(userdataInstance);

		rlua_rawgeti(rstate, LUA_REGISTRYINDEX, *(int*)(lua_topointer(L, pos)));
		res += 1;
		break;

	}
	return res;
}

static int _resume(DWORD rL)
{
	lua_State* L = reinterpret_cast<lua_State*>((int)rlua_tonumber(rL, lua_upvalueindex(1)));

	const int nargs = rlua_gettop(rL);
	//printf("%d | %d\n", nargs, lua_gettop(L));
	for (int i = 1; i < nargs + 1; i++)
		wrap(L, rL, i);

	rlua_pop(rL, nargs);
	return lua_resume(L, nargs);
}

int yield3Hit;

int _rbxYieldHandler(lua_State* L, DWORD rL)
{
	r_pushnumber(rL, (int)L);
	rlua_pushcclosure(rL, yield3Hit, 1);
	return lua_yield(L, 0);
}


static int RobloxFunction(lua_State* L) {
	int T = rlua_newthread(rL);
	int Key = lua_tonumber(L, lua_upvalueindex(1));
	int ArgC = lua_gettop(L);
	rlua_rawgeti(T, LUA_REGISTRYINDEX, Key);
	for (int i = ArgC; i != 0; i--) {
		unwrap(L, T, i * -1);
	}
	rlua_pcall(T, ArgC, LUA_MULTRET, 0);
	ArgC = rlua_gettop(T);
	for (int i = ArgC; i != 0; i--) {
		wrap(L, T, i * -1);
	}
	return ArgC;
}
static int HttpGet(lua_State* l);


int wrap(lua_State *L, int rstate, int pos) {
	int res = 0;
	switch (rlua_type(rstate, pos)) {
	case RLUA_TNIL:
		res += 1;
		lua_pushnil(L);
		//	printf("wrapping nil\n");
		break;
	case RLUA_TLIGHTUSERDATA:
		res += 1;
		lua_pushstring(L, "unsupported lua type");
		break;
	case RLUA_TTHREAD:
		//lua_newthread(L);
		lua_pushthread(L);
		res += 1;
		break;
	case RLUA_TNUMBER:
		res += 1;
		//	printf("wrapping number\n");
		lua_pushnumber(L, rlua_tonumber(rstate, pos));
		break;
	case RLUA_TBOOLEAN:
		res += 1;
		//printf("wrapping boolean\n");
		lua_pushboolean(L, rlua_toboolean(rstate, pos));
		break;
	case RLUA_TSTRING:
		res += 1;
		//printf("wrapping string\n");
		lua_pushstring(L, rlua_tolstring(rstate, pos, 0));
		break;
	case RLUA_TFUNCTION: {
		res += 1;
		//printf("wrapping function\n");
		rlua_pushvalue(rstate, pos);
		int ref = rluaL_ref(rstate, LUA_REGISTRYINDEX);

		//rlua_rawseti(rstate, LUA_REGISTRYINDEX, ref);

		lua_pushnumber(L, ref);

		lua_pushcclosure(L, RobloxFunction, 1);

		break;
	}
	case RLUA_TTABLE: {
		rlua_pushvalue(rstate, pos);
		lua_newtable(L);
		rlua_pushnil(rstate);
		while (rlua_next(rstate, -2) != 0) {
			wrap(L, rstate, -2);
			wrap(L, rstate, -1);
			lua_settable(L, -3);
			rlua_pop(rstate, 1);
		}
		rlua_pop(rstate, 1);
		res += 1;

		break;
	}
	case RLUA_TUSERDATA:
		//printf("wrapping userdata\n");
		printf("pushing end\n");
		rlua_pushvalue(rstate, pos);
		int Key = rluaL_ref(rstate, LUA_REGISTRYINDEX);
		int* U = (int*)lua_newuserdata(L, 8);
		*U = Key;
		lua_getfield(L, -10002, "robject");
		lua_setmetatable(L, -2);
		res += 1;
		break;

	}
	return res;
}

static int LuaFunction(int T) {
	int Key = rlua_tonumber(T, lua_upvalueindex(1));
	lua_State* L = (lua_State*)(int)rlua_tonumber(T, lua_upvalueindex(2));
	lua_rawgeti(L, -10000, Key);
	for (int i = rlua_gettop(T); i != 0; i--) {
		wrap(L, T, 1);
	}
	lua_pcall(L, lua_gettop(L) - 1, 0, 0);
	return 0;
}


static int r_getObjects(lua_State *L) { //since im lazy i don't detect if its actually calling from game
	std::string id = lua_tostring(L, 2);
	rlua_getfield(rL, -10002, "game");
	rlua_getfield(rL, -1, "InsertService");
	rlua_getfield(rL, -1, "LoadLocalAsset");
	rlua_pushvalue(rL, -2);
	rlua_pushstring(rL, id.c_str());
	rlua_pcall(rL, 2, 1, 0);
	wrap(L, rL, -1);
	lua_newtable(L);
	lua_pushvalue(L, -2);
	lua_rawseti(L, -2, 1);
	return 1;
}

#define DbgPrint printf

int Index(lua_State* L) {
	try {
		DbgPrint("[Index] Start\n");
		int T = rL;//GetThreadPointer(L);

		const char* Key = lua_tostring(L, -1);
		lua_pop(L, 1);
		DbgPrint("[Index] Key: %s\n", Key);

		if (Key == std::string("GetObjects"))
		{
			lua_pushcfunction(L, r_getObjects);
		}
		else if (Key == std::string("HttpGet"))
		{
			lua_pushcfunction(L, HttpGet);
		}
		else
		{

			unwrap(L, T, 1);
			rlua_getfield(T, -1, Key);
			wrap(L, T, -1);
			rlua_pop(T, 1);
			DbgPrint("[Index] End\n");
		}
	}
	catch (std::exception e) {
		//Error(0, e.what(), "Error", 0);
		luaL_error(L, e.what());
	}
	return 1;
}

void r_lua_setfield(int L, int idx, const char *k) {
	rlua_pushvalue(L, idx);
	if (rlua_getmetatable(L, -1)) {
		rlua_getfield(L, -1, "__newindex");
	    rlua_pushvalue(L, -3);
		rlua_pushstring(L, k);
		rlua_pushvalue(L, -6);
		rlua_pcall(L, 3, 0, 0);
		rlua_pop(L, 3);
	}
	else {
		rlua_pop(L, 1);
		rlua_setfield(L, idx, k);
	}
}

static int NewIndex(lua_State *L) {
	try {
		const char* key = lua_tolstring(L, -2, NULL);
		printf("key: %s\n", key);
		int rstate = rL;
		unwrap(L, rstate, -3);
		unwrap(L, rstate, -1);
		r_lua_setfield(rstate, -2, key);
	}
	catch (std::exception e) {
		printf("Error: %s\n", e.what());
	}
	return 0;
}

void PushGlobal(int rstate, lua_State *L, std::string g) {
	rlua_getfield(rstate, -10002, g.c_str());
	wrap(L, rstate, -1);
	lua_setfield(L, -10002, g.c_str());
	rlua_pop(rstate, 1);
}

void PushMeta(int rstate, lua_State *L, lua_CFunction cf, std::string g) {
	lua_pushnumber(L, rstate);
	lua_pushcclosure(L, cf, 1);
	lua_setfield(L, -2, g.c_str());
}

static int __mathmf(int state, lua_State * L, std::string m, int idxa, int idxb) {
	rlua_rawgeti(state, LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -2));
	rluaL_getmetafield(state, -1, m.c_str());
	unwrap(L, state, -2);
	unwrap(L, state, -1);
	rlua_pcall(state, 2, 1, 0);
	wrap(L, state, -1);
	return 0;
}


static int u_metaf(lua_State *L) {
	int rstate = lua_tonumber(L, -10003);
	std::string method = lua_tostring(L, -10004);
	__mathmf(rstate, L, method, -2, -1);
	return 1;
}


void pushMetaMeth(int rstate, lua_State *L, std::string method) {
	if (method == "__type") {
		lua_pushstring(L, "Object");
		lua_setfield(L, -2, method.c_str());
	}
	else if (method == "__metatable") {
		lua_pushstring(L, "This metatable is locked");
		lua_setfield(L, -2, method.c_str());
	}
	else if (method == "__tostring") {
		
	}
	else {
		lua_pushnumber(L, rstate);
		lua_pushstring(L, method.c_str());
		lua_pushcclosure(L, u_metaf, 2);
		lua_setfield(L, -2, method.c_str());
	}
}

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

inline std::string download_url(const std::string &url) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	std::string rtn;

	if (interwebs) {
		HINTERNET url_file = InternetOpenUrlA(interwebs, url.c_str(), NULL, NULL, NULL, NULL);

		if (url_file) {
			char buffer[2000];
			DWORD bytes_read;

			do {
				InternetReadFile(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);

			InternetCloseHandle(interwebs);
			InternetCloseHandle(url_file);
			std::string p = replace_all(rtn, "|n", "\r\n");
			return p;
		}
	}

	InternetCloseHandle(interwebs);
	std::string p = replace_all(rtn, "|n", "\r\n");
	return p;
}

static int HttpGet(lua_State* l)
{
	const std::string url = download_url(lua_tostring(l, -1));
	InitLua(rL, l, url.c_str());
	return 1;
}

static int loadstring_implement(lua_State* l)
{
	const std::string source = lua_tostring(l, -1);
	InitLua(rL, l, source.c_str());
	return 1;
}

void InitLua(int rstate, lua_State *L, const char* src) {

	lua_register(L, "HttpGet", HttpGet);
	lua_register(L, "loadstring", loadstring_implement);

	lua_newtable(L);
	lua_pushcfunction(L, Index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, NewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushliteral(L, "This metatable is locked");
	lua_setfield(L, -2, "__metatable");
	
	pushMetaMeth(rstate, L, "__add");
	pushMetaMeth(rstate, L, "__sub");
	pushMetaMeth(rstate, L, "__mul");
	pushMetaMeth(rstate, L, "__div");
	pushMetaMeth(rstate, L, "__pow");
	pushMetaMeth(rstate, L, "__unm");
	pushMetaMeth(rstate, L, "__mod");
	pushMetaMeth(rstate, L, "__concat");
	//lua_pushcfunction(L, __eq);
	//lua_setfield(L, -2, "__eq");
	pushMetaMeth(rstate, L, "__eq");
	pushMetaMeth(rstate, L, "__lt");
	pushMetaMeth(rstate, L, "__le");
	pushMetaMeth(rstate, L, "__type");
	pushMetaMeth(rstate, L, "__call");
	pushMetaMeth(rstate, L, "__metatable");

	lua_setglobal(L, "robject");

	
	
	
	









	PushGlobal(rstate, L, "game");
	PushGlobal(rstate, L, "Game");
	PushGlobal(rstate, L, "workspace");
	PushGlobal(rstate, L, "Workspace");
	PushGlobal(rstate, L, "Axes");
	PushGlobal(rstate, L, "BrickColor");
	PushGlobal(rstate, L, "CFrame");
	PushGlobal(rstate, L, "Color3");
	PushGlobal(rstate, L, "ColorSequence");
	PushGlobal(rstate, L, "ColorSequenceKeypoint");
	PushGlobal(rstate, L, "Faces");
	PushGlobal(rstate, L, "Instance");
	PushGlobal(rstate, L, "NumberRange");
	PushGlobal(rstate, L, "NumberSequence");
	PushGlobal(rstate, L, "NumberSequenceKeypoint");
	PushGlobal(rstate, L, "PhysicalProperties");
	PushGlobal(rstate, L, "Random");
	PushGlobal(rstate, L, "Ray");
	PushGlobal(rstate, L, "Rect");
	PushGlobal(rstate, L, "Region3");
	PushGlobal(rstate, L, "Region3int16");
	PushGlobal(rstate, L, "TweenInfo");
	PushGlobal(rstate, L, "UDim");
	PushGlobal(rstate, L, "UDim2");
	PushGlobal(rstate, L, "Vector2");
	PushGlobal(rstate, L, "Vector2int16");
	PushGlobal(rstate, L, "Vector3");
	PushGlobal(rstate, L, "Vector3int16");
	PushGlobal(rstate, L, "Enum");
	PushGlobal(rstate, L, "math");
	PushGlobal(rstate, L, "ypcall");
	PushGlobal(rstate, L, "xpcall");
	PushGlobal(rstate, L, "rawequal");
	PushGlobal(rstate, L, "spawn");
	PushGlobal(rstate, L, "Spawn");
	PushGlobal(rstate, L, "LoadLibrary");


	PushGlobal(rstate, L, "tick");
	PushGlobal(rstate, L, "time");
	PushGlobal(rstate, L, "Delay");
	PushGlobal(rstate, L, "delay");
	PushGlobal(rstate, L, "Wait");
	PushGlobal(rstate, L, "wait");
	PushGlobal(rstate, L, "printidentity");
	PushGlobal(rstate, L, "typeof");
	PushGlobal(rstate, L, "warn");
	PushGlobal(rstate, L, "print");

	int err = luaL_dostring(L, src);
	if (err)
	{
		printf("Lua Error: %s\n", lua_tostring(L, -1));
	}
	
	lua_pcall(L, 0, 0, 0);

}
		printf("unsupported lua type\n");
		rlua_pushstring(rstate, "unsupported lua type");
		res += 1;
		break;
	case LUA_TTHREAD:
		res += 1;
		rlua_newthread(rstate);
		//printf("Thread type\n");
		break;
	case LUA_TNUMBER: {
		res += 1;
		r_pushnumber(rstate, lua_tonumber(L, pos));
		break;
	}
	case LUA_TBOOLEAN:
		res += 1;
		rlua_pushboolean(rstate, lua_toboolean(L, pos));
		break;
	case LUA_TSTRING:
		printf("wrapping string\n");
		res += 1;
		rlua_pushstring(rstate, lua_tolstring(L, pos, 0));
		break;
	case LUA_TFUNCTION: {
		res += 1;
		lua_pushvalue(L, pos);
		printf("pushed value\n");
		int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		r_pushnumber(rstate, ref);
		printf("pushed ref\n");
		r_pushnumber(rstate, (int)L);
		printf("pushed state\n");
		rlua_pushcclosure(rstate, int3breakpoint, 2);
		printf("passed pushcclosure\n");
		break;
	}
	case LUA_TTABLE: {
		lua_pushvalue(L, pos);
		rlua_createtable(rstate, 0, 0);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			unwrap(L, rstate, -2);
			unwrap(L, rstate, -1);
			rlua_settable(rstate, -3);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		res += 1;
		break;
	}
	case LUA_TUSERDATA:
		int k = (int)lua_touserdata(L, pos);
		rlua_rawgeti(rstate, LUA_REGISTRYINDEX, *(int*)k);
		res += 1;
		break;

	}
	return res;
}

static int _resume(DWORD rL)
{
	lua_State* L = reinterpret_cast<lua_State*>((int)rlua_tonumber(rL, lua_upvalueindex(1)));

	const int nargs = rlua_gettop(rL);
	//printf("%d | %d\n", nargs, lua_gettop(L));
	for (int i = 1; i < nargs + 1; i++)
		wrap(L, rL, i);

	rlua_pop(rL, nargs);
	return lua_resume(L, nargs);
}

int yield3Hit;

int _rbxYieldHandler(lua_State* L, DWORD rL)
{
	r_pushnumber(rL, (int)L);
	rlua_pushcclosure(rL, yield3Hit, 1);
	return lua_yield(L, 0);
}



static int CallFunction(lua_State *L) {
	try {
		int oldrstate = (int)lua_tonumber(L, -10004);
		int rstate = rlua_newthread(oldrstate); //when ur 2 lazy 2 write upvalueindex disgusting

		
		int elems = rlua_gettop(rstate);
		rlua_rawgeti(rstate, LUA_REGISTRYINDEX, lua_tonumber(L, -10003));
		int top = lua_gettop(L);
		int args = 0;
		for (int i = 1; i <= top; i++, args++)
			unwrap(L, rstate, (i));

		lua_pop(L, top);
		rlua_pcall(rstate, args, LUA_MULTRET, 0);
		if (rlua_type(rstate, -1) == RLUA_TSTRING) {
			std::string s = rlua_tolstring(rstate, -1, NULL);
			if (s == "attempt to yield across metamethod/C-call boundary") {
				rlua_pop(rstate, 1);

				//return _rbxYieldHandler(L, rstate);
			}
		}

		int _min = rlua_gettop(rstate) - elems;
		for (int i = _min; i != 0; i--)
			wrap(L, rstate, -(i));


		rlua_pop(oldrstate, _min);
		//rbx::state = oldrstate;
		return _min;
	}
	catch (std::exception e) {
		std::cout << "Callhandler Exception: " << e.what() << std::endl;
		return 0;
	}
}


int wrap(lua_State *L, int rstate, int pos) {
	int res = 0;
	switch (rlua_type(rstate, pos)) {
	case RLUA_TNIL:
		res += 1;
		lua_pushnil(L);
		//	printf("wrapping nil\n");
		break;
	case RLUA_TLIGHTUSERDATA:
		res += 1;
		lua_pushstring(L, "unsupported lua type");
		break;
	case RLUA_TTHREAD:
		//lua_newthread(L);
		lua_pushthread(L);
		res += 1;
		break;
	case RLUA_TNUMBER:
		res += 1;
		//	printf("wrapping number\n");
		lua_pushnumber(L, rlua_tonumber(rstate, pos));
		break;
	case RLUA_TBOOLEAN:
		res += 1;
		//printf("wrapping boolean\n");
		lua_pushboolean(L, rlua_toboolean(rstate, pos));
		break;
	case RLUA_TSTRING:
		res += 1;
		//printf("wrapping string\n");
		lua_pushstring(L, rlua_tolstring(rstate, pos, 0));
		break;
	case RLUA_TFUNCTION: {
		res += 1;
		//printf("wrapping function\n");
		rlua_pushvalue(rstate, pos);
		int ref = rluaL_ref(rstate, LUA_REGISTRYINDEX);

		lua_pushnumber(L, ref);
		lua_pushnumber(L, rstate);
		lua_pushcclosure(L, CallFunction, 2);

		break;
	}
	case RLUA_TTABLE: {
		rlua_pushvalue(rstate, pos);
		lua_newtable(L);
		rlua_pushnil(rstate);
		while (rlua_next(rstate, -2) != 0) {
			wrap(L, rstate, -2);
			wrap(L, rstate, -1);
			lua_settable(L, -3);
			rlua_pop(rstate, 1);
		}
		rlua_pop(rstate, 1);
		res += 1;

		break;
	}
	case RLUA_TUSERDATA:
		//printf("wrapping userdata\n");
		r_PushObject_id(rstate, L, pos);
		res += 1;
		break;

	}
	return res;
}

static int LuaFunction(int T) {
	int Key = rlua_tonumber(T, lua_upvalueindex(1));
	lua_State* L = (lua_State*)(int)rlua_tonumber(T, lua_upvalueindex(2));
	lua_rawgeti(L, -10000, Key);
	for (int i = rlua_gettop(T); i != 0; i--) {
		wrap(L, T, 1);
	}
	lua_pcall(L, lua_gettop(L) - 1, 0, 0);
	return 0;
}


static int r_getObjects(lua_State *L) { //since im lazy i don't detect if its actually calling from game
	std::string id = lua_tostring(L, 2);
	rlua_getfield(rL, -10002, "game");
	rlua_getfield(rL, -1, "InsertService");
	rlua_getfield(rL, -1, "LoadLocalAsset");
	rlua_pushvalue(rL, -2);
	rlua_pushstring(rL, id.c_str());
	rlua_pcall(rL, 2, 1, 0);
	wrap(L, rL, -1);
	lua_newtable(L);
	lua_pushvalue(L, -2);
	lua_rawseti(L, -2, 1);
	return 1;
}

#define DbgPrint printf

int Index(lua_State* L) {
	try {
		DbgPrint("[Index] Start\n");
		int T = rL;//GetThreadPointer(L);

		const char* Key = lua_tostring(L, -1);
		lua_pop(L, 1);
		DbgPrint("[Index] Key: %s\n", Key);

		unwrap(L, T, 1);
		rlua_getfield(T, -1, Key);
		wrap(L, T, -1);
		rlua_pop(T, 1);
		DbgPrint("[Index] End\n");
	}
	catch (std::exception e) {
		//Error(0, e.what(), "Error", 0);
		luaL_error(L, e.what());
	}
	return 1;
}

void r_lua_setfield(int L, int idx, const char *k) {
	rlua_pushvalue(L, idx);
	if (rlua_getmetatable(L, -1)) {
		rlua_getfield(L, -1, "__newindex");
	    rlua_pushvalue(L, -3);
		rlua_pushstring(L, k);
		rlua_pushvalue(L, -6);
		rlua_pcall(L, 3, 0, 0);
		rlua_pop(L, 3);
	}
	else {
		rlua_pop(L, 1);
		rlua_setfield(L, idx, k);
	}
}

static int NewIndex(lua_State *L) {
	try {
		const char* key = lua_tolstring(L, -2, NULL);
		printf("key: %s\n", key);
		int rstate = rL;
		unwrap(L, rstate, -3);
		unwrap(L, rstate, -1);
		r_lua_setfield(rstate, -2, key);
	}
	catch (std::exception e) {
		printf("Error: %s\n", e.what());
	}
	return 0;
}

void PushGlobal(int rstate, lua_State *L, std::string g) {
	rlua_getfield(rstate, -10002, g.c_str());
	wrap(L, rstate, -1);
	lua_setfield(L, -10002, g.c_str());
	rlua_pop(rstate, 1);
}

void PushMeta(int rstate, lua_State *L, lua_CFunction cf, std::string g) {
	lua_pushnumber(L, rstate);
	lua_pushcclosure(L, cf, 1);
	lua_setfield(L, -2, g.c_str());
}

static int __mathmf(int state, lua_State * L, std::string m, int idxa, int idxb) {
	rlua_rawgeti(state, LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -2));
	rluaL_getmetafield(state, -1, m.c_str());
	unwrap(L, state, -2);
	unwrap(L, state, -1);
	rlua_pcall(state, 2, 1, 0);
	wrap(L, state, -1);
	return 0;
}


static int u_metaf(lua_State *L) {
	int rstate = lua_tonumber(L, -10003);
	std::string method = lua_tostring(L, -10004);
	__mathmf(rstate, L, method, -2, -1);
	return 1;
}


void pushMetaMeth(int rstate, lua_State *L, std::string method) {
	if (method == "__type") {
		lua_pushstring(L, "Object");
		lua_setfield(L, -2, method.c_str());
	}
	else if (method == "__metatable") {
		lua_pushstring(L, "This metatable is locked");
		lua_setfield(L, -2, method.c_str());
	}
	else if (method == "__tostring") {
		
	}
	else {
		lua_pushnumber(L, rstate);
		lua_pushstring(L, method.c_str());
		lua_pushcclosure(L, u_metaf, 2);
		lua_setfield(L, -2, method.c_str());
	}
}

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

inline std::string download_url(const std::string &url) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	std::string rtn;

	if (interwebs) {
		HINTERNET url_file = InternetOpenUrlA(interwebs, url.c_str(), NULL, NULL, NULL, NULL);

		if (url_file) {
			char buffer[2000];
			DWORD bytes_read;

			do {
				InternetReadFile(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);

			InternetCloseHandle(interwebs);
			InternetCloseHandle(url_file);
			std::string p = replace_all(rtn, "|n", "\r\n");
			return p;
		}
	}

	InternetCloseHandle(interwebs);
	std::string p = replace_all(rtn, "|n", "\r\n");
	return p;
}

static int HttpGet(lua_State* l)
{
	const std::string url = download_url(lua_tostring(l, -1));
	InitLua(rL, l, url.c_str());
	return 1;
}

void InitLua(int rstate, lua_State *L, const char* src) {
	
	
	
	lua_newtable(L);
	
	lua_pushcfunction(L, Index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, NewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushstring(L, "The metatable is locked");
	lua_setfield(L, -2, "__metatable");
	
	

	pushMetaMeth(rstate, L, "__add");
	pushMetaMeth(rstate, L, "__sub");
	pushMetaMeth(rstate, L, "__mul");
	pushMetaMeth(rstate, L, "__div");
	pushMetaMeth(rstate, L, "__pow");
	pushMetaMeth(rstate, L, "__unm");
	pushMetaMeth(rstate, L, "__mod");
	pushMetaMeth(rstate, L, "__concat");
	//lua_pushcfunction(L, __eq);
	//lua_setfield(L, -2, "__eq");
	pushMetaMeth(rstate, L, "__eq");
	pushMetaMeth(rstate, L, "__lt");
	pushMetaMeth(rstate, L, "__le");
	pushMetaMeth(rstate, L, "__type");
	pushMetaMeth(rstate, L, "__call");
	pushMetaMeth(rstate, L, "__metatable");

	lua_setglobal(L, "robject");




	PushGlobal(rstate, L, "game");
	PushGlobal(rstate, L, "Game");
	PushGlobal(rstate, L, "workspace");
	PushGlobal(rstate, L, "Workspace");
	PushGlobal(rstate, L, "Axes");
	PushGlobal(rstate, L, "BrickColor");
	PushGlobal(rstate, L, "CFrame");
	PushGlobal(rstate, L, "Color3");
	PushGlobal(rstate, L, "ColorSequence");
	PushGlobal(rstate, L, "ColorSequenceKeypoint");
	PushGlobal(rstate, L, "Faces");
	PushGlobal(rstate, L, "Instance");
	PushGlobal(rstate, L, "NumberRange");
	PushGlobal(rstate, L, "NumberSequence");
	PushGlobal(rstate, L, "NumberSequenceKeypoint");
	PushGlobal(rstate, L, "PhysicalProperties");
	PushGlobal(rstate, L, "Random");
	PushGlobal(rstate, L, "Ray");
	PushGlobal(rstate, L, "Rect");
	PushGlobal(rstate, L, "Region3");
	PushGlobal(rstate, L, "Region3int16");
	PushGlobal(rstate, L, "TweenInfo");
	PushGlobal(rstate, L, "UDim");
	PushGlobal(rstate, L, "UDim2");
	PushGlobal(rstate, L, "Vector2");
	PushGlobal(rstate, L, "Vector2int16");
	PushGlobal(rstate, L, "Vector3");
	PushGlobal(rstate, L, "Vector3int16");
	PushGlobal(rstate, L, "Enum");
	PushGlobal(rstate, L, "math");
	PushGlobal(rstate, L, "ypcall");
	PushGlobal(rstate, L, "xpcall");
	PushGlobal(rstate, L, "rawequal");
	PushGlobal(rstate, L, "spawn");
	PushGlobal(rstate, L, "Spawn");
	PushGlobal(rstate, L, "LoadLibrary");


	PushGlobal(rstate, L, "tick");
	PushGlobal(rstate, L, "time");
	PushGlobal(rstate, L, "Delay");
	PushGlobal(rstate, L, "delay");
	PushGlobal(rstate, L, "Wait");
	PushGlobal(rstate, L, "wait");
	PushGlobal(rstate, L, "printidentity");
	PushGlobal(rstate, L, "typeof");
	PushGlobal(rstate, L, "warn");
	PushGlobal(rstate, L, "print");

	int err = luaL_dostring(L, src);
	if (err)
	{
		printf("Lua Error: %s\n", lua_tostring(L, -1));
	}
	
	lua_pcall(L, 0, 0, 0);

}
