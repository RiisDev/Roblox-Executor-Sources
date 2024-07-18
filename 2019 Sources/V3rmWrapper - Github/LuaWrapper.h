#pragma once
#include <Windows.h>
#include <string>
#include <iostream>

#include "Offsets.h"
#include "ConsoleCol.h"

int VanillaFunctionHandler(lua_State* L);
void Wrap(lua_State* LS, DWORD RLS, int Index);
struct UserDataCachingStruct {
	int ReferenceToUserData;
};

typedef int UserDataHandle;

namespace AOB {
	bool Check(const BYTE* pd, const BYTE* aob, const char* mask)
	{
		for (; *mask; ++mask, ++pd, ++aob)
			if (*mask != '?' && *pd != *aob)
				return false;

		return (*mask) == NULL;
	}

	DWORD FindPattern(const char* aob, const char* mask)
	{
		for (DWORD ind = (DWORD)GetModuleHandle(0); ind <= 0xFFFFFFF; ++ind) {
			if (Check((BYTE*)ind, (BYTE*)aob, mask))
				return ind;
		}
		return 0x00000000;
	}
}

DWORD int3breakpoint = AOB::FindPattern("\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC", "xxxxxxxx");

void UnWrap(DWORD RLS, lua_State* LS, int Index) {
	int Type = r_l_type(RLS, Index);
	std::cout << white << "Vanilla Type Used: " << r_l_type(RLS, Index) << "\n";
	switch (Type)
	{
	case R_LUA_TNIL: {
		lua_pushnil(LS);
		break;
	}
	case R_LUA_TLIGHTUSERDATA: {
		lua_pushlightuserdata(LS, r_l_touserdata(RLS, Index));
		break;
	}
	case R_LUA_TNUMBER: {
		lua_pushnumber(LS, r_l_tonumber(RLS, Index));
		break;
	}
	case R_LUA_TBOOLEAN: {
		lua_pushboolean(LS, r_l_toboolean(RLS, Index));
		break;
	}
	case R_LUA_TSTRING: {
		lua_pushstring(LS, r_l_tolstring(RLS, Index, 0));
		break;
	}
	case R_LUA_TTHREAD: {
		lua_newthread(LS);
		break;
	}
	case R_LUA_TFUNCTION: {
		r_l_pushvalue(RLS, Index);
		lua_pushnumber(LS, r_l_lref(RLS, LUA_REGISTRYINDEX));
		lua_pushcclosure(LS, VanillaFunctionHandler, 1);
		break;
	}
	case R_LUA_TTABLE: {
		r_l_pushvalue(RLS, Index);
		lua_newtable(LS);
		r_l_pushnil(RLS);
		while (r_l_next(RLS, -2) != R_LUA_TNIL)
		{
			UnWrap(RLS, LS, -2);
			UnWrap(RLS, LS, -1);
			lua_settable(LS, -3);
			r_l_pop(RLS, 1);
		}
		r_l_pop(RLS, 1);
		break;
	}
	case R_LUA_TUSERDATA: {
		UserDataHandle UD = (UserDataHandle)r_l_touserdata(RLS, Index);
		lua_rawgeti(LS, LUA_REGISTRYINDEX, UD);
		if (!lua_isnil(LS, -1))
			break;
		lua_settop(LS, -2);
		r_l_pushvalue(RLS, Index);
		UserDataCachingStruct* UserData = (UserDataCachingStruct*)lua_newuserdata(LS, 8);
		UserData->ReferenceToUserData = r_l_lref(RLS, LUA_REGISTRYINDEX);
		r_l_getmetatable(RLS, Index);
		UnWrap(RLS, LS, -1); 
		lua_setmetatable(LS, -2);
		lua_getmetatable(LS, -1);
		lua_pushstring(LS, "__CachedUserData");
		lua_pushboolean(LS, true);
		lua_settable(LS, -3);
		lua_settop(LS, -2);
		lua_rawseti(LS, LUA_REGISTRYINDEX, UD);
		lua_rawgeti(LS, LUA_REGISTRYINDEX, UD);
		r_l_settop(RLS, -2);
	}
	default: break;
	}
}


