#pragma once
#include "RobloxLua.h"
#include <vector>
#include <iterator>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
static std::vector<std::string> SplitArguments(std::string string)
{
	std::vector<std::string> elements;
	std::stringstream start(string);
	std::istream_iterator<std::string> begin(start);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vectorStrings(begin, end);
	return vectorStrings;
}

void LuaC(std::string stringtorun)
{
	std::istringstream StringStreamedString(stringtorun);
	std::vector<std::string> SplitLines;
	std::string HeldLine;
	while (std::getline(StringStreamedString, HeldLine)) {
		SplitLines.push_back(HeldLine);
		HeldLine = "";
	}

	for (unsigned i = 0; i < SplitLines.size(); i++)
	{
		std::string CurrentLine = SplitLines[i];
		std::vector<std::string> Arg = SplitArguments(CurrentLine);
		try
		{

			if (Arg.at(0) == "pushstring")
			{
				std::string k;
				for (size_t i = 1; i < Arg.size(); i++) {
					if (i < (Arg.size() - 1)) {
						k.append(Arg.at(i) + ' ');
					}
					else if (i == (Arg.size() - 1)) {
						k.append(Arg.at(i));
					}
					else {

					}
				}
				RBLX::r_lua_pushstring(rL, k.c_str());
			}
			else if (Arg.at(0) == "getfield")
			{
				RBLX::r_lua_getfield(rL, -1, Arg.at(2).c_str());
			}
			else if (Arg.at(0) == "setfield")
			{
				RBLX::r_lua_setfield(rL, std::stoi(Arg.at(1)), Arg.at(2).c_str());
			}
			else if (Arg.at(0) == "pushnumber")
			{
				RBLX::r_lua_pushnumber(rL, std::stoi(Arg.at(1)));
			}
			else if (Arg.at(0) == "pcall")
			{
				RBLX::r_lua_pcall(rL, std::stoi(Arg[1]), std::stoi(Arg[2]), std::stoi(Arg[3]));
			}
			else if (Arg.at(0) == "getglobal")
			{
				RBLX::r_lua_getglobal(rL, Arg.at(1).c_str());
			}
			else if (Arg.at(0) == "emptystack")
			{
				RBLX::r_lua_settop(rL, 0);
			}
			else if (Arg.at(0) == "settop")
			{
				RBLX::r_lua_settop(rL, std::stoi(Arg.at(1).c_str()));
			}
			else if (Arg.at(0) == "pushboolean") 
			{
				RBLX::r_lua_pushboolean(rL, Arg.at(1).c_str());
			}
			if (Arg.at(0) == "pushvalue") 
			{
				RBLX::r_lua_pushvalue(rL, std::stoi(Arg.at(1)));
			}
		}
		catch (...)
		{
			printf("Could not Run LuaC Script! \n");
		}
	}
}
