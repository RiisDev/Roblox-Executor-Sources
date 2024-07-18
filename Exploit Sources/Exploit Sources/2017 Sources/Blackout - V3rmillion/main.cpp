#include <Windows.h>
#include <iostream>

#include "memory.h"
#include "blackoutlua.h"
#include "commands.h"

using namespace std;
using namespace rbx_Instance;

VOID GetCoreInternals() {
	INT ScriptContextBytes = ScriptContext_bytes_addr;
	ScriptContext = (PDWORD)Memory::Scan(PAGE_READWRITE, (PCHAR)&ScriptContextBytes, "xxxx");
	rbx_L = rbx_lua_newthread(ScriptContext[55] - (DWORD)&ScriptContext[55]); // disgusting way of getting the state, I don't use this anymore
	DataModel = rbx_GetParent((DWORD)ScriptContext);
	Players = rbx_GetService("Players");
	Workspace = rbx_GetService("Workspace");
	Lighting = rbx_GetService("Lighting");
	LocalPlayer = rbx_GetLocalPlayer(Players);
}

DWORD WINAPI Main(PVOID lpvParameter) {
	CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)SetThreadIdentity, NULL, NULL, NULL);
	StartConsole("Blackout | By Quad Core @V3rmillion");
	SubTitle("Blackout - Private Beta\n");
	GetCoreInternals();

	print("\nScript Context: %X\n", ScriptContext);
	print("Lua State: %X\n", rbx_L);

	string Source;

	while (true) {
		cout << "\n>";
		getline(cin, Source);
		vector<string> Arguments = SplitString(Source, ' ');

		HandleCommands(Arguments);
	}
	return 0;
}

BYTE DllMain(HMODULE hmodDll, DWORD fdwReason, PVOID pvdReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, NULL, Main, hmodDll, NULL, NULL);
	}
	return TRUE;
}