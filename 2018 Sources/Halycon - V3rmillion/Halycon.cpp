#define _CRT_SECURE_NO_WARNINGS //msvc doesn't like freopen
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>

#include "rlua.h"

extern HWND txtbox;

HWND txtbox = NULL;
HWND InputField = NULL;


#define execmd 500


using namespace std;
void Initiate();

RLua rLua;

int lua_State;

int LuaState_Address = 0x69FB60;

typedef int(__thiscall * openstate)(void* self);
openstate openState = (openstate)LuaState_Address;

int WalkSpeed_Address = 0x6B1FD0;
typedef void(__thiscall * setwalkspeed)(int self, float value);
setwalkspeed SetWalkSpeed = (setwalkspeed)WalkSpeed_Address;
int MaxHealth_Address = 0x6B11C0;
typedef void(__thiscall * setmaxhealth)(int self, float value);
setmaxhealth SetMaxHealth = (setmaxhealth)MaxHealth_Address;
int SetMinutesAfterMidnight_Address = 0x6BD450;
typedef void(__thiscall * setminutesaftermidnight)(void* lighting, double value);
setminutesaftermidnight SetMinutesAfterMidnight = (setminutesaftermidnight)SetMinutesAfterMidnight_Address;
int SetFogEnd_Address = 0x6BD330;
typedef void(__thiscall * setfogend)(void* self, float aidss);
setfogend SetFogE = (setfogend)SetFogEnd_Address;
int SetFogStart_Address = 0x6BD360;
typedef void(__thiscall * setfogstart)(void* lighting, float value);
setfogstart SetFogStart = (setfogstart)SetFogStart_Address;

int SetJumpPower_Address = 0x6AE710;
typedef void(__thiscall * setjumppower)(int self, float value);
setjumppower SetJumpPower = (setjumppower)SetJumpPower_Address;

int SetTransparency_Address = 0xC044D1;
typedef int(__thiscall * settransparency)(int self, float value);
settransparency SetTransparency = (settransparency)SetTransparency_Address;

int SetPersonalRankServer_Address = 0xA8CA20;
typedef void(__thiscall * setpersonalrankserver)(int self, int value);
setpersonalrankserver setPSR = (setpersonalrankserver)SetPersonalRankServer_Address;

int SetBreakjoints_Address = 0x5B8AD0;
typedef void(__thiscall * setbreakjoints)(void* self);
setbreakjoints BreakJoints = (setbreakjoints)SetBreakjoints_Address;





#define SetParent								 0x413830

typedef void(__thiscall *setparent)(void* self, int parent);
setparent setParent = (setparent)SetParent;



const char* COMMAND_LIST[] = {
	"ban [p] - Ban's [p].\r\n" // 1
	"shutdown - Shutsdown server.\r\n" // 2
	"time [n] - Set's time to [n].\r\n" // 3
	"fogend [n] - Set's fog to [n].\r\n" // 4
	"kill [p] - Kill's [p].\r\n" // 5
	"ws [p] [n] - Set's [p]'s walkspeed to [n].\r\n" // 6
	"god [p] - God's the [p].\r\n" // 7
	"kick [p] - Kick's [p]\r\n" // 8
	"ungod [p] - Ungod's [p].\r\n" // 9
	//"fogend [n] - Set's fog to [n]\r\n"
	"loopws [p] - Make's [p] go faster.\r\n" // 10
	"loopjump [p] - Make's [p] loopjump.\r\n" // 11
	"unchorall - Unchorsall workspace.\r\n" // 12
	"clearallchildren - Clear's all children from workspace.\r\n" // 13
	"blockhead [p] - Make's [p]'s head block.\r\n" // 14
	"music [n] - Set's music to [n].\r\n" // 15
	"invisible [p] - Make's [p] invisible.\r\n" // 16
	"visible [p] - Make's [p] visible.\r\n" // 18
	"freeze [p] - Freeze's [p].\r\n" // 19
	"thaw [p] - Thaw's [p].\r\n" // 20
	"punish [p] - Punishes the [p].\r\n" // 21
	"unpunish [p] - Unpunishes the [p].\r\n" // 22
	"stealttools [p] - Steal's the [p]'s tools from the [p2].\r\n" // 23
	"takettools [p] - Take's the [p]'s tools from the [p2].\r\n" // 24
	"btools [p] - Give's [p] btools.\r\n" // 25
	"unblockhead [p] - Unblockhead's [p].\r\n" // 26
	"faceless [p] - Make's [p] faceless.\r\n" // 27
	"shadowcolor [n] - Set's shadowcolor to [n].\r\n" // 28
	"globalshadows [n] - Set's global shadow to [n].\r\n" // 29
	"fogcolor [r] [g] [b] - Set's fog color to [r] [g] [b].\r\n " // 30
	"charapp [p] [id] - Set's [p]'s character to [p2] [id].\r\n" // 31
	"dpants [p] - Remove's [p]'s pants.\r\n"  // 32
	"topless [p] - Make's [p] topless.\r\n" // 33
	"rlimbs [p] - Remove's [p]'s limb.\r\n"
	"explore - Explores Skryim7.\r\n" // 34
	"noclip - Toggles noclip\r\n" // 35
	"airwalk - Toggles airwalk.\r\n" // 36
	"swim - Toggles swim.\r\n" // 37
	"jesusfly - Toggles fly.\r\n" // 38
	"superjump - Toggles superjump.\r\n " // 39
	"disableallstatecommands - Disables all state commands.\r\n" // 40
};



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

	int Scan(DWORD mode, char* content, char* mask)
	{
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
	}
}

