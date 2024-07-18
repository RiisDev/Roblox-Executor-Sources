#include <Windows.h>
#include <thread>

#include "utils/utils.hpp"
#include "memcheck/memcheck.hpp"

void core()
{
	if constexpr (debug)
		memcheck::utils::open_console();

	memcheck::initiate();
}

bool __stdcall DllMain(void*, long reason, void*)
{
	if (reason == DLL_PROCESS_ATTACH)
		std::thread{ core }.detach();

	return true;
}