#pragma once
#include <Windows.h>
#include <algorithm>
#include <functional>
#include <WinInet.h>
#include <string>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
#include <limits>
/////////////////////
#include <sstream>
#include <fstream>
#include <iomanip>
//#include "DoCommand.h"
#include <windef.h>
////////////////////////////////
//#include "form.h"
#include "VMProtectSDK.h"
//#include "Addresses_1.h"
#include <stdexcept>
//#include "LUA.h"

static std::string fioanwf89gaefj = VMProtectDecryptStringA("Tick");
static std::string ata90etfasfjaf = VMProtectDecryptStringA("Au");

static DWORD ROBLOX_BASE_ALT = (DWORD)GetModuleHandle(NULL);//DWORD ROBLOX_BASE = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");
#define rbxaddy(x) (x - 0x400000 + ROBLOX_BASE_ALT)
//#define rbxdword(x) (x - 0x400000 + ROBLOX_BASE_ALT)
												 //_______________________________________________________________________________________________

												 ///		DEFINES & ADDRESSES

static BOOLEAN DoesConsoleExist();
static int LuaC = 0;
static int ScriptContextVftable;
static int Restricted = 0;
static int FindFirstPlayer(int Instance, std::string Name);
static BOOL InstanceExists(int Instance);
static bool IsInCharString(char *str1, char *search);
static void setParent(int Instance, int NewParent);
extern void ClearConsoleOutput();
extern void ConsoleOutput(std::string format, ...);
extern void HandleCommand(std::string command);
//extern void Replicate(int self, DWORD* r);

#define CameraType_PropDescriptor				rbxaddy(0x19271B4)
#define CameraSubject_PropDescriptor			rbxaddy(0x1927458)

static int aSetParent = rbxaddy(0x5235F0);
// find ReplicatedProperty with Archivable
#define ReplicateProperty						rbxaddy(0x51C4D0)
#define AnchorAddr                              rbxaddy(0x73C100)
/* Search for "NegativePart" follow the first result then decompile(F5)
it and it should be the second last sub_NUMBERS and should have a (1) within it's brackets.
Example: sub_6FBDE0(1); */

//_______________________________________________________________________________________________//
//		TYPEDEFS
//_______________________________________________________________________________________________//

#define CHAR_LIMIT			5000
static HWND ConsoleFieldDoCommand = NULL; // for the console

typedef void(__thiscall *r)(void* self, int r);
//typedef void(__thiscall *r)(int self, int r);
typedef int(__thiscall *setP)(int self, void *newparent);
typedef void(__thiscall *s)(int self, int newparent);
typedef void(__thiscall *jsgiojs)(int self);


/*int RealPar = rbxaddy(0x5997E0);
typedef int(__thiscall *loadreal)(int InsertService, int GearId);
static loadreal LoadGear = (loadreal)RealPar;*/


static jsgiojs TryStuffOnStuff = (jsgiojs)rbxaddy(0xAF55B0);
static r _Replicate = (r)ReplicateProperty;
static setP _rSetParent = (setP)aSetParent;

static void SetCanBeDropped(int self, int c) {
	if (self > 0) {
		__try {
			*(BYTE *)(self + 441) = c;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
}

static void SetAnchored(int self, int a2) {
	if (self != 0) {
		try {
			((void(__thiscall*)(int, int))AnchorAddr)(self, a2);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "", 0);
		}
		catch (...) {}
	}
}

static void Replicate(int self, int r) {
	if (self > 1000) {
		try {
			_Replicate((void*)self, (int)r);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "", 0);
		}
		catch (...) {}
	}
}

static void setParent(int ins, int npar) {
	int Instance = ins;
	int NewParent = npar;
	if (Instance > 1000 && NewParent > 1000) {
		try {
			//((void(__thiscall*)(char*, void*))aSetParent)((char*)Instance, (void*)NewParent);
			_rSetParent(Instance, (void*)NewParent);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "", 0);
		}
		catch (...) {}
		Sleep(45);
	}
}

//using namespace std;
//_______________________________________________________________________________________________

