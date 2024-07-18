
#include <Windows.h>
#include <iostream>
#include "../Rev/RevVM.hpp"

BOOL WINAPI OpenConsole(LPCSTR ConsoleTitle)
{
	DWORD nOldProtect;
	if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(FreeConsole) = 0xC3;
	if (!VirtualProtect(FreeConsole, 1, nOldProtect, &nOldProtect))
		return FALSE;
	
	SetConsoleTitleA(ConsoleTitle);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdout);
	return TRUE;
}

void* placeHook(DWORD address, void* hook, bool revert = false) {
	DWORD oldprot;
	if (!revert) {
		void* oldmem = new void*;
		void* result = new void*;
		memcpy(oldmem, (void*)address, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		*(char*)address = 0xE9; *(DWORD*)(address + 1) = (DWORD)hook - address - 5;
		memcpy(result, oldmem, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
		return result;
	}
	else {
		VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		memcpy((void*)address, hook, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
		return NULL;
	}
}
DWORD RL = 0;
int gettop_d(int a1)
{
	RL = a1;
	return (*(DWORD*)(a1 + 20) - *(DWORD*)(a1 + 8)) >> 4;
}

#define Format(x)(x - 0x400000 + (DWORD)GetModuleHandleA(0))

void CreateConsole(const char* Name) {
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


VOID WINAPI OVMEntry()
{
	CreateConsole("OVM2 Beta - by the public <3");
	void* old = placeHook(Format(0x84da80), gettop_d);
	do { Sleep(1); } while (RL == 0);
	placeHook(Format(0x84da80), old, 1);
	lua_State* L = lua_open();
	do
	{
		std::string script;
		std::getline(std::cin, script);
		RunScript(RL, L, script);
	} while (true);
}

BOOL WINAPI DllMain(HMODULE DllModule, DWORD TReason, LPVOID Reserved)
{
	if (TReason == DLL_PROCESS_ATTACH)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OVMEntry, NULL, NULL, NULL);
	return TRUE;
}
