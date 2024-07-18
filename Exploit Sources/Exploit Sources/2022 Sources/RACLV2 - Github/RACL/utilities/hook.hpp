#pragma once

#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <cstring>

inline std::uintptr_t tramp_hook(std::uintptr_t func, std::uintptr_t new_func, std::size_t inst_size)
{
	constexpr auto extra_size = 5;

	auto clone = reinterpret_cast<std::uintptr_t>(VirtualAlloc(nullptr, inst_size + extra_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

	if (!clone)
		return 0;

	std::memmove(reinterpret_cast<void*>(clone), reinterpret_cast<void*>(func), inst_size);

	const auto jmp_pos = (func - clone - extra_size);

	*reinterpret_cast<std::uint8_t*>(clone + inst_size) = 0xE9;
	*reinterpret_cast<std::uintptr_t*>(clone + inst_size + 1) = jmp_pos;

	DWORD old_protect;

	VirtualProtect(reinterpret_cast<void*>(func), inst_size, 0x40, &old_protect);

	std::memset(reinterpret_cast<void*>(func), 0x90, inst_size);

	const auto rel_location = (new_func - func - extra_size);
	*reinterpret_cast<std::uint8_t*>(func) = 0xE9;
	*reinterpret_cast<std::uintptr_t*>(func + 1) = rel_location;

	VirtualProtect(reinterpret_cast<void*>(func), inst_size, old_protect, &old_protect);

	return clone;
}
