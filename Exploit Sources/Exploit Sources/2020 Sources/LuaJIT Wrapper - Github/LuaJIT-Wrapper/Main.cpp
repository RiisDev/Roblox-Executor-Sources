#include "LuaWrapper.h"
#include <string>
#include <iostream>
#include <vector>
#include "Drawing.hpp"
#include "Client.h"
#include "Obfuscation.hpp"
#include "MsgBoxHook.hpp"
#include "TaintedBypass.hpp"
#include "RbxFunctions.hpp"
#pragma comment(lib, "wininet.lib")


using LuaWrapper::m_rL;
using LuaWrapper::m_L;

// no this is definitely not axon you shut ur mouth

/*
* Halting updates
* I've seen alot of errors and stuff in my code and I've decided to leave it like this for a while
* I'll be finishing my C++ course and will come back when I have a level of mastery in C++
*/

void Execute(std::string Script) {
	Script = "spawn(function() script=Instance.new(\"LocalScript\") " + Script + "\r\nend)";
	if (luaL_loadbuffer(m_L, Script.c_str(), Script.size(), RAND_STR(5).c_str())) /* simple call obfuscation */
	{
		r_lua_getglobal(m_rL, "warn");
		r_lua_pushstring(m_rL, lua_tostring(m_L, -1));
		r_lua_pcall(m_rL, 1, 0, 0);
		return;
	}
	else
	{
		lua_pcall(m_L, 0, 0, 0);
		
	}
	UserDataGC(m_L);
}

DWORD WINAPI LuaPipe(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	OBFUSCATE_STR(a1,"\\\\.\\pipe\\LuaJIT");
	string lpipe = ("\\\\.\\pipe\\JIT"); /* simple pipe hiding technique */	
	hPipe = CreateNamedPipe(TEXT(a1.decrypt()),
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
			Execute(WholeScript);

			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

void SetRBXLuaState()
{
	static DWORD SC;	
	SC = FindFirstClass(ReturnDataModel(), "ScriptContext");
	m_rL = (SC + 164) + *(DWORD*)(SC + 164);
	*(DWORD*)(*(DWORD*)(m_rL + 108) + 24) = 7;
}

void WrapGlobals()
{
	std::vector<const char*> Globals = {
	"game","Game","workspace","Workspace","printidentity",
	"Axes","BrickColor","CFrame","Color3","ColorSequence","ColorSequenceKeypoint",
	"NumberRange","NumberSequence","NumberSequenceKeypoint","PhysicalProperties","Ray",
	"Rect","Region3","Region3int16","TweenInfo","UDim","UDim2","Vector2",
	"Vector2int16","Vector3","Vector3int16","Enum","Faces",
	"Instance","math","warn","typeof","type","print",
	"printidentity","ypcall","Wait","wait","delay","Delay","tick", "pcall", "spawn", "Spawn"
	};
	for (int i = 0; i < Globals.size(); i++)
	{
		r_lua_getglobal(m_rL, Globals[i]);
		LuaWrapper::Wrap(m_rL, m_L, -1);
		lua_setglobal(m_L, Globals[i]);
		r_lua_pop(m_rL, 1);
	}
	printf("Globals wrapped!\n");
}

void main()
{
	ConsoleBypass("LuaJIT Wrapper");
	printf("Getting DataModel..\n");
	DataModel = ReturnDataModel();
	printf("Setting LuaState..\n");
	SetRBXLuaState();
	m_L = luaL_newstate();
	printf("Loading bypasses..\n");
	IsTaintedBypass();
	LoadLogBypass();
	LuaWrapper::VehHandlerpush();
	printf("Loading libraries..\n");
	luaL_openlibs(m_L);
	printf("Wrapping Globals..\n");
	WrapGlobals();
	DrawingAPI::InitiateDrawingAPI(m_L);
	Register(m_L);
	lua_newtable(m_L);
	lua_setglobal(m_L, "_G");
	lua_setglobal(m_L, "shared");
	printf("Loaded Lua functions..\n");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LuaPipe, NULL, NULL, NULL);
	printf("Injected!");
	string player = GetPlayerName();
	std::cout << "Welcome, " + player << endl;
	SetClientName("Roblox Lua Just-In-Time Client");
}

void TeleportHandler()
{
	main();
	while (true)
	{
		if (DataModel != ReturnDataModel())
		{
			Sleep(1);
			main();
			std::cout << "Reloaded module!" << endl;
		}
		Sleep(1);
	}
}


BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, LPVOID) {
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		DWORD OldProtection;
		VirtualProtect(Module, 0x1000, PAGE_READWRITE, &OldProtection);
		ZeroMemory(Module, 0x1000);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)TeleportHandler, NULL, NULL, NULL);
		break;
	}
	return TRUE;
}