//		INSTANCE INTERACTION FUNCTIONS
static int Bypass = 1;
static int UserId;
static int Terrain;
static int NetworkClient;
static int ScriptContext;
static int DataModel;
static int CookieService;
static int Debris;
static int Workspace;
static int InsertService;
static int Players;
static int LocalPlayer;
static int Lighting;
static int State;
static int Teams;
static int IndexInstance;
static int ReplicatedStorage;
static int ContentProvider;
static bool Changing;
static int rState = 0;
static bool rainbow = false;

static std::string LatestVersion;
static std::string loopCommand1;
static std::string loopCommand2;
static std::string PlayerName;
static std::string LocalPlayerUsername;
static std::string AssignedUsername = " ";
static std::string ProtectWaffle = VMProtectDecryptStringA("vPancakeAttacks");
static std::string DownloadURL(const char* URL); // PLOX  
static BOOL MemcheckStatus;
static BOOL HWID_BOOL = FALSE;
static BOOL Rejected = FALSE;
static BOOL Free = FALSE;
static BOOL AttemptToBypassAntiHax = FALSE;
static BOOL MemChkRan = FALSE;

static int TM = 0;

struct InstanceTable {
	DWORD begin;
	DWORD stop;
	BYTE valid;
};

static InstanceTable GetInstanceTable(DWORD instance, DWORD offset) {
	InstanceTable rtn;
	VMProtectBeginMutation("iikfnsaighsfahs9fh*(Hf)*");
	__try {
		if (!*(DWORD*)(instance + offset)) {
			rtn.valid = false;
			VMProtectEnd();
			return rtn;
		}

		rtn.begin = *(DWORD*)(*(DWORD *)(instance + offset));
		rtn.stop = *(DWORD*)(*(DWORD *)(instance + offset) + 4);
		rtn.valid = true;
		return rtn;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		rtn.valid = false;
	}
	return rtn;
}

static std::vector<DWORD> GetChildren(DWORD instance) { // start child ptr
	VMProtectBeginMutation("hriey9q3hjd920j9dj291nd");
	std::vector<DWORD> rtn;
	if (instance > 1000) {
		InstanceTable instanceTable = GetInstanceTable(instance, 0x2C);
		if (!instanceTable.valid) {
			VMProtectEnd();
			return rtn;
		}

		DWORD inc = instanceTable.begin;
		DWORD incCompare = instanceTable.stop;

		while (inc != incCompare) {
			rtn.push_back(*(DWORD*)inc);
			inc += 8;
		}
	}
	return rtn;
}

