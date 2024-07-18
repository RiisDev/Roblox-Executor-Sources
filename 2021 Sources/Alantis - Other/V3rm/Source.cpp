#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <queue>

#include "LuauSerializer.h"
#include "LuauTranspiler.h"
#include "NewFunctions.h"
#include "nlohmann/json.hpp"
#include "Retcheck.h"
#include "CallCheck Bypass.h"


#include "Rlua.h"

extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lobject.h"
#include "Lua/llimits.h"
#include "nlohmann/json_fwd.hpp"
}

using namespace std;
using json = nlohmann::json;

void ConsoleBypass(const char* Name)
{
	DWORD AAAAAAAAAAAAAAA;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &AAAAAAAAAAAAAAA);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

bool ExecuteScript1(DWORD RS, lua_State* L, std::string Script) {
	//std::cout << "[Newthread]" << std::endl;
	DWORD xD = r_lua_newthread(RS); // back
	//std::cout << "[Newthread Made]" << std::endl;
	std::string Bytecode = LuauTranspiler::compile(lua_open(), Script);
	//std::cout << "[LuauTranspiler]" << std::endl;

	Script = "spawn(function() script = Instance.new('LocalScript') script.Disabled = true script.Parent = nil\r\n" + Script + "\r\nend)";
	lua_getglobal(L, "string");
	lua_getfield(L, -1, "dump");
	if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@aaa"))
	{
		std::cout << "[Error]: " << lua_tostring(L, -1) << "\n";
		lua_pop(L, 1);
	}
	lua_pcall(L, 1, 1, 0);

	Script = replaceAll(Script, "game:GetObjects", "getobjects");
	Script = replaceAll(Script, "game:HttpGet", "httpget");

	size_t scriptsz;
	const auto scriptb = lua_tolstring(L, -1, &scriptsz);
	std::cout << "[PushLString]" << std::endl;
	//r_lua_setfield(xD, LUA_GLOBALSINDEX, "SCRIPT_VALUE");
	r_luau_deserialize(xD, "tyo", LuauTranspiler::compile(lua_open(), Script).c_str(), LuauTranspiler::compile(lua_open(), Script).size());
	std::cout << "[Deserialized Bytecode!]" << std::endl;
	lua_pcall(L, 0, 0, 0);
	r_lua_spawn(xD);
	std::cout << "[Spawn]" << std::endl;
	return true;
}

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


	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\AtlantisOP"), //This is your pipe name you can change it to whatever you want just make sure it has \\\\.\\pipe\\ at the beginning
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		9999999,
		9999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';

				if (strlen(buffer) != NULL) {
					ScriptQueue.push(buffer);
				}
			}
		}
	}
}

void SetLevel(DWORD RobloxState, int Level)
{
	int v3 = *(DWORD*)(RobloxState + EmptySpace1);
	*(DWORD*)(v3 + EmptySpace2) = Level;
}

DWORD SetThreadIdentity(DWORD rL, int level)
{
	return *(DWORD*)(*(DWORD*)(rL + Identity2) + Identity1) = level;
}

