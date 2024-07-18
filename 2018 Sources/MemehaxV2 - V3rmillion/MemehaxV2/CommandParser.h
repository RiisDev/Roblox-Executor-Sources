#include <windows.h>
#include <string>
#include <vector>
#include "LuaCFunctions.h"
#include "CustomFunctions.h"

#ifndef ass
#define ass

using std::vector;
using std::string;

extern vector<string> Parse(string input);
extern vector<string> ParseByChar(string input, char delimeter);

bool memes = false;

inline void Commands(std::string input) {
	std::vector<std::string> Input = Parse(input);

	if (Input.at(0) == "explore") {
		vector<string> lolcheck = ParseByChar(Input.at(1), '.');
		if (lolcheck.at(0) == "game" && lolcheck.size() == 1) {
			lolagain = "game.GetChildren";
		}
		if (lolcheck.at(0) == "workspace" && lolcheck.size() == 1) {
			lolagain = "workspace.GetChildren";
		}
		else {
			lolagain = Input.at(1) + ".GetChildren";
		}
		printcthreadtemp();
	}

	//DISGUSTING ^^

	//if (Input.at(0) == "setvalue" || Input.at(0) == "setnumber") {
	//	if (Input.at(2) == "=") {
	//		changevalue(Input.at(1), stod(Input.at(3)));
	//	}
	//}

	//if (Input.at(0) == "setbool" || Input.at(0) == "setboolean") {
	//	if (Input.at(2) == "=") {
	//		changebool(Input.at(1), (Input.at(3)));
	//	}
	//}

	//if (Input.at(0) == "setstring" || Input.at(0) == "setstring") {
	//	if (Input.at(2) == "=") {
	//		std::string lolol;
	//		for (int i = 3; i < Input.size(); ++i) {
	//			if ((Input.size() - 1) > i) lolol.append(Input.at(i) + " ");
	//			else if ((Input.size() - 1) == i) lolol.append(Input.at(i));
	//		}
	//		changestring(Input.at(1), lolol);
	//	}
	//}

	if (Input.at(0) == "bring") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "HumanoidRootPart");
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, "LocalPlayer");
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "CFrame");
			LuaC::rbx_setfield(State, -7, "CFrame");
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "to" || Input.at(0) == "goto") {
		if (Input.at(1) != "others" || Input.at(2) != "all") {
			vector<string> plrs;
			plrs = GetPlayerName(Input.at(1));
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, "LocalPlayer");
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "HumanoidRootPart");
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Torso");
				LuaC::rbx_getfield(State, -1, "CFrame");
				LuaC::rbx_setfield(State, -7, "CFrame");
				LuaC::rbx_settop(State, 0);
			}
		}
	}

	if (Input.at(0) == "tp") {
		if (Input.at(2) != "others" || Input.at(2) != "all") {
			vector<string> plrs;
			plrs = GetPlayerName(Input.at(1));
			vector<string> plrs2;
			plrs2 = GetPlayerName(Input.at(2));
			if (Input.size() == 3) {
				for (int i = 0; i < plrs.size(); ++i) {
					for (int i = 0; i < plrs2.size(); ++i) {
						std::string memes = plrs[i];
						std::string memes2 = plrs2[i];
						LuaC::rbx_getservice(State, "Players");
						LuaC::rbx_getfield(State, -1, memes);
						LuaC::rbx_getfield(State, -1, "Character");
						LuaC::rbx_getfield(State, -1, "HumanoidRootPart");
						LuaC::rbx_getservice(State, "Players");
						LuaC::rbx_getfield(State, -1, memes2);
						LuaC::rbx_getfield(State, -1, "Character");
						LuaC::rbx_getfield(State, -1, "Torso");
						LuaC::rbx_getfield(State, -1, "CFrame");
						LuaC::rbx_setfield(State, -7, "CFrame");
						LuaC::rbx_settop(State, 0);
					}
				}
			}
		}
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		if (Input.size() == 5) {
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "HumanoidRootPart");
				LuaC::rbx_getglobal(State, "CFrame");
				LuaC::rbx_getfield(State, -1, "new");
				LuaC::rbx_pushnumber(State, stoi(Input.at(2)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(3)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(4)));
				LuaC::rbx_pcall(State, 3, 1, 0);
				LuaC::rbx_setfield(State, -3, "CFrame");
			}
		}
	}

	/*if (Input.at(0) == "fixlighting") {
		Exe("game.Lighting.Brightness = 1");
		Exe("game.Lighting.GlobalShadows = true");
		Exe("game.Lighting.Outlines = false");
		Exe("game.Lighting.TimeOfDay = 14");
		Exe("game.Lighting.FogEnd = 100000");
	}*/

	/*if (Input.at(0) == "bs" || Input.at(0) == "baseplate") {
		Exe("local b = Instance.new('Part', workspace) b.Name = 'Baseplate' b.Anchored = true b.Size = Vector3.new(2048, 5, 2048)");

		if (Input.size() == 4) {
			LuaC::rbx_getservice(State, "Workspace");
			LuaC::rbx_getfield(State, -1, "Baseplate");
			LuaC::rbx_getglobal(State, "Vector3");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushnumber(State, stoi(Input.at(1)));
			LuaC::rbx_pushnumber(State, stoi(Input.at(2)));
			LuaC::rbx_pushnumber(State, stoi(Input.at(3)));
			LuaC::rbx_pcall(State, 3, 1, 0);
			LuaC::rbx_setfield(State, -3, "Position");
			LuaC::rbx_settop(State, 0);
		}
	}*/

	if (Input.at(0) == "fencingreach" || Input.at(0) == "swordreach" || Input.at(0) == "normalswordreach" || Input.at(0) == "venomshankreach" || Input.at(0) == "darkheartreach") {
		vector<string> plrs;
		std::string swordname;
		plrs = GetPlayerName(Input.at(1));
		if (Input.at(0) == "fencingreach") swordname = "Foil";
		if (Input.at(0) == "normalswordreach") swordname = "Normal Sword";
		if (Input.at(0) == "swordreach") swordname = "Sword";
		if (Input.at(0) == "venomshankreach") swordname = "Venomshank";
		if (Input.at(0) == "darkheartreach") swordname = "Darkheart";
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Backpack");
			LuaC::rbx_getfield(State, -1, swordname);
			LuaC::rbx_getfield(State, -1, "Handle");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "SelectionBox");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Backpack");
			LuaC::rbx_getfield(State, -1, swordname);
			LuaC::rbx_getfield(State, -1, "Handle");
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Backpack");
			LuaC::rbx_getfield(State, -1, swordname);
			LuaC::rbx_getfield(State, -1, "Handle");
			LuaC::rbx_getfield(State, -1, "SelectionBox");
			LuaC::rbx_pushvalue(State, -8);
			LuaC::rbx_setfield(State, -2, "Adornee");
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Backpack");
			LuaC::rbx_getfield(State, -1, swordname);
			LuaC::rbx_getfield(State, -1, "Handle");
			LuaC::rbx_getglobal(State, "Vector3");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_pushnumber(State, 30);
			LuaC::rbx_pcall(State, 3, 1, 0);
			LuaC::rbx_setfield(State, -3, "Size");
			LuaC::rbx_settop(State, 0);

			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Backpack");
			LuaC::rbx_getfield(State, -1, swordname);
			LuaC::rbx_getfield(State, -1, "Handle");
			LuaC::rbx_getfield(State, -1, "SelectionBox");
			LuaC::rbx_getglobal(State, "Color3");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_pushnumber(State, 0);
			LuaC::rbx_pushnumber(State, 0);
			LuaC::rbx_pcall(State, 3, 1, 0);
			LuaC::rbx_setfield(State, -3, "Color3");
			LuaC::rbx_settop(State, 0);

			if (Input.size() == 5) {
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Backpack");
				LuaC::rbx_getfield(State, -1, swordname);
				LuaC::rbx_getfield(State, -1, "Handle");
				LuaC::rbx_getfield(State, -1, "SelectionBox");
				LuaC::rbx_getglobal(State, "Color3");
				LuaC::rbx_getfield(State, -1, "new");
				LuaC::rbx_pushnumber(State, stoi(Input.at(2)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(3)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(4)));
				LuaC::rbx_pcall(State, 3, 1, 0);
				LuaC::rbx_setfield(State, -3, "Color3");
				LuaC::rbx_settop(State, 0);
			}
		}
	}

	if (Input.at(0) == "deletews") {
		LuaC::rbx_getglobal(State, "workspace");
		LuaC::rbx_getfield(State, -1, "ClearAllChildren");
		LuaC::rbx_pushvalue(State, -2);
		LuaC::rbx_pcall(State, 1, 0, 0);
	}

	if (Input.at(0) == "control") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		if (Input.at(1) != "all" || Input.at(1) != "others" || Input.at(1) != "me") {
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, "LocalPlayer");
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Destroy");
				LuaC::rbx_pushvalue(State, -2);
				LuaC::rbx_pcall(State, 1, 0, 0);
				LuaC::rbx_settop(State, 0);
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, "LocalPlayer");
				LuaC::rbx_pushvalue(State, -4);
				LuaC::rbx_setfield(State, -2, "Character");
				LuaC::rbx_settop(State, 0);
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Humanoid");
				LuaC::rbx_getglobal(State, "workspace");
				LuaC::rbx_getfield(State, -1, "CurrentCamera");
				LuaC::rbx_pushvalue(State, -3);
				LuaC::rbx_setfield(State, -2, "CameraSubject");
				LuaC::rbx_settop(State, 0);
				MessageBox(NULL, "This command is buggy. Credit to MightyDanTheMan.", "Memehax Reminder", NULL);
			}
		}
	}

	if (Input.at(0) == "spectate" || Input.at(0) == "view") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		if (Input.at(1) != "all" || Input.at(1) != "others") {
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Humanoid");
				LuaC::rbx_getglobal(State, "workspace");
				LuaC::rbx_getfield(State, -1, "CurrentCamera");
				LuaC::rbx_pushvalue(State, -3);
				LuaC::rbx_setfield(State, -2, "CameraSubject");
				LuaC::rbx_settop(State, 0);
			}
		}
	}

	if (Input.at(0) == "kill") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Humanoid");
			LuaC::rbx_pushnumber(State, NULL);
			LuaC::rbx_setfield(State, -2, "Health");
			LuaC::rbx_settop(State, 0);
		}
	}



	if (Input.at(0) == "explode") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			try {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getglobal(State, "Instance");
				LuaC::rbx_getfield(State, -1, "new");
				LuaC::rbx_pushstring(State, "Explosion");
				LuaC::rbx_pushvalue(State, -4);
				LuaC::rbx_pcall(State, 2, 1, 0);
				LuaC::rbx_pushnumber(State, 999999999);
				LuaC::rbx_setfield(State, -2, "BlastPressure");
				LuaC::rbx_settop(State, 0);
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Torso");
				LuaC::rbx_getfield(State, -1, "Position");
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Explosion");
				LuaC::rbx_pushvalue(State, -6);
				LuaC::rbx_setfield(State, -2, "Position");
				LuaC::rbx_settop(State, 0);
			}
			catch (...) {
				break;
			}
		}
	}

	if (Input.at(0) == "bigfire") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "Fire");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 1, 0);
			LuaC::rbx_pushnumber(State, 100);
			LuaC::rbx_setfield(State, -2, "Size");
			LuaC::rbx_pushstring(State, "BigFire");
			LuaC::rbx_setfield(State, -2, "Name");
			LuaC::rbx_settop(State, 0);
		}

		if (Input.size() == 5) {
			vector<string> plrs;
			plrs = GetPlayerName(Input.at(1));
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Torso");
				LuaC::rbx_getfield(State, -1, "BigFire");
				LuaC::rbx_getglobal(State, "Color3");
				LuaC::rbx_getfield(State, -1, "new");
				LuaC::rbx_pushnumber(State, stoi(Input.at(2)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(3)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(4)));
				LuaC::rbx_pcall(State, 4, 1, 0);
				LuaC::rbx_setfield(State, -3, "Color");
				LuaC::rbx_settop(State, 0);
			}
		}
	}

	if (Input.at(0) == "unbigfire") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "BigFire");
			LuaC::rbx_getfield(State, -1, "Remove");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "fire") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "Fire");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 0, 0);
			LuaC::rbx_settop(State, 0);
		}

		if (Input.size() == 5) {
			vector<string> plrs;
			plrs = GetPlayerName(Input.at(1));
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Torso");
				LuaC::rbx_getfield(State, -1, "Fire");
				LuaC::rbx_getglobal(State, "Color3");
				LuaC::rbx_getfield(State, -1, "new");
				LuaC::rbx_pushnumber(State, stoi(Input.at(2)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(3)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(4)));
				LuaC::rbx_pcall(State, 3, 1, 0);
				LuaC::rbx_setfield(State, -3, "Color");
				LuaC::rbx_settop(State, 0);
			}
		}
	}

	if (Input.at(0) == "unfire") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "Fire");
			LuaC::rbx_getfield(State, -1, "Remove");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "sparkles" || Input.at(0) == "sp") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "Sparkles");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 0, 0);
			LuaC::rbx_settop(State, 0);
		}

		if (Input.size() == 5) {
			vector<string> plrs;
			plrs = GetPlayerName(Input.at(1));
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Torso");
				LuaC::rbx_getfield(State, -1, "Sparkles");
				LuaC::rbx_getglobal(State, "Color3");
				LuaC::rbx_getfield(State, -1, "new");
				LuaC::rbx_pushnumber(State, stoi(Input.at(2)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(3)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(4)));
				LuaC::rbx_pcall(State, 3, 1, 0);
				LuaC::rbx_setfield(State, -3, "SparkleColor");
				LuaC::rbx_settop(State, 0);
			}
		}
	}

	if (Input.at(0) == "unsp" || Input.at(0) == "unsparkles") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "Sparkles");
			LuaC::rbx_getfield(State, -1, "Remove");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "forcefield" || Input.at(0) == "ff") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "ForceField");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "unff" || Input.at(0) == "unforcefield") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "ForceField");
			LuaC::rbx_getfield(State, -1, "Remove");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "smoke") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "Smoke");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 0, 0);
			LuaC::rbx_settop(State, 0);
		}

		if (Input.size() == 5) {
			vector<string> plrs;
			plrs = GetPlayerName(Input.at(1));
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Torso");
				LuaC::rbx_getfield(State, -1, "Smoke");
				LuaC::rbx_getglobal(State, "Color3");
				LuaC::rbx_getfield(State, -1, "new");
				LuaC::rbx_pushnumber(State, stoi(Input.at(2)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(3)));
				LuaC::rbx_pushnumber(State, stoi(Input.at(4)));
				LuaC::rbx_pcall(State, 3, 1, 0);
				LuaC::rbx_setfield(State, -3, "Color");
				LuaC::rbx_settop(State, 0);
			}
		}
	}

	if (Input.at(0) == "unsmoke") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "Smoke");
			LuaC::rbx_getfield(State, -1, "Remove");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "redmist") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "Smoke");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 1, 0);
			LuaC::rbx_pushstring(State, "RedMist");
			LuaC::rbx_setfield(State, -2, "Name");
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "RedMist");
			LuaC::rbx_getglobal(State, "Color3");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushnumber(State, 10);
			LuaC::rbx_pushnumber(State, 0);
			LuaC::rbx_pushnumber(State, 0);
			LuaC::rbx_pcall(State, 3, 1, 0);
			LuaC::rbx_setfield(State, -3, "Color");
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "unredmist") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "RedMist");
			LuaC::rbx_getfield(State, -1, "Remove");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "particles" || Input.at(0) == "pt") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		if (Input.size() == 2) {
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Torso");
				LuaC::rbx_getglobal(State, "Instance");
				LuaC::rbx_getfield(State, -1, "new");
				LuaC::rbx_pushstring(State, "ParticleEmitter");
				LuaC::rbx_pushvalue(State, -4);
				LuaC::rbx_pcall(State, 2, 0, 0);
				LuaC::rbx_settop(State, 0);
			}
		}

		if (Input.size() == 3) {
			vector<string> plrs;
			plrs = GetPlayerName(Input.at(1));
			for (int i = 0; i < plrs.size(); ++i) {
				std::string memes = plrs[i];
				LuaC::rbx_getservice(State, "Players");
				LuaC::rbx_getfield(State, -1, memes);
				LuaC::rbx_getfield(State, -1, "Character");
				LuaC::rbx_getfield(State, -1, "Torso");
				LuaC::rbx_getglobal(State, "Instance");
				LuaC::rbx_getfield(State, -1, "new");
				LuaC::rbx_pushstring(State, "ParticleEmitter");
				LuaC::rbx_pushvalue(State, -4);
				LuaC::rbx_pcall(State, 2, 1, 0);
				LuaC::rbx_pushstring(State, "LuaC::rbxassetid://" + Input.at(2));
				LuaC::rbx_setfield(State, -2, "Texture");
				LuaC::rbx_pushnumber(State, 100);
				LuaC::rbx_setfield(State, -2, "VelocitySpread");
				LuaC::rbx_pushnumber(State, 130);
				LuaC::rbx_setfield(State, -2, "Rate");
				LuaC::rbx_settop(State, 0);
			}
		}
	}

	if (Input.at(0) == "ws" || Input.at(0) == "walkspeed" || Input.at(0) == "speed") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		int speed = stoi(Input.at(2));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Humanoid");
			LuaC::rbx_pushnumber(State, speed);
			LuaC::rbx_setfield(State, -2, "WalkSpeed");
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "jp" || Input.at(0) == "jumppower") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		int power = stoi(Input.at(2));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Humanoid");
			LuaC::rbx_pushnumber(State, power);
			LuaC::rbx_setfield(State, -2, "JumpPower");
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "rise" || Input.at(0) == "float") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, "LocalPlayer");
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "BodyVelocity");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 1, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "unrise" || Input.at(0) == "unfloat" || Input.at(0) == "fall") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, "LocalPlayer");
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getfield(State, -1, "BodyVelocity");
			LuaC::rbx_getfield(State, -1, "Destroy");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "hh" || Input.at(0) == "hipheight" || Input.at(0) == "height") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		int height = stoi(Input.at(2));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Humanoid");
			LuaC::rbx_pushnumber(State, height);
			LuaC::rbx_setfield(State, -2, "HipHeight");
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "unhh" || Input.at(0) == "unhipheight" || Input.at(0) == "unheight") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Humanoid");
			LuaC::rbx_pushnumber(State, 0);
			LuaC::rbx_setfield(State, -2, "HipHeight");
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "unpt" || Input.at(0) == "unparticles") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "ParticleEmitter");
			LuaC::rbx_getfield(State, -1, "Remove");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "moonman") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getfield(State, -1, "Mesh");
			LuaC::rbx_getfield(State, -1, "Destroy");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getfield(State, -1, "face");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_setfield(State, -2, "Transparency");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "SpecialMesh");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getfield(State, -1, "Mesh");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://23265118");
			LuaC::rbx_setfield(State, -2, "MeshId");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getfield(State, -1, "Mesh");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://450502375");
			LuaC::rbx_setfield(State, -2, "TextureId");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "illuminati") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Humanoid");
			LuaC::rbx_pushnumber(State, 3.5);
			LuaC::rbx_setfield(State, -2, "HipHeight");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Right Arm");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_setfield(State, -2, "Transparency");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Left Arm");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_setfield(State, -2, "Transparency");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Right Leg");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_setfield(State, -2, "Transparency");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Left Leg");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_setfield(State, -2, "Transparency");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_setfield(State, -2, "Transparency");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getfield(State, -1, "face");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_setfield(State, -2, "Transparency");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "ParticleEmitter");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 1, 0);
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://362575925");
			LuaC::rbx_setfield(State, -2, "Texture");
			LuaC::rbx_pushnumber(State, 100);
			LuaC::rbx_setfield(State, -2, "VelocitySpread");
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "SpecialMesh");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "Mesh");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://438530093");
			LuaC::rbx_setfield(State, -2, "MeshId");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Torso");
			LuaC::rbx_getfield(State, -1, "Mesh");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://438530120");
			LuaC::rbx_setfield(State, -2, "TextureId");
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getglobal(State, "game");
			LuaC::rbx_getfield(State, -1, "Lighting");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "Sky");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 1, 0);
			LuaC::rbx_pushstring(State, "wow");
			LuaC::rbx_setfield(State, -2, "Name");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://259226828");
			LuaC::rbx_setfield(State, -2, "SkyboxBk");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://259226828");
			LuaC::rbx_setfield(State, -2, "SkyboxDn");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://259226828");
			LuaC::rbx_setfield(State, -2, "SkyboxFt");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://259226828");
			LuaC::rbx_setfield(State, -2, "SkyboxLf");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://259226828");
			LuaC::rbx_setfield(State, -2, "SkyboxRt");
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://259226828");
			LuaC::rbx_setfield(State, -2, "SkyboxUp");
			LuaC::rbx_settop(State, 0);
		}
		if (memes == false) {
			LuaC::rbx_getglobal(State, "workspace");
			LuaC::rbx_getglobal(State, "Instance");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushstring(State, "Sound");
			LuaC::rbx_pushvalue(State, -4);
			LuaC::rbx_pcall(State, 2, 1, 0);
			LuaC::rbx_pushstring(State, "LuaC::rbxassetid://512338922");
			LuaC::rbx_setfield(State, -2, "SoundId");
			LuaC::rbx_pushstring(State, "meme");
			LuaC::rbx_setfield(State, -2, "Name");
			LuaC::rbx_pushnumber(State, 1);
			LuaC::rbx_setfield(State, -2, "Volume");
			LuaC::rbx_getfield(State, -1, "Play");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
			memes = true;
		}
	}

	if (Input.at(0) == "cubehead") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getglobal(State, "Vector3");
			LuaC::rbx_getfield(State, -1, "new");
			LuaC::rbx_pushnumber(State, 6);
			LuaC::rbx_pushnumber(State, 6);
			LuaC::rbx_pushnumber(State, 6);
			LuaC::rbx_pcall(State, 3, 1, 0);
			LuaC::rbx_setfield(State, -3, "Size");
			LuaC::rbx_settop(State, 0);
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getfield(State, -1, "Mesh");
			LuaC::rbx_getfield(State, -1, "Destroy");
			LuaC::rbx_pushvalue(State, -2);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}

	if (Input.at(0) == "blockhead") {
		vector<string> plrs;
		plrs = GetPlayerName(Input.at(1));
		for (int i = 0; i < plrs.size(); ++i) {
			std::string memes = plrs[i];
			LuaC::rbx_getservice(State, "Players");
			LuaC::rbx_getfield(State, -1, memes);
			LuaC::rbx_getfield(State, -1, "Character");
			LuaC::rbx_getfield(State, -1, "Head");
			LuaC::rbx_getfield(State, -1, "Mesh");
			LuaC::rbx_getfield(State, -1, "Destroy");
			LuaC::rbx_pushvalue(State, -1);
			LuaC::rbx_pcall(State, 1, 0, 0);
			LuaC::rbx_settop(State, 0);
		}
	}
}

//some commands are patched
//anything that doesn't use setfield is guaranteed to work

#endif