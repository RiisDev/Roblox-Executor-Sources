// Hungry Bird.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "typedefs.h"
#include "hungrybirdform.h"
#include "FECheck.h"
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
	if (cmd == "burn" && args.size() >= 2) { //commands start here kill bob
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				newFireOnCharacter(plr);
				Sleep(3000);
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
	if (cmd == "charapp") {
		if (args.size() == 3) {
			int noobhere = GetPlayer(args.at(1));
			string noobnamehere = *GetName(noobhere);
			string goodurl = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=";
			goodurl.append(args.at(2));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, noobnamehere.c_str());
			lua_pushstring(lua_State2, goodurl.c_str());
			lua_setfield(lua_State2, -2, "CharacterAppearance");
			lua_settop(lua_State2, 0);
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Health");
			lua_settop(lua_State2, 0); //s2 im john ceese!
		}

	}

	if (cmd == "ws" && args.size() >= 3) { //commands start here kill bob
		try {
			int Character = GetCharacter(args.at(1));
			std::string charname = GetName(Character)->c_str();
			float hax = atoi(args.at(2).c_str()); // walkspeed to change
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
	if (cmd == "jp") {
		if (args.size() == 3) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			float hax = atoi(args.at(2).c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushnumber(lua_State2, hax);
			lua_setfield(lua_State2, -2, "JumpPower");
			lua_settop(lua_State2, 0);
		}
	}

	if (cmd == "particles") {
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

	if (cmd == "trumpify") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			string texture = "rbxassetid://526577438";
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
	if (cmd == "airfart") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			string texture = "rbxassetid://10893695";
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "ParticleEmitter");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_pushstring(lua_State2, texture.c_str());
			lua_setfield(lua_State2, -2, "Texture");
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
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
	if (cmd == "bighead") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string Target = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1,"Players");
				lua_getfield(lua_State2, -1, Target.c_str());
					lua_getfield(lua_State2, -1, "Character");
						lua_getfield(lua_State2, -1, "Head");
							lua_getfield(lua_State2, -1, "Mesh");
								lua_getglobal(lua_State2, "Vector3");
								lua_getfield(lua_State2, -1, "new");
								lua_pushnumber(lua_State2, double(3));
								lua_pushnumber(lua_State2, double(3));
								lua_pushnumber(lua_State2, double(3));
								lua_pcall(lua_State2, 3, 1, 0);

								lua_setfield(lua_State2, -3, "Scale");
								//rbxlua_pcall(CluaState, 0, 0, 0); 
		}
	}
	if (cmd == "changestat") {
		if (args.size() == 4) {
			int Player = GetPlayer(args.at(1));
			string Target = *GetName(Player);
			string stat = args.at(2);
			float number = atoi(args.at(4).c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1,"Players");
			lua_getfield(lua_State2, -1, Target.c_str());
			lua_getfield(lua_State2, -1, "leaderstats");
			lua_pushnumber(lua_State2, number);
			lua_setfield(lua_State2, -2, stat.c_str());
		}
	}
	if (cmd == "mlg") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			string texture = "rbxassetid://65978963";
			string texture2 = "rbxassetid://433960422";
			string texture3 = "rbxassetid://195696364";
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "ParticleEmitter");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_pushstring(lua_State2, texture.c_str());
			lua_setfield(lua_State2, -2, "Texture");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "ParticleEmitter");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_pushstring(lua_State2, texture2.c_str());
			lua_setfield(lua_State2, -2, "Texture");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "ParticleEmitter");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_pushstring(lua_State2, texture3.c_str());
			lua_setfield(lua_State2, -2, "Texture");
			lua_settop(lua_State2, 0);
		}
	}

	if (cmd == "hillary") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			string texture = "rbxassetid://574840995";
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
	if (cmd == "unhillary") {
		if (args.size() == 3) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_getfield(lua_State2, -1, "ParticleEmitter");
			lua_getfield(lua_State2, -1, "remove");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "rh") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_getfield(lua_State2, -1, "Destroy");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "freecam") {
		if (args.size() == 2) {
			string NameEntered = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Head");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");


			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "HumanoidRootPart");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Reflectance");
			lua_emptystack(lua_State2);
		}
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
	if (cmd == "untrump") {
		if (args.size() == 3) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_getfield(lua_State2, -1, "ParticleEmitter");
			lua_getfield(lua_State2, -1, "remove");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "unparticles") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_getfield(lua_State2, -1, "ParticleEmitter");
			lua_getfield(lua_State2, -1, "remove");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "killcamera") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, "Camera");
			lua_getfield(lua_State2, -1, "remove");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "sparkles") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "Sparkles");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "unsparkles") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_getfield(lua_State2, -1, "Sparkles");
			lua_getfield(lua_State2, -1, "remove");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
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
	if (cmd == "hugefire" && args.size() >= 2) { // omfg
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				newBombFireOnCharacter(plr);
			}
		}
		catch (...) {};
	}
	if (cmd == "megasmoke" && args.size() >= 2) { // omfg
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				MegaSmokeBig(plr);
			}
		}
		catch (...) {};
	}
	if (cmd == "btools" && args.size() >= 2) { // omfg
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				Btools(plr);
			}
		}
		catch (...) {};
	}
	if (cmd == "destorytool" && args.size() >= 2) { // omfg
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				destroytool(plr);
			}
		}
		catch (...) {};
	}
	if (cmd == "copytool" && args.size() >= 2) { // omfg
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				copytool(plr);
			}
		}
		catch (...) {};
	}
	if (cmd == "movetool" && args.size() >= 2) { // omfg
		try {
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			if (Player) {
				std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
				movetool(plr);
			}
		}
		catch (...) {};
	}
	if (cmd == "print" && args.size() >= 2) { // omfg
		string string;
		try {
			for (int i = 1; i < args.size(); i++) {
				if (i < (args.size() - 1)) {
					string.append(args.at(i) + ' ');
				}
				if (i == (args.size() - 1)) {
					string.append(args.at(i));
				}
			}
			lua_getglobal(lua_State2, "print");
			lua_pushstring(lua_State2, string.c_str());
			lua_pcall(lua_State2, 1, 0, 0);
			lua_emptystack(lua_State2);
		}
		catch (...) {};
	}


	if (cmd == "warn" && args.size() >= 2) { // omfg
		string string;
		try {
			for (int i = 1; i < args.size(); i++) {
				if (i < (args.size() - 1)) {
					string.append(args.at(i) + ' ');
				}
				if (i == (args.size() - 1)) {
					string.append(args.at(i));
				}
			}
			lua_getglobal(lua_State2, "warn");
			lua_pushstring(lua_State2, string.c_str());
			lua_pcall(lua_State2, 1, 0, 0);
			lua_emptystack(lua_State2);
		}
		catch (...) {};
	}
	if (cmd == "printidentity" && args.size() >= 0) { // omfg
		string string;
		try {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ChangeContextLevel, 0, 0, 0);
			lua_getglobal(lua_State2, "printidentity");
			lua_pcall(lua_State2, 0, 0, 0);
			lua_settop(lua_State2, 0);
		}
		catch (...) {};
	}

	if (cmd == "fakememcheck" && args.size() >= 0) { // omfg
		string string;
		try {
			printToLocalConsole("RC7 Cracked Memcheck Loaded!");
		}
		catch (...) {};
	}
	if (cmd == "lvl7" && args.size() >= 0) { // omfg
		string string;
		try {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)lvl7, 0, 0, 0);
		}
		catch (...) {};
	}
	if (cmd == "lvl4" && args.size() >= 0) { // omfg
		string string;
		try {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)lvl4, 0, 0, 0);
		}
		catch (...) {};
	}
	if (cmd == "lvl2" && args.size() >= 0) { // omfg
		string string;
		try {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)lvl2, 0, 0, 0);
		}
		catch (...) {};
	}
	if (cmd == "lvl10" && args.size() >= 2) { // omfg
		string string;
		try {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)lvl10, 0, 0, 0);
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
	if (cmd == "cupcakevagina" && args.size() >= 1) { // omfg
		try {
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "Sound");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_pushstring(lua_State2, "rbxassetid://690441689");
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
	if (cmd == "unclefucker" && args.size() >= 1) { // omfg
		try {
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "Sound");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_pushstring(lua_State2, "rbxassetid://686656986");
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
	if (cmd == "rekt" && args.size() >= 2) { // omfg
		string string;
		try {
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "Sound");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 1, 0);
			lua_pushstring(lua_State2, "rbxassetid://221842357");
			lua_setfield(lua_State2, -2, "SoundId");
			lua_pushstring(lua_State2, "Lobby-Music");
			lua_setfield(lua_State2, -2, "Name");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Volume");
			lua_getfield(lua_State2, -1, "Play");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
			Sleep(6000);
			std::string PlayerName = args.at(1).c_str();
			int Player = GetPlayer(PlayerName);
			std::string plr = GetName(Player)->c_str(); //game.Players.Bobby1234.Character:BreakJoints()
			lua_getglobal(ROBLOX_State, "game");
			lua_getfield(ROBLOX_State, -1, "Players");
			lua_getfield(ROBLOX_State, -1, plr.c_str());
			lua_getfield(ROBLOX_State, -1, "Character");
			lua_getfield(ROBLOX_State, -1, "BreakJoints");
			lua_pushvalue(ROBLOX_State, -2);
			lua_pcall(ROBLOX_State, 1, 0, 0);
		}
		catch (...) {};
	}
	if (cmd == "cmds") { // omfg
		string string;
		try {
			ConsoleOutput("-|Basic Commands|-");
			ConsoleOutput("\r\n");
			ConsoleOutput("fakememcheck (says RC7 Cracked Memcheck Loaded! but isnt really)");
			ConsoleOutput("\r\n");
			ConsoleOutput("printidentity (prints the current identity) (LUA)");
			ConsoleOutput("\r\n");
			ConsoleOutput("warn [s] (LUA)");
			ConsoleOutput("\r\n");
			ConsoleOutput("credits (please use if showcasing)");
			ConsoleOutput("\r\n");
			ConsoleOutput("cmdcount (sees how many cmds there are)");
			ConsoleOutput("\r\n");
			ConsoleOutput("\r\n");
			ConsoleOutput("-|Build Tools|-");
			ConsoleOutput("\r\n");
			ConsoleOutput("btools [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("destroytool [p] (from building tools)");
			ConsoleOutput("\r\n");
			ConsoleOutput("copytool [p] (from building tools)");
			ConsoleOutput("\r\n");
			ConsoleOutput("movetool [p] (from building tools)");
			ConsoleOutput("\r\n");
			ConsoleOutput("\r\n");
			ConsoleOutput("-|Player Commands|-");
			ConsoleOutput("\r\n");
			ConsoleOutput("Kill [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("god [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("ungod [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("ff [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("megasmoke [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("hugefire [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("explode [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("punish [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("unpunish [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("invisible [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("visible [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("burn [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("unfire [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("particles [p] [n]");
			ConsoleOutput("\r\n");
			ConsoleOutput("unparticles [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("sparkles [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("unsparkles [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("ws [p] [n]");
			ConsoleOutput("\r\n");
			ConsoleOutput("jp [p] [n]");
			ConsoleOutput("\r\n");
			ConsoleOutput("freecam [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("rh [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("noarms [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("blockhead [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("killcamera [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("rekt [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("freeze [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("thaw [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("crash [p] (coming soon)");
			ConsoleOutput("\r\n");
			ConsoleOutput("ghost [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("unghost [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("fov [p] [n]");
			ConsoleOutput("\r\n");
			ConsoleOutput("jump [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("sit [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("2dwalk [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("un2dwalk [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("hidename [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("showname [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("changestat [p] [n]");
			ConsoleOutput("\r\n");
			ConsoleOutput("\r\n");
			ConsoleOutput("-|Teleport Commands|-");
			ConsoleOutput("\r\n");
			ConsoleOutput("tp [p1] [p2]");
			ConsoleOutput("\r\n");
			ConsoleOutput("bring [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("\r\n");
			ConsoleOutput("-|World Commands|-");
			ConsoleOutput("\r\n");
			ConsoleOutput("fogend [n]");
			ConsoleOutput("\r\n");
			ConsoleOutput("gravity [n]");
			ConsoleOutput("\r\n");
			ConsoleOutput("skybox [n]");
			ConsoleOutput("\r\n");
			ConsoleOutput("music [n]");
			ConsoleOutput("\r\n");
			ConsoleOutput("\r\n");
			ConsoleOutput("-|Random Commands|-");
			ConsoleOutput("\r\n");
			ConsoleOutput("trumpify [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("untrump [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("hillary [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("unhillary [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("airfart [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("newpart");
			ConsoleOutput("\r\n");
			ConsoleOutput("mlg [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("cupcakevagina");
			ConsoleOutput("\r\n");
			ConsoleOutput("unclefucker");
			ConsoleOutput("\r\n");
			ConsoleOutput("duck [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("harambezilla [p]");
			ConsoleOutput("\r\n");
			ConsoleOutput("no-oder [p] (only works for 2.0 packages)");
			ConsoleOutput("\r\n");
		}
		catch (...) {};
	}
	if (cmd == "credits") { //commands start here health player 0
		ConsoleOutput("\r\n");
		ConsoleOutput("max5pop: for the idea of Kronetri, founder/owner/creator");
		ConsoleOutput("\r\n");
		ConsoleOutput("FroggysFriend: Head Developer");
		ConsoleOutput("\r\n");
		ConsoleOutput("Octum: Head Supervisor and Head Developer (UI Design)");
		ConsoleOutput("\r\n");
		ConsoleOutput("Anarchy: Owner");
		ConsoleOutput("\r\n");
		ConsoleOutput("RexAttack: Developer");
		ConsoleOutput("\r\n");
		ConsoleOutput("Minccino: Being cool, also provided some bypassed audios. Thanks!");
		ConsoleOutput("\r\n");

	}
	if (cmd == "speech") {
		if (args.size() == 2) {

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
			string string;
			for (int i = 1; i < args.size(); i++) {
				if (i < (args.size() - 1)) {
					string.append(args.at(i) + ' ');
				}
				if (i == (args.size() - 1)) {
					string.append(args.at(i));
				}

				lua_pushstring(lua_State2, (string.c_str()));
				lua_setfield(lua_State2, -2, "InitialPrompt");
				lua_pushvalue(lua_State2, -2);
				lua_pcall(lua_State2, 1, 0, 0);
			}
		}
	}
	if (cmd == "cmdcount") { //commands start here health player 0
		ConsoleOutput("\r\n");
		ConsoleOutput("there is currently 73 commands!");
		ConsoleOutput("\r\n");

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
	if (cmd == "unfire") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_getfield(lua_State2, -1, "Fire");
			lua_getfield(lua_State2, -1, "remove");
			lua_pushvalue(lua_State2, -2);
			lua_pcall(lua_State2, 1, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "punish") {
		if (args.size() == 2) {
			string noob1 = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, noob1.c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Lighting");
			lua_setfield(lua_State2, -3, "Parent");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "unpunish") {
		if (args.size() == 2) {
			string noob1 = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Lighting");
			lua_getfield(lua_State2, -1, noob1.c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_setfield(lua_State2, -3, "Parent");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "freeze") {
		if (args.size() == 2) {
			string NameEntered = *GetName(GetPlayer(args.at(1)));

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Head");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
		
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
	

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
		

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");


			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushstring(lua_State2, "true");
			lua_setfield(lua_State2, -2, "Anchored");
			lua_emptystack(lua_State2);
		}
	}
	if (cmd == "thaw") {
		if (args.size() == 2) {
			string NameEntered = *GetName(GetPlayer(args.at(1)));

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Head");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");

			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");


			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");


			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");


			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");


			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");
;


			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Players");
			lua_getfield(lua_State2, -1, NameEntered.c_str());
			lua_getfield(lua_State2, -1, "Character");
			lua_getfield(lua_State2, -1, "HumanoidRootPart");
			lua_pushstring(lua_State2, "false");
			lua_setfield(lua_State2, -2, "Anchored");

			lua_emptystack(lua_State2);
		}
	}
	if (cmd == "ff") {
		if (args.size() == 2) {
			int Character = GetCharacter(args.at(1));
			string charname = *GetName(Character);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, charname.c_str());
			lua_getglobal(lua_State2, "Instance");
			lua_getfield(lua_State2, -1, "new");
			lua_pushstring(lua_State2, "ForceField");
			lua_pushvalue(lua_State2, -4);
			lua_pcall(lua_State2, 2, 0, 0);
			lua_settop(lua_State2, 0);
		}
	}
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
	if (cmd == "normalgravity") {
		if (args.size() == 0) {
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_pushnumber(lua_State2, 196.200);
			lua_setfield(lua_State2, -2, "Gravity");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "gravity") {
		if (args.size() == 2) {
			int newgravity = atoi(args.at(1).c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_pushnumber(lua_State2, newgravity);
			lua_setfield(lua_State2, -2, "Gravity");
			lua_settop(lua_State2, 0);
		}
	}

	if (cmd == "tp") {
		if (args.size() == 3) {
			string noob1 = *GetName(GetPlayer(args.at(1)));
			string noob2 = *GetName(GetPlayer(args.at(2)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, noob1.c_str());
			lua_getfield(lua_State2, -1, "HumanoidRootPart");
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, noob2.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_getfield(lua_State2, -1, "CFrame");
			lua_setfield(lua_State2, -6, "CFrame");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "fov") {
		if (args.size() == 3) {
			float fov = atoi(args.at(2).c_str());
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1,"Workspace");
			lua_getfield(lua_State2, -1, "Camera");
			lua_pushnumber(lua_State2, fov);
			lua_setfield(lua_State2, -2, "FieldOfView");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "jump") {
		if (args.size() == 2) {
			string noob1 = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, noob1.c_str());
			lua_getfield(lua_State2, -1,"Humanoid");
			lua_pushboolean(lua_State2, true);
			lua_setfield(lua_State2, -2, "Jump");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "hidename") {
		if (args.size() == 2) {
			string noob1 = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, noob1.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushstring(lua_State2, "None");
			lua_setfield(lua_State2, -2, "DisplayDistanceType");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "showname") {
		if (args.size() == 2) {
			string noob1 = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, noob1.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushstring(lua_State2, "Subject");
			lua_setfield(lua_State2, -2, "DisplayDistanceType");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "2dwalk") {
		if (args.size() == 2) {
			string noob1 = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, noob1.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushboolean(lua_State2, false);
			lua_setfield(lua_State2, -2, "AutoRotate");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "un2dwalk") {
		if (args.size() == 2) {
			string noob1 = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, noob1.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushboolean(lua_State2, true);
			lua_setfield(lua_State2, -2, "AutoRotate");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "sit") {
		if (args.size() == 2) {
			string noob1 = *GetName(GetPlayer(args.at(1)));
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, noob1.c_str());
			lua_getfield(lua_State2, -1, "Humanoid");
			lua_pushboolean(lua_State2, true);
			lua_setfield(lua_State2, -2, "Sit");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "duck") {
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

			//lua_pushstring(lua_State2, Scale.c_str());

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

			//getglobal(State, "game");
			//getfield(State, -1, "Workspace");
			//getfield(State, -1, PlayerName.c_str());
			//getfield(State, -1, "Shirt Graphic");
			//pushstring(State, noTshit.c_str());
			//setfield(State, -2, "Graphic");
		}
	}
	if (cmd == "harambezilla") {
		if (args.size() == 2) {
			string PlayerName = *GetName(GetPlayer(args.at(1)));

			string duck1 = "rbxassetid://430330296";
			string duck2 = "rbxassetid://430330316";
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

			//lua_pushstring(lua_State2, Scale.c_str());

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

			//getglobal(State, "game");
			//getfield(State, -1, "Workspace");
			//getfield(State, -1, PlayerName.c_str());
			//getfield(State, -1, "Shirt Graphic");
			//pushstring(State, noTshit.c_str());
			//setfield(State, -2, "Graphic");
		}
	}
	if (cmd == "invisible") {
		if (args.size() == 2) {
			int tepig_is_a_fucking_noob = GetCharacter(args.at(1));
			string tepigs_name = *GetName(tepig_is_a_fucking_noob);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_getfield(lua_State2, -1, "face");
			lua_pushnumber(lua_State2, 1);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "ghost") {
		if (args.size() == 2) {
			int tepig_is_a_fucking_noob = GetCharacter(args.at(1));
			string tepigs_name = *GetName(tepig_is_a_fucking_noob);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_getfield(lua_State2, -1, "face");
			lua_pushnumber(lua_State2, 0.5);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "unghost") {
		if (args.size() == 2) {
			int tepig_is_a_fucking_noob = GetCharacter(args.at(1));
			string tepigs_name = *GetName(tepig_is_a_fucking_noob);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_getfield(lua_State2, -1, "face");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "visible") {
		if (args.size() == 2) {
			int tepig_is_a_fucking_noob = GetCharacter(args.at(1));
			string tepigs_name = *GetName(tepig_is_a_fucking_noob);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Torso");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Left Arm");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Left Leg");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Right Arm");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Right Leg");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
			lua_getglobal(lua_State2, "game");
			lua_getfield(lua_State2, -1, "Workspace");
			lua_getfield(lua_State2, -1, tepigs_name.c_str());
			lua_getfield(lua_State2, -1, "Head");
			lua_getfield(lua_State2, -1, "face");
			lua_pushnumber(lua_State2, 0);
			lua_setfield(lua_State2, -2, "Transparency");
			lua_settop(lua_State2, 0);
		}
	}
	if (cmd == "smoke" && args.size() >= 3) { //commands start here health player 0
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
	ConsoleOutput("Loading Assets...");
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
	ConsoleOutput("Welcome to Kronetri Recoded");
	ConsoleOutput("\r\n");
	ConsoleOutput("Loading Assets Successful.");
	ConsoleOutput("\r\n");
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)lvl2, 0, 0, 0);
	bool FilteringEnabled = FE(luaroblox::Workspace);
	if (FilteringEnabled) {
		ConsoleOutput("\r\n");
		ConsoleOutput("Filtering is enabled ):\r\n");
		ConsoleOutput("\r\n");
		printToLocalConsole("Filtering is enabled ):");
	}
	else {
		ConsoleOutput("\r\n");
		ConsoleOutput("Filtering is disabled :)\r\n");
		ConsoleOutput("\r\n");
		printToLocalConsole("Filtering is disabled!");
	}
}