#define textbox		500


void ConsoleOutput(const char * format, ...) {
	char message[textbox];
	va_list vl;
	va_start(vl, format);
	vsnprintf_s(message, textbox, format, vl);
	va_end(vl);

	SendMessage(txtbox, EM_SETSEL, -1, 0);
	SendMessage(txtbox, EM_REPLACESEL, FALSE, (LPARAM)message);
}



void HandleCommand_r(const string& cmd);
void HandleCommand(string cmd) {
	try {
		HandleCommand_r(cmd);
	}
	catch (...) {
		ConsoleOutput("Oops, I crashed when trying to execute the command '%s'.\r\n");
	}
}

//--------------------------------------------------------------------------------
//CODE COPIED FROM STIGMA SOURCE, WITH SOME THINGS REMOVED



#define ALX_CONSOLE_WINDOW              (WM_APP + 605)
#define ALX_INPUT_FIELD                 (WM_APP + 601)
#define INPUT_CHAR_LIMIT                500
HWND MainWindowHWND;
HMENU hMenu;
HMENU hMenuPopupFile;
HMENU hMenuPopupAbout;
HMODULE hInstance;
HWND ScriptTextBoxHWND;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#define MYMENU_EXIT                     (WM_APP + 101)
#define MYMENU_ABOUTMB                  (WM_APP + 102) 
#define MYMENU_EXECUTECODE              (WM_APP + 103)
#define MYMENU_SCRIPTTEXTBOX            (WM_APP + 104)
#define MYMENU_CLEARSCRIPT              (WM_APP + 105)
#define MYMENU_MINIMIZE                 (WM_APP + 109)
#define MYMENU_COMMANDS                 (WM_APP + 110)

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CTLCOLORSTATIC:
	{
		HDC hEdit = (HDC)wParam;
		SetTextColor(hEdit, RGB(0, 0, 0));
		SetBkColor(hEdit, RGB(255, 255, 255));
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MYMENU_EXIT:
			if (MessageBox(hwnd, "Are you sure you want to exit?", ":(", MB_YESNOCANCEL) == IDYES)
				ExitThread(0);
			break;

		case MYMENU_COMMANDS:
			HandleCommand("cmds");
			break;
		case ALX_INPUT_FIELD:
			if (HIWORD(wParam) == EN_MAXTEXT) {
				char cText[textbox];
				SendMessage((HWND)lParam, WM_GETTEXT, INPUT_CHAR_LIMIT, (LPARAM)cText);

				if (strcmp(cText, "") == 0)
					break;

				SendMessage((HWND)lParam, WM_SETTEXT, NULL, (LPARAM)"");

				//std::string command = cText;
				HandleCommand(cText);
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


BOOL RegisterDLLWindowClass(char *szClassName)
{
	WNDCLASSEX wc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = szClassName;
	wc.lpfnWndProc = DLLWindowProc;
	wc.style = CS_DBLCLKS;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = "Test";
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	if (!RegisterClassEx(&wc))
		return 0;
	return 1;
}

void RefreshContextMenu(HMENU hhMenu)
{
	hMenuPopupFile = CreatePopupMenu();
	AppendMenu(hMenuPopupFile, MF_STRING, MYMENU_MINIMIZE, TEXT("Minimize"));
	AppendMenu(hMenuPopupFile, MF_STRING, MYMENU_EXIT, TEXT("Exit"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupFile, TEXT("File"));

	hMenuPopupAbout = CreatePopupMenu();
	AppendMenu(hMenuPopupAbout, MF_STRING, MYMENU_COMMANDS, TEXT("Commands"));
	AppendMenu(hMenuPopupAbout, MF_STRING, MYMENU_ABOUTMB, TEXT("About"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupAbout, TEXT("Help"));
}

HMENU CreateDLLWindowMenu()
{
	HMENU heyMenu;
	heyMenu = CreateMenu();

	if (heyMenu == NULL)
		return FALSE;

	RefreshContextMenu(heyMenu);
	return heyMenu;
}

void CreateFWindows()
{
	txtbox = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL, 10, 20, 520, 200, MainWindowHWND, (HMENU)ALX_CONSOLE_WINDOW, hInstance, 0);
	InputField = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VISIBLE, 10, 250, 500, 20, MainWindowHWND, (HMENU)ALX_INPUT_FIELD, hInstance, 0);
	SendMessage(InputField, EM_SETLIMITTEXT, INPUT_CHAR_LIMIT, NULL);

	//WaypointBox = CreateWindowEx(NULL, "LISTBOX", "", WS_CHILD | WS_BORDER | WS_VSCROLL | WS_VISIBLE, 10, 280, 520, 100, MainWindow, (HMENU)ALX_WAYPOINT_BOX, HInstance, 0);
	//SendMessage(WaypointBox, LVM_SETITEMTEXT, )

	HFONT textFont = CreateFont(18, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
	SendMessage(txtbox, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(InputField, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));


	UpdateWindow(MainWindowHWND);
}

void ShowForm()
{
	hInstance = GetModuleHandle(NULL);
	hMenu = CreateDLLWindowMenu();
	RegisterDLLWindowClass("DLLWindowClass");
	MainWindowHWND = CreateWindowEx(NULL, "DLLWindowClass", "Halycon v2.34 [HAS A LUA OBJECT SCANNER] ", WS_SYSMENU | WS_MINIMIZEBOX,
		100,
		100,
		560,
		335, NULL, hMenu, hInstance, NULL);
	CreateFWindows();
	ShowWindow(MainWindowHWND, SW_SHOWNORMAL);
	Initiate();
	MSG messages;
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
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



int DataModel;
int Workspace;
int Lighting;
int Players;
int Camera;
int Head;
int LogService;
int CurrentPlayer;
int State;
int Teams;
bool Changing;
int antiscript = 0;
bool Seizure = false;
bool Night = false;
std::string* PlayerName;

int GetParent(int Instance) {
	return *(int*)(Instance + 0x34);
}

std::string* GetName(int Instance) {
	return (std::string*)(*(int*)(Instance + 0x28) - 0x4);
}

const char* GetClass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
}


void DoLoop() {
	//VMProtectBeginUltra("SEXYdsf");
	do {


		if (Seizure == true && Lighting != 0) {

			if (Night == false) {
				SetMinutesAfterMidnight((void*)Lighting, 24 * 60.0);
				Night = true;
			}
			else {
				SetMinutesAfterMidnight((void*)Lighting, 8 * 60.0);
				Night = false;
			}
		}
	} while (true);
	//	VMProtectEnd();
}

bool loopws = false;




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

// loop stuff



void SetHumanoidState(int self, int state) {
	int sPtr = *(int*)(self + 0x2DC);
	if (sPtr) {
		*(BYTE*)(self + state + 0x229) = 1;
		*(int*)(sPtr + 0x20) = state;
	}
}

void doState() {
	if (Changing == true) {
		int LocalCharacter = GetCharacter(*PlayerName);
		if (LocalCharacter != 0) {
			int Humanoid = FindFirstClass(LocalCharacter, "Humanoid");
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

void DoWalkSpeedLoop(std::string Name) {
	do {
		while (loopws == true) {
			int Character = GetCharacter(Name);
			if (Character != 0) {
				int LocalHumanoid = FindFirstClass(Character, "Humanoid");
				if (LocalHumanoid != 0) {
					float speed = 100;
					SetWalkSpeed(LocalHumanoid, speed);
				}
			}
			else {
				//Print(txtbox, "Failed to loopwalkspeed the player.");
			}
		}
	} while (true);
}



int ScriptContextVftable;
int ScriptContext;

void ShowWindowForm() {
	ShowForm();
}

void HandleCommand_r(const string& command) {
	std::vector<std::string> Arguments = split(command);
	ConsoleOutput("\r\n>%s\r\n", command.c_str());
	if (Arguments.size() != 0) {
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

		else if (Arguments.at(0) == "airwalk") {
			State = 10;
			Changing = true;
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
			Changing = false;
		}
			else if (Arguments.at(0) == "cmds") {
				ConsoleOutput("\r\n");
				for (int i = 0; i < _countof(COMMAND_LIST); ++i)
					ConsoleOutput("%s", COMMAND_LIST[i]);
				ConsoleOutput("\r\n");
			}
			else if (Arguments.at(0) == "god") {
				if (Arguments.at(1) == "all") {
					//int speed = atoi(Arguments.at(2).c_str());
					std::vector<int> vectord = GetAllPlayerCharacters(Workspace);
					for (int i = 0; i < vectord.size(); i++) {
						int Humanoid = FindFirstClass(vectord[i], "Humanoid");
						if (Humanoid > 150)
						{
							SetMaxHealth(Humanoid, HUGE_VAL);
						}
					}
				}
				else if (Arguments.at(1) == "others") {
					//int speed = atoi(Arguments.at(2).c_str());
					std::vector<int> vectord = GetAllPlayerCharacters(Workspace, *PlayerName);
					for (int i = 0; i < vectord.size(); i++) {
						int Humanoid = FindFirstClass(vectord[i], "Humanoid");
						if (Humanoid > 150)
						{
							SetMaxHealth(Humanoid, HUGE_VAL);
						}
					}
				}
				else {
					int Character = GetCharacter(Arguments.at(1));
					//int speed = atoi(Arguments.at(2).c_str());
					//cout << to_string(Character) << speed << endl;
					if (Character > 150)
					{
						int Humanoid = FindFirstClass(Character, "Humanoid");
						if (Humanoid > 150)
						{
							SetMaxHealth(Humanoid, HUGE_VAL);
						}
					}
				}
			}
			else if (Arguments.at(0) == "kill") {
				if (Arguments.at(1) == "all") {
					//int speed = atoi(Arguments.at(2).c_str());
					std::vector<int> vectord = GetAllPlayerCharacters(Workspace);
					for (int i = 0; i < vectord.size(); i++) {
						BreakJoints((void*)vectord[i]);
					}
				}
				else if (Arguments.at(1) == "others") {
					//int speed = atoi(Arguments.at(2).c_str());
					std::vector<int> vectord = GetAllPlayerCharacters(Workspace, *PlayerName);
					for (int i = 0; i < vectord.size(); i++) {
						BreakJoints((void*)vectord[i]);
					}
				}
				else {
					int Character = GetCharacter(Arguments.at(1));
					//int speed = atoi(Arguments.at(2).c_str());
					//cout << to_string(Character) << speed << endl;
					if (Character > 150)
					{
						BreakJoints((void*)Character);
					}

				}
			}
			else if (Arguments.at(0) == "ws") {
				if (Arguments.at(1) == "all") {
					int speed = atoi(Arguments.at(2).c_str());
					std::vector<int> vectord = GetAllPlayerCharacters(Workspace);
					for (int i = 0; i < vectord.size(); i++) {
						int Humanoid = FindFirstClass(vectord[i], "Humanoid");
						if (Humanoid > 150)
						{
							SetWalkSpeed(Humanoid, speed);
						}
					}
				}
				else if (Arguments.at(1) == "others") {
					int speed = atoi(Arguments.at(2).c_str());
					std::vector<int> vectord = GetAllPlayerCharacters(Workspace, *PlayerName);
					for (int i = 0; i < vectord.size(); i++) {
						int Humanoid = FindFirstClass(vectord[i], "Humanoid");
						if (Humanoid > 150)
						{
							SetWalkSpeed(Humanoid, speed);
						}
					}
				}
				else {
					int Character = GetCharacter(Arguments.at(1));
					int speed = atoi(Arguments.at(2).c_str());
					//cout << to_string(Character) << speed << endl;
					if (Character > 150)
					{
						int Humanoid = FindFirstClass(Character, "Humanoid");
						if (Humanoid > 150)
						{
							SetWalkSpeed(Humanoid, speed);
						}
					}
				}
			}
			else if (Arguments.at(0) == "unchorall") {
				int People = FindFirstClass(Workspace, "Model");
				if (People != 0) {
					setParent((void*)People, Lighting);
				}
			}
			else if (Arguments.at(0) == "loopws") {
				DoWalkSpeedLoop(Arguments.at(1));
			}
			else if (Arguments.at(0) == "seizure") {
				if (Arguments.size() == 2) {
					if (Arguments.at(1) == "on") {
						Seizure = true;
					}
					else if (Arguments.at(1) == "off") {
						Seizure = false;
					}
				}
			}
			else if (Arguments.at(0) == "time") {
				if (Arguments.size() == 2) {
					double Time = std::stod(Arguments.at(1).c_str());
					SetMinutesAfterMidnight((void*)Lighting, Time * 60.0);
				}
			}
			else if (Arguments.at(0) == "sfog") {
				if (Arguments.size() == 2) {
					float FogS = std::stof(Arguments.at(1).c_str());
					float FogE = std::stof(Arguments.at(1).c_str());
					SetFogStart((void*)Lighting, FogS);
					SetFogE((void*)Lighting, FogE);
				}
			}
			else if (Arguments.at(0) == "naked") {
				if (Arguments.size() == 2) {
					DWORD Character = GetCharacter(Arguments.at(1));
					if (Character != 0) {
						DWORD Shirt = FindFirstClass(Character, "Shirt");
						if (Shirt != 0) {
							setParent((void*)Shirt, Lighting);
						}
						DWORD Pants = FindFirstClass(Character, "Pants");
						if (Pants != 0) {
							setParent((void*)Pants, Lighting);
						}
					}
				}
			}
			else if (Arguments.at(0) == "topless") {
				DWORD Character = GetCharacter(Arguments.at(1));
				if (Character != 0) {
					DWORD Shirt = FindFirstClass(Character, "Shirt");
					if (Shirt != 0) {
						setParent((void*)Shirt, Lighting);
					}
				}
			}
			else if (Arguments.at(0) == "unchorall") {
				int People = FindFirstClass(Workspace, "Model");
				if (People != 0) {
					setParent((void*)People, Lighting);
				}
			}
			else if (Arguments.at(0) == "kick") {
				int Character = GetPlayer(Arguments.at(1));
				if (Character > 150) {
					setParent((void*)Character, 0);
				}
			}
			else if (Arguments.at(0) == "shutdown") {
				std::vector<int> vector = GetChildren(Players);
				for (int i = 0; i < vector.size(); i++) {
					setParent((void*)vector[i], 0);
				}
			}
			else if (Arguments.at(0) == "rhumanoid") {
				int Character = GetCharacter(Arguments.at(1));
				if (Character > 150) {
					int Humanoid = FindFirstClass(Character, "Humanoid");
					if (Humanoid > 0) {
						setParent((void*)Humanoid, Lighting);
					}
				}
			}
			else if (Arguments.at(0) == "rlimbs") {
				int Character = GetCharacter(Arguments.at(1));
				if (Character > 150) {
					int LArm = FindFirstChild_PartOf(Character, "Left Arm");
					int RArm = FindFirstChild_PartOf(Character, "Right Arm");
					int LLeg = FindFirstChild_PartOf(Character, "Left Leg");
					int RLeg = FindFirstChild_PartOf(Character, "Right Leg");
					if (LArm > 150) {
						setParent((void*)LArm, Lighting);
					}
					if (RArm > 150) {
						setParent((void*)RArm, Lighting);
					}
					if (LLeg > 150) {
						setParent((void*)LLeg, Lighting);
					}
					if (RLeg > 150) {
						setParent((void*)RLeg, Lighting);
					}
				}
			}
			else if (Arguments.at(0) == "btools") {
				int Character = GetPlayer(Arguments.at(1));
				if (Character > 150) {
					setPSR(Character, 255);
				}
			}
			else if (Arguments.at(0) == "psr") {
				int Character = GetPlayer(Arguments.at(1));
				if (Character > 150) {
					int VAL = FindFirstClass(Character, "IntValue");
					int item = FindFirstChild_PartOf(VAL, Arguments.at(2));
					setParent((void*)item, Lighting);
					setParent((void*)item, VAL);
					
				}
			}
			else if (Arguments.at(0) == "clearallchildren") {
				//else if (Arguments.at(0) == "clearws") {
				std::vector<int> vectord = GetChildren(Workspace);
				for (int i = 0; i < vectord.size(); i++) {
					if (*GetName(vectord[i]) != "Terrain" && *GetName(vectord[i]) != "Camera")
					{
						setParent((void*)vectord[i], Lighting);
						Sleep(50);
					}
				}
			}
			else if (Arguments.at(0) == "stealtools") {
				if (Arguments.at(1) == "all") {
					std::vector<int> vectord = GetChildren(Players);
					int you = FindFirstChild_PartOf(Players, *PlayerName);
					int yourPack = FindFirstClass(you, "Backpack");
					int enemy = GetPlayer(Arguments.at(1));
					for (int i = 0; i < vectord.size(); i++) {
						std::vector<int> packitems = GetChildren(FindFirstClass(enemy, "Backpack"));
						setParent((void*)packitems[i], yourPack);
					}
				}
				else if (Arguments.at(1) == "others") {
					std::vector<int> vectord = GetAllPlayerCharacters(Players, *PlayerName);
					int you = FindFirstChild_PartOf(Players, *PlayerName);
					int yourPack = FindFirstClass(you, "Backpack");
					int enemy = GetPlayer(Arguments.at(1));
					for (int i = 0; i < vectord.size(); i++) {
						std::vector<int> packitems = GetChildren(FindFirstClass(enemy, "Backpack"));
						setParent((void*)packitems[i], yourPack);
					}
				}
				else {
					int you = FindFirstChild_PartOf(Players, *PlayerName);
					int yourPack = FindFirstClass(you, "Backpack");
					int enemy = GetPlayer(Arguments.at(1));
					std::vector<int> packitems = GetChildren(FindFirstClass(enemy, "Backpack"));
					for (int i = 0; i < packitems.size(); i++) {
						setParent((void*)packitems[i], yourPack);
					}
				}
			}
			else if (Arguments.at(0) == "dhead") {
				int Character = GetCharacter(Arguments.at(1));
				if (Character != 0) {
					int Head = FindFirstChild_PartOf(Character, "Head");
					if (Head != 0) {
						SetTransparency(Head, 0);
					}
				}
			}
		}
		else {
			//Print(txtbox, "Invaild command: NULL\r\n.");
		}
	//}
}

void Initiate() {
	ConsoleOutput("Checking for Filtering Enabled & Disabled Games...\r\n");
	boolean filtering = DataModel;
	if (filtering) {
		ConsoleOutput("Filtering is enabled. \r\n");
	}
	else {
		ConsoleOutput("Filtering is not enabled. \r\n");
	}
	ConsoleOutput("This now has an Lua Object Scanner!\r\n");
	ConsoleOutput("Scanning... \r\n");
	Sleep(1000);
	ScriptContextVftable = 0xF6019C;
	ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
	DataModel = GetParent(ScriptContext);
	Workspace = FindFirstClass(DataModel, "Workspace");
	Players = FindFirstClass(DataModel, "Players");
	Lighting = FindFirstClass(DataModel, "Lighting");
	CurrentPlayer = GetLocalPlayer(Players);
	PlayerName = GetName(CurrentPlayer);
	//lua_State = openState((void*)ScriptContext);
	//Print(txtbox, "LuaState: " + IntToHex(lua_State) + "\r\n");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Loop, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DoWalkSpeedLoop, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DoLoop, NULL, NULL, NULL);
	ConsoleOutput("OK! \r\n");
	ConsoleOutput("Scanning for Lua objects...\r\n");
	try {
		rLua.initiate(ScriptContext);
	}
	catch (char* e) {
		ConsoleOutput("%s\r\n", e);
	}
	ScanResults scanResults = rLua.getInitResults();

	if (scanResults.failCount > 0) {
		ConsoleOutput("\r\nFailed to scan %d objects.\r\n", scanResults.failCount);
		for (int i = 0; i < scanResults.failCount; ++i)
			ConsoleOutput("%d, ", scanResults.failList[i]);

		ConsoleOutput("\r\n");
	}
	else
		ConsoleOutput("OK\r\n");

}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ShowWindowForm, 0, 0, 0); //lets go
	}
	return 1;
}