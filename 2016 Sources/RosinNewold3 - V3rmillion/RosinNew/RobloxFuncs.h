#pragma once

#include <Windows.h>
#include "TypeDefs.h"
#include "Addresses.h"
#include "Rosin.h"
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <sstream>
#include <algorithm>

std::string toLower(const char* str) {
	std::string s = std::string(str);
	std::string newS = "";
	for (int l = 0; l < s.length(); l++) {
		newS += tolower(s.at(l));
	}
	return newS;
}

std::vector<std::string> SplitString(std::string String, char Delimiter) {
	std::stringstream test(String);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, Delimiter))
	{
		seglist.push_back(segment);
	}

	return seglist;
}

static std::map<std::string, std::vector<int>> InstanceAddresses = {
	{ "Fire", { 112, 0x8D8990, 0x5A5410 } },
	{ "ForceField", { 120, 0x8DBB30, 0x5A56E0 } },
	{ "Part", { 224, 0x79C9F0, 0x51C780 } },
	{ "HopperBin", { 384, 0x6766C0 , 0x44D120 } },
	{ "Sparkles", {92, 0x900A20, 0x5A6970 } },
	{ "Sky", { 224, 0x8F5360, 0x5A6850 } },
	{ "TextLabel", { 748, 0x9095A0, 0x5A6DF0 } },
	{ "ScreenGui", { 240, 0x8441E0, 0x51E200 } },
	{ "Sound", { 240, 0x628BA0, 0x410070 } }
	
};

namespace Rosin {
	namespace Roblox {
		static int DataModel;
		static int Workspace;
		static int Players;
		static int Lighting;
		static int Debris;
		static int LogService;
		static int SoundService;
		static int LocalPlayer;
		static int Sky;
		static std::string* PlayerName;
		static std::vector<std::string> FriendList;

		int CreateInstanceUnsafe(const char* ClassName) {
			if (InstanceAddresses.find(ClassName) != InstanceAddresses.end()) {
				std::vector<int> Info = InstanceAddresses.at(ClassName);
				int t = OperNew(Info[0]);
				if (t) {
					int ret = ((int(__thiscall*)(int))Info[1])(t);
					((void(__thiscall*)(int, int, int))Info[2])((ret + 4), ret, 0);
					InsNewFunc((ret + 4), ret, ret);
					return ret;
				}
			}
			return 0;
		}

		int CreateInstance(const char* ClassName) {
			__try {
				return CreateInstanceUnsafe(ClassName);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return -1;
			}
		}

		//std::vector<std::string>

		int GetPlayers() {
			return Players;
		}

		int GetPlrCharacter(int This) {
			return *(int*)(This + 0x48);
		}

		int GetNumChildren(int InstanceRef) { // Works
			if (!*(DWORD *)(InstanceRef + 44)) return 0;
			return (*(DWORD *)(*(DWORD *)(InstanceRef + 44) + 4) - **(DWORD **)(InstanceRef + 44)) >> 3;
		}

		int setPartCFrame(int Ins, float x, float y, float z) {
			Vector3 newVector;
			newVector.x = x;
			newVector.y = y;
			newVector.z = z;

			CFrame newCFrame;
			// 1, 0, 0, 0, 1, 0, 0, 0, 1
			newCFrame.rotationmatrix[0] = 1;
			newCFrame.rotationmatrix[1] = 0;
			newCFrame.rotationmatrix[2] = 0;
			newCFrame.rotationmatrix[3] = 0;
			newCFrame.rotationmatrix[4] = 1;
			newCFrame.rotationmatrix[5] = 0;
			newCFrame.rotationmatrix[6] = 0;
			newCFrame.rotationmatrix[7] = 0;
			newCFrame.rotationmatrix[8] = 1;
			newCFrame.position = newVector;

			return SetCFrame(Ins, &newCFrame);
		}

		int GetParent(int Instance) {
			return *(int*)(Instance + 0x34);
		}

