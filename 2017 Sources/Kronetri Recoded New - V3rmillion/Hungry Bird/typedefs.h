#include "Windows.h"
#include<stdio.h>
#include <conio.h>
#include <string>
#include <psapi.h>
#include <iostream>
#include "luastate.h"
#include <string>
//#include "VMProtectSDK.h"
#include <vector>
#include <sstream>
#include <istream>
#include <numeric>
#include <iterator> 
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <TlHelp32.h>
#include <fstream>

#include <stdio.h>
#include "wininet.h"
#include "urlmon.h"

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Rpcrt4.lib")

bool EnableBypass = false;

DWORD Base2 = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");
DWORD GetAddress(int address) {
	return{ address - 0x400000 + Base2 };
}

void RunTypedefCommand(std::string);
std::string enteredpw = "N/A";

namespace luaroblox {
	const char* AsshurtVersion = "V3.16";
	static int Game;
	static int Players;
	static int Workspace;
	static int Lighting;
	static int Terrain;
	static int InsertService;
	static int ChatService;
	static int ReplicatedStorage;
	static int ReplicatedFirst;
	static int Debris;
	static int Teams;
	static int CurrentPlayer;
	static int ScriptContext;
	std::string* PlayerName;
	int State;
	int rState = 0;          // Used by rainbow function
	int rState2 = 0;          // Used by rainbow function Water
	bool Rainbow = false;    // Set it to true to turn on rainbow effect
	bool RainbowWater = false;    // Set it to true to turn on rainbow effect Water
	bool Changing;
	bool IsAuth = false;
	bool tploop;
	bool StateActive = false;
	bool LoopJump = false; //Used By LoopJump Cmd
	bool LoopJumpPause = false; //Used By LoopJump Cmd
	bool LoopPlat = false; //Used By LoopPlat Cmd
	bool LoopPlatPause = false; //Used By LoopPlat Cmd
	bool LoopSit = false; //Used By LoopSit Cmd
	bool LoopKillOn = false;
	bool RainbowCooldown = false;
	std::vector<std::string> loopkilllist;
	bool SpamChat = false;
	bool CoolDownSpamChat = false;
	bool LoopSitPause = false;
	std::string SpamChatMsg = "default";
}

#define SoundIdOffset 0x170                      
#define useridoffset 0xA0
#define AgeOffset 0x244
#define ChildrenOffset 0x2C
#define NameOffset 0x28
#define FEOffset 0x25B //24B
#define CharacterAppearanceOffset 0xF4
#define LocalPlayerOffset 0x154
#define SetParentOffset 0x34
#define GetCharacterOffset 0x48
#define GetRobloxLockedOffset 35
#define AccountAgeOffset 0x24C
#define SkyboxBkOffset 0x90
#define SkyboxDnOffset 0x0C0
#define SkyboxFtOffset 0x0A8
#define SkyboxLfOffset 0x60
#define SkyboxRtOffset 0x78
#define SkyboxUpOffset 0x48

//ADDRESSES (IMPORTANT)
DWORD DataModelVfTable = GetAddress(0x0115B384);
DWORD ScriptC = GetAddress(0x01180FD4); //ScriptContext
int ScriptContextVst = Scan(PAGE_READWRITE, (char*)&ScriptC, "xxxx");

typedef int(__thiscall *CallGlobalState)(int self, int cf);
DWORD globalstates = FindPatternA("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x56\x57\x6A\x05\xFF\x75\x08\x8B\xF9\x33\xF6\xE8\x00", "xxxxxxxxxxxxxxx?");
static CallGlobalState GetLuaState = (CallGlobalState)globalstates; //globalStates[vm_index].state (function name)
																	//static CallGlobalState GetLuaState = (CallGlobalState)GetAddress(0x5200D0); //globalStates[vm_index].state (function name)

int GetGlobalState(DWORD* Instance, int a2) {
	DWORD *v2 = Instance; // edi@1
	signed int v3 = a2; // esi@1

	return (unsigned int)&v2[14 * v3 + 41] ^ v2[14 * v3 + 41];
}


int LUASTATESLOL = GetLuaState(ScriptContextVst, 2);

const int BreakJoints_Address = GetAddress(0x7FBD40);
const int BinType_address = GetAddress(0x6BB390); //BinType (Hopperbin)
const int Clear_address = GetAddress(0x898C10);

//const int ReplicateProp_address = 0;
//const int SetParent_address = GetAddress(0x421020); //Parent
//const int DraggingV1_address = GetAddress(0x6A1460); //DraggingV1
//const int CameraTarget_address = GetAddress(0x61D430); //CameraSubject

