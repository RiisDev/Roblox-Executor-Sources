#pragma once
#include "Vitals.h"
#include <string>
#include <vector>
#include "Signatures.h"
#include "Core.h"
#include <signal.h>
extern "C" {
#include "Lua\lua.h"
#include "Lua\lua.hpp"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
}

#define log_addy_start 0xB838D
#define checkArguments(L, cond,numarg,extramsg)	\
		((void)((cond) || CLog(3, extramsg)))\

HANDLE crashLog;

using Wrap::rLS;
using Wrap::mLS;

typedef std::auto_ptr<std::istream>(__cdecl *gdd)(int, int, int, int); gdd getstringS = (rgc)x(0x58B970);

static int implementation_getObjects(lua_State *L) {
	getglobal(rLS, "game");
	int instance = checkInstance(rLS, -1, "ContentProvider");
	if (instance == 1) {
		
		
		stream(getstringS((int)&a, (int)&b, (int)&c, 0));
	}
	else
		exceptions::Error();
	return 0;
}

static int indexToDM(lua_State *L) {
	if (lua_type(L, 2) == LUA_TSTRING) {
		if (checkString(L, 2) == "GetObjects") {
			lua_pushcfunction(L, implementation_getObjects);
		}
	}

	lua_pushvalue(L, lua_upvalueindex(1));
	lua_pushvalue(L, 1);
	lua_pushvalue(L, 2);
	lua_pcall(L, 2, 1, 0);

	return TRUE;
};

static int InitializeImplements(lua_state* mainL) {
	luaL_openlibs(mLS);

	//more in core.hpp
	lua_register(mLS, "getreg", getreg);
	lua_register(mLS, "getgenv", getgenv);
	lua_register(mLS, "getrenv", getrenv);
	lua_register(mLS, "keypress", PressKey);
	lua_register(mLS, "decompile", Decompile); //scrapped - you will need to adjust the deserializer located in Utility.hpp
	lua_register(mLS, "keyrelease", ReleaseKey);
	lua_register(mLS, "loadstring", loadstringChunk);
	lua_register(mLS, "setreadonly", setReadOnly);
	lua_register(mLS, "toclipboard", CopyToClipboard);
	lua_register(mLS, "getclipboard", GetClipboard);
	lua_register(mLS, "mousemoverel", MouseMove);
	lua_register(mLS, "getnilinstances", getNilInstances); // also scrapped (Utility.hpp)
	lua_register(mLS, "getrawmetatable", getRawMetatable);
	lua_register(mLS, "setrawmetatable", setRawMetatable);
	lua_register(mLS, "MouseButton1Click", MouseButton1Click);
	lua_register(mLS, "MouseButton2Click", MouseButton2Click);
	lua_register(mLS, "MouseButton1Press", MouseButton1Press);
	lua_register(mLS, "MouseButton2Press", MouseButton2Press);
	lua_register(mLS, "MouseButton1Release", MouseButton1Release);
	lua_register(mLS, "MouseButton2Release", MouseButton2Release);

	lua_getglobal(mLS, "game");
	lua_pushvalue(mLS, -1);
	lua_setglobal(mLS, "game");
}