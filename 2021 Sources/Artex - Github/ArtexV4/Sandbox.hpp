#pragma once
#include "RBX.h"

namespace Enviroment
{
	namespace Register
	{
		void SetGlobal(std::uintptr_t r_L, int field, int function, const char* global)
		{
			field = LUA_GLOBALSINDEX ? field : LUA_GLOBALSINDEX;
			RBX::r_lua_createtable(r_L, 0, 0);
			RBX::r_lua_pushcclosure(r_L, function, 0, 1, 0);
			RBX::r_lua_setfield(r_L, field, global);
		}
	}

	namespace Functions
	{
		void sandboxGlobalThread(std::uintptr_t r_L)
		{
			RBX::r_lua_createtable(r_L, 0, 0);
			RBX::r_lua_createtable(r_L, 0, 1);

			RBX::r_lua_pushstring(r_L, "The metatable is locked!");
			RBX::r_lua_setfield(r_L, -2, "__metatable");
			RBX::r_lua_pushstring(r_L, "__index");
			RBX::r_lua_pushvalue(r_L, LUA_GLOBALSINDEX);
			RBX::r_lua_settable(r_L, -3);
			RBX::r_lua_setmetatable(r_L, -2);
			RBX::r_lua_replace(r_L, LUA_GLOBALSINDEX);

			RBX::r_lua_setfield(r_L, LUA_GLOBALSINDEX, "_G");
		}
	}
}