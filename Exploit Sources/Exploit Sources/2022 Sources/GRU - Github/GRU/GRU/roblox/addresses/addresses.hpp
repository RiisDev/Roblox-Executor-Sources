#pragma once
#include <Windows.h>
#include <cstdint>

namespace roblox::addresses
{
	constexpr std::uintptr_t zstd_decompress = 0x5351B0;
	constexpr std::uintptr_t create_zstd_instance = 0x532F40;
	constexpr std::uintptr_t destroy_zstd_instance = 0x532FB0;
	constexpr std::uintptr_t bytecode_magic = 0x21D6E1C;

	constexpr std::uintptr_t task_scheduler = 0x3A5D510;
}