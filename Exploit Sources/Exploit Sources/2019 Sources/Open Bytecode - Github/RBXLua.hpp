#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <WinInet.h>
#include <sstream>
#include <TlHelp32.h>
#include <Psapi.h>
#include "RBX.hpp"
#include "Retcheck.hpp"

using namespace syn;

#pragma comment(lib, "wininet")
#pragma comment(lib, "psapi")

DWORD WINAPI __stdcall Entry(PVOID);

using getfieldFn = void(__cdecl*)(std::uintptr_t rL, int index, const char* key);
using pcallFn = int(__cdecl*)(std::uintptr_t rL, int err, int fake, int nresults);
using pushlstringFn = void(__cdecl*)(std::uintptr_t rL, const char* str, size_t size);
using deserializerFn = int(__cdecl*)(std::uintptr_t rL, const char* name, const char* bytes, size_t size);

std::uintptr_t base = reinterpret_cast<std::uintptr_t>(GetModuleHandle(0));

DWORD retcheck(DWORD address)
{
	return (retcheckBypass(address - 0x400000 + base));
}

DWORD format(DWORD address)
{
	return (address - 0x400000 + base);
}

std::uintptr_t ScriptContextVFtable = format(0x1A93A3C);


namespace RBX
{
	getfieldFn r_lua_getfield = reinterpret_cast<getfieldFn>(retcheck(0x764ab0));
	pcallFn r_lua_pcall = reinterpret_cast<pcallFn>(retcheck(0x765ab0));
	deserializerFn deserialize = reinterpret_cast<deserializerFn>(format(0x841170));
	pushlstringFn r_lua_pushlstring = reinterpret_cast<pushlstringFn>(retcheck(0x765F50));
}

void InitializeClasses(DWORD ScriptContext)
{
	std::cout << "Initialzing Roblox Library classes " << std::endl;
	syn::Instance* RInstance;
	RInstance->instance_ptr = ScriptContext;
	DWORD DataModel = RInstance->GetParent();
	std::cout << "DataModel; " << DataModel << std::endl;

}