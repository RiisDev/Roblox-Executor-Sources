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

static const char kBytecodeMagic[] = "RSB1";
static const unsigned int kBytecodeHashSeed = 42;
static const unsigned int kBytecodeHashMultiplier = 41;

enum BytecodeConstantType
{
	Constant_Nil,
	Constant_Bool,
	Constant_Number,
	Constant_String,
	Constant_Method,
	Constant_Global,
};

class LuauSerializer {
public:
	static std::string writeClosure(lua_State* L);
	static int readClosure(lua_State* L, const char* chunkname, const char* source, size_t len);
};