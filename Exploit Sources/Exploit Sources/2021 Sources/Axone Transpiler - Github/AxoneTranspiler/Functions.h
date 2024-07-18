#pragma once
#include <Windows.h>
#include <WinInet.h>
#include <string>
#include <WinUser.h>
#include <string>
#include <Shlwapi.h>
#include <fstream>
#include <filesystem>
#include "CallCheck Bypass.h"
#include "Rlua.h"
int FireClickDetector(DWORD rL)
{
	r_lua_getglobal(rL, "game");
	r_lua_getfield(rL, -1, "GetService");
	r_lua_insert(rL, -2);
	r_lua_pushstring(rL, "Players");
	r_lua_pcall(rL, 2, 1, 0);
	r_lua_getfield(rL, -1, "LocalPlayer");
	uintptr_t LocalPtr = *reinterpret_cast<uintptr_t*>(r_lua_touserdata(rL, -1));



	float Distance = 0.0;
	if (r_lua_gettop(rL) == 2)
	{
		Distance = (float)r_lua_tonumber(rL, 2, 0);
		r_lua_remove(rL, 2);
	}


	uintptr_t clickDetectorPointer = *reinterpret_cast<uintptr_t*>(r_lua_touserdata(rL, 1));

	r_fireclickdetector(clickDetectorPointer, Distance, LocalPtr);

	return 0;
}
int GetGenv(int rL) {
	r_lua_pushvalue(rL, LUA_GLOBALSINDEX);
	//if (debug == true) { std::cout << "Global Index : " << LUA_GLOBALSINDEX << std::endl; }
	return 1;
}

int GetSenv(int rL) {
	r_lua_pushvalue(rL, LUA_ENVIRONINDEX);
	//if (debug == true) { std::cout << "Lua Environnement Index : " << LUA_ENVIRONINDEX << std::endl; }
	return 1;
}

int GetRenv(int rL) {
	r_lua_pushvalue(rL, LUA_GLOBALSINDEX);
	//if (debug == true) { std::cout << "Global Index" << LUA_GLOBALSINDEX << std::endl; }
	return 1;
}

int GetReg(int rL) {
	r_lua_pushvalue(rL, LUA_REGISTRYINDEX);
	//if (debug == true) { std::cout << "Registry Index" << LUA_REGISTRYINDEX << std::endl; }
	return 1;
}

int DebugGetReg(int rL) {
	r_lua_pushvalue(rL, -10000);
	return 1;
}

int hookfunction(DWORD rL) {
	if (r_lua_type(rL, -1) != R_LUA_TFUNCTION)
		std::cout << "Argument has to be a function" << std::endl;
	if (r_lua_type(rL, -2) != R_LUA_TFUNCTION)
		std::cout << "Argument has to be a function" << std::endl;
	
	r_lua_tocfunction(rL, -1);
	
	return 1;
}

void RegisterCustom()
{
	r_luaL_register(RL, "fireclickdetector", reinterpret_cast<int>(FireClickDetector));
	r_luaL_register(RL, "getreg", reinterpret_cast<int>(GetReg));
	r_luaL_register(RL, "getrenv", reinterpret_cast<int>(GetRenv));
	r_luaL_register(RL, "getgenv", reinterpret_cast<int>(GetGenv));
	r_luaL_register(RL, "getsenv", reinterpret_cast<int>(GetSenv));
	//r_luaL_register(RL, "hookfunction", reinterpret_cast<int>(hookfunction));
}