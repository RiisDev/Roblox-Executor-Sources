#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include "retcheck.h"


extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lobject.h"
#include "Lua/lstate.h"
#include "Lua/lfunc.h"
#include "Lua/lopcodes.h"
}

static std::uintptr_t r_L = 0;

#define R_LUA_TNIL 0
#define R_LUA_TLIGHTUSERDATA 4
#define R_LUA_TNUMBER 2
#define R_LUA_TBOOLEAN 1
#define R_LUA_TSTRING 5
#define R_LUA_TTHREAD 9
#define R_LUA_TFUNCTION 6
#define R_LUA_TTABLE 8
#define R_LUA_TUSERDATA 7
#define R_LUA_VECTOR 3
#define R_LUA_TPROTO 10

#define RobloxBase 0x400000
#define ModuleBase reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr))

static std::uintptr_t aslr(std::uintptr_t addr)
{
	return addr - RobloxBase + ModuleBase;
}

namespace Addresses
{
	static auto luaF_newLClosure = aslr(0x154D100);
	static auto luaF_newproto = aslr(0x154D170);
	static auto luaM_realloc_ = aslr(0x154FD90);
	static auto luaS_newlstr = aslr(0x154D4A0);
	static auto lua_newthread = Retcheck::unprotect((byte*)(aslr(0x1534290)));
	static auto spawn = aslr(0x7af260);
}

namespace RBX
{
	struct Instance
	{
		std::uintptr_t obj;
		int ext;
	};

	typedef union
	{
		void* gc;
		void* p;
		double n;
		int b;
	} r_Value;

	struct r_TValue
	{
		Value value;
		int obj;
		int tt;
	};

	using luaM_realloc_Fn = void* (__cdecl*)(std::uintptr_t rL, size_t alloc);
	using luaS_newlstrFn = TString * (__stdcall*)(std::uintptr_t rL, const char* src, std::size_t size);
	using r_spawnFn = std::uintptr_t(__cdecl*)(std::uintptr_t rL);
	using newthreadFn = std::uintptr_t(__cdecl*)(std::uintptr_t rL);
	using luaFNewProtoFn = std::uintptr_t(__cdecl*)(std::uintptr_t rL);
	using luaFNewLClosureFn = void* (__cdecl*)(std::uintptr_t rL, int size, int fn, int protoAlloc);

	static auto r_lua_newthread = reinterpret_cast<newthreadFn>(Addresses::lua_newthread);
	static auto r_luaM_realloc_ = reinterpret_cast<luaM_realloc_Fn>(Addresses::luaM_realloc_);
	static auto r_luaF_newLClosure = reinterpret_cast<luaFNewLClosureFn>(Addresses::luaF_newLClosure);
	static auto r_luaF_newProto = reinterpret_cast<luaFNewProtoFn>(Addresses::luaF_newproto);
	static auto spawn = reinterpret_cast<r_spawnFn>(Addresses::spawn);
	static auto luaS_newlstr = reinterpret_cast<luaS_newlstrFn>(Addresses::luaS_newlstr);

	namespace Offsets
	{
		// luastate structs & offsets
		static std::uintptr_t l_gt = 64;
		static std::uintptr_t l_top = 16;
		static std::uintptr_t l_base = 20;
	}

	struct RState
	{
		RState(std::uintptr_t long_rL)
		{
			long_rL = r_L ? long_rL : r_L;
		}
		static std::uintptr_t l_gt;
		static std::uintptr_t l_top;
		static std::uintptr_t l_base;
	};
}

namespace offsets
{
	//first offsets of deserializer start (initializer offsets)
	static std::uintptr_t source = 8;
	static std::uintptr_t maxstacksize = 83;
	static std::uintptr_t params = 80;
	static std::uintptr_t nups = 82;
	static std::uintptr_t vararg = 81;

	//conversion offsets
	static std::uintptr_t sizecode = 68;
	static std::uintptr_t code = 12;
	static std::uintptr_t sizep = 60;
	static std::uintptr_t p = 32;
	static std::uintptr_t sizek = 76;
	static std::uint16_t k_k = 20;
	static std::uintptr_t sizelineinfo = 68;
	static std::uintptr_t lineinfo_notcomp = 24;

	
	static std::uintptr_t sizelineinfo_comp = 72;
	static std::uintptr_t lineinfo = 16;
	static std::uintptr_t function_name = 40;
	//bitwise operator offset
	static std::uintptr_t lua_number_key = aslr(0x29B7830);
}


static double r_xor_number(double* Number) {
	int rxor = offsets::lua_number_key;

	PVOID OldNumber = Number;
	double NewNumber;
	__asm {
		mov edx, OldNumber
		mov eax, rxor
		movsd xmm1, qword ptr[edx]
		xorpd xmm1, [eax]
		movsd NewNumber, xmm1
	}
	return NewNumber;
}
