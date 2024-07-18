#pragma once
#include "xorstring.h"
#include "Base64Encryption.h"
#include "Wrapper.h"
#include "Stuffs.h"
#include "Tools.h"
#include "Lua/lua.h"

HANDLE clip;
using rlua_CFunction = int(*)(DWORD);
auto rlua_tocfunction = [](DWORD rState, int idx) -> rlua_CFunction {
	TValue* o = (TValue*)rlua_index2adr(rState, idx);
	return (!iscfunction(o)) ? NULL : cast(rlua_CFunction, clvalue(o)->c.f);
};

static int CustomMTIndex(lua_State* L) {
	int reference = lua_tointeger(L, lua_upvalueindex(1));
	int State = GetThreadPointer(L);
	const char* key = lua_tolstring(L, -1, NULL);
	if (lua_type(L, -2) == LUA_TTABLE) {
		rlua_rawgeti(State, LUA_REGISTRYINDEX, reference);
		rlua_getfield(State, -1, key);
		wrap(L, State, -1);
		rlua_settop(State, -3);
		return 1;
	}
	lua_pushnil(L);
	return 1;
}

static int CustomMTNewIndex(lua_State* L) {
	int reference = lua_tointeger(L, lua_upvalueindex(1));
	int State = GetThreadPointer(L);
	const char* key = lua_tolstring(L, -2, NULL);
	rlua_rawgeti(State, LUA_REGISTRYINDEX, reference);
	unwrap(L, State, -1);
	rlua_setfield(State, -2, key);
	rlua_settop(State, -2);
	return 1;
}

static int SpawnFunc(lua_State *L) {
	int NewThread = rlua_newthread(luaState);
	unwrap(L, NewThread, 1);
	Spawn(NewThread);
	rlua_settop(NewThread, 0);
	return 1;
}

static int MoveMouse(lua_State *L) {
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);

	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(INPUT));
	return 1;
}

static int KeyPress(lua_State* L) {
	int KeyCode = lua_tointeger(L, -1);
	INPUT input;
	input.type = INPUT_KEYBOARD;
	memset(&input.ki, 0, sizeof(KEYBDINPUT));
	input.ki.wScan = MapVirtualKey(KeyCode, MAPVK_VK_TO_VSC);
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	SendInput(1, &input, sizeof(input));
	return 1;
}

TValue* r_gettop(DWORD rL) {
	return *(TValue**)(rL + 16);
}

static int is_panda(lua_State* Ls) { //Jackass Panda ( Synixxal credit )
	lua_pushboolean(Ls, true);
	return 1;
}


bool checkactivewindow(lua_State* L)
{
	HWND foreground = GetForegroundWindow();
	if (foreground)
	{
		char window_title[256];
		GetWindowText(foreground, window_title, 256);

		std::string str(window_title);
		if (str == "Roblox")
		{
			lua_pushboolean(L, true);
		}
		else
		{
			lua_pushboolean(L, false);
		}
	}
	return 1;
}

