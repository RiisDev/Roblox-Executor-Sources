#pragma once
#include "LuaWrapper.h"

using LuaWrapper::m_L;
using LuaWrapper::m_rL;

int getrawmetatable(lua_State* L) {
	LuaWrapper::Unwrap(L, m_rL, 1);

	if (r_lua_getmetatable(m_rL, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	LuaWrapper::Wrap(m_rL, L, -1);

	return 1;
}

int getgenv(lua_State* L)
{
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	LuaWrapper::Wrap(m_rL, L, -1);
	return 1;
}

int getrenv(lua_State* L)
{
	lua_pushvalue(L, int(m_rL));
	LuaWrapper::Wrap(m_rL, L, -1);
	return 1;
}

int getfenv(lua_State* L) {
	lua_pushvalue(m_L, LUA_GLOBALSINDEX);
	lua_xmove(m_L, L, 1);
	return 1;
}

int getsenv(lua_State* L) {
	lua_pushvalue(L, LUA_ENVIRONINDEX);
	LuaWrapper::Wrap(m_rL, L, -1);
	return 1;
}

int getreg(lua_State* L)
{
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	LuaWrapper::Wrap(m_rL, L, -1);
	return 1;
}

int GetHardwareID(lua_State* L) {
	HW_PROFILE_INFO hwProfileInfo;
	GetCurrentHwProfileA(&hwProfileInfo);
	string hwidWString = hwProfileInfo.szHwProfileGuid;
	string hwid(hwidWString.begin(), hwidWString.end());
	lua_pushstring(L, hwid.c_str());
	return 1;
}

bool CheckInstance(lua_State* m_L, int Index)
{
	lua_getglobal(m_L, "typeof");
	lua_pushvalue(m_L, Index);
	lua_pcall(m_L, 1, 1, 0);
	const auto Check = lua_tostring(m_L, -1);
	lua_pop(m_L, 1);
	return !strcmp(Check, "Instance");
}

int fireclickdetector(lua_State* m_L)
{
	/* rewritten my lua5.1 fireclickdetector to work with luaJIT */

	if (!lua_isuserdata(m_L, 1) || !CheckInstance(m_L, 1))
		return luaL_argerror(m_L, 1, "expected userdata <ClickDetector>");

	uintptr_t rawInstancePtr = DereferenceSmartPointerInstance((DWORD)lua_touserdata(m_L, -1));

	if (!lua_isnoneornil(m_L, 2) && !lua_isnumber(m_L, 2))
		return luaL_argerror(m_L, 1, "variant <none, number> expected");

	if (GetInstanceClassName(lua_touserdata(m_L, 1) != "ClickDetector"))
	{
		return luaL_argerror(m_L, 1, "userdata <ClickDetector> expected");
	}

	float distance = 0.0;
	if (lua_isnumber(m_L, 2))
		distance = (float)lua_tonumber(m_L, 2);

	lua_getglobal(m_L, "game");
	lua_getfield(m_L, -1, "GetService");
	lua_insert(m_L, -2);
	lua_pushstring(m_L, "Players");
	lua_pcall(m_L, 2, 1, 0);
	lua_getfield(m_L, -1, "LocalPlayer");

	uintptr_t rawPlayerInstancePtr = DereferenceSmartPointerInstance((DWORD)lua_touserdata(m_L, -1));

	FireClick(rawInstancePtr, distance, rawPlayerInstancePtr);
	return 0;
}


void Register(lua_State* L)
{
	lua_register(L, "getrawmetatable", getrawmetatable);
	lua_register(L, "getgenv", getgenv);
	lua_register(L, "getrenv", getrenv);
	lua_register(L, "getreg", getreg);
	lua_register(L, "getfenv", getfenv);
	lua_register(L, "getsenv", getsenv);
	lua_register(L, "getmenv", getsenv);
	lua_register(L, "gethwid", GetHardwareID);
	//lua_register(L, "fireclickdetector", fireclickdetector);
}
