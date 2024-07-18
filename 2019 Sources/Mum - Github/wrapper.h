#pragma once
#include "globals.h"
#include <string>
#include <vector>
#include "r_lua.h"
#include "HttpGet.h"
#include "Sk8r.h"
extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
}

int registry;

namespace Bridge
{
	DWORD m_rL;
	lua_State* m_L;

	void push(lua_State* L, DWORD rL, int index);
	void push(DWORD rL, lua_State* L, int index);

	int rbxFunctionBridge(DWORD rL);
	int vanillaFunctionBridge(lua_State* L);
	LONG WINAPI vehHandler(PEXCEPTION_POINTERS ex);
	VOID VehHandlerpush();
	std::vector<int> int3breakpoints;
	int resumea(DWORD thread);
}

namespace Bridge
{
	LONG WINAPI vehHandler(PEXCEPTION_POINTERS ex)
	{
		switch (ex->ExceptionRecord->ExceptionCode)
		{
		case (DWORD)0x80000003L:
		{
			if (ex->ContextRecord->Eip == int3breakpoints[0])
			{
				ex->ContextRecord->Eip = (DWORD)(Bridge::rbxFunctionBridge);
				return EXCEPTION_CONTINUE_EXECUTION;
			}

			if (ex->ContextRecord->Eip == int3breakpoints[1])
			{
				ex->ContextRecord->Eip = (DWORD)(Bridge::resumea);
				return EXCEPTION_CONTINUE_EXECUTION;
			}
			return -1;
		}
		default: return 0;
		}
		return 0;
	}

	DWORD locateINT3() {
		DWORD _s = x(0x400000);
		const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
		for (int i = 0; i < MAX_INT; i++) {
			if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {
				return (_s + i);
			}
		}
		return NULL;
	}

	VOID VehHandlerpush()
	{
		int3breakpoints.push_back(locateINT3());
		int3breakpoints.push_back(locateINT3());
		AddVectoredExceptionHandler(1, vehHandler);
	}

