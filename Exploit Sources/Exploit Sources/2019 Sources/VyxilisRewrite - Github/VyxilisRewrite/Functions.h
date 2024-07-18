#pragma once
#include <Windows.h>
#include <string>
#include <iostream>

#include "Offsets.h"

namespace LuaCFunctions {
	std::string GetLocalPlayerName() {
		r_lua_getglobal(RLS, "game");
		r_lua_getfield(RLS, -1, "Players");
		r_lua_getfield(RLS, -1, "LocalPlayer");
		r_lua_getfield(RLS, -1, "Name");
		return r_lua_tostring(RLS, -1);
	}
}