std::map<const char*, lua_CFunction> CClosures {

	std::make_pair("setclipboard", [](lua_State *L) {
     	Scanner::ClipBoard::Set(lua_tostring(L, -1));
		return 1;
	}),
	std::make_pair("toclipboard", [](lua_State *L) {
		Scanner::ClipBoard::Set(lua_tostring(L, -1));
		return 1;
	}),
	std::make_pair("getgenv", [](lua_State* L) {
		lua_pushvalue(L, LUA_GLOBALSINDEX);
		return 1;
	}),
	std::make_pair("getrenv", [](lua_State* L) {
		int State = GetThreadPointer(L);
		wrap(L, State, LUA_GLOBALSINDEX);
		return 1;
	}),
	std::make_pair("getreg", [](lua_State* L) {
		int State = GetThreadPointer(L);
		wrap(L, State, LUA_REGISTRYINDEX);
		return 1;
	}),
	std::make_pair("setreadonly", [](lua_State *L) {
		if (lua_gettop(L) < 0) return luaL_error(L, "setreadonly require 2 arguments");
		int State = GetThreadPointer(L);
		int TrueORFalse = lua_toboolean(L, 2);
		if (lua_type(L, 1) == LUA_TTABLE) {
			lua_getfield(L, 1, "__key");
			if (lua_type(L, -1) > LUA_TNIL) {
				rlua_rawgeti(State, LUA_REGISTRYINDEX, lua_tointeger(L, -1));
				if (TrueORFalse) rlua_setreadonly(State, -1, 1); else rlua_setreadonly(State, -1, 0);
				r_lua_pop(State, 1);
			}
			lua_pop(L, 1);
			return 1;
		}
		else {
			unwrap(L, State, 1);
			if (TrueORFalse) rlua_setreadonly(State, -1, 1); else rlua_setreadonly(State, -1, 0);
			return 1;
		}
		return 1;
	}),
	std::make_pair("isreadonly", [](lua_State *L) {
		if (lua_gettop(L) < 0) return luaL_error(L, "isreadonly requires 1 argument");
		int State = GetThreadPointer(L);
		if (lua_type(L, 1) == LUA_TTABLE) {
			lua_getfield(L, 1, "__key");
			if (lua_type(L, -1) > LUA_TNIL) {
				rlua_rawgeti(State, LUA_REGISTRYINDEX, lua_tointeger(L, -1));
				DWORD Table = (DWORD)rlua_index2adr(State, -1);
				lua_pushboolean(L, (int)*(unsigned char*)(*(DWORD*)Table + 8));
				r_lua_pop(State, 1);
			}
			else {
				lua_pop(L, 1);
			}
			return 1;
		}
		unwrap(L, State, 1);
		DWORD Table = (DWORD)rlua_index2adr(State, -1);
		lua_pushboolean(L, (int)*(unsigned char*)(*(DWORD*)Table + 8));
		return 1;
	}),
		// Test

	std::make_pair("writefile", [](lua_State* L) {
		const char* filename = lua_tostring(L, -1);
		const char* content = lua_tostring(L, -2);

		char path[MAX_PATH];
		get_file("PandaModule.vmp.dll", "", path, MAX_PATH);

		std::string backuss;
		backuss += path;
		backuss += "Workspace\\";
		backuss += content;
		std::ofstream o(backuss.c_str());

		o << filename;
		return 1;
	}),

		// Test End
	std::make_pair("getrawmetatable", [](lua_State* L) {
		int State = GetThreadPointer(L);
		if (lua_type(L, -1) != LUA_TUSERDATA) {
			luaL_error(L, "getrawmetatable only accepts a userdata");
			return 0;
		}
		unwrap(L, State, 1);
		if (!rlua_getmetatable(State, -1)) {
			lua_pushnil(L);
			return 1;
		}
		int ReferenceKey = rluaL_ref(State, LUA_REGISTRYINDEX);
		rlua_settop(State, -2);
		lua_createtable(L, 0, 0);
		lua_pushinteger(L, ReferenceKey);
		lua_setfield(L, -2, "__key");
		lua_createtable(L, 0, 0);
		lua_pushinteger(L, ReferenceKey);
		lua_pushcclosure(L, CustomMTIndex, 1);
		lua_setfield(L, -2, "__index");
		lua_pushinteger(L, ReferenceKey);
		lua_pushcclosure(L, CustomMTNewIndex, 1);
		lua_setfield(L, -2, "__newindex");
		lua_setmetatable(L, -2);
		return 1;
	}),
	std::make_pair("setrawmetatable", [](lua_State* L) {
		int State = GetThreadPointer(L);
		if (r_lua_gettop(State) < 2) {
			rlua_pushboolean(State, 0);
			rlua_pushstring(State, "2 or more arguments expected");
			return 1;
		}

		TValue* top = r_gettop(State);
		TValue* obj1 = top - 1;
		TValue* obj0 = top - 2;


		if (!(obj0->tt == RLUA_TTABLE || obj0->tt == RLUA_TUSERDATA) || !(obj1->tt == RLUA_TNIL || obj1->tt == RLUA_TTABLE)) {
			rlua_pushboolean(State, 0);
			rlua_pushstring(State, "bad argument types");
			return 1;
		}

		rlua_setmetatable(State, 1);
		rlua_pushboolean(State, 1);
		return 1;
	}),
	std::make_pair("checkcaller", [](lua_State* L) {
		int State = GetThreadPointer(L);
		rlua_pushboolean(State, (*reinterpret_cast<int*>(State - 108) & 24) == 7);
		return 1;
	}),
	std::make_pair("info", [](lua_State *L) {
		for (int i = lua_gettop(L); i != 0; i--) {
			writerf(1, lua_tostring(L, -i));
		}
		return 1;
	}),
	std::make_pair("getclipboard", [](lua_State *L) {
		if (OpenClipboard(NULL)) {
			clip = GetClipboardData(CF_TEXT);
			CloseClipboard();
		}
		string text = (char*)clip;
		if (text != "") {
			lua_pushstring(L, text.c_str());
		}
		return 1;
	}),
};
int LoadLibraryImpl(lua_State* L) {
	std::string RbxGuiLink = "https://raw.githubusercontent.com/Roblox/Core-Scripts/master/CoreScriptsRoot/Libraries/RbxGui.lua";
	std::string RbxStamperLink = "https://raw.githubusercontent.com/Roblox/Core-Scripts/master/CoreScriptsRoot/Libraries/RbxStamper.lua";
	std::string RbxUtilityLink = "https://raw.githubusercontent.com/Roblox/Core-Scripts/master/CoreScriptsRoot/Libraries/RbxUtility.lua";
	std::string key = lua_tostring(L, -1);
	if (key == "RbxGui") {
		luaL_dostring(L, DownloadURL(RbxGuiLink).c_str());
	}
	else if (key == "RbxStamper") {
		luaL_dostring(L, DownloadURL(RbxStamperLink).c_str());
	}
	else if (key == "RbxUtility") {
		luaL_dostring(L, DownloadURL(RbxUtilityLink).c_str());
	}
	return 1;
}

// These are the Function Unattended Added by SkieHacker


static int _kick(lua_State* L) {
	const char* arg1 = lua_tostring(L, -1);
	const int result = MessageBoxA(NULL, "The script you are running is expecting a crash, are you sure you want to continue?", "Panda-Exploit Kick Notification", MB_YESNO);

	switch (result)
	{
	case IDYES:
		ExecuteScript("game:GetService('Players').LocalPlayer:kick");
		return 1;
		break;
	case IDNO:
		return 1;
		break;
	}
}

// Ends Here

void OpenCustomFunctions(lua_State *L) {
	for (auto closure : CClosures)
		lua_register(L, closure.first, closure.second);

	lua_State* Thread = lua_newthread(L);

	lua_register(Thread, "LoadLibrary", LoadLibraryImpl);
	lua_register(Thread, "HttpGet", DownloadString);
	lua_register(Thread, "GetObjects", GetObjects);
	lua_register(Thread, "spawn", SpawnFunc);
	lua_register(Thread, "Spawn", SpawnFunc);
	lua_register(Vanilla, "_kick", _kick); // Kick Function Implementation
	lua_register(Thread, "mousemoverel", MoveMouse);
	lua_register(Thread, "keypress", KeyPress);
	lua_register(Vanilla, "PANDA_LOADED", is_panda); //Panda Checkcaller ( So it can Support by Panda Secured Module )
}