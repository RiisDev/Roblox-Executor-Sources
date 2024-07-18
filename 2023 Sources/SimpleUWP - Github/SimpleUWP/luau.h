#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

// NOTE: THIS DLL WAS MADE FOR LEARNING PURPOUSES SO I MADE IT UNDERSTANABLE
// LICENSED UNDER NOSKID LICENSE THAT MEANS IF YOU WILL USE THIS SOURCE IN YOUR EXXPLOIT YOUR FEDERAL PROTECTION WILL BE TURNED OFF AND YOU WILL BE IN JAIL

std::uintptr_t i_cum = 0; // base of addresses (0 = pro)
std::uintptr_t fax = (std::uintptr_t)GetModuleHandleA(NULL);
#define x(a) a - i_cum + fax

// oh no this was encrypted by FBI!!!
namespace addresses
{
	std::uintptr_t getscheduler = ((0xFB1 + 0xFB1 * 2) - (0xFB1 * 3)) + x(0x77C750);
	std::uintptr_t luavm_load = ((0xFB1 + 0xFB1 * 2) - (0xFB1 * 3)) + x(0x52B300);
	std::uintptr_t print = ((0xFB1 + 0xFB1 * 2) - (0xFB1 * 3)) + x(0xAF9490);
	std::uintptr_t task_spawn = ((0xFB1 + 0xFB1 * 2) - (0xFB1 * 3)) + x(0x469A60);
}

namespace offsets
{
	namespace smell_fart
	{
		int top = ((0xFB1 + 0xFB1 * 2) - (0xFB1 * 3)) + (0x8);
		int baes = ((0xFB1 + 0xFB1 * 2) - (0xFB1 * 3)) + (0xC);
		int cosmos = ((0xFB1 + 0xFB1 * 2) - (0xFB1 * 3)) + (0x48); // aka space (aka extra_space (aka roblox_extra_space (aka RobloxExtraSpace)))
	}

	namespace cosmos
	{
		int universe = ((0xFB1 + 0xFB1 * 2) - (0xFB1 * 3)) + (0x14); // WHERE FBI EXISTS!!!
		int universe_level = ((0xFB1 + 0xFB1 * 2) - (0xFB1 * 3)) + (0x18); // OUR LEVEL OF SECURITY
	}
}

#define LUA_SUSSYINDEX -10000
#define LUA_USELESSINDEX -10001
#define LUA_GOODINDEX -10002
#define LUA_GETUSEFULTHINGSFROMCLOSURES(i) (LUA_GOODINDEX - (i))

#define RobloxCool_CCV __cdecl
#define RobloxFastCum_CCV __fastcall

#define get_roblox_unprotected_global_or_not_global_function_from_enviroment(addr) reinterpret_cast<int(RobloxCool_CCV*)(std::uintptr_t L)>(addr)

// UNION FOR ROBOLOX TO GET CUMVALUES
typedef union
{
	std::uintptr_t gc;
	void* p;
	double n;
	int b;
	float v[2]; 
} RobloxCumValue;

// MAIN CUMMING STRUCTURE
typedef struct RobloxCumTypeValue
{
	RobloxCumValue value;
	int extra[1];
	int tt;
} RobloxCumTypeValue;

// DEFINE WHAT IS CUMFUNCTION
typedef int (*lua_CumFunction)(std::uintptr_t L);

// DEFINE WHY CUMFUNCTION EXISTS
struct CumRegistry
{
	const char* name;
	lua_CumFunction func;
};
typedef struct CumRegistry CumRegistry;

// DEFINE ROBLOX HJACKED FUNCTIONS FROM SYNAPSEX CLMV
namespace luau
{
	auto getscheduler = reinterpret_cast<std::uintptr_t(RobloxCool_CCV*)()>(addresses::getscheduler);
	auto load = reinterpret_cast<int(RobloxFastCum_CCV*)(std::uintptr_t L, std::string * bytecode, const char* chunk, int env)>(addresses::luavm_load);
	auto print = reinterpret_cast<void(RobloxCool_CCV*)(int type, const char* fmt, ...)>(addresses::print);

	namespace task
	{
		auto spawn = get_roblox_unprotected_global_or_not_global_function_from_enviroment(addresses::task_spawn);
	}
}

#define CUM_ENTRY 1