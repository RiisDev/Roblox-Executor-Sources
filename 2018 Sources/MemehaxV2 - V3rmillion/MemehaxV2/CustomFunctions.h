#include <windows.h>
#include "LuaCFunctions.h"
#include <string>
#include <vector>
#include <time.h>
#include <algorithm>

#ifndef okay
#define okay

using std::string;
using std::vector;

extern unsigned long State;
extern vector<string> Parse(string input);
extern vector<string> ParseByChar(string input, char delimeter);

std::string lol;
std::string lolagain;

inline void cl_get(string sGet) {
	vector<string> get = ParseByChar(sGet, '.');
	LuaC::rbx_getglobal(State, get.at(0).c_str());
	for (int i = 1; i < get.size(); ++i) {
		if (get.at(i) != get.at(0)) {
			LuaC::rbx_getfield(State, -1, get.at(i).c_str());
		}
	}
}
//written by topdeedoop but i've been too lazy to replace it lMaO ded

inline void exetemp() {
	LuaC::rbx_getglobal(State, "print");
	LuaC::rbx_pushstring(State, lol);
	LuaC::rbx_pcall(State, 1, 0, 0);
	//NASTY
}

inline void exethreadtemp() {
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)exetemp, NULL, NULL, NULL);
	//NASTY
}

inline void printctemp() {
	string target;
	cl_get(lolagain);
	LuaC::rbx_pushvalue(State, -2);
	LuaC::rbx_pcall(State, 1, 1, 0);
	LuaC::rbx_pushnil(State);
	while (LuaC::rbx_next(State, -2) != 0) {
		string name;
		string classname;
		LuaC::rbx_pushvalue(State, -1);
		LuaC::rbx_getfield(State, -1, "Name");
		name = LuaC::rbx_tolstring(State, -1, 0);
		rbx_pop(State, 2);
		LuaC::rbx_pushvalue(State, -1);
		LuaC::rbx_getfield(State, -1, "ClassName");
		classname = LuaC::rbx_tolstring(State, -1, 0);
		rbx_pop(State, 3);
		lol = classname + ": " + name;
		Sleep(45);
		exethreadtemp();
	}
	//NASTYYYYY
}

inline void printcthreadtemp() {
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)printctemp, NULL, NULL, NULL);
	//still NASTY
}

inline string GetLocalPlayer() {
	LuaC::rbx_getglobal(State, "game");
	LuaC::rbx_getfield(State, -1, "Players");
	LuaC::rbx_getfield(State, -1, "LocalPlayer");
	LuaC::rbx_getfield(State, -1, "Name");
	std::string LocalPlayer = LuaC::rbx_tolstring(State, -1, 0);
	return LocalPlayer;
}

inline vector<string> GetPlayerName(std::string input)
{
	vector<string> Players;
	if (input == "me") {
		LuaC::rbx_getglobal(State, "game");
		LuaC::rbx_getfield(State, -1, "Players");
		LuaC::rbx_getfield(State, -1, "LocalPlayer");
		LuaC::rbx_getfield(State, -1, "Name");
		std::string LocalPlayer = LuaC::rbx_tolstring(State, -1, 0);
		Players.push_back(LocalPlayer);
	}
	else if (input == "all") {
		LuaC::rbx_getglobal(State, "game");
		LuaC::rbx_getfield(State, -1, "Players");
		LuaC::rbx_getfield(State, -1, "GetChildren");
		LuaC::rbx_pushvalue(State, -2);
		LuaC::rbx_pcall(State, 1, 1, 0);
		LuaC::rbx_pushnil(State);
		while (LuaC::rbx_next(State, -2) != 0) {
			std::string Player;
			LuaC::rbx_getfield(State, -1, "Name");
			Player = LuaC::rbx_tolstring(State, -1, 0);
			rbx_pop(State, 2);
			Players.push_back(Player);
		}
	}
	else if (input == "others") {
		LuaC::rbx_getglobal(State, "game");
		LuaC::rbx_getfield(State, -1, "Players");
		LuaC::rbx_getfield(State, -1, "GetChildren");
		LuaC::rbx_pushvalue(State, -2);
		LuaC::rbx_pcall(State, 1, 1, 0);
		LuaC::rbx_pushnil(State);
		while (LuaC::rbx_next(State, -2) != 0) {
			std::string Player;
			LuaC::rbx_getfield(State, -1, "Name");
			Player = LuaC::rbx_tolstring(State, -1, 0);
			rbx_pop(State, 2);
			Players.push_back(Player);
		}
		std::string LocalPlayer;
		LuaC::rbx_getglobal(State, "game");
		LuaC::rbx_getfield(State, -1, "Players");
		LuaC::rbx_getfield(State, -1, "LocalPlayer");
		LuaC::rbx_getfield(State, -1, "Name");
		LocalPlayer = LuaC::rbx_tolstring(State, -1, 0);
		std::vector<std::string>::iterator iteration = find(Players.begin(), Players.end(), LocalPlayer);
		Players.erase(iteration);
	}
	else if (input == "random") {
		LuaC::rbx_getglobal(State, "game");
		LuaC::rbx_getfield(State, -1, "Players");
		LuaC::rbx_getfield(State, -1, "GetChildren");
		LuaC::rbx_pushvalue(State, -2);
		LuaC::rbx_pcall(State, 1, 1, 0);
		LuaC::rbx_pushnil(State);
		while (LuaC::rbx_next(State, -2) != 0) {
			std::string Player;
			LuaC::rbx_getfield(State, -1, "Name");
			Player = LuaC::rbx_tolstring(State, -1, 0);
			rbx_pop(State, 3);
			Players.push_back(Player);
		}
		srand(time(0));
		int pick = rand() % Players.size();
		std::string meme = Players[pick];
		Players.clear();
		Players.push_back(meme);
	}
	else {
		vector<string> Iteration;
		LuaC::rbx_getglobal(State, "game");
		LuaC::rbx_getfield(State, -1, "Players");
		LuaC::rbx_getfield(State, -1, "GetChildren");
		LuaC::rbx_pushvalue(State, -2);
		LuaC::rbx_pcall(State, 1, 1, 0);
		LuaC::rbx_pushnil(State);
		while (LuaC::rbx_next(State, -2) != 0) {
			std::string Player;
			LuaC::rbx_getfield(State, -1, "Name");
			Player = LuaC::rbx_tolstring(State, -1, 0);
			rbx_pop(State, 2);
			Iteration.push_back(Player);
		}
		for (DWORD i = 0; i < Iteration.size(); ++i) {
			string PlayerName = Iteration[i];
			std::transform(PlayerName.begin(), PlayerName.end(), PlayerName.begin(), tolower);
			std::string ShortName = PlayerName.substr(0, input.size());
			if (ShortName == input) {
				Players.push_back(Iteration[i]);
			}
		}
	}
	return Players;
}

#endif