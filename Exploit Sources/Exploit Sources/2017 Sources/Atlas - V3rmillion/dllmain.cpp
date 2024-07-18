
#include "stdafx.h"

BOOL WINAPI DllMain(HINSTANCE Dll, DWORD Reason, LPVOID Useless)
{
	if (Reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(Dll);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, NULL, NULL, NULL);
	}
	return TRUE;
}