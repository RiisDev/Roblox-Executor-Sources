#include "rlua_wrap.h"
#include "Memory.h"
#include "wrap_lua.h"
#include "detours.h"
#include "Memcheck.h"
#pragma comment(lib, "detours.lib")


unsigned long SkIdTab = aslr(0x6bb00e);





std::string Input() {
	std::string wotthefuck;
	getline(std::cin, wotthefuck);
	return wotthefuck;
}

typedef void(__cdecl *retcheckf)();
retcheckf retcheck = (retcheckf)ceaslr(0x1587F70);

void rektretcheck() {
	return;
}




void Main()
{
	DWORD OldPerm;
	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &OldPerm);
	*(BYTE*)&FreeConsole = 0xC3;

	AllocConsole();
	SetConsoleTitleA("Debug Console");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	
	DWORD SkIdT = *(DWORD*)(SkIdTab + 0x2);
	int ScriptContext = SkId::ScanforSkid((char*)&SkIdT);
	printf("ScriptContext: %x\n", ScriptContext);
	RobloxLuaState = *(DWORD*)(ScriptContext + 220) - (ScriptContext + 220);
	printf("LuaState: %x\n", RobloxLuaState);

	do
	{
		std::string x = Input();
		ExecuteScript(x);
	} while (true);

}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&Main, NULL, NULL, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}