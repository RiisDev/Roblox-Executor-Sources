#include <windows.h>
#include <string>
#include <vector>
#include "LuaCFunctions.h"
#include "CustomFunctions.h"

#ifndef ussr
#define ussr

using std::vector;
using std::string;

extern vector<string> Parse(string input);
extern vector<string> ParseByChar(string input, char delimeter);

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
	vector<std::string> Input;
	Input = Parse(input);

	switch (returnop(Input.at(0))) {

	case OP_GETGLOBAL: {
		LuaC::rbx_getglobal(State, Input.at(1));
		break;
	}

	case OP_GETFIELD: {
		int one = stoi(Input.at(1));
		std::string two;
		for (int i = 2; i < Input.size(); ++i) {
			if ((Input.size() - 1) > i) two.append(Input.at(i) + " ");
			else if ((Input.size() - 1) == i) two.append(Input.at(i));
		}
		LuaC::rbx_getfield(State, one, two);
		break;
	}

	case OP_SETGLOBAL: {
		LuaC::rbx_setglobal(State, Input.at(1));
		break;
	}

	case OP_SETFIELD: {
		int one = stoi(Input.at(1));
		std::string two;
		for (int i = 2; i < Input.size(); ++i) {
			if ((Input.size() - 1) > i) two.append(Input.at(i) + " ");
			else if ((Input.size() - 1) == i) two.append(Input.at(i));
		}
		LuaC::rbx_setfield(State, one, two);
		break;
	}

	//case OP_RAWGETI: {
	//	int one = stoi(Input.at(1));
	//	int two = stoi(Input.at(2));
	//	LuaC::rbx_rawgeti(State, one, two);
	//}

	//case OP_RAWSETI: {
	//	int one = stoi(Input.at(1));
	//	signed int two = stoi(Input.at(2));
	//	LuaC::rbx_rawseti(State, one, two);
	//}

	case OP_PCALL: {
		int nargs = stoi(Input.at(1));
		int nresults = stoi(Input.at(2));
		int errfunc = stoi(Input.at(3));
		LuaC::rbx_pcall(State, nresults, nargs, errfunc);
		break;
	}

	case OP_CALL: {
		int one = stoi(Input.at(1));
		int two = stoi(Input.at(2));
		LuaC::rbx_pcall(State, one, two, 0);
		break;
	}

	case OP_PUSHNIL: {
		LuaC::rbx_pushnil(State);
		break;
	}

	case OP_PUSHSTRING: {
		std::string one;
		int size = Input.size();
		for (int i = 1; i < size; ++i) {
			if ((size - 1) > i) one.append(Input.at(i) + " ");
			else if ((size - 1) == i) one.append(Input.at(i));
		}
		LuaC::rbx_pushstring(State, one);
		break;
	}

	case OP_PUSHVALUE: {
		int one = stoi(Input.at(1));
		LuaC::rbx_pushvalue(State, one);
		break;
	}

	case OP_PUSHNUMBER: {
		double one = stod(Input.at(1));
		LuaC::rbx_pushnumber(State, one);
		break;
	}

	case OP_SETTOP: {
		int one = stoi(Input.at(1));
		LuaC::rbx_settop(State, one);
		break;
	}

	case OP_PUSHBOOLEAN: {
		if (Input.at(1) == "true" || Input.at(1) == "1") {
			LuaC::rbx_pushboolean(State, true);
		}
		else if (Input.at(1) == "false" || Input.at(1) == "0") {
			LuaC::rbx_pushboolean(State, false);
		}
		break;
	}

	case OP_POP: {
		int one = stoi(Input.at(1));
		LuaC::rbx_pop(State, one);
		break;
	}

	case OP_WAIT: {
		double one = stod(Input.at(1));
		Sleep(one * 1000);
		break;
	}

	//I WANT TO DIE ^

	case OP_EMPTYSTACK: {
		LuaC::rbx_settop(State, 0);
		break;
	}

	default: {
		break;
	}
	}
}

#endif

//WHAT A GREAT MEME