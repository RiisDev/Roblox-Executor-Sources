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
#include <vector>

class TaskScheduler {
private:
	//	std::queue<thread>* queue;
public:
	TaskScheduler(lua_State *pL);
	void QueueScript(char *scrip);
	lua_State *L;
	std::vector<std::string> queue;
};