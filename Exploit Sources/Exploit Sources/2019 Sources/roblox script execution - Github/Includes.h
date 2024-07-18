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

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lobject.h"
#include "Lua/lstate.h"
#include "Lua/ldo.h"
}