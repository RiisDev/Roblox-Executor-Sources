#include "Memory.h"
#include "Wrapper.h"
//#include "Menu.h"
#include "Addresses.h"
#include "Commands.h"
#include "stdafx.h"

#define state rL

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

inline void getservice(int L, const char* s)
{
	rlua_getglobal(L, "game");
	rlua_getfield(L, -1, "GetService");
	rlua_pushvalue(L, -2);
	rlua_pushstring(L, s);
	rlua_pcall(L, 2, 1, 0);
}

inline void Notification(string title, string text, string id)
{
	getservice(rL, "CoreGui");
	rlua_getfield(rL, -1, "RobloxGui");
	rlua_getfield(rL, -1, "SendNotification");
	rlua_getfield(rL, -1, "Fire");
	rlua_pushvalue(rL, -2);
	rlua_pushstring(rL, title.c_str()); // Title
	rlua_pushstring(rL, text.c_str());
	rlua_pushstring(rL, id.c_str()); // Logo/Icon
	rlua_pushnumber(rL, 5);
	rlua_pcall(rL, 5, 0, 0);
}

void exeLua(lua_State *L, string script)
{
	lua_State *T = lua_newthread(L);
	SetUpEnvironment(T);
	try {
		int err = luaL_dostring(T, script.c_str());
		printf("Success: %u\n", err);
		if (err) {
			const char* errmsg = lua_tolstring(T, (-1), NULL);
			std::string err = "ERROR: ";
			err += errmsg;
			SingletonPrint(ErrorColors::Error, err.c_str());
		}
	}

	catch (std::exception e) {
		std::string err = "Error: ";
		err += e.what();
		SingletonPrint(ErrorColors::Error, err.c_str());
	}

	catch (...) {
		printf("There was an error when executing.\n");
	}
	lua_pcall(T, 0, 0, 0);
	rlua_settop(rL, 0);
	lua_settop(T, 0);
}

bool Execute(lua_State *L, const char* s) {
	lua_State *T = lua_newthread(L);
	SetUpEnvironment(T);
	if (luaL_loadstring(T, s)) {
	    rlua_getfield(rL, LUA_GLOBALSINDEX, "print");
	    rlua_pushstring(rL, lua_tostring(T, -1));
		rlua_pcall(rL, 1, 0, 0);
		lua_pop(T, 1);

		return false;
	}
	lua_pcall(T, 0, 0, 0);
	rlua_settop(rL, 0);
	lua_settop(T, 0);
	return true;
}

void Authenticate() {

}

std::string Input() {
	std::string in;
	getline(std::cin, in);
	return in;
}

int chathook(int newluastate_containingtext)
{
	const char* textx = rlua_tolstring(newluastate_containingtext, -2, NULL);
	string text = textx;
	if (text.substr(0, 1) == "/") {
		if (text.substr(1, 1) == "e") {
			text = text.substr(2, std::string::npos);
			rlua_getglobal(rL, "print");
			rlua_pushstring(rL, text.c_str());
			rlua_pcall(rL, 1, 0, 0);
			Eggsecute(text);
		}
	}
	return 0;
}

int main(lua_State *L) {
	Console("Cyber v2 Debug Log");
	enable_exceptionHandler(); 
	std::cout << "Authenticating...\n";
	Sleep(1000);
	std::cout << "Scanning...\n";
	if (Scan() == true)
		cout << "Ready, use console or ui!\n";
	else {
		cout << "Sorry an error has occured while scanning!!\n";
		cout << "ScriptContext is: " << aScriptContext << endl;

		cout << "luaState is : " << rL << endl << endl << endl;
		return 1;
	}
	global_L = L;

	*(DWORD*)Context = 6;
	while (true) {
		cout << "\n>";
		std::string i = Input();
		Eggsecute(i);
		
	}

	return 0;
}

void Eggsecute(std::string input) {
	yes();
	lua_State *L = lua_open();
	lua_State *T = lua_newthread(L);
	luaL_openlibs(T);
	SetUpEnvironment(T);
	exeLua(T, input.c_str());
}


DWORD WINAPI LuaPipe(PVOID lvpParameter)
{
	HANDLE hPipe;
	char buffer[102400];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\CLua"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		9999,
		9999,
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
					if (strlen(buffer) != NULL) {
						Eggsecute(buffer);
					}
				}
				catch (std::exception e) {
					//errorhere
				}
				catch (...) {
					//errorhere
				}
			}
		}
		DisconnectNamedPipe(hPipe);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, NULL, (LPTHREAD_START_ROUTINE)&main, NULL, NULL, NULL);
		CreateThread(NULL, 0, LuaPipe, NULL, 0, NULL);
		break;
	}
	case DLL_PROCESS_DETACH: break;
	}
	return true;
}

//just let me open my other source