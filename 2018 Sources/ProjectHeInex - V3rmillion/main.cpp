#define _CRT_SECURE_NO_WARNINGS //msvc doesn't like freopen
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <istream>
#include <numeric>
#include <iostream>
#include <iterator> 
#include <vector>
#include <algorithm>

#include "aLua.h"


int SetWalkSpeed_Address = 0x6CE870;
typedef void(__thiscall * SetWalkS)(int self, float fuckingpowa);
SetWalkS SetWalkSpeed = (SetWalkS)SetWalkSpeed_Address;
int SetJumpPower_Address = 0x6CD9F0;
typedef void(__thiscall * setjump)(int self, float fuckingpowa);
setjump SetJumpPower = (setjump)SetJumpPower_Address;
int SetMaxHealth_Address = 0x6CDDD0;
typedef void(__thiscall * SetMaxH)(int self, float fuckingmaxhealth);
SetMaxH SetMaxHealth = (SetMaxH)SetMaxHealth_Address;
typedef int(__thiscall * opens)(void* self);
opens OpenState = (opens)0x60F460; // gets the rbxlua_state.

CLua lua;

int lua_State;

const char *Commands =
"- noclip - Walk through walls.\r\n"
"- swim - Swim in the air. Like fly.\r\n"
"- superjump - Jump in the air.\r\n"
"- airwalk - Walk in the air.\r\n"
"- jesusfly - Fly on drugs.\r\n"
"- ragdoll - Absolutely useless but fun.\r\n"
"- disable - Disable any command you activated.\r\n"
"- ws [n] - Set your walkspeed to [n].\r\n"
"- time [n] - Set time (in hours) to [n]\r\n"
"- restart - If you crashed, just type this.\r\n"
"- loadstring [s] - Execute script.\r\n";


int DataModel;
int Workspace;
int Players;
int Lighting;
int State;
bool Changing;
int rState = 0;
bool rainbow = false;
std::string* PlayerName;

int GetParent(int Instance) {
	return *(int*)(Instance + 0x34);
}

std::string* GetName(int Instance) {
	return (std::string*)(*(int*)(Instance + 0x28));
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

int FindFirstChild(int Instance, std::string Name) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		std::string* CurrentName = GetName(*(int*)i);
		CurrentName->resize(Name.size());
		if (*CurrentName == Name)
		{
			return *(int*)i;
		}
	}
	return 0;
}

const char* GetClass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
}

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

int GetLocalPlayer(int Plr) {
	return *(int*)(Plr + 0x16C);
}

int GetCharacter(std::string Name) {
	if (Name == "me") {
		int Character = FindFirstChild_PartOf(Workspace, *PlayerName);
		if (Character != 0) {
			return Character;
		}
		else
		{
			return 0;
		}
	}
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

void Destroy(DWORD Instance) { // dont use
	__asm {
		mov		ecx, Instance
			mov     eax, [ecx]
			call    dword ptr[eax + 20h]
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

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

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

int ScriptContextVftable;
int ScriptContext;

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
		//VMProtectBeginUltra("snsdkfbwkebsdkfbniwe");
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
		//VMProtectEnd();
	}


}

bool RunCommand(std::string Cmd) {
	std::vector<std::string> In = split(Cmd);
	if (In.size() != 0) {
		if (In.at(0) == "noclip") {
			State = 11;
			Changing = true;
		}

		else if (In.at(0) == "swim") {
			State = 4;
			Changing = true;
		}

		else if (In.at(0) == "superjump") {
			State = 13;
			Changing = true;
		}

		else if (In.at(0) == "airwalk") {
			State = 10;
			Changing = true;
		}

		else if (In.at(0) == "jesusfly") {
			State = 12;
			Changing = true;
		}

		else if (In.at(0) == "ragdoll") {
			State = 1;
			Changing = true;
		}

		else if (In.at(0) == "disable") {
			Changing = false;
		}

		else if (In.at(0) == "loadstring") {
			std::cout << "lol you believed it didnt you\n";
		}
		else if (In.at(0) == "ws") {
			if (In.size() == 3) {
				float powa = ::atof(In.at(2).c_str());
				int Character = GetCharacter(In.at(1));
				if (Character != 0) {
					int Humanoid = FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetWalkSpeed(Humanoid, powa);
					}
				}
			}
		}

		else if (In.at(0) == "jmppower") {
			if (In.size() == 3) {
				float powa = ::atof(In.at(2).c_str());
				int Character = GetCharacter(In.at(1));
				if (Character != 0) {
					int Humanoid = FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetJumpPower(Humanoid, powa);
					}
				}
			}
		}

		else if (In.at(0) == "god") {
			if (In.size() == 2) {
				int Character = GetCharacter(In.at(1));
				if (Character != 0) {
					int Humanoid = FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetMaxHealth(Humanoid, HUGE_VAL);
					}
				}
			}
		}

		else if (In.at(0) == "ungod") {
			if (In.size() == 2) {
				int Character = GetCharacter(In.at(1));
				if (Character != 0) {
					int Humanoid = FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetMaxHealth(Humanoid, 100);
					}
				}
			}
		}

		else if (In.at(0) == "kill") {
			if (In.size() == 2) {
				int Character = GetCharacter(In.at(1));
				if (Character != 0) {
					int Humanoid = FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetMaxHealth(Humanoid, 0);
					}
				}
			}
		}
		else if (In.at(0) == "kick") {
			lua.getglobal(lua_State, "game");
			lua.getfield(lua_State, -1, "Players");

			lua.getfield(lua_State, -1, "FindFirstChild");
			lua.pushvalue(lua_State, -2);
			lua.pushstring(lua_State, "LocalPlayer");
			lua.pcall(lua_State, 2, 1, 0);

			if (!lua.isnil(lua_State, -1)) {
				lua.getfield(lua_State, -1, "Destroy");
				lua.pushvalue(lua_State, -2);
				lua.pcall(lua_State, 1, 0, 0);
			}
		}
	}
	return 1;
}

void Start() {
	Console("Project HeInex by JaydenVerm @ VERMILLION");
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	std::cout << "Scanning... " << std::endl;
	ScriptContextVftable = 0xF7CBE4;
	ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
	DataModel = GetParent(ScriptContext); // lol
	Workspace = FindFirstClass(DataModel, "Workspace");
	Lighting = FindFirstClass(DataModel, "Lighting");
	Players = FindFirstClass(DataModel, "Players");
	int LocalPlayer = GetLocalPlayer(Players);
	PlayerName = GetName(LocalPlayer);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Loop, NULL, NULL, NULL);
	lua_State = OpenState((void*)ScriptContext);
	DWORD RobloxBase = (DWORD)GetModuleHandle(NULL);
	std::cout << "Roblox Base: " << std::hex << RobloxBase << std::endl;
	std::cout << "Lua State: " << IntToHex(lua_State) << std::endl;
	std::cout << ">PLAYERS< OK " << std::endl;
	std::cout << ">WORKSPACE< OK " << std::endl;
	std::cout << ">LIGHTING< OK " << std::endl;
	std::cout << ">DATAMODEL< OK " << std::endl;
	std::cout << ">SCRIPTCONTEXT< OK " << std::endl;
	std::cout << "Type in 'cmds' to view list of commands " << std::endl;
	do
	{
		std::cout << ">";
		std::string In = Input();
		RunCommand(In);
	} while (true);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Start, 0, 0, 0); //lets go
	}
	return 1;
}

