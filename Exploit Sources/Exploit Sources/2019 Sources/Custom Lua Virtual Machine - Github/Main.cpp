#include "WatchingVM.h"

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
DWORD RL = 0;
int gettop_d(int a1)
{
	RL = a1;
	return (*(DWORD*)(a1 + 20) - *(DWORD*)(a1 + 8)) >> 4;
}

void Init()
{
	CreateConsole("CLVM Tutorial?");

	std::cout << "Scanning, Please wait...\n";
	void* old = placeHook(RBase(0x8491D0), gettop_d);
	do { Sleep(1); } while (RL == 0);
	placeHook(RBase(0x8491D0), old, 1);


	std::cout << "Lua State: " << std::hex << RL << std::endl;

	lua_State* L = lua_open();
	//luaL_openlibs(L);
	std::cout << "Scan finished!!\n";

	std::string script;

	while (true)
	{
		std::getline(std::cin, script);
		RunScript(RL, L, script.c_str());
	}

}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&Init, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default: break;
	}

	return TRUE;
}