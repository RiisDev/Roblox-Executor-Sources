#pragma once
#include <Windows.h>

#include "memory.h"

#define ModuleBase (DWORD)GetModuleHandle(NULL)
#define IdaBase 0x400000
#define ScanAddress(x) (ModuleBase + x - IdaBase)

#define print printf

/* - Offsets for instances - */

#define Parent_off 0x34
#define LocalPlayer_off 0x154
#define ClassName_off 0x10
#define Name_off 0x28
#define Start_of_children_off 0x2C
#define End_of_children_off 4

/* - ROBLOX Lua C internal memory locations - */

#define ScriptContext_bytes_addr ScanAddress(0x1188B80)
#define rbx_lua_getfield_addr ScanAddress(0x50E310)
#define rbx_lua_settop_addr ScanAddress(0x510210)
//#define rbx_lua_settop_addr Memory::ScanForSignature("\x55\x8B\xEC\x8B\x4D\x0C\x8B\x55\x08\x85\xC9\x78", "xxxx?xx?xx?x")
#define rbx_lua_pushstring_addr ScanAddress(0x50F570)
#define rbx_lua_pushvalue_addr ScanAddress(0x50F630)
#define rbx_lua_pcall_addr ScanAddress(0x50F010)
#define rbx_lua_pushnumber_addr ScanAddress(0x50F4F0)
#define rbx_lua_setfield_addr ScanAddress(0x50FE80)
#define rbx_lua_newthread_addr ScanAddress(0x50ED10)
#define rbx_lua_toboolean_addr ScanAddress(0x510380)

/* - Global variable declaration - */

PDWORD ScriptContext;
INT rbx_L;
DWORD DataModel;
DWORD Players;
DWORD Workspace;
DWORD Lighting;
DWORD LocalPlayer;