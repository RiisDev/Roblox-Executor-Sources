

#include "stdafx.h"
#include "typedefs.h"
#include "aexform.h"

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include "Windows.h"

std::vector<std::string> split(std::string s, ...) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

void ReallyRunCommand(std::string input) {
	std::vector<std::string> args = split(input, ' ');

	int numArgs = args.size();
	int ROBLOX_State = lua_State2;
	if (numArgs < 1)
		return;
	std::string cmd = args.at(0);

	if (cmd == "changestat") {
		if (args.size() == 4) {
			int Character = GetCharacter(args.at(2));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "leaderstats");
			lua_getfield(lua_State2, -1, args.at(2).c_str());
			int TheValue = atof(args.at(3).c_str());
			lua_pushnumber(lua_State2, TheValue);
			lua_setfield(lua_State2, -2, "Value");
		}
	}
	if (cmd == "freeze" || cmd == "Freeze" || cmd == "frz" || cmd == "Frz") {
		if (args.size() == 2) {
			string PlayerName = *GetName(GetPlayer(args.at(1)));

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Head");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Reflectance");
			lua_emptystack(lua_State2);
		}
	}
	if (cmd == "thaw" || cmd == "Thaw" || cmd == "unfreeze" || cmd == "Unfreeze" || cmd == "unfrz" || cmd == "Unfrz") {
		if (args.size() == 2) {
			string PlayerName = *GetName(GetPlayer(args.at(1)));

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Head");
			lua_pushstring(lua_State2, false);
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushstring(lua_State2, false);
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushstring(lua_State2, false);
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushstring(lua_State2, false);
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushstring(lua_State2, false);
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushstring(lua_State2, false);
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");
			lua_emptystack(lua_State2);
		}
	}
	if (cmd == "duck" || cmd == "Duck") {
		if (args.size() == 2) {
			string PlayerName = *GetName(GetPlayer(args.at(1)));

			string duck1 = "rbxassetid://9419831";
			string duck2 = "rbxassetid://9419827";
			string noTshit = "rbxassetid://1";

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Torso");

			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "SpecialMesh");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);

			lua_pushstring(lua_State2, duck1.c_str());
			lua_setfield(lua_State2, -2, "MeshId");

			lua_pushstring(lua_State2, duck2.c_str());
			lua_setfield(lua_State2, -2, "TextureId");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_getfield(lua_State2, -1, "face");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);

			//lua_getglobal(lua_State2, "game");
			//lua_getfield(lua_State2, -1, "Workspace");
			//lua_getfield(lua_State2, -1, PlayerName.c_str());
			//lua_getfield(lua_State2, -1, "Shirt Graphic");
			//lua_pushstring(lua_State2, noTshit.c_str());
			//lua_setfield(lua_State2, -2, "Graphic");
		}
	}
	if (cmd == "ungod") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushnumber(lua_State2, 100);
			lua_setfield(lua_State2, -2, "MaxHealth");
lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "removetools" || cmd == "Removetools" || cmd == "rtools" || cmd == "Rtools") {
		if (args.size() == 2) {
			string PlayerName = *GetName(GetPlayer(args.at(1)));

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Humanoid");

			lua_getfield(lua_State2, -1, "UnequipTools");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 1, 0);

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Backpack");

			lua_getfield(lua_State2, -1, "ClearAllChildren");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_emptystack(lua_State2);
		}
	}
	if (cmd == "god") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushnumber(lua_State2, HUGE_VAL);
			lua_setfield(lua_State2, -2, "MaxHealth");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "kill" && args.size() >= 2) { //commands start here kill bob
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				lua_getglobal(ROBLOX_State, "game");
				lua_getfield(ROBLOX_State, -1, "Players");
				lua_getfield(ROBLOX_State, -1, plr.c_str());
				lua_getfield(ROBLOX_State, -1, "Character");
				lua_getfield(ROBLOX_State, -1, "BreakJoints");
				lua_pushvalue(ROBLOX_State, -2);
				lua_pcall(ROBLOX_State, 1, 0, 0);
			}
		}
		catch (...) {};
	}
	if (cmd == "fire" && args.size() >= 2) { //commands start here kill bob
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				newFireOnCharacter(plr);
			}
		}
		catch (...) {};
	}
	if (cmd == "sparkles" && args.size() >= 2) { //commands start here kill bob
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				newSparklesOnCharacter(plr);
			}
		}
		catch (...) {};
	}
	if (cmd == "music" && args.size() >= 2) { // omfg
		string string;
		try {
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "Sound");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_pushstring(lua_State2, ("rbxassetid://" + args.at(1)).c_str());
			lua_setfield(lua_State2, -2, "SoundId");
			lua_pushstring(lua_State2, "Lobby-Music");
			lua_setfield(lua_State2, -2, "Name");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Volume");
			lua_getfield(lua_State2, -1, "Play");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
		catch (...) {};
	}
	
	if (cmd == "stopmusic" && args.size() >= 2) { // omfg
		string string;
		try {
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_pushstring(lua_State2, "Sound");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
		catch (...) {};
	}
	if (cmd == "skybox") {
		if (args.size() == 2) {
			string skybox = "rbxassetid://" + args.at(1);
			if (cmd == "skybox") {
				if (args.size() == 2) {
					string skybox = "rbxassetid://" + args.at(1);
					lua_getglobal(lua_State2, "game");
					lua_getfield(lua_State2, -1, "Lighting");
					lua_getglobal(lua_State2, "Instance");
					lua_getfield(lua_State2, -1, "new");
					lua_pushstring(lua_State2, "Sky");
					lua_pushvalue(lua_State2, -4);
					lua_pcall(lua_State2, 2, 1, 0);
					lua_pushstring(lua_State2, "skybox");
					lua_setfield(lua_State2, -2, "Name");
					lua_pushstring(lua_State2, skybox.c_str());
					lua_setfield(lua_State2, -2, "SkyboxBk");
					lua_pushstring(lua_State2, skybox.c_str());
					lua_setfield(lua_State2, -2, "SkyboxDn");
					lua_pushstring(lua_State2, skybox.c_str());
					lua_setfield(lua_State2, -2, "SkyboxFt");
					lua_pushstring(lua_State2, skybox.c_str());
					lua_setfield(lua_State2, -2, "SkyboxLf");
					lua_pushstring(lua_State2, skybox.c_str());
					lua_setfield(lua_State2, -2, "SkyboxRt");
					lua_pushstring(lua_State2, skybox.c_str());
					lua_setfield(lua_State2, -2, "SkyboxUp");
					lua_settop(lua_State2, 0);
				}
			}
				}
			}

	if (cmd == "ff" && args.size() >= 2) { //commands start here kill bob
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				newFFOnCharacter(plr);
			}
		}
		catch (...) {};
	}

	if (cmd == "smoke" && args.size() >= 2) { //commands start here kill bob
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				newsmoke(plr);
			}
		}
		catch (...) {};
	}
	if (cmd == "changestat") {
	if (args.size() == 4) {
			int Player = GetPlayer(args.at(1));
			string Target = *GetName(Player);
			string stat = args.at(2);
			float number = atoi(args.at(3).c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, Target.c_str());
			lua_getfield(lua_State2, -1, "leaderstats");
			lua_getfield(lua_State2, -1, stat.c_str());
			lua_pushnumber(lua_State2, number);
			lua_setfield(lua_State2, -2, "Value");
		}
	}
	
	if (cmd == "particles" || cmd == "Particles") {
		if (args.size() == 3) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			string texture = "rbxassetid://" + args.at(2);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "ParticleEmitter");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_pushstring(lua_State2, texture.c_str());
			lua_setfield(lua_State2, -2, "Texture");
			lua_settop(lua_State2, 0);
		}
	}

	if (cmd == "explode") {
		if (args.size() == 2) {
			string fuckingnoob = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "Explosion");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, fuckingnoob.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_getfield(lua_State2, -1, "Position");
			lua_setfield(lua_State2, -6, "Position");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "name") {
		if (args.size() == 5) {

			try {
				int Player = GetPlayer(args.at(1));  //FIX NOWWWWWWWWWWWWWWWWW
				string Target = *GetName(Player);
				string name = args.at(3);
				float number = atoi(args.at(3).c_str());
				lua_getglobal(lua_State2, "game");
				lua_getfield(lua_State2, -1, "Players");
				lua_getfield(lua_State2, -1, Target.c_str());
				lua_getfield(lua_State2, -1, "Name");
				lua_pushvalue(lua_State2, -2);
				lua_pcall(lua_State2, 2, 1, 0);
				lua_getfield(lua_State2, -1, name.c_str());
				lua_pushnumber(lua_State2, number);
				lua_setfield(lua_State2, -2, "Value");


			}
			catch (...) {};
		}
	}
	if (cmd == "ws" && args.size() >= 3) { //commands start here kill bob
		try {
			int Character = GetCharacter(args.at(1));
			std::string charname = GetName(Character)->c_str();
			float hax = atoi(args.at(2).c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushnumber(lua_State2, hax);
			lua_setfield(lua_State2, -2, "WalkSpeed");
			lua_settop(lua_State2, 0);
		}
		catch (...) {};
	}
	if (cmd == "stun" && args.size() >= 3) { //commands start here kill bob
		try {
			int Character = GetCharacter(args.at(1));
			std::string charname = GetName(Character)->c_str();
			float hax = atoi(args.at(2).c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushnumber(lua_State2, hax);
			lua_setfield(lua_State2, -2, "WalkSpeed");
			lua_settop(lua_State2, 0);
		}
		catch (...) {};
	}
	if (cmd == "jp" || cmd == "jumppower" && args.size() >= 3) { //commands start here kill bob
		try {
			int Character = GetCharacter(args.at(1));
			std::string charname = GetName(Character)->c_str();
			float hax = atoi(args.at(2).c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushnumber(lua_State2, hax);
			lua_setfield(lua_State2, -2, "JumpPower");
			lua_settop(lua_State2, 0);
		}
		catch (...) {};
	}
	if (cmd == "blockhead") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_getfield(lua_State2, -1, "Mesh");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "noob" || cmd == "Noob") {
		if (args.size() == 2) {
			string PlayerName = *GetName(GetPlayer(args.at(1)));

			string skid = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=59977935";
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_pushstring(lua_State2, skid.c_str());
			lua_setfield(lua_State2, -2, "CharacterAppearance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "BreakJoints");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_emptystack(lua_State2);
		}
	}
	if (cmd == "no-oder") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "CharacterMesh");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "CharacterMesh");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "CharacterMesh");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);


			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Roblox 2.0 Right Arm");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "CharacterMesh");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);

			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "noarms") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "punish") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "brokenlegs") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "HumanoidRootPart");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "visible") {
		if (args.size() == 2) {
			int bushdidtexas = GetCharacter(args.at(1));
			string bubbles = *GetName(bushdidtexas);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "goto" || cmd == "Goto") {
		if (args.size() == 2) {
			string PlayerName = *GetName(GetPlayer(args.at(1)));

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Torso");
			lua_getfield(lua_State2, -1, "CFrame");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, "LocalPlayer");
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "HumanoidRootPart");
			lua_pushvalue(lua_State2, -6);
			lua_setfield(lua_State2, -2, "CFrame");
		}
	}
	if (cmd == "bring" || cmd == "Bring") {
		if (args.size() == 2) {
			string PlayerName = *GetName(GetPlayer(args.at(1)));

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, "LocalPlayer");
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Torso");
			lua_getfield(lua_State2, -1, "CFrame");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, PlayerName.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "HumanoidRootPart");
			lua_pushvalue(lua_State2, -6);
			lua_setfield(lua_State2, -2, "CFrame");
		}
	}
	if (cmd == "invisible") {
		if (args.size() == 2) {
			int bushdidtexas = GetCharacter(args.at(1));
			string bubbles = *GetName(bushdidtexas);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 1);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 1);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 1);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 1);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 1);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 1);
		}
	}
	if (cmd == "ghost") {
		if (args.size() == 2) {
			int bushdidtexas = GetCharacter(args.at(1));
			string bubbles = *GetName(bushdidtexas);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0.5);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0.5);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0.5);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0.5);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0.5);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, bubbles.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0.5);
		}
	}
	if (cmd == "btools") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, charname.c_str());
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
			lua_getfield(lua_State2, -1, charname.c_str());
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
			lua_getfield(lua_State2, -1, charname.c_str());
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
	}
	if (cmd == "cmds") { // omfg
		string string;
		try {
			ConsoleOutput("\r\n");
			ConsoleOutput("Fire [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Smoke [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Sparkles [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Kill [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Btools [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Blockhead [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("FF [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("God [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("UnGod [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("printidentity");
			ConsoleOutput("\r\n");
			ConsoleOutput("noarms [p]");
			ConsoleOutput("\r\n");			
			ConsoleOutput("credits");
			ConsoleOutput("\r\n");
			ConsoleOutput("Ws [p] [v]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Duck [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("changestat [p] [s] [v] *CRASHES ALOT*");
			ConsoleOutput("\r\n");
			ConsoleOutput("brokenlegs [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Punish [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Skybox [ID]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Explode [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Invisible [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Visible [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Ghost [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Music [ID]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Noob [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Jumppower [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Goto [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Bring [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Fogend [v]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Freeze [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Removetools [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Thaw [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Particles [p] [v]");
			ConsoleOutput("\r\n");
			ConsoleOutput("Noclip [p]");
		}
		catch (...) {};
	}
	if (cmd == "credits") { // omfg
		string string;
		try {
			ConsoleOutput("\r\n");
			ConsoleOutput("Alex (Owner of R1) Helping me a fuck ton.");
			ConsoleOutput("\r\n");
			ConsoleOutput("Binary (dev) legit saving my life");
			ConsoleOutput("\r\n");
			ConsoleOutput("FroggysFriend (Owner of Kronetri) Helping me and inspiration");
			ConsoleOutput("\r\n");
			
		}
		catch (...) {};
	}
	if (cmd == "printidentity" && args.size() >= 1) { // omfg
		string string;
		try {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ChangeContextLevel, 0, 0, 0);
			lua_getglobal(lua_State2, "printidentity");
			lua_pcall(lua_State2, 0, 0, 0);
			lua_settop(lua_State2, 0);
		}
		catch (...) {};
	}
	if (cmd == "fogend" && args.size() >= 2) {
		try {
			float FogEnd = atoi(args.at(1).c_str());

			lua_getglobal(ROBLOX_State, "game");
			lua_getfield(ROBLOX_State, -1, "Lighting");
			lua_pushnumber(ROBLOX_State, FogEnd);
			lua_setfield(ROBLOX_State, -2, "FogEnd");
		}
		catch (...) {};
	}
}

  



void RunCommand(std::string input) {
	//ConsoleOutput(input.c_str());

	std::vector<std::string> args = split(input, ' ');

	int numArgs = args.size();
	if (numArgs < 1)
		return;
	std::string cmd = args.at(0);

	if (args.size() >= 2) {
		if (strstr(args.at(1).c_str(), "all") && strlen(args.at(1).c_str()) == 3) {
			std::vector<int> Players = GetChildren(luaroblox::Players);

			int size = Players.size();
			for (int i = 0; i < size; i++) {
				std::string plr = GetName(Players[i])->c_str();
				std::string finish = cmd + " " + plr;

				//finish off string
				if (args.size() > 2) {
					for (int i = 2; i < args.size(); i++) {
						finish = finish + " " + args.at(i).c_str();
					}
				}
				//done :U

				ReallyRunCommand(finish.c_str());
			}
			return;
		}
		if (strstr(args.at(1).c_str(), "others") && strlen(args.at(1).c_str()) == 6) {
			std::vector<int> Players = GetChildren(luaroblox::Players);

			int size = Players.size();
			for (int i = 0; i < size; i++) {
				//ExecuteCmd(cmd + " " + Players[i]);
				if (Players[i] != GetLocalPlayer(luaroblox::Players)) {
					std::string plr = GetName(Players[i])->c_str();
					std::string finish = cmd + " " + plr;

					//finish off string
					if (args.size() > 2) {
						for (int i = 2; i < args.size(); i++) {
							finish = finish + " " + args.at(i).c_str();
						}
					}
					//done :U

					ReallyRunCommand(finish.c_str());
				}
			}
			return;
		}
	}

	ReallyRunCommand(input);
}

void RunTypedefCommand(std::string input) {
	RunCommand(input);
}

void Intilization() {
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CreateThisWindow, NULL, NULL, NULL);
	Sleep(100);
	ConsoleOutput("Scanning..");
	try {
		luaroblox::Game = Scan(PAGE_READWRITE, (char*)&DataModelVfTable, "xxxx");
		luaroblox::Workspace = FindFirstClass(luaroblox::Game, "Workspace");
		luaroblox::Lighting = FindFirstClass(luaroblox::Game, "Lighting");
		luaroblox::Players = FindFirstClass(luaroblox::Game, "Players");
		luaroblox::Teams = FindFirstClass(luaroblox::Game, "Teams");
		luaroblox::ReplicatedStorage = FindFirstClass(luaroblox::Game, "ReplicatedStorage");
		luaroblox::ChatService = FindFirstClass(luaroblox::Game, "ChatService");
		luaroblox::Terrain = FindFirstClass(luaroblox::Workspace, "Terrain");
		luaroblox::InsertService = FindFirstClass(luaroblox::Workspace, "InsertService");
		luaroblox::CurrentPlayer = GetLocalPlayer(luaroblox::Players);
		luaroblox::PlayerName = GetName(luaroblox::CurrentPlayer);
		EnableMemecheck();
	}
	catch (...) {};

	Sleep(100);
	ConsoleOutput("\r\n");
	ConsoleOutput("Thank you for purchasing Aex!");
	ConsoleOutput("\r\n");
	ConsoleOutput("Loading was Successful. Developed by Austin V2.");
	ConsoleOutput("\r\n");
	ConsoleOutput("Developer / saving my life goes to Binary#3614");
	ConsoleOutput("\r\n");
	ConsoleOutput("Type 'credits' for a list of credits.");
	ConsoleOutput("\r\n");
	ConsoleOutput("Type 'cmds' for a list of commands.");


}


