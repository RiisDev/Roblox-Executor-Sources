#pragma once
#include "RobloxLua.h"

#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
std::vector<std::string> Split(std::string str, char Delim)
{
	std::vector<std::string> Args;
	std::stringstream ss(str);
	std::string Arg;
	while (std::getline(ss, Arg, Delim))
		Args.push_back(Arg);
	return Args;
}

std::string to_lower(std::string str)
{
	std::string retn("");
	for (size_t i = 0; i < str.length(); i++)
	{
		int ascii = (int)str.at(i);
		if (ascii >= (int)'a' && ascii <= (int)'z')
			retn += (char)ascii;
		else
			retn += (char)(ascii + ((int)'a' - (int)'A'));
	}
	return retn;
}

const char* cmds[]{
	"ws [p] [v] Edit's the players walkspeed to [Value].",
	"jp [p] [v] Edit the Player's walkspeed to [Value]."
};

void hCmd(std::string command)
{
	std::vector<std::string> Arg = Split(command, ' ');

	try
	{
		if (to_lower(Arg.at(0)) == "cmds")
		{
			for (int i = 0; i < 2; i++)
			{
				std::cout << cmds[i] << std::endl;
			}
		}

		else if (to_lower(Arg.at(0)) == "ws")
		{
			if (to_lower(Arg.at(1)) == "me")
			{
				RBLX::r_lua_getglobal(rL, "game");
				RBLX::r_lua_getfield(rL, -1, "Players");
				RBLX::r_lua_getfield(rL, -1, "LocalPlayer");
				RBLX::r_lua_getfield(rL, -1, "Character");
				RBLX::r_lua_getfield(rL, -1, "Humanoid");
				RBLX::r_lua_pushnumber(rL, std::stof(Arg[2]));
				RBLX::r_lua_setfield(rL, -2, "WalkSpeed");
				RBLX::r_lua_settop(rL, 0);
			}
			else
			{
				RBLX::r_lua_getglobal(rL, "game");
				RBLX::r_lua_getfield(rL, -1, "Players");
				RBLX::r_lua_getfield(rL, -1, Arg.at(1).c_str());
				RBLX::r_lua_getfield(rL, -1, "Character");
				RBLX::r_lua_getfield(rL, -1, "Humanoid");
				RBLX::r_lua_pushnumber(rL, std::stof(Arg[2]));
				RBLX::r_lua_setfield(rL, -2, "WalkSpeed");
				RBLX::r_lua_settop(rL, 0);
			}
		}
		else if (to_lower(Arg.at(0)) == "print")
		{
			std::string ok;
			for (size_t i = 1; i < Arg.size(); i++) {
				if (i < (Arg.size() - 1)) {
					ok.append(Arg.at(i) + ' ');
				}
				else if (i == (Arg.size() - 1)) {
					ok.append(Arg.at(i));
				}
				else {

				}
			}
			RBLX::r_lua_getglobal(rL, "print");
			RBLX::r_lua_pushstring(rL, ok.c_str());
			RBLX::r_lua_pcall(rL, 1, 0, 0);
		}

		else if (to_lower(Arg.at(0)) == "jp")
		{
			if (to_lower(Arg.at(1)) == "me")
			{
				RBLX::r_lua_getglobal(rL, "game");
				RBLX::r_lua_getfield(rL, -1, "Players");
				RBLX::r_lua_getfield(rL, -1, "LocalPlayer");
				RBLX::r_lua_getfield(rL, -1, "Character");
				RBLX::r_lua_getfield(rL, -1, "Humanoid");
				RBLX::r_lua_pushnumber(rL, std::stof(Arg[2]));
				RBLX::r_lua_setfield(rL, -2, "JumpPower");
				RBLX::r_lua_settop(rL, 0);
			}
			else
			{
				RBLX::r_lua_getglobal(rL, "game");
				RBLX::r_lua_getfield(rL, -1, "Players");
				RBLX::r_lua_getfield(rL, -1, Arg.at(1).c_str());
				RBLX::r_lua_getfield(rL, -1, "Character");
				RBLX::r_lua_getfield(rL, -1, "Humanoid");
				RBLX::r_lua_pushnumber(rL, std::stof(Arg[2]));
				RBLX::r_lua_setfield(rL, -2, "JumpPower");
				RBLX::r_lua_settop(rL, 0);
			}
		}
		else if (to_lower(Arg.at(0)) == "kill")
		{
			if (to_lower(Arg.at(1)) == "me")
			{
				RBLX::r_lua_getglobal(rL, "game");
				RBLX::r_lua_getfield(rL, -1, "Players");
				RBLX::r_lua_getfield(rL, -1, "LocalPlayer");
				RBLX::r_lua_getfield(rL, -1, "Character");
				RBLX::r_lua_getfield(rL, -1, "BreakJoints");
				RBLX::r_lua_pushvalue(rL, -2);
				RBLX::r_lua_pcall(rL, 1, 0, 0);
				RBLX::r_lua_settop(rL, 0);
			}
			else
			{
				RBLX::r_lua_getglobal(rL, "game");
				RBLX::r_lua_getfield(rL, -1, "Players");
				RBLX::r_lua_getfield(rL, -1, Arg.at(1).c_str());
				RBLX::r_lua_getfield(rL, -1, "Character");
				RBLX::r_lua_getfield(rL, -1, "BreakJoints");
				RBLX::r_lua_pushvalue(rL, -2);
				RBLX::r_lua_pcall(rL, 1, 0, 0);
				RBLX::r_lua_settop(rL, 0);
			}
		}

	}
	catch (...)
	{
		std::cout << "was not able to run the Command inputted!" << std::endl;
	}
}
