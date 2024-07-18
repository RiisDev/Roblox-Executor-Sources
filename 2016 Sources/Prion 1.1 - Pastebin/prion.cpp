#pragma once
#include <Windows.h>

//_______________________________________________________________________________________________

///		DEFINES & ADDRESSES

int ScriptContextVftable;

#define WaveHeight_PropDescriptor				(DWORD*)0x1640B08
#define DisplayDistanceType_PropDescriptor		(DWORD*)0x163794C
#define FogColor_PropDescriptor					(DWORD*)0x1638CB4
#define FogStart_PropDescriptor					(DWORD*)0x1638CE8
#define FogEnd_PropDescriptor					(DWORD*)0x1638D1C

#define SetParent								0x448160
#define ReplicateProperty						0x510180
#define SetWalkSpeed							0x6B6620
#define SetMxHealth								0x6B5B50
#define SetJmpPower								0x6B5730

//_______________________________________________________________________________________________

///		TYPEDEFS

typedef void(__thiscall *sfs)(int self, float c);
typedef void(__thiscall *r)(int self, DWORD* r);
typedef void(__thiscall *s)(int self, int newparent);

static sfs SetJumpPower = (sfs)SetJmpPower;
static sfs SetWS = (sfs)SetWalkSpeed;
static sfs SetMaxHealth = (sfs)SetMxHealth;
static r Replicate = (r)ReplicateProperty;
static s rSetParent = (s)SetParent;

//_______________________________________________________________________________________________

///		INSTANCE INTERACTION FUNCTIONS

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
		call    dword ptr [eax + 20h]
	}
}

//_______________________________________________________________________________________________

/// STRUCT INTERACTION FUNCTIONS

void SetHumanoidState(int self, int state) {
	int statePtr = *(int*)(self + 0x2D4);
	if (statePtr) {
		*(BYTE*)(self + state + 0x229) = 1;
		*(int*)(statePtr + 0x20) = state;
	}
}

void SetFogEnd(int Lighting, float TFogEnd) {
	*(float*)(Lighting + 392) = TFogEnd;
	Replicate(Lighting, FogEnd_PropDescriptor);
}

void SetFogStart(int Lighting, float TFogStart) {
	*(float*)(Lighting + 388) = TFogStart;
	Replicate(Lighting, FogStart_PropDescriptor);
}

void SetFogColor(int Lighting, float R, float G, float B) {
	*(float*)(Lighting + 376) = R;
	*(float*)(Lighting + 380) = G;
	*(float*)(Lighting + 384) = B;
	Replicate(Lighting, FogColor_PropDescriptor);
}

void SetWaveHeight(int Terrain, float Height) {
	*(float*)(Terrain + 300) = Height;
	Replicate(Terrain, WaveHeight_PropDescriptor);
}

void SetDisplayDistanceType(int Humanoid, int Type) {
	int* Location = (int*)(Humanoid + 484);
	if (*Location != Type) {
		*Location = Type;
		Replicate(Humanoid, DisplayDistanceType_PropDescriptor);
	}
}

_______________________________________________________________________-


#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
//#include "detours.h"
//#include "LUA.h"
#include "ACM_DB.h"
//#include "RobloxDatatypes.h"

bool isFF = false;
bool ContextHooked = false;
/*
double PatchedWalkSpeed(int self) { // prevent walkspeed detection from client
	return 16;
}

double PatchedJumpPower(int self) { // prevent jmppower detection from client
	return 50;
}

DWORD* PatchedIdentity() {
	return (DWORD*)0x1630BB8;
}
*/

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

