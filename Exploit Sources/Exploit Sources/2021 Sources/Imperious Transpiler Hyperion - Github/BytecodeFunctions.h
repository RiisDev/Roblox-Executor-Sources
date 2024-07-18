#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <ctime>
#include <queue>
#include "Tools.h"

#include "XHosted/curl/curl.h"

#include "Rlua.h"
#include "LuauSerializer.h"
#include "LuauTranspiler.h"
#include "CallCheck Bypass.h"

bool noobs = false;

union r_Value
{
	void* gc;
	void* p;
	double n;
	bool b;
};

typedef struct r_TValue
{
	r_Value value;
	byte tt;
	byte shit;
};

	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
		return size * nmemb;
	}

	std::string URL(std::string url)
	{
		std::string webBuffer;

		CURL* curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
			int http_code(0);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &webBuffer);
			CURLcode res = curl_easy_perform(curl);

			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
			curl_easy_cleanup(curl);

			if (res == CURLE_OK) {
				if (http_code == 200) {
					return webBuffer;
				}
				else {
					return "HTTP ERROR CODE: " + std::to_string(http_code);
				}
			}
			else if (res == CURLE_OPERATION_TIMEDOUT == res)
			{
				return "HTTPGET REQUEST TIMED OUT";
			}
		}
		return {};
	}

	int getreg_impl(DWORD rL) {
		if (r_lua_gettop(rL) != 0)
			throw std::runtime_error("getreg does not require arguments");
		r_lua_pushvalue(rL, LUA_REGISTRYINDEX);
		return 1;
	}

	int require(DWORD rL)
	{
		*reinterpret_cast<int*>(*reinterpret_cast<int*>(rL + EmptySpace1) + EmptySpace2) = 2;
		r_lua_getglobal(rL, "require");
		r_lua_pcall(rL, 1, 1, 1);
		*reinterpret_cast<int*>(*reinterpret_cast<int*>(rL + EmptySpace1) + EmptySpace2) = 6;
		return 1;
	}

	int getenv_impl(DWORD rL) {
		if (r_lua_gettop(rL) != 0)
			throw std::runtime_error("getgenv does not require arguments");
		r_lua_pushvalue(rL, LUA_GLOBALSINDEX);
		r_lua_xmove(rL, rL, 1);
		return 1;
	}

	int getgenv(uintptr_t rL)
	{
		if (r_lua_gettop(rL) != 0)
			throw std::runtime_error("getgenv does not require arguments");
		r_lua_pushvalue(rL, LUA_GLOBALSINDEX);
		r_lua_xmove(rL, rL, 1);
		return 1;
	}

	int getrenv(uintptr_t rL)
	{
		if (r_lua_gettop(rL) != 0)
			throw std::runtime_error("getrenv does not require arguments");
		r_lua_pushvalue(rL, LUA_GLOBALSINDEX);
		r_lua_xmove(rL, rL, 1);
		return 1;
	}

	int getrawmetatable_impl(DWORD ls) {
		if (r_lua_type(ls, -1) == RLUA_TUSERDATA || r_lua_type(ls, -1) == RLUA_TSTRING || r_lua_type(ls, -1) == RLUA_TTABLE) {
			r_lua_getmetatable(ls, -1);
			return 1;
		}
		return 0;
	}

	//int getgc_impl(uintptr_t rL)
	//{
	//	GCObject* Obj = *(GCObject**)((*(DWORD*)(rL + 16) - (rL + 16)) + 32);
	//	r_lua_createtable(rL, 0, 0);

	//	for (int i = 0; Obj != nullptr; i++)
	//	{
	//		if ((*(BYTE*)((DWORD)Obj + 4)) == 6 || (*(BYTE*)((DWORD)Obj + 4)) == 7 || (*(BYTE*)((DWORD)Obj + 4)) == 8)
	//		{
	//			r_lua_pushnumber(rL, i);
	//			r_StkId& topstack = *(r_StkId*)(rL + 24);
	//			topstack->tt = (*(BYTE*)((DWORD)Obj + 4));
	//			topstack->value.p = (r_TValue*)(void*)(DWORD)Obj;
	//			*(uintptr_t*)((rL)+24) += sizeof(r_TValue);
	//			r_lua_settable(rL, -3);
	//		}
	//		Obj = Obj->gch.next;
	//	}
	//	return 1;
	//}

	int isluau(DWORD rL)
	{
		r_lua_pushboolean(rL, true);
		return 1;
	}

	int isrbxactive(DWORD rL)
	{
		if (RobloxActive == GetForegroundWindow())
		{
			r_lua_pushboolean(rL, true);
		}
		else
		{
			r_lua_pushboolean(rL, false);
		}
		return 1;
	}

	int isreadonly_impl(DWORD rL) {
		rluaL_checktype(rL, 1, RLUA_TTABLE);
		const auto Table = r_lua_topointer(rL, -1);
		r_lua_pushboolean(rL, *(BYTE*)(Table + 8));
		return 1;
		r_lua_settop(rL, 0);
		return 0;
	}

	int setreadonly_impl(DWORD ls) {
		r_lua_setreadonly(ls, 1, r_lua_toboolean(ls, 2));
		r_lua_settop(ls, 0);
		return 1;
	}

	int getObjects_impl(DWORD ls) {
		const char* str = r_lua_tolstring(ls, -1, 0);
		r_lua_settop(ls, 0);
		r_lua_getfield(ls, -10002, "game");
		r_lua_getfield(ls, -1, "GetService");
		r_lua_pushvalue(ls, -2);
		r_lua_pushstring(ls, "InsertService");
		r_lua_pcall(ls, 2, 1, 0);
		r_lua_getfield(ls, -1, "LoadLocalAsset");
		r_lua_pushvalue(ls, -2);
		r_lua_pushstring(ls, str);
		r_lua_pcall(ls, 2, 1, 0);
		r_lua_createtable(ls, 0, 0);
		r_lua_pushvalue(ls, -2);
		r_lua_rawseti(ls, -2, 1);
		return 1;
	}


	void load_script(const DWORD RS, lua_State* L, std::string Script) {

		Script = "script = Instance.new(\"LocalScript\")" + Script;
		const auto bytecode = pandatranspiler::script_to_bytecode(lua_open(), Script, noobs);

		lua_getglobal(L, "string");
		lua_getfield(L, -1, "dump");
		if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@Script")) {
			const std::string err = lua_tostring(L, -1);
			throw std::exception(err.c_str());
		}
		auto ret = r_lua_deserialize(RS, "PandaBytecode", bytecode.c_str(), bytecode.size(), 0);
		lua_pcall(L, 0, 0, 0);
	}

	int loadstring(const DWORD ls) {
		const std::string script = rluaL_checklstring(ls, 1, nullptr);
		if (script.empty()) {
			r_lua_pushnil(ls);
			return 1;
		}
		try
		{
			load_script(ls, L, script);
			return 1;
		}
		catch (const std::exception& ex)
		{
			r_lua_pushnil(ls);
			//r_lua_print(3, ex.what());
		}
		return 1;
	}

	int mouse1click_impl(DWORD ls) {
		INPUT Inputs[1] = { 0 };
		Inputs->type = INPUT_MOUSE;
		memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
		SendInput(1, Inputs, sizeof(INPUT));
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int mouse1press_impl(DWORD ls) {
		INPUT Inputs[1] = { 0 };
		Inputs->type = INPUT_MOUSE;
		memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int mouse1release_impl(DWORD ls) {
		INPUT Inputs[1] = { 0 };
		Inputs->type = INPUT_MOUSE;
		memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int mouse2click_impl(DWORD ls) {
		INPUT Inputs[1] = { 0 };
		Inputs->type = INPUT_MOUSE;
		memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
		SendInput(1, Inputs, sizeof(INPUT));
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int mouse2press_impl(DWORD ls) {
		INPUT Inputs[1] = { 0 };
		Inputs->type = INPUT_MOUSE;
		memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	bool checkinstance(DWORD rL, int Index)
	{
		r_lua_getglobal(rL, "typeof");
		r_lua_pushvalue(rL, Index);
		r_lua_pcall(rL, 1, 1, 0);
		const auto check = rlua_tostring(rL, -1);
		r_lua_pop(rL, 1);
		return !strcmp(check, "Instance");
	}

	int fireclickdetector(DWORD rL)
	{
		if (!isuserdata(rL, 1) || !checkinstance(rL, 1))
			return rluaL_error(rL, "expected userdata <ClickDetector>");

		if (!r_lua_isnoneornil(rL, 2) && !IsNumber(2))
			return rluaL_error(2, "variant <none, number> expected");

		int CDectector = *(DWORD*)r_lua_touserdata(rL, 1);
		float Dist = 0.0;
		if (r_lua_gettop(rL) > 1)
			Dist = (float)r_lua_tonumber(rL, 2, 0);
		r_lua_getfield(rL, -10002, "game");
		r_lua_getfield(rL, -1, "GetService");
		r_lua_pushvalue(rL, -2);
		r_lua_pushstring(rL, "Players");
		r_lua_pcall(rL, 2, 1, 0);
		r_lua_getfield(rL, -1, "LocalPlayer");
		DWORD Plr = *(DWORD*)r_lua_touserdata(rL, -1);

		((void(fireclickdetector_CCV*)(DWORD, float, DWORD))FnFire)(CDectector, Dist, Plr);
		return 0;
	}

	int mouse2release_impl(DWORD ls) {
		INPUT Inputs[1] = { 0 };
		Inputs->type = INPUT_MOUSE;
		memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int HttpGet_impl(DWORD rL) {
		std::string URL = r_lua_tolstring(rL, 1, NULL);
		std::string Data = GianTools::ReadWebsite(URL.c_str());
		r_lua_pushstring(rL, Data.c_str());
		return 1;
	}

	int protectedfunc(DWORD ls)
	{
		rluaL_error(ls, "Function disabled.");
		return 0;
	}

	int mousemoverel_impl(DWORD ls) {
		DWORD x = r_lua_tonumber(ls, -1, 0);
		DWORD y = r_lua_tonumber(ls, -2, 0);

		mouse_event(MOUSEEVENTF_MOVE, x, y, NULL, NULL);

		return 1;
	}

	int keypress_impl(DWORD ls) {
		DWORD keycode = r_lua_tonumber(ls, -1, 0);
		keybd_event(0, (BYTE)MapVirtualKeyA(keycode, MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE, 0);
		return 0;
	}

	int mousemoveabs_impl(DWORD ls) {
		DWORD x = r_lua_tonumber(ls, -1, 0);
		DWORD y = r_lua_tonumber(ls, -2, 0);

		HWND activewindow = GetForegroundWindow();
		DWORD x2 = GetSystemMetrics(SM_CXSCREEN);
		DWORD y2 = GetSystemMetrics(SM_CYSCREEN);

		RECT s;
		GetClientRect(activewindow, &s);
		POINT hit = { s.left, s.top };
		ClientToScreen(activewindow, &hit);

		x2 = x2 - 1;
		y2 = y2 - 1;
		x = (0xFFFF / x2) * (x + hit.x);
		y = (0xFFFF / y2) * (y + hit.y);

		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, NULL, NULL);

		return 0;
	}

	int getfflag(DWORD rL)
	{
		r_lua_getglobal(rL, "GlobalSettings");
		r_lua_getfield(rL, -1, "GetFFlag");
		r_lua_pushstring(rL, r_lua_tolstring(rL, -1, 0));
		r_lua_pcall(rL, 1, 1, 0);
		bool fflag_result = r_lua_toboolean(rL, -1);
		r_lua_pushboolean(rL, fflag_result);
		return 1;
	}

	int checkcaller_impl(DWORD ls) {
		r_lua_pushboolean(ls, TRUE);
		return 1;
	}

	int getnamecallmethod_impl(DWORD rL) {
		r_TValue* topstack = *(r_TValue**)(rL + 24);
		topstack->tt = 5;
		topstack->value.p = (r_TValue*)(*(DWORD*)(rL + 104));
		*(DWORD*)((rL)+12) += sizeof(r_TValue);
		return 1;
	}
