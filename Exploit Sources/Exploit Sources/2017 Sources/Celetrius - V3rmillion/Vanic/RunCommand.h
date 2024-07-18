#pragma once
#include "SDK.h"
#include "RLua.h"
#include "Roblox.h"

std::string Input() {
	std::string Vanic;
	getline(std::cin, Vanic);
	return Vanic;
}

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

const char *Commands = ""
"* Vanic Commands --> *\r\n"
"Lua C Execution ---->\r\n"
"getglobal [s] - Gets the global of [s].\r\n"
"setglobal [s] - Sets the global of [s].\r\n"
"getfield [i] [s] - Gets the field of [s].\r\n"
"pushstring [s] - Pushs [s] on the stack.\r\n"
"pushnumber [n] - Pushes [n] on the stack.\r\n"
"pushvalue [n] - Pushes [n] on the stack.\r\n"
"pcall [n] [n] [n] - Runs pcall.\r\n"
"settop [n] - Sets the element stack to [n].\r\n"
"emptystack - Emptys the stack.\r\n"
"Player Commands ---->\r\n"
"* clicktp - Enables ctrl + click teleportation.\r\n"
"* disableclicktp - Disables ctrl + click teleportation.\r\n"
"* ff [p] - Gives [p] a forcefield.\r\n"
"* unff [p] - Removes [p]'s existing forcefield.\r\n"
"* fire [p] - Torches [p] on fire.\r\n"
"* unfire [p] - Removes [p]'s existing fire.\r\n"
"* sparkles [p] - Gives [p] sparkles.\r\n"
"* unsparkles [p] - Removes [p]'s sparkles.\r\n"
"* smoke [p] - Makes [p] smoke.\r\n"
"* unsmoke [p] - Unsmokes [p].\r\n"
"* health [p] [n] - Sets [p]'s health to [n].\r\n"
"* ws [p] [n] - Sets [p]'s walkspeed to [n].\r\n"
"* god [p] - Gods [p].\r\n"
"* ungod [p] - Ungods [p].\r\n"
"* sgod [p] - Semigods [p].\r\n"
"* kill [p] - Kills [p].\r\n"
"* jp [p] [n] - Sets [p]'s jumppower.\r\n"
"* hipheight [p] [n] - Sets [p]'s hipheight to [n].\r\n"
"* explode [p] - Explodes [p].\r\n"
"* removetools [p] - Removes [p]'s tools.\r\n"\
"* ragdoll [p] - Ragdolls [p].\r\n"
"* unragdoll [p] - Unragdolls [p].\r\n"
"* select [p] - Selects [p].\r\n"
"* unselect [p] - Unselects [p].\r\n"
"Misc. Commands ---->\r\n"
"* print [s] - Prints [s] to the console.\r\n"
"* printidentity - Prints the context identity level.\r\n"
"* warn [s] - Prints [s] as a warning message to the console.\r\n"
;







void RunLuaC(std::string Command) {
	std::vector<std::string> In = split(Command);

	if (In.at(0) == "getglobal") {
		lua_getfield(L, -10002, In.at(1).c_str());
	}

	else if (In.at(0) == "getfield") {
		lua_getfield(L, atoi(In.at(1).c_str()), In.at(2).c_str());
	}

	else if (In.at(0) == "setfield") {
		lua_setfield(L, atoi(In.at(1).c_str()), In.at(2).c_str());
	}

	else if (In.at(0) == "setglobal") {
		lua_setfield(L, -10002, In.at(1).c_str());
	}

	else if (In.at(0) == "pushstring") {
		lua_pushstring(L, In.at(1).c_str());
	}

	else if (In.at(0) == "pushvalue") {
		lua_pushvalue(L, atoi(In.at(1).c_str()));
	}

	else if (In.at(0) == "pushnumber") {
		lua_pushnumber(L, atoi(In.at(1).c_str()));
	}

	else if (In.at(0) == "settop") {
		lua_settop(L, atoi(In.at(1).c_str()));
	}

	else if (In.at(0) == "emptystack") {
		lua_emptystack(L);
	}

	else if (In.at(0) == "pcall") {
		DWORD Arg1 = atoi(In.at(1).c_str());
		DWORD Arg2 = atoi(In.at(2).c_str());
		DWORD Arg3 = atoi(In.at(3).c_str());
		lua_pcall(L, Arg1, Arg2, Arg3);
	}
	
	else {

	}
}