static int GetCharacterFromPlayer(int player) {
	int Character = 0;
	if (player > 1000) {
		__try {
			Character = *(int*)(player + 0x48);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return Character;
};

static bool is_number(const std::string& s) {
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

static int GetParent(int Instance) { // Search: Parent, and it should be the second sub_NUMBERS then follow it and get the offset
	if (Instance > 1000) {
		__try {
			int Parent = *(int*)(Instance + 0x34);
			if (Parent != 0) {
				return Parent;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return int(0);
}

static std::string to_lowercase(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(std::tolower));
	return s;
}

/*const char* *GetName(int Instance) {
	//std::string returnVal = std::string("0");
	int ret = 0;
	if (Instance > 10000) {
		//*(std::string*)*(int*)(Instance + 0x28);
		__try {
			std::string s = *(std::string*)*(int*)(Instance + 0x28);
			//if (s) {
				return s.c_str();
			//}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return 0;
}*/

static std::string* GetName(int Instance) {
	if (Instance) {
		__try {
			return (std::string*)(*(int*)(Instance + 0x28));
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return (std::string*)"0";
}

/*
inline const char *GetClass(DWORD fuck) {
std::string *rere;
__asm {
mov ecx, fuck
mov eax, [ecx]
call dword ptr[eax + 10h]
add eax, 4
mov rere, eax
}
return rere->c_str();
}*/

static const char* GetClass(int self) {
	if (self) {
		const char* className = "0";
		__try {
			className = (const char*)(*(int(**)(void))(*(DWORD *)self + 16))();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
		return className;
	}
	return ("0");
}

static int FindFirstChild_PartOf(int Instance, std::string Name) {
	std::vector<DWORD> Children = GetChildren(Instance);
	for (std::size_t x = 0; x < Children.size(); x++) {
		if (Children[x] != 0) {
			std::string InstanceName = *GetName(Children[x]);
			if (InstanceName == Name) {
				return Children[x];
			}
		}
	}
	return int(0);
}

static int FindFirstClass(int Instance, const char* ClassName) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), ClassName, strlen(ClassName)) == 0)
		{
			return *(int*)i;
		}
	}
	return int(0);
}

static int FindFirstChild(int Instance, std::string Name) {
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

static bool IsInCharString(char *str1, char *search) {
	for (std::size_t i = 0; i < strlen(str1); ++i) {
		if (strncmp(&str1[i], search, strlen(search)) == 0) {
			return true;
		}
	}
	return false;
}

static int FindFirstChild_lower(int Instance, std::string Name) {
	if (Instance > 10000) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		if (StartOfChildren != 0) {
			DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
			if (EndOfChildren != 0) {
				for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
					int index = *(int*)i;
					if (index > 0) {
						try {
							if (strstr(to_lowercase(*GetName(index)).c_str(), to_lowercase(Name).c_str())) {
								return index;
							}
						}
						catch (std::exception) {}
						catch (...) {}
					}
				}
			}
		}
	}
	return int(0);
}

static int GetService(std::string ServiceName) {
	std::vector<DWORD> Services = GetChildren(DataModel);
	for (int c = 0; c < Services.size(); c++) {
		DWORD Service = Services.at(c);
		if (strcmp(GetClass(Service), ServiceName.c_str()) == 0) {
			return Service;
		}
	}
	return int(0);
}

static std::string GetPlayerName(std::string Name) {
	DWORD StartOfChildren = *(DWORD*)(Players + 0x2C);
	if (StartOfChildren != 0) {
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		if (EndOfChildren != 0) {
			if (Name == "me") {
				return *GetName(LocalPlayer);
			}
			for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
				int index = *(int*)i;
				if (index > 0) {
					std::string indexPlayerName = *GetName(index);
					if (strstr(to_lowercase(indexPlayerName).c_str(), to_lowercase(Name).c_str())) {
						return indexPlayerName;
					}
				}
			}
		}
	}
	return std::string("0");
}

static std::string GetChildName(int Instance, std::string Name) {
	if (Instance != 0) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		if (StartOfChildren != 0) {
			DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
			if (EndOfChildren != 0) {
				for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
					int index = *(int*)i;
					if (index > 0) {
						std::string indexChildName = *GetName(index);
						if (strstr(to_lowercase(indexChildName).c_str(), to_lowercase(Name).c_str())) {
							return indexChildName;
						}
					}
				}
			}
		}
	}
	return std::string("0");
}

static int GetChildNamed(int Instance, std::string Name) {
	if (Instance > 10000) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		if (StartOfChildren != 0) {
			DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
			if (EndOfChildren != 0) {
				for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
					int index = *(int*)i;
					if (index > 0) {
						if (strstr(to_lowercase(*GetName(index)).c_str(), to_lowercase(Name).c_str())) {
							return index;
						}
					}
				}
			}
		}
	}
	return int(0);
}

