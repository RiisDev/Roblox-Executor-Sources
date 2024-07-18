#include <Windows.h>
#include <string>
#include <iostream>
#include <Psapi.h>

#include "ConsoleCol.h"
#include "Offsets.h"
#include "Functions.h"

#include "LuaC/LuaC.h"
#include "LuaParser/LuaParser.h"
#include "Commands/Commands.h"

void PipeC() {
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\VyxilisLuaC"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';

				LuaCExecute(buffer);
			}
		}
		DisconnectNamedPipe(hPipe);
	}
}

void PipeLimLua() {
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\VyxilisLimitedLua"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';

				ExecuteParserScript(buffer);
			}
		}
		DisconnectNamedPipe(hPipe);
	}
}

void* placeHook(DWORD address, void* hook, bool revert = false) {
	DWORD oldprot;
	if (!revert) {
		void* oldmem = new void*;
		void* result = new void*;
		memcpy(oldmem, (void*)address, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		*(char*)address = 0xE9; *(DWORD*)(address + 1) = (DWORD)hook - address - 5;
		memcpy(result, oldmem, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
		return result;
	}
	else {
		VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		memcpy((void*)address, hook, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
		return NULL;
	}
}

DWORD RLSorg = 0;
DWORD hookaddr = 0x7F4CD0;

int gettopd(DWORD rState)
{
	RLSorg = rState;
	return (*(DWORD *)(rState + 24) - *(DWORD *)(rState + 12)) >> 4;
}

namespace Memory {
	bool compare(BYTE* address, BYTE* pattern, BYTE* mask) {
		for (; *mask; address++, pattern++, mask++) {
			if (*mask == 'x' && *address != *pattern) {
				return false;
			}
		}
		return true;
	}

	DWORD Scan(BYTE* aob, BYTE* mask, BYTE prot = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
		MEMORY_BASIC_INFORMATION mbi;
		DWORD j = (DWORD)GetModuleHandle(NULL);
		while (j < 0x7FFFFFFF && VirtualQuery((void*)j, &mbi, sizeof(mbi))) {
			if (!(mbi.Protect & PAGE_GUARD) && (mbi.State & MEM_COMMIT) && (mbi.Protect & prot)) {
				for (DWORD k = (DWORD)mbi.BaseAddress; k < ((DWORD)mbi.BaseAddress + mbi.RegionSize); ++k) {
					if (compare((BYTE*)k, (BYTE*)aob, (BYTE*)mask)) {
						return k;
					}
				}
			}
			j += mbi.RegionSize;
		}
		return 0;
	}
}

void ConsoleBypass(const char* Title) {
	DWORD superskiddoisachink;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &superskiddoisachink);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

int main() {
	ConsoleBypass("Vyxilis Rewritten | Lua-C & Limited Lua");
	std::cout << iyellow << "Scanning... ";

	void* old = placeHook(ASLR(hookaddr), gettopd);
	do { Sleep(1); } while (RLSorg == 0);
	placeHook(ASLR(hookaddr), old, 1);
	RLS = RLSorg;

	LS = luaL_newstate();
	luaL_openlibs(LS);

	std::cout << igreen << "success!\n";

	std::cout << iyellow << "Setting Level 6 Identity... ";
	*(DWORD*)Identity = 6;
	std::cout << igreen << "success!\n";

	std::cout << iyellow << "Setting up Lua C pipe... ";
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PipeC, NULL, NULL, NULL);
	std::cout << igreen << "success!\n";

	std::cout << iyellow << "Setting up Limited Lua pipe... ";
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)PipeLimLua, NULL, NULL, NULL);
	std::cout << igreen << "success!\n";

	std::cout << ipurple << "Welcome to Vyxilis, " << LuaCFunctions::GetLocalPlayerName() << ", enjoy!\n";
	std::cout << ipurple << "Thank you so much ringarang for helping me with this!\n";
	std::cout << white << "\n";

	while (true) {
		std::string into;
		std::getline(std::cin, into);
		ExecuteParserScript(into.c_str());
		LuaCExecute(into.c_str());
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
			if (!(hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL))) {
				CloseHandle(hDllMainThread);
				return FALSE;
			}
			CloseHandle(hThread);
		}
		return TRUE;
	}
}
