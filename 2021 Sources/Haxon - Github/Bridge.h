#pragma once
#include <string>
#include <vector>
#include "r_lua.h"
extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
}

#include <unordered_map>

#define AXONDEBUG false


struct Userdata
{
	int32_t reference;
};

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

#include <Windows.h>
#include <WinINet.h>
#include <string>
#include <WinInet.h>
#include "Globals.h"

#pragma comment(lib, "wininet") 

using namespace std;




std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string DownloadURL(const char* URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL, NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}


//static int UserDataGC(lua_State* Thread) {
//	void* UD = lua_touserdata(Thread, 1);
//	if (Bridge::m_rL) {
//
//		r_lua_rawgeti(Bridge::m_rL, LUA_REGISTRYINDEX, (int)UD);
//		if (r_lua_type(Bridge::m_rL, -1) <= R_LUA_TNIL) {
//			lua_pushnil(Thread);
//			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
//
//		}
//	}
//	return 0;
//}

static int GarbageCollector(lua_State* L) {
	void* UD = lua_touserdata(L, 1);
	lua_pushvalue(L, 1);
	lua_gettable(L, LUA_REGISTRYINDEX);
	if (!lua_isnil(L, -1)) {
		int Reference = lua_tointeger(L, -1);
		r_lua_pushnil(Bridge::m_rL);
		r_lua_rawseti(Bridge::m_rL, LUA_REGISTRYINDEX, Reference);
		r_lua_settop(Bridge::m_rL, 0);
		MessageBox(0, "Gar Collected", "Collector", 0);
		return 1;
	}
	lua_pop(L, 1);
	return 0;
}


namespace Bridge
{


	static int DownloadString(lua_State* L) {
		int args = lua_gettop(L) - 1;
		std::string url;
		if (args == 2) {
			url = DownloadURL(lua_tostring(L, -2));
		}
		else {
			url = DownloadURL(lua_tostring(L, -1));
		}
		lua_pushstring(L, url.c_str());
		return 1;
	}


	static int GetObjects(lua_State* L) {
		auto asset = lua_tostring(L, -1);
		auto hold = std::string("return {game:GetService('InsertService'):LoadLocalAsset('") + std::string(asset) + std::string("')}");
		luaL_dostring(L, hold.c_str());
		return 1;
	}
	int IndexYeet(lua_State* L) {
		const char* key = lua_tostring(L, -1);
		//std::cout << key << std::endl;
		if (key == std::string("HttpGet")) {
			//lua_getglobal(L, "HttpGet");
			lua_pushcfunction(L, DownloadString);
		}
		else if (key == std::string("GetObjects")) {
			lua_pushcfunction(L, GetObjects);
		}

		else
		{
			//DebugOutput("Index Key: ", key);
			push(L, m_rL, 1);
			r_lua_getfield(m_rL, -1, key);
			push(m_rL, L, -1);
			//r_lua_settop(RLS, -2);
		}
		return 1;
	}


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
			r_lua_pushcclosure(rL, int3breakpoints[0], NULL, 1, NULL);
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
			r_lua_rawgeti(rL, -10000, reinterpret_cast<Userdata*>(lua_touserdata(L, index))->reference);
			if (!r_lua_type(rL, -1))
			{
				r_lua_settop(rL, -2);
				r_lua_newuserdata(rL, 0, 0);
			}

			break;
		}
	}
	void push(DWORD rL, lua_State* L, int index)
	{

		int RTType = r_lua_type(rL, index);

		if (RTType == R_LUA_TLIGHTUSERDATA)
		{
			lua_pushlightuserdata(L, nullptr);
		}
		if (RTType == R_LUA_TNIL)
		{
			lua_pushnil(L);
		}
		else if (RTType == R_LUA_TNUMBER)
		{
			lua_pushnumber(L, r_lua_tonumber(rL, index, 0));
		}
		else if (RTType == R_LUA_TBOOLEAN)
		{
			lua_pushboolean(L, r_lua_toboolean(rL, index));
		}
		else if (RTType == R_LUA_TSTRING)
		{
			lua_pushstring(L, r_lua_tostring(rL, index));
		}
		else if (RTType == R_LUA_TTHREAD)
		{
			lua_newthread(L);
		}
		else if (RTType == R_LUA_TFUNCTION)
		{
			r_lua_pushvalue(rL, index);
			lua_pushnumber(L, r_luaL_ref(rL, LUA_REGISTRYINDEX));
			lua_pushcclosure(L, vanillaFunctionBridge, 1);
		}
		else if (RTType == R_LUA_TTABLE)
		{
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
		}
		else if (RTType == R_LUA_TUSERDATA)
		{
			uintptr_t rawInstancePtr = r_lua_touserdata(rL, index);
			lua_rawgeti(L, LUA_REGISTRYINDEX, rawInstancePtr);
			if (!lua_type(L, -1))
			{
				lua_settop(L, -2);
				r_lua_pushvalue(rL, index);
				reinterpret_cast<Userdata*>(lua_newuserdata(L, sizeof(Userdata)))->reference = r_luaL_ref(rL, -10000);
				r_lua_getmetatable(rL, index);
				Bridge::push(rL, L, -1);
				lua_pushcfunction(L, IndexYeet);
				lua_setfield(L, -2, "__index");
				lua_setmetatable(L, -2);
				lua_pushvalue(L, -1);
				lua_rawseti(L, -10000, rawInstancePtr);
				r_lua_settop(rL, -2);
			}
		}
		else
		{
			lua_pushnil(L);
		}
	}

	static int resume(lua_State* thread)
	{
		lua_State* L = lua_tothread(thread, lua_upvalueindex(1));
		const int nargs = lua_gettop(thread);
		lua_xmove(thread, L, nargs);
		return lua_resume(L, nargs);
		lua_newtable(L);
		lua_pushstring(L, "This metatable is locked");
		lua_setfield(L, -2, "__metatable");
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
			//r_luaL_error(m_rL, lua_tostring(L, -1));
			return luaL_error(L, errormessage);
			//MessageBoxA(NULL, "SUCCESS VANILLA", "vanillabridge", NULL);
			delete[] errormessage;
		}

		int args = 0;

		for (int arg = 1; arg <= r_lua_gettop(rL); ++arg, ++args)
			Bridge::push(rL, L, arg);

		r_lua_settop(rL, 0);

		return args;
		lua_close(L);
		//MessageBoxA(NULL, "SUCCESS VANILLA 2", "vanillabridge", NULL);
	}

	int rbxFunctionBridge(DWORD rL)
	{

		lua_pushstring(m_L, std::to_string(++registry).c_str());
		lua_State* L = lua_newthread(m_L);
		lua_settable(m_L, LUA_REGISTRYINDEX);

		int key = r_lua_tonumber(rL, lua_upvalueindex(1), 0);

		lua_rawgeti(L, LUA_REGISTRYINDEX, key);

		for (int arg = 1; arg <= r_lua_gettop(rL); ++arg)
			Bridge::push(rL, L, arg);

		switch (lua_pcall(L, r_lua_gettop(rL), LUA_MULTRET, 0))
		{


		case LUA_YIELD:

			r_lua_pushlightuserdata(m_rL, (void*)L);
			r_lua_pushcclosure(m_rL, int3breakpoints[1], NULL, 1, NULL);

			return -1;
		case LUA_ERRRUN:
			//printf("RVX ROBLOX ERROR: %s\n", lua_tostring(L, -1));
			r_luaL_error(rL, luaL_checkstring(L, -1));
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
}