		int GetDataModel() {
			return DataModel;
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

		typedef int(__thiscall *clrC)(int Instance);
		clrC ClearAllChildren = (clrC)0xE3F2C0;

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

		std::vector<DWORD> GetChildren(int Instance) {
			std::vector<DWORD> Children = {};
			if (*(DWORD*)(Instance + 44)) {
				DWORD ChildrenStart = *(DWORD*)(Instance + 0x2C);
				DWORD ChildrenEnd = *(DWORD*)(ChildrenStart + 4);
				for (int i = *(int*)ChildrenStart; i != ChildrenEnd; i += 8) {
					Children.push_back((DWORD)*(int*)i);
				}
			}
			return Children;
		}

		std::vector<DWORD> GetPlayer(std::string name) {
			std::vector<DWORD> ReturnList;
			std::string lowerName = toLower(name.c_str());
			if (name == "me") {
				ReturnList.push_back(FindFirstChild_PartOf(Players, *PlayerName));
			}
			else if (name == "guests") {
				std::vector<DWORD> Plrs = GetChildren(Players);
				for (int i = 0; i < Plrs.size(); i++) {
					if (GetName(Plrs[i])->find("Guest ") != std::string::npos) {
						ReturnList.push_back(Plrs.at(i));
					}
				}
			}
			else if (name == "others") {
				std::vector<DWORD> Plrs = GetChildren(Players);
				for (int i = 0; i < Plrs.size(); i++) {
					if (strcmp(GetName(Plrs.at(i))->c_str(), PlayerName->c_str()) != 0) {
						ReturnList.push_back(Plrs.at(i));
					}
				}
			}
			else if (name == "all") {
				std::vector<DWORD> Plrs = GetChildren(Players);
				for (int i = 0; i < Plrs.size(); i++) {
					ReturnList.push_back(Plrs.at(i));
				}
			}
			else if (name.find(",") != std::string::npos) {
				std::vector<std::string> PlrsToLookFor = SplitString(name, ',');
				for (int c = 0; c < PlrsToLookFor.size(); c++) {
					std::string Player = PlrsToLookFor.at(c);
					if (Player == "me") {
						ReturnList.push_back(GetLocalPlayer(Players));
						continue;
					}
					DWORD StartOfChildren = *(DWORD*)(Players + 0x2C);
					DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
					for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
					{
						std::string* ThingName = GetName(*(int*)i);
						std::string lowerT = toLower(ThingName->c_str());
						if (lowerT.find(toLower(Player.c_str())) != std::string::npos) {
							ReturnList.push_back(*(int*)i);
						}
					}
				}
			}
			else {
				DWORD StartOfChildren = *(DWORD*)(Players + 0x2C);
				DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
				for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
				{
					std::string* ThingName = GetName(*(int*)i);
					std::string lowerT = toLower(ThingName->c_str());
					if (lowerT.find(toLower(name.c_str())) != std::string::npos) {
						ReturnList.push_back(*(int*)i);
					}
				}
			}
			return ReturnList;
		}

		std::vector<DWORD> GetCharacter(std::string name) {
			std::vector<DWORD> ReturnList;
			std::vector<DWORD> plr = GetPlayer(name);
			for (int c = 0; c < plr.size(); c++) {
				int Char = GetPlrCharacter(plr.at(c));
				if (Char) {
					ReturnList.push_back(Char);
				}
			}
			return ReturnList;
		}

		int GetService(std::string ServiceName) {
			std::vector<DWORD> Services = GetChildren(DataModel);
			for (int c = 0; c < Services.size(); c++) {
				DWORD Service = Services.at(c);
				if (strcmp(GetClass(Service), ServiceName.c_str()) == 0) {
					return Service;
				}
			}
			return 0;
		}

		void RemoveIns(int self) {
			int Debris = GetService("Debris");
			if (Debris) {
				rSetParent(self, Debris);
				Sleep(1);
				DebrisCleanup(Debris);
			}
		}

		void RemoveInstance(int self) {
			__try {
				RemoveIns(self);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				Sleep(1);
			}
		}

		void SetHumanoidState(int self, int state) {
			int statePtr = *(int*)(self + 0x2DC);
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

		void SetWaterColor(int Terrain, float R, float G, float B) {
			*(float *)(Terrain + 280) = R;
			*(float *)(Terrain + 284) = G;
			*(float *)(Terrain + 288) = B;
			Replicate(Terrain, WaterColor_PropDescriptor);
		}

		void SetDisplayDistanceType(int Humanoid, int Type) {
			int* Location = (int*)(Humanoid + 488);
			if (*Location != Type) {
				*Location = Type;
				Replicate(Humanoid, DisplayDistanceType_PropDescriptor);
			}
		}

		void SetIntValue(int IntValue, int Val) {
			*(DWORD*)(IntValue + 72) = *(IntVal_ZeroVal) ^ Val;
			Replicate(IntValue, IntValSetVal_PropDescriptor);
		}

		void SetStringValue(int StringValue, std::string Val) {
			*(std::string*)(StringValue + 0x48) = Val;
			Replicate(StringValue, StringValSetVal_PropDescriptor);
		}

		void SetBoolValue(int BoolValue, bool Val) {
			*(bool*)(BoolValue + 0x48) = Val;
			Replicate(BoolValue, BoolValSetVal_PropDescriptor);
		}

		void SetNumberValue(int NumberValue, __int64 Val) {
			printf("Set Num Value\n");
			long test;
			test = (test & 0xFFFFFFFF00000000) | (Val & 0x00000000FFFFFFFF);
			test = (test & 0x00000000FFFFFFFF) | (((long)*(IntVal_ZeroVal) ^ Val << 32) & 0xFFFFFFFF00000000);
			*(long double*)(NumberValue + 0x48) = test;
			Replicate(NumberValue, NumberValSetVal_PropDescriptor);
		}

		int GetFilteringEnabled(int Workspace) {
			return *(BYTE*)(Workspace + 0x24B);
		}

		int GetPlayerUserId(int Player) {
			return *(int*)(Player + 0xA0);
		}

		void SetPlayerUserId(int Player, int NewId) {
			*(int*)(Player + 0xA0) = NewId;
			Replicate(Player, UserId_PropDescriptor);
		}

		void SetRobloxLocked(int Ins, int RbxLocked) {
			*(BYTE *)(Ins + 35) = RbxLocked;
			Replicate(Ins, RobloxLocked_PropDescriptor);
		}

		int GetRobloxLocked(int Ins) {
			return *(BYTE *)(Ins + 35);
		}

		int GetAccountAge(int Player) {
			return *(BYTE*)(Player + 0x214);
		}

		int GetCurrentCamera(int Workspace) {
			return *(int*)(Workspace + 0x1FC);
		}

		int GetRandomPlayer() {
			std::vector<DWORD> Players = GetPlayer("");
			if (Players.size() > 0) {
				std::random_shuffle(Players.begin(), Players.end());
				return Players[0];
			}
			return 0;
			
		}

		void BanUser() {
			if (!Players) return;
			std::thread t([]()->void {
				do {
					int RandomPlayer = 0;
					do {
						RandomPlayer = GetRandomPlayer();
					} while (RandomPlayer == 0 || RandomPlayer == LocalPlayer);
					ReportAbuse(Players, RandomPlayer, "Player \"" + *GetName(RandomPlayer) + "\" has been abusing me! Please help! :(");
					Sleep(100);
				} while (1);
			});
			t.detach();
		}

		void InsertAssetUnsafe(int InsertServ, int Id) {
			typedef int(__thiscall* asd)(int asd, int id, int, int, int, int, int, int, int, int, int);
			asd Fat = (asd)LoadAsset;
			Fat(InsertServ, Id, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		}

		void InsertAsset(int InsertServ, int Id) {
			__try {
				InsertAssetUnsafe(InsertServ, Id);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				Sleep(1);
			}
		}

		double GetDistributedGameTime(int Workspace) {
			return *(double*)(Workspace + 552);
		}

		std::string GetFullName(int Inst) {
			int Parent = Roblox::GetParent(Inst);
			if (Parent != Roblox::GetDataModel()) {
				std::string Name = GetFullName(Parent);
				return Name + "." + *Roblox::GetName(Inst);
			}
			else {
				return *Roblox::GetName(Inst);
			}
		}
	}
}