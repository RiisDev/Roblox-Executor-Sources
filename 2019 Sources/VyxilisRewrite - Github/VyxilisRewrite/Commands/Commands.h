#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <istream>
#include <iterator>
#include <sstream>
#include <algorithm>

#include "../Offsets.h"
#include "../ConsoleCol.h"

static std::vector<std::string> Split_String(std::string String, char Delimiter) {
	std::stringstream test(String);
	std::string segment;
	std::vector<std::string> seglist;

	while (std::getline(test, segment, Delimiter)) {
		seglist.push_back(segment);
	}

	return seglist;
}

inline std::vector<std::string> GetPlayersFromArguments(std::string Arguments)
{
	std::vector<std::string> CandidatePlayers;
	if (Arguments == "" || Arguments == " ") {
		r_lua_getglobal(RLS, "game");
		r_lua_getfield(RLS, -1, "Players");
		r_lua_getfield(RLS, -1, "LocalPlayer");
		r_lua_getfield(RLS, -1, "Name");
		std::string LocalPlayer = r_lua_tostring(RLS, -1);
		CandidatePlayers.push_back(LocalPlayer);
		return CandidatePlayers;
	}
	if (Arguments == "me") {
		r_lua_getglobal(RLS, "game");
		r_lua_getfield(RLS, -1, "Players");
		r_lua_getfield(RLS, -1, "LocalPlayer");
		r_lua_getfield(RLS, -1, "Name");
		std::string LocalPlayer = r_lua_tostring(RLS, -1);
		CandidatePlayers.push_back(LocalPlayer);
	}
	else if (Arguments == "all" || Arguments == "everyone") {
		r_lua_getglobal(RLS, "game");
		r_lua_getfield(RLS, -1, "Players");
		r_lua_getfield(RLS, -1, "GetChildren");
		r_lua_pushvalue(RLS, -2);
		r_lua_pcall(RLS, 1, 1, 0);
		r_lua_pushnil(RLS);
		while (r_lua_next(RLS, -2) != 0) {
			std::string Player;
			r_lua_getfield(RLS, -1, "Name");
			Player = r_lua_tostring(RLS, -1);
			r_lua_pop(RLS , 2);
			CandidatePlayers.push_back(Player);
		}
	}
	else if (Arguments == "others") {
		r_lua_getglobal(RLS, "game");
		r_lua_getfield(RLS, -1, "Players");
		r_lua_getfield(RLS, -1, "GetChildren");
		r_lua_pushvalue(RLS, -2);
		r_lua_pcall(RLS, 1, 1, 0);
		r_lua_pushnil(RLS);
		while (r_lua_next(RLS, -2) != 0) {
			std::string Player;
			r_lua_getfield(RLS, -1, "Name");
			Player = r_lua_tostring(RLS, -1);
			r_lua_pop(RLS, 2);
			CandidatePlayers.push_back(Player);
		}
		std::string LocalPlayer;
		r_lua_getglobal(RLS, "game");
		r_lua_getfield(RLS, -1, "Players");
		r_lua_getfield(RLS, -1, "LocalPlayer");
		r_lua_getfield(RLS, -1, "Name");
		LocalPlayer = r_lua_tostring(RLS, -1);
		std::vector<std::string>::iterator iteration = find(CandidatePlayers.begin(), CandidatePlayers.end(), LocalPlayer);
		CandidatePlayers.erase(iteration);
	}
	else if (Arguments == "random") {
		r_lua_getglobal(RLS, "game");
		r_lua_getfield(RLS, -1, "Players");
		r_lua_getfield(RLS, -1, "GetChildren");
		r_lua_pushvalue(RLS, -2);
		r_lua_pcall(RLS, 1, 1, 0);
		r_lua_pushnil(RLS);
		while (r_lua_next(RLS, -2) != 0) {
			std::string Player;
			r_lua_getfield(RLS, -1, "Name");
			Player = r_lua_tostring(RLS, -1);
			r_lua_pop(RLS, 2);
			CandidatePlayers.push_back(Player);
		}
		srand(time(0));
		int Pick = rand() % CandidatePlayers.size();
		std::string TheLuckySod = CandidatePlayers[Pick];
		CandidatePlayers.clear();
		CandidatePlayers.push_back(TheLuckySod);
	}
	else if (Arguments == "guests") {
		r_lua_getglobal(RLS, "game");
		r_lua_getfield(RLS, -1, "Players");
		r_lua_getfield(RLS, -1, "GetChildren");
		r_lua_pushvalue(RLS, -2);
		r_lua_pcall(RLS, 1, 1, 0);
		r_lua_pushnil(RLS);
		while (r_lua_next(RLS, -2) != 0) {
			std::string Player;
			r_lua_getfield(RLS, -1, "Name");
			Player = r_lua_tostring(RLS, -1);
			r_lua_pop(RLS, 2);
			if (Player.find("Guest ") != std::string::npos) {
				CandidatePlayers.push_back(Player);
			}
		}
	}
	else {
		std::vector<std::string> Iteration;
		r_lua_getglobal(RLS, "game");
		r_lua_getfield(RLS, -1, "Players");
		r_lua_getfield(RLS, -1, "GetChildren");
		r_lua_pushvalue(RLS, -2);
		r_lua_pcall(RLS, 1, 1, 0);
		r_lua_pushnil(RLS);
		while (r_lua_next(RLS, -2) != 0) {
			std::string Player;
			r_lua_getfield(RLS, -1, "Name");
			Player = r_lua_tostring(RLS, -1);
			r_lua_pop(RLS, 2);
			Iteration.push_back(Player);
		}
		for (DWORD i = 0; i < Iteration.size(); ++i) {
			std::string PlayerName = Iteration[i];
			std::transform(PlayerName.begin(), PlayerName.end(), PlayerName.begin(), tolower);
			std::string ShortName = PlayerName.substr(0, Arguments.size());
			if (ShortName == Arguments) {
				CandidatePlayers.push_back(Iteration[i]);
			}
		}
	}
	return CandidatePlayers;
}

