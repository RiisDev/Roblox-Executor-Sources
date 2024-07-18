#include "main.h"
using namespace std;

int main(int argc, const char *argv[]) {
	/* THREADS */
	CreateThread(0, NULL, ThreadProc, (LPVOID)L"Cryztal - Developed by Tactical BFG", NULL, NULL);
	Sleep(10);

	DWORD asdmemes;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &asdmemes);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	
			std::cout << "Done!" << endl;
			

			getservice(L, "Players");
			
			luacputime = time(NULL);
			getfield(L, -1, "LocalPlayer");
			getfield(L, -1, "Name");
			string userID = toLstring(L, -1, NULL);
			myplr = userID.c_str();
			wstring playername(userID.length(), L' ');
			copy(userID.begin(), userID.end(), playername.begin());
			system("cls");
			copy(userID.begin(), userID.end(), playername.begin());
			cout << "Welcome to Cryztal, " << myplr << "Type 'mml' to load MakerModelLua Admin." endl;
	return 0;
}

void HookThread(void *entrypoint)
{
// ya its the 32 bit support thing but still
	bool MessageOn = true; 
	typedef int(__stdcall* MessageBoxKernel)(HWND, LPCSTR, LPCSTR, UINT);
	MessageBoxKernel MsgBoxK = (MessageBoxKernel)GetProcAddress(GetModuleHandleA("user32"), "MessageBoxA");
	DWORD size = MAX_PATH;
	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, false, GetCurrentProcessId());
	LPVOID mem = VirtualAllocEx(proc, NULL, size, MEM_COMMIT, PAGE_READWRITE);
	char pathbuff[MAX_PATH];
	GetModuleFileNameA(NULL, pathbuff, MAX_PATH);
	WriteProcessMemory(proc, mem, pathbuff, strlen(pathbuff), NULL);
	CreateRemoteThread(proc, NULL, NULL, (LPTHREAD_START_ROUTINE)entrypoint, mem, NULL, NULL);
	if (MessageOn == true)
	{
	
		VirtualFreeEx(proc, mem, size, MEM_FREE);
	}
	else
	{
		VirtualFreeEx(proc, mem, size, MEM_FREE);
	}
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		inj_hModule = hModule;
		HookThread(main);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}