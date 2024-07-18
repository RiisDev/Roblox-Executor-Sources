#include <Windows.h>
#include <string>
#include <stdint.h>

#define ASLR(Address) (Address - 0x400000 + reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr)))

extern "C"
{
#include "Lua\lobject.h"
}

namespace RBX
{
	using TValue = TValue;

	constexpr int R_LUA_TNONE = -1;
	constexpr int R_LUA_TNIL = 0;
	constexpr int R_LUA_TLIGHTUSERDATA = 1;
	constexpr int R_LUA_TNUMBER = 2;
	constexpr int R_LUA_TBOOLEAN = 3;
	constexpr int R_LUA_TSTRING = 4;
	constexpr int R_LUA_TTHREAD = 5;
	constexpr int R_LUA_TFUNCTION = 6;
	constexpr int R_LUA_TTABLE = 7;
	constexpr int R_LUA_TUSERDATA = 8;
	constexpr int R_LUA_TPROTO = 9;
	constexpr int R_LUA_TUPVAL = 10;

	using SpawnFn = int(__cdecl*)(int);
	extern SpawnFn Spawn = reinterpret_cast<SpawnFn>(ASLR(0x811550));

	/*

	"Spawn function requires 1 argument"
	.text:0080CF90                 push    ebp <----- This is the address
.text:0080CF91                 mov     ebp, esp
.text:0080CF93                 push    0FFFFFFFFh

	*/

	using DeserializeFn = int(__cdecl*)(uintptr_t, std::string&, const char*, unsigned long);
	extern DeserializeFn Deserialize = reinterpret_cast<DeserializeFn>(ASLR(0x67F710));

	/*
	"oldResult, moduleRef  = ...\nnewResult"
	Press F5
	v11 = sub_68D4D0(v9, v10, (int)&loc_1CAD4DE + 4, 1);

	Double click on the sub address

	int __cdecl sub_68D4D0(int a1, int a2, int a3, int a4)
{
  int v5; // [esp+0h] [ebp-1Ch]
  int *v6; // [esp+Ch] [ebp-10h]
  int v7; // [esp+18h] [ebp-4h]

  v6 = &v5;
  v7 = 0;
  return sub_68CA90(a1, a2 + 24, a3, a4);
}

The return sub is the Deserializer

	*/

	using SandboxThreadFn = void(__cdecl*)(int, int, int);
	extern SandboxThreadFn SandboxThread = reinterpret_cast<SandboxThreadFn>(ASLR(0x811340));
}