static int GetLocalPlayer(int Players) {
	int localPlayer = 0;
	if (Players > 10000) {
		__try {
			localPlayer = *(int*)(Players + 0x154);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return localPlayer;
}

static int GetUserId(int Player) {
	int userId = 0;
	if (Player > 10000) {
		__try {
			userId = *(int*)(Player + 0x0A8);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return userId;
};

static int FindFirstPlayer(int Instance, std::string Name) {
	if (Instance > 10000) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		if (Name == "me") {
			return GetLocalPlayer(Players);
		}
		else {
			if (StartOfChildren != 0) {
				DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
				if (EndOfChildren != 0) {
					for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
						int index = *(int*)i;
						if (index != 0) {
							try {
								if (strstr(to_lowercase(*GetName(index)).c_str(), to_lowercase(Name).c_str())) {
									return index;
								}
							}
							catch (std::exception) {}
							catch (...) {}
						}
					}
				}
			}
		}
	}
	return int(0);
}

static int GetFilteringEnabled(int Workspace) {
	if (Workspace > 0) {
		return *(BYTE*)(Workspace + 0x24B);
	}
	return int(0);
}

static int GetRobloxLocked(int Ins) {
	if (Ins > 0) {
		return *(BYTE *)(Ins + 35);
	}
	return int(0);
}

static int GetAccountAge(int Player) {
	if (Player > 0) {
		return *(BYTE*)(Player + 0x214);
	}
	return int(0);
}

static int GetCurrentCamera(int Workspace) {
	int Camera = 0;
	if (Workspace != 0) {
		__try {
			Camera = *(int*)(Workspace + 0x1FC);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return Camera;
}

static int GetCameraType(int Camera) {
	int CameraType = 0;
	if (Camera != 0) {
		__try {
			CameraType = *(int*)(Camera + 0x19C);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return CameraType;
}

static void SetCameraType(int camera, int type) {
	if (camera > 0) {
		__try {
			*(int*)(camera + 0x19C) = type;
			Replicate(camera, CameraType_PropDescriptor);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
}

static std::string GetFullName(int Inst) {
	if (Inst > 0) {
		int Parent = GetParent(Inst); Sleep(1);
		if (Parent != DataModel) {
			std::string Name = GetFullName(Parent);
			return Name + "." + *GetName(Inst);
		}
		else {
			return *GetName(Inst);
		}
	}
	return std::string("error.");
}

static int GetHumanoidFromPlayer(int player) {
	int Character = GetCharacterFromPlayer(player);
	if (Character != 0) {
		int Humanoid = FindFirstClass(Character, "Humanoid");
		if (Humanoid != 0) {
			return Humanoid;
		}
	}
	return int(0);
}

static int GetPlayer(std::string Name) {
	if (Name == "me") {
		int Player = GetLocalPlayer(Players);
		if (Player != 0) {
			return Player;
		}
	}
	else {
		int Player = FindFirstPlayer(Players, Name);
		if (Player != 0) {
			return Player;
		}
	}
	return int(0);
}

static void rDestroy(int Instance, std::string Name) { // NameDestroyer
	if (Instance != 0) {
		std::string instanceName = GetChildName(Instance, Name);
		if (instanceName != "0") {
			int instance = FindFirstChild_PartOf(Instance, instanceName);
			if (instance != 0) {
				setParent(instance, Lighting);
			}
		}
	}
}

static void cDestroy(int Instance, const char* Name) { // ClassDestroyer
	if (Instance > 10000) {
		int wot = FindFirstClass(Instance, Name);
		if (wot != 0) {
			__try {
				setParent(wot, Lighting);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}
	}
}

static void iDestroy(int Instance) { // InstanceDestroyer
	if (Instance > 10000) {
		__try {
			setParent(Instance, Lighting);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
}

static int LocalPlayer_Get(const char* name) {
	if (LocalPlayer != 0) {
		int instance = FindFirstClass(LocalPlayer, name);
		if (instance != 0) {
			return int(instance);
		}
	}
	return int(0);
}

static int LocalPlayer_Find(const char* name) {
	if (LocalPlayer != 0) {
		int instance = FindFirstChild_lower(LocalPlayer, name);
		if (instance != 0) {
			return int(instance);
		}
	}
	return int(0);
}

static BOOL CheckIfArchivable(int Instance) {
	if (*(BYTE*)(Instance + 0x21) == 1) {
		return TRUE;
	}
	return FALSE;
}

////////////////////////////////////////////////////////////
////////////////////////	BETA	////////////////////////
////////////////////////////////////////////////////////////

static std::vector<std::string> GetArguments(std::string input) {
	std::vector<std::string> rtn;

	if (input[0] == ' ') {
		input = input.substr(1);
	}
	//BYTE
	std::size_t size = input.size();
	DWORD pos1 = 0;

	for (std::size_t i = 0; i < size; ++i) {
		if (input[i] == ' ') {
			rtn.push_back(input.substr(pos1, i - pos1));
			pos1 = i + 1;
		}
		else if (i == size - 1) {
			rtn.push_back(input.substr(pos1, i - pos1 + 1));
			pos1 = i + 1;
		}
	}
	return rtn;
}

static int GetCharacter(std::string Name) {
	if (Name != "") {
		if (Name == "me") {
			int Player = LocalPlayer;
			if (Player != 0) {
				return GetCharacterFromPlayer(Player);
			}
		}
		else {
			int Player = FindFirstPlayer(Players, Name);
			if (Player != 0) {
				return GetCharacterFromPlayer(Player);
			}
		}
	}
	return int(0);
}

static std::vector<DWORD> GetPlayers() { // Search: GetPlayers
	std::vector<DWORD> rtn;
	InstanceTable instanceTable = GetInstanceTable(Players, 0x138);

	if (!instanceTable.valid)
		return rtn;

	DWORD inc = instanceTable.begin;
	DWORD incCompare = instanceTable.stop;

	while (inc < incCompare) {
		rtn.push_back(*(DWORD*)inc);
		inc += 8;
	}
	return rtn;
}

static std::vector<DWORD> GetTeams() { // Search: GetTeams
	std::vector<DWORD> rtn;
	InstanceTable instanceTable = GetInstanceTable(Teams, 0x4C);

	if (!instanceTable.valid)
		return rtn;

	DWORD inc = instanceTable.begin;
	DWORD incCompare = instanceTable.stop;

	while (inc < incCompare) {
		rtn.push_back(*(DWORD*)inc);
		inc += 8;
	}
	return rtn;
}

static std::vector<std::string> SplitString(std::string String, char Delimiter) {
	std::stringstream test(String);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, Delimiter)) {
		seglist.push_back(segment);
	}

	return seglist;
}

static std::vector<DWORD> GetPlayersFromMessage(std::vector<std::string> argList) {
	std::vector<DWORD> rtn;
	std::string input;
	if (argList.size() >= 1) {
		input = argList[0];//argList[i];
						   //std::string Compare = GetPlayerName(input.data(), loopCheck).c_str();
		if (input == "me") {
			rtn.push_back(LocalPlayer);
		}
		else if (input == "all" || input == "everyone") {
			rtn = GetPlayers();
		}
		else if (input == "guests") {
			std::vector<DWORD> players = GetPlayers();
			for (std::size_t i = 0; i < players.size(); i++) {
				if (players[i] != 0) {
					std::string name = *GetName(players[i]);
					if (name.find("Guest ") != std::string::npos) {
						rtn.push_back(players.at(i));
					}
				}
			}
		}
		else if (input == "others") {
			std::vector<DWORD> players = GetPlayers();
			for (std::size_t i = 0; i < players.size(); ++i) {
				if (players[i] != 0 && LocalPlayer != players[i]) {
					rtn.push_back(players[i]);
				}
			}
		}
		else if (input.find(",") != std::string::npos) {
			std::vector<std::string> PlayersToLookFor = SplitString(input, ',');
			for (std::size_t c = 0; c < PlayersToLookFor.size(); c++) {
				std::string Player = PlayersToLookFor.at(c);
				if (Player == "me") {
					rtn.push_back(GetLocalPlayer(Players));
					continue;
				}
				std::vector<DWORD> players = GetPlayers(); // fixed
				for (std::size_t i = 0; i < players.size(); ++i) {
					if (players[i] != 0) {
						std::string PlayerName = *GetName(players[i]);
						std::string lowerT = to_lowercase(PlayerName);//->c_str());
						if (lowerT.find(to_lowercase(Player.c_str())) != std::string::npos) {
							rtn.push_back(players[i]);
						}
					}
				}
			}
		}
		else {
			std::vector<DWORD> players = GetPlayers(); // fixed
			for (std::size_t i = 0; i < players.size(); ++i) {
				if (players[i] != 0) {
					std::string PlayerName = *GetName(players[i]);
					std::string lowerT = to_lowercase(PlayerName.c_str());
					if (lowerT.find(to_lowercase(input.c_str())) != std::string::npos) {
						rtn.push_back(players[i]);
					}
				}
				//if ((**GetName(players[i])).c_str() == Compare) {
				//	rtn.push_back(players[i]);
				//}
			}
		}
	}
	return rtn;
}

static int GetRandomPlayer() {
	std::vector<DWORD> Players = GetPlayers();
	if (Players.size() > 0) {
		std::random_shuffle(Players.begin(), Players.end());
		return Players[0];
	}
	return int(0);
}

static int TorsoExists(int Player) {
	if (Player != 0) {
		int Character = GetCharacterFromPlayer(Player);
		if (Character != 0) {
			int Torso = FindFirstChild_PartOf(Character, "Torso");
			if (Torso != 0) {
				return Torso;
				//return TRUE;
			}
		}
	}
	return NULL;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

static int SetChildrenParent(int Instance, int newParent) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	if (StartOfChildren != 0) {
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
			__try {
				setParent(*(int*)i, newParent);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}
	}
	return 0;
}

static BOOL InstanceExists(int Instance) {
	if (Instance > 1000) {
		try {
			std::string name = *GetName(Instance);
			if (*GetName(*(int*)Instance) == std::string("0")) {
				return FALSE;
			}
			else if (name.size() != 0) {
				return TRUE;
			}
			else {
				return FALSE;
			}
		}
		catch (std::exception) {}
		catch (...) {}
	}
	return FALSE;
}

static int TemporaryGetTool(std::string ToolName) {
	int Tool = 0;
	InsertService = FindFirstClass(DataModel, "InsertService");
	std::vector<DWORD> InsertServiceChildren = GetChildren(InsertService);
	for (std::size_t j = 0; j < InsertServiceChildren.size(); j++) {
		//if (FoldersFound2 == FoldersFound) {
		if (Tool == 0) {
			std::string ClassType = GetClass(InsertServiceChildren[j]);
			if (ClassType == "Folder") {
				//std::cout << "4";
				std::vector<DWORD> FolderChildren = GetChildren(InsertServiceChildren[j]);
				for (std::size_t j = 0; j < FolderChildren.size(); j++) {
					//std::cout << "5";
					if (Tool == 0) {
						std::string ClassType = GetClass(FolderChildren[j]);
						if (ClassType == "Model") {
							//std::cout << "6";
							if (Tool == 0) {
								//std::cout << "7";
								std::vector<DWORD> ModelChildren = GetChildren(FolderChildren[j]);
								for (std::size_t j = 0; j < ModelChildren.size(); j++) {
									//std::cout << "8";
									std::string ClassType = GetClass(ModelChildren[j]);
									if (ClassType == "Tool") {
										std::string ToolName1 = *GetName(ModelChildren[j]);
										if (ToolName1 == ToolName) {
											//std::cout << "9";
											Tool = ModelChildren[j];
											std::cout << "Sword found!\n";
											//std::cout << " = 10";
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return Tool;
}

static void RecurseUnanchor(int Ins) {
	std::vector<DWORD> Children = GetChildren(Ins);
	for (std::size_t i = 0; i < Children.size(); i++) {
		DWORD Child = Children.at(i);
		if (strcmp(GetClass(Child), "Part") == 0) {
			SetAnchored(Child, 0);
		}
		RecurseUnanchor(Child);
	}
	return;
}

//_______________________________________________________________________________________________

static BYTE FilteringEnabledCheck(int Workspace) {
	if (Workspace > 1000) {
		__try {
			return *(BYTE*)(Workspace + 0x24B);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return BYTE(-1337);
}

static void SetCameraSubject(int camera, int subject) {
	if (camera > 1000) {
		__try {
			*(int*)(camera + 0x1A0) = subject;
			Replicate(camera, CameraSubject_PropDescriptor);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
}

static char RobloxLocked(int instance) {
	if (instance > 0) {
		return *(BYTE*)(instance + 35);
	}
	return char(0);
}

static int PlaceId;//rbxaddy(0x237EC54);

static void AntiHax() {
	if (PlaceId == 177052655) {
		do {
			try {
				int Player = LocalPlayer;
				if (Player != 0) {
					int PlayerGui = FindFirstChild_PartOf(Player, "PlayerGui"); Sleep(250);
					if (PlayerGui != 0) {
						std::vector<DWORD> CharacterChildren = GetChildren(PlayerGui);
						for (std::size_t j = 0; j < CharacterChildren.size(); j++) {
							std::string InstanceName = *GetName(CharacterChildren[j]);
							if (InstanceName == "NO FF") {
								iDestroy(CharacterChildren[j]);
							}
							else if (InstanceName == "AntiSpeedHack") {
								iDestroy(CharacterChildren[j]);
							}
							else if (InstanceName == "AntiTheftHacking") {
								iDestroy(CharacterChildren[j]);
							}
						}
					}
				}
			}
			catch (std::exception) { Sleep(2250); }
			/*__except (EXCEPTION_EXECUTE_HANDLER) {
			Sleep(1000); // do absolutely fucking nothing
			}*/
		} while (true);
	}
}