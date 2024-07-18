#include "Wrapper.h"
#include "Rlua.h"
#include "Scanner.h"
#include "Cloak.h"
#include "CoreFunctions.h"
#include "xorstring.h"
#include "mouselib.h"
#include "ConsoleColors.h"
#include "DebugFunctions.h"
#include <shellapi.h>
#include <functional>
#include "Bit.h"
#include "Roblox.hpp"
#include "DrawingAPI.hpp"
#include "utf8.h"
#include <string>
#include "TestFunctions.h"
using namespace std;

// =====================================================================================================
//  _____                _                  _______        _                 _                   
// |  __ \              | |                |__   __|      | |               | |                  
// | |__) |_ _ _ __   __| | __ _   ______     | | ___  ___| |__  _ __   ___ | | ___   __ _ _   _ 
// |  ___/ _` | '_ \ / _` |/ _` | |______|    | |/ _ \/ __| '_ \| '_ \ / _ \| |/ _ \ / _` | | | |
// | |  | (_| | | | | (_| | (_| |             | |  __/ (__| | | | | | | (_) | | (_) | (_| | |_| |
// |_|   \__,_|_| |_|\__,_|\__,_|             |_|\___|\___|_| |_|_| |_|\___/|_|\___/ \__, |\__, |
//                                                                                    __/ | __/ |
//                                                                                   |___/ |___/ 
// =====================================================================================================

string Copyright = "Panda-Technology";

DWORD locateINT3() {
	DWORD _s = ScanAddress(0x400000);
	const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
	for (int i = 0; i < MAX_INT; i++)
		if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0)
			return (_s + i);

	return NULL;
}
DWORD locateINT31() {
	DWORD _s = ScanAddress(0x400000);
	const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC };
	for (int i = 0; i < MAX_INT; i++)
		if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0)
			return (_s + i);

	return NULL;
}

const char* GetClass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

template<class Ret, class... Args>
std::function<Ret(Args...)> Call(DWORD Address, std::string Convention)
{
	if (Convention == "Stdcall")
	{
		typedef Ret(__stdcall* Fn_t)(Args...);
		return (Fn_t)Address;
	}
	else if (Convention == "Cdecl")
	{
		typedef Ret(__cdecl* Fn_t)(Args...);
		return (Fn_t)Address;
	}
	else if (Convention == "Fastcall")
	{
		typedef Ret(__fastcall* Fn_t)(Args...);
		return (Fn_t)Address;
	}
	else if (Convention == "Thiscall")
	{
		typedef Ret(__thiscall* Fn_t)(Args...);
		return (Fn_t)Address;
	}
}

//typedef int(__thiscall* findfirstchild)(DWORD, const std::string&);
//findfirstchild FindFirstClass = (findfirstchild)(unprotect(ScanAddress(0x60D860)));
int FindFirstClass(int Instance, const char* Name)
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}


DWORD DataModel;
using getDataModel_t = void(__thiscall*)(DWORD, DWORD); //Conversion
using getDataModel_tt = DWORD(__cdecl*)();


// Address for GetData 1 & 2 //

DWORD getdata1 = Adresses::getdatamodel_addr;
DWORD getdata12 = Adresses::getdatamodel2_addr;


uintptr_t GetDataModel()
{
	std::string getdata1CCV = "__thiscall";
	std::string getdata2CCV = "__cdecl";

	volatile DWORD StackPad[16]{};
	static DWORD DMPad[16]{};
	auto Function = Call<DWORD, DWORD, DWORD>(ScanAddress(getdata1), getdata1CCV); //Conversion for GetData1
	auto Function2 = Call<DWORD>(ScanAddress(getdata12), getdata2CCV);  //Conversion for GetData2
	Function(Function2(), (DWORD)DMPad);
	DWORD DM = DMPad[0];
	return DM + 0xC; //old panda wrapper
}

void UpdateIdentity(DWORD L, DWORD sandboxthread, DWORD rL) { 
	BYTE* FByte = (BYTE*)sandboxthread;
	int FSize = 240;
	DWORD FIRST;
	DWORD SECOND;
	int firsted = 0;
	int seconded = 0;
	DWORD s = 0x0;
	for (int i = 0; i < FSize; i++) {
		s++;
		if (FByte[i - 2] == 0x5 && FByte[i - 1] == 0x8B)
        {
            if (firsted == 0)
            {
				FIRST = FByte[i + 1]; //Because its the first result but the if return true multiple times...
                firsted = 1;
            }

        }
        if (FByte[i] == 0xF && FByte[i + 1] == 0x11)
        {
			SECOND = FByte[i + 3];
        }
	}
	std::cout << white << "[" << green << "x" << white << "]" << white << " [Client] -> Identity Started! -> " << std::hex << "0x" << FIRST << " + 0x" << std::hex << SECOND << " = 6" << std::endl;
	DWORD a1 = L;
	int v3 = *(DWORD*)(a1 + FIRST);
	*(DWORD*)(v3 + SECOND) = rL;
}

