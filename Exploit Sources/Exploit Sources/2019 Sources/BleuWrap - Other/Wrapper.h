#include <windows.h>
#include <tchar.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <stdlib.h> 
#include <fstream> 
#include <iterator>
#include <iostream>
#include <cctype>
#include <WinInet.h>

#define Bleu 1
#define Roblox 2

#define RW(idx)(BleuBridge(R, L, idx, Roblox)) /* Roblox Wrap */
#define BW(idx)(BleuBridge(R, L, idx, Bleu)) /* Bleu Wrap */

lua_State* WL;
rbx_State WR;
int GlobalIndex = -100;

typedef enum{
	WRAPCALL,
	WRAPYIELD,
	Bleu,
	Roblox
}

void BleuBridge(rbx_State R, lua_State* L, int Index, int Mode);

static int YieldHandler(rbx_State R) {
	lua_State* L = WL;
	lua_State* nL;

	lua_rawgeti(L, LUA_REGISTRYINDEX,
		rbx_tonumber(R, lua_upvalueindex(2)));
	nL = lua_tothread(L, -1);
	lua_pop(L, 1);

	lua_pushnil(L);
	lua_rawseti(L, LUA_REGISTRYINDEX,
		rbx_tonumber(R, lua_upvalueindex(2)));

	int ReturnValues = rbx_gettop(R);
	for (int i = 1; i <= ReturnValues; i++) {
		BleuBridge(R, nL, i, Bleu);
	}

	int status = lua_resume(nL, ReturnValues);
	if (!status) {
		int NewReturnValues = lua_gettop(nL);
		for (int i = 1; i <= NewReturnValues; i++) {
			BleuBridge(R, nL, i, Roblox);
		}
		lua_pop(nL, NewReturnValues);
		return NewReturnValues;
	}
	else if (status == LUA_ERRRUN) {
		if (lua_type(nL, -1) == LUA_TSTRING) {
			rbxL_error(R, lua_tostring(nL, -1));
		}
		else {
			rbxL_error(R, "Error occurred, no output from Lua.");
		}
	}
	else if (status == LUA_YIELD) {
		return -1;
	}
}

static int BleuCallHandler(lua_State* L) {
	rbx_State R = rbx_newthread(WR);
	int Args = lua_gettop(L);

	rbx_pushnumber(R, lua_tonumber(L, lua_upvalueindex(1)));
	rbx_gettable(R, LUA_REGISTRYINDEX);

	for (int i = 1; i <= Args; i++) {
		RW(i);
	}

	if (!rbx_pcall(R, Args, LUA_MULTRET, 0)) {
		int Rets = rbx_gettop(R);

		for (int i = 1; i <= Rets; i++) {
			BW(i);
		}

		rbx_settop(R, 0);

		return Rets;
	}
	else {
		if (rbx_type(R, -1) == RBX_TSTRING) {
			if (std::string("attempt to yield across metamethod/C-call boundary") == rbx_tostring(R, -1)) {
				rbx_pop(R, 1);
				GlobalIndex--;

				lua_pushthread(L);
				lua_rawseti(L, LUA_REGISTRYINDEX, GlobalIndex);

				rbx_pushnumber(R, WRAPYIELD);
				rbx_pushnumber(R, GlobalIndex);

				rbx_pushcclosure(R, CallCheck, 2);
				return lua_yield(L, 0);
			}
			else {
				luaL_error(L, rbx_tostring(R, -1));
			}
		}
		else {
			luaL_error(L, "Error occurred, no output from Lua.");
		}
	}

	return 0;
}

static int RobloxCallHandler(rbx_State R) {
	lua_State* L = lua_newthread(WL);
	lua_pushnumber(WL, 1);
	lua_settable(WL, LUA_REGISTRYINDEX);

	int Args = rbx_gettop(R);

	lua_pushnumber(L, rbx_tonumber(R, lua_upvalueindex(2)));
	lua_gettable(L, LUA_REGISTRYINDEX);

	for (int i = 1; i <= Args; i++) {
		BW(i);
	}

	int status = lua_pcall(L, Args, LUA_MULTRET, 0);
	if (!status) {
		int Rets = lua_gettop(L);

		for (int i = 1; i <= Rets; i++) {
			RW(i);
		}

		lua_settop(L, 0);

		return Rets;
	}
	else if (status == LUA_ERRRUN) {
		if (lua_type(L, -1) == LUA_TSTRING) {
			rbxL_error(R, lua_tostring(L, -1));
		}
		else {
			rbxL_error(R, "Error occurred, no output from Lua.");
		}
	}
	else if (status == LUA_YIELD) {
		return -1;
	}

	return 0;
}

bool CheckCache(rbx_State R, lua_State* L, int Index) {
	rbx_pushvalue(R, Index);
	rbx_gettable(R, LUA_REGISTRYINDEX);
	if (rbx_type(R, -1) != RBX_TNIL) {
		BW(-1);
		lua_gettable(L, LUA_REGISTRYINDEX);
		rbx_pop(R, 1);
		return true;
	}
	else {
		rbx_pop(R, 1);
		return false;
	}
}