void RunCommand(std::string Command) {
	std::vector<std::string> In = split(Command);

	if (In.at(0) == "cmds") {
		printf(Commands);
	}

	

	else if (In.at(0) == "ff") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				lua_getglobal(L, "Instance");
				lua_getfield(L, -1, "new");
				lua_pushstring(L, "ForceField");
				lua_pushvalue(L, -4);
				lua_pcall(L, 2, 0, 0);
				lua_emptystack(L);
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "unff") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "ForceField");
					if (!lua_isnil(L, -1)) {
						lua_getfield(L, -1, "Destroy");
						lua_pushvalue(L, -2);
						lua_pcall(L, 1, 0, 0);
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "fire") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				lua_getfield(L, -1, "Torso");
				if (!lua_isnil(L, -1)) {
					lua_getglobal(L, "Instance");
					lua_getfield(L, -1, "new");
					lua_pushstring(L, "Fire");
					lua_pushvalue(L, -4);
					lua_pcall(L, 2, 0, 0);
					lua_emptystack(L);
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "unfire") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Torso");
					lua_getfield(L, -1, "Fire");
					if (!lua_isnil(L, -1)) {
						lua_getfield(L, -1, "Destroy");
						lua_pushvalue(L, -2);
						lua_pcall(L, 1, 0, 0);
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}


	else if (In.at(0) == "sparkles" || In.at(0) == "sp") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				lua_getfield(L, -1, "Torso");
				if (!lua_isnil(L, -1)) {
					lua_getglobal(L, "Instance");
					lua_getfield(L, -1, "new");
					lua_pushstring(L, "Sparkles");
					lua_pushvalue(L, -4);
					lua_pcall(L, 2, 0, 0);
					lua_emptystack(L);
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "unsparkles" || In.at(0) == "unsp") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Torso");
					lua_getfield(L, -1, "Sparkles");
					if (!lua_isnil(L, -1)) {
						lua_getfield(L, -1, "Destroy");
						lua_pushvalue(L, -2);
						lua_pcall(L, 1, 0, 0);
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "smoke" || In.at(0) == "lit") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				lua_getfield(L, -1, "Torso");
				if (!lua_isnil(L, -1)) {
					lua_getglobal(L, "Instance");
					lua_getfield(L, -1, "new");
					lua_pushstring(L, "Smoke");
					lua_pushvalue(L, -4);
					lua_pcall(L, 2, 0, 0);
					lua_emptystack(L);
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "unsmoke" || In.at(0) == "nosmoke") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Torso");
					lua_getfield(L, -1, "Smoke");
					if (!lua_isnil(L, -1)) {
						lua_getfield(L, -1, "Destroy");
						lua_pushvalue(L, -2);
						lua_pcall(L, 1, 0, 0);
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "god" || In.at(0) == "immortal") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushnumber(L, HUGE_VAL);
						lua_setfield(L, -2, "MaxHealth");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "ungod" || In.at(0) == "unimmortal") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushnumber(L, 100);
						lua_setfield(L, -2, "MaxHealth");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "sgod" || In.at(0) == "silentgod") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushnumber(L, 1000000);
						lua_setfield(L, -2, "MaxHealth");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "kill" || In.at(0) == "die") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushnumber(L, 0);
						lua_setfield(L, -2, "Health");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "health" || In.at(0) == "hp") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			float Num = atoi(In.at(2).c_str());
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushnumber(L, Num);
						lua_setfield(L, -2, "MaxHealth");
						Sleep(10);
						lua_pushnumber(L, Num);
						lua_setfield(L, -2, "Health");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "hipheight" || In.at(0) == "hh") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			float Num = atoi(In.at(2).c_str());
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushnumber(L, Num);
						lua_setfield(L, -2, "HipHeight");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}
	else if (In.at(0) == "jpower" || In.at(0) == "jp") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			float Num = atoi(In.at(2).c_str());
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushnumber(L, Num);
						lua_setfield(L, -2, "JumpPower");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}
	
	else if (In.at(0) == "explode") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			lua_getglobal(L, "Instance");
			lua_getfield(L, -1, "new");
			lua_pushstring(L, "Explosion");
			lua_pushvalue(L, -4);
			lua_pcall(L, 2, 1, 0);

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			lua_getfield(L, -1, Player.c_str());
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, "Torso");
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Position");
					lua_setfield(L, -6, "Position");
					lua_emptystack(L);
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}

		}
	}

	else if (In.at(0) == "removetools") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Players");
			lua_getfield(L, -1, Player.c_str());
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, "Character");
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_getfield(L, -1, "UnequipTools");
						lua_pushvalue(L, -2);
						lua_pcall(L, 1, 1, 0);
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}

			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Players");
			lua_getfield(L, -1, Player.c_str());
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, "Backpack");
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "ClearAllChildren");
					lua_pushvalue(L, -2);
					lua_pcall(L, 1, 0, 0);
					lua_emptystack(L);
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}
	
	else if (In.at(0) == "ragdoll") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushboolean(L, true);
						lua_setfield(L, -2, "PlatformStand");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "unragdoll") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushboolean(L, false);
						lua_setfield(L, -2, "PlatformStand");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "select") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Players");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Character");
					if (!lua_isnil(L, -1)) {
						lua_getfield(L, -1, "Torso");
						if (!lua_isnil(L, -1)) {
							lua_getglobal(L, "Instance");
							lua_getfield(L, -1, "new");
							lua_pushstring(L, "SelectionBox");
							lua_pushvalue(L, -4);
							lua_pcall(L, 2, 1, 0);
							lua_pushvalue(L, -4);
							lua_setfield(L, -2, "Adornee");
							lua_emptystack(L);
						}
						else {
							lua_emptystack(L);
						}
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "unselect") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Players");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Character");
					if (!lua_isnil(L, -1)) {
						lua_getfield(L, -1, "Torso");
						if (!lua_isnil(L, -1)) {
							lua_getfield(L, -1, "SelectionBox");
							if (!lua_isnil(L, -1)) {
								lua_getfield(L, -1, "Destroy");
								lua_pushvalue(L, -2);
								lua_pcall(L, 1, 0, 0);
								lua_emptystack(L);
							}
						}
						else {
							lua_emptystack(L);
						}
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "ws" || In.at(0) == "walkspeed") {
		std::vector<int> Players = GetPlayers(In.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			float Num = atoi(In.at(2).c_str());
			lua_getglobal(L, "game");
			lua_getfield(L, -1, "Workspace");
			if (!lua_isnil(L, -1)) {
				lua_getfield(L, -1, Player.c_str());
				if (!lua_isnil(L, -1)) {
					lua_getfield(L, -1, "Humanoid");
					if (!lua_isnil(L, -1)) {
						lua_pushnumber(L, Num);
						lua_setfield(L, -2, "WalkSpeed");
						lua_emptystack(L);
					}
					else {
						lua_emptystack(L);
					}
				}
				else {
					lua_emptystack(L);
				}
			}
			else {
				lua_emptystack(L);
			}
		}
	}

	else if (In.at(0) == "print") {
		std::string string;
		for (int i = 1; i < In.size(); i++) {
			if (i < (In.size() - 1)) {
				string.append(In.at(i) + ' ');
			}
			if (i == (In.size() - 1)) {
				string.append(In.at(i));
			}
			lua_getglobal(L, "print");
			lua_pushstring(L, string.c_str());
			lua_pcall(L, 1, 0, 0);
			lua_emptystack(L);
		}
	}

	else if (In.at(0) == "warn") {
		std::string string;
		for (int i = 1; i < In.size(); i++) {
			if (i < (In.size() - 1)) {
				string.append(In.at(i) + ' ');
			}
			if (i == (In.size() - 1)) {
				string.append(In.at(i));
			}
			lua_getglobal(L, "warn");
			lua_pushstring(L, string.c_str());
			lua_pcall(L, 1, 0, 0);
			lua_emptystack(L);
		}
	}

	else if (In.at(0) == "printidentity") {
		lua_getglobal(L, "printidentity");
		lua_pcall(L, 0, 0, 0);
		lua_emptystack(L);
	}

	
}