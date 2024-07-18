#pragma once
#include "Definitions.h"
#include "Memory.h"

inline long offset(long Address) {
	return (Address - 0x400000 + (DWORD)GetModuleHandle(NULL));
}

#define loc_call offset(0x71F3B0) //done
#define loc_pushvalue offset(0x720E50) //done
#define loc_getfield offset(0x71FA10) //done
#define loc_pushstring offset(0x720D90) //done
#define loc_pushnumber offset(0x720D00) //done
#define loc_pushboolean offset(0x7208A0) //done
#define loc_pushnil offset(0x720C90) //done
#define loc_setfield offset(0x721710) //done
#define loc_settop offset(0x721AF0) //done
#define loc_Scriptcontext offset(0xFF9508) //done //wtf is this
#define loc_identityflag offset(0x58CA16) //done //there was no address here so i added one l0l
#define loc_type offset(0x722160) //done
#define loc_tonumber offset(0x71B950) //done
#define loc_tolstring offset(0x721D30) //done
#define loc_toboolean offset(0x721C70) //done
#define loc_touserdata offset(0x722130) //done
#define loc_next offset(0x720650) //done
#define loc_getmetatable offset(0x71FD00) //done
#define loc_rawgeti offset(0x712C90) //done
#define loc_rawseti offset(0x721240) //done
#define loc_ref offset(0x71B070) //done
#define loc_createtable offset(0x71F660) //done
#define loc_getmetafield offset(0x71ABD0) //done

DWORD GetScriptContext(DWORD aScriptContext) {
	DWORD SC = (DWORD)ScanA((char*)&aScriptContext);
	return SC;
}

DWORD GetRLState(DWORD ScriptContext) {
	return (ScriptContext + 56 * 1 + 164 - *(DWORD *)(ScriptContext + 56 * 1 + 164));
}

bool Scan() {
	ScriptContext = GetScriptContext(loc_Scriptcontext);
	if (ScriptContext == NULL)
		return false;

	rL = GetRLState(ScriptContext);
	if (rL == NULL)
		return false;

	return true;
}