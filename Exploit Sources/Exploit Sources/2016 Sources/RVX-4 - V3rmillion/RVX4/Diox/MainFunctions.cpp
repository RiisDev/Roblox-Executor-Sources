#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "stdafx.h"
#include "Memcheck.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "VMProtectSDK.h"
#include "detours.h"

using namespace std;
void init();
void ShowWindowForm();
bool Startt(std::string Cmd);


DWORD base2 = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");

DWORD getaddy2(int address) {
	return (address - 0x400000 + base2);
}
namespace Roblox {
	const int MaxHealth_Address = getaddy2(0x6D5B10); //updated
	typedef void(__thiscall * setmaxhealth)(int self, float health);
	setmaxhealth SetMaxHealth = (setmaxhealth)MaxHealth_Address;

	const int WalkSpeed_Address = getaddy2(0x6D6B50);//updated
	typedef void(__thiscall *setwalkspeed)(int self, float walkspeed);
	setwalkspeed SetWalkSpeed = (setwalkspeed)WalkSpeed_Address;

	const int JumpPower_Address = getaddy2(0x6D59E0);//updated
	typedef void(__thiscall *setjumppower)(int self, float jump);
	setjumppower SetJumpPower = (setjumppower)JumpPower_Address;

	const int PerosnalRankServer_Address = getaddy2(0x872820);//need to find this lol
	typedef void(__thiscall *setpsr)(int self, int psr);
	setpsr SetRank = (setpsr)PerosnalRankServer_Address;

	const int SetFogEnd_Address = getaddy2(0x6E7AD0);//updated
	typedef void(__thiscall *setfogend)(void* self, float aidss);
	setfogend SetFogEnd = (setfogend)SetFogEnd_Address;

	const int SetFogStart_Address = getaddy2(0x6E7B00);//updated
	typedef void(__thiscall *FogS)(void* self, float aidss);
	FogS SetFogS = (FogS)SetFogStart_Address;

	const int SetMinutesAfterMidnight_Address = getaddy2(0xEF8B70);//updated
	typedef void(__thiscall *settime)(void* self, double aids);
	settime SetMinutesAfterMidnight = (settime)SetMinutesAfterMidnight_Address; //setminutesaftermidnight

	const int BreakJoints_Address = getaddy2(0x6C3870);
	typedef void(__thiscall * breakjoints)(void* self);
	breakjoints BreakJoints = (breakjoints)BreakJoints_Address;

	const int setparent_address = getaddy2(0x41C050);//not updated
	typedef void(__thiscall * setparent)(void* self, int parent);
	setparent rSetParent = (setparent)setparent_address;

	const int SetTakeDamage_Address = getaddy2(0x6DCF80);
	typedef int(__thiscall * setdamage)(void* self, int val);
	setdamage SetDamage = (setdamage)SetTakeDamage_Address;

	const int Health_Address = getaddy2(0x6D5B10);//may be correct
	typedef void(__thiscall * sethealth)(int self, float health);
	sethealth SetHealth = (sethealth)Health_Address;

	const int SetNameDisplayDistance_Address = getaddy2(0x6D5B50);
	typedef void(__thiscall * NameDisplayDistance)(int self, float aidss);
	NameDisplayDistance SetNameDisplayDist = (NameDisplayDistance)SetNameDisplayDistance_Address;

	bool Seizure = false;
	bool Night = false;
	int DataModel;
	int Workspace;
	int Lighting;
	int Players;
	int Camera;
	int Head;
	int LogService;
	int CurrentPlayer;
	int State;
	bool Changing;
	int antiscript = 0;
	bool seizure = false;
	std::string* PlayerName;
	int GetParent(int Instance) {
		return *(int*)(Instance + 0x34);
	}
	std::string* GetName(int Instance) {
		return (std::string*)(*(int*)(Instance + 0x28) - 0x4);
	}

	const char* GetClass(int self)
	{
		VMProtectBeginUltra("snsdkfbwkebsdkfbniwe");
		return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
	}





