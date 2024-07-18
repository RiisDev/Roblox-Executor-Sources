#pragma once
#include "bigheckinglua.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <istream>
#include <iterator>
#include <sstream>


using std::vector;
using std::string;





std::vector<std::string> Split(std::string str, char Delim) {
	std::vector<std::string> Args;
	std::stringstream ss(str);
	std::string Arg;
	while (getline(ss, Arg, Delim))
		Args.push_back(Arg);
	return Args;
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


enum LuaC_OPCODES {
	OP_GETGLOBAL,
	OP_GETFIELD,
	OP_SETFIELD,
	OP_SETGLOBAL,
	OP_RAWGETI,
	OP_RAWSETI,
	OP_PUSHSTRING,
	OP_PUSHVALUE,
	OP_PUSHNUMBER,
	OP_PUSHBOOLEAN,
	OP_PUSHNIL,
	OP_PUSHINTEGER,
	OP_PCALL,
	OP_CALL,
	OP_SETTOP,
	OP_EMPTYSTACK,
	OP_WAIT,
	OP_POP,
	OP_PUSH,
	OP_TOSTRING
};

inline LuaC_OPCODES returnop(std::string func) {
	if (func == "getglobal") return OP_GETGLOBAL;
	if (func == "getfield") return OP_GETFIELD;
	if (func == "setglobal") return OP_SETGLOBAL;
	if (func == "setfield") return OP_SETFIELD;
	if (func == "rawgeti") return OP_RAWGETI;
	if (func == "rawseti") return OP_RAWSETI;
	if (func == "pushstring") return OP_PUSHSTRING;
	if (func == "pushnumber") return OP_PUSHNUMBER;
	if (func == "pushvalue") return OP_PUSHVALUE;
	if (func == "pushnil") return OP_PUSHNIL;
	if (func == "pushinteger") return OP_PUSHINTEGER;
	if (func == "pushboolean" || func == "pushbool") return OP_PUSHBOOLEAN;
	if (func == "pcall") return OP_PCALL;
	if (func == "call") return OP_CALL;
	if (func == "settop") return OP_SETTOP;
	if (func == "emptystack") return OP_EMPTYSTACK;
	if (func == "wait") return OP_WAIT;
	if (func == "pop") return OP_POP;
	if (func == "tostring") return OP_TOSTRING;
}


inline void LuaCExecute(std::string input) {
	//vector<std::string> Input;
	std::istringstream StringStreamedString(input);
	std::vector<std::string> SplitLines;
	std::string HeldLine;
	while (std::getline(StringStreamedString, HeldLine)) {
		SplitLines.push_back(HeldLine);
		HeldLine = "";
	}
	for (unsigned i = 0; i < SplitLines.size(); i++) {

		std::string CurrentLine = SplitLines[i];
		std::vector<std::string> Input = SplitArguments(CurrentLine);



		switch (returnop(Input.at(0))) {

		


		case OP_GETGLOBAL: {
			r_lua_getglobal(Roblox, Input.at(1).c_str());
			printf("[DEBUG]Running: OP_GETGLOBAL\n", OP_GETGLOBAL);
			break;
		}

		case OP_GETFIELD: {
			int one = stoi(Input.at(1));
			r_lua_getfield(Roblox, one, (Input.at(2).c_str()));
			printf("[DEBUG]Running: OP_GETFIELD\n", OP_GETFIELD);
			break;
		}
		case OP_PCALL: {

			int e = stoi(Input.at(1));
			int e1 = stoi(Input.at(2));
			int e2 = stoi(Input.at(3));

			r_lua_pcall(Roblox, e, e1, e2);
			printf("[DEBUG] Running:  OP_PCALL\n");
			break;


		}
		case OP_PUSHSTRING: {
			std::string k;
			for (size_t i = 1; i < Input.size(); i++) {
				if (i < (Input.size() - 1)) {
					k.append(Input.at(i) + ' ');
				}
				else if (i == (Input.size() - 1)) {
					k.append(Input.at(i));
				}
				else {

				}
			}
			r_lua_pushstring(Roblox, k.c_str());
			printf("[DEBUG] Running: OP_PUSHSTRING\n");
		}


		}
		
	}
	SplitLines.clear();
}


