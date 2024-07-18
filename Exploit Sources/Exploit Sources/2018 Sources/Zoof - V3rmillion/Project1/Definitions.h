#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <string>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

DWORD ScriptContext = 0x0;
DWORD rL = 0x0;
lua_State *L = luaL_newstate();