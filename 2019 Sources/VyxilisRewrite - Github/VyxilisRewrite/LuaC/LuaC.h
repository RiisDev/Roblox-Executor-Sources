#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <istream>
#include <iterator>
#include <sstream>

#include "../Offsets.h"


static std::vector<std::string> SplitArguments(std::string string)
{
	std::vector<std::string> elements;
	std::stringstream start(string);
	std::istream_iterator<std::string> begin(start);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vectorStrings(begin, end);
	return vectorStrings;
}


void LuaCExecute(std::string input) {
	try {
		std::vector<std::string> args = SplitArguments(input);

		if (args.at(0) == "getfield") {
			r_lua_getfield(RLS, stoi(args.at(1)), args.at(2).c_str());
		}

		else if (args.at(0) == "getglobal") {
			r_lua_getglobal(RLS, args.at(1).c_str());
		}

		else if (args.at(0) == "setfield") {
			r_lua_setfield(RLS, stoi(args.at(1)), args.at(2).c_str());
		}

		else if (args.at(0) == "pushvalue") {
			r_lua_pushvalue(RLS, stoi(args.at(1)));
		}

		else if (args.at(0) == "pushstring") {
			std::string string;
			for (size_t i = 1; i < args.size(); i++) {
				if (i < (args.size() - 1)) {
					string.append(args.at(i) + ' ');
				}
				if (i == (args.size() - 1)) {
					string.append(args.at(i));
				}
			}
			r_lua_pushstring(RLS, string.c_str());
		}

		else if (args.at(0) == "pushnumber") {
			r_lua_pushnumber(RLS, stoi(args.at(1)));
		}

		else if (args.at(0) == "pushboolean") {
			r_lua_pushnumber(RLS, stoi(args.at(1)));
		}

		else if (args.at(0) == "pcall") {
			r_lua_pcall(RLS, stoi(args.at(1)), stoi(args.at(2)), stoi(args.at(3)));
		}

		else if (args.at(0) == "emptystack") {
			r_lua_settop(RLS, 0);
		}
	}
	catch (...) {

	}
}