void LuaCmds() {
	//DWORD pushstring2aob = aobscan2::FindPatternA("RobloxPlayerBeta.exe", (BYTE*)"\x55\x8B\xEC\x8B\x55\x0C\x85\xD2\x75\x0D\xFF\x75\x08\xE8\xFE\xFE", "xxxxxxxxxxxxxxxx");
	DWORD pushstring2aob = FindPatternA("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x8B\x55\x0C\x85\xD2\x75\x0D\xFF\x75\x08\xE8\xFE\xFE", "xxxxxxxxxxxxxxxx");
	DWORD pcall2aob = FindPatternA("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x8B\x45\x14\x83\xEC\x08\x53\x56\x57\x8B\x7D\x08\x85", "xxxxxxxxxxxxxxxx");
	DWORD GetFieldAob = FindPatternA("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56", "xxxxxxxxxxxxxxxx");
	DWORD settopaob = FindPatternA("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x8B\x4D\x0C\x8B\x55\x08\x85\xC9\x78\x35\x8B\x42\x1C", "xxxxxxxxxxxxxxxx");
	DWORD pushvalueaob = FindPatternA("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\x75\x08\x56\xE8\x00\x00\xFF\xFF\x8B\x08\x8B\x56\x10", "xxxxxxxxxxxx??xxxxxxx");
	DWORD pushbooleanaob = FindPatternA("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x8B\x55\x08\x33\xC0\x39\x45\x0C\x8B\x4A\x10\x0F\x95\xC0\xC7\x41\x08\x03\x00\x00\x00", "xxxxxxxxxxxxxxxx");
	DWORD tolstringaob = FindPatternA("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\xFF\xFF\x8B\xF8\x83\xC4\x08\x83\x7F", "xxxxxxxxxxxxx??xxxxxxxxx");
	DWORD lua_typeaob = FindPatternA("RobloxPlayerBeta.exe", "\x55\x8B\xEC\xFF\x75\x0C\xFF\x75\x08\xE8\x00\x00\xFF\xFF\x8B\x48", "xxxxxxxxxx??xxxx");

	lua_pushstring2 = (lua_PushString)pushstring2aob; //Libraries must contain a \"Help\" function (sub_517700(a1, "Help");)
	lua_setfield2 = (lua_SetField)GetAddress(0x496F90); //__metatable (sub_517DD0(v5, -2, "__metatable");)
	lua_pcall2 = (lua_PCall)pcall2aob; //Breakpoint %d condition runtime error: %s ( if ( !sub_517260(v3, 0, 1, 0) ) {)
	lua_tolstring2 = (lua_ToLString)tolstringaob; //Current identity is %d (sub_54D9B0(a1, -1, 0);)
												  //lua_getop = (lua_GetTop)GetAddress(0x545D20); //Argument 2 missing or nil (v37 = sub_516CC0((int)v4);)
	lua_pushnumber2 = (lua_PushNumber)GetAddress(0x496600); //"field '%s' missing in data table (else{ sub_THIS IS IT(a1, COERCE__INT64((double)v17));)
															//lua_toboolean2 = (lua_ToBoolean)GetAddress(0x53EC70); //"field '%s' missing in date table (v8 = sub_5181C0(a1, -1);)
															//lua_getfield2 = (lua_GetField)GetAddress(0x53CC10); //isdst (sub_516B20(a1, -1, "isdst"))
	lua_getfield2 = (lua_GetField)GetFieldAob; //isdst (sub_516B20(a1, -1, "isdst"))
	lua_settop2 = (lua_SetTop)settopaob; //isdst (sub_54D780(a1, -2);)
	lua_pushvalue2 = (lua_PushValue)pushvalueaob; //lua_isfunction(L, -1) (sub_54CB70(a2, -2);)
	lua_pushboolean2 = (lua_PushBoolean)pushbooleanaob; //Search string Connected -> scroll down 1 passed the first (----------) and find (call sub_54C640)
	lua_pushcclosure2 = (lua_PushCclosure)GetAddress(0x496F90); //newproxy (sub_5466B0(a1, (int)sub_53EC90, 1);)
	lua_type2 = (lua_Type)lua_typeaob; //tables cannot be cyclic (a2 = (const char *)sub_53F4D0(a1, v6);)
}




HW_PROFILE_INFO thenewhwidprofileinfo;