	void push(lua_State* L, DWORD rL, int index)
	{
		switch (lua_type(L, index))
		{
		case LUA_TLIGHTUSERDATA:
			r_lua_pushlightuserdata(rL, nullptr);

			break;
		case LUA_TNIL:
			r_lua_pushnil(rL);
			break;
		case LUA_TNUMBER:
			r_lua_pushnumber(rL, lua_tonumber(L, index));
			break;
		case LUA_TBOOLEAN:
			r_lua_pushboolean(rL, lua_toboolean(L, index));
			break;
		case LUA_TSTRING:
			r_lua_pushstring(rL, lua_tostring(L, index));
			break;
		case LUA_TTHREAD:
			r_lua_newthread(rL);
			break;
		case LUA_TFUNCTION:
			lua_pushvalue(L, index);
			r_lua_pushnumber(rL, luaL_ref(L, LUA_REGISTRYINDEX));
			r_lua_pushcclosure(rL, Bridge::int3breakpoints[0], 1);
			break;
		case LUA_TTABLE:
			lua_pushvalue(L, index);
			r_lua_newtable(rL);
			lua_pushnil(L);
			while (lua_next(L, -2) != LUA_TNIL)
			{
				Bridge::push(L, rL, -2);
				Bridge::push(L, rL, -1);
				r_lua_settable(rL, -3);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
			break;
		case LUA_TUSERDATA:
			lua_pushvalue(L, index);
			lua_gettable(L, LUA_REGISTRYINDEX);
			if (!lua_isnil(L, -1))
				r_lua_getfield(rL, LUA_REGISTRYINDEX, lua_tostring(L, -1));
			else
				r_lua_newuserdata(rL, 0);
			lua_pop(L, 1);
			break;
		default: break;
		}
	}

	void push(DWORD rL, lua_State* L, int index)
	{
		switch (r_lua_type(rL, index))
		{
		case R_LUA_TLIGHTUSERDATA:
			lua_pushlightuserdata(L, nullptr);
			break;
		case R_LUA_TNIL:
			lua_pushnil(L);
			break;
		case R_LUA_TNUMBER:
			lua_pushnumber(L, r_lua_tonumber(rL, index));
			break;
		case R_LUA_TBOOLEAN:
			lua_pushboolean(L, r_lua_toboolean(rL, index));
			break;
		case R_LUA_TSTRING:
			lua_pushstring(L, r_lua_tostring(rL, index));
			break;
		case R_LUA_TTHREAD:
			lua_newthread(L);
			break;
		case R_LUA_TFUNCTION:
			r_lua_pushvalue(rL, index);
			lua_pushnumber(L, r_luaL_ref(rL, LUA_REGISTRYINDEX));
			lua_pushcclosure(L, vanillaFunctionBridge, 1);
			break;
		case R_LUA_TTABLE:
			r_lua_pushvalue(rL, index);
			lua_newtable(L);
			r_lua_pushnil(rL);
			while (r_lua_next(rL, -2) != R_LUA_TNIL)
			{
				Bridge::push(rL, L, -2);
				Bridge::push(rL, L, -1);
				lua_settable(L, -3);
				r_lua_pop(rL, 1);
			}
			r_lua_pop(rL, 1);
			break;
		case R_LUA_TUSERDATA:
			r_lua_pushvalue(rL, index);
			r_lua_pushstring(rL, std::to_string(++registry).c_str());

			r_lua_pushvalue(rL, -2);
			r_lua_settable(rL, LUA_REGISTRYINDEX);
			r_lua_pop(rL, 1);
			lua_newuserdata(L, 0);
			lua_pushvalue(L, -1);
			lua_pushstring(L, std::to_string(registry).c_str());
			lua_settable(L, LUA_REGISTRYINDEX);
			r_lua_getmetatable(rL, index);

			Bridge::push(rL, L, -1);
			r_lua_pop(rL, 1);
			lua_setmetatable(L, -2);
			break;
		default: break;
		}
	}

	static int resume(lua_State* thread)
	{
		lua_State* L = lua_tothread(thread, lua_upvalueindex(1));
		const int nargs = lua_gettop(thread);
		lua_xmove(thread, L, nargs);
		return lua_resume(L, nargs);
		lua_close(L);

	}

	int resumea(DWORD thread)
	{
		lua_State* L = (lua_State*)r_lua_touserdata(thread, lua_upvalueindex(1));
		const int nargs = r_lua_gettop(thread);
		for (int arg = 1; arg <= nargs; ++arg)
			Bridge::push(thread, L, arg);
		return lua_resume(L, nargs);
		lua_close(L);

	}

	int vanillaFunctionBridge(lua_State* L)
	{
		r_lua_pushstring(m_rL, std::to_string(++registry).c_str());
		DWORD rL = r_lua_newthread(m_rL);
		r_lua_settable(m_rL, LUA_REGISTRYINDEX);

		int key = lua_tonumber(L, lua_upvalueindex(1));

		r_lua_rawgeti(rL, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= lua_gettop(L); ++arg)
			Bridge::push(L, rL, arg);

		if (r_lua_pcall(rL, lua_gettop(L), LUA_MULTRET, 0))
		{
			const char* errormessage = r_lua_tostring(rL, -1, 0);

			if (!errormessage || strlen(errormessage) == 0)
				errormessage = "Error occoured, no output from Lua\n";

			if (strcmp(errormessage, "attempt to yield across metamethod/C-call boundary") == 0)
			{

				r_lua_pop(rL, 1);
				lua_pushthread(L);
				lua_pushcclosure(L, &resume, 1);
				Bridge::push(L, rL, -1);

				return lua_yield(L, 0);
			}
			printf("Embox Vanilla Error: %s\n", r_lua_tostring(rL, -1));
			return 0;
			MessageBoxA(NULL, "SUCCESS VANILLA", "vanillabridge", NULL);
			delete[] errormessage;
		}

		int args = 0;

		for (int arg = 1; arg <= r_lua_gettop(rL); ++arg, ++args)
			Bridge::push(rL, L, arg);

		r_lua_settop(rL, 0);

		return args;
		lua_close(L);
		MessageBoxA(NULL, "SUCCESS VANILLA 2", "vanillabridge", NULL);
	}

	int rbxFunctionBridge(DWORD rL)
	{

		lua_pushstring(m_L, std::to_string(++registry).c_str());
		lua_State* L = lua_newthread(m_L);
		lua_settable(m_L, LUA_REGISTRYINDEX);

		int key = r_lua_tonumber(rL, lua_upvalueindex(1));

		lua_rawgeti(L, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= r_lua_gettop(rL); ++arg)
			Bridge::push(rL, L, arg);

		switch (lua_pcall(L, r_lua_gettop(rL), LUA_MULTRET, 0))
		{
		
		
		case LUA_YIELD:

			r_lua_pushlightuserdata(m_rL, (void*)L);
			r_lua_pushcclosure(m_rL, Bridge::int3breakpoints[1], 1);
			return -1;
		case LUA_ERRRUN:
				printf("Embox Roblox Error: %s\n", lua_tostring(L, -1));
				return -1;
		default: break;
		}

		int args = 0;

		for (int arg = 1; arg <= lua_gettop(L); ++arg, ++args)
			Bridge::push(L, rL, arg);

		lua_settop(L, 0);

		return args;
		lua_close(L);

	}

	static int newwait(lua_State* L) {
		r_lua_getglobal(m_rL, "wait");
		int ArgCount = 0;
		for (int x = lua_gettop(L); x != 0; x -= 1) {
			push(L, m_rL, x * -1);
			ArgCount += 1;
		}
		r_lua_pcall(m_rL, ArgCount, 0, 0);
		return ArgCount;
	}

	static int newspawn(lua_State* L) {
		r_lua_getglobal(m_rL, "spawn");
		int ArgCount = 0;
		for (int x = lua_gettop(L); x != 0; x -= 1) {
			push(L, m_rL, x * -1);
			ArgCount += 1;
		}
		r_lua_pcall(m_rL, ArgCount, 0, 0);
		return ArgCount;
	}

	void toClipboard(const std::string &s) {
		OpenClipboard(0);
		EmptyClipboard();
		HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
		if (!hg) {
			CloseClipboard();
			return;
		}
		memcpy(GlobalLock(hg), s.c_str(), s.size());
		GlobalUnlock(hg);
		SetClipboardData(CF_TEXT, hg);
		CloseClipboard();
		GlobalFree(hg);
	}

	void fn_toclipboard(lua_State *L) {
		std::string tc = lua_tostring(L, -1);
		toClipboard(tc);
	}

	int MouseButton1Down(lua_State* L) {
		INPUT input;
		input.type = INPUT_MOUSE;
		memset(&input.mi, 0, sizeof(MOUSEINPUT));
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(input));
		return 0;
	}