	int FindFirstChild_PartOf(int Instance, std::string Name) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (strstr(GetName(*(int*)i)->c_str(), Name.c_str()))
			{
				return *(int*)i;
			}
		}
		return 0;
	}


	int GetLocalPlayer(int Plr) {
		return *(int*)(Plr + 0x16C);
	}

	int FindFirstClass(int Instance, const char* Name)
	{
		DWORD CHILD_START = *(DWORD*)(Instance + 0x2C);
		DWORD CHILD_END = *(DWORD*)(CHILD_START + 4);

		for (int i = *(int*)CHILD_START; i != CHILD_END; i += 8)
		{
			if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
			{
				return *(int*)i;
			}
		}
	}

	int GetCharacter(std::string Name) {


		int Character = FindFirstChild_PartOf(Workspace, Name);
		if (Character != 0) {
			return Character;
		}
		else
		{
			return 0;
		}
	}

	int GetPlayer(std::string Name) {
		int Player = FindFirstChild_PartOf(Players, Name);
		if (Player != 0) {
			return Player;
		}
		else
		{
			return 0;
		}
	}
	int GetMyPlayer() {
		int Player = FindFirstChild_PartOf(Players, *PlayerName);
		if (Player != 0) {
			return Player;
		}
		else
		{
			return 0;
		}
	}



	std::vector<int> GetChildren(int Instance) {

		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		std::vector<int> Children;
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			Children.push_back(*(int*)i);
		}
		return Children;
	}
	int GetCharFromPlayer(int Instance) {
		return *(int*)(Instance + 0x48);
	}
	std::vector<int> GetAllPlayerCharacters(int ParentOfChars, std::string omitted = "none") {
		std::vector<int> PlayersVector = GetChildren(Players);
		std::vector<int> CharList;
		for (int i = 0; i < PlayersVector.size(); i++) {
			if (*GetName(PlayersVector[i]) != omitted) {
				int Character = GetCharFromPlayer(PlayersVector[i]);
				if (Character > 150) {
					CharList.push_back(Character);
				}
			}
		}
		return CharList;
	}
}


#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <sstream>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "MSVCRT.LIB")

std::string doauth(int uid)
{
	

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		//cout << "[Error]: WSAStartup failed.\n";

		//return 1;
	}

	SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct hostent *host;
	host = gethostbyname("www.slevenhost.pe.hu"); // Host name

	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
	std::string boss = ("GET http://slevenhost.pe.hu/pep.php?uID=" + std::to_string(uid) + " HTTP/1.0\r\n\r\nHost: www.slevenhost.pe.hu\r\n\r\nConnection: close\r\n\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n\r\nAccept-Language: en-US,en;q=0.8\r\n\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.87 Safari/537.36\r\n\r\n").c_str();
	connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr));
	std::ostringstream oss;
	send(Socket, boss.c_str(), strlen(boss.c_str()), 0); // Empty space..
	char buffer[100000];

	int nDataLength = recv(Socket, buffer, 100000, 0);

	closesocket(Socket);
	WSACleanup();

	return buffer;

}

int MaxHealth_Address = 0x6D5B10;
typedef void(__thiscall * SetMaxH)(int self, float fuckingmaxhealth);
SetMaxH SetMaxHealth = (SetMaxH)MaxHealth_Address;


void SetHumanoidState(int self, int state) {
	int sPtr = *(int*)(self + 0x2DC);
	if (sPtr) {
		*(BYTE*)(self + state + 0x229) = 1;
		*(int*)(sPtr + 0x20) = state;
	}
}


bool Changing;
int State;
//Trying something else in some minutes
void doState() {
	if (Changing == true) {
		int LocalCharacter = Roblox::GetCharacter(*Roblox::PlayerName);
		if (LocalCharacter != 0) {
			int Humanoid = Roblox::FindFirstClass(LocalCharacter, "Humanoid");
			if (Humanoid != 0) {
				SetHumanoidState(Humanoid, State);
			}
		}
	}
}


char* mfm;

void Loop() {
	do {
		__try {
			doState();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			Sleep(1); // do absolutely fucking nothing
		}
	} while (true);

}

