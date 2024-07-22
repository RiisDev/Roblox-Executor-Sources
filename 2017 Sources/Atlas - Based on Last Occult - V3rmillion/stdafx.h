#pragma once
#pragma comment(lib, "Lua/lua51.lib")

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <iostream>
#include <map>
#include "util.h"
extern "C" {
#include "lua.h"
#include "lauxlib.h"
	// lua internals 
#include "lualib.h"
#include "lopcodes.h"
#include "lstate.h"
#include "lobject.h"
#include "lfunc.h"
#include "lundump.h"
}
#include "UI.h"

VOID Main();

#define DefineImportDLL(DLLName, Export, VarName, Type, Return, ...) \
	typedef Return (Type *tdef_##VarName)(__VA_ARGS__);				 \
	static tdef_##VarName VarName = (tdef_##VarName)GetProcAddress(GetModuleHandle(DLLName), Export);