	int MouseButton2Down(lua_State* L) {
		INPUT input;
		input.type = INPUT_MOUSE;
		memset(&input.mi, 0, sizeof(MOUSEINPUT));
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
		SendInput(1, &input, sizeof(input));
		return 0;
	}

	int MouseButton1Up(lua_State* L) {
		INPUT input;
		input.type = INPUT_MOUSE;
		memset(&input.mi, 0, sizeof(MOUSEINPUT));
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(input));
		return 0;
	}

	int MouseButton2Up(lua_State* L) {
		INPUT input;
		input.type = INPUT_MOUSE;
		memset(&input.mi, 0, sizeof(MOUSEINPUT));
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		SendInput(1, &input, sizeof(input));
		return 0;
	}
/* HttpGet yeet */
	static int Lua_GetSite(lua_State* L) {
		lua_pushstring(L, std::string(DownloadURL(lua_tostring(L, -1))).c_str());
		return 1;
	};

	static int MT__index(lua_State *L) {
		std::string key = std::string(lua_tolstring(L, -1, NULL));
		if (key == std::string("HttpGet")) {
			lua_pushcfunction(L, Lua_GetSite);
		}
		else if (key == std::string("GetObjects")) {
			lua_pushcfunction(L, Custom_GetObjects);
		}
		else {
			push(L, m_rL, 1);
			r_lua_getfield(m_rL, -1, key.c_str());
			push(m_rL, L, -1);
		};
		return 1;
	};

	int MT__newindex(lua_State *L) {
		const char* key = lua_tostring(L, -2);
		push(L, m_rL, -3);
		push(L, m_rL, -1);
		r_lua_setfield(m_rL, -2, key);
		r_lua_pop(m_rL, 1);
		return 1;
	}

	int getgenv(lua_State *L) {
		lua_pushvalue(m_L, LUA_GLOBALSINDEX);
		Bridge::push(m_rL, L, -1);
		return 1;
	}

	int getreg(lua_State *L) {
		lua_pushvalue(m_L, LUA_REGISTRYINDEX);
		Bridge::push(m_rL, L, -1);
		return 1;
	}

	void RegisterCF(lua_State *L) {
		lua_register(L, "wait", reinterpret_cast<lua_CFunction>(newwait));
		lua_register(L, "spawn", reinterpret_cast<lua_CFunction>(newspawn));
		lua_register(L, "clipboardset", (lua_CFunction)fn_toclipboard);
		//lua_register(L, "loadfile", (lua_CFunction)loadfile);
		lua_register(L, "getreg", getreg);
		lua_register(L, "getgenv", getgenv);
	}

	void AddFunction(lua_State* L, std::string Name, lua_CFunction function) {
		lua_newtable(L);
		lua_pushcfunction(L, function);
		lua_setglobal(L, Name.c_str());
		lua_settop(L, 0);
	}

	void RegisterMT(lua_State *L) {
		lua_newtable(L);
		lua_pushcfunction(L, MT__index);
		lua_setfield(L, -2, "__index");
		lua_pushcfunction(L, MT__newindex);
		lua_setfield(L, -2, "__newindex");
		lua_pushstring(L, "The metatable is locked");
		lua_setfield(L, -2, "__metatable");
		lua_setfield(L, LUA_REGISTRYINDEX, "MetaTableMagic");
	}

	void PushGlobal1(DWORD rL, lua_State* L, const char* s)
	{
		r_lua_getglobal(rL, s);
		Bridge::push(rL, L, -1);
		lua_setglobal(L, s);
		r_lua_pop(rL, 1);
	}

