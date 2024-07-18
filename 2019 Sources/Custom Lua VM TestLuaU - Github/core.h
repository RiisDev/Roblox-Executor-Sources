#pragma once
#include "scriptcontext.h"
#include "InternalUI.h"
void CreateConsole(const char* Name) {
	DWORD consoleOldProtect = 0;
	VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &consoleOldProtect);
	*(UINT*)FreeConsole = 0xC3;
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

int gettop_d(int a1)
{
	RL = a1;
	return (*(DWORD*)(a1 + 24) - *(DWORD*)(a1 + 28)) >> 4;
}

LONG WINAPI danghui_int3cbreakpoint(PEXCEPTION_POINTERS ex)
{
	if (ex->ContextRecord->Eip == int3)
	{
		ex->ContextRecord->Eip = (DWORD)(Bridge::rbxFunctionBridge);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void core()
{
	
	

	CreateConsole("Netex");
	void* old = placeHook(Format(0x8525e0), gettop_d);
	do { Sleep(1); } while (RL == 0);
	placeHook(Format(0x8525e0), old, 1);


	int3breakpoints.push_back(locateINT3());

	AddVectoredExceptionHandler(1, danghui_int3cbreakpoint);



	lua_State* L = lua_open();


	ImGUI::Begin();

	


}

