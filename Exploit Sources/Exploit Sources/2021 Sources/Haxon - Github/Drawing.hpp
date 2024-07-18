#pragma once
#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>

extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
}

#include "Structs.hpp"

class DrawingAPI {
public:
	static bool InitiateDrawingAPI(lua_State* LS);
};
