#pragma once

#include <string>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include <istream>
#include <iterator>
#include "RLua.h"
#include "Scanner.h"
#define _CRT_SECURE_NO_WARNINGS
std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}
void CommandCentral(std::string cmd)
{
	std::vector<std::string>cmdsplit = split(cmd);
	if (cmdsplit.at(0) == "cmds")
	{
		std::cout << "cmds - Prints out cmds!" << std::endl;
		std::cout << "print - prints out to console!" << std::endl;
		std::cout << ">" << std::endl;
	}
	else if (cmdsplit.at(0) == "print") {
		LuaCFunctions::getglobal(robloxluastate, "print");
		LuaCFunctions::pushstring(robloxluastate, cmdsplit.at(1).c_str()); //credits to rakion99 for this one c_str
		LuaCFunctions::pcall(robloxluastate, 1, 0, 0);
		std::cout << "Command Executed!" << std::endl;
		std::cout << ">" << std::endl;
	}
}