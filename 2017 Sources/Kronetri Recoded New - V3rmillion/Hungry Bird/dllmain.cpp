#include <Windows.h>
#include "main.h"


int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Intilization, NULL, NULL, NULL);
	}
	return TRUE;
}