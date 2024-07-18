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
#include "Lua\llimits.h"
#include "Lua\lgc.h"
#include "Lua\lfunc.h"
}

#include "structs.h"
#include "globals.h"


class DrawingAPI {
public:
	static bool InitiateDrawingAPI(lua_State* L);
};
