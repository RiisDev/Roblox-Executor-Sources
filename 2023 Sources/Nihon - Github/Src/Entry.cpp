#pragma once
#include "Entry.hpp"

auto Core::Init(std::string ModuleName) -> void {
	auto taskScheduler{ TaskScheduler::getSingleton() };
	taskScheduler->initialize();

	auto L = taskScheduler->luaState;
	
	taskScheduler->setIdentity(8);
	luaL_sandboxthread(L);
	
	auto execution{ Execution::getSingleton() };
	execution->setupHook();

	auto environment{ Environment::getSingleton() };
	environment->Register(L);

	auto bridge{ Bridge::getSingleton() };
	std::thread(bridge->createPipe, Config::PipeName).detach();

	execution->executeScript(initScript);
}

auto WINAPI DllMain(HMODULE hModule, std::uintptr_t dwReason, void*) -> int {
	DisableThreadLibraryCalls(hModule);

	if (dwReason == DLL_PROCESS_ATTACH)
		std::thread(Core::Init, Config::Name).detach();

	return 1;
}
