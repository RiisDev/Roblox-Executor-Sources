#include "Convert.h"

DWORD Renv = 0;

DWORD WINAPI input(PVOID lvpParameter)
{
	std::string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\NotTrollicusWrapper"),
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

			auto* L = luaL_newstate();
			DWORD luaState = RLua.lua_newthread(Renv);
		
			int identity[] = { 6 };
			int unk[] = { 0, 0 };
			Sandbox(luaState, identity, unk);
			zx3::Conversion::ExecuteScript(L, luaState, WholeScript);

			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}


const char* GetClass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}


int FindFirstClass(int Instance, const char* Name)
{
	DWORD CHILD_START = *(DWORD*)(Instance + 0x2C);
	DWORD CHILD_END = *(DWORD*)(CHILD_START + 4);

	for (int i = *(int*)CHILD_START; i != CHILD_END; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}



using gmDataModelFn = void* (__stdcall*)(void* md);
extern gmDataModelFn GetDataModel;

gmDataModelFn GetDataModel = reinterpret_cast<gmDataModelFn>(SignatureFind::Addresses::RobloxBase(0xe0eed0));

void maininit()
{
	auto* L = luaL_newstate();


	*(BYTE*)SignatureFind::Addresses::RobloxBase(0x2071FA4) = 0;



	unsigned char r_dm[8];

	int dm = *reinterpret_cast<std::uintptr_t*>(GetDataModel(r_dm)) + 0x44;//*reinterpret_cast<uintptr_t*>(GetDataModel(r_dm)) + 0x44;
	int ScriptContext = FindFirstClass(dm, "ScriptContext");


	int v2 = ScriptContext;
	int v3 = 0;

	Renv = *(DWORD*)(v2 + 56 * v3 + 172) - (v2 + 56 * v3 + 172);
	int newthread = RLua.lua_newthread(Renv);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)input, nullptr, 0, nullptr);

	std::string source;
	while (std::getline(std::cin, source)) {
		zx3::Conversion::ExecuteScript(L, newthread, source);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		DWORD c = 0;
		VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &c);
		*reinterpret_cast<UINT*>(FreeConsole) = 0xC3;
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)maininit, nullptr, 0, nullptr);
	}
	return 1;
}