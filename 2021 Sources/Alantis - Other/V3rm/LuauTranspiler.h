#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\lobject.h"
#include "Lua\lstring.h"
#include "Lua\lfunc.h"
#include "Lua\ldo.h"
#include "Lua\lopcodes.h"
#include "Lua\lstring.h"
}

namespace LuauTranspiler {
	std::string compile(lua_State* L, std::string source);
	Proto* convert(lua_State* L, Proto* p);
};