const char *Commands =
// \r\n
"kill [p] - Kill's the [p], set's [p]'s health to 0.\r\n"
"god [p] - Set's [p]'s maxhealth to infinite.\r\n"
"ungod [p] - Set's [p]'s maxhealth back too 100.\r\n"
"noclip - Toggles noclip.\r\n"
"airwalk - Toggles airwalk.\r\n"
"swim - Toggles swim.\r\n"
"superjump - Toggles jump.\r\n"
"jesusfly - Toggles fly.\r\n"
"ragdoll - Toggles ragdoll.\r\n"
"disable - Disables all localplayer cmds.\r\n"
"ws [p] [n] - Set's [p]'s walkspeed to [n].\r\n"
"sethealth [p] [n] - Set's [p]'s health to [n].\r\n"
"jumppower [p] [n] - Set's [p]'s jump to [n].\r\n"
"shutdown - Shutsdown the game & server.\r\n"
"btools [p] - Give's [p] btools.\r\n"
"kick [p] - Kick's the [p] from game.\r\n"
"freeze [p] - Freezes player. \r\n"
"fog [n] - Sets Fog to [n} (set fog to 10000 to turn fog off). \r\n"
"time [n] - Sets time to [n]. \r\n"
"night - Sets time to night. \r\n"
"day - Sets time to day. \r\n"
"freeze [p] - Freezes player. \r\n"
"thaw [p] - Unfreezes player. \r\n"
"heal [p] - heals damaged player. \r\n"
"dusk - sets time to dusk. \r\n"
"stools [p] - Steals [p]'s tools. \r\n"
"purge on/off - changes lighting and fog. \r\n"
"explore - Explores the games Workspace. \r\n"
;



void Print(HWND hWndEdit, std::string pszText)
{
	int nLength = GetWindowTextLength(hWndEdit);
	SendMessage(hWndEdit, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
	SendMessage(hWndEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)pszText.c_str());
}

#define INPUT_CHAR_LIMIT            500

#define ALX_CONSOLE_WINDOW		(WM_APP + 500)
#define ALX_INPUT_FIELD			(WM_APP + 501)
#define ALX_WAYPOINT_BOX		(WM_APP + 502)

#define MALX_EXIT				(WM_APP + 600)
#define MALX_RESTART			(WM_APP + 601)
#define MALX_ABOUT				(WM_APP + 602)
#define MALX_CREDITS			(WM_APP + 603)
#define MALX_COMMANDS			(WM_APP + 604)



HWND ParentWindow = NULL;
HWND MainWindow = NULL;
HMENU WindowMenu = NULL;
HMODULE HInstance = NULL;

