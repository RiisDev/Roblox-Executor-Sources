#include "stdafx.h"


namespace Lua
{
	static void GetField(int luaState, int value, const char* field) {
		AD_GETFIELD(luaState, value, field);
	}

	static void GetGlobal(int luaState, const char* Global) {
		AD_GETFIELD(luaState, -10002, Global);
	}

	static void PushNumber(int luaState, double value) {
		AD_PUSHNUMBER(luaState, value);
	}

	static void PushBoolean(int luaState, bool boolean) {
		int property = *(DWORD *)(luaState + 16);
		*(DWORD*)property = boolean;
		*(DWORD *)(property + 8) = 3;
		*(DWORD *)(luaState + 16) += 16;
	}

	static void PushValue(int luaState, int value) {
		AD_PUSHVALUE(luaState, value);
	}

	static void PushString(int luaState, const char* string) {
		AD_PUSHSTRING(luaState, string);
	}

	static void PushCClosure(int luaState, void* function) {
		PUSHCCLOSURE(luaState, function, 0);
	}

	void SetField(int luaState, int value, const char* field) {
		AD_SETFIELD(luaState, value, field);
	}

	const char* ToString(int luaState, int string) {
		Retcheck_Disable();
		const char* String = AD_TO_STRING(luaState, string, NULL);
		Retcheck_Enable();
		return String;
	};

	void Pcall(int ls, int naArgs, int nResults, int errorFunction) {
		Retcheck_Disable();
		AD_PCALL(ls, naArgs, nResults, errorFunction);
		Retcheck_Enable();
	}

	void GetService(int luaState, std::string service) {
		GetGlobal(LuaState, "game");
		GetField(LuaState, -1, "GetService");
		PushValue(LuaState, -2);
		PushString(LuaState, service.c_str());
		Pcall(LuaState, 2, 1, 0);
	}

	static int SetTop(int luaState, int value) {
		Retcheck_Disable();
		int Stack = AD_SETTOP(luaState, value);
		Retcheck_Enable();
		return Stack;
	}

	static int Pop(int luaState, int value) {
		return SetTop(luaState, -(value)-1);
	}
}