void Wrap(lua_State* LS, DWORD RLS, int Index) {
	int Type = lua_type(LS, Index);
	std::cout << white << "Roblox Type Used: " << Type << "\n";
	switch (Type)
	{
	case LUA_TNIL: {
		r_l_pushnil(RLS);
		break;
	}
	case LUA_TLIGHTUSERDATA: {
		r_l_pushlightuserdata(RLS, lua_touserdata(LS, Index));
		break;
	}
	case LUA_TNUMBER: {
		r_l_pushnumber(RLS, lua_tonumber(LS, Index));
		break;
	}
	case LUA_TBOOLEAN: {
		r_l_pushboolean(RLS, lua_toboolean(LS, Index));
		break;
	}
	case LUA_TSTRING: {
		r_l_pushstring(RLS, lua_tostring(LS, Index));
		break;
	}
	case LUA_TTHREAD: {
		r_l_newthread(RLS);
		break;
	}
	case LUA_TFUNCTION: {
		lua_pushvalue(LS, Index);
		r_l_pushnumber(RLS, luaL_ref(LS, LUA_REGISTRYINDEX));
		r_l_pushcclosure(RLS, int3bp[0], 1);
		break;
	}
	case LUA_TTABLE: {
		lua_pushvalue(LS, Index);
		r_l_createtable(RLS, 0, 0);
		lua_pushnil(LS);
		while (lua_next(LS, -2) != LUA_TNIL)
		{
			Wrap(LS, RLS, -2);
			Wrap(LS, RLS, -1);
			r_l_settable(RLS, -3);
			lua_pop(LS, 1);
		}
		lua_pop(LS, 1);
		break;
	}
	case LUA_TUSERDATA: {
		if(luaL_getmetafield(LS, Index, "__CachedUserData")){
		lua_settop(LS, -2);
		UserDataCachingStruct* UserData = (UserDataCachingStruct*)lua_touserdata(LS, Index);
		r_l_rawgeti(RLS, LUA_REGISTRYINDEX, UserData->ReferenceToUserData);
		}
		else {
			lua_newuserdata(LS, 0);
		}
		break;
	}
	case LUA_TNONE: {
		r_l_pushnil(RLS);
		break;
	}
	default: break;
	}
}

static int LUAYield(lua_State* thread)
{
	lua_State* PassedLuaThread = lua_tothread(thread, lua_upvalueindex(1));
	const int NumOfArgs = lua_gettop(thread);
	lua_xmove(thread, PassedLuaThread, NumOfArgs);
	return lua_resume(PassedLuaThread, NumOfArgs);
}


int VanillaFunctionHandler(lua_State* L) {
	DWORD rL = r_l_newthread(RobloxState);
	
	int key = lua_tonumber(L, lua_upvalueindex(1));
	r_l_rawgeti(rL, LUA_REGISTRYINDEX, key);
	
	for (int FunctionArgs = 1; FunctionArgs <= lua_gettop(L); ++FunctionArgs) {
		Wrap(L, rL, FunctionArgs);
	}

	int PCall = r_l_pcall(rL, lua_gettop(L), LUA_MULTRET, 0);
	if (PCall) {
		std::string ErrorMessageSTR = r_l_tolstring(rL, -1, 0);
		if (ErrorMessageSTR.find("yield") != std::string::npos) {
			r_l_pop(rL, 1);
			lua_pushthread(L);
			lua_pushcclosure(L, &LUAYield, 1);
			Wrap(L, rL, -1);
			return lua_yield(L, 0);
		}
	}
	else {
		for (int FunctionArgs = 1; FunctionArgs <= r_l_gettop(rL); ++FunctionArgs) {
			UnWrap(rL, L, FunctionArgs);
		}
		r_l_settop(rL, 0);
	}
	return 1;
}

int RobloxFunctionHandler(DWORD rL) {
	lua_State* L = lua_newthread(VanillaState);

	int key = r_l_tonumber(rL, lua_upvalueindex(1));
	lua_rawgeti(L, LUA_REGISTRYINDEX, key);
	
	for (int FunctionArgs = 1; FunctionArgs <= r_l_gettop(rL); ++FunctionArgs) {
		UnWrap(rL, L, FunctionArgs);
	}
	
	int Err = lua_pcall(L, r_l_gettop(rL), LUA_MULTRET, 0);
	if (Err == LUA_ERRRUN) {
		std::cout << ired << "Roblox Error: " << lua_tostring(L, -1) << "\n";
		return 0;
	}
	for (int FunctionArgs = 1; FunctionArgs <= lua_gettop(L); ++FunctionArgs) {
		Wrap(L, rL, FunctionArgs);
	}
	lua_settop(L, 0);
	return 1;
}

using namespace std;

namespace Wrapper {
	void Execute_Script(std::string Script) {

		std::string Str;

		if (luaL_loadstring(VanillaState, Script.c_str())) {
			return;
		}
		else {
			lua_pcall(VanillaState, 0, 0, 0);
		}

	}
}



