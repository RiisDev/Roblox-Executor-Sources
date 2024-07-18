#include <Windows.h>
#include "main/main.hpp"
#include "roblox/bypasses/exceptionfilter/expectionfilter.hpp"
#include <thread>
#include "roblox/bypasses/console/console.hpp"
#include "roblox/bypasses/istainted/istainted.hpp"

bool __stdcall DllMain(HMODULE mod, DWORD reason, void*)
{
	DisableThreadLibraryCalls(mod);

	if (reason == DLL_PROCESS_ATTACH)
	{
		hyperion::bypasses::console::allocate_console("Hyperion client");
		//hyperion::bypasses::istained::terminate_istained(); removed until i can bother to fix early injection crash on it
		hyperion::bypasses::suef::terminate_suef_hook();
		SetUnhandledExceptionFilter(hyperion::bypasses::suef::top_level_exception_handler);
		std::thread(hyperion::main::main_thread).detach();
	}

	return true;
}