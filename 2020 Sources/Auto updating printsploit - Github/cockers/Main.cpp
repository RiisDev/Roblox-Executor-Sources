#include <iostream>
#include "Retcheck.h"
#include <sstream>
#include <vector>
#include <fstream>
#include <istream>
#include <iterator>
#include "AutoPP.h"
#include "EyeStep/eyestep_utility.h"

using namespace EyeStep::util;
uint32_t rL;

void DebugConsole()
{
	DWORD OldPerm;
	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &OldPerm);
	*(BYTE*)& FreeConsole = 0xC3;
	VirtualProtect(&FreeConsole, 1, OldPerm, NULL);
	AllocConsole();
	SetConsoleTitleA("Print-Sploit -- Auto Updating Edition");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
}




void runPrint(std::string str)
{
	r_lua_getglobal(rL, "print");
	r_lua_pushstring(rL, str.c_str());
	r_lua_pcall(rL, 1, 0, 0);
}

void main()
{
	/* Init */
	DebugConsole();
	// Epic code (dont skid too hard ryle)
	EyeStep::open(GetCurrentProcess());
	printf("EyeStep Loaded. \n");
	auto func = getPrologue(EyeStep::scanner::scan_xrefs("The meta", 1)[1]);
	auto calls = getCalls(nextCall(func));
	auto index2adr = nextCall(calls[0]);
	auto index2adr_xref = EyeStep::scanner::scan_xrefs(index2adr);
	auto func2 = getPrologue(EyeStep::scanner::scan_xrefs("__meta", 0)[0]);
	
	uint32_t a_lua_pushstring = getCalls(func2)[0];
	uint32_t a_getfield = calls[0];
	uint32_t a_touserdata = calls[1];
	uint32_t a_settop = calls[2];
	uint32_t a_setfield = calls[5];
	uint32_t a_pcall = getPrologue(index2adr_xref[15]);
	/* Init the Actual Auto updating offsets! */
	calls = getCalls(calls[0]);

	uint32_t a_index2adr = calls[0];

	r_index2adr = reinterpret_cast<T_index2adr>(EyeStep::util::createRoutine(unprotect(a_index2adr), 2));
	r_lua_getfield = reinterpret_cast<T_getfield>(EyeStep::util::createRoutine(unprotect(a_getfield), 3));
	r_lua_pcall = reinterpret_cast<rLua_pcall>(EyeStep::util::createRoutine(unprotect(a_pcall), 4));
	r_lua_pushstring = reinterpret_cast<T_pushstring>(EyeStep::util::createRoutine(unprotect(a_lua_pushstring), 2));
	r_lua_touserdata = reinterpret_cast<rtouserdata>(EyeStep::util::createRoutine(unprotect(a_touserdata), 2));
	r_lua_settop = reinterpret_cast<rsettop>(EyeStep::util::createRoutine(unprotect(a_settop), 2));
	r_lua_setfield = reinterpret_cast<T_setfield>(EyeStep::util::createRoutine(unprotect(a_setfield), 3));

	/* init luastate so we can actually print*/
	rL = EyeStep::util::debug_r32(a_index2adr + 3, EyeStep::R32_EBP, 8, 1)[0];

	r_lua_getfield(rL, -10002, "warn");
	r_lua_pushstring(rL, "Loaded Print Exploit Made by ElKoax and EyeStep!");
	r_lua_pcall(rL, 1, 0, 0);

	while (true)
	{
		std::string str;
		std::getline(std::cin, str);
		runPrint(str);
	}
}





BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);

		DWORD OldProtection;
		VirtualProtect(Module, 4096, PAGE_READWRITE, &OldProtection);
		ZeroMemory(Module, 4096);

		HANDLE hThread = NULL;
		HANDLE hDllMainThread = GetCurrentThread();
		if (Reserved == NULL) {

			if (!(hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL))) {
				CloseHandle(hDllMainThread);
				return FALSE;
			}
			CloseHandle(hThread);
		}
		break;
	}
	return TRUE;
}
