#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>

using rbx_getscheduler_t = std::uintptr_t(__cdecl*)();
extern const rbx_getscheduler_t rbx_getscheduler;

using rbx_output_t = void(__fastcall*)(std::int16_t output_type, const char* str);
extern const rbx_output_t rbx_output;

using rbx_getstate_t = std::uintptr_t(__thiscall*)(std::uintptr_t SC, int* state_type);
extern const rbx_getstate_t rbx_getstate;

extern void rbx_spawn(std::uintptr_t rl);
extern void rbx_deserialize(std::uintptr_t rl, const char* chunk_name, const char* bytecode, int byte_len);
extern void rbx_decrement_top(std::uintptr_t rl, std::int32_t amount);
extern void rbx_setidentity(std::uintptr_t rl, std::int8_t identity);

namespace offsets
{
	namespace scheduler
	{
		constexpr std::uintptr_t jobs_start = 0x12C;
		constexpr std::uintptr_t jobs_end = 0x130;
	}

	namespace job
	{
		constexpr std::uintptr_t name = 0x10;
	}

	namespace waiting_scripts_job
	{
		constexpr std::uintptr_t datamodel = 0x28;
		constexpr std::uintptr_t script_context = 0x130;
	}

	namespace identity
	{
		constexpr std::uintptr_t extra_space = 0x6C;
		constexpr std::uintptr_t identity = 0x18;
	}

	namespace luastate
	{
		constexpr std::uintptr_t top = 0x18;
	}
}