#include "returncheck.hpp"

std::vector<hyperion::bypasses::returncheck::cache> cloned_funcs;

std::uintptr_t hyperion::bypasses::returncheck::check_in_array(std::uintptr_t address)
{
	for (const auto &inst : cloned_funcs)
	{
		if (inst.original == address)
		{
			if (inst.cloned != 0)
				return inst.cloned;
			else
				return address;
		}
	}
	return 0;
}

std::uintptr_t hyperion::bypasses::returncheck::calculate_function_size(std::uintptr_t originaladdress)
{
	std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(originaladdress);

	do
		bytes += 0x10;
	while (!(bytes[0] == 0x55 && bytes[1] == 0x8B && bytes[2] == 0xEC));

	return reinterpret_cast<std::uintptr_t>(bytes) - originaladdress;
}

std::uintptr_t hyperion::bypasses::returncheck::scan_for_retcheck(std::uintptr_t originaladdress, std::size_t function_size)
{
	std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(originaladdress);

	for (int i = 0; i < function_size; ++i)
	{
		if ((bytes[i] == 0x72 && bytes[i + 2] == 0xA1 && bytes[i + 7] == 0x8B) || (bytes[i] == 0x72 && bytes[i + 3] == 0xE8 && bytes[i + 7] == 0xFF && bytes[i + 8] == 0x83))
			return i;
	}
	return 0;
}

std::uintptr_t hyperion::bypasses::returncheck::clone_function(std::uintptr_t originaladdress, std::size_t function_size)
{
	void* clone = VirtualAlloc(0, function_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(clone, reinterpret_cast<void*>(originaladdress), function_size);
	return reinterpret_cast<std::uintptr_t>(clone);
}

void hyperion::bypasses::returncheck::patch(std::uintptr_t originaladdress, std::uintptr_t new_function, std::uintptr_t location, std::size_t function_size)
{
	*reinterpret_cast<std::uint8_t*>(new_function + location) = 0xEB;

	std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(new_function);
	std::uintptr_t addy = new_function;

	for (int i = 0; i < function_size; ++i)
	{
		if (bytes[i] == 0xE8)
		{
			auto address = originaladdress + ((addy + i) - new_function);

			auto original_calladdr = (address + *reinterpret_cast<int*>(address + 1)) + 5;

			if (original_calladdr % 16 == 0)
			{
				auto relativeaddy = original_calladdr - (addy + i) - 5;
				*reinterpret_cast<int*>((addy + i) + 1) = relativeaddy;
				i = i + 0x4;
			}
		}
	}
}

std::uintptr_t hyperion::bypasses::returncheck::return_safe_address(std::uintptr_t originaladdress)
{
	const auto cached_address = check_in_array(originaladdress);
	if (cached_address != 0)
		return cached_address;

	auto function_size = calculate_function_size(originaladdress);

	if (auto location = scan_for_retcheck(originaladdress, function_size))
	{
		auto unprotected_func = clone_function(originaladdress, function_size);

		patch(originaladdress, unprotected_func, location, function_size);

		std::printf("Bypassed retcheck at: %p\n", originaladdress);
		cloned_funcs.emplace_back(originaladdress, unprotected_func);
		return unprotected_func;
	}

	std::printf("Found no ret sig, amoy must have ate it, fat fuck: %p with size: %i\n", originaladdress, function_size);
	cloned_funcs.emplace_back(originaladdress, 0);
	return originaladdress;
}