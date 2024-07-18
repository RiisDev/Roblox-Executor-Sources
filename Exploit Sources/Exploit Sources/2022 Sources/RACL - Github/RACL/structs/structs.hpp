#pragma once

#include <cstdint>

struct rule_t
{
	std::string rule_name;

	std::string short_name;

	std::uint32_t unk[3];
};

struct dll_info_t
{
	std::uintptr_t base;

	std::uint8_t pad[0x4];

	std::uint32_t size;

	const wchar_t* const path;
};

struct allocation_info_t
{
	std::uintptr_t base;

	std::uint32_t size;
};