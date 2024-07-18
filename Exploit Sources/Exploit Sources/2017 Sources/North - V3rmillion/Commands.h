#pragma once
#include "SDK.h"

// INITS
typedef void(*tFunction)(std::vector<std::string> params);
typedef std::map<const char*, const char*> props_map;

std::map<std::string, tFunction> cmds;

// TODO: print usage on incorrect command usage

// FUNCTIONS
void kill(std::vector<std::string> params) {

	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater.\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));

		for (int i = 0; i < plrs.size(); i++) {
			try {
				std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "BreakJoints" };
				rLua::Utils::getInstance(inst);
				rLua::pushvalue(rLua::state, -2);
				rLua::pcall(rLua::state, 1, 0, 0);
			}
			catch (std::exception e) {}
		} 
}

void ws(std::vector<std::string> params) {
	if (!validateCommand(params, 2)) {
		printf("\nIncorrect usage. Command expects 2 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));


	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, std::stoi(params.at(2)));
		rLua::setfield(rLua::state, -2, "WalkSpeed");
	}
}

void airwalk(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, 50);
		rLua::setfield(rLua::state, -2, "HipHeight");
	}
}

void noairwalk(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, 0);
		rLua::setfield(rLua::state, -2, "HipHeight");
	}
}

void btools(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}


	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Backpack" };
		int grab = rLua::Utils::newInstance(inst, "HopperBin", "n_Grab");
		rLua::pushnumber(rLua::state, 2);
		rLua::setfield(rLua::state, grab, "BinType");
		rLua::settop(rLua::state, 0);

		int clone = rLua::Utils::newInstance(inst, "HopperBin", "n_Clone");
		rLua::pushnumber(rLua::state, 3);
		rLua::setfield(rLua::state, clone, "BinType");
		rLua::settop(rLua::state, 0);

		int hammer = rLua::Utils::newInstance(inst, "HopperBin", "n_Hammer");
		rLua::pushnumber(rLua::state, 4);
		rLua::setfield(rLua::state, hammer, "BinType");
		rLua::settop(rLua::state, 0);
	}
}

void notools(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> grab_inst = { "Players", plrs.at(i), "Backpack", "n_Grab"};
		std::vector<const char*> clone_inst = { "Players", plrs.at(i), "Backpack", "n_Clone" };
		std::vector<const char*> hammer_inst = { "Players", plrs.at(i), "Backpack", "n_Hammer" };
		try {
			rLua::Utils::deleteInstance(grab_inst);
			rLua::Utils::deleteInstance(clone_inst);
			rLua::Utils::deleteInstance(hammer_inst);
		}
		catch(std::exception e) {}
	}
}

void watch(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));	
		std::vector<const char*> inst = { "Players", plrs.at(0), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::getglobal(rLua::state, "workspace");
		rLua::getfield(rLua::state, -1, "Camera");
		rLua::pushvalue(rLua::state, -3);
		rLua::setfield(rLua::state, -2, "CameraSubject");
}

void unwatch(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	std::vector<const char*> inst = { "Players", "LocalPlayer", "Character", "Humanoid" };
	rLua::Utils::getInstance(inst);
	rLua::getglobal(rLua::state, "workspace");
	rLua::getfield(rLua::state, -1, "Camera");
	rLua::pushvalue(rLua::state, -3);
	rLua::setfield(rLua::state, -2, "CameraSubject");
}

void punish(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character" };
		rLua::Utils::getInstance(inst);
		rLua::pushnil(rLua::state);
		rLua::setfield(rLua::state, -2, "Parent");
	}
}

void unpunish(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character" };
		rLua::getglobal(rLua::state, "workspace");
		rLua::Utils::getInstance(inst);
		rLua::pushvalue(rLua::state, -5);
		rLua::setfield(rLua::state, -2, "Parent");
		rLua::settop(rLua::state, 0);
	}
}

void jp(std::vector<std::string> params) {
	if (!validateCommand(params, 2)) {
		printf("\nIncorrect usage. Command expects 2 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, std::stoi(params.at(2)));
		rLua::setfield(rLua::state, -2, "JumpPower");
		rLua::settop(rLua::state, 0);
	}
}

void moonwalk(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushbool(rLua::state, 0);
		rLua::setfield(rLua::state, -2, "AutoRotate");
		rLua::settop(rLua::state, 0);
	}
}

void nomoon(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushbool(rLua::state, 1);
		rLua::setfield(rLua::state, -2, "AutoRotate");
		rLua::settop(rLua::state, 0);
	}
}