char file[999998];
unsigned long readIFILE;
char* FileToString(std::string linktodownload)
{
	try {
		HINTERNET hOpen, hURL;

		hOpen = InternetOpen(TEXT("HungryBird/WinInet"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

		if (!hOpen) TerminateProcess(GetCurrentProcess(), NULL);

		hURL = InternetOpenUrl(hOpen, linktodownload.c_str(), NULL, -1, 0, INTERNET_FLAG_NO_AUTO_REDIRECT);
		if (!hURL)
		{
			TerminateProcess(GetCurrentProcess(), NULL);
		}

		if (!InternetReadFile(hURL, file, 999997, &readIFILE))
		{
			TerminateProcess(GetCurrentProcess(), NULL);
		}
		return file;
	}
	catch (...) {};
}

void EnableMemecheck() {
	try {
		if (EnableBypass == false) {
			try {
				EnableBypass = true;
				LuaCmds();
				//Sleep(400);
				//antimemcheck();
				Sleep(400);
				lua_State2 = LUASTATESLOL;

				Sleep(100);
			}
			catch (...) {};
		}
	}
	catch (...) {};
}

std::string GetLocalPlayerName() {
	try {
		EnableMemecheck();
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, "LocalPlayer");
		lua_getfield(lua_State2, -1, "Name");
		const char* Name = lua_tostring(lua_State2, -1);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		return Name;
	}
	catch (...) {};
}

int GetLocalPlayer(int Plr) {
	try {
		return *(int*)(Plr + LocalPlayerOffset);
	}
	catch (...) {};
}

std::string* GetName(int Instance) {
	try {
		return (std::string*)(*(int*)(Instance + 0x28));
	}
	catch (...) {};
}

inline const char *GetClass(DWORD fuck)
{
	try {
		return (const char*)(*(int(**)(void))(*(int*)fuck + 16))();
	}
	catch (...) {};
}

bool CheckClass(int Instance, const char* instanceclass) {
	try {
		if (memcmp(GetClass(Instance), instanceclass, strlen(instanceclass)) == 0) {
			return true;
		}
		else { return false; }
	}
	catch (...) {};
}

int GetParent(int Instance) {
	return *(int*)(Instance + SetParentOffset);
}
std::vector<int> GetChildren(int Instance) {
	try {
		std::vector<int> rtn;

		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			rtn.push_back(*(int*)i);
		}
		return rtn;
	}
	catch (...) {};
}

