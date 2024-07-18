#pragma once

#include <string_view>
#include <Windows.h>
#include <memory>

namespace memcheck::utils
{
	void open_console();

	void hook(std::uintptr_t to_patch, std::uintptr_t func, std::size_t size = 0x5);
}