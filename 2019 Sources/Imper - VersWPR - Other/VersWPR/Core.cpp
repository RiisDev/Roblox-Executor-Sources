#include "Authentication.h"
#include "Vitals.h"
#include "Open.h"
#include "Implementations.h"
#include "MainSecurity.h"

#include <cstdint>
#include <unordered_map>
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
#include <sstream>
#include <string>
#include <direct.h>
#include <intrin.h>
#include <fstream>
#include <WinInet.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#define VERSION "version-6b060bf0723c4a04"
#define identitySet() DWORD* identity  = (DWORD*)(rLS - 32); *identity ^= (6 ^ (unsigned __int8)*identity) & 0x1F;
std::string version = "http://setup.roblox.com/version";
#pragma comment(lib, "wininet.lib")
using namespace std;
Utils *Core;

void ghg(int a, bool debug)
{
	if (debug)
	{

	}
}

Utility *Core;

static int initializeEnvironment(lua_State *L) {
	//vital code, edit at your own risk.
	STRT* robloxEnv = emit(STRT*, (int*)(rLS + _OFF));
	Global->Table(rLS, Scope, rGlobal);
	Global->CacheInstance(rLS, Scope, rGlobal);
	delete Scope;
	return Global->Wrap(L, rLS, rGlobal);
}

void InitializeImper() {
	Core->Load();
	Core->OpenConsole();
	Core->DisableLogs();
	Authentication->Verify();
	Authentication->Status();
	Sleep(10);
	ScriptContext = Locate(VFT);

	rLS = openState(ScriptContext);

	identitySet();

	mLS = luaL_newstate();
	globalVariables::Initialize(mLS);
	globalTable(mLS, initializeEnvironment);
	globalVariables::InitializeImplements(mLS);

	Pipe->Start();
	Core->Untrace();
	Core->CloseConsole();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		HideModule(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Console, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)antiCheck, NULL, NULL, NULL);
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}