void SetCacheBleu(rbx_State R, lua_State* L, int GlobalIndex) {
	lua_pushnumber(L, GlobalIndex);
	lua_pushvalue(L, -2);
	lua_settable(L, LUA_REGISTRYINDEX);
}

void SetCacheRoblox(rbx_State R, lua_State* L, int GlobalIndex) {
	rbx_pushvalue(R, -1);
	rbx_pushnumber(R, GlobalIndex);
	rbx_settable(R, LUA_REGISTRYINDEX);
}

void BleuBridge(rbx_State R, lua_State* L, int Index, int Mode) {
	if (Mode == Bleu) {
		int Type = rbx_type(R, Index);
		if (Type == RBX_TNUMBER) {
			lua_pushnumber(L, rbx_tonumber(R, Index));
		}
		else if (Type == RBX_TBOOLEAN) {
			lua_pushboolean(L, rbx_toboolean(R, Index));
		}
		else if (Type == RBX_TSTRING) {
			size_t sz;
			const char* str = rbx_tolstring(R, Index, &sz);
			lua_pushlstring(L, str, sz);
		}
		else if (Type == RBX_TTABLE) {
			lua_newtable(L);
			rbx_pushvalue(R, Index);
			rbx_pushnil(R);
			while (rbx_next(R, -2)) {
				BW(-2);
				BW(-1);
				rbx_pop(R, 1);
				lua_settable(L, -3);
			}
			rbx_pop(R, 1);
		}
		else if (Type == RBX_TFUNCTION) {
			GlobalIndex--;
			rbx_pushvalue(R, Index);

			rbx_pushnumber(R, GlobalIndex);
			rbx_pushvalue(R, -2);
			rbx_settable(R, LUA_REGISTRYINDEX);

			rbx_pop(R, 1);

			lua_pushnumber(L, GlobalIndex);
			lua_pushcclosure(L, &BleuCallHandler, 1);
		}
		else if (Type == RBX_TUSERDATA) {
			if (!CheckCache(R, L, Index)) {
				GlobalIndex--;
				rbx_pushvalue(R, Index);
				SetCacheRoblox(R, L, GlobalIndex);

				rbx_pushnumber(R, GlobalIndex);
				rbx_pushvalue(R, -2);
				rbx_settable(R, LUA_REGISTRYINDEX);

				lua_newuserdata(L, 0);
				SetCacheBleu(R, L, GlobalIndex);

				lua_pushvalue(L, -1);
				lua_pushnumber(L, GlobalIndex);

				lua_settable(L, LUA_REGISTRYINDEX);

				if (rbx_getmetatable(R, -1)) {
					BW(-1);
					rbx_pop(R, 1);
					lua_setmetatable(L, -2);
				}

				rbx_pop(R, 1);
			}
		}
		else {
			lua_pushnil(L);
		}
	}
	else if (Mode == Roblox) {
		int Type = lua_type(L, Index);
		if (Type == LUA_TNUMBER) {
			rbx_pushnumber(R, lua_tonumber(L, Index));
		}
		else if (Type == LUA_TBOOLEAN) {
			rbx_pushboolean(R, lua_toboolean(L, Index));
		}
		else if (Type == LUA_TSTRING) {
			size_t sz;
			const char* str = lua_tolstring(L, Index, &sz);
			rbx_pushlstring(R, str, sz);
		}
		else if (Type == LUA_TTABLE) {
			rbx_newtable(R);
			lua_pushvalue(L, Index);
			lua_pushnil(L);
			while (lua_next(L, -2)) {
				RW(-2);
				RW(-1);
				lua_pop(L, 1);
				rbx_settable(R, -3);
			}
			lua_pop(L, 1);
		}
		else if (Type == LUA_TFUNCTION) {
			GlobalIndex--;
			lua_pushvalue(L, Index);

			lua_pushnumber(L, GlobalIndex);
			lua_pushvalue(L, -2);
			lua_settable(L, LUA_REGISTRYINDEX);

			lua_pop(L, 1);

			rbx_pushnumber(R, WRAPCALL);
			rbx_pushnumber(R, GlobalIndex);
			rbx_pushcclosure(R, CallCheck, 2);
		}
		else if (Type == LUA_TUSERDATA) {
			lua_pushvalue(L, Index);
			lua_gettable(L, LUA_REGISTRYINDEX);
			if (!lua_isnil(L, -1)) {
				RW(-1);
				rbx_gettable(R, LUA_REGISTRYINDEX);
			}
			else {
				rbx_getglobal(R, "newproxy");
				rbx_pcall(R, 0, 1, 0);
			}
			lua_pop(L, 1);
		}
		else {
			rbx_pushnil(R);
		}
	}
}