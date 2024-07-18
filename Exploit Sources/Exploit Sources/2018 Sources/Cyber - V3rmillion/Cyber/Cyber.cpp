#pragma once

#include <Windows.h>

#include "Memory.hpp"
#include "Wrapper.hpp"
#include "Offsets.hpp"

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
};

inline void getservice(int L, const char* s)
{
	rlua_getglobal(L, "game");
	rlua_getfield(L, -1, "GetService");
	rlua_pushvalue(L, -2);
	rlua_pushstring(L, s);
	rlua_pcall(L, 2, 1, 0);
}

inline void Notification(std::string title, std::string text, std::string id)
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

void exeLua(lua_State *L, std::string script)
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
	};
	lua_pcall(T, 0, 0, 0);
	rlua_settop(rL, 0);
	lua_settop(T, 0);
	return true;
};

int main(lua_State *L) {
	Console("Cyber");
	enable_exceptionHandler();
	std::cout << "Authenticating... ";
	Sleep(1000);
	std::cout << "Scanning... ";
	if (Scan() == true)
		std::cout << "Ready!!\n";
	else {
		std::cout << "Sorry an error has occured while scanning!!\n";
		std::cout << "ScriptContext is: " << aScriptContext << std::endl;
		std::cout << "luaState is : " << rL << std::endl << std::endl << std::endl;
		return 1;
	};
	*(DWORD*)Context = 6;
	//Notification("Cyber", "Loaded.", "rbxassetid://1246336204");
	std::cout << "This is Cyber's console do not close it.\n";
	return 0;
};

void Eggsecute(std::string input) {
	lua_State *L = lua_open();
	lua_State *T = lua_newthread(L);
	luaL_openlibs(T);
	SetUpEnvironment(T);
	exeLua(T, input.c_str());
};

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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			DisableThreadLibraryCalls(hModule);
			CreateThread(NULL, 0, LuaPipe, NULL, 0, NULL);
			CreateThread(0, NULL, (LPTHREAD_START_ROUTINE)&main, NULL, NULL, NULL);
			break;
		};
	};
	return true;
};