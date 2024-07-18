// OpenSourceWrapper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <iostream>
#include <intrin.h>
#include <WinInet.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include "wrap.hpp"
#pragma comment(lib, "wininet.lib")

namespace Memory {

	bool Compare(const char* pData, const char* bMask, const char* szMask)
	{
		while (*szMask) {
			if (*szMask != '?') {
				if (*pData != *bMask) return 0;
			}
			++szMask, ++pData, ++bMask;
		}
		return 1;
	}

	DWORD Scan(const char* vftable)  // Credits goes to tepig
	{
		MEMORY_BASIC_INFORMATION BasicMemoryInformation = {};
		SYSTEM_INFO SystemInformation = {};
		GetSystemInfo(&SystemInformation);
		DWORD StartingMemorySearchPosition = (DWORD)SystemInformation.lpMinimumApplicationAddress;
		DWORD MaximumSearchBoundary = (DWORD)SystemInformation.lpMaximumApplicationAddress;
		do {
			while (VirtualQuery((void*)StartingMemorySearchPosition, &BasicMemoryInformation, sizeof(BasicMemoryInformation))) {
				if ((BasicMemoryInformation.Protect & PAGE_READWRITE) && !(BasicMemoryInformation.Protect & PAGE_GUARD)) {
					for (DWORD Key = (DWORD)(BasicMemoryInformation.BaseAddress); ((Key - (DWORD)(BasicMemoryInformation.BaseAddress) < BasicMemoryInformation.RegionSize)); ++Key) {
						if (Compare((const char*)Key, vftable, "xxxx")) {
							return Key;
						};
					};
				};
				StartingMemorySearchPosition += BasicMemoryInformation.RegionSize;
			};
		} while (StartingMemorySearchPosition < MaximumSearchBoundary);
		return 0x0;
	}
}

namespace Console {
	void CreateConsole(const char *Name) {
		DWORD consoleOldProtect = 0;
		VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &consoleOldProtect);
		*(UINT*)FreeConsole = 0xC2;
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		DWORD cons = (WS_CAPTION | DS_MODALFRAME | WS_MINIMIZEBOX | WS_SYSMENU);
		SetWindowLong(GetConsoleWindow(), GWL_STYLE, cons);
		SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
		SetConsoleTitleA(Name);
	}

	void CloseConsole() {
		PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
	}
}

LONG WINAPI CallBack(PEXCEPTION_POINTERS ex)
{
	if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		if (ex->ContextRecord->Eip == int3breakpoint)
		{
			ex->ContextRecord->Eip = (DWORD)(LuaFunction);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}
void init()
{
	Console::CreateConsole("Open Source Wrapper");

	lua_State* l = lua_open(); //initialize the vanilla lua state
	luaL_openlibs(l);
	int vf_table = ASLR(0x18C2504);
	int script_context = Memory::Scan((char*)&vf_table);
	rL = *(int*)(script_context + 164) - (script_context + 164);
	
	printf("Script Context: %X\n", script_context);

	AddVectoredExceptionHandler(TRUE, CallBack);

	int thread = rlua_newthread(rL);

	do
	{
		std::string source;
		std::getline(std::cin, source);
		InitLua(thread, l, source.c_str());
		source = "";
	} while (true);

}
BOOL WINAPI DllMain(HMODULE dll, DWORD reason, PVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(dll);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init, NULL, NULL, NULL);
	}
	return TRUE;
}