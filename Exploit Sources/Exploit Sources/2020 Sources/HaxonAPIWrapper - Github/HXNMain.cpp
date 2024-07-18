#include <string>
#include <windows.h>
#include <iostream>
#include "HXNWrap.h"

#include "CoreFunctions.h"

#include"EyeStep/rinstances.h"

using namespace std;















DWORD WINAPI HXN_PIPE(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;

	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\NOTEPAD_CyberBloxModule"),
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
			ExecuteScript(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}





namespace Memory22 {
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
		for (; *mask; ++aob, ++mask, ++location) {
			__try {
				if (*mask == 'x' && *location != *aob)
					return 0;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	}

	DWORD find_Pattern(DWORD size, BYTE* pattern, char* mask,
		BYTE protection = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
		SYSTEM_INFO SI = { 0 };
		GetSystemInfo(&SI);
		DWORD start = (DWORD)SI.lpMinimumApplicationAddress;
		DWORD end = (DWORD)SI.lpMaximumApplicationAddress;
		MEMORY_BASIC_INFORMATION mbi;
		while (start < end && VirtualQuery((void*)start, &mbi, sizeof(mbi))) {
			// Make sure the memory is committed, matches our protection, and isn't PAGE_GUARD.
			if ((mbi.State & MEM_COMMIT) && (mbi.Protect & protection) && !(mbi.Protect & PAGE_GUARD)) {
				// Scan all the memory in the region.
				for (DWORD i = (DWORD)mbi.BaseAddress; i < (DWORD)mbi.BaseAddress + mbi.RegionSize; ++i) {
					if (compare((BYTE*)i, pattern, mask)) {
						return i;
					}
				}
			}
			// Move onto the next region of memory.
			start += mbi.RegionSize;
		}
		return 0;
	}

	int Scan(DWORD mode, char* content, char* mask) {
		return find_Pattern(0x7FFFFFFF, (BYTE*)content, mask, mode);
	}
}




DWORD ScriptContext;



void HXN_MAININITTHREAD()
{



	DWORD consoleOldProtect = 0;
	VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &consoleOldProtect);
	*(UINT*)FreeConsole = 0xC2;
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	DWORD cons = (WS_CAPTION | DS_MODALFRAME | WS_MINIMIZEBOX | WS_SYSMENU);
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, cons);
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	SetConsoleTitleA("Hookah| Certified By ElKoax and Humanity");

	printf("Hookah, loading..\n");

	// so we are going to initilise out eyestep hooks.



	
	if (eyestep::use == 0)
	{
		printf("OOOPS!, we failed to initilise eyestep. \n");
		return;
	}
	else
	{
		printf("Eyestep has been initilised. \n");
		eyestep::use(GetCurrentProcess());
		printf("Eyestep thread (%x08)\n", eyestep::use);
	}


	if (util::debug == 0)
	{
		printf("Failed to Initilise the hooking utility. \n");
	}
	else
	{
		printf("Success, we are now going to hook onto luaH_new...");
                int fuckoff = 0;
		int HNEWHOOK = fuckoff;
		HXN_STATE = HNEWHOOK;
		printf("Done!\n");
		printf("Hook State! (%x08)\n", util::debug);
		printf("Roblox Hook State (%x08)\n", HXN_STATE);
	}


	
	


//	DWORD ScriptContextVFT_Addy = HXN_ASLR(0x1A75F84);
//	ScriptContext = Memory22::Scan(PAGE_READWRITE, (char*)& ScriptContextVFT_Addy, (char*)"xxxx");
//
//	DWORD _rLs = ScriptContext + 56 * 0 + 172 - *(DWORD*)(ScriptContext + 56 * 0 + 172);
	

	HXN_LSTATE = luaL_newstate();
	VehHandlerpush();
	luaL_openlibs(HXN_LSTATE);


	//HXN_STATE = _rLs;

	
	

		


	printf("Setting Up Hookah!\n");
	std::vector<std::string> Globals{
	"printidentity","game","Game","workspace","Workspace",
	"Axes","BrickColor","CFrame","Color3","ColorSequence","ColorSequenceKeypoint",
	"NumberRange","NumberSequence","NumberSequenceKeypoint","PhysicalProperties","Ray",
	"Rect","Region3","Region3int16","TweenInfo","UDim","UDim2",
	"Vector2", "Vector2int16","Vector3","Vector3int16","Enum","Faces",
	"Instance","math","warn","typeof","type","spawn","print",
	"printidentity","ypcall","Wait","wait","delay","Delay","tick","bit32"
	};
	for (auto func : Globals) {
		PushGlobal(HXN_STATE, HXN_LSTATE, func.c_str());
		//printf("Wrapped: \n", func);
		//std::cout << "Wrapped Global: " << func << std::endl;
	}



	lua_newtable(HXN_LSTATE);
	lua_setglobal(HXN_LSTATE, "_G");
	RegisterFunction();

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HXN_PIPE, NULL, NULL, NULL);


	modifyrbx();
	//luaL_dostring(HXN_LSTATE, "print('HaxonAPI Loaded!')");
	//luaL_dostring(HXN_LSTATE, "bit32 = bit");
	
	printf("Done Setting Up!\n");



    //Sleep(2000);
    //luaL_dostring(HXN_LSTATE, "HideConsole()");
	


	

   

	



	
	

	//return 0;
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


__forceinline void UnlinkModule(HINSTANCE Module) {
	unsigned long PEB_DATA = 0;
	_asm {
		pushad;
		pushfd;
		mov eax, fs: [30h]
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
			if (!(hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HXN_MAININITTHREAD, NULL, NULL, NULL))) {
				CloseHandle(hDllMainThread);
				return FALSE;
			}
			CloseHandle(hThread);
		}
		return TRUE;
	}
}