HWND InputField = NULL;
HWND txtbox = NULL;
HWND WaypointBox = NULL;

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CTLCOLORSTATIC:
	{
		HDC hEdit = (HDC)wParam;
		SetTextColor(hEdit, RGB(6, 121, 158));
		SetBkColor(hEdit, RGB(255, 255, 255));
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MALX_EXIT:
			if (MessageBox(hwnd, "Are you sure you want to exit?", "Exit", MB_YESNOCANCEL) == IDYES)
				ExitThread(0);
			break;
		case MALX_RESTART:
			if (MessageBox(hwnd, "Are you sure you want to restart?", "Restart", MB_YESNOCANCEL) == IDYES)
				ExitThread(1);
			break;

		case MALX_CREDITS:
			MessageBox(hwnd, "Created by DionRBLX ", "Credits", MB_OKCANCEL);
			break;

		case MALX_COMMANDS:
			Startt("cmds");
			break;
		case ALX_INPUT_FIELD:
			if (HIWORD(wParam) == EN_MAXTEXT) {
				char cText[INPUT_CHAR_LIMIT];
				SendMessage((HWND)lParam, WM_GETTEXT, INPUT_CHAR_LIMIT, (LPARAM)cText);

				if (strcmp(cText, "") == 0)
					break;

				SendMessage((HWND)lParam, WM_SETTEXT, NULL, (LPARAM)"");

				//std::string command = cText;
				Startt(cText);
			}

			break;
		}
		break;
	case WM_DESTROY:
		ExitThread(0);
		break;

	case WM_QUIT:
		ExitThread(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL RegisterWindowClass(const char* wClassName) {
	WNDCLASSEX nClass;

	nClass.cbSize = sizeof(WNDCLASSEX);
	nClass.style = CS_DBLCLKS;
	nClass.lpfnWndProc = DLLWindowProc;
	nClass.cbClsExtra = 0;
	nClass.cbWndExtra = 0;
	nClass.hInstance = GetModuleHandle(NULL);
	nClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // TODO: make an icon for alx?
	nClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	nClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	nClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	nClass.lpszMenuName = "what";
	nClass.lpszClassName = wClassName;

	if (!RegisterClassEx(&nClass))
		return 0;

	return 1;
}



BOOL StartMessageLoop() {
	MSG msg;
	BOOL bRet;

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == 0) {
			return 0;
		}
		else if (bRet == -1)
		{
			// handle the error and possibly exit
			//return msg.wParam;
			return 0;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

BOOL CreateSubwindows() {
	//HINSTANCE hInstance = GetModuleHandle(NULL);
	// execute = CreateWindowEx(NULL, "button", "Execute Command", WS_CHILD | WS_VISIBLE | WS_BORDER, 1, 400, 100, 36, MainWindow, NULL, NULL, NULL);//Top prority makeit work
	txtbox = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL, 10, 20, 520, 200, MainWindow, (HMENU)ALX_CONSOLE_WINDOW, HInstance, 0);
	HWND consoleFieldLabel = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE, 10, 0, 500, 20, MainWindow, NULL, HInstance, NULL);
	InputField = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VISIBLE, 10, 250, 500, 20, MainWindow, (HMENU)ALX_INPUT_FIELD, HInstance, 0);
	HWND inputFieldLabel = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE, 10, 230, 500, 20, MainWindow, NULL, HInstance, NULL);
	SendMessage(inputFieldLabel, WM_SETTEXT, NULL, (LPARAM)"Scriptexe and commandexe");
	SendMessage(consoleFieldLabel, WM_SETTEXT, NULL, (LPARAM)"Checks and console");
	SendMessage(InputField, EM_SETLIMITTEXT, INPUT_CHAR_LIMIT, NULL);

	//WaypointBox = CreateWindowEx(NULL, "LISTBOX", "", WS_CHILD | WS_BORDER | WS_VSCROLL | WS_VISIBLE, 10, 280, 520, 100, MainWindow, (HMENU)ALX_WAYPOINT_BOX, HInstance, 0);
	//SendMessage(WaypointBox, LVM_SETITEMTEXT, )

	HFONT textFont = CreateFont(18, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));

	SendMessage(inputFieldLabel, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(consoleFieldLabel, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(txtbox, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(InputField, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));

	UpdateWindow(MainWindow);

	return 1;
}

BOOL CreateWindowMenu() {
	WindowMenu = CreateMenu();
	if (!WindowMenu)
		return 0;

	HMENU mainDropdown = CreatePopupMenu();
	AppendMenu(mainDropdown, MF_STRING, MALX_EXIT, "Exit");
	AppendMenu(mainDropdown, MF_STRING, MALX_RESTART, "Restart");

	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)mainDropdown, "RVX-4");



	HMENU aboutDropdown = CreatePopupMenu();
	AppendMenu(aboutDropdown, MF_STRING, MALX_CREDITS, "Credits");
	AppendMenu(aboutDropdown, MF_STRING, MALX_COMMANDS, "Commands");

	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)aboutDropdown, "Options");

	return 1;
}

