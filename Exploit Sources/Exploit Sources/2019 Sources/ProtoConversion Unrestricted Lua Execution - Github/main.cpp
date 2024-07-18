#include "encrypt.h"
#include "StaticDefs.h"
#include "utilites.h"
#include "Translate.hpp"


DWORD GetPointer(DWORD RVA) // instead of modbase + va - imagebase, it's modbase + rva
{
	DWORD ModBase = (DWORD)GetModuleHandleA(0);
	return ModBase + RVA;
}

#define GetRootObjPtr GetPointer(0x0) // this function will return a special root object which is the parent of DataModel, and then we can just offset to ScriptContext which is much faster
#define GetRootObjCallingConvention __stdcall // roblox started to shift the calling convention of this function so im making a macro here


typedef DWORD(GetRootObjCallingConvention* GetRootObjFunc)(int unk);
GetRootObjFunc GetRootObject = (GetRootObjFunc)GetRootObjPtr;

int script_context;

DWORD ReturnRobloxLuaState()
{
	/* snip or pay me   */
	return 0;
}

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

DWORD WINAPI Initiate()
{

	CreateConsole("OpenSource | ProtoConversion");

	printf("Scanning.. ");

	lua_State* L = lua_open();

	printf("Done\n");

	DWORD rL = ReturnRobloxLuaState();
	printf("LuaState: %X\n", rL);
	printf("ScriptContext: %X\n", script_context);
	printf("Debug initialized!\n");

	printf("You can now execute scripts!\n");


	do
	{
		std::string source;
		std::getline(std::cin, source);
		run(L, rL, source.c_str());
		source = "";
	} while (true);

}
BOOL WINAPI DllMain(HMODULE dll, DWORD reason, PVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(dll);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Initiate, NULL, NULL, NULL);
	}
	return TRUE;
}