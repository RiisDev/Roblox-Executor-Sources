#ifndef HANDLE_COMMANDS
#define HANDLE_COMMANDS

#pragma once

#include "Includes/SysIncludes.h"
#include "form.h"
#include "clua1.h"
#include "func.h"

using namespace func;
using namespace CLua;

bool FilteringEnabled = false;

void HandleCommand_r(const std::string& command);

const char* Commands = {
	"\nCurrent System48 Commands:\r\n"
	"If you would like help, simply execute the command 'help'\r\n"
	"NOTE: You're able to use plr/all/others/me/guest/random\r\n"
	"Use the command 'clear' (or 'clr'/'cls') to clear out the output\r\n"
	"You can do short variants of the players names too.\r\n\r\n"

	"Player Commands:\r\n"
	"[X] guestchat\r\n"
	"[X] ff [plr]\r\n"
	"[X] unff [plr]\r\n"
	"[X] sparkles/sp [plr]\r\n"
	"[X] unsparkles/unsp [plr]\r\n"
	"[X] fire [plr]\r\n"
	"[X] unfire [plr]\r\n"
	"[X] smoke [plr]\r\n"
	"[X] unsmoke [plr]\r\n"
	"[X] btools [plr]\r\n"
	"[X] rtools [plr]\r\n"
	"[X] bigfire [plr]\r\n"
	"[X] char/charapp [plr] [id]\r\n"
	"[X] ws/walkspeed/speed [plr] [value]\r\n"
	"[X] jp/jumppower/jpower [plr] [value]\r\n" 
	"[X] explode [plr]\r\n"
	"[X] kill [plr]\r\n"
	"[X] respawn [plr]\r\n"
	"[X] tp [plr] [plr2]\r\n"
	"[X] goto/to [plr]\r\n"
	"[X] bring [plr]\r\n"
	"[X] invis [plr]\r\n"
	"[X] vis [plr]\r\n"
	"[X] freeze [plr]\r\n"
	"[X] thaw [plr]\r\n"
	"[X] rtools [plr]\r\n"
	"[X] god [plr]\r\n"
	"[X] sgod [plr]\r\n"
	"[X] ungod [plr]\r\n"
	"[X] unlock [plr]\r\n"
	"[X] lock [plr]\r\n"
	"[X] unlock [plr]\r\n"
	"[X] watch/view [plr]\r\n"
	"[X] unwatch/unview [plr]\r\n"
	"[X] clone [plr]\r\n"
	"[X] firstperson [plr]\r\n"
	"[X] thirdperson [plr]\r\n"
	"[X] shirt [plr] [templateid]\r\n"
	"[X] pants [plr] [templateid]\r\n"
	"[X] bighead [plr]\r\n"
	"[X] peanuthead [plr]\r\n"
	"[X] changestat [plr] [statname] [value]\r\n"
	"[X] guest [plr]\r\n"
	"[X] unguest [plr]\r\n"
	"[X] noob [plr]\r\n"
	"[X] unnoob [plr]\r\n"
	"[X] particles [plr] [decalid]\r\n"
	"[X] duck [plr]\r\n"
	"[X] crash/kick [plr]\r\n"
	"[X] fling [plr]\r\n"
	"[X] char [plr] [userid]\r\n"
	"[X] unchar [plr]\r\n"
	"[X] nojump [plr]\r\n"
	"[X] rejump [plr]\r\n"
	"[X] stun [plr]\r\n"
	"[X] guestchat\r\n"
	"[X] chat [plr] [input]\r\n"
	"[X] punish [plr]\r\n"
	"[X] unpunish [plr]\r\n"
	"[X] rhum [plr] (To get the player back, execute respawn on them)\r\n"
	"[X] light [plr]\r\n"
	"[X] unlight [plr]\r\n\r\n"

	"Server Commands:\r\n"
	"[X] unclebill (complete annihilation!)\r\n"
	"[X] time [value]\r\n"
	"[X] gravity [value]\r\n"
	"[X] music [id]\r\n"
	"[X] daytime\r\n"
	"[X] nighttime\r\n"
	"[X] noon\r\n"
	"[X] sunrise\r\n"
	"[X] stopmusic\r\n"
	"[X] skybox [decalid]\r\n"
	"[X] removeskybox/rskybox\r\n"
	"[X] unanchorws\r\n"
	"[X] unlockws\r\n"
	"[X] fogend [value]\r\n"
	"[X] fogstart [value]\r\n"
	"[X] ambient [value]\r\n"
	"[X] fogcolor [value]\r\n"
	"[X] redfog\r\n"
	"[X] bluefog\r\n"
	"[X] greenfog\r\n"
	"[X] blackfog\r\n"
	"[X] whitefog\r\n\r\n"

	"Game Specific Commands:\r\n"
	"[X] fencingreach\r\n"
	"[X] longfencingreach\r\n"
	"[X] customfencingreach [value]\r\n"
	"[X] recruitplazareach\r\n"
	"[X] drivebloxcash [plr] [value]\r\n"
	"[X] apocstats\r\n\r\n"

	"C Lua Commands:\r\n"
	"[X] getglobal [name]\r\n"
	"[X] getfield [name]\r\n"
	"[X] setfield [name]\r\n"
	"[X] pushnumber [number]\r\n"
	"[X] pcall [nargs] [nresults]\r\n"
	"[X] pushstring [string]\r\n"
	"[X] pushvalue [index]\r\n"
	"[X] clearstack\r\n\r\n"

	"State Commands:\r\n"
	"[X] noclip\r\n"
	"[X] airswim/swim\r\n"
	"[X] jesusfly\r\n"
	"[X] ragdoll (completely useless, but nevertheless fun!)\r\n"
	"[X] disable/clip/unjesusfly/unswim/unragdoll\r\n\r\n"

	"HotKey Commands:\n"
	"These are special hotkey commands you can execute by pressing a key on your keyboard whilst in-game\n"
	"[X] F1 - Quick Respawn\n"
	"[X] F2 - Toggle ForceField\n"
	"[X] F3 - Toggle Sparkles\n"
	"[X] F4 - Toggle Fire\n"
	"[X] F5 - Btools\n"
	"[X] F6 - Bring Others\n"
	"[X] F7 - Toggle Invisibility\n"
	"[X] F8 - UncleBill\n"
	"[X] F10 - Toggle Noclip\n"
};