void god(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, 999999999999);
		rLua::setfield(rLua::state, -2, "MaxHealth");
		rLua::pushnumber(rLua::state, 999999999999);
		rLua::setfield(rLua::state, -2, "Health");
		rLua::settop(rLua::state, 0);
	}
}

void name(std::vector<std::string> params) {
	return;
	if (!validateCommand(params, 2)) {
		printf("\nIncorrect usage. Command expects 2 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character" };
		rLua::Utils::getInstance(inst);
		rLua::pushstring(rLua::state, params.at(2).c_str());
		rLua::setfield(rLua::state, -2, "Name");
		rLua::settop(rLua::state, 0);
	}
}

void ungod(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;

	}
	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, 100);
		rLua::setfield(rLua::state, -2, "MaxHealth");
		rLua::pushnumber(rLua::state, 100);
		rLua::setfield(rLua::state, -2, "Health");
		rLua::settop(rLua::state, 0);
	}
}

void charr(std::vector<std::string> params) {
	if (!validateCommand(params, 2)) {
		printf("\nIncorrect usage. Command expects 2 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i) };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, std::stoi(params.at(2)));
		rLua::setfield(rLua::state, -2, "CharacterAppearanceId");
		rLua::settop(rLua::state, 0);
		std::vector<const char*> inst_br = { "Players", plrs.at(i), "Character", "BreakJoints" };
		rLua::Utils::getInstance(inst_br);
		rLua::pushvalue(rLua::state, -2);
		rLua::pcall(rLua::state, 1, 0, 0);
	}
}

void rchar(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;

	}
	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i) };
		rLua::Utils::getInstance(inst);
		rLua::getfield(rLua::state, -1, "UserId");
		rLua::setfield(rLua::state, -2, "CharacterAppearanceId");
		rLua::settop(rLua::state, 0);
		std::vector<const char*> inst_br = { "Players", plrs.at(i), "Character", "BreakJoints" };
		rLua::Utils::getInstance(inst_br);
		rLua::pushvalue(rLua::state, -2);
		rLua::pcall(rLua::state, 1, 0, 0);
	}
}

void noob(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;

	}
	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i) };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, 60882279); // lol
		rLua::setfield(rLua::state, -2, "CharacterAppearanceId");
		rLua::settop(rLua::state, 0);
		std::vector<const char*> inst_br = { "Players", plrs.at(i), "Character", "BreakJoints" };
		rLua::Utils::getInstance(inst_br);
		rLua::pushvalue(rLua::state, -2);
		rLua::pcall(rLua::state, 1, 0, 0);
	}
}

void ff(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character" };
		rLua::Utils::newInstance(inst, "ForceField", "n_FF");
	}
}

void unff(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;

	}
	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		try {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "n_FF" };
		rLua::Utils::deleteInstance(inst);
		} catch (std::exception e) {}
	}
}

void smoke(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Torso" };
		rLua::Utils::newInstance(inst, "Smoke", "n_Smoke");
	}
}

void unsmoke(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;

	}
	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		try {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Torso", "n_Smoke" };
		rLua::Utils::deleteInstance(inst);
		} catch (std::exception e) {}
	}
}

void sparkles(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Torso" };
		rLua::Utils::newInstance(inst, "Sparkles", "n_Sparkes");
	}
}

void unsparkles(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		try {
			std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Torso", "n_Sparkles" };
			rLua::Utils::deleteInstance(inst);
		} catch (std::exception e) {}
	}
}

void fire(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Torso" };
		rLua::Utils::newInstance(inst, "Fire", "n_Fire");
	}
}

void unfire(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		try {
			std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Torso", "n_Fire" };
			rLua::Utils::deleteInstance(inst);
		}
		catch (std::exception e) {}
	}
}

void fogend(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> inst = { "Lighting" };
	rLua::Utils::getInstance(inst);
	rLua::pushnumber(rLua::state, std::stoi(params.at(1)));
	rLua::setfield(rLua::state, -2, "FogEnd");
}

void fogcolor(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> inst = { "Lighting" };
	rLua::Utils::getInstance(inst);
	//rLua::Utils::newColor3(std::stod(params.at(1)), std::stod(params.at(2)), std::stod(params.at(3)));
	rLua::setfield(rLua::state, -2, "FogColor");
}

void sound(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> p = { "Workspace" };
	rLua::Utils::newInstance(p, "Sound", "n_Sound");
	rLua::pushstring(rLua::state, ("rbxasset://" + params.at(1)).c_str());
	rLua::setfield(rLua::state, -2, "SoundId");
}

