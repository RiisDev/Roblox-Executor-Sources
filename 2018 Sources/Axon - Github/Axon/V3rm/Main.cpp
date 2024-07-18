
#include "globals.h"
#include "Bridge.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <fstream>
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")
using namespace std;

DWORD ScriptContext;
DWORD ScriptContextVFTable = x(0x114B4E0);

DWORD grabGlobalStateIndex(DWORD ScriptContext, int idx)
{
	DWORD* context = reinterpret_cast<DWORD*>(ScriptContext);
	return context[idx] - (DWORD)&context[idx];
}

using Bridge::m_rL;
using Bridge::m_L;

void PushGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Bridge::push(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
}

DWORD WINAPI input(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Axon"),
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
			if (luaL_loadstring(m_L, WholeScript.c_str()))
				printf("Error: %s\n", lua_tostring(m_L, -1));
			else
				lua_pcall(m_L, 0, 0, 0);

			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}


bool CompareData(const char* Data, const char* Pattern, const char* Mask) {
	while (*Mask) {
		if (*Mask != '?') {
			if (*Data != *Pattern) {
				return false;
			};
		};
		++Mask;
		++Data;
		++Pattern;
	};
	return true;
};


DWORD ScanForScriptContext(const char* SCVFT_Offsetted) {
	MEMORY_BASIC_INFORMATION BasicMemoryInformation = {};
	SYSTEM_INFO SystemInformation = {};
	GetSystemInfo(&SystemInformation);
	DWORD StartingMemorySearchPosition = (DWORD)SystemInformation.lpMinimumApplicationAddress;
	DWORD MaximumSearchBoundary = (DWORD)SystemInformation.lpMaximumApplicationAddress;
	do {
		while (VirtualQuery((void*)StartingMemorySearchPosition, &BasicMemoryInformation, sizeof(BasicMemoryInformation))) {
			if ((BasicMemoryInformation.Protect & PAGE_READWRITE) && !(BasicMemoryInformation.Protect & PAGE_GUARD)) {
				for (DWORD Key = (DWORD)(BasicMemoryInformation.BaseAddress); ((Key - (DWORD)(BasicMemoryInformation.BaseAddress) < BasicMemoryInformation.RegionSize)); ++Key) {
					if (CompareData((const char*)Key, SCVFT_Offsetted, "xxxx")) {
						return Key;
					};
				};
			};
			StartingMemorySearchPosition += BasicMemoryInformation.RegionSize;
		};
	} while (StartingMemorySearchPosition < MaximumSearchBoundary);
	return 0x0;
};

int getRawMetaTable(lua_State *L) {
	Bridge::push(L, m_rL, 1);

	if (r_lua_getmetatable(m_rL, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Bridge::push(m_rL, L, -1);

	return 1;
}

static int UserDataGC(lua_State *Thread) {
	void *UD = lua_touserdata(Thread, 1);
	if (m_rL) {

		r_lua_rawgeti(m_rL, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(m_rL, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);

		}
	}
	return 0;
}
void main()
{
	ScriptContext = ScanForScriptContext((char*)&ScriptContextVFTable);
	m_rL = grabGlobalStateIndex(ScriptContext, 41);
	m_L = luaL_newstate();
	Bridge::VehHandlerpush();
	luaL_openlibs(m_L);
	luaL_newmetatable(m_L, "garbagecollector");
	lua_pushcfunction(m_L, UserDataGC);
	lua_setfield(m_L, -2, "__gc");
	lua_pushvalue(m_L, -1);
	lua_setfield(m_L, -2, "__index");
	PushGlobal(m_rL, m_L, "game");
	PushGlobal(m_rL, m_L, "Game");
	PushGlobal(m_rL, m_L, "workspace");
	PushGlobal(m_rL, m_L, "Workspace");
	PushGlobal(m_rL, m_L, "Axes");
	PushGlobal(m_rL, m_L, "BrickColor");
	PushGlobal(m_rL, m_L, "CFrame");
	PushGlobal(m_rL, m_L, "Color3");
	PushGlobal(m_rL, m_L, "ColorSequence");
	PushGlobal(m_rL, m_L, "ColorSequenceKeypoint");
	PushGlobal(m_rL, m_L, "NumberRange");
	PushGlobal(m_rL, m_L, "NumberSequence");
	PushGlobal(m_rL, m_L, "NumberSequenceKeypoint");
	PushGlobal(m_rL, m_L, "PhysicalProperties");
	PushGlobal(m_rL, m_L, "Ray");
	PushGlobal(m_rL, m_L, "Rect");
	PushGlobal(m_rL, m_L, "Region3");
	PushGlobal(m_rL, m_L, "Region3int16");
	PushGlobal(m_rL, m_L, "TweenInfo");
	PushGlobal(m_rL, m_L, "UDim");
	PushGlobal(m_rL, m_L, "UDim2");
	PushGlobal(m_rL, m_L, "Vector2");
	PushGlobal(m_rL, m_L, "Vector2int16");
	PushGlobal(m_rL, m_L, "Vector3");
	PushGlobal(m_rL, m_L, "Vector3int16");
	PushGlobal(m_rL, m_L, "Enum");
	PushGlobal(m_rL, m_L, "Faces");
	PushGlobal(m_rL, m_L, "Instance");
	PushGlobal(m_rL, m_L, "math");
	PushGlobal(m_rL, m_L, "warn");
	PushGlobal(m_rL, m_L, "typeof");
	PushGlobal(m_rL, m_L, "type");
	PushGlobal(m_rL, m_L, "spawn");
	PushGlobal(m_rL, m_L, "Spawn");
	PushGlobal(m_rL, m_L, "print");
	PushGlobal(m_rL, m_L, "printidentity");
	PushGlobal(m_rL, m_L, "ypcall");
	PushGlobal(m_rL, m_L, "Wait");
	PushGlobal(m_rL, m_L, "wait");
	PushGlobal(m_rL, m_L, "delay");
	PushGlobal(m_rL, m_L, "Delay");
	PushGlobal(m_rL, m_L, "tick");
	PushGlobal(m_rL, m_L, "LoadLibrary");
	lua_register(m_L, "getrawmetatable", getRawMetaTable);
	lua_newtable(m_L);
	lua_setglobal(m_L, "_G");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
	printf("RVX INJECTED!\n");
	MessageBoxA(NULL, "Africanus is better than you\nKai fucked me on my house :D\nAero is Gay\nSettings bombed a school\n<Aspect> bditt is less gay than Pudding Mug\nxGladius is less gay than Kai but still true gay love\nTrapFX is a weeb", "The Truth", MB_OK);
}



BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default: break;
	}

	return TRUE;
}