const char* Help = {
	"\nHeya there, welcome to the nifty 'help' section of System48 V2!\r\n"
	"System48 was designed to be an extremely easy and simple exploit to use.\r\n"
	"This help page will give you a bunch of FAQ's.\r\n\r\n"

	"Q: Wait!? Where's the FilteringEnabled check?\r\n"
	"A: System48 only displays if a game is currently FilteringEnabled. It doesn't tell you if the game is not FE.\r\n"
	"Q: I keep crashing on this one command for some reason? What can I do about it?\r\n"
	"A: It was most likely just a bugged or broken command. Report it to Quad Core or AzuLX via the System48 Discord\r\n"
	"Q: I want to close System48 without closing Roblox, how can I do that?\r\n"
	"A: Simply press the 'X' button on the top of the GUI. It will just remove the console from Roblox\r\n"
	"Q: I have a bunch of command suggestions/overall exploit suggestions. Who can I ask about them?\r\n"
	"A: The System48 development team loves to hear the community's idea's and suggestions, do not hesitate to contact us!\r\n"

	"If you have any further questions, make sure to just contact the main developers for System48!\r\n"
};

std::string validp = "";

std::string getPlayerName() {
	getglobal(State, "game");
	getfield(State, -1, "Players");
	getfield(State, -1, "LocalPlayer");
	getfield(State, -1, "Name");
	validp = tostring(State, -1);
	return validp;
	clearstack(State);
}


void HandleCommand(std::string command) {
	try {
		HandleCommand_r(command);
	}

	catch (std::exception error) {
		sys->Print(RGB(255, 0, 0), "\nException caught during the command, '%s'. This may due to invalid arguments or a bad command call.", command.c_str());
	}

	catch (...) {
		sys->Print(RGB(255, 0, 0), "\r\nAn unexpected error occured when executing '%s'.\nRoblox has a chance of crashing.", command.c_str());
	}
}

#define Start_of_children_off 0x2C
#define End_of_children_off 4

std::vector<INT> GetChildren(DWORD Instance) {
	DWORD StartOfChildren = *(DWORD*)(Instance + Start_of_children_off);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + End_of_children_off);
	std::vector<INT> Children;
	for (INT i = *(INT*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		Children.push_back(*(INT*)i);
	}
	return Children;
}

INT* index2adr(INT rbx_L, INT idx) {
	INT result;
	INT v3;
	const char rbx_luaO_nilobject[] = { 0, 0, 0, 0 };

	if (idx <= 0)
	{
		if (idx <= -10000)
		{
			switch (idx)
			{
			case -10002: // LUA_GLOBALSINDEX
				result = rbx_L + 104;
				break;
			case -10001: // LUA_ENVIRONINDEX
				result = rbx_L + 72;
				*(INT*)result = *(INT*)(**(INT**)(*(INT*)(rbx_L + 12) + 16) + 12);
				*(INT*)(result + 8) = 7;
				break;
			case -10000: // LUA_REGISTRYINDEX
				result = rbx_L + 8 + *(INT*)(rbx_L + 8) + 168;
				break;
			default:
				v3 = **(INT**)(*(INT*)(rbx_L + 12) + 16);
				if (-10002 - idx > *(unsigned char*)(v3 + 7))
					result = (INT)&rbx_luaO_nilobject;
				else
					result = v3 + 16 * (-10002 - idx) + 8;
				break;
			}
		}
		else
		{
			result = *(INT*)(rbx_L + 16) + 16 * idx;
		}
	}
	else
	{
		result = (INT)&rbx_luaO_nilobject;
		if ((unsigned int)(16 * idx + *(INT*)(rbx_L + 28) - 16) < *(INT*)(rbx_L + 16))
			result = 16 * idx + *(INT*)(rbx_L + 28) - 16;
	}
	return (INT*)result;
}

INT touserdata(INT rbx_L, signed int idx) {
	INT result;

	INT* o = index2adr(rbx_L, idx);
	if (o[2] == 1)
	{
		result = *o;
	}
	else if (o[2] == 8)
	{
		result = *o + 24;
	}
	else
	{
		result = NULL;
	}
	return result;
}

bool getFiltering() {
	getglobal(State, "game");
	getfield(State, -1, "Workspace");
	getfield(State, -1, "FilteringEnabled");
	bool isFiltered = toboolean(State, -1);
	if (!isFiltered) {
		//do something???
	}
	else {
		sys->Print(RGB(255, 0, 0), "FilteringEnabled is on!\r\n");
		FilteringEnabled = true;
	}
	return true;
}

int HumanoidStateType = 0;
bool Changing = false;
bool HasTheThreadBeenCreated = false;

void ChangeState() {
	if (Changing) {
		Sleep(10);
	    getfield(State, -10002, "game");
		getfield(State, -1, "GetService");
		pushvalue(State, -2);
		pushstring(State, "Players");
		pcall(State, 2, 1, 0);
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "Character");
		getfield(State, -1, "Humanoid");
		getfield(State, -1, "ChangeState");
		pushvalue(State, -2);
		pushnumber(State, HumanoidStateType);
		pcall(State, 2, 0, 0);
		clearstack(State);
	}
}

void Humanoid_state_loop() {
	while (1) {
		__try {
			ChangeState();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			Sleep(1);
		}
	}
}