void freeze(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, 0);
		rLua::setfield(rLua::state, -2, "WalkSpeed");
	}
}

void unfreeze(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid" };
		rLua::Utils::getInstance(inst);
		rLua::pushnumber(rLua::state, 16);
		rLua::setfield(rLua::state, -2, "WalkSpeed");
	}
}

void clear(std::vector<std::string> params) {
	system("cls");
}

void getglobal(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	rLua::getglobal(rLua::state, params.at(1).c_str());
}

void getfield(std::vector<std::string> params) {
	if (!validateCommand(params, 2)) {
		printf("\nIncorrect usage. Command expects 2 paramater(s).\n");
		return;
	}

	rLua::getfield(rLua::state, std::stoi(params.at(1)), params.at(2).c_str());
}

void pcall(std::vector<std::string> params) {
	if (!validateCommand(params, 3)) {
		printf("\nIncorrect usage. Command expects 3 paramater(s).\n");
		return;
	}

	rLua::pcall(rLua::state, std::stoi(params.at(1)), std::stoi(params.at(2)), std::stoi(params.at(3)));
}

void setfield(std::vector<std::string> params) {
	if (!validateCommand(params, 2)) {
		printf("\nIncorrect usage. Command expects 2 paramater(s).\n");
		return;
	}

	rLua::setfield(rLua::state, std::stoi(params.at(1)), params.at(2).c_str());
}

void pushvalue(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	rLua::pushvalue(rLua::state, std::stoi(params.at(1)));
}

void pushstring(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	rLua::pushstring(rLua::state, params.at(1).c_str());
}

void pushnumber(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	rLua::pushnumber(rLua::state, std::stoi(params.at(1)));
}

void pushnil(std::vector<std::string> params) {
	rLua::pushnil(rLua::state);
}

void pushbool(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	rLua::pushbool(rLua::state, std::stoi(params.at(1)));
}

void tostring(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	printf("\n%s\n", rLua::tolstring(rLua::state, std::stoi(params.at(1)), NULL));
}

void noclip(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		printf(plrs.at(0));
		for (int x = 1; x < 18; x++) {
			printf(plrs.at(i));
			std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid", "SetStateEnabled" };
			rLua::Utils::getInstance(inst);
			rLua::pushvalue(rLua::state, -2);
			rLua::pushnumber(rLua::state, x);
			rLua::pushbool(rLua::state, 0);
			rLua::pcall(rLua::state, 3, 0, 0);
			rLua::settop(rLua::state, 0);
		}
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid", "SetStateEnabled" };
		rLua::Utils::getInstance(inst);
		rLua::pushvalue(rLua::state, -2);
		rLua::pushnumber(rLua::state, 10);
		rLua::pushbool(rLua::state, 1);
		rLua::pcall(rLua::state, 3, 0, 0);
		rLua::settop(rLua::state, 0);
	}
}

void clip(std::vector<std::string> params) {
	if (!validateCommand(params, 1)) {
		printf("\nIncorrect usage. Command expects 1 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		printf(plrs.at(0));
		for (int x = 1; x < 18; x++) {
			printf(plrs.at(i));
			std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Humanoid", "SetStateEnabled" };
			rLua::Utils::getInstance(inst);
			rLua::pushvalue(rLua::state, -2);
			rLua::pushnumber(rLua::state, x);
			rLua::pushbool(rLua::state, 1);
			rLua::pcall(rLua::state, 3, 0, 0);
			rLua::settop(rLua::state, 0);
		}
	}
}

void statchange(std::vector<std::string> params) {
	if (!validateCommand(params, 3)) {
		printf("\nIncorrect usage. Command expects 3 paramater(s).\n");
		return;
	}

	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	std::vector<const char*> inst = { "Players", plrs.at(0), "leaderstats", params.at(2).c_str(), "ClassName" };
	rLua::Utils::getInstance(inst);
	const char* type = rLua::tolstring(rLua::state, -1, NULL);
	std::cout << type;
	rLua::pushvalue(rLua::state, -2);
	if (std::string(type) == "IntValue") {
		rLua::pushnumber(rLua::state, std::stoi(params.at(3)));
	} else if(type == "NumberValue") {
		rLua::pushnumber(rLua::state, std::stod(params.at(3)));
	} else if (type == "BoolValue") {
		rLua::pushbool(rLua::state, std::stod(params.at(3)));
	} else {
		printf("\nUnsupported leaderstat value (%s).\n", type);
		return;
	}
	rLua::setfield(rLua::state, -2, "Value");
}	

void tp(std::vector<std::string> params) {
	std::vector<const char*> dest = rLua::Utils::parsePlayers(params.at(2));
	std::vector<const char*> src = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < src.size(); i++) {
		std::vector<const char*> instt = { "Players", src.at(i), "Character", "HumanoidRootPart" };
		rLua::Utils::getInstance(instt);
		rLua::Utils::newCFrame(110, 110, 110);
		rLua::setfield(rLua::state, -2, "CFrame");
	}
}