void Thread() {
	ConsoleBypass("Atlantis Software // Nexus42");
	system("color b");
	int errors = 0;
	std::cout << logo << endl;
	std::cout << ConsoleColors::White() << " [" << ConsoleColors::LightAqua() << "Atlantis DLL Credits" << ConsoleColors::White() << "]" << ConsoleColors::LightRed() << "\n    - Imperious | Transpiler\n    - MellonYT | Addresses\n\n    - Nexus42 | Modifying\n    - DeepPain | Kewl Functions\n    - Zexea | Kewl Functions\n\n" << ConsoleColors::LightAqua();
	
	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Initializing JSON... ";
	
	string jsr = DownloadURL(DownloadURL("https://raw.githubusercontent.com/AtlantisSoftware4481/AtlantisDevelopment/master/DLL%20JSON%20-%20Nexus42/Link%20-%20Nexus42.txt").c_str());
	
	if (json::accept(jsr)) {
		
		js = json::parse(jsr);
		
		std::cout << ConsoleColors::LightGreen() << "Complete.\n" << ConsoleColors::LightAqua();
	}
	else {
		errors++;
		
		std::cout << ConsoleColors::LightRed() << "Failed.\n" << ConsoleColors::LightAqua();
		std::cout << jsr << "\n\n";
	}
	
	string curV = js["version"];
	
	string code = DownloadURL("https://setup.rbxcdn.com/version");
	
	if (code != curV) {
		print("Update Detected! Please DM Nexus42#2801 and Wait. Updated Version: " + code);
		Sleep(1200);
		::ShowWindow(GetConsoleWindow(), SW_HIDE);
		return;
	}
	print("On Latest Version: " << curV);
	
	if (!filesystem::exists(GetDLLPath("\\Workspace"))) {
		filesystem::create_directory(GetDLLPath("\\Workspace"));
		print("Created Workspace Folder");
	}
	
	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Updating JSON... ";
	UpdateAddresses(RL, js);
	
	std::cout << ConsoleColors::LightGreen() << "Complete.\n" << ConsoleColors::LightAqua();
	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Returning Datamodel... ";
	DWORD DataModel = ReturnDataModel();
	
	std::cout << ConsoleColors::LightGreen() << "Complete.\n" << ConsoleColors::LightAqua();
	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Scanning ScriptContext... ";
	DWORD ScriptContext = FindChildOfInstance(DataModel, "ScriptContext");
	
	std::cout << ConsoleColors::LightGreen() << "Complete.\n" << ConsoleColors::LightAqua();
	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Generating Stack... ";
	RL = r_lua_newthread(RBX_LuaState(ScriptContext));
	
	std::cout << ConsoleColors::LightGreen() << "Complete.\n" << ConsoleColors::LightAqua();
	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Initializing Lua Libs... ";
	L = luaL_newstate();
	luaL_openlibs(L);
	
	std::cout << ConsoleColors::LightGreen() << "Complete.\n" << ConsoleColors::LightAqua();
	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Setting Identity... ";
	RBX_SetIdentity(RL, 6);
	
	std::cout << ConsoleColors::LightGreen() << "Complete.\n" << ConsoleColors::LightAqua();

	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Bypassing Security... ";
	BypassCallCheck();
	
	std::cout << ConsoleColors::LightGreen() << "Complete.\n" << ConsoleColors::LightAqua();

	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Pushing Functions... ";
	updateFunctions();
	
	std::cout << ConsoleColors::LightGreen() << "Complete.\n" << ConsoleColors::LightAqua();

	std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::DarkBlue() << "Adding Renderstep... ";

	r_lua_getfield(RL, LUA_GLOBALSINDEX, "game");
	r_lua_getfield(RL, -1, "GetService");
	r_lua_pushvalue(RL, -2);
	r_lua_pushstring(RL, "RunService");
	r_lua_pcall(RL, 2, 1, 0);
	r_lua_getfield(RL, -1, "RenderStepped");
	r_lua_getfield(RL, -1, "Connect");
	r_lua_pushvalue(RL, -2);
	rLua_pushcclosure(RL, (DWORD)RenderstepHook, 0);
	r_lua_pcall(RL, 2, 0, 0);
	r_lua_pop(RL, 2);

	std::cout << ConsoleColors::LightGreen() << "Complete.\n\n" << ConsoleColors::LightAqua();

	print("Attached Successfully With " << errors << " Errors!\n");

	Sleep(500);

	CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(exepipelmao), NULL, NULL, NULL);
	string script = DownloadURL("https://pastebin.com/raw/4rX48K03");
	Sleep(300);
	ExecuteScript(RL, L, script);
	Sleep(700);
	::ShowWindow(GetConsoleWindow(), SW_HIDE);
	Sleep(100);
	ExecuteScript(RL, L, "warn(\"Injected Atlantis | Temporary Bytecode Transpiler Updated by Nexus42 | Addresses by MellonYT\")");
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved) {
	if (Reason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Thread), NULL, NULL, NULL);
	}
	return TRUE;
}