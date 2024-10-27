#pragma once

#include <cstdint>
#include <string>
#include <Windows.h>

namespace memory::module {
	inline std::uintptr_t get_base(const std::string& module_name = "") {
		return reinterpret_cast<std::uintptr_t>(GetModuleHandleA(module_name.empty() ? nullptr : module_name.c_str()));
	}

	inline std::uintptr_t rebase(std::uintptr_t addr, std::uintptr_t base = 0x400000, const std::string& module_name = "") {
		return addr - base + get_base(module_name);
	}
}