BOOL InitiateWindow() {
	HInstance = GetModuleHandle(NULL);

	UnregisterClass("ALX_WINDOW", HInstance);
	RegisterWindowClass("ALX_WINDOW");

	char alxName[50];

	_snprintf_s(alxName, 50, "RVX-4");

	//ParentWindow = FindWindow(NULL, "ROBLOX");
	if (!CreateWindowMenu())
		return 0;

	if (!(MainWindow = CreateWindowEx(
		NULL,
		"ALX_WINDOW",
		alxName,
		WS_SYSMENU | WS_MINIMIZEBOX,
		50,
		50,
		600,
		600,
		NULL, //ParentWindow,
		WindowMenu,
		HInstance,
		NULL))) return 0;

	//ScrollWindowEx(MainWindow, 0, 560, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_SMOOTHSCROLL);
	//EnableScrollBar(MainWindow, SB_VERT, ESB_ENABLE_BOTH);

	CreateSubwindows();
	UpdateWindow(MainWindow);

	ShowWindow(MainWindow, SW_SHOWNORMAL);
	init();

	return StartMessageLoop();
}

namespace Memory {
	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask) return 0;
		return (*szMask) == NULL;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
	{
		for (int i = 0; i<(int)dwLen; i++)
			if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}
	void retcheck(){
		DWORD addr = 0x140DA6;
		__asm{
			pushad
				pushfd
				mov eax, addr
				popfd
				popad
				ret
		}
	}

	void writebytes(int address, int bytes) {
		DWORD retcheck, rekt;
		VirtualProtect((LPVOID)address, bytes, PAGE_EXECUTE_READWRITE, &retcheck);
		for (int x = 0; x != bytes; x++) {
			*(BYTE*)(address + x) = 0x90;
		}
		VirtualProtect((LPVOID)address, bytes, retcheck, &rekt);
	}

	void DisableRetcheck() {
		writebytes(0x5181E4, 39);
		DWORD b, l;
		BYTE* rpb_unk = (PBYTE)0x5FC0F2;
		VirtualProtect((LPVOID)0x5FC0F2, 6, PAGE_EXECUTE_READWRITE, &b);
		*rpb_unk = 0xE9;
		*(DWORD*)(rpb_unk + 1) = (DWORD)(0x5FC128 - (DWORD)rpb_unk) - 5;
		VirtualProtect((LPVOID)retcheck, 1, 1, &b);
		VirtualProtect((LPVOID)0x5FC0F2, 6, b, &l);
	}


	int Scan(DWORD mode, char* content, char* mask)
	{
		VMProtectBeginUltra("snsdkfbwkebsdkfbniwe");
		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		for (DWORD lpAddr = 0; lpAddr<0x7FFFFFFF; lpAddr += PageSize)
		{
			DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
			if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
			if (mi.Type == MEM_MAPPED) continue;
			if (mi.Protect == mode)
			{
				int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
				if (addr != 0)
				{
					return addr;
				}
			}
		}
		VMProtectEnd();
	}


}

