#pragma once

#include "RevBase.hpp"

void r_lua_settop(uintptr_t rL, signed int idx);
void r_lua_pushlightuserdata(uintptr_t RL, void* ud);