void giraffe(std::vector<std::string> params) {
	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Head" };
		rLua::Utils::getInstance(inst);
		rLua::Utils::newVector3(7, 7, 1);
		rLua::setfield(rLua::state, -2, "Size");
	}
}

void ungiraffe(std::vector<std::string> params) {
	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Head" };
		rLua::Utils::getInstance(inst);
		rLua::Utils::newVector3(2, 1, 1);
		rLua::setfield(rLua::state, -2, "Size");
	}
}

void norm(std::vector<std::string> params) {
	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Head" };
		rLua::Utils::getInstance(inst);
		rLua::Utils::newVector3(15, 15, 15);
		rLua::setfield(rLua::state, -2, "Size");
	}
}

void head(std::vector<std::string> params) {
	std::vector<const char*> plrs = rLua::Utils::parsePlayers(params.at(1));
	for (int i = 0; i < plrs.size(); i++) {
		std::vector<const char*> inst = { "Players", plrs.at(i), "Character", "Head" };
		rLua::Utils::getInstance(inst);
		rLua::Utils::newVector3(15, 15, 15);
		rLua::setfield(rLua::state, -2, "Size");
	}
}

const char* help_text =
"supported [p] inputs: all, others, me, alias (part of username), username\n"
"kill [p] - kills [p]\n"
"ws [p] [speed] - sets [p]'s walspeed to [speed]\n"
"airwalk [p] - lets [p] walk on air\n"
"noairwalk [p] - removes airwalk from [p]\n"
"btools [p] - gives [p] building tools\n"
"notools [p] - removes building tools from [p]\n"
"notools [p] - removes building tools from [p]\n"
"watch [p] - sets YOUR camera to [p]\n"
"unwatch - sets YOUR camera to back to normal\n"
"punish [p] - removes the character of [p]\n"
"unpunish [p] - readd the character of [p]\n"
"jp [p] [power] - sets the jumping power of [p] to [power]\n"
"moonwalk [p] - remove the rotation of [p]\n"
"nomoon [p] - fixes the rotation of [p]\n"
"god [p] - gives [p] 999999999999 health\n"
"ungod [p] - restores the health of [p]\n"
"name [p] [name] - sets the name of [p] to [name]\n"
"char [p] [id] - sets the appearance of [p] to the appearance of roblox user [id]. (this command will kill you)\n"
"rchar [p] [id] - sets the appearance of [p] back to normal\n"
"noob [p] - makes [p] a noob\n"
"ff [p] - gives [p] a forcefield\n"
"unff [p] - remove the forcefield of [p]\n"
"smoke [p] - makes [p] smoke\n"
"unsmoke [p] - makes [p] not smoke\n"
"sparkles [p] - makes [p] sparkle\n"
"unsparkle [p] - makes [p] not sparkle\n"
"fire [p] - makes [p] appear to be on fire\n"
"unfire [p] - makes [p] appear not on fire\n"
"fogend [end] - sets fogend to [end]\n"
"freeze [p] - freezes [p] in place\n"
"unfreeze [p] - unfreezes [p]\n"
"tp [p] [where] - teleports [p] to [where]\n"
"statchange [p] [name] [value] -sets the leaderboard stat [name] of [p] to [value]\n"
"admin [p] - allow [p] to use commands via chat (prefix is !)\n"
"-------- MISC COMMANDS --------\n"
"clear - clears the console\n"
"-------- LUA C COMMANDS (Advanced Users) --------\n"
"getglobal [str]\n"
"getfield [idx] [str]\n"
"setfield [idx] [str]\n"
"pcall [nargs] [nret] [nerr]\n"
"pushvalue [idx]\n"
"pushstring [str]\n"
"pushnil\n"
"pushbool [num (0-1)]\n"
"tostring [idx]\n";

void help(std::vector<std::string> params) {
	printf("\n%s\n", help_text);
}









