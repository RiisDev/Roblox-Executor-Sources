#pragma once

#include <windows.h>

#include "lua\\lapi.h"
#include "lua\\lua.h"
#include "lua\\lauxlib.h"
#include "lua\\lualib.h"

#define offset(add) (add - 0x400000 + (DWORD)GetModuleHandle(NULL))

DWORD ScriptContext = 0x0;
DWORD rL = 0x0;