#pragma once
#include <string>
#include <Windows.h>
#include "Retcheck.h"

using namespace std;

Retcheck ret;
/* luac library */

#define ScanAddress(Address) ((DWORD)GetModuleHandle(NULL) + Address - 0x400000)

#define SCRIPTCONTEXT 0x011AD1EC
#define VM_HACKFLAG 0x1566B98
#define DESERIALIZE_ADDRESS 0x404740
#define GETGLOBALSTATE_ADDRESS 0x64D550
#define SPAWN_ADDRESS 0x657EB0
#define NEWTHREAD_ADDRESS 0x492210

namespace cLua {
	typedef void(*clua_deserialize)(DWORD rL, string& code, const char *chunkname, unsigned int modkey);
	typedef DWORD(__thiscall *clua_getglobalstate)(DWORD, DWORD);
	typedef void(*clua_spawn)(DWORD);
	typedef DWORD(__cdecl *clua_newthread)(DWORD);

	clua_deserialize Deserialize = (clua_deserialize)ScanAddress(DESERIALIZE_ADDRESS);
	clua_getglobalstate GetGlobalState = (clua_getglobalstate)ScanAddress(GETGLOBALSTATE_ADDRESS);
	clua_spawn Spawn = (clua_spawn)ScanAddress(SPAWN_ADDRESS);
	clua_newthread rbx_lua_newthread = (clua_newthread)ret.unprotect<DWORD>((BYTE*)(ScanAddress(NEWTHREAD_ADDRESS)));
}