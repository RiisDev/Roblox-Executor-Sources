#pragma once
#include "RunCommand.h"  //visual studio 2017 needed!
#include "Memory.h"
#include "Auth.h"

void Vanic_CheckFiltering() {
	lua_getglobal(L, "game");
	lua_getfield(L, -1, "Workspace");
	lua_getfield(L, -1, "FilteringEnabled");
	bool isFiltered = lua_toboolean(L, -1);
	if (isFiltered) {
		printf("Filtering is enabled. Most commands won't replicate.\n");
	}
	else {
		printf("Filtering is disabled.\n");
	}
}

void Vanic_Start() {
	DWORD OldPerm, OldPermMsg, Base;

	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &OldPerm);
	VirtualProtect(&MessageBoxA, 5, PAGE_EXECUTE_READWRITE, &OldPerm);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	SetConsoleTitle("Celetrius | By Celetrius Development Team");
	printf("Celetrius Development Team\r\nDeniality - Creator of Celetrius\r\nFuZion Panders - Helping with ideas and commands\r\n");
	HWND ConsoleHandle = GetConsoleWindow();
	SetWindowLong(ConsoleHandle, GWL_STYLE,
		GetWindowLong(ConsoleHandle, GWL_STYLE) & ~WS_MAXIMIZEBOX);
	SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(ConsoleHandle, SW_NORMAL);
	printf("Authenticating..\r\n");
	Auth(); //remove this, this removes whitelist.
	printf("Scanning.. ");

	DWORD SCV = aslr(0xscriptcontextaddress);
	ScriptContext = Memory::Scan((char*)&SCV);
	DataModel = getParent(ScriptContext);
	Players = findFirstClass(DataModel, "Players");
	LocalPlayer = getLocalPlayer(Players);
	PlayerName = getName(LocalPlayer);
	L = //snip this is basically the luastate, skid off tears lua state  xD

	printf("Completed.\nWelcome %s, enjoy using Vanic!\nScript Context: %x\nLua State: %x\n", PlayerName, ScriptContext, L);

	Vanic_CheckFiltering();

	printf("Type 'cmds' for the list of commands.\r\n");

	while (true) {
		printf("> ");
		std::string Cmd = Input();
		auto Context = (int*(__cdecl*)
			())Memory::SigScan("\x55\x8B\xEC\x51\x56\xE8\x00\x00\x00\x00\x50", "xxxxxx????x");
		*(int*)Context() = 7;
		RunCommand(Cmd);
		RunLuaC(Cmd);
	}
}