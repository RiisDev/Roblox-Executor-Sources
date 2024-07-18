#include "Detours\detours.h"

void WINAPI RyseExitMessage(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
	int ans = MessageBoxExA(hWnd, "Ryse has caught a ROBLOX crash! Would you like to exit safely?", "Ryse Crash", MB_YESNO, NULL);
	if (ans == IDYES) { exit(0); }
}

void VEH_Bypass() {
	HMODULE mod = GetModuleHandleA("ntdll.dll");
	if (mod) {
		DWORD old;
		DWORD KIException = cast(DWORD, GetProcAddress(mod, "KiUserExceptionDispatcher"));
		VirtualProtect((LPVOID)KIException, 1, PAGE_EXECUTE_READWRITE, &old);
		for (int i = 0; i < 24; i++) {
			*(char*)(KIException + i) = 0x90;
		}
		VirtualProtect((LPVOID)KIException, 1, old, &old);
	}
}

DWORD LocateNTFunction(const PCHAR func) {
	return reinterpret_cast<DWORD>((GetProcAddress(GetModuleHandleA("ntdll.dll"), func)));
}

template <typename T>
void HookLocation(DWORD Address, T JumpLocation) {
	DWORD od;
	VirtualProtect((LPVOID)Address, 5, PAGE_EXECUTE_READWRITE, &od);
	*(BYTE*)(Address) = 0xE9;
	*(DWORD*)(Address + 1) = ((DWORD)JumpLocation) - Address - 5;
	VirtualProtect((LPVOID)Address, 5, od, &od);
}