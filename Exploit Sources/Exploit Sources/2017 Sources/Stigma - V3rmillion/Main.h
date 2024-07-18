#pragma once
extern "C" {
#include "Lua/lua.h"
#include "Lua/lualib.h"
#include "Lua/lauxlib.h"
#include "Lua/ldo.h"
#include "Lua/lfunc.h"
#include "Lua/lmem.h"
#include "Lua/lopcodes.h"
#include "Lua/lstring.h"
#include "Lua/lundump.h"
#include "Lua/lvm.h"
}
#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "LuaSerializer.h"
#include "RbxDefs.h"
#include "SigScanner.h"
#include "Hook.h"
#include "TaskScheduler.h"
#include "Util.h"

#undef getline // was causing conflicts with ldebug.h's getline
// in Main.cpp Initialize


#include <iterator>
