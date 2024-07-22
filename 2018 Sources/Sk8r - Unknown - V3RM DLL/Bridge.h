#pragma once
#include "globals.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "r_lua.h"
#include "Httpget.h"
#include <vector>
extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\lstate.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lapi.h"
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

	static int UserDataGC(lua_State *Thread) {
		void *UD = lua_touserdata(Thread, 1);
		if (m_rL) {
			r_lua_rawgeti(m_rL, LUA_REGISTRYINDEX, (int)UD);
			if (r_lua_type(m_rL, -1) <= R_LUA_TNIL) {
				lua_pushnil(Thread);
				lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);

			}
		}
		return 0;
	}

	VOID VehHandlerpush()
	{
		int3breakpoints.push_back(locateINT3());
		int3breakpoints.push_back(locateINT3());
		AddVectoredExceptionHandler(1, vehHandler);
	}

	/*void CacheInstances(lua_State* L, DWORD rL) {
		std::unordered_map<std::string, std::string> Userdatas = {
			{"DataModel", "DataModel"},
		};

		for (const auto& Data : Userdatas) {
			int* Userdata = (int*)r_lua_touserdata(rL, lua_upvalueindex(1));
			lua_rawgeti(L, LUA_REGISTRYINDEX, (int)Userdata);
			int* Instance = r_lua_newuserdata(rL, sizeof(Userdata));
			if (Instance) {
				r_lua_getmetatable(rL, -1);
				MessageBox(NULL, "PROBLEM 1", "PROBLEM 1", MB_OK | MB_ICONQUESTION);
				//MessageBox(NULL, (LPSTR) rL, "rL", MB_OK | MB_ICONQUESTION);
				r_lua_getfield(rL, LUA_REGISTRYINDEX, lua_tostring(L, -1));
				MessageBox(NULL, "PROBLEM 2", "PROBLEM 2", MB_OK | MB_ICONQUESTION);
				if (r_lua_type(rL, -1) == R_LUA_TUSERDATA) {
					int* U = (int*)r_lua_touserdata(rL, -1);
					r_lua_pop(rL, 1);
					lua_pushlightuserdata(L, (void*)U);
					lua_gettable(L, LUA_REGISTRYINDEX);
					if (lua_type(L, -1) == LUA_TUSERDATA) {
						MessageBox(NULL, "TEST1", "TEST1", MB_OK | MB_ICONQUESTION);
						lua_pop(L, -1);
						MessageBox(NULL, "TEST1", "TEST1", MB_OK | MB_ICONQUESTION);
						//lua_pushcfunction(L, UserDataGC);
						MessageBox(NULL, "TEST2", "TEST2", MB_OK | MB_ICONQUESTION);
						//lua_setfield(L, -2, "__gc");
						MessageBox(NULL, "TEST3", "TEST3", MB_OK | MB_ICONQUESTION);
						lua_pushlightuserdata(L, (void*)U);
						lua_pushvalue(L, -2);
						lua_settable(L, LUA_REGISTRYINDEX);
					}
					else {
						r_lua_pop(rL, 1);
					}
				}
				lua_setmetatable(L, -2);
				lua_pushvalue(L, -1);
				lua_rawseti(L, LUA_REGISTRYINDEX, (int)Userdata);
				r_lua_rawgeti(rL, LUA_REGISTRYINDEX, (int)Userdata);
				if (r_lua_type(rL, -1) <= R_LUA_TNIL) {
					r_lua_pop(rL, 1);
					r_lua_pushvalue(rL, -1);
					r_lua_rawseti(rL, LUA_REGISTRYINDEX, (int)Userdata);
				}
			}
		}
		r_lua_rawgeti(rL, LUA_REGISTRYINDEX, *(int *)lua_touserdata(L, -2));
		r_luaL_getmetafield(rL, -1, "__gc");
		r_lua_pushvalue(rL, -2);
		r_lua_rawgeti(rL, LUA_REGISTRYINDEX, *(int *)lua_touserdata(L, -1));
		r_lua_pcall(rL, 2, 1, 0);
		push(L, rL, -1);
		return;
	}*/

	void push(lua_State* L, DWORD rL, int index)
	{
		//printf("ROBLOX: %d\n", lua_type(L, index));
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
		case LUA_TNONE:
			r_lua_pushnil(rL);
			break;
		default: break;
		}
	}

	void push(DWORD rL, lua_State* L, int index)
	{
		//printf("Skater: %d\r\n", r_lua_type(rL, index));
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
		case R_LUA_TNONE:
			lua_pushnil(L);
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
	}

	int resumea(DWORD thread)
	{
		lua_State* L = (lua_State*)r_lua_touserdata(thread, lua_upvalueindex(1));
		const int nargs = r_lua_gettop(thread);
		for (int arg = 1; arg <= nargs; ++arg)
			Bridge::push(thread, L, arg);
		return lua_resume(L, nargs);
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
			const char* error = lua_tolstring(m_L, -1, 0);
			//luaL_dostring(m_L, (std::string("warn('") += std::string(lua_tostring(m_L, -1)) += std::string("');")).c_str());
			//printf("Error: %s\n", error);//r_lua_pushstring(m_rL, (std::string("warn('") += std::string(error) += std::string("');")).c_str());
			return 0;
			delete[] errormessage;
		}

		int args = 0;

		for (int arg = 1; arg <= r_lua_gettop(rL); ++arg, ++args)
			Bridge::push(rL, L, arg);

		r_lua_settop(rL, 0);

		return args;
		lua_close(L);
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
				//printf("SK8R ERROR: %s\n", lua_tostring(L, -1));
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

	int CopyString(lua_State *Ls) {
		const char* str = lua_tostring(Ls, -1);

		int len = strlen(str);
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
		void *mem_ptr = GlobalLock(hMem);
		memcpy(mem_ptr, str, len + 1);
		GlobalUnlock(hMem);
		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
		return 0;
	}

	void RegisterCF(lua_State *L) {
		lua_register(L, "setclipboard", CopyString);
	}

	void AddFunction(lua_State* L, std::string Name, lua_CFunction function) {
		lua_newtable(L);
		lua_pushcfunction(L, function);
		lua_setglobal(L, Name.c_str());
		lua_settop(L, 0);
	}

	void PushGlobal(DWORD rL, lua_State* L, const char* s)
	{
		r_lua_getglobal(rL, s);
		Bridge::push(rL, L, -1);
		lua_setglobal(L, s);
		r_lua_pop(rL, 1);
	}

	void RegisterRF(lua_State *L) {
		PushGlobal(m_rL, m_L, "game");
		PushGlobal(m_rL, m_L, "Game");
		PushGlobal(m_rL, m_L, "workspace");
		PushGlobal(m_rL, m_L, "Workspace");
		PushGlobal(m_rL, m_L, "Axes");
		PushGlobal(m_rL, m_L, "BrickColor");
		PushGlobal(m_rL, m_L, "CFrame");
		PushGlobal(m_rL, m_L, "Color3");
		PushGlobal(m_rL, m_L, "ColorSequence");
		PushGlobal(m_rL, m_L, "ColorSequenceKeypoint");
		PushGlobal(m_rL, m_L, "NumberRange");
		PushGlobal(m_rL, m_L, "NumberSequence");
		PushGlobal(m_rL, m_L, "NumberSequenceKeypoint");
		PushGlobal(m_rL, m_L, "PhysicalProperties");
		PushGlobal(m_rL, m_L, "Ray");
		PushGlobal(m_rL, m_L, "Rect");
		PushGlobal(m_rL, m_L, "Region3");
		PushGlobal(m_rL, m_L, "Region3int16");
		PushGlobal(m_rL, m_L, "TweenInfo");
		PushGlobal(m_rL, m_L, "UDim");
		PushGlobal(m_rL, m_L, "UDim2");
		PushGlobal(m_rL, m_L, "Vector2");
		PushGlobal(m_rL, m_L, "Vector2int16");
		PushGlobal(m_rL, m_L, "Vector3");
		PushGlobal(m_rL, m_L, "Vector3int16");
		PushGlobal(m_rL, m_L, "Enum");
		PushGlobal(m_rL, m_L, "time");
		PushGlobal(m_rL, m_L, "Faces");
		PushGlobal(m_rL, m_L, "Instance");
		PushGlobal(m_rL, m_L, "math");
		PushGlobal(m_rL, m_L, "warn");
		PushGlobal(m_rL, m_L, "typeof");
		PushGlobal(m_rL, m_L, "type");
		PushGlobal(m_rL, m_L, "spawn");
		PushGlobal(m_rL, m_L, "Spawn");
		PushGlobal(m_rL, m_L, "print");
		PushGlobal(m_rL, m_L, "info");
		PushGlobal(m_rL, m_L, "error");
		PushGlobal(m_rL, m_L, "printidentity");
		PushGlobal(m_rL, m_L, "ypcall");
		PushGlobal(m_rL, m_L, "Wait");
		PushGlobal(m_rL, m_L, "wait");
		PushGlobal(m_rL, m_L, "delay");
		PushGlobal(m_rL, m_L, "Delay");
		PushGlobal(m_rL, m_L, "tick");
		PushGlobal(m_rL, m_L, "LoadLibrary");
	}

	void SetUpEnvironment(lua_State *L) {
		RegisterCF(L);
		RegisterRF(L);
	}
	static int GlobalsIndex(lua_State *Thread) {
		try {
			int RThread = r_lua_newthread(m_rL);
			if (!RThread) return luaL_error(Thread, "Roblox's Thread Was Incorrect");
			if (lua_type(Thread, 2) != LUA_TSTRING) return luaL_error(Thread, "Argument 2 Expected String, Got %s", lua_typename(Thread, lua_type(Thread, 1)));
			r_lua_getglobal(RThread, lua_tostring(Thread, 2));
			printf("Wrapping Cached Global: %s", lua_tostring(Thread, 2));
			if (r_lua_type(RThread, -1) > R_LUA_TNIL) {
				Bridge::push(RThread, Thread, -1);
				return 1;
			}
		}
		catch (exception e) {
			return luaL_error(Thread, "%s", e.what());
		}
		catch (...) {
			return luaL_error(Thread, "C++ exception thrown");
		}
		return 0;
	}
}