void Scan() {
	std::cout << white << "[" << green << "x" << white << "]" << white << " [Client] -> DataModel Started!" << endl;
	
	uintptr_t GDM;
	GDM = GetDataModel();
	uintptr_t ScriptContext;
	ScriptContext = FindFirstClass(GDM, "ScriptContext");
	luaState = State(ScriptContext, 1);

	std::cout << white << "[" << green << "x" << white << "]" << white << " [Client] -> BreakPoints Bypassed!"  << endl;
	
	RobloxThread = rlua_newthread(luaState);

	int3breakpoint = Scanner::AOB::FindPattern("\xCC\xCC\xCC\xCC", "xxxx");
	int3breakpoint1 = Scanner::AOB::FindPattern("\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC", "xxxxxxxxxxxxxxxx");
	AddVectoredExceptionHandler(1, int3cbreakpoint);

	UpdateIdentity(luaState, sandboxthreadoffset, 7);

}

void DebugConsole()
{
	DWORD OldPerm;
	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &OldPerm);
	*reinterpret_cast<BYTE*>(&FreeConsole) = 0xC3;
	VirtualProtect(&FreeConsole, 1, OldPerm, NULL);
	AllocConsole();
	SetConsoleTitle("Panda Gen 3.4 Wrapper [ Initializing Modules ]");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::cout << iblue << R"(

  _____                _                  _______        _                 _                    
 |  __ \              | |                |__   __|      | |               | |                   
 | |__) |_ _ _ __   __| | __ _   ______     | | ___  ___| |__  _ __   ___ | | ___   __ _ _   _  
 |  ___/ _` | '_ \ / _` |/ _` | |______|    | |/ _ \/ __| '_ \| '_ \ / _ \| |/ _ \ / _` | | | | 
 | |  | (_| | | | | (_| | (_| |             | |  __/ (__| | | | | | | (_) | | (_) | (_| | |_| | 
 |_|   \__,_|_| |_|\__,_|\__,_|             |_|\___|\___|_| |_|_| |_|\___/|_|\___/ \__, |\__, | 
                                                                                    __/ | __/ | 
                                                                                   |___/ |___/                                                                                                                                                                                                           
//)" << '\n';
	std::cout << iblue << "[" << red << " Panda Client " << iblue << "]" << igreen << " " << "__________________________________________________\n";
	std::cout << iblue << "[" << red << " Panda Client " << iblue << "]" << iblue << " " << " Initializing Exploit \r\n";
}

DWORD WINAPI InputLua(PVOID lvpParameter)
{
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	XorS(ObfuscateString, "\\\\.\\pipe\\MYPPISHARD"); //PandaOfficialWrapper ;
	hPipe = CreateNamedPipe(TEXT(ObfuscateString.decrypt()),
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
				ExecuteScript(buffer);
			}
		}
		DisconnectNamedPipe(hPipe);
	}
}

void TeleportHandler()
{

	int ls = luaState;
	bool state = false;
	while (true) {
		Sleep(3000);
		Scan();
		if (ls != luaState)
			if (!state) state = true;
			else
				if (state)
				{
					state = false;
					GetGlobals();
					OpenCustomFunctions(Vanilla);
					RegisterDebugFunctions(Vanilla);
					UpdateIdentity(luaState, sandboxthreadoffset, 7);
				}
	}
}

void Main() {
	DebugConsole();
	Timer::Start();
	Scan();
	if (!Vanilla) {
		Vanilla = lua_open();
		luaL_openlibs(Vanilla);
		std::cout << white << "[" << green << "x" << white << "]" << white << " [Client] -> Wrapping Globals!" << endl;
		GetGlobals();
		std::cout << white << "[" << green << "x" << white << "]" << white << " [Client] -> Running Functions to instance!" << endl;
		luaopen_bit(Vanilla);
		OpenCustomFunctions(Vanilla);	
		RegisterDebugFunctions(Vanilla);
	}
	SetThreadPointer(Vanilla, RobloxThread);
	ExecuteScript(R"(
local mt = getrawmetatable(game)
    local old = mt.__index
    make_writeable(mt)
    mt.__index = newcclosure(function(t, k)
        if checkcaller() then
            if k == "HttpGet" then
                return HttpGet
            elseif k == "HttpGetAsync" then
                return HttpGet    
            elseif k == "GetObjects" then
                return GetObjects
            end    
        end
        return old(t, k)
    end)
    )"); //Lmfao the Init Script
	ExecuteScript(DownloadURL("https://raw.githubusercontent.com/SkieAdmin/PandaHub/master/PandaWelcome.lua").c_str()); //Panda Official
	ExecuteScript("game.StarterGui:SetCore('SendNotification', {Duration = 4; Title='Exploit Loaded'; Text='Successfully Loaded'})"); // No Reason to Add
	DrawingAPI::InitiateDrawingAPI(Vanilla);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InputLua, NULL, NULL, NULL);
}



unsigned int ProtectSections(HMODULE Module) {
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
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)erase_peheader, NULL, NULL, NULL);
		ZeroMemory(Module, 4096);
		ProtectSections(Module);
		HANDLE hThread = NULL;
		HANDLE hDllMainThread = OpenThread(THREAD_ALL_ACCESS, NULL, GetCurrentThreadId());
		if (Reserved == NULL) {
			if (!(hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, NULL, NULL, NULL))) {
				CloseHandle(hDllMainThread);
				return FALSE;
			}
			if (!(hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)TeleportHandler, NULL, NULL, NULL))) {
				CloseHandle(hDllMainThread);
				return FALSE;
			}
		}	
		return TRUE;
	}
}