int FindFirstClass(int Instance, const char* Name) {
	try {
		DWORD StartOfChildren = *(DWORD*)(Instance + ChildrenOffset);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
			{
				return *(int*)i;
			}
		}

		return 0;
	}
	catch (int e) {

	}

}
int FindFirstChild_PartOf(int Instance, std::string Name) {
	try {
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
	catch (...) {};
}
int GetPlayer(std::string Name) {
	try {
		if (Name == "me") {
			int Character = luaroblox::CurrentPlayer;
			if (Character != 0) {
				return Character;
			}
			else
			{

			}
		}
		int Character = FindFirstChild_PartOf(luaroblox::Players, Name);
		if (Character != 0) {
			return Character;
		}
		else
		{
			return 0;
		}
	}
	catch (...) {};
}
std::string* GetSoundId(int Instance) {
	try {
		return (std::string*)(Instance + SoundIdOffset);
	}
	catch (...) {};
}
int GetCharacter(std::string Name) {
	try {
		if (Name == "me") {
			int Character = FindFirstChild_PartOf(luaroblox::Workspace, *luaroblox::PlayerName);
			if (Character != 0) {
				return Character;
			}
			else
			{

			}
		}
		int Character = FindFirstChild_PartOf(luaroblox::Workspace, Name);
		if (Character != 0) {
			return Character;
		}
		else
		{
			return 0;
		}
	}
	catch (...) {};
}
int* GetUserId(int Instance) {
	try {
		return (int*)(Instance + useridoffset);
	}
	catch (...) {};
}
int* GetAccountAge(int Instance) {
	try {
		return (int*)(Instance + AgeOffset);
	}
	catch (...) {};
}
int GetParentofInstance(int Instance) {
	try {
		return *(int*)(Instance + SetParentOffset);
	}
	catch (...) {};
}

bool* GetFEEnabled(int Instance) {
	try {
		return (bool*)(*(bool*)(Instance + FEOffset));
	}
	catch (...) {};
}
typedef int*(__cdecl *GetContextA)();
GetContextA ContextLevel = (GetContextA)GetAddress2(0x54A760);


int *ChangeContextLevel() {
	int *Level = ContextLevel();
	if (*Level == 0) //ensures default context level is 0
		*Level = 2; //changes the context level to 2
	return Level;
}
int *lvl7() {
	int *Level = ContextLevel();
		*Level = 7; //changes the context level to 2
	return Level;
}
int *lvl4() {
	int *Level = ContextLevel();
	*Level = 4; //changes the context level to 2
	return Level;
}

int *lvl2() {
	int *Level = ContextLevel();
	*Level = 2; //changes the context level to 2
	return Level;
}
int *lvl10() {
	int *Level = ContextLevel();
	*Level = 10; //changes the context level to 2
	return Level;
}

void newFFOnCharacter(std::string plr) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "ForceField");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}
void chat(std::string word) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, "LocalPlayer");
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "HumanoidRootPart");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Dialog");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 1, 0);

		lua_pushstring(lua_State2, (word.c_str()));
		lua_setfield(lua_State2, -2, "InitialPrompt");
		lua_pushvalue(lua_State2, -2);
		lua_pcall(lua_State2, 1, 0, 0);
	}
	catch (...) {};
}
void LocalFF() {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, "LocalPlayer");
		lua_getfield(lua_State2, -1, "Character");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "ForceField");
		lua_pushvalue(lua_State2, -4);
	    lua_pcall(lua_State2, 2, 0, 0);
		lua_settop(lua_State2, 0);
}
void Btools(std::string plr) {
	try {
		// move tool
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Backpack");

		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "HopperBin");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 1, 0);
		lua_pushnumber(lua_State2, 2);
		lua_setfield(lua_State2, -2, "BinType");

		// copy tool
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Backpack");

		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "HopperBin");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 1, 0);
		lua_pushnumber(lua_State2, 3);
		lua_setfield(lua_State2, -2, "BinType");

		// destorytool
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Backpack");

		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "HopperBin");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 1, 0);
		lua_pushnumber(lua_State2, 4);
		lua_setfield(lua_State2, -2, "BinType");
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}
void destroytool(std::string plr) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Backpack");

		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "HopperBin");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 1, 0);
		lua_pushnumber(lua_State2, 4);
		lua_setfield(lua_State2, -2, "BinType");
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}
void copytool(std::string plr) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Backpack");

		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "HopperBin");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 1, 0);
		lua_pushnumber(lua_State2, 3);
		lua_setfield(lua_State2, -2, "BinType");
	}
	catch (...) {};
}
void movetool(std::string plr) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Backpack");

		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "HopperBin");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 1, 0);
		lua_pushnumber(lua_State2, 2);
		lua_setfield(lua_State2, -2, "BinType");
	}
	catch (...) {};
}
void NewSparklesOnCharacter(std::string plr) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Sparkles");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}

void setnewSkybox(std::string SkyName, std::string Player) {
	try {
		std::string format = "http://www.roblox.com/asset/?id=" + Player;
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Lighting");
		lua_getfield(lua_State2, -1, SkyName.c_str());
		lua_pushstring(lua_State2, format.c_str());
		lua_setfield(lua_State2, -2, "SkyboxUp");
		lua_pushstring(lua_State2, format.c_str());
		lua_setfield(lua_State2, -2, "SkyboxBk");
		lua_pushstring(lua_State2, format.c_str());
		lua_setfield(lua_State2, -2, "SkyboxDn");
		lua_pushstring(lua_State2, format.c_str());
		lua_setfield(lua_State2, -2, "SkyboxFt");
		lua_pushstring(lua_State2, format.c_str());
		lua_setfield(lua_State2, -2, "SkyboxLf");
		lua_pushstring(lua_State2, format.c_str());
		lua_setfield(lua_State2, -2, "SkyboxRt");
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}

void newSkybox(std::string SkyboxID) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Lighting");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Sky");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		Sleep(300);
		setnewSkybox("Sky", SkyboxID);
	}
	catch (...) {};
}
void SetTimeLUA(double Time) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Lighting");

		lua_getfield(lua_State2, -1, "SetMinutesAfterMidnight");
		lua_pushvalue(lua_State2, -2);
		lua_pushnumber(lua_State2, Time);
		lua_pcall(lua_State2, 2, 1, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}

void newFireOnCharacter(std::string plr) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Fire");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}
/*
void spampart(std::string plr) {
try {
lua_getglobal(lua_State2, "game");
lua_getfield(lua_State2, -1, "Workspace");
lua_getglobal(lua_State2, "Instance");
lua_getfield(lua_State2, -1, "new");
lua_pushstring(lua_State2, "Part");
lua_pushvalue(lua_State2, -4);
lua_pcall(lua_State2, 2, 0, 0);
lua_settop(lua_State2, 0);
}
catch (...) {};
}
*/