void DoCommand(std::string Cmd) {
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
						SetWS(Humanoid, powa);
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

		else if (In.at(0) == "punish") {
			if (In.size() == 2) {
				int Character = GetCharacter(In.at(1));
				if (Character != 0) {
					rSetParent(Character, Lighting);
				}
			}
		}

		else if (In.at(0) == "unpunish") {
			if (In.size() == 2) {
				int Character = FindFirstChild(Lighting, In.at(1));
				if (Character != 0) {
					rSetParent(Character, Workspace);
				}
			}
		}

		else if (In.at(0) == "hidename") {
			if (In.size() == 2) {
				int Character = GetCharacter(In.at(1));
				if (Character != 0) {
					int Humanoid = FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetDisplayDistanceType(Humanoid, 2);
					}
				}
			}
		}

		else if (In.at(0) == "showname") {
			if (In.size() == 2) {
				int Character = GetCharacter(In.at(1));
				if (Character != 0) {
					int Humanoid = FindFirstClass(Character, "Humanoid");
					if (Humanoid != 0) {
						SetDisplayDistanceType(Humanoid, 0);
					}
				}
			}
		}

		else if (In.at(0) == "fog") {
			if (In.size() == 2) {
				float No = ::atof(In.at(1).c_str());
				SetFogStart(Lighting, No);
				SetFogEnd(Lighting, No + 15);
			}
		}

		else if (In.at(0) == "rainbow") {
			if (In.size() == 2) {
				if (In.at(1) == "on") {
					rainbow = true;
				}
				else if (In.at(1) == "off") {
					rainbow = false;
				}
			}
		}

		else if (In.at(0) == "fogcolor") {
			if (In.size() == 4) {
				float No1 = ::atof(In.at(1).c_str());
				float No2 = ::atof(In.at(2).c_str());
				float No3 = ::atof(In.at(3).c_str());
				SetFogColor(Lighting, No1, No2, No3);
			}
		}

		else if (In.at(0) == "waterheight") {
			if (In.size() == 2) {
				float n = ::atof(In.at(1).c_str());
				int Terrain = FindFirstClass(Workspace, "Terrain");
				if (Terrain != 0) {
					SetWaveHeight(Terrain, n);
				}
			}
		}
		
		/*
		else if (In.at(0) == "setcontext") {
			if (In.size() == 2) {
				if (ContextHooked == false) {
					typedef int*(__cdecl *getidentity)(int);
					getidentity GetIdentity = (getidentity)0x60FA10;

					DetourTransactionBegin();
					DetourUpdateThread(GetCurrentThread());
					DetourAttach(&(LPVOID&)GetIdentity, (PBYTE)PatchedIdentity);
					DetourTransactionCommit();
					ContextHooked = true;
				}
				DWORD n = ::atoi(In.at(1).c_str());
				*(DWORD*)0x1630BB8 = n;
			}
		}

		else if (In.at(0) == "music") {
			if (In.size() == 2) {
				lua_emptystack(lua_State);
				lua_getglobal(lua_State, "workspace");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "Sound");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				
				lua_pushstring(lua_State, ("rbxassetid://" + In.at(1)).c_str());
				lua_setfield(lua_State, -2, "SoundId");

				lua_pushstring(lua_State, "POSITRON_AUDIO");
				lua_setfield(lua_State, -2, "Name");

				lua_pushnumber(lua_State, 1);
				lua_setfield(lua_State, -2, "Volume");

				lua_getfield(lua_State, -1, "Play");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
				lua_emptystack(lua_State);
			}
		}

		else if (In.at(0) == "localply_ff_r") {
			lua_emptystack(lua_State);
			if (isFF == false) {
				isFF = true;
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				if (!lua_isnil(lua_State, -1)) {
					lua_getglobal(lua_State, "Instance");
					lua_getfield(lua_State, -1, "new");
					lua_pushstring(lua_State, "ForceField");
					lua_pushvalue(lua_State, -4);
					lua_pcall(lua_State, 2, 0, 0);
				}
			}
			else {
				isFF = false;
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				if (!lua_isnil(lua_State, -1)) {
					lua_getfield(lua_State, -1, "ForceField");
					if (!lua_isnil(lua_State, -1)) {
						lua_getfield(lua_State, -1, "Destroy");
						lua_pushvalue(lua_State, -2);
						lua_pcall(lua_State, 1, 0, 0);
					}
				}
			}
			lua_emptystack(lua_State);
		}

		else if (In.at(0) == "localply_btools_r") {
			lua_emptystack(lua_State);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Backpack");
			if (!lua_isnil(lua_State, -1)) {

				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "HopperBin");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushnumber(lua_State, 2);
				lua_setfield(lua_State, -2, "BinType");

			}
			lua_emptystack(lua_State);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Backpack");
			if (!lua_isnil(lua_State, -1)) {

				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "HopperBin");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushnumber(lua_State, 3);
				lua_setfield(lua_State, -2, "BinType");

			}
			lua_emptystack(lua_State);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Backpack");
			if (!lua_isnil(lua_State, -1)) {

				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "HopperBin");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushnumber(lua_State, 4);
				lua_setfield(lua_State, -2, "BinType");

			}
			lua_emptystack(lua_State);
		}
		*/


	}
}



