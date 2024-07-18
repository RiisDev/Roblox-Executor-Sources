#include <Windows.h>
#include <thread>
#include "Roblox/Execution/Execution.h"
#include "Roblox/RLua/RLua.h"
#include "Roblox/Environment/Functions.h"

void ConsoleBypass(const char* title)
{
	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, new unsigned long);
	*reinterpret_cast<unsigned char*>(FreeConsole) = 0xC3;
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	SetConsoleTitleA(title);
}

DWORD __stdcall Pipe(void*)
{
	char buffer[999999];
	DWORD len;
	HANDLE pipe = CreateNamedPipeW(TEXT("\\\\.\\pipe\\ExploitPipe"), PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, PIPE_WAIT, 1, 999999, 999999, NMPWAIT_USE_DEFAULT_WAIT, NULL);

	while (pipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(pipe, NULL) != FALSE)
		{
			while (ReadFile(pipe, buffer, sizeof(buffer) - 1, &len, NULL) != FALSE)
			{
				buffer[len] = '\0';
			}
			Execute(rL, buffer);
		}
		DisconnectNamedPipe(pipe);
	}

	return 0;
}

void Main()
{
	ConsoleBypass("locust is gay");

	rL = r_newthread(GetState());
	SetLevel(rL, 7);

	InitLBI();

	AddVectoredExceptionHandler(1, VEH); // Initialize callcheck bypass
	registerfunc(rL, reinterpret_cast<int>(getgenv), "getgenv");
	registerfunc(rL, reinterpret_cast<int>(getreg), "getreg");
	registerfunc(rL, reinterpret_cast<int>(getrawmetatable), "getrawmetatable");
	registerfunc(rL, reinterpret_cast<int>(setrawmetatable), "setrawmetatable");

	CreateThread(0, 0, Pipe, 0, 0, 0);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == 1)
	{
		std::thread(Main).detach();
	}
	return TRUE;
}