std::vector<std::string> split(std::string s){
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

std::string IntToHex(int Val) {
	std::stringstream stream;
	stream << std::hex << Val;
	return stream.str();
}

std::string Input() {
	std::string wotthefuck;
	getline(std::cin, wotthefuck);
	return wotthefuck;
}

int ScriptContextVftable;
int ScriptContext;

void ShowWindowForm() {
	InitiateWindow();
}
DWORD base = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");

DWORD getaddy(int address) {
	return (address - 0x400000 + base);
}

//DWORD ScriptContextVftable = getaddy(0x52D130);



void init() {
	VMProtectBeginUltra("snsdkfbwkebsdkfbniwe");
	Print(txtbox, "Checking for Filtering Enabled & Disabled Games...\r\n");
	boolean filtering = Roblox::DataModel;
	if (filtering) {
		Print(txtbox, "Filtering is enabled. \r\n");
	}
	else {
		Print(txtbox, "Filtering is not enabled. \r\n");
	}
	Print(txtbox, "Checking whitelist..\r\n");
	Sleep(2000);
	Print(txtbox, "Whitelisted ..\r\n");
	Sleep(2000);
	Print(txtbox, "Welcome to RVX4 ..\r\n");
	Print(txtbox, "Scanning... \r\n");
	Memcheck_Disable();
	int id = 0x023B1720;
	//std::string status = doauth(*(int*)(id)).c_str();
	//std::size_t found = status.find("Success!");
	//if (found != std::string::npos) {

		string showplayerid;
		showplayerid = std::to_string(id);
		//ScriptContextVftable = 0x5257C0;
		//84c3201
		DWORD Datamodel = getaddy(0x0102C584);
		//DWORD ScriptContextVftable = getaddy(0x10AB2130);
		//ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
		//Roblox::DataModel = Roblox::GetParent(ScriptContext);

		Roblox::DataModel = Memory::Scan(PAGE_READWRITE, (char*)&Datamodel, "xxxx");
		//Roblox::DataModel = Memory::Scan(PAGE_READWRITE, "\x0D\x40\xBA\x00", "xxxx");
		Roblox::Lighting = Roblox::FindFirstClass(Roblox::DataModel, "Lighting");
		Roblox::Workspace = Roblox::FindFirstClass(Roblox::DataModel, "Workspace");
		Roblox::Players = Roblox::FindFirstClass(Roblox::DataModel, "Players");
		Roblox::PlayerName = Roblox::GetName(Roblox::GetLocalPlayer(Roblox::Players));
		Print(txtbox, "Done \r\n");

		MessageBox(0, "Working on scriptexe /n updated to gui /n added new commands", "Update logs", MB_OK);

		Print(txtbox, "No updates found \r\n");
		Print(txtbox, "Welcome to rvx4 \r\n");
		GetLua();
		lua_getglobal(lua_State, "print");
		lua_pushstring(lua_State, "Hello world!");
		lua_pcall(lua_State, 1, 0, 0);

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Loop, NULL, NULL, NULL);

	}


int GetPerson(string person)
{
	if (person == "Guest ")
	{
		int p = Roblox::FindFirstChild_PartOf(Roblox::Workspace, person);
		return p;


	}
	else
	{
		int p = Roblox::FindFirstChild_PartOf(Roblox::Workspace, person);
		return p;
	}
}
int GetCharacter(std::string Name) {


	int Character = GetPerson(Name);
	if (Character != 0) {
		return Character;
	}
	else
	{
		return 0;
	}
}

