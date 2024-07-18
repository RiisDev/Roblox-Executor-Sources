#pragma once
#pragma comment(lib, "Lua/lua51.lib")

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <iostream>
#include <map>
#include "Lua/lua.hpp"
#include "Lua/luadefs.h"
#include "Lua/luaop.h"
#include "UI.h"

VOID Main();

#define DefineImportDLL(DLLName, Export, VarName, Type, Return, ...) \
	typedef Return (Type *tdef_##VarName)(__VA_ARGS__);				 \
	static tdef_##VarName VarName = (tdef_##VarName)GetProcAddress(GetModuleHandle(DLLName), Export);
