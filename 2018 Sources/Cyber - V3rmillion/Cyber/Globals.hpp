#pragma once

#include <windows.h>

#include "LuaSourceFiles\\lapi.hpp"
#include "LuaSourceFiles\\lua.hpp"
#include "LuaSourceFiles\\lauxlib.hpp"
#include "LuaSourceFiles\\lualib.hpp"

#define offset(add) (add - 0x400000 + (DWORD)GetModuleHandle(NULL))

DWORD ScriptContext = 0x0;
DWORD rL = 0x0;