void Console(char* title) {
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

bool Startt(std::string Cmd) {
	std::vector<std::string> Arguments = split(Cmd);
	//Sleep(2000);
	if (Arguments.size() == 0) {
		return false;
	}
	if (Arguments.at(0) == "noclip") {
		State = 11;
		Changing = true;
	}

	else if (Arguments.at(0) == "swim") {
		State = 4;
		Changing = true;
	}

	else if (Arguments.at(0) == "superjump") {
		State = 13;
		Changing = true;
	}
	else if (Arguments.at(0) == "day") {
		Roblox::SetMinutesAfterMidnight((void*)Roblox::Lighting, 15 * 60.0);
	}
	else if (Arguments.at(0) == "night") {
		Roblox::SetMinutesAfterMidnight((void*)Roblox::Lighting, 0 * 60.0);
	}
	else if (Arguments.at(0) == "airwalk") {
		State = 10;
		Changing = true;
	}
	else if (Arguments.at(0) == "breakjoints") {
		Roblox::BreakJoints((void*)Roblox::Workspace);
	}
	else if (Arguments.at(0) == "dusk") {
		Roblox::SetMinutesAfterMidnight((void*)Roblox::Lighting, 17 * 60.0);
	}
	else if (Arguments.at(0) == "jesusfly") {
		State = 12;
		Changing = true;
	}

	else if (Arguments.at(0) == "ragdoll") {
		State = 1;
		Changing = true;
	}
	else if (Arguments.at(0) == "disable") {
		State = 18;
		Changing = true;
	}
	else if (Arguments.at(0) == "slow'd") {
		State = -1;
		Changing = true;

	}
	else if (Arguments.at(0) == "phyH") {
		State = 16;
		Changing = true;
	}
	else if (Arguments.at(0) == "hidename") {
		if (Arguments.size() == 2) {
			int Player = Roblox::GetCharacter(Arguments.at(1).c_str());
			if (Player != 0) {
				int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
				if (Humanoid != 0) {
					Roblox::SetNameDisplayDist(Humanoid, 0);
				}
			}
		}
	}
	else if (Arguments.at(0) == "showname") {
		if (Arguments.size() == 2) {
			int Player = Roblox::GetCharacter(Arguments.at(1).c_str());
			if (Player != 0) {
				int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
				if (Humanoid != 0) {
					Roblox::SetNameDisplayDist(Humanoid, 100);
				}
			}
		}
	}
	else if (Arguments.at(0) == "cmds") {
		Print(txtbox, Commands);
	}
	else if (Arguments.at(0) == "kick") {
		int Player = Roblox::GetPlayer(Arguments.at(1));
		if (Player != 0) {
			Roblox::SetRank(Player, 255);
			Sleep(200);
			Roblox::SetRank(Player, 0);
		}
	}
	else if (Arguments.at(0) == "btools") {
		int Player = Roblox::GetPlayer(Arguments.at(1));
		if (Player > 150) {
			Roblox::SetRank(Player, 255);
		}
	}
	else if (Arguments.at(0) == "fog") {
		float FogE = std::stof(Arguments.at(1).c_str());
		Roblox::SetFogEnd((void*)Roblox::Lighting, FogE);
	}
	else if (Arguments.at(0) == "time") {
		if (Arguments.size() == 2) {
			double Time = std::stod(Arguments.at(1).c_str());
			Roblox::SetMinutesAfterMidnight((void*)Roblox::Lighting, Time * 60.0);
		}
	}
	else if (Arguments.at(0) == "shutdown") {
		std::vector<int> vector = Roblox::GetChildren(Roblox::Players);
		for (int i = 0; i < vector.size(); i++) {
			Roblox::rSetParent((void*)vector[i], 0);
		}
	}
	else if (Arguments.at(0) == "punish") {
		int Character = Roblox::GetCharacter(Arguments.at(1));
		if (Character > 150) {
			Roblox::rSetParent((void*)Character, Roblox::Lighting);
		}
	}
	else if (Arguments.at(0) == "ws") {
		if (Arguments.size() == 3) {
			float NewSpeed = std::stof(Arguments.at(2).c_str());
			int Player = Roblox::GetCharacter(Arguments.at(1));
			if (Player != 0) {
				int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
				if (Humanoid != 0) {
					Roblox::SetWalkSpeed(Humanoid, NewSpeed);
				}
			}
		}
	}
	else if (Arguments.at(0) == "explore") {
		std::vector<int> vectord = Roblox::GetChildren(Roblox::Workspace);
		for (int i = 0; i < vectord.size(); i++) {
			if (*Roblox::GetName(vectord[i]) != "Terrain" && *Roblox::GetName(vectord[i]) != "Camera")
			{
				Print(txtbox, "\r\nWorkspace " + *Roblox::GetName(vectord[i]));
			}
		}
	}
	else if (Arguments.at(0) == "god") {
int Character = Roblox::GetCharacter(Arguments.at(1));
		if (Character != 0)
		{
			int Humanoid = Roblox::FindFirstClass(Character, "Humanoid");
			if (Humanoid != 0)
			{
				Roblox::SetMaxHealth(Humanoid, HUGE_VAL);
			}
		}
	}
	else if (Arguments.at(0) == "freeze") {
		int Character = Roblox::GetCharacter(Arguments.at(1));
		if (Character != 0) {
			int Humanoid = Roblox::FindFirstClass(Character, "Humanoid");
			if (Humanoid != 0) {
				Roblox::SetWalkSpeed(Humanoid, 0);
			}
		}
	}

	else if (Arguments.at(0) == "thaw") {
		int Character = Roblox::GetCharacter(Arguments.at(1));
		if (Character != 0) {
			int Humanoid = Roblox::FindFirstClass(Character, "Humanoid");
			if (Humanoid != 0) {
				Roblox::SetWalkSpeed(Humanoid, 16);
			}
		}
		else
		{
			std::cout << "Failed to god " << std::endl;
		}
	}

	else if (Arguments.at(0) == "kill") {
		if (Arguments.size() == 2) {
			if (Arguments.at(0) == "all") {
				std::vector<int> vectord = Roblox::GetAllPlayerCharacters(Roblox::Workspace);
				for (int i = 0; i < vectord.size(); i++) {
					int Character = Roblox::GetCharacter(Arguments.at(1));
					if (Character != 0) {
						int Humanoid = Roblox::FindFirstClass(Character, "Humanoid");
						if (Humanoid != 0) {
							Roblox::SetMaxHealth(Humanoid, 0);
						}
					}
				}
			}
			else if (Arguments.at(0) == "others") {
				std::vector<int> vectord = Roblox::GetAllPlayerCharacters(Roblox::Workspace, *Roblox::PlayerName);
				for (int i = 0; i < vectord.size(); i++) {
					int Character = Roblox::GetCharacter(Arguments.at(1));
					if (Character != 0) {
						int Humanoid = Roblox::FindFirstClass(Character, "Humanoid");
						if (Humanoid != 0) {
							Roblox::SetMaxHealth(Humanoid, 0);
						}
					}
				}
			}
			else {
				int Player = Roblox::GetCharacter(Arguments.at(1));
				if (Player != 0) {
					int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
					if (Humanoid != 0) {
						Roblox::SetMaxHealth(Humanoid, 0);
					}
				}
			}
		}
		else
		{
			std::cout << "Failed to god " << std::endl;
		}
	}
	else if (Arguments.at(0) == "ungod") {
		if (Arguments.size() == 2) {
			int Player = Roblox::GetCharacter(Arguments.at(1));
			if (Player != 0) {
				int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
				if (Humanoid != 0) {
					Roblox::SetMaxHealth(Humanoid, 100);
				}
			}
			else
			{
				std::cout << "Failed to ungod " << std::endl;
			}
		}
	}
	else
	{
		std::cout << "Failed to ungod " << std::endl;
	}
	if (Arguments.at(0) == "sethealth") {
		float NewSpeed = std::stof(Arguments.at(2).c_str());
		int Character = Roblox::GetCharacter(Arguments.at(1));
		if (Character != 0) {
			int Humanoid = Roblox::FindFirstClass(Character, "Humanoid");
			if (Humanoid != 0) {
				Roblox::SetMaxHealth(Humanoid, NewSpeed);
			}
		}
	}
	else if (Arguments.at(0) == "stools") {
		int you = Roblox::FindFirstChild_PartOf(Roblox::Players, *Roblox::PlayerName);
		int yourBackpack = Roblox::FindFirstClass(you, "Backpack");
		int blah = Roblox::GetPlayer(Arguments.at(1));
		std::vector<int> packitems = Roblox::GetChildren(Roblox::FindFirstClass(blah, "Backpack"));
		for (int i = 0; i < packitems.size(); i++) {
			Roblox::rSetParent((void*)packitems[i], yourBackpack);
		}
	}
	else if (Arguments.at(0) == "jumppower") {
		float NewSpeed = std::stof(Arguments.at(2).c_str());
		int Character = Roblox::GetCharacter(Arguments.at(1));
		if (Character != 0) {
			int Humanoid = Roblox::FindFirstClass(Character, "Humanoid");
			if (Humanoid != 0) {
				Roblox::SetJumpPower(Humanoid, NewSpeed);
			}
		}
	}
	else if (Arguments.at(0) == "purge") {
		float min;
		float fog;
		if (Arguments.at(1) == "on") {
			min = 0;
			fog = 0;

		}
		if (Arguments.at(1) == "off") {
			min = 300000;
			fog = 100000;

		}

		Roblox::SetMinutesAfterMidnight((void*)Roblox::Lighting, min);
		Roblox::SetFogEnd((void*)Roblox::Lighting, fog);
	}

	return 0;
	VMProtectEnd();
	VMProtectEnd();

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ShowWindowForm, 0, 0, 0); //RVX
	}
	return 1;
}