/* Ugh, i am David. These are all things that it loads i think.... Ya know more than me so ya prob know */
		PushGlobal1(m_rL, L, "Axes");
		PushGlobal1(m_rL, L, "BrickColor");
		PushGlobal1(m_rL, L, "CFrame");
		PushGlobal1(m_rL, L, "Color3");
		PushGlobal1(m_rL, L, "ColorSequence");
		PushGlobal1(m_rL, L, "ColorSequenceKeypoint");
		PushGlobal1(m_rL, L, "NumberRange");
		PushGlobal1(m_rL, L, "NumberSequence");
		PushGlobal1(m_rL, L, "NumberSequenceKeypoint");
		PushGlobal1(m_rL, L, "PhysicalProperties");
		PushGlobal1(m_rL, L, "Ray");
		PushGlobal1(m_rL, L, "Rect");
		PushGlobal1(m_rL, L, "Region3");
		PushGlobal1(m_rL, L, "Region3int16");
		PushGlobal1(m_rL, L, "TweenInfo");
		PushGlobal1(m_rL, L, "UDim");
		PushGlobal1(m_rL, L, "UDim2");
		PushGlobal1(m_rL, L, "Vector2");
		PushGlobal1(m_rL, L, "Vector2int16");
		PushGlobal1(m_rL, L, "Vector3");
		PushGlobal1(m_rL, L, "Vector3int16");
		PushGlobal1(m_rL, L, "Enum");
		PushGlobal1(m_rL, L, "Faces");
		PushGlobal1(m_rL, L, "Instance");
		PushGlobal1(m_rL, L, "math");
		PushGlobal1(m_rL, L, "warn");
		PushGlobal1(m_rL, L, "typeof");
		PushGlobal1(m_rL, L, "type");
		PushGlobal1(m_rL, L, "spawn");
		PushGlobal1(m_rL, L, "Spawn");
		PushGlobal1(m_rL, L, "print");
		PushGlobal1(m_rL, L, "printidentity");
		PushGlobal1(m_rL, L, "ypcall");
		PushGlobal1(m_rL, L, "Wait");
		PushGlobal1(m_rL, L, "wait");
		PushGlobal1(m_rL, L, "delay");
		PushGlobal1(m_rL, L, "Delay");
		PushGlobal1(m_rL, L, "tick");
		PushGlobal1(m_rL, L, "LoadLibrary");
	} */

	void SetUpMum(lua_State *L) {
		PushGlobal1(m_rL, L, "game");
		PushGlobal1(m_rL, L, "Game");
		PushGlobal1(m_rL, L, "workspace");
		PushGlobal1(m_rL, L, "Workspace");
		RegisterMT(L);
		RegisterRF(L);
		RegisterCF(L);
        /* Add function... Like wtf! Explain to me*/
		AddFunction(L, "MouseButton1Up", MouseButton1Up);
		AddFunction(L, "MouseButton2Up", MouseButton2Up);
		AddFunction(L, "MouseButton1Down", MouseButton1Down);
		AddFunction(L, "MouseButton2Down", MouseButton2Down); 
	}
}
