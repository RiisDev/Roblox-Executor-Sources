#pragma once

/* Includes */

#include "Data.h"

using namespace std;

constexpr unsigned int Command(const char* str, int h = 0) {
	return !str[h] ? 5381 : (Command(str, h + 1) * 33) ^ str[h];
}

namespace Settings
{
	namespace Hotkeys
	{
		static bool Active;
	}
	namespace Mouse
	{
		static bool Teleport;
		static bool XYZ_Position;
		static bool Destroy;
		static bool Explode;
	}
	namespace ChatHook
	{
		static bool Active;
		static std::string Prefix = ";";
		static std::string LastCommand = "";
	}
}

static std::vector<std::string> SplitArguments(std::string string)
{
	std::vector<std::string> elements;
	std::stringstream start(string);
	std::istream_iterator<std::string> begin(start);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vectorStrings(begin, end);
	return vectorStrings;
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

static const char* GetClass(int self) {
	if (self) {
		const char* Class = "0";
		__try {
			Class = (const char*)(*(int(**)(void))(*(int *)self + 16))();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
		return Class;
	}
	return ("0");
}

static std::string* GetName(int Instance) {
	if (Instance) {
		__try {
			return (std::string*)(*(int*)(Instance + 0x28));
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return (std::string*)"0";
}

static int FindFirstChild(int Instance, std::string Name) {
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

static int FindFirstClass(int Instance, const char* ClassName) {
	if (Instance > 10000) {
		int StartOfChildren = *(int*)(Instance + 0x2C);
		if (StartOfChildren != 0) {
			int EndOfChildren = *(int*)(StartOfChildren + 4);
			if (EndOfChildren != 0) {
				for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
					int index = *(int*)i;
					if (index > 0) {
						try {
							if (memcmp(GetClass(index), ClassName, strlen(ClassName)) == 0) {
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

static std::string Lowercase(std::string string) {
	std::transform(string.begin(), string.end(), string.begin(), std::ptr_fun<int, int>(std::tolower));
	return string;
}

struct InstanceTable {
	DWORD begin;
	DWORD stop;
	BYTE valid;
};

static InstanceTable GetInstanceTable(DWORD instance, DWORD offset) {
	InstanceTable rtn;

	__try {
		if (!*(DWORD*)(instance + offset)) {
			rtn.valid = false;
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

static std::vector<DWORD> GetPlayers() {
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

static std::vector<DWORD> GetChildren(DWORD instance) {
	std::vector<DWORD> rtn;
	if (instance > 1000) {
		InstanceTable instanceTable = GetInstanceTable(instance, 0x2C);
		if (!instanceTable.valid) {
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

static std::string GetPlayerName(std::string Name) {
	DWORD StartOfChildren = *(DWORD*)(Players + 0x2C);
	if (StartOfChildren != 0) {
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		if (EndOfChildren != 0) {
			if (Name == "me") {
				return *GetName(PlayerInstance);
			}
			for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
				int index = *(int*)i;
				if (index > 0) {
					std::string indexPlayerName = *GetName(index);
					if (strstr(Lowercase(indexPlayerName).c_str(), Lowercase(Name).c_str())) {
						return indexPlayerName;
					}
				}
			}
		}
	}
	return std::string("0");
}



static std::vector<DWORD> GetPlayersFromArguments(std::vector<std::string> argList) {
	std::vector<DWORD> rtn;
	std::string input;
	if (argList.size() >= 1) {
		input = argList[0];	
		if (input == "me") {
			rtn.push_back(PlayerInstance);
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
				if (players[i] != 0 && PlayerInstance != players[i]) {
					rtn.push_back(players[i]);
				}
			}
		}
		else if (input.find(",") != std::string::npos) {
			std::vector<std::string> PlayersToLookFor = SplitString(input, ',');
			for (std::size_t c = 0; c < PlayersToLookFor.size(); c++) {
				std::string Player = PlayersToLookFor.at(c);
				if (Player == "me") {
					rtn.push_back(PlayerInstance);
					continue;
				}
				std::vector<DWORD> players = GetPlayers();
				for (std::size_t i = 0; i < players.size(); ++i) {
					if (players[i] != 0) {
						std::string PlayerName = *GetName(players[i]);
						std::string lowerT = Lowercase(PlayerName);
						if (lowerT.find(Lowercase(Player.c_str())) != std::string::npos) {
							rtn.push_back(players[i]);
						}
					}
				}
			}
		}
		else {
			std::vector<DWORD> players = GetPlayers();
			for (std::size_t i = 0; i < players.size(); ++i) {
				if (players[i] != 0) {
					std::string PlayerName = *GetName(players[i]);
					std::string lowerT = Lowercase(PlayerName.c_str());
					if (lowerT.find(Lowercase(input.c_str())) != std::string::npos) {
						rtn.push_back(players[i]);
					}
				}
			}
		}
	}
	return rtn;
}

static void __cdecl RunCommand(const std::string& CommandString) {

	std::vector<std::string> FullCommand = SplitArguments(CommandString);

	if (FullCommand.size() == 0) { return; }

	std::vector <DWORD> players;

	std::string Cmd = FullCommand.front(); FullCommand.erase(FullCommand.begin());

	players = GetPlayersFromArguments(FullCommand);

	switch (Command(Cmd.c_str())) {

	case Command("sky"): case Command("skybox"): {
		std::string ID;

		try {
			ID = "http://www.roblox.com/asset/?id=" + FullCommand.at(0);
		}
		catch (...) {
			break;
		}
		try {
			GetService(L, "Lighting");
			GetGlobal(L, "Instance");
			GetField(L, -1, "new");
			PushString(L, "Sky");
			PushValue(L, -4);
			Pcall(L, 2, 1, 0);

			PushString(L, ID.c_str());
			SetField(L, -2, "SkyboxBk");

			PushString(L, ID.c_str());
			SetField(L, -2, "SkyboxDn");

			PushString(L, ID.c_str());
			SetField(L, -2, "SkyboxFt");

			PushString(L, ID.c_str());
			SetField(L, -2, "SkyboxLf");

			PushString(L, ID.c_str());
			SetField(L, -2, "SkyboxRt");

			PushString(L, ID.c_str());
			SetField(L, -2, "SkyboxUp");

			PushString(L, "Flame_Skybox");
			SetField(L, -2, "Name");

			PushValue(L, -2);
			Pcall(L, 1, 0, 0);
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("day"):
	{
		try {
			GetService(L, "Lighting");
			PushString(L, "14");
			SetField(L, -2, "TimeOfDay");
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("time"):
	{
		std::string Time;

		try {
			Time = FullCommand.at(0) + "00:00";
		}
		catch (...) {
			break;
		}

		try {
			GetService(L, "Lighting");
			PushString(L, Time.c_str());
			SetField(L, -2, "TimeOfDay");
			Pcall(L, 1, 0, 0);
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("night"):
	{
		try {
			GetService(L, "Lighting");
			PushString(L, "0");
			SetField(L, -2, "TimeOfDay");
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("btools"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Backpack");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "HopperBin");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
				PushNumber(L, 1);
				SetField(L, -2, "BinType");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Backpack");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "HopperBin");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
				PushNumber(L, 2);
				SetField(L, -2, "BinType");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Backpack");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "HopperBin");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
				PushNumber(L, 3);
				SetField(L, -2, "BinType");

				GetGlobal(L, "game");
				GetField(L, -1, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Backpack");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "HopperBin");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
				PushNumber(L, 4);
				SetField(L, -2, "BinType");
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}

	case Command("unneon"): case Command("unneonify"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");

				PushString(L, "Plastic");
				SetField(L, -2, "Material");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");

				PushString(L, "Plastic");
				SetField(L, -2, "Material");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");

				PushString(L, "Plastic");
				SetField(L, -2, "Material");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");

				PushString(L, "Plastic");
				SetField(L, -2, "Material");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");

				PushString(L, "Plastic");
				SetField(L, -2, "Material");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("neon"): case Command("neonify"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");

				PushString(L, "Neon");
				SetField(L, -2, "Material");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");

				PushString(L, "Neon");
				SetField(L, -2, "Material");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");

				PushString(L, "Neon");
				SetField(L, -2, "Material");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");

				PushString(L, "Neon");
				SetField(L, -2, "Material");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");

				PushString(L, "Neon");
				SetField(L, -2, "Material");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "HumanoidRootPart");

				PushString(L, "Neon");
				SetField(L, -2, "Material");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("kill"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "BreakJoints");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}
	case Command("explode"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Workspace");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Explosion");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				GetGlobal(L, "game");
				GetField(L, -1, "Workspace");
				GetField(L, -1, Player.c_str());

				GetField(L, -1, "Torso");

				GetField(L, -1, "Position");
				SetField(L, -6, "Position");
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}

	case Command("box"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "SelectionBox");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushValue(L, -3);
				SetField(L, -2, "Adornee");
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}

	case Command("unbox"): case Command("nobox"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "SelectionBox");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}

	case Command("unff"): case Command("noff"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "ForceField");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}


	case Command("unsmoke"): case Command("nosmoke"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				GetField(L, -1, "Smoke");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}

	case Command("ff"): case Command("forcefield"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "ForceField");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}


	case Command("smoke"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Smoke");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}

	case Command("unsparkles"): case Command("nosparkles"): case Command("nosp"): case Command("unsp"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				GetField(L, -1, "Sparkles");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("sp"): case Command("sparkles"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Sparkles");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("light"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "PointLight");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("unlight"): case Command("nolight"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				GetField(L, -1, "PointLight");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("unfire"): case Command("nofire"): case Command("unbigfire"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				GetField(L, -1, "Fire");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("unbodyfire"): case Command("nobodyfire"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "Fire");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				GetField(L, -1, "Fire");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");
				GetField(L, -1, "Fire");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");
				GetField(L, -1, "Fire");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");
				GetField(L, -1, "Fire");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");
				GetField(L, -1, "Fire");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("bigfire"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Fire");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushNumber(L, 15);
				SetField(L, -2, "Size");
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("fire"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Fire");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("bodyfire"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Fire");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Fire");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Fire");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Fire");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Fire");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Fire");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("ws"): case Command("walkspeed"): case Command("speed"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				float WalkSpeed;
				try {
					WalkSpeed = stod(FullCommand.at(1));
				}
				catch (...) {
					WalkSpeed = 16;
				}
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");

				PushNumber(L, double(WalkSpeed));
				SetField(L, -2, "WalkSpeed");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("sethealth"): case Command("health"): case Command("shealth"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				float Health;
				try {
					Health = stod(FullCommand.at(1));
				}
				catch (...) {
					Health = 100;
				}
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");

				PushNumber(L, double(Health));
				SetField(L, -2, "MaxHealth");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("jp"): case Command("jumppower"): case Command("jpower"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				float JumpPower;
				try {
					JumpPower = stod(FullCommand.at(1));
				}
				catch (...) {
					JumpPower = 16;
				}
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");

				PushNumber(L, double(JumpPower));
				SetField(L, -2, "JumpPower");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("god"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");

				PushNumber(L, HUGE_VAL);
				SetField(L, -2, "MaxHealth");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("ungod"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");

				PushNumber(L, 100);
				SetField(L, -2, "MaxHealth");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("heal"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");

				PushNumber(L, HUGE_VAL);
				SetField(L, -2, "Health");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("rshirt"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Shirt");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("rtshirt"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Shirt Graphic");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("fuckup"):
	{
		try {

		}
		catch (...) {
			EmptyStack(L);
		}

		break;
	}

	case Command("respawn"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				GetGlobal(L, "Workspace");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Model");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushString(L, Player.c_str());
				SetField(L, -2, "Name");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Part");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushString(L, "Torso");
				SetField(L, -2, "Name");

				PushBoolean(L, true);
				SetField(L, -2, "Anchored");

				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 10000);
				PushNumber(L, 10000);
				PushNumber(L, 10000);
				Pcall(L, 3, 1, 0);
				SetField(L, -3, "Position");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Humanoid");
				PushValue(L, -4);
				Pcall(L, 2, 0, 0);

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Torso");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Humanoid");
				PushValue(L, -4);
				SetField(L, -2, "Torso");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());

				GetService(L, "Players");
				GetField(L, -1, "LocalPlayer");

				PushValue(L, -4);
				SetField(L, -2, "Character");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}


	case Command("clone"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				PushBoolean(L, true);
				SetField(L, -2, "Archivable");

				GetField(L, -1, "Clone");
				PushValue(L, -2);
				Pcall(L, 1, 1, 0);

				GetGlobal(L, "Workspace");
				PushValue(L, -1);
				SetField(L, -3, "Parent");

				Sleep(10);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("noob"): case Command("noobify"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());

				PushString(L, "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=275836972");
				SetField(L, -2, "CharacterAppearance");
				Pcall(L, 1, 0, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "BreakJoints");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				Sleep(10);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("guest"): case Command("guestify"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());

				PushString(L, "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=1");
				SetField(L, -2, "CharacterAppearance");
				Pcall(L, 1, 0, 0);


				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "BreakJoints");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				Sleep(10);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("rpants"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Pants");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("naked"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				RunCommand("rshirt " + Player);
				RunCommand("rtshirt " + Player);
				RunCommand("rpants " + Player);
			}
			catch (...) {};
		}
		break;
	}

	case Command("bring"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "HumanoidRootPart");
				GetGlobal(L, "Workspace");
				GetField(L, -1, (const char*)PlayerName);
				GetField(L, -1, "HumanoidRootPart");

				GetField(L, -1, "CFrame");
				SetField(L, -5, "CFrame");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("goto"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetGlobal(L, "Workspace");
				GetField(L, -1, (const char*)PlayerName);
				GetField(L, -1, "HumanoidRootPart");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "HumanoidRootPart");

				GetField(L, -1, "CFrame");
				SetField(L, -5, "CFrame");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("tp"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			std::string SecondPlayer = GetPlayerName(FullCommand.back());

			try {
				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "HumanoidRootPart");
				GetGlobal(L, "Workspace");
				GetField(L, -1, SecondPlayer.c_str());
				GetField(L, -1, "HumanoidRootPart");
				GetField(L, -1, "CFrame");
				SetField(L, -5, "CFrame");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("invis"): case Command("invisible"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");


				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("duckinvis"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");
				PushNumber(L, 1);
				SetField(L, -2, "Transparency");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("vis"): case Command("visible"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				PushNumber(L, 0);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");
				PushNumber(L, 0);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");
				PushNumber(L, 0);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");
				PushNumber(L, 0);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");
				PushNumber(L, 0);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				PushNumber(L, 0);
				SetField(L, -2, "Transparency");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("duck"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "SpecialMesh");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				std::string MeshId = "rbxassetid://9419831";
				std::string TextureId = "rbxassetid://9419827";

				PushString(L, TextureId.c_str());
				SetField(L, -2, "TextureId");
				PushString(L, MeshId.c_str());;
				SetField(L, -2, "MeshId");
				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 5);
				PushNumber(L, 5);
				PushNumber(L, 5);
				Pcall(L, 3, 1, 0);
				SetField(L, -3, "Scale");

				RunCommand("duckinvis " + Player);
				RunCommand("naked " + Player);
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}

	case Command("harambe"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "SpecialMesh");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				std::string MeshId = "rbxassetid://430330296";
				std::string TextureId = "rbxassetid://430330316";

				PushString(L, TextureId.c_str());
				SetField(L, -2, "TextureId");
				PushString(L, MeshId.c_str());;
				SetField(L, -2, "MeshId");
				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 0.5);
				PushNumber(L, 0.5);
				PushNumber(L, 0.5);
				Pcall(L, 3, 1, 0);
				SetField(L, -3, "Scale");

				RunCommand("invis " + Player);
				RunCommand("naked " + Player);
			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}

	case Command("bighead"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "Mesh");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 2);
				PushNumber(L, 2);
				PushNumber(L, 2);
				Pcall(L, 3, 1, 0);
				SetField(L, -3, "Scale");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("gravity"):
	{
		int Gravity;
		try {
			Gravity = stod(FullCommand.at(0));
		}
		catch (...) {
			break;
		}

		try {
			GetService(L, "Workspace");
			PushNumber(L, Gravity);
			SetField(L, -2, "Gravity");
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("char"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			std::string Appearance;

			try {
				Appearance = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=" + FullCommand.back() + "&placeId=0";
			}
			catch (...) {
				break;
			}

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());

				PushString(L, Appearance.c_str());
				SetField(L, -2, "CharacterAppearance");
				Pcall(L, 1, 0, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "BreakJoints");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("blockhead"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "Mesh");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("devconsole"): case Command("developerconsole"):
	{
		try {
			GetGlobal(L, "game");
			GetField(L, -1, "CoreGui");
			GetField(L, -1, "RobloxGui");
			GetField(L, -1, "DeveloperConsole");

			PushBoolean(L, true);
			SetField(L, -2, "Visible");
			break;
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("kick"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");
				PushNumber(L, 0);
				SetField(L, -2, "WalkSpeed");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "SkateboardPlatform");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushBoolean(L, true);
				SetField(L, -2, "Anchored");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "SkateboardPlatform");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "HumanoidRootPart");
				GetField(L, -1, "CFrame");
				SetField(L, -5, "CFrame");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "HumanoidRootPart");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("disconnect"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "SkateboardPlatform");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushBoolean(L, true);
				SetField(L, -2, "Anchored");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "SkateboardPlatform");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "HumanoidRootPart");
				GetField(L, -1, "CFrame");
				SetField(L, -5, "CFrame");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "HumanoidRootPart");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("fogend"):
	{
		int FogEnd;
		try {
			FogEnd = stod(FullCommand.at(0));
		}
		catch (...) {
			break;
		}
		try {
			GetService(L, "Lighting");
			PushNumber(L, FogEnd);
			SetField(L, -2, "FogEnd");
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("fogcolor"):
	{
		int R;
		int G;
		int B;
		try {
			R = stod(FullCommand.at(0));
			G = stod(FullCommand.at(1));
			B = stod(FullCommand.at(2));

		}
		catch (...) {
			break;
		}
		try {
			GetService(L, "Lighting");

			GetGlobal(L, "Color3");
			GetField(L, -1, "fromRGB");
			PushNumber(L, R);
			PushNumber(L, G);
			PushNumber(L, B);
			Pcall(L, 3, 1, 0);
			SetField(L, -3, "FogColor");

		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("fogstart"):
	{
		int FogEnd;
		try {
			FogEnd = stod(FullCommand.at(0));
		}
		catch (...) {
			break;
		}
		try {
			GetService(L, "Lighting");
			PushNumber(L, FogEnd);
			SetField(L, -2, "FogStart");
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("smallhead"): case Command("minihead"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "Mesh");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 0.5);
				PushNumber(L, 0.5);
				PushNumber(L, 0.5);
				Pcall(L, 3, 1, 0);
				SetField(L, -3, "Scale");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "Mesh");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 0);
				PushNumber(L, -0.3);
				PushNumber(L, 0);
				Pcall(L, 3, 1, 0);
				SetField(L, -3, "Offset");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}


	case Command("normalhead"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "Mesh");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 1.25);
				PushNumber(L, 1.25);
				PushNumber(L, 1.25);
				Pcall(L, 3, 1, 0);
				SetField(L, -3, "Scale");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "Mesh");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 0);
				PushNumber(L, 0);
				PushNumber(L, 0);
				Pcall(L, 3, 1, 0);
				SetField(L, -3, "Offset");

			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}


	case Command("unduck"): case Command("unharambe"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				GetField(L, -1, "Mesh");
				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				RunCommand("vis " + Player);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("pf"):
	{
		GetGlobal(L, "game");
		GetField(L, -1, "GetService");
		PushValue(L, -2);
		PushString(L, "Players");
		Pcall(L, 2, 1, 0);

		GetField(L, -1, "LocalPlayer");
		GetField(L, -1, "Character");
		GetField(L, -1, "Humanoid");
		GetField(L, -1, "Remove");
		PushValue(L, -2);
		Pcall(L, 1, 0, 0);

		GetGlobal(L, "game");
		GetField(L, -1, "GetService");
		PushValue(L, -2);
		PushString(L, "Players");
		Pcall(L, 2, 1, 0);

		GetField(L, -1, "LocalPlayer");
		GetField(L, -1, "Character");

		GetGlobal(L, "Instance");
		GetField(L, -1, "new");
		PushString(L, "Humanoid");
		PushValue(L, -4);
		Pcall(L, 2, 1, 0);

		PushNumber(L, 50);
		SetField(L, -2, "WalkSpeed");

		PushNumber(L, 100);
		SetField(L, -2, "JumpPower");
		break;
	}

	case Command("minigun"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Backpack");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Tool");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushString(L, "rbxassetid://756406939");
				SetField(L, -2, "TextureId");

				PushString(L, "Flame - Minigun");
				SetField(L, -2, "Name");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Part");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushString(L, "Handle");
				SetField(L, -2, "Name");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 1);
				PushNumber(L, 1);
				PushNumber(L, 4);
				Pcall(L, 3, 1, 0);

				SetField(L, -3, "Size");

				GetService(L, "Players");

				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Backpack");
				GetField(L, -1, "Flame - Minigun");
				GetField(L, -1, "Handle");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "SpecialMesh");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushString(L, "rbxassetid://2806162");
				SetField(L, -2, "MeshId");

				PushString(L, "rbxassetid://756546599");
				SetField(L, -2, "TextureId");
			}
			catch (...)
			{
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("shiny"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				PushNumber(L, 1);
				SetField(L, -2, "Reflectance");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				PushNumber(L, 1);
				SetField(L, -2, "Reflectance");


				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");
				PushNumber(L, 1);
				SetField(L, -2, "Reflectance");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");
				PushNumber(L, 1);
				SetField(L, -2, "Reflectance");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");
				PushNumber(L, 1);
				SetField(L, -2, "Reflectance");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");
				PushNumber(L, 1);
				SetField(L, -2, "Reflectance");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("hidename"): case Command("noname"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "FindFirstChildOfClass");
				PushValue(L, -2);
				PushString(L, "Humanoid");
				Pcall(L, 2, 1, 0);

				PushString(L, "None");
				SetField(L, -2, "DisplayDistanceType");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("visname"): case Command("showname"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "FindFirstChildOfClass");
				PushValue(L, -2);
				PushString(L, "Humanoid");
				Pcall(L, 2, 1, 0);

				PushString(L, "Viewer");
				SetField(L, -2, "DisplayDistanceType");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}


	case Command("rot"): case Command("rotate"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "FindFirstChildOfClass");
				PushValue(L, -2);
				PushString(L, "Humanoid");
				Pcall(L, 2, 1, 0);

				PushBoolean(L, true);
				SetField(L, -2, "AutoRotate");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("norot"): case Command("norotate"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");

				GetField(L, -1, "FindFirstChildOfClass");
				PushValue(L, -2);
				PushString(L, "Humanoid");
				Pcall(L, 2, 1, 0);

				PushBoolean(L, false);
				SetField(L, -2, "AutoRotate");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}


	case Command("disable"): case Command("dis"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");

				GetField(L, -1, "FindFirstChildOfClass");
				PushValue(L, -2);
				PushString(L, "Humanoid");
				Pcall(L, 2, 1, 0);

				PushBoolean(L, true);
				SetField(L, -2, "PlatformStand");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("enable"): case Command("ena"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");

				GetField(L, -1, "FindFirstChildOfClass");
				PushValue(L, -2);
				PushString(L, "Humanoid");
				Pcall(L, 2, 1, 0);

				PushBoolean(L, false);
				SetField(L, -2, "PlatformStand");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("stopmusic"): case Command("nomusic"): case Command("nosound"): case Command("stopsound"):
	{
		try {
			GetService(L, "Workspace");
			GetField(L, -1, "FindFirstChildOfClass");
			PushValue(L, -2);
			PushString(L, "Sound");
			Pcall(L, 2, 1, 0);

			GetField(L, -1, "Destroy");
			PushValue(L, -2);
			Pcall(L, 1, 0, 0);
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("music"): case Command("sound"):
	{

		std::string Song;
		try {
			Song = "rbxassetid://" + FullCommand.at(0);
		}
		catch (std::invalid_argument e) {
			break;
		}

		try {
			GetService(L, "Workspace");

			GetGlobal(L, "Instance");
			GetField(L, -1, "new");
			PushString(L, "Sound");
			PushValue(L, -4);
			Pcall(L, 2, 1, 0);

			PushString(L, Song.c_str());
			SetField(L, -2, "SoundId");

			PushNumber(L, 1);
			SetField(L, -2, "Volume");

			GetField(L, -1, "Play");
			PushValue(L, -2);
			Pcall(L, 1, 0, 0);
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("ranim"): case Command("ranimation"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Animate");
				PushBoolean(L, true);
				SetField(L, -2, "Disabled");

			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("rhum"): case Command("rhumanoid"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");
				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

			}
			catch (...) {
				EmptyStack(L);

			}
		}
		break;
	}

	case Command("noreset"): case Command("unreset"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");

				PushString(L, "Flame");
				SetField(L, -2, "Name");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("reset"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Flame");

				PushString(L, "Humanoid");
				SetField(L, -2, "Name");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("hum"): case Command("humanoid"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Humanoid");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}


	case Command("anim"): case Command("animate"): {
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Animate");
				PushBoolean(L, false);
				SetField(L, -2, "Disabled");

			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("flamehat"): {

		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Accessory");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushString(L, "FlameHat");
				SetField(L, -2, "Name");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "FlameHat");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 0);
				PushNumber(L, -0.05);
				PushNumber(L, 0.22);
				Pcall(L, 3, 1, 0);

				SetField(L, -3, "AttachmentPos");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "FlameHat");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 0);
				PushNumber(L, 1);
				PushNumber(L, 0);
				Pcall(L, 3, 1, 0);

				SetField(L, -3, "AttachmentUp");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "FlameHat");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 1);
				PushNumber(L, 0);
				PushNumber(L, 0);
				Pcall(L, 3, 1, 0);

				SetField(L, -3, "AttachmentRight");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "FlameHat");

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 0);
				PushNumber(L, 0);
				PushNumber(L, -1);
				Pcall(L, 3, 1, 0);

				SetField(L, -3, "AttachmentForward");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "FlameHat");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Part");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushString(L, "Handle");
				SetField(L, -2, "Name");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "FlameHat");
				GetField(L, -1, "Handle");
				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "SpecialMesh");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				std::string MeshId = "rbxassetid://79396918";
				std::string TextureId = "rbxassetid://734714525";

				PushString(L, TextureId.c_str());
				SetField(L, -2, "TextureId");
				PushString(L, MeshId.c_str());;
				SetField(L, -2, "MeshId");
				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, 0.675);
				PushNumber(L, 0.675);
				PushNumber(L, 0.675);
				Pcall(L, 3, 1, 0);
				SetField(L, -3, "Scale");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Head");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "FlameHat");
				GetField(L, -1, "Handle");
				GetField(L, -1, "MoveTo");
				PushValue(L, -2);
				PushValue(L, -7);
				Pcall(L, 2, 0, 0);

			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("print"):
	{
		std::string Text;

		try {
			Text = Text + FullCommand.at(0) + " ";

			for (std::size_t n = 0; n < (FullCommand.size() - 1); ++n) {
				if (n > 0) {
					Text = Text + " " + FullCommand.at(n + 1);
				}
				else
					Text = Text + FullCommand.at(n + 1);
			}
		}
		catch (...) {
			break;
		}

		EventLog::Log(MESSAGE_OUTPUT, Text.c_str());
		break;
	}

	case Command("info"):
	{
		std::string Text;

		try {
			Text = Text + FullCommand.at(0) + " ";

			for (std::size_t n = 0; n < (FullCommand.size() - 1); ++n) {
				if (n > 0) {
					Text = Text + " " + FullCommand.at(n + 1);
				}
				else
					Text = Text + FullCommand.at(n + 1);
			}
		}
		catch (...) {
			break;
		}

		EventLog::Log(MESSAGE_INFO, Text.c_str());
		break;
	}

	case Command("error"):
	{
		std::string Text;

		try {
			Text = Text + FullCommand.at(0) + " ";

			for (std::size_t n = 0; n < (FullCommand.size() - 1); ++n) {
				if (n > 0) {
					Text = Text + " " + FullCommand.at(n + 1);
				}
				else
					Text = Text + FullCommand.at(n + 1);
			}
		}
		catch (...) {
			break;
		}

		EventLog::Log(MESSAGE_ERROR, Text.c_str());
		break;
	}

	case Command("fling"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			int N1 = rand() % 15000 + 1500;
			int N2 = rand() % 15000 + 1500;

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "BodyForce");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				GetGlobal(L, "Vector3");
				GetField(L, -1, "new");
				PushNumber(L, N1);
				PushNumber(L, 15000);
				PushNumber(L, N2);
				Pcall(L, 3, 1, 0);

				SetField(L, -3, "force");

				Sleep(350);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				GetField(L, -1, "BodyForce");

				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("chat"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			std::string Text;

			try {
				for (std::size_t n = 0; n < (FullCommand.size() - 1); ++n) {
					if (n > 0)
						Text = Text + " " + FullCommand.at(n + 1);
					else
						Text = Text + FullCommand.at(n + 1);
				}
			}
			catch (...) {
				break;
			}

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");

				GetGlobal(L, "game");
				GetField(L, -1, "Chat");
				GetField(L, -1, "Chat");
				PushValue(L, -2);
				PushValue(L, -5);
				PushString(L, Text.c_str());
				PushString(L, "Red");
				Pcall(L, 4, 0, 0);
				Sleep(5);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}
	
	case Command("warn"):
	{
		std::string Text;

		try {
			Text = Text + FullCommand.at(0) + " ";

			for (std::size_t n = 0; n < (FullCommand.size() - 1); ++n) {
				if (n > 0) {
					Text = Text + " " + FullCommand.at(n + 1);
				}
				else
					Text = Text + FullCommand.at(n + 1);
			}
		}
		catch (...) {
			break;
		}


		EventLog::Log(MESSAGE_WARN, Text.c_str());
		break;
	}

	case Command("thaw"): case Command("unfreeze"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				PushBoolean(L, false);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");

				PushBoolean(L, false);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");

				PushBoolean(L, false);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");

				PushBoolean(L, false);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");

				PushBoolean(L, false);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");

				PushBoolean(L, false);
				SetField(L, -2, "Anchored");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("ghost"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				PushNumber(L, 0.5);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");

				PushNumber(L, 0.5);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");

				PushNumber(L, 0.5);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");

				PushNumber(L, 0.5);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");

				PushNumber(L, 0.5);
				SetField(L, -2, "Transparency");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");

				PushNumber(L, 0.5);
				SetField(L, -2, "Transparency");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("emptystack"):
	{
		try {
			EmptyStack(L);
		}
		catch (...) {};
		break;
	}

	case Command("freeze"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");

				PushBoolean(L, true);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");

				PushBoolean(L, true);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");

				PushBoolean(L, true);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");

				PushBoolean(L, true);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");

				PushBoolean(L, true);
				SetField(L, -2, "Anchored");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");

				PushBoolean(L, true);
				SetField(L, -2, "Anchored");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}
	case Command("fencingreach"):
	{
		try {
			GetService(L, "Players");
			GetField(L, -1, "LocalPlayer");
			GetField(L, -1, "Backpack");
			GetField(L, -1, "Foil");
			GetField(L, -1, "Handle");

			GetGlobal(L, "Instance");
			GetField(L, -1, "new");
			PushString(L, "SelectionBox");
			PushValue(L, -4);
			Pcall(L, 2, 1, 0);

			PushValue(L, -3);
			SetField(L, -2, "Adornee");

			GetService(L, "Players");
			GetField(L, -1, "LocalPlayer");
			GetField(L, -1, "Backpack");
			GetField(L, -1, "Foil");
			GetField(L, -1, "Handle");

			GetGlobal(L, "Vector3");
			GetField(L, -1, "new");
			PushNumber(L, 1);
			PushNumber(L, 1);
			PushNumber(L, 30);
			Pcall(L, 3, 1, 0);
			SetField(L, -3, "Size");
			break;
		}
		catch (...) {
			EmptyStack(L);
		}
		break;
	}

	case Command("lock"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "HumanoidRootPart");
				PushBoolean(L, true);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				PushBoolean(L, true);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				PushBoolean(L, true);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");
				PushBoolean(L, true);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");
				PushBoolean(L, true);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");
				PushBoolean(L, true);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");
				PushBoolean(L, true);
				SetField(L, -2, "Locked");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("unlock"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "HumanoidRootPart");
				PushBoolean(L, false);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				PushBoolean(L, false);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Torso");
				PushBoolean(L, false);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Arm");
				PushBoolean(L, false);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Left Leg");
				PushBoolean(L, false);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Arm");
				PushBoolean(L, false);
				SetField(L, -2, "Locked");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Right Leg");
				PushBoolean(L, false);
				SetField(L, -2, "Locked");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("punish"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetGlobal(L, "game");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				PushValue(L, -4);
				SetField(L, -2, "Parent");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("rtools"): case Command("removetools"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Backpack");

				GetField(L, -1, "ClearAllChildren");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("face"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				std::string FaceID;
				try {
					FaceID = "rbxassetid://" + FullCommand.at(1);
				}
				catch (...) {
					FaceID = "rbxassetid://0";
				}
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "face");

				PushString(L, FaceID.c_str());
				SetField(L, -2, "Texture");
				EmptyStack(L);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("pepe"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {

				GetGlobal(L, "game");
				GetField(L, -1, "GetService");
				PushValue(L, -2);
				PushString(L, "Players");
				Pcall(L, 2, 1, 0);

				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "BillboardGui");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushBoolean(L, true);
				SetField(L, -2, "AlwaysOnTop");

				GetGlobal(L, "UDim2");
				GetField(L, -1, "new");
				PushNumber(L, 4);
				PushNumber(L, 0);
				PushNumber(L, 4.5);
				PushNumber(L, 0);
				Pcall(L, 4, 1, 0);
				SetField(L, -3, "Size");

				GetGlobal(L, "game");
				GetField(L, -1, "GetService");
				PushValue(L, -2);
				PushString(L, "Players");
				Pcall(L, 2, 1, 0);

				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "BillboardGui");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "ImageLabel");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushString(L, "rbxassetid://756586183");
				SetField(L, -2, "Image");

				PushNumber(L, 1);
				SetField(L, -2, "Transparency");

				GetGlobal(L, "UDim2");
				GetField(L, -1, "new");
				PushNumber(L, 1);
				PushNumber(L, 0);
				PushNumber(L, 1);
				PushNumber(L, 0);
				Pcall(L, 4, 1, 0);
				SetField(L, -3, "Size");
				
				Sleep(25);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("hipheight"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			try {
				if (FullCommand.size() == 2) {
					GetGlobal(L, "game");
					GetField(L, -1, "GetService");
					PushValue(L, -2);
					PushString(L, "Players");
					Pcall(L, 2, 1, 0);

					GetField(L, -1, Player.c_str());
					GetField(L, -1, "Character");
					GetField(L, -1, "Humanoid");

					PushNumber(L, stod(FullCommand.at(1)));

					SetField(L, -2, "HipHeight");
				}
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("unpunish"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetGlobal(L, "Workspace");

				GetGlobal(L, "game");

				GetField(L, -1, Player.c_str());
				PushValue(L, -3);
				SetField(L, -2, "Parent");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("sit"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");

				PushBoolean(L, true);
				SetField(L, -2, "Sit");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("printidentity"):
	{
		PrintIdentity(L);

		break;
	}
	
	case Command("jump"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Humanoid");

				PushBoolean(L, true);
				SetField(L, -2, "Jump");
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("name"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			std::string Name;

			try {
				Name = FullCommand.at(2);

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());

				PushBoolean(L, false);
				SetField(L, -2, "Archivable");

				PushString(L, Name.c_str());
				SetField(L, -2, "Name");
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("printpos"): case Command("printposition"):
	{
		Settings::Mouse::XYZ_Position = !Settings::Mouse::XYZ_Position;

		break;
	}

	case Command("clicktp"): case Command("clickteleport"):
	{
		Settings::Mouse::Teleport = !Settings::Mouse::Teleport;

		break;
	}

	case Command("clickexplode"):
	{
		Settings::Mouse::Explode = !Settings::Mouse::Explode;

		break;
	}

	case Command("clickdestroy"): case Command("clickremove"):
	{
		Settings::Mouse::Destroy = !Settings::Mouse::Destroy;

		break;
	}

	case Command("esp"):
	{
		players = GetPlayers();

		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			GetService(L, "Players");
			GetField(L, -1, "LocalPlayer");
			GetField(L, -1, "TeamColor");
			GetField(L, -1, "Name");

			const char* LocalPlayer = ToString(L, -1);

			GetService(L, "Players");
			GetField(L, -1, Player.c_str());
			GetField(L, -1, "TeamColor");
			GetField(L, -1, "Name");

			if (ToString(L, -1) != LocalPlayer) {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "BillboardGui");
				GetField(L, -1, "Remove");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "BillboardGui");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushBoolean(L, true);
				SetField(L, -2, "AlwaysOnTop");

				GetGlobal(L, "UDim2");
				GetField(L, -1, "new");
				PushNumber(L, 4);
				PushNumber(L, 0);
				PushNumber(L, 4.5);
				PushNumber(L, 0);
				Pcall(L, 4, 1, 0);
				SetField(L, -3, "Size");

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Character");
				GetField(L, -1, "Head");
				GetField(L, -1, "BillboardGui");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Frame");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushNumber(L, 0);
				SetField(L, -2, "BorderSizePixel");;

				PushNumber(L, 0.5);
				SetField(L, -2, "Transparency");

				GetGlobal(L, "UDim2");
				GetField(L, -1, "new");
				PushNumber(L, 1);
				PushNumber(L, 0);
				PushNumber(L, 1);
				PushNumber(L, 0);
				Pcall(L, 4, 1, 0);
				SetField(L, -3, "Size");


				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "TeamColor");
				GetField(L, -1, "Color");

				GetGlobal(L, "Workspace");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "Head");
				GetField(L, -1, "BillboardGui");
				GetField(L, -1, "Frame");

				PushValue(L, -6);
				SetField(L, -2, "BackgroundColor3");
			}
		}
		break;
	}
	
	case Command("knifemaster"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);

			try {
				GetService(L, "Players");
				GetField(L, -1, "LocalPlayer");
				GetField(L, -1, "TeamColor");
				GetField(L, -1, "Name");

				const char* LocalPlayer = ToString(L, -1);

				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "TeamColor");
				GetField(L, -1, "Name");

				if (ToString(L, -1) != LocalPlayer) {
					RunCommand("goto " + Player);
				}
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	case Command("prefix"):
	{
		try {
			Settings::ChatHook::Prefix = FullCommand.at(0);
		}
		catch (...) {};
		
		break;
	}

	case Command("chathook"):
	{
		Settings::ChatHook::Active != Settings::ChatHook::Active;

		break;
	}

	case Command("getfield"):
	{
		try {
			GetField(L, stof(FullCommand.at(0)), FullCommand.at(1).c_str());
		}
		catch (...) {};
		
		break;
	}

	case Command("getglobal"):
	{
		try {
			GetGlobal(L, FullCommand.at(0).c_str());
		}
		catch (...) {};

		break;
	}

	case Command("hotkeys"):
	{
		Settings::Hotkeys::Active != Settings::Hotkeys::Active;
		break;
	}

	case Command("setfield"):
	{
		try {
			SetField(L, stof(FullCommand.at(0)), FullCommand.at(1).c_str());
		}
		catch (...) {};

		break;
	}

	case Command("pushstring"):
	{
		try {
			PushString(L, FullCommand.at(0).c_str());
		}
		catch (...) {};

		break;
	}

	case Command("getservice"):
	{
		try {
			GetService(L, FullCommand.at(0).c_str());
		}
		catch (...) {};

		break;
	}

	case Command("pushvalue"):
	{
		try {
			PushValue(L, stof(FullCommand.at(0)));
		}
		catch (...) {};

		break;
	}

	case Command("pushnumber"):
	{
		try {
			PushNumber(L, stof(FullCommand.at(0)));
		}
		catch (...) {};

		break;
	}

	case Command("pushboolean"):
	{
		try {
			PushBoolean(L, stoi(FullCommand.at(0)));
		}
		catch (...) {};

		break;
	}

	case Command("pcall"):
	{
		try {
			Pcall(L, stof(FullCommand.at(0)), stof(FullCommand.at(1)), stof(FullCommand.at(2)));
		}
		catch (...) {};

		break;
	}

	case Command("change"): case Command("changestat"): case Command("stat"):
	{
		for (unsigned i = 0; i < players.size(); ++i) {
			std::string Player = *GetName(players[i]);
			std::string StatName;
			std::string StatAmount;

			try {
				StatName = FullCommand.at(1);
				StatAmount = FullCommand.at(2);
			}
			catch (...) {
				break;
			}

			try {
				GetService(L, "Players");
				GetField(L, -1, Player.c_str());
				GetField(L, -1, "leaderstats");
				GetField(L, -1, StatName.c_str());
				int Value = stod(StatAmount.c_str());

				PushString(L, StatAmount.c_str());
				SetField(L, -2, "Value");
				Pcall(L, 1, 0, 0);
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		break;
	}

	default: {	break;	}
}
}

static std::string GetActiveWindowTitle()
{
	char Title[256];
	HWND Window = GetForegroundWindow();
	GetWindowText(Window, Title, sizeof(Title));

	return Title;
}

static void ExecuteChat() {
	do {
		if (GetKeyState(VK_RETURN) < 0 && Settings::ChatHook::LastCommand.substr(0, 1) == Settings::ChatHook::Prefix && Settings::ChatHook::Active) {
			try {
				RunCommand(Settings::ChatHook::LastCommand.substr(1, std::string::npos));

				Settings::ChatHook::LastCommand = "";
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		Sleep(55);
	} while (true);
}

void LoadChat()
{
	do {
		if (ScriptContext != 0 && Settings::ChatHook::Active) {
			try {
				GetService(L, "Players");
				GetField(L, -1, "LocalPlayer");
				GetField(L, -1, "PlayerGui");
				GetField(L, -1, "Chat");

				if (Type(L, -1) == LUA_TNIL)
				{
					MessageBox(NULL, "Custom Chat Detected...", "Flame won't work here.", MB_OK);
					Settings::ChatHook::Active != Settings::ChatHook::Active;
				}
				else {
					GetField(L, -1, "Frame");
					GetField(L, -1, "ChatBarParentFrame");
					GetField(L, -1, "Frame");
					GetField(L, -1, "BoxFrame");
					GetField(L, -1, "Frame");
					GetField(L, -1, "ChatBar");
					GetField(L, -1, "Text");

					std::string Command = ToString(L, -1);

					if (Command.substr(0, 1) == ChatHookPrefix) {
						Settings::ChatHook::LastCommand = Command;
					}
				}
			}
			catch (...) {
				EmptyStack(L);
			}
		}
		Sleep(85);
	} while (true);
}

static void Hotkeys()
{
	do {
		if (GetActiveWindowTitle() == "ROBLOX" && Settings::Hotkeys::Active) {
			if (GetKeyState(VK_F1) < 0)
			{
				RunCommand("god me");
			}
			else if (GetKeyState(VK_F2) < 0)
			{
				RunCommand("pepe me");
			}
			else if (GetKeyState(VK_F3) < 0)
			{
				RunCommand("ws me 100");
				RunCommand("jp me 150");
			}
			else if (GetKeyState(VK_F4) < 0)
			{
				RunCommand("invis me");
			}
			else if (GetKeyState(VK_F5) < 0)
			{
				RunCommand("ff me");
			}
			else if (GetKeyState(VK_F6) < 0)
			{
				RunCommand("flamehat me");
			}
			else if (GetKeyState(VK_F7) < 0)
			{
				RunCommand("esp");
			}
			else if (GetKeyState(VK_F8) < 0)
			{
				RunCommand("knifemaster random");
			}

			Sleep(100);
		}
	} while (true);
}

static void ClickFunctions()
{
	do {
		if (GetKeyState(VK_LBUTTON) < 0)
		{
			if (GetActiveWindowTitle() == "ROBLOX" && Settings::Mouse::Teleport) {
				GetService(L, "Players");
				GetField(L, -1, "LocalPlayer");
				GetField(L, -1, "GetMouse");
				PushValue(L, -2);
				Pcall(L, 1, 1, 0);
				GetField(L, -1, "Hit");
				GetGlobal(L, "Workspace");
				GetField(L, -1, (const char*)PlayerName);
				GetField(L, -1, "HumanoidRootPart");
				PushValue(L, -4);
				SetField(L, -2, "CFrame");
				Sleep(200);
			}
		}

		if (GetKeyState(VK_LBUTTON) < 0)
		{
			if (GetActiveWindowTitle() == "ROBLOX" && Settings::Mouse::XYZ_Position) {
				GetService(L, "Players");
				GetField(L, -1, "LocalPlayer");
				GetField(L, -1, "GetMouse");
				PushValue(L, -2);
				Pcall(L, 1, 1, 0);
				GetField(L, -1, "Hit");
				GetField(L, -1, "p");

				EventLog::Log(MESSAGE_ERROR, "Current XYZ Position:");
				GetGlobal(L, "warn");
				PushValue(L, -2);
				Pcall(L, 1, 0, 0);
			}
		}

		if (GetKeyState(VK_LBUTTON) < 0)
		{
			if (GetActiveWindowTitle() == "ROBLOX" && Settings::Mouse::Explode) {
				GetService(L, "Players");
				GetField(L, -1, "LocalPlayer");
				GetField(L, -1, "GetMouse");
				PushValue(L, -2);
				Pcall(L, 1, 1, 0);
				GetField(L, -1, "Hit");
				GetField(L, -1, "p");

				GetGlobal(L, "Workspace");

				GetGlobal(L, "Instance");
				GetField(L, -1, "new");
				PushString(L, "Explosion");
				PushValue(L, -4);
				Pcall(L, 2, 1, 0);

				PushValue(L, -4);
				SetField(L, -2, "Position");

				PushNumber(L, 10);
				SetField(L, -2, "BlastRadius");

				PushNumber(L, 4000);
				SetField(L, -2, "BlastPressure");
			}
		}

		if (GetKeyState(VK_LBUTTON) < 0)
		{
			if (GetActiveWindowTitle() == "ROBLOX" && Settings::Mouse::Destroy) {
				GetService(L, "Players");
				GetField(L, -1, "LocalPlayer");
				GetField(L, -1, "GetMouse");
				PushValue(L, -2);
				Pcall(L, 1, 1, 0);

				GetField(L, -1, "Target");

				if (Type(L, -1) != LUA_TNIL) {
					GetField(L, -1, "Remove");
					PushValue(L, -2);
					Pcall(L, 1, 0, 0);
				}
				else {
					EmptyStack(L);
				}
			}
			Sleep(185);
		}
	} while (true);
}