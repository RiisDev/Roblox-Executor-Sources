#pragma once
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <WinInet.h>
#include <TlHelp32.h>

#pragma comment(lib, "wininet")
#pragma comment(lib, "user32")

typedef void*(__cdecl *_rluaM_realloc__Def)(uintptr_t, void*, size_t, size_t, size_t);
typedef void*(__stdcall *_r_luaS_newlstr__Def)(uintptr_t, const char*, size_t);
typedef int(__cdecl *_r_lua_newThread__Def)(uintptr_t rL);
typedef int(__cdecl *_spawn_Def)(uintptr_t rL);
