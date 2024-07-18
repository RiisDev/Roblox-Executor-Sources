#pragma once
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

namespace hyperion::bypasses::returncheck
{

	struct cache
	{
		uintptr_t original;
		uintptr_t cloned;
	public:
		cache(uintptr_t original, uintptr_t cloned)
			: original(original), cloned(cloned) {};
	};

	std::uintptr_t check_in_array(std::uintptr_t address);

	std::uintptr_t calculate_function_size(std::uintptr_t originaladdress);

	std::uintptr_t scan_for_retcheck(std::uintptr_t originaladdress, std::size_t function_size);

	std::uintptr_t clone_function(std::uintptr_t originaladdress, std::size_t function_size);

	std::uintptr_t return_safe_address(std::uintptr_t originaladdress);

	void patch(std::uintptr_t originaladdress, std::uintptr_t new_function, std::uintptr_t location, std::size_t function_size);
}