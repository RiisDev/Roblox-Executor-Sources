#pragma once
#include <iostream>
#include <Windows.h>

namespace hyperion::bypasses::console
{
	static void patch_console()
	{
		const auto free_console = &FreeConsole;

		DWORD old;
		VirtualProtect(free_console, 1, PAGE_EXECUTE_READWRITE, &old);
		*reinterpret_cast<std::uint8_t*>(free_console) = 0xC3;
		VirtualProtect(free_console, 1, old, &old);
	}

	static void allocate_console(const std::string& console_name)
	{
		patch_console();
		
		AllocConsole();
		FILE* file_stream;
		freopen_s(&file_stream, "CONIN$", "r", stdin);
		freopen_s(&file_stream, "CONOUT$", "w", stdout);
		freopen_s(&file_stream, "CONOUT$", "w", stderr);

		SetConsoleTitleA(console_name.c_str());
	}
}