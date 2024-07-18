#include "Vanilla.h"

DWORD WINAPI LuaPipe(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Khemical"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			Wrapper::Execute_Script(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

BOOL WINAPI CONSOLEBYPASS()
{
	DWORD nOldProtect;
	if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(FreeConsole) = 0xC3;
	if (!VirtualProtect(FreeConsole, 1, nOldProtect, &nOldProtect))
		return FALSE;
	AllocConsole();
}

int WrapGlobal(RbxState *RLS, lua_State* LS, const char* index) {
	r_lua_getglobal(RLS, index);
	UnWrap(RLS, LS, -1);
	lua_setglobal(LS, index);
	return 1;
}

void WrapGlobals(RbxState*rL, lua_State*vL)
{
	for (auto k : rL->Globals) {
		WrapGlobal(rL, vL, k.c_str());
	}
}

void TestFunction_1(lua_State*vL) {
	r_lua_getglobal(LuaC.RobloxState , "print"); // print
	LuaC.r_lua_pushstring(LuaC.RobloxState, lua_tostring(vL, -1)); // print ...
	LuaC.r_lua_pcall(LuaC.RobloxState, 1, 0, 0); // 
}

int main() 
{
	CONSOLEBYPASS();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	SetWindowPos(ConsoleHandle, HWND_TOPMOST, 50, 20, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(ConsoleHandle, 1);
	SetConsoleTitleA("Khemical");
	printf("Scanning..\n");
	LuaC.RobloxState->ScriptContext = Ringascan::s_scan::Scan((BYTE*)&LuaC.RobloxState->ScriptContextVFTable, (BYTE*)"xxxx", PAGE_READWRITE);
	LuaC.RobloxState = (RbxState*)LuaC.RobloxState->ScriptContext + 56 * LuaC.RobloxState->CoreScript + 220 + *(DWORD *)(LuaC.RobloxState->ScriptContext + 56 * LuaC.RobloxState->CoreScript + 220);
	std::cout << "Roblox State: " << LuaC.RobloxState->rawState;

	LuaC.VanillaState = luaL_newstate();

	printf("Done!\n");
	Int3BreakPoints();

	luaL_openlibs(LuaC.VanillaState);

	printf("identity..\n");
	SetIdentity(LuaC.RobloxState, 6);

	WrapGlobals(LuaC.RobloxState, LuaC.VanillaState);

	printf("table..\n");
	lua_newtable(LuaC.VanillaState);

	lua_setglobal(LuaC.VanillaState, "_G");

	printf("func..\n");
	lua_register(LuaC.VanillaState, "TestFunction", (lua_CFunction)TestFunction_1);

	printf("pipe..\n");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LuaPipe, NULL, NULL, NULL);
	printf("Hooked!\n");
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
	case DLL_PROCESS_DETACH:
		exit(0);
		break;
	}
	return TRUE;
}
