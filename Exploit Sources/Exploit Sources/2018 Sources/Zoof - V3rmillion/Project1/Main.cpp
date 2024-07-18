#include "Functions.h"
#include "Definitions.h"
#include "Metamethods.h"
#include "RFunctions.h"

void Console(const char* string) {
	DWORD consoleOldProtect;
	VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &(DWORD)consoleOldProtect);
	*(UINT*)FreeConsole = 0xC2;
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	DWORD cons = (WS_CAPTION | DS_MODALFRAME | WS_MINIMIZEBOX | WS_SYSMENU);
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, cons);
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	SetConsoleTitleA(string);
	HANDLE HA = GetStdHandle(STD_OUTPUT_HANDLE);
}

int main() {
	Console("Zoof | Open source project by NiftyHearts");
	std::cout << "Scanning... ";
	if (Scan())
		std::cout << "done!\n";
	else
		std::cout << "ERROR: Something went wrong while scanning.\n";
	std::cout << "Creating environment... ";
	luaL_openlibs(L);
	createmetas();
	rlua_register_rf(L);
	std::cout << "done!\n";
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << "This open source project was made by Niftyhearts, Enjoy!\n";
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Welcome to Zoof, " << GetLocalPlayerName() << "! Please enter your script!\r\n\n";
	
	for (;;) {
		std::cout << "[SCRIPT]: ";
		std::string input;
		std::getline(std::cin, input);
		Execute(input);
	}
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)init,NULL, NULL, NULL);
		break;
	}
	case DLL_PROCESS_DETACH: break;
	}
	return true;
}

