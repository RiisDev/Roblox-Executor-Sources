#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <iostream>
#include <ctime>
#include <sstream>
#include <vector>
#include <iterator>
#include <io.h>
#include <fstream>
#include <wininet.h>
#include <stdlib.h>
#include <map>
#include <Psapi.h>
#include <stdarg.h>
#include <algorithm>


extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using namespace std;

#define offset(add) (add - 0x400000 + (DWORD)GetModuleHandle(NULL))

DWORD ScriptContext = 0x0;
DWORD rL = 0x0;
lua_State *L = lua_open();
bool debug = true;
string LocalPlayerName = "";

void Eggsecute(std::string input);