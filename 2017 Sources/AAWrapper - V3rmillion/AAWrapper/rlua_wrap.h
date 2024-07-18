#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <WinInet.h>
#include <fstream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <string>
#include <Psapi.h>
#include <tlhelp32.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "luaconf.h"
}

using namespace std;

BOOL ExecuteScript(string Script);
static int CallMethod(lua_State* L);
static int r_wrap(unsigned int rL, lua_State* L, int lastidx);
static int l_unwrap(lua_State* L, unsigned int rL, int idx);
