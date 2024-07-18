#pragma once

#include <string>
#include <Windows.h>

#include "Scheduler.hpp"

namespace Offsets { // Version: 2.590.678.0
	// Function offsets:
	constexpr uintptr_t get_scheduler = 0x7D15B0;
	constexpr uintptr_t get_state = 0x47F1C0;
	constexpr uintptr_t identity_flag = 0x2E3EF24;
	constexpr uintptr_t luavm_load = 0x580FE0;
	constexpr uintptr_t task_spawn = 0x580270;
	constexpr uintptr_t print = 0xD8A5C0;

	// Other offsets:
	constexpr int16_t top = 20/*, base = 8*/;
	constexpr int16_t extra_space = 72, identity = 24;
}

namespace Types {
	using get_scheduler = uintptr_t(__cdecl*)();
	using get_state = uintptr_t(__thiscall*)(uintptr_t script_context, uint32_t* identity, uintptr_t* script);
	using luavm_load = int32_t(__fastcall*)(uintptr_t state, std::string* source, const char* chunk_name, int32_t env);
	using task_spawn = int32_t(__cdecl*)(uintptr_t state);
	using print = int32_t(__cdecl*)(int32_t type, const char* message, ...);
}

namespace Roblox {
	// Get the base address which all the function offsets are relative to.
	const auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));

	// Roblox functions:
	const auto get_scheduler = reinterpret_cast<Types::get_scheduler>(base + Offsets::get_scheduler);
	const auto get_state = reinterpret_cast<Types::get_state>(base + Offsets::get_state);
	const auto luavm_load = reinterpret_cast<Types::luavm_load>(base + Offsets::luavm_load);
	const auto task_spawn = reinterpret_cast<Types::task_spawn>(base + Offsets::task_spawn);
	const auto print = reinterpret_cast<Types::print>(base + Offsets::print);

	// Custom functions:
	inline uintptr_t get_global_state(uint32_t identity, uintptr_t script = NULL) {
		return get_state(Scheduler::get_script_context(), &identity, &script);
	}

	inline void set_identity(uintptr_t state, uint32_t identity) noexcept {
		*reinterpret_cast<int32_t*>(*reinterpret_cast<uintptr_t*>(state + Offsets::extra_space) + Offsets::identity) = identity;
	}

	inline void pop_stack(uintptr_t state, uint8_t amount) noexcept {
		*reinterpret_cast<uintptr_t*>(state + Offsets::top) -= 0x10 * amount;
	}
}