static std::vector<std::string> ParseArguments(std::string string)
{
	std::vector<std::string> elements;
	std::stringstream start(string);
	std::istream_iterator<std::string> begin(start);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vectorStrings(begin, end);
	return vectorStrings;
}

constexpr unsigned int FormatCommand(const char* str, int h = 0) {
	return !str[h] ? 5381 : (FormatCommand(str, h + 1) * 33) ^ str[h];
}


void RunCommand(std::string CommandInput) {
	std::vector<std::string> CMDInput = ParseArguments(CommandInput);

	if (CMDInput.at(0) == "cmds") {
		std::cout << igreen << "Vyxilis Commands: \n";
		std::cout << ipurple << "Supported Command Arguments: " << white << "[me] [all/everyone] [others] [random] [guests] [playername (short names are allowed!)] (if no argument is supplied it will default to running the command on you)\n";
		std::cout << iyellow << "cmds" << ipurple << "- " << white << "prints out this list in the console!\n";
		std::cout << iyellow << "esp/wallhack [plr] " << ipurple << "- " << white << "Places a marker above [plr] head that allows you to always know their location!\n";
		std::cout << iyellow << "explode/bigoof [plr] " << ipurple << "- " << white << "Explodes [plr], thus killing them!\n";
		return;
	}

	if (CMDInput.size() == 0) { return; }

	std::vector<std::string> Players = GetPlayersFromArguments(CMDInput.at(1));

	std::string CMD = CMDInput.front(); CMDInput.erase(CMDInput.begin());

	if (Players.size() == 0) { return; }

	switch (FormatCommand(CMD.c_str()))
	{

	case FormatCommand("esp"): case FormatCommand("wallhack"): {

		for (unsigned i = 0; i < Players.size(); ++i) { //do this for every command: it loops thru all the players in the arg list and runs it on them
			try
			{
				std::string CurrentPlayerIteration = Players[i]; //and this is the current player name
				//so if you are getting their team or whatever you would
				//getservice players
				//getfield -1 then CurrentPlayerIteration.c_str() so u get the player that its being run on rn

				r_lua_getService(RLS, "Players");
				r_lua_getfield(RLS, -1, "LocalPlayer");
				r_lua_getfield(RLS, -1, "TeamColor");
				r_lua_getfield(RLS, -1, "Name");

				const char* LocalPlayer = r_lua_tostring(RLS, -1);

				r_lua_getService(RLS, "Players");
				r_lua_getfield(RLS, -1, CurrentPlayerIteration.c_str());
				r_lua_getfield(RLS, -1, "TeamColor");
				r_lua_getfield(RLS, -1, "Name");

				if (r_lua_tostring(RLS, -1) != LocalPlayer) {

					r_lua_getService(RLS, "Players");
					r_lua_getfield(RLS, -1, CurrentPlayerIteration.c_str());
					r_lua_getfield(RLS, -1, "Character");
					r_lua_getfield(RLS, -1, "Head");
					r_lua_getfield(RLS, -1, "BillboardGui");
					r_lua_getfield(RLS, -1, "Remove");
					r_lua_pushvalue(RLS, -2);
					r_lua_pcall(RLS, 1, 0, 0);


					r_lua_getService(RLS, "Players");
					r_lua_getfield(RLS, -1, CurrentPlayerIteration.c_str());
					r_lua_getfield(RLS, -1, "Character");
					r_lua_getfield(RLS, -1, "Head");

					r_lua_getglobal(RLS, "Instance");
					r_lua_getfield(RLS, -1, "new");
					r_lua_pushstring(RLS, "BillboardGui");
					r_lua_pushvalue(RLS, -4);
					r_lua_pcall(RLS, 2, 1, 0);

					r_lua_pushboolean(RLS, true);
					r_lua_setfield(RLS, -2, "AlwaysOnTop");


					r_lua_getglobal(RLS, "UDim2");
					r_lua_getfield(RLS, -1, "new");
					r_lua_pushnumber(RLS, 4);
					r_lua_pushnumber(RLS, 0);
					r_lua_pushnumber(RLS, 4.5);
					r_lua_pushnumber(RLS, 0);
					r_lua_pcall(RLS, 4, 1, 0);
					r_lua_setfield(RLS, -3, "Size");


					r_lua_getService(RLS, "Players");
					r_lua_getfield(RLS, -1, CurrentPlayerIteration.c_str());
					r_lua_getfield(RLS, -1, "Character");
					r_lua_getfield(RLS, -1, "Head");
					r_lua_getfield(RLS, -1, "BillboardGui");

					r_lua_getglobal(RLS, "Instance");
					r_lua_getfield(RLS, -1, "new");
					r_lua_pushstring(RLS, "Frame");
					r_lua_pushvalue(RLS, -4);
					r_lua_pcall(RLS, 2, 1, 0);


					r_lua_pushnumber(RLS, 0);
					r_lua_setfield(RLS, -2, "BorderSizePixel");

					r_lua_pushnumber(RLS, 0.5);
					r_lua_setfield(RLS, -2, "Transparency");


					r_lua_getglobal(RLS, "UDim2");
					r_lua_getfield(RLS, -1, "new");
					r_lua_pushnumber(RLS, 1);
					r_lua_pushnumber(RLS, 0);
					r_lua_pushnumber(RLS, 1);
					r_lua_pushnumber(RLS, 0);
					r_lua_pcall(RLS, 4, 1, 0);
					r_lua_setfield(RLS, -3, "Size");


					r_lua_getService(RLS, "Players");
					r_lua_getfield(RLS, -1, CurrentPlayerIteration.c_str());
					r_lua_getfield(RLS, -1, "TeamColor");
					r_lua_getfield(RLS, -1, "Color");

					r_lua_getglobal(RLS, "Workspace");
					r_lua_getfield(RLS, -1, CurrentPlayerIteration.c_str());
					r_lua_getfield(RLS, -1, "Head");
					r_lua_getfield(RLS, -1, "BillboardGui");
					r_lua_getfield(RLS, -1, "Frame");

					r_lua_pushvalue(RLS, -6);
					r_lua_setfield(RLS, -2, "BackgroundColor3");
				}
			}
			catch (std::exception E) {
				printf("Error While Running ESP/WallHack Command: %s\n", E.what());
			}
		}
		break;
	}


	case FormatCommand("explode"): case FormatCommand("bigoof") : {
		for (int i = 0; i < Players.size(); ++i) {
			try {
				std::string CurrentPlayerIteration = Players[i];
				r_lua_getService(RLS, "Players");
				r_lua_getfield(RLS, -1, CurrentPlayerIteration.c_str());
				r_lua_getfield(RLS, -1, "Character");
				r_lua_getglobal(RLS, "Instance");
				r_lua_getfield(RLS, -1, "new");
				r_lua_pushstring(RLS, "Explosion");
				r_lua_pushvalue(RLS, -4);
				r_lua_pcall(RLS, 2, 1, 0);
				r_lua_pushnumber(RLS, 999999999);
				r_lua_setfield(RLS, -2, "BlastPressure");
				r_lua_settop(RLS, 0);
				r_lua_getService(RLS, "Players");
				r_lua_getfield(RLS, -1, CurrentPlayerIteration.c_str());
				r_lua_getfield(RLS, -1, "Character");
				r_lua_getfield(RLS, -1, "Torso");
				r_lua_getfield(RLS, -1, "Position");
				r_lua_getService(RLS, "Players");
				r_lua_getfield(RLS, -1, CurrentPlayerIteration.c_str());
				r_lua_getfield(RLS, -1, "Character");
				r_lua_getfield(RLS, -1, "Explosion");
				r_lua_pushvalue(RLS, -6);
				r_lua_setfield(RLS, -2, "Position");
				r_lua_settop(RLS, 0);
			}
			catch (std::exception E) {
				printf("Error While Running Explode/bigoof Command: %s\n", E.what());
			}
		}
	}

	default: {
		break;
	}
	}
}
