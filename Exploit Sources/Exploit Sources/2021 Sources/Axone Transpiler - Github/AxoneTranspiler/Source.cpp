#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <iostream>
#include <queue>
#include "LuauSerializer.h"
#include "LuauTranspiler.h"
#include "Retcheck.h"
#include "CallCheck Bypass.h"
#include "Rlua.h"
#include "Mouse_Keyboard.h"
#include "Functions.h"
extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lobject.h"
#include "Lua/llimits.h"
}



const char* ExploitName = "AxoneApi";
#define PipeName "\\\\.\\pipe\\AxoneApi"

void ConsoleBypass(const char* Name)
{
	DWORD ree;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &ree);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

bool ExecuteScript(DWORD RS, lua_State* L, std::string Script) {
	DWORD xD = r_lua_newthread(RS);
	std::string Bytecode = LuauTranspiler::compile(lua_open(), Script);

	Script = "spawn(function() script = Instance.new('LocalScript') script.Disabled = true; script.Parent = nil\r\n" + Script + "\r\nend)";

	lua_getglobal(L, "string");
	lua_getfield(L, -1, "dump");
	if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@AxoneTranspiler"))
	{
		const std::string ERR = lua_tostring(L, -1);
		lua_close(L);
		throw std::exception(ERR.c_str());
	}
	r_lua_deserialize(xD, ExploitName, LuauTranspiler::compile(lua_open(), Script).c_str(), LuauTranspiler::compile(lua_open(), Script).size(), 0);
	lua_pcall(L, 0, 0, 0);
	r_lua_spawn(xD);
	return true;
}

union r_Value
{
	void* gc;
	void* p;
	double n;
	bool b;
};

typedef struct r_TValue
{
	r_Value value;
	byte tt;
	byte shit;
};

int RenderstepHook(DWORD rL)
{
	if (!ScriptQueue.empty()) {
		auto script = ScriptQueue.front();
		ExecuteScript(rL, L, script);
		ScriptQueue.pop();
	}
	return 1;
}

void Execute(const std::string& Script) {
	if (Script != "") {
		ScriptQueue.push(Script);
	}
}

int exepipelmao() {
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT(PipeName),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,   
		1,
		9999999,
		9999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE) 
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';

				if (strlen(buffer) != NULL) {
					ScriptQueue.push(buffer);
				}
			}
		}

		DisconnectNamedPipe(hPipe);
	}
}

void Thread() {
	ConsoleBypass("Axone Api // n4hdha & itsgabite");
	DWORD DataModel = ReturnDataModel();
	DWORD ScriptContext = FindChildOfInstance(DataModel, "ScriptContext");
	RL = r_lua_newthread(RBX_LuaState(ScriptContext));
	DWORD Thread = r_lua_newthread(RL);
	L = luaL_newstate();
	luaL_openlibs(L);
	BypassCallCheck();
	RegisterMouseKeyboard();
	RegisterCustom();
	std::cout << "Call Check Bypassed." << std::endl;
	r_lua_getfield(RL, LUA_GLOBALSINDEX, "game");
	r_lua_getfield(RL, -1, "GetService");
	r_lua_pushvalue(RL, -2);
	r_lua_pushstring(RL, "RunService");
	r_lua_pcall(RL, 2, 1, 0);
	r_lua_getfield(RL, -1, "RenderStepped");
	r_lua_getfield(RL, -1, "Connect");
	r_lua_pushvalue(RL, -2);
	r_lua_pushcclosure(RL, (DWORD)RenderstepHook, 0);
	r_lua_pcall(RL, 2, 0, 0);
	r_lua_pop(RL, 2);
	
	std::cout << "Ready to Execute Scripts." << std::endl;
	CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(exepipelmao), NULL, NULL, NULL);
	while (true) {
		std::string Scriptt = "";
		std::getline(std::cin, Scriptt);
		ExecuteScript(RL, L, Scriptt);
	}
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved) {
	if (Reason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Thread), NULL, NULL, NULL);
	}
	return TRUE;
}