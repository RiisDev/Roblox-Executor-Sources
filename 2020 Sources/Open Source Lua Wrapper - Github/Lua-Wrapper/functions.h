#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include <thread>
#include <fstream>
#include <filesystem>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <chrono>
#include <string>
#include <regex>
#include <cmath>
#include "globals.h"
#include "bridge.h"
#include "Lua/lapi.h"
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")
#include "r_lua.h"
#include <tchar.h>
#include <dwmapi.h>
#include "input.h"

using namespace std;

using Bridge::m_rL;
using Bridge::m_L;


static int UserData(lua_State* Thread) {
	void* UD = lua_touserdata(Thread, 1);
	if (m_rL) {

		r_lua_rawgeti(m_rL, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(m_rL, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);

		}
	}
	return 0;
}

int getgenv(lua_State* L)
{
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	Bridge::push(m_rL, L, -1);
	return 1;
}

int getrawmetatable(lua_State* L) {
	Bridge::push(L, m_rL, 1);

	if (r_lua_getmetatable(m_rL, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Bridge::push(m_rL, L, -1);

	return 1;
}

int getreg(lua_State* LS) {
	lua_pushvalue(LS, LUA_REGISTRYINDEX);
	Bridge::push(m_rL, LS, -1);
	return 1;
}

int getrenv(lua_State* LS) {
	lua_pushvalue(LS, int(m_rL));
	Bridge::push(m_rL, LS, -1);
	return 1;
}

int getsenv(lua_State* LS) {
	lua_pushvalue(LS, LUA_ENVIRONINDEX);
	Bridge::push(m_rL, LS, -1);
	return 1;
}

int copyclipboard(lua_State* L) {
	string name = lua_tostring(L, -1);
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, name.size());
	if (!hg) {
		CloseClipboard();
	}
	memcpy(GlobalLock(hg), name.c_str(), name.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
	return 1;
}

static int getgc(lua_State* L) {
	static const char* const opts[] = { "stop", "restart", "collect",
	  "count", "step", "setpause", "setstepmul", NULL };
	static const int optsnum[] = { LUA_GCSTOP, LUA_GCRESTART, LUA_GCCOLLECT,
	  LUA_GCCOUNT, LUA_GCSTEP, LUA_GCSETPAUSE, LUA_GCSETSTEPMUL };
	int o = luaL_checkoption(L, 1, "collect", opts);
	int ex = luaL_optint(L, 2, 0);
	int res = lua_gc(L, optsnum[o], ex);
	switch (optsnum[o]) {
	case LUA_GCCOUNT: {
		int b = lua_gc(L, LUA_GCCOUNTB, 0);
		lua_pushnumber(L, res + ((lua_Number)b / 1024));
		return 1;
	}
	case LUA_GCSTEP: {
		lua_pushboolean(L, res);
		return 1;
	}
	default: {
		lua_pushnumber(L, res);
		return 1;
	}
	}
}

void loadlibrary()
{
	lua_register(m_L, "getrawmetatable", getrawmetatable);
	lua_register(m_L, "getgenv", getgenv);
	lua_register(m_L, "getrenv", getrenv);
	lua_register(m_L, "getsenv", getsenv);
	lua_register(m_L, "getmenv", getsenv);
	lua_register(m_L, "getgc", getgc);
	Keyboard::keyboardlib(m_L);
	Mouse::mouselib(m_L);
	printf("custom library loaded");
}