void newBombFireOnCharacter(std::string plr) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Fire");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Fire");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Fire");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Fire");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Fire");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Fire");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Fire");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}

void setCharHealth(std::string Player, float health) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, Player.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Humanoid");
		lua_pushnumber(lua_State2, health);
		lua_setfield(lua_State2, -2, "MaxHealth");
		lua_pushnumber(lua_State2, health);
		lua_setfield(lua_State2, -2, "Health");
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}

void SetCharWalkSpeed(std::string Player, float health) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, Player.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Humanoid");
		lua_pushnumber(lua_State2, health);
		lua_setfield(lua_State2, -2, "WalkSpeed");
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}

void SetCharJumpPower(std::string Player, float health) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, Player.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Humanoid");
		lua_pushnumber(lua_State2, health);
		lua_setfield(lua_State2, -2, "JumpPower");
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}


void SetStringValue(std::string Player, std::string thevalue, std::string statvalue) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, Player.c_str());
		lua_getfield(lua_State2, -1, "leaderstats");
		lua_getfield(lua_State2, -1, thevalue.c_str());
		lua_pushstring(lua_State2, statvalue.c_str());
		lua_setfield(lua_State2, -2, "Value");
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}

void SetIntValue(std::string Player, std::string thevalue, const char* statvalue) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, Player.c_str());
		lua_getfield(lua_State2, -1, "leaderstats");
		lua_getfield(lua_State2, -1, thevalue.c_str());
		int TheValue = atof(statvalue);
		lua_pushnumber(lua_State2, TheValue);
		lua_setfield(lua_State2, -2, "Value");
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}
void LUASetFogStart(float character) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Lighting");
		lua_pushnumber(lua_State2, character);
		lua_setfield(lua_State2, -2, "FogStart");
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}
void newsmoke(std::string plr) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}

void MegaSmokeBig(std::string plr) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, "Players");
		lua_getfield(lua_State2, -1, plr.c_str());
		lua_getfield(lua_State2, -1, "Character");
		lua_getfield(lua_State2, -1, "Torso");
		lua_getglobal(lua_State2, "Instance");
		lua_getfield(lua_State2, -1, "new");
		lua_pushstring(lua_State2, "Smoke");
		lua_pushvalue(lua_State2, -4);
		lua_pcall(lua_State2, 2, 0, 0);
		lua_pop(lua_State2, 2);
		lua_pop(lua_State2, 4);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}

void printToLocalConsole(const char* input, ...) {
	try {
		lua_getglobal(lua_State2, "print");
		lua_pushstring(lua_State2, input);
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}
void warnToLocalConsole(const char* input, ...) {
	try {
		lua_getglobal(lua_State2, "warn");
		lua_pushstring(lua_State2, input);
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}
void ClearAllChildren(std::string Service) {
	try {
		lua_getglobal(lua_State2, "game");
		lua_getfield(lua_State2, -1, Service.c_str());
		lua_getfield(lua_State2, -1, "ClearAllChildren");
		lua_pushvalue(lua_State2, -2);
		lua_pcall(lua_State2, 1, 0, 0);
		lua_emptystack(lua_State2);
	}
	catch (...) {};
}


//BinType
typedef void(__thiscall *BinT)(int self, int type);
BinT SetBinType = (BinT)BinType_address;

//ClearTerrain
typedef void(__thiscall *ClearTerrain)(int self);
ClearTerrain Clear = (ClearTerrain)Clear_address;

typedef void(__thiscall *settime)(int self, double time);
#ifndef MODE_PROP_DESCRIPTOR
typedef void(__thiscall *FogSE)(int self, float value);
typedef void(__thiscall *FogColor)(int self, float RGB[3]);
typedef void(__thiscall *DispDistType)(int self, int value);
#endif
typedef void(__thiscall *SFS)(int self, float value);
typedef void(__thiscall *breakJoints)(int self);
//typedef void(__thiscall *rProp)(int self, DWORD* r);
//typedef void(__thiscall *setParent)(int self, int newparent);

/*
// Shuts down game. Don't use.
typedef void(__thiscall *setPSRank)(int self, int value);
typedef void(__thiscall *setAnchored)(int self, int value);
typedef void(__thiscall *setTransparency)(int self, float value);
*/

breakJoints BreakJoints = (breakJoints)BreakJoints_Address;