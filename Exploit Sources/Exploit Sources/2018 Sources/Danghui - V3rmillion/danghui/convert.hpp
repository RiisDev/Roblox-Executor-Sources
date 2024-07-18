#pragma once

#include "Lua/lua.hpp"
#include "Lua/lobject.h"
#include "types.hpp"

int danghui_convert(lua_State* L, unsigned int rL, TValue* src, r_TValue* dest);
void* r_luaS_newlstr(unsigned int rL, const char* str, size_t l);
lua_Number r_luaA_signnumber(unsigned int rL, lua_Number from);