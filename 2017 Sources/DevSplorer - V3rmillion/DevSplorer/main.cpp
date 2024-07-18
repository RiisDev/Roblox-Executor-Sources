#include <Windows.h>
#include <iostream>

extern int explorerinit(); //bitch find my form
using namespace std;


DWORD WINAPI Main_thread(LPVOID lpParam)
{
	explorerinit();
	return S_OK;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  _reason,
	LPVOID lpReserved
	)
{

	//we only care if it attaches
	if (_reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0x1000, &Main_thread, 0, 0, NULL);
	}
	return TRUE;
}

