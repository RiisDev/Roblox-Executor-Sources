#pragma once

#include <Windows.h>
#include <cstdint>
#include <iostream>

#pragma section(".text")
__declspec(allocate(".text")) const std::uint8_t ret_stub[] = { 0x90,0xC3 };

namespace utilities::io
{
	constexpr auto debug = true;

	inline void initiate(const char* const console_name)
	{
		static auto global_stub_pointer = reinterpret_cast<std::uintptr_t>(ret_stub);

		if constexpr (debug)
		{
			if (const auto lib = LoadLibraryA("KERNEL32.dll"))
			{
				if (const auto free_console_address = reinterpret_cast<std::uintptr_t>(&FreeConsole))
				{
					DWORD old_protection;

					constexpr const auto size = sizeof(std::uintptr_t) + sizeof(std::uint8_t) * 2;

					VirtualProtect(reinterpret_cast<void*>(&FreeConsole), size, PAGE_EXECUTE_READWRITE, &old_protection);

					*reinterpret_cast<void**>(free_console_address + sizeof(std::uint8_t) * 2) = &global_stub_pointer;

					VirtualProtect(reinterpret_cast<void*>(&FreeConsole), size, old_protection, &old_protection);
				}

				AllocConsole();

				FILE* file_stream;

				freopen_s(&file_stream, "CONIN$", "r", stdin);
				freopen_s(&file_stream, "CONOUT$", "w", stdout);
				freopen_s(&file_stream, "CONOUT$", "w", stderr);

				SetConsoleTitleA(console_name);
			}
		}
	}
}