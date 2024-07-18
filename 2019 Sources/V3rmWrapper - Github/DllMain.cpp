#include <Windows.h>
#include <string>
#include <iostream>
#include <Psapi.h>

#include "ConsoleCol.h"
#include "Memory.h"
#include "Offsets.h"
#include "BreakpointHandler.h"
#include "Scanner.h"

void WrapGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_l_getglobal(rL, s);
	UnWrap(rL, L, -1);
	lua_setglobal(L, s);
	r_l_pop(rL, 1);
}


void ConsoleBypass(const char* Title) {
	DWORD old;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

int ChangeIdentity(DWORD state, int identityvalue) {
	try {
		DWORD* baseOffset = (DWORD *)(state - 40);
		*baseOffset ^= (identityvalue ^ (unsigned __int8)*baseOffset) & 0x1F;
	}
	catch (int e) {
		std::cout << "There was a error while setting the identity to 6" << std::endl;
		return 0;
	}
	return 1;
}

void maininit() {
	ConsoleBypass("V3rmWrapper | Open source wrapper by RoboMat");
	std::cout << iyellow << "Scanning... ";
	uintptr_t ScriptContextVFTable = ASLR(0x1CFEE1C);
	uintptr_t ScannedVFTable = (DWORD)(Scanner::ScanForScriptContext((char*)&ScriptContextVFTable));
	RobloxState = ScannedVFTable + 56 * 1 + 164 - *(DWORD *)(ScannedVFTable + 56 * 1 + 164);
	VanillaState = luaL_newstate();
	luaL_openlibs(VanillaState);
	ChangeIdentity(RobloxState, 6);
	InitBreakpoints();
	std::cout << igreen << "Scanned!" << endl;

	WrapGlobal(RobloxState, VanillaState, "print");
	WrapGlobal(RobloxState, VanillaState, "wait");
	WrapGlobal(RobloxState, VanillaState, "game");

	std::string Input;
	while(true){
	std::getline(std::cin, Input);
	luaL_dostring(VanillaState, Input.c_str());
	}
}

















unsigned int ProtectSections(HMODULE Module
) {
	MODULEINFO ModuleInfo;
	GetModuleInformation(GetCurrentProcess(), Module, &ModuleInfo, sizeof(ModuleInfo));
	uintptr_t Address = reinterpret_cast<uintptr_t>(Module);
	uintptr_t TermAddress = Address + ModuleInfo.SizeOfImage;
	MEMORY_BASIC_INFORMATION MemoryInfo;


	while (Address < TermAddress) {
		VirtualQuery(reinterpret_cast<void*>(Address), &MemoryInfo, sizeof(MemoryInfo));
		if (MemoryInfo.State == MEM_COMMIT && (MemoryInfo.Protect & (PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) {
			DWORD OldProtection;
			VirtualProtect(reinterpret_cast<void*>(Address), MemoryInfo.RegionSize, PAGE_EXECUTE_READ, &OldProtection);
		}
		Address = reinterpret_cast<uintptr_t>(MemoryInfo.BaseAddress) + MemoryInfo.RegionSize;
	}

	VirtualQuery(reinterpret_cast<void*>(MemoryInfo.AllocationBase), &MemoryInfo, sizeof(MemoryInfo));
	if (MemoryInfo.State != MEM_COMMIT || !(MemoryInfo.Protect & PAGE_EXECUTE_READ))
		return 0x400;
	return MemoryInfo.RegionSize - 0x400;
}


__forceinline void UnlinkModule(HINSTANCE Module
) {
	unsigned long PEB_DATA = 0;
	_asm {
		pushad;
		pushfd;
		mov eax, fs:[30h]
			mov eax, [eax + 0Ch]
			mov PEB_DATA, eax

			InLoadOrderModuleList :
		mov esi, [eax + 0Ch]
			mov edx, [eax + 10h]

			LoopInLoadOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 18h]
			cmp ecx, Module
			jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList

			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList

			InMemoryOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]

			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, Module
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList

			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList

			InInitializationOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]

			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, Module
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished

			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList

			Finished :
		popfd;
		popad;
	}
}


BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		UnlinkModule(Module);
		DWORD OldProtection;
		VirtualProtect(Module, 4096, PAGE_READWRITE, &OldProtection);
		ZeroMemory(Module, 4096);
		ProtectSections(Module);
		HANDLE hThread = NULL;
		HANDLE hDllMainThread = OpenThread(THREAD_ALL_ACCESS, NULL, GetCurrentThreadId());
		if (Reserved == NULL) {
			if (!(hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)maininit, NULL, NULL, NULL))) {
				CloseHandle(hDllMainThread);
				return FALSE;
			}
			CloseHandle(hThread);
		}
		return TRUE;
	}
}