_________________________________________________________________________________________________


// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <WinInet.h>
#include <fstream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <string>
#include <Psapi.h>
#include <tlhelp32.h>
#include "ACM_DB.h"
#include "LUA.h"
#include "Memscan.h"
//#include "segments.h" // FUCK WINDOWS
#include "form.h"
//#include "Memcheck.h"
//#include "detours.h"

#define ACMERR 0xFF5

#pragma comment(lib, "wininet") 
#pragma comment(lib, "Dbghelp.lib")
//#pragma comment(lib, "detours.lib")
/*
int lua_state = 0;

typedef void(__cdecl *retcheckf)();
retcheckf retcheck = (retcheckf)0x53F0E0;

void rektretcheck() {
	return;
}

*/

void doRainbow() {
	if (rainbow == true) {
		if (rState == 0) {
			rState = 1;
			SetFogColor(Lighting, 255, 0, 0);
		}
		else if (rState == 1) {
			rState = 2;
			SetFogColor(Lighting, 0, 255, 0);
		}
		else if (rState == 2) {
			rState = 0;
			SetFogColor(Lighting, 0, 0, 255);
		}
		Sleep(300);
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

void rLoop() {
	do {
		__try {
			doRainbow();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			Sleep(1); // do absolutely fucking nothing
		}
	} while (true);

}

std::string Input() {
	std::string wotthefuck;
	getline(std::cin, wotthefuck);
	return wotthefuck;
}

void Console(char* title) {
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
}

/*
DWORD PatchedGetTop(int state) {
	if (lua_State == 0) {
		lua_State = state;
	}
	return (*(DWORD *)(state + 16) - *(DWORD *)(state + 28)) >> 4;
}
*/


void Main() {
	//Console("debug");
	*(DWORD*)0x1637460 = 0; // get rid of the ChangedState event to prevent client-side changestate detection
	//Memcheck_Disable(); lol get rekt
	
	/*
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)retcheck, (PBYTE)rektretcheck);
	DetourAttach(&(LPVOID&)lua_gettop, (PBYTE)PatchedGetTop);
	DetourTransactionCommit();
	*/
	
	ScriptContextVftable = 0xF7030C;
	int ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
	DataModel = GetParent(ScriptContext); // lol
	Workspace = FindFirstClass(DataModel, "Workspace");
	Lighting = FindFirstClass(DataModel, "Lighting");
	Players = FindFirstClass(DataModel, "Players");
	int LocalPlayer = GetLocalPlayer(Players);
	PlayerName = GetName(LocalPlayer);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Loop, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)rLoop, NULL, NULL, NULL);

	/*
	lua_emptystack(lua_State);
	lua_getglobal(lua_State, "workspace");
	lua_getfield(lua_State, -1, "CurrentCamera");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Hint");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 1, 0);
	lua_pushstring(lua_State, "Prion | by [FaZe] WD Gaster @ V3rmillion");
	lua_setfield(lua_State, -2, "Text");
	lua_emptystack(lua_State);
	*/
	InitiateWindow();
	
	
}

int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless){
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
	}
	return TRUE;
}