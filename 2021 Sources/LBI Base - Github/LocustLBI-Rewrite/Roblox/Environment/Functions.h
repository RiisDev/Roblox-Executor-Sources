#pragma once
#include "../RLua/RLua.h"

int getgenv(DWORD rL)
{
	r_pushvalue(rL, LUA_GLOBALSINDEX);
	return 1;
}

int getreg(DWORD rL)
{
	r_pushvalue(rL, LUA_REGISTRYINDEX);
	return 1;
}

int getrawmetatable(DWORD rL)
{
	r_getmetatable(rL, 1);
	return 1;
}

int setrawmetatable(DWORD rL)
{
	r_setmetatable(rL, 1);
	return 0;
}