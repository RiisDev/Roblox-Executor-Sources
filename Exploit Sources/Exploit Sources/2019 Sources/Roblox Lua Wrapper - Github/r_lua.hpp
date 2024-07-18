#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "retcheck_brandon.h"
#include "retcheck.h"


#define ASLR(x)(unprotect(x - 0x400000 + (int)GetModuleHandleA(0)))
#define aslr_2(x)(x - 0x400000 + (int)GetModuleHandleA(0))

#define rlua_pop(L,n)            rlua_settop(L, -(n)-1)

#define rlua_newtable(L)         rlua_createtable(L, 0, 0)
typedef void(__cdecl *luaL_getmetafield_Def)(uintptr_t, int, const char*);
const auto rluaL_getmetafield = reinterpret_cast<luaL_getmetafield_Def>(ASLR(0x007EA860));
typedef void(__fastcall *lua_getfield_Def)(uintptr_t, int, const char*);
const auto rlua_getfield = reinterpret_cast<lua_getfield_Def>(ASLR(0x007EFC80));
typedef void(__stdcall *lua_setfield_Def)(uintptr_t, int, const char*);
const auto rlua_setfield = reinterpret_cast<lua_setfield_Def>(ASLR(0x007F1A90));
typedef void(__cdecl *lua_settable_Def)(uintptr_t, int);
const auto rlua_settable = reinterpret_cast<lua_settable_Def>(ASLR(0x007F1DF0));
typedef void(__stdcall *lua_settop_Def)(uintptr_t rL, int);
const auto rlua_settop = reinterpret_cast<lua_settop_Def>(ASLR(0x007F1E80));
typedef void(__stdcall* rpushnumber)(int, double);
rpushnumber r_pushnumber = (rpushnumber)ASLR(0x007F0FD0);
typedef void(__cdecl *lua_pushnil_Def)(uintptr_t);
const auto rlua_pushnil = reinterpret_cast<lua_pushnil_Def>(ASLR(0x007F0F60));
typedef void(__cdecl *lua_pushstring_Def)(uintptr_t, const char*);
const auto rlua_pushstring = reinterpret_cast<lua_pushstring_Def>(ASLR(0x007F1060));
typedef void(__cdecl *lua_pushthread_Def)(uintptr_t);
const auto rlua_pushthread = reinterpret_cast<lua_pushthread_Def>(ASLR(0x007F10A0));
typedef void(__stdcall *lua_pushvalue_Def)(uintptr_t rL, int);
const auto rlua_pushvalue = reinterpret_cast<lua_pushvalue_Def>(ASLR(0x007F1130));
typedef int(__cdecl *lua_tothread_Def)(uintptr_t rL, int);
const auto rlua_tothread = reinterpret_cast<lua_tothread_Def>(ASLR(0x007F24C0));
typedef bool(__cdecl *lua_toboolean_Def)(uintptr_t, bool);
const auto rlua_toboolean = reinterpret_cast<lua_toboolean_Def>(ASLR(0x007F2000));
typedef double(__cdecl *lua_tonumber_Def)(uintptr_t, int);
const auto rlua_tonumber = reinterpret_cast<lua_tonumber_Def>(ASLR(0x007F23B0));
typedef int(__cdecl *lua_type_Def)(uintptr_t, int);
const auto rlua_type = reinterpret_cast<lua_type_Def>(ASLR(0x007F2510));
typedef int(__cdecl *lua_pcall_Def)(uintptr_t, int, int, int);
const auto rlua_pcall = reinterpret_cast<lua_pcall_Def>(ASLR(0x007F1860));
typedef void(__fastcall *lua_pushcclosure_Def)(DWORD, DWORD, DWORD);
const auto rlua_pushcclosure = reinterpret_cast<lua_pushcclosure_Def>(ASLR(0x007F20C0));
typedef const char*(__stdcall *lua_tolstring_Def)(uintptr_t, size_t osize, size_t ksize);
const auto rlua_tolstring = reinterpret_cast<lua_tolstring_Def>(ASLR(0x007F2200));
typedef int(__cdecl *lua_next_Def)(uintptr_t rL, int);
const auto rlua_next = reinterpret_cast<lua_next_Def>(ASLR(0x007F0900));
typedef void(__cdecl *lua_createtable_Def)(uintptr_t, int, int);
const auto rlua_createtable = reinterpret_cast<lua_createtable_Def>(ASLR(0x007EF940));
typedef int(__cdecl *luaL_ref_def)(uintptr_t, int);
const auto rluaL_ref = reinterpret_cast<luaL_ref_def>(Ret::unprotect<DWORD>((BYTE*)aslr_2(0x007EACB0)));
typedef bool(__cdecl *lua_pushboolean_def)(uintptr_t, bool);
const auto rlua_pushboolean = reinterpret_cast<lua_pushboolean_def>(ASLR(0x007F0B50));
typedef void(__cdecl *lua_rawgeti_def)(uintptr_t, int, int);
const auto rlua_rawgeti = reinterpret_cast<lua_rawgeti_def>(ASLR(0x007F13F0));
typedef int(__cdecl *lua_newthread_def)(uintptr_t);
const auto rlua_newthread = reinterpret_cast<lua_newthread_def>(ASLR(0x007F0750));
typedef int(__cdecl *gettop_def)(uintptr_t);
const auto rlua_gettop = reinterpret_cast<gettop_def>(ASLR(0x007F0190));
typedef int(__fastcall *rlua_getmetatable_def)(uintptr_t, int);
const auto rlua_getmetatable = reinterpret_cast<rlua_getmetatable_def>(ASLR(0x007EFF60));
