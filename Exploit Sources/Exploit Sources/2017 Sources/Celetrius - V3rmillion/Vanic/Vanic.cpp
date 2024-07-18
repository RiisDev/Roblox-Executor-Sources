#include "Vanic.h"

int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Vanic_Start, 0, 0, 0);
	}
	return TRUE;
}