void HandleCommand_r(const std::string& command) {
	VMProtectBeginMutation("Sys Function: HandleCommand | Mutation"); /* It doesnt pack it much, just mutates the shit out of the cmds so people cant see the HaK */
	std::vector<std::string> Arguments = SplitString(command, ' ');
	std::string Input = Arguments.at(0);

    sys->Print(RGB(0, 0, 0), "\r\n> %s", command.c_str());

	if (Arguments.at(0) == "cmds" || Arguments.at(0) == "commands") {
		sys->Print(RGB(0, 0, 0), Commands);
	}

	if (Arguments.at(0) == "help" || Arguments.at(0) == "Help") {
		sys->Print(RGB(0, 0, 0), Help);
	}

	if (Arguments.at(0) == "clear" || Arguments.at(0) == "clr" || Arguments.at(0) == "cls") {
		SendMessage(sys->OutputField, WM_SETTEXT, 0, (LPARAM)(""));
		/* Just for aesthetics tbh */
		sys->Print(RGB(0, 0, 200), "Welcome to System48 v%s\n", SYSTEM48_VERSION);
		sys->Print(RGB(0, 0, 0), "\nAuthenticating User... ");
		sys->Print(RGB(0, 0, 200), "Done!\n");
		sys->Print(RGB(0, 0, 0), "Scanning... ");
		sys->Print(RGB(0, 0, 200), "Done!\n");

		if (FilteringEnabled == true) {
			sys->Print(RGB(255, 0, 0), "FilteringEnabled is on!\r\n");
		}

		sys->Print(RGB(0, 0, 0), "System48 successfully initialized!\nInput 'cmds' for a list of commands!\n");

        #ifndef DISABLETRIAL
		sys->Print(RGB(0, 0, 0), "Welcome to the offical System48 trial!");
        #endif
	}

	if (Arguments.at(0) == "ff" || Arguments.at(0) == "Ff") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "ForceField");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unff" || Arguments.at(0) == "Unff") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			
			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "ForceField");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "ff" || Arguments.at(0) == "Ff") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "ForceField");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unff" || Arguments.at(0) == "Unff") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "ForceField");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "FirstPerson" || Arguments.at(0) == "firstperson") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, Player.c_str());
			pushstring(State, "LockFirstPerson");
			setfield(State, -2, "CameraMode");
		}
	}

	if (Arguments.at(0) == "ThirdPerson" || Arguments.at(0) == "thirdperson") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, Player.c_str());
			pushstring(State, "Classic");
			setfield(State, -2, "CameraMode");
		}
	}

	if (Arguments.at(0) == "sparkles" || Arguments.at(0) == "Sparkles" || Arguments.at(0) == "sp" || Arguments.at(0) == "Sp") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Torso");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Sparkles");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unsparkles" || Arguments.at(0) == "Unsparkles" || Arguments.at(0) == "unsp" || Arguments.at(0) == "Unsp") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Torso");
			getfield(State, -1, "Sparkles");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "fire" || Arguments.at(0) == "Fire") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Head");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Fire");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unfire" || Arguments.at(0) == "Unfire") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Head");
			getfield(State, -1, "Fire");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "smoke" || Arguments.at(0) == "Smoke") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Torso");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Smoke");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unsmoke" || Arguments.at(0) == "Unsmoke") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Torso");
			getfield(State, -1, "Smoke");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "btools" || Arguments.at(0) == "Btools") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 1);
			setfield(State, -2, "BinType");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 2);
			setfield(State, -2, "BinType");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 3);
			setfield(State, -2, "BinType");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 4);
			setfield(State, -2, "BinType");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "char" || Arguments.at(0) == "Char" || Arguments.at(0) == "charapp" || Arguments.at(0) == "Charapp") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			std::string wat = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=" + Arguments.at(2);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, Player.c_str());
			pushstring(State, wat.c_str());
			setfield(State, -2, "CharacterAppearance");

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "BreakJoints");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unchar") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "userId");
			pushvalue(State, -3);
			getfield(State, -1, Player.c_str());
			pushvalue(State, -3);
			setfield(State, -2, "CharacterAppearance");
			pushvalue(State, -2);
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "BreakJoints");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unnoob") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "userId");
			pushvalue(State, -3);
			getfield(State, -1, Player.c_str());
			pushvalue(State, -3);
			setfield(State, -2, "CharacterAppearance");
			pushvalue(State, -2);
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "BreakJoints");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}


	if (Arguments.at(0) == "ws" || Arguments.at(0) == "Ws" || Arguments.at(0) == "speed" || Arguments.at(0) == "Speed" || Arguments.at(0) == "walkspeed" || Arguments.at(0) == "Walkspeed") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			float Speed = atof(Arguments.at(2).c_str());
			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Humanoid");
			pushnumber(State, Speed);
			setfield(State, -2, "WalkSpeed");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "jp" || Arguments.at(0) == "Jp" || Arguments.at(0) == "jpower" || Arguments.at(0) == "Jpower" || Arguments.at(0) == "jumppower" || Arguments.at(0) == "Jumppower") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			float Power = atof(Arguments.at(2).c_str());
			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Humanoid");
			pushnumber(State, Power);
			setfield(State, -2, "JumpPower");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "explode" || Arguments.at(0) == "Explode") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Explosion");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Torso");
			getfield(State, -1, "Position");
			setfield(State, -6, "Position");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "fog" || Arguments.at(0) == "Fog" || Arguments.at(0) == "fogend" || Arguments.at(0) == "Fogend") {
		float Value = atoi(Arguments.at(1).c_str());
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushnumber(State, Value);
		setfield(State, -2, "FogEnd");
		clearstack(State);
	}

	if (Arguments.at(0) == "fogstart" || Arguments.at(0) == "Fogstart")
	{
		float Value = atoi(Arguments.at(1).c_str());
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushnumber(State, Value);
		setfield(State, -2, "FogStart");
		clearstack(State);
	}




	if (Arguments.at(0) == "kill" || Arguments.at(0) == "Kill") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "BreakJoints");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "tp" || Arguments.at(0) == "Tp" || Arguments.at(0) == "teleport" || Arguments.at(0) == "Teleport") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		std::vector<int> Players2 = GetPlayers(Arguments.at(2));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);

			for (int i = 0; i < Players2.size(); i++) {
				std::string PlayerName2 = getName(Players2[i]);

				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, PlayerName2.c_str());
				getfield(State, -1, "Character");
				getfield(State, -1, "Torso");
				getfield(State, -1, "CFrame");

				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, PlayerName.c_str());
				getfield(State, -1, "Character");
				getfield(State, -1, "HumanoidRootPart");
				pushvalue(State, -6);
				setfield(State, -2, "CFrame");
			}
		}
	}

	if (Arguments.at(0) == "goto" || Arguments.at(0) == "Goto" || Arguments.at(0) == "to" || Arguments.at(0) == "To") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getfield(State, -1, "CFrame");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "HumanoidRootPart");
			pushvalue(State, -6);
			setfield(State, -2, "CFrame");
		}
	}

	if (Arguments.at(0) == "bring" || Arguments.at(0) == "Bring") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getfield(State, -1, "CFrame");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "HumanoidRootPart");
			pushvalue(State, -6);
			setfield(State, -2, "CFrame");
		}
	}

	if (Arguments.at(0) == "invis" || Arguments.at(0) == "Invis" || Arguments.at(0) == "invisible" || Arguments.at(0) == "Invisible") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "vis" || Arguments.at(0) == "Vis" || Arguments.at(0) == "visible" || Arguments.at(0) == "Visible") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "freeze" || Arguments.at(0) == "Freeze" || Arguments.at(0) == "frz" || Arguments.at(0) == "Frz") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (DWORD a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "thaw" || Arguments.at(0) == "Thaw" || Arguments.at(0) == "unfreeze" || Arguments.at(0) == "Unfreeze" || Arguments.at(0) == "unfrz" || Arguments.at(0) == "Unfrz") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "removetools" || Arguments.at(0) == "Removetools" || Arguments.at(0) == "rtools" || Arguments.at(0) == "Rtools") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");

			getfield(State, -1, "UnequipTools");
			pushvalue(State, -2);
			pcall(State, 1, 1, 0);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Backpack");

			getfield(State, -1, "ClearAllChildren");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "god" || Arguments.at(0) == "God") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, HUGE_VAL);
			setfield(State, -2, "MaxHealth");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "ungod" || Arguments.at(0) == "Ungod") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, 100);
			setfield(State, -2, "MaxHealth");
			pushnumber(State, 100);
			setfield(State, -2, "Health");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "time" || Arguments.at(0) == "Time") {
		float Time = atof(Arguments.at(1).c_str());
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushnumber(State, Time);
		setfield(State, -2, "TimeOfDay");
		clearstack(State);
	}

	if (Arguments.at(0) == "gravity" || Arguments.at(0) == "Gravity") {
		float Gravity = atof(Arguments.at(1).c_str());
		getglobal(State, "game");
		getfield(State, -1, "Workspace");
		pushnumber(State, Gravity);
		setfield(State, -2, "Gravity");
		clearstack(State);
	}

	if (Arguments.at(0) == "rh" || Arguments.at(0) == "Rh" || Arguments.at(0) == "rhum" || Arguments.at(0) == "Rhum") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "hum" || Arguments.at(0) == "humanoid") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "Character");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Humanoid");
			pushvalue(State, -4);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}
	if (Arguments.at(0) == "punish" || Arguments.at(0) == "Punish") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			pushvalue(State, -5);
			setfield(State, -2, "Parent");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unpunish" || Arguments.at(0) == "Unpunish") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "Workspace");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			pushvalue(State, -5);
			setfield(State, -2, "Parent");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "MakeJoints");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "sound" || Arguments.at(0) == "Sound" || Arguments.at(0) == "music" || Arguments.at(0) == "Music") {
		getglobal(State, "game");
		getfield(State, -1, "Workspace");
		getglobal(State, "Instance");
		getfield(State, -1, "new");
		pushstring(State, "Sound");
		pushvalue(State, -4);
		pcall(State, 2, 1, 0);

		pushstring(State, ("rbxassetid://" + Arguments.at(1)).c_str());
		setfield(State, -2, "SoundId");

		pushstring(State, "System48Music");
		setfield(State, -2, "Name");

		pushnumber(State, 1);
		setfield(State, -2, "Volume");

		pushstring(State, "true");
		setfield(State, -2, "Playing");
		pushvalue(State, -2);
		pcall(State, 1, 0, 0);
		clearstack(State);
	}

	if (Arguments.at(0) == "stopsound" || Arguments.at(0) == "Stopsound" || Arguments.at(0) == "stopmusic" || Arguments.at(0) == "Stopmusic" || Arguments.at(0) == "nosound" || Arguments.at(0) == "Nosound" || Arguments.at(0) == "nomusic" || Arguments.at(0) == "Nomusic") {
		getglobal(State, "game");
		getfield(State, -1, "Workspace");
		getfield(State, -1, "System48Music");
		getfield(State, -1, "Destroy");
		pushvalue(State, -2);
		pcall(State, 1, 0, 0);
		clearstack(State);
	}

	if (Arguments.at(0) == "sky" || Arguments.at(0) == "Sky" || Arguments.at(0) == "skybox" || Arguments.at(0) == "Skybox") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		getglobal(State, "Instance");
		getfield(State, -1, "new");
		pushstring(State, "Sky");
		pushvalue(State, -4);
		pcall(State, 2, 1, 0);

		pushstring(State, ("rbxassetid://" + Arguments.at(1)).c_str());
		setfield(State, -2, "SkyboxBk");

		pushstring(State, ("rbxassetid://" + Arguments.at(1)).c_str());
		setfield(State, -2, "SkyboxDn");

		pushstring(State, ("rbxassetid://" + Arguments.at(1)).c_str());
		setfield(State, -2, "SkyboxFt");

		pushstring(State, ("rbxassetid://" + Arguments.at(1)).c_str());
		setfield(State, -2, "SkyboxLf");

		pushstring(State, ("rbxassetid://" + Arguments.at(1)).c_str());
		setfield(State, -2, "SkyboxRt");

		pushstring(State, ("rbxassetid://" + Arguments.at(1)).c_str());
		setfield(State, -2, "SkyboxUp");
		clearstack(State);
	}

	if (Arguments.at(0) == "duck" || Arguments.at(0) == "Duck") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			std::string duck1 = "rbxassetid://9419831";
			std::string duck2 = "rbxassetid://9419827";
			std::string noTshit = "rbxassetid://1";

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Torso");

			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "SpecialMesh");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);

			pushstring(State, duck1.c_str());
			setfield(State, -2, "MeshId");

			pushstring(State, duck2.c_str());
			setfield(State, -2, "TextureId");

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Head");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Head");
			getfield(State, -1, "face");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Left Arm");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Right Arm");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Left Leg");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Right Leg");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
		}
	}


	if (Arguments.at(0) == "crash" || Arguments.at(0) == "Crash" || Arguments.at(0) == "kick") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			call(State, 2, 1);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "SkateboardPlatform");
			pushvalue(State, -4);
			call(State, 2, 1);
			pushboolean(State, true);
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");
			getglobal(State, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "SkateboardPlatform");
			getglobal(State, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "HumanoidRootPart");
			getfield(State, -1, "CFrame");
			setfield(State, -5, "CFrame");
			getglobal(State, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "HumanoidRootPart");
			getfield(State, -1, "Remove");
			pushvalue(State, -2);
			call(State, 1, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unlock" || Arguments.at(0) == "Unlock") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushstring(State, false);
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushstring(State, false);
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushstring(State, false);
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushstring(State, false);
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushstring(State, false);
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushstring(State, false);
			setfield(State, -2, "Locked");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "lock" || Arguments.at(0) == "Lock") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushstring(State, "true");
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushstring(State, "true");
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushstring(State, "true");
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushstring(State, "true");
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushstring(State, "true");
			setfield(State, -2, "Locked");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushstring(State, "true");
			setfield(State, -2, "Locked");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "getglobal") {
		getglobal(State, Arguments.at(1).c_str());
	}

	if (Arguments.at(0) == "getfield") {
		DWORD Index = atof(Arguments.at(1).c_str());
		getfield(State, Index, Arguments.at(2).c_str());
	}

	if (Arguments.at(0) == "pushnumber") {
		double Number = atof(Arguments.at(1).c_str());
		pushnumber(State, Number);
	}

	if (Arguments.at(0) == "setfield") {
		DWORD Index = atof(Arguments.at(1).c_str());
		setfield(State, Index, Arguments.at(2).c_str());
	}

	if (Arguments.at(0) == "pcall") {
		DWORD Args = atof(Arguments.at(1).c_str());
		DWORD Results = atof(Arguments.at(2).c_str());
		pcall(State, Args, Results, 0);
	}

	if (Arguments.at(0) == "pushstring") {
		pushstring(State, Arguments.at(1).c_str());
	}

	if (Arguments.at(0) == "pushvalue") {
		DWORD Index = atof(Arguments.at(1).c_str());
		pushvalue(State, Index);
	}

	if (Arguments.at(0) == "clearstack") {
		settop(State, 0);
	}

	if (Arguments.at(0) == "watch" || Arguments.at(0) == "Watch" || Arguments.at(0) == "view") {
		std::vector<int> Players = GetPlayers2(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Humanoid");

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, "CurrentCamera");
			pushvalue(State, -4);
			setfield(State, -2, "CameraSubject");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unwatch" || Arguments.at(0) == "Unwatch" || Arguments.at(0) == "unview") {
		std::vector<int> Players = GetPlayers2(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, "CurrentCamera");
			pushvalue(State, -4);
			setfield(State, -2, "CameraSubject");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "clone" || Arguments.at(0) == "Clone") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			pushstring(State, "true");
			setfield(State, -2, "Archivable"); Sleep(10);

			getfield(State, -1, "Clone");
			pushvalue(State, -2);
			pcall(State, 1, 1, 0); Sleep(5);

			//pushstring(State, "System48Clone");
			//setfield(State, -2, "Name");

			getglobal(State, "workspace");
			pushvalue(State, -1);
			setfield(State, -3, "Parent"); Sleep(20);
		}
	}

	if (Arguments.at(0) == "particles") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);

			std::string lol = "rbxassetid://" + Arguments.at(2);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");

			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "ParticleEmitter");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);

			pushstring(State, lol.c_str());
			setfield(State, -2, "Texture");

			pushnumber(State, 75);
			setfield(State, -2, "VelocitySpread");

			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "shirt" | Arguments.at(0) == "Shirt") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);
			std::string asg = "http://www.roblox.com/asset/?id=" + Arguments.at(2);
			std::string p = asg;
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Shirt");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Shirt");
			pushstring(State, p.c_str());
			setfield(State, -2, "ShirtTemplate");
			pcall(State, 1, 0, 0);
		}
	}

	if (Arguments.at(0) == "pants" | Arguments.at(0) == "Pants") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);
			std::string asg = "http://www.roblox.com/asset/?id=" + Arguments.at(2);
			std::string p = asg;
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Pants");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Pants");
			pushstring(State, p.c_str());
			setfield(State, -2, "PantsTemplate");
			pcall(State, 1, 0, 0);
		}
	}

	if (Arguments.at(0) == "bighead") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushnumber(State, 0);
			setfield(State, -2, "Shape");
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "change" || Arguments.at(0) == "stats" || Arguments.at(0) == "stat" || Arguments.at(0) == "statchange" || Arguments.at(0) == "changestat") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);

			std::string Stat = Arguments.at(2);
			int Value = atof(Arguments.at(3).c_str());

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "leaderstats");
			getfield(State, -1, Stat.c_str());
			pushnumber(State, Value);
			setfield(State, -2, "Value");
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "bigfire") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Fire");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 30);
			setfield(State, -2, "Size");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unbigfire") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getfield(State, -1, "Fire");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);

		}
	}

	if (Arguments.at(0) == "chat") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int i = 0; i < Players.size(); i++) {
			std::string PlayerName = getName(Players[i]);
			std::string Text = Arguments.at(2);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");

			getglobal(State, "game");
			getfield(State, -1, "Chat");
			getfield(State, -1, "Chat");
			pushvalue(State, -2);
			pushvalue(State, -5);
			pushstring(State, command.substr(Arguments.at(0).length() + Arguments.at(1).length() + 2).c_str());
			pushstring(State, "Blue");
			pcall(State, 4, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "dialog" || Arguments.at(0) == "Dialog") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);
			std::string Text = Arguments.at(2);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Dialog");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
		}
	}

	if (Arguments.at(0) == "unanchorws" || Arguments.at(0) == "Unanchorws") {
		std::vector<int> Wrkspace = getChildren(Workspace);
		for (int c = 0; c < Wrkspace.size(); c++) {
			std::string Children = getName(Wrkspace[c]);

			getglobal(State, "Workspace");
			getfield(State, -1, Children.c_str());
			getfield(State, -1, "IsA");
			pushvalue(State, -2);
			pushstring(State, "BasePart");
			pcall(State, 2, 1, 0);
			if (toboolean(State, -1) == true) {
				pop(State, 1);
				pushnil(State);
				setfield(State, -2, "Anchored");
			}
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unlockws" || Arguments.at(0) == "Unlockws") {
		std::vector<int> Wrkspace = getChildren(Workspace);
		for (int c = 0; c < Wrkspace.size(); c++) {
			std::string Children = getName(Wrkspace[c]);

			getglobal(State, "Workspace");
			getfield(State, -1, Children.c_str());
			getfield(State, -1, "IsA");
			pushvalue(State, -2);
			pushstring(State, "BasePart");
			pcall(State, 2, 1, 0);
			if (toboolean(State, -1) == true) {
				pop(State, 1);
				pushnil(State);
				setfield(State, -2, "Locked");
			}
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "guest" || Arguments.at(0) == "Guest") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			std::string wat = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=1";
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, Player.c_str());
			pushstring(State, wat.c_str());
			setfield(State, -2, "CharacterAppearance");

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "BreakJoints");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "unguest") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "userId");
			pushvalue(State, -3);
			getfield(State, -1, Player.c_str());
			pushvalue(State, -3);
			setfield(State, -2, "CharacterAppearance");
			pushvalue(State, -2);
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "BreakJoints");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "noob" || Arguments.at(0) == "Noob") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string Player = getName(Players[a]);

			std::string wat = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=59977935";
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, Player.c_str());
			pushstring(State, wat.c_str());
			setfield(State, -2, "CharacterAppearance");

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, Player.c_str());
			getfield(State, -1, "BreakJoints");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "fling") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "BodyForce");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			getglobal(State, "Vector3");
			getfield(State, -1, "new");
			pushnumber(State, -9999);
			pushnumber(State, 9999);
			pushnumber(State, -9999);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "force");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "respawn") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "Vector3");
			getfield(State, -1, "new");
			pushnumber(State, 0);
			pushnumber(State, -1000000);
			pushnumber(State, 0);
			pcall(State, 3, 1, 0);
			getglobal(State, "Workspace");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "MoveTo");
			pushvalue(State, -2);
			pushvalue(State, -5);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "baseplate") {
		getglobal(State, "Workspace");
		getglobal(State, "Instance");
		getfield(State, -1, "new");
		pushstring(State, "Part");
		pushvalue(State, -4);
		pcall(State, 2, 1, 0);
		getglobal(State, "Vector3");
		getfield(State, -1, "new");
		pushnumber(State, 1000);
		pushnumber(State, 1);
		pushnumber(State, 1000);
		pcall(State, 3, 1, 0);
		setfield(State, -3, "Size");
		getglobal(State, "Workspace");
		getfield(State, -1, "Part");
		pushstring(State, "System48_Baseplate");
		setfield(State, -2, "Name");
		getglobal(State, "Workspace");
		getfield(State, -1, "System48_Baseplate");
		//pushboolean(State, true);
		setfield(State, -2, "Anchored");
		getglobal(State, "Workspace");
		getfield(State, -1, "System48_Baseplate");
		//pushboolean(State, true);
		setfield(State, -2, "Locked");
		clearstack(State);
	}

	if (Arguments.at(0) == "fogcolor") {
		getglobal(State, "Color3");
		getfield(State, -1, "new");
		pushnumber(State, atof(Arguments.at(1).c_str()));
		pushnumber(State, atof(Arguments.at(2).c_str()));
		pushnumber(State, atof(Arguments.at(3).c_str()));
		pcall(State, 3, 1, 0);
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushvalue(State, -3);
		setfield(State, -2, "FogColor");
		clearstack(State);
	}
	
	if (Arguments.at(0) == "ambient") {
		getglobal(State, "Color3");
		getfield(State, -1, "new");
		pushnumber(State, atof(Arguments.at(1).c_str()));
		pushnumber(State, atof(Arguments.at(2).c_str()));
		pushnumber(State, atof(Arguments.at(3).c_str()));
		pcall(State, 3, 1, 0);
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushvalue(State, -3);
		setfield(State, -2, "Ambient");
		clearstack(State);
	}

	if (Arguments.at(0) == "fencingreach") {
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getfield(State, -1, "Foil");
			getfield(State, -1, "Handle");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "SelectionBox");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushvalue(State, -3);
			setfield(State, -2, "Adornee");
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getfield(State, -1, "Foil");
			getfield(State, -1, "Handle");
			getglobal(State, "Vector3");
			getfield(State, -1, "new");
			pushnumber(State, 1);
			pushnumber(State, 1);
			pushnumber(State, 30);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "Size");
			clearstack(State);
	}

	if (Arguments.at(0) == "recruitplazareach") {
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getfield(State, -1, "_classic");
			getfield(State, -1, "Handle");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "SelectionBox");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushvalue(State, -3);
			setfield(State, -2, "Adornee");
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getfield(State, -1, "_classic");
			getfield(State, -1, "Handle");
			getglobal(State, "Vector3");
			getfield(State, -1, "new");
			pushnumber(State, 1);
			pushnumber(State, 1);
			pushnumber(State, 30);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "Size");
			clearstack(State);
	}

	if (Arguments.at(0) == "longfencingreach") {
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "Backpack");
		getfield(State, -1, "Foil");
		getfield(State, -1, "Handle");
		getglobal(State, "Instance");
		getfield(State, -1, "new");
		pushstring(State, "SelectionBox");
		pushvalue(State, -4);
		pcall(State, 2, 1, 0);
		pushvalue(State, -3);
		setfield(State, -2, "Adornee");
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "Backpack");
		getfield(State, -1, "Foil");
		getfield(State, -1, "Handle");
		getglobal(State, "Vector3");
		getfield(State, -1, "new");
		pushnumber(State, 1);
		pushnumber(State, 1);
		pushnumber(State, 70);
		pcall(State, 3, 1, 0);
		setfield(State, -3, "Size");
		clearstack(State);
	}

	if (Arguments.at(0) == "customfencingreach") {

		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "Backpack");
		getfield(State, -1, "Foil");
		getfield(State, -1, "Handle");
		getglobal(State, "Instance");
		getfield(State, -1, "new");
		pushstring(State, "SelectionBox");
		pushvalue(State, -4);
		pcall(State, 2, 1, 0);
		pushvalue(State, -3);
		setfield(State, -2, "Adornee");
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "Backpack");
		getfield(State, -1, "Foil");
		getfield(State, -1, "Handle");
		getglobal(State, "Vector3");
		getfield(State, -1, "new");
		pushnumber(State, 1);
		pushnumber(State, 1);
		pushnumber(State, atof(Arguments.at(1).c_str()));
		pcall(State, 3, 1, 0);
		setfield(State, -3, "Size");
		clearstack(State);
	}


	if (Arguments.at(0) == "finishmap") {
		std::string Map = Arguments.at(1);

		getglobal(State, "Workspace");
		getfield(State, -1, Map.c_str());
		getfield(State, -1, "MapFinish");
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "Character");
		getfield(State, -1, "Torso");
		getfield(State, -1, "Position");
		setfield(State, -7, "Position");
		clearstack(State);
	}

	if (Arguments.at(0) == "rtools") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);
			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			getfield(State, -1, "UnequipTools");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			pushvalue(State, -4);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Backpack");
			getfield(State, -1, "ClearAllChildren");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "peanuthead") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			getfield(State, -1, "Mesh");
			getglobal(State, "Vector3");
			getfield(State, -1, "new");
			pushnumber(State, 0.55);
			pushnumber(State, 0.55);
			pushnumber(State, 0.55);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "Scale");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "apocstats") {
		getglobal(State, "game");
		getfield(State, -1, "PlaceId");
		double PlaceID = tonumber(State, -1);
		if (PlaceID == 6194809)
		{
			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "playerstats");
			getfield(State, -1, "Hunger");
			pushnumber(State, 100);
			setfield(State, -2, "Value");
			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "playerstats");
			getfield(State, -1, "Thirst");
			pushnumber(State, 100);
			setfield(State, -2, "Value");
			clearstack(State);
		}
		else {
			sys->Print(RGB(255, 0, 0), "\nYou're currently not in Apocalpyse Rising");
		}
	}

	if (Arguments.at(0) == "hidename") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushstring(State, "None");
			setfield(State, -2, "DisplayDistanceType");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "hidename") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushstring(State, "Viewer");
			setfield(State, -2, "DisplayDistanceType");
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "removeskybox" || Arguments.at(0) == "rskybox") {
		std::vector<int> Lightng = getChildren(Lighting);
		for (int c = 0; c < Lightng.size(); c++) {
			std::string Children = getInstanceName(Lightng[c]);

			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			getfield(State, -1, Children.c_str());
			getfield(State, -1, "IsA");
			pushvalue(State, -2);
			pushstring(State, "Sky");
			pcall(State, 2, 1, 0);
			if (toboolean(State, -1) == true) {
				pop(State, 1);
				getfield(State, -1, "Destroy");
				pushvalue(State, -2);
				pcall(State, 1, 0, 0);
			}
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "nojump") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, 0);
			setfield(State, -2, "JumpPower");
			(State);
		}
	}

	if (Arguments.at(0) == "rejump") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, 50);
			setfield(State, -2, "JumpPower");
			clearstack(State);

		}
	}

	if (Arguments.at(0) == "stun") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			//pushboolean(State, true);
			setfield(State, -2, "PlatformStand");
			clearstack(State);

		}
	}

	if (Arguments.at(0) == "removeshirt") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Shirt");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}

	if (Arguments.at(0) == "light") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "SpotLight");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 1000);
			setfield(State, -2, "Brightness");
			pushnumber(State, 30);
			setfield(State, -2, "Range");
			clearstack(State);

		}
	}

	if (Arguments.at(0) == "unlight") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getfield(State, -1, "SpotLight");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);

		}
	}

	if (Arguments.at(0) == "sgod") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, 9999999999);
			setfield(State, -2, "MaxHealth");
			clearstack(State);

		}
	}


	if (Arguments.at(0) == "guestchat") {
		getglobal(State, "game");
		getfield(State, -1, "Chat");
		getfield(State, -1, "ClientChatModules");
		getfield(State, -1, "CommandModules");
		getfield(State, -1, "SwallowGuestChat");
		getfield(State, -1, "Destroy");
		pushvalue(State, -2);
		pcall(State, 1, 1, 0);
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "PlayerGui");
		getfield(State, -1, "Chat");
		getfield(State, -1, "Destroy");
		pushvalue(State, -2);
		pcall(State, 1, 1, 0);
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "PlayerScripts");
		getfield(State, -1, "ChatScript");
		getfield(State, -1, "Clone");
		pushvalue(State, -2);
		pcall(State, 1, 1, 0);
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "PlayerScripts");
		pushvalue(State, -1);
		setfield(State, -6, "Parent");
		clearstack(State);
	}

	if (Arguments.at(0) == "daytime") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushnumber(State, 12);
		setfield(State, -2, "TimeOfDay");
		clearstack(State);
	}

	if (Arguments.at(0) == "nighttime") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushnumber(State, 12);
		setfield(State, -2, "TimeOfDay");
		clearstack(State);
	}

	if (Arguments.at(0) == "noon") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushnumber(State, 4);
		setfield(State, -2, "TimeOfDay");
		clearstack(State);
	}

	if (Arguments.at(0) == "sunrise") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushnumber(State, 6);
		setfield(State, -2, "TimeOfDay");
		clearstack(State);
	}

	if (Arguments.at(0) == "name") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			pushstring(State, Arguments.at(2).c_str());
			setfield(State, -2, "Name");
			clearstack(State);

		}
	}

	if (Arguments.at(0) == "drivebloxcash") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			int Value = atof(Arguments.at(2).c_str());

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Playerdata");
			getfield(State, -1, "Money");
			pushnumber(State, Value);
			setfield(State, -2, "Value");
			clearstack(State);

		}
	}

	if (Arguments.at(0) == "guestchat")
	{
		getglobal(State, "game");
		getfield(State, -1, "Chat");
		getfield(State, -1, "ClientChatModules");
		getfield(State, -1, "CommandModules");
		getfield(State, -1, "SwallowGuestChat");
		getfield(State, -1, "Destroy");
		pushvalue(State, -2);
		pcall(State, 1, 1, 0);
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "PlayerGui");
		getfield(State, -1, "Chat");
		getfield(State, -1, "Destroy");
		pushvalue(State, -2);
		pcall(State, 1, 1, 0);
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "PlayerScripts");
		getfield(State, -1, "ChatScript");
		getfield(State, -1, "Clone");
		pushvalue(State, -2);
		pcall(State, 1, 1, 0);
		getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "PlayerScripts");
		pushvalue(State, -1);
		setfield(State, -6, "Parent");
		clearstack(State);
	}

	if (Arguments.at(0) == "unclebill") {

		getglobal(State, "game");
		getfield(State, -1, "Workspace");
		getglobal(State, "Instance");
		getfield(State, -1, "new");
		pushstring(State, "Sound");
		pushvalue(State, -4);
		pcall(State, 2, 1, 0);
		pushstring(State, ("rbxassetid://666951532"));
		setfield(State, -2, "SoundId");
		pushstring(State, "System48Music");
		setfield(State, -2, "Name");
		pushnumber(State, 1);
		setfield(State, -2, "Volume");

		pushstring(State, "true");
		setfield(State, -2, "Playing");
		pushvalue(State, -2);
		pcall(State, 1, 0, 0);
		clearstack(State);

		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushnumber(State, 40);
		setfield(State, -2, "FogEnd");
		clearstack(State);

		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		getglobal(State, "Color3");
		getfield(State, -1, "new");
		pushnumber(State, 255);
		pushnumber(State, 0);
		pushnumber(State, 0);
		pcall(State, 3, 1, 0);
		setfield(State, -3, "FogColor");
		clearstack(State);

		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		pushnumber(State, 0);
		setfield(State, -2, "TimeOfDay");
		clearstack(State);
	}

	if (Arguments.at(0) == "redfog") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		getglobal(State, "Color3");
		getfield(State, -1, "new");
		pushnumber(State, 255);
		pushnumber(State, 0);
		pushnumber(State, 0);
		pcall(State, 3, 1, 0);
		setfield(State, -3, "FogColor");
		clearstack(State);
	}

	if (Arguments.at(0) == "greenfog") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		getglobal(State, "Color3");
		getfield(State, -1, "new");
		pushnumber(State, 0);
		pushnumber(State, 255);
		pushnumber(State, 0);
		pcall(State, 3, 1, 0);
		setfield(State, -3, "FogColor");
		clearstack(State);
	}

	if (Arguments.at(0) == "bluefog") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		getglobal(State, "Color3");
		getfield(State, -1, "new");
		pushnumber(State, 0);
		pushnumber(State, 0);
		pushnumber(State, 255);
		pcall(State, 3, 1, 0);
		setfield(State, -3, "FogColor");
		clearstack(State);
	}

	if (Arguments.at(0) == "whitefog") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		getglobal(State, "Color3");
		getfield(State, -1, "new");
		pushnumber(State, 255);
		pushnumber(State, 255);
		pushnumber(State, 255);
		pcall(State, 3, 1, 0);
		setfield(State, -3, "FogColor");
		clearstack(State);
	}

	if (Arguments.at(0) == "blackfog") {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		getglobal(State, "Color3");
		getfield(State, -1, "new");
		pushnumber(State, 0);
		pushnumber(State, 0);
		pushnumber(State, 0);
		pcall(State, 3, 1, 0);
		setfield(State, -3, "FogColor");
		clearstack(State);
	}

	if (Arguments.at(0) == "noclip") {
		if (HasTheThreadBeenCreated == false) {
			CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)Humanoid_state_loop, NULL, NULL, NULL);
			HasTheThreadBeenCreated = true;
		}

		HumanoidStateType = 11;
		Changing = true;
	}

	if (Arguments.at(0) == "swim" || Arguments.at(0) == "airswim") {
		if (HasTheThreadBeenCreated == false) {
			CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)Humanoid_state_loop, NULL, NULL, NULL);
			HasTheThreadBeenCreated = true;
		}

		HumanoidStateType = 4;
		Changing = true;
	}

	if (Arguments.at(0) == "jesusfly") {
		if (HasTheThreadBeenCreated == false) {
			CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)Humanoid_state_loop, NULL, NULL, NULL);
			HasTheThreadBeenCreated = true;
		}

		HumanoidStateType = 12;
		Changing = true;
	}

	if (Arguments.at(0) == "ragdoll") {
		if (HasTheThreadBeenCreated == false) {
			CreateThread(NULL, NULL, (PTHREAD_START_ROUTINE)Humanoid_state_loop, NULL, NULL, NULL);
			HasTheThreadBeenCreated = true;
		}

		HumanoidStateType = 1;
		Changing = true;
	}

	if (Arguments.at(0) == "disable" || Arguments.at(0) == "clip" || Arguments.at(0) == "unjesusfly" || Arguments.at(0) == "unfly" || Arguments.at(0) == "unragdoll") {
		Changing = false;
	}

	if (Input == "fes") {
		std::vector<int> Players = GetPlayers(Arguments.at(1));
		for (int a = 0; a < Players.size(); a++) {
			std::string PlayerName = getName(Players[a]);

			getglobal(State, "game");
			getfield(State, -1, "GetService");
			pushvalue(State, -2);
			pushstring(State, "Players");
			pcall(State, 2, 1, 0);
			getfield(State, -1, PlayerName.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			INT userdata = touserdata(State, -1);
			std::vector<INT> Children = GetChildren(*(INT*)(userdata + 4));
			for (INT i = 0; i < Children.size(); i++) {
				std::string Head_children = getName(Children[i]);
				getglobal(State, "game");
				getfield(State, -1, "GetService");
				pushvalue(State, -2);
				pushstring(State, "Players");
				pcall(State, 2, 1, 0);
				getfield(State, -1, PlayerName.c_str());
				getfield(State, -1, "Character");
				getfield(State, -1, "Head");
				getfield(State, -1, Head_children.c_str());
				getfield(State, -1, "IsA");
				pushvalue(State, -2);
				pushstring(State, "Sound");
				pcall(State, 2, 1, 0);
				if (toboolean(State, -1) == true) {
					pop(State, 1);
					pushboolean(State, true);
					setfield(State, -2, "Playing");
				}
				clearstack(State);
			}
		}
	}

	VMProtectEnd();
	}

	
#endif