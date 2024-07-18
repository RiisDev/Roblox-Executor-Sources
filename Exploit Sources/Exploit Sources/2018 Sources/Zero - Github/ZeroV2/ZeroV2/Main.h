#pragma once
#include "RBX.h"
//#include "ReturnCheck.h"
#include "Scan.h"
#include "ZeroLua.h"
//#include "VMProtectSDK.h"
#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <WinINet.h>
#include <algorithm>
#include <conio.h>
#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <WinINet.h>
#include <algorithm>
#include <random>
#include <typeinfo>
#include <random>
#include <typeinfo>
#include <fcntl.h>
#include <io.h>
#include "Color.h"
#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <WinINet.h>
#include <algorithm>
#include <random>
#include <typeinfo>
//#include "Auth.h"

#pragma comment(lib, "wininet") 


#pragma warning (disable: 4996)


class Vector3 {
	float X, Y, Z;
public:
	Vector3(float x, float y, float z) {
		X = x;
		Y = y;
		Z = z;
	}
};

std::string Input() {
	std::string Lithium;
	getline(std::cin, Lithium);
	return Lithium;
}

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

const char* Credits = ""
"Captain Rex - Owner and Developer"
"Deniality - Co-Owner and Developer\r\n"
"bditt - Developer"
"Ice Bear - Developer\r\n"

;


const char *Commands = ""

"printidentity - Prints the context level of this exploit.\r\n"
"print [input] - Prints [input] to the output.\r\n"
"warn [input] - Prints [input] as a warn message to the output.\r\n"
"shutdown - Shutdowns the server.\r\n"
"clearteams - Clears all teams.\r\n"
"clearworkspace - Clears the workspace.\r\n"
"clearterrain - Clears the terrain.\r\n"
"clearlighting - Clears the lighting.\r\n"
"hidename [p] - Hides [p]'s nametag.\r\n"
"showname [p] - Shows [p]'s nametag.\r\n"
"disablereset [p] - Disables [p] from resetting.\r\n"
"allowreset [p] - Allows [p] to reset.\r\n"
"rteam [name] - Removes team [name].\r\n"
"sit [p] - Makes [p] sit.\r\n"
"jump [p] - Makes [p] jump.\r\n"
"naked [p] - Makes [p] naked.\r\n"
"aeffect [p] - Puts effects on [p].\r\n"
"watch [p] - Watches [p].\r\n"
"unwatch - Unwatches the original player, restores cameras to you.\r\n"
"cmds - Show the list of all commands.\r\n"
"credits - Shows the list of the credits.\r\n"
"rescan - Rescans ROBLOX.\r\n"
"punish [p] - Punishes [p].\r\n"
"unpunish [p] - Unpunishes [p].\r\n"
"fencing - Makes your fencing foil bigger!!\r\n"
"hipheight [p] [n] - Sets [p]'s hipheight to [n].\r\n"
"apocbadges [p] - Gives [p] all apoc's badges!!\r\n"
"jpower [p] [n] - Sets [p]'s jumppower to [n].\r\n"
"nojump [p] - Disallows [p] from jumping.\r\n"
"ws [p] [n] - Sets [p]'s speed to [n].\r\n"
"speedy - Makes you fast & speedy. ;c\r\n"
"freecam [p] - Makes [p] spectate ;u\r\n"
"health [p] [n] - Sets [p]'s health to [n].\r\n"
"heal [p] - Heals [p].\r\n"
"kill [p] - Forcefully kills [p].\r\n"
"breakjoints - Kills everyone.\r\n"
"suicide - Forces you to suicide.\r\n"
"kick [p] - Forcefully kicks [p].\r\n"
"kickplrwmsg [p] [msg] - Kicks [p] with a message of [msg].\r\n"
"shutdownplr [p] - Shuts down [p]'s game.\r\n"
"god [p] - Forcefully makes [p] godded.\r\n"
"ungod [p] - Ungods [p].\r\n"
"sgod [p] - Semigods [p].\r\n"
"explode [p] - Blows up [p].\r\n"
"unff [p] - Removes [p]'s forcefield.\r\n"
"ff [p] - Gives [p] a forcefield.\r\n"
"newtool [p] - Gives [p] a tool.\r\n"
"newteam [name] - Creates a new team called [name].\r\n"
"particle [p] [n] - Gives [p] a particle and sets the texture to [n].\r\n"
"broccoliobama - Displays broccoli obama everywhere.\r\n"
"trump - Displays trump everywhere.\r\n"
"hillary - Displays hillary everywhere.\r\n"
"berniesanders = Displays bernie sanders everywhere.\r\n"
"mountaindew - Displays mountain dew everywhere!!\r\n"
"doritos - Displays doritos everywhere!!\r\n"
"c00lkid - Displays c00lkid official logo everywhere xD.\r\n"
"removeface [p] - Removes [p]'s face.\r\n"
"removeparticle [p] - Removes [p]'s particle.\r\n"
"newtool [p] - Gives [p] a tool.\r\n"
"unsmoke [p] - Removes [p]'s smoke.\r\n"
"smoke [p] - Sets [p] on smoke.\r\n"
"unsparkles [p] - Unsparkles [p].\r\n"
"sparkles [p] - Sets [p] on sparkles.\r\n"
"unfire [p] - Removes [p]'s fire.\r\n"
"fire [p] - Sets [p] on fire.\r\n"
"bigfire [p] - Spawns a large fire on [p].\r\n"
"freeze [p] - Freezes [p] in [p]'s tracks.\r\n"
"thaw [p] - Thaws [p].\r\n"
"light [p] - Lights up [p].\r\n"
"unlight [p] - UnLights [p].\r\n"
"pepe - Sets the sky to show pepe.\r\n"
"skybox [n] - Sets the lighting to show [n].\r\n"
"rskybox - Removes the current skybox showing.\r\n"
"fogend [n] - Sets the fogend to [n].\r\n"
"fogstart [n] - Sets the fogstart to [n].\r\n"
"nofog - Clears the fog.\r\n"
"foggy - Makes the server foggy.\r\n"
"sunpos [n] - Sets the sun pos to [n].\r\n"
"normalsunpos - Sets the sunpos back to normal.\r\n"
"setshirt [p] [id] - Sets [p]'s shirt's apperance to [id].\r\n"
"setpants [p] [id] - Sets [p]'s pants apperance to [id].\r\n"
"time [n] - Sets the time to [n].\r\n"
"day - Sets the time to day.\r\n"
"night - Sets the time to night.\r\n"
"dawn - Sets time to dawn.\r\n"
"changestat [p] [stat] [n] - Sets [p]'s [stat] to [n].\r\n"
"fling [p] - Flings [p] back to spawn.\r\n"
"setblurval [n] - Sets blur's value to [n] [must used blur cmd].\r\n"
"blur - Makes the lighting blur.\r\n"
"unblur - Makes the lighting not blur.\r\n"
"bloom - Makes the lighting bloom.\r\n"
"unbloom - Unblooms the lighting.\r\n"
"spin [p] - Makes [p] spin slowly.\r\n"
"unspin [p] - Makes [p] not spin anymore.\r\n"
"weirdify [p] - Makes [p] weird111!\r\n"
"dialog [p] - Make [p] have a dialog!\r\n"
"nodialog [p] - Removes [p]'s dialog.\r\n"
"newspawn - Creates a new spawnlocation.\r\n"
"removetools [p] - Removes [p]'s tools.\r\n"
"btools [p] - Gives [p] building tools.\r\n"
"charapp [p] [n] - Sets [p]'s character apperance to [n].\r\n"
"music [n] - Plays [n].\r\n"
"ohbabyatriple - Plays preloaded song ;c!11\r\n"
"purge - Plays preloaded song and starts the purge!!\r\n"
"clubpenguinearrape - Plays preloaded song. ;c\r\n"
"shutthefuckup - Plays preloaded song. ;u\r\n"
"rickroll - Plays preloaded song of rickroll :D\r\n"
"rickrollrape - Plays preloaded song of rickroll but loader 50x!!\r\n"
"allahakbar - Plays preloaded song of terrorists!!\r\n"
"allahakbartrap - Plays preloaded trap of terrorists!!\r\n"
"volume [n] - Sets the current playing sound volume to [n].\r\n"
"pitch [n] - Sets the current playing sound pitch to [n].\r\n"
"stopmusic - Stops playing the current sound.\r\n"
"fov [n] - Sets your field of view to [n].\r\n"
"tp [p1] [p2] - Teleports [p1] to [p2].\r\n"
"goto [p] - Forcefully goes to [p].\r\n"
"bring [p] - Brings [p] to you.\r\n"
"gravity [n] - Sets the gravity to [n].\r\n"
"normalgravity - Sets the gravity back to normal.\r\n"
"fogcolor [n] [n] [n] - Sets the fogcolor to [n].\r\n"
"ambient [n] [n] [n] - Set the ambient to [n].\r\n"
"watercolor [n] [n] [n] - Sets the watercolor to [n].\r\n"
"brightness [n] - Sets the brightness to [n].\r\n"
"dark - Sets the server dark.\r\n"
"bright - Sets the server bright.\r\n"
"removearms [p] - Removes [p]'s arms.\r\n"
"removelegs [p] - Removes [p]'s legs.\r\n"
"rhumanoid [p] - Removes [p]'s humanoid.\r\n"
"ghumanoid [p] - Gives [p] a humanoid.\r\n"
"cylhead [p] - Makes [p] have a cylinderhead.\r\n"
"blockhead [p] - Makes [p] have a blockhead.\r\n"
"peahead [p] - Makes [p] have a peahead.\r\n"
"bighead [p] - Makes [p]'s head big.\r\n"
"giant [p] - Makes [p] giant size.\r\n"
"normalsizehead [p] - Makes [p]'s head size to normal.\r\n"
"normalhead [p] - Fixes [p]'s head.\r\n"
"smallhead [p] - Sets [p]'s head to small.\r\n"
"gotospawn [p] - ForceFully makes [p] go back to spawn.\r\n"
"disableregen [p] - Disables health regeneration for [p].\r\n"
"enableregen [p] - Enables health regeneration for [p].\r\n"
"disableanimate [p] - Disables animation for [p].\r\n"
"enableanimate [p] - Enables animation for [p].\r\n"
"outlines on - Enables the outlines for lighting.\r\n"
"outlines off - Disables the outlines for lighting.\r\n"
"enableturns [p] - Enables auto rotating turning for [p].\r\n"
"disableturns [p] - Disables auto rotating turning for [p].\r\n"
"lock [p] - Locks [p] denying plrs from using btools on [p].\r\n"
"unlock [p] - Unlocks [p] allowing pls to use btools on [p].\r\n"
"invishead [p] - Makes [p]'s head invisible.\r\n"
"ghosthead [p] - Makes [p]'s head look like a ghost's head.\r\n"
"vishead [p] - Makes [p]'s head visible.\r\n"
"invis [p] - Makes [p] invisible.\r\n"
"vis [p] - Makes [p] visible.\r\n"
"nohead [p] - Deletes [p]'s head.\r\n"
"ghost [p] - Sets [p] as a ghost whoooo.\r\n"
"ragdoll [p] - Makes [p] ragdolled into the ground.\r\n"
"unragdoll [p] - Unragdolls [p].\r\n"
"duck [p] - Format's [p] to a duck.\r\n"
"unchar [p] - Unchars [p].\r\n"
"rechar [p] - Rechars [p].\r\n"
"guest [p] - Makes [p] a guest.\r\n"
"select [p] - Selects [p], has a box around [p].\r\n"
"unselect [p] - UnSelects [p], removes the box around [p].\r\n"
"crash [p] - Crashes [p].\r\n"
"name [p] [string] - Names [p] [string]. [p] for me doesn't work.\r\n"
"cameralock [p] - Locks [p]'s camera in first person.\r\n"
"camerahalflock [p] - Locks [p]'s camera half way.\r\n"
"cameraunlock [p] - Unlocks [p]'s camera so they can zoom out/in.\r\n"
"wavespeed [n] - Sets the water's wave speed to [n].\r\n"
"destroy [i] [c] - For example if [i] was workspace and [c] was Part, it would call workspace and destroy Part.\r\n"
"clone [p] - Clones [p].\r\n"
"cls - Clears the console.\r\n"
"exit - Exits the console & ROBLOX.\r\n"
;



bool hotkeybools = false;
bool ChatHook = false;
bool Changing = false;

#include <thread>

void ChangeState(int Location) {
	if (Changing == false) {
		State = Location;
		Changing = true;
	}
	else if (State != Location) {
		State = Location;
		Changing = true;
	}
	else {
		Changing = false;
	}
}

void SetLocalHumanoidState(int State) {
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, "LocalPlayer");
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "Humanoid");
	lua_getfield(lua_State, -1, "ChangeState");
	lua_pushvalue(lua_State, -2);
	lua_pushnumber(lua_State, double(State));
	lua_pcall(lua_State, 2, 0, 0);
	lua_getfield(lua_State, -1, "SetStateEnabled");
	lua_pushvalue(lua_State, -3);
	lua_pushnumber(lua_State, double(State));
	lua_pushnumber(lua_State, 1);
	lua_pcall(lua_State, 3, 0, 0);
}

void doState() {
	if (Changing == true) {
		int LocalCharacter = GetCharacterFromPlayer(GetLocalPlayer(Players));
		if (LocalCharacter != 0) {
			int Humanoid = FindFirstClass(LocalCharacter, "Humanoid");
			int HumanoidRootPart = FindFirstChild_PartOf(LocalCharacter, "HumanoidRootPart");
			if (Humanoid > 100000 && HumanoidRootPart > 100000) {
				int Weld = FindFirstClass(HumanoidRootPart, "Motor6D");
				if (Weld > 100000) {
					SetLocalHumanoidState(State);
				}
			}
		}
	}
}


void Loop() {
	while (true) {
		Sleep(45);
		int Workspace = FindFirstClass(DataModel, "Workspace");
		if (Workspace > 0) {
			if (Changing == true) {
				try {
					std::thread run(&doState);
					run.detach();

				}
				catch (std::exception) {
					Sleep(5);
				}
			}
		}
		else {
			printf("\n Detected that you have been teleported to another game.\n"); Sleep(2500);
		}
	}
}



std::string Error = "There was a error between the exploit and the player.\r\n";

void Scan() {
	//VMProtectBeginUltra("Scanner");
	std::cout << "Scanning.. ";
	try {
		//DWORD DataModelA = getaddy(0x6d6f4e);
		DWORD ScriptContextVFTable = *(DWORD*)((getaddy(0x6b760e)) + 0x02);
		ScriptContext = Memory::Scan((char*)&ScriptContextVFTable);
		DataModel = GetParent(ScriptContext);
		Workspace = FindFirstClass(DataModel, "Workspace");
		Lighting = FindFirstClass(DataModel, "Lighting");
		Players = FindFirstClass(DataModel, "Players");
		CurrentPlayer = GetLocalPlayer(Players);
		PlayerName = GetName(CurrentPlayer);
		lua_State = *(DWORD *)(ScriptContext + 0xDC) - (ScriptContext + 0xDC);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Loop, NULL, NULL, NULL);

		std::cout << blue << "Success." << std::endl;
	}
	catch (std::exception) {
		std::cout << red << "Unsuccess." << std::endl;
		exit(0);
	}
	//VMProtectEnd();
}

void DoConsoleCommands(std::string Cmd) {
	//VMProtectBeginUltra("ConsoleCommands");
	std::vector<std::string> In = split(Cmd);

	if (In.at(0) == "cmds") {
		std::cout << blue << Commands << white << std::endl;
	}
	else if (In.at(0) == "cls") {
		system("cls");
	}

	else if (In.at(0) == "rescan") {
		Scan();
	}

	else if (In.at(0) == "credits") {
		std::cout << Credits << std::endl;
	}
	//VMProtectEnd();
}

void luaC(std::string input) {
	try {
		std::vector<std::string> args = split(input);

		if (args.at(0) == "getfield") {
			lua_getfield(lua_State, stoi(args.at(1)), args.at(2).c_str());
		}

		else if (args.at(0) == "getglobal") {
			lua_getglobal(lua_State, args.at(1).c_str());
		}

		else if (args.at(0) == "setfield") {
			lua_setfield(lua_State, stoi(args.at(1)), args.at(2).c_str());
		}

		else if (args.at(0) == "pushvalue") {
			lua_pushvalue(lua_State, stoi(args.at(1)));
		}

		else if (args.at(0) == "pushstring") {
			lua_pushstring(lua_State, args.at(1).c_str());
		}

		else if (args.at(0) == "pushnumber") {
			lua_pushnumber(lua_State, stoi(args.at(1)));
		}

		else if (args.at(0) == "pushbool") {
			lua_pushnumber(lua_State, stoi(args.at(1)));
		}

		else if (args.at(0) == "pcall") {
			lua_pcall(lua_State, stoi(args.at(1)), stoi(args.at(2)), stoi(args.at(3)));

		}

		else if (args.at(0) == "emptystack") {
			lua_settop(lua_State, 0);
		}
	}
	catch (...) {

	}
}

void DoLuaCommand3(std::string Cmd) {
	std::vector<std::string> In = split(Cmd);

	if (In.at(0) == "kick") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushnumber(lua_State, HUGE * 0);
				lua_setfield(lua_State, -2, "HipHeight");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushnumber(lua_State, HUGE * 0);
				lua_setfield(lua_State, -2, "HipHeight");
			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "noclip") {
		ChangeState(11);
	}


	else if (In.at(0) == "guest") {
		try {
			if (In.at(1) == "me") {
				ChangeChar("LocalPlayer", "1");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				ChangeChar(PlayerName, "1");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "guestchat") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Chat");
			lua_getfield(lua_State, -1, "ClientChatModules");
			lua_getfield(lua_State, -1, "CommandModules");
			lua_getfield(lua_State, -1, "SwallowGuestChat");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 1, 0);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "PlayerGui");
			lua_getfield(lua_State, -1, "Chat");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 1, 0);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "PlayerScripts");
			lua_getfield(lua_State, -1, "ChatScript");
			lua_getfield(lua_State, -1, "Clone");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 1, 0);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "PlayerScripts");
			lua_pushvalue(lua_State, -1);
			lua_setfield(lua_State, -6, "Parent");
			Sleep(150);
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "bigfire") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");

				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "Fire");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);

				lua_pushstring(lua_State, "30");
				lua_setfield(lua_State, -2, "Size");
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");

				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "Fire");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);

				lua_pushstring(lua_State, "30");
				lua_setfield(lua_State, -2, "Size");
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "hotkeys") {
		hotkeybools = true;
	}

	else if (In.at(0) == "watch") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, "CurrentCamera");
				lua_pushvalue(lua_State, -4);
				lua_setfield(lua_State, -2, "CameraSubject");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, "CurrentCamera");
				lua_pushvalue(lua_State, -4);
				lua_setfield(lua_State, -2, "CameraSubject");
			}
		}
		catch (std::exception) {

		}
	}


	else if (In.at(0) == "unwatch") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Humanoid");

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, "CurrentCamera");
			lua_pushvalue(lua_State, -4);
			lua_setfield(lua_State, -2, "CameraSubject");
		}
		catch (std::exception) {

		}
	}
	else if (In.at(0) == "respawn") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "LoadCharacter");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "LoadCharacter");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "duck") {
		if (In.at(1) == "me") {
			string asset1 = "rbxassetid://9419831";
			string asset2 = "rbxassetid://9419827";

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Torso");

			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "SpecialMesh");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, asset1.c_str());
			lua_setfield(lua_State, -2, "MeshId");

			lua_pushstring(lua_State, asset2.c_str());
			lua_setfield(lua_State, -2, "TextureId");

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Head");
			lua_pushnumber(lua_State, 1);
			lua_setfield(lua_State, -2, "Transparency");

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Head");
			lua_getfield(lua_State, -1, "face");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Left Arm");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Right Arm");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Left Leg");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Right Leg");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Torso");
			lua_getfield(lua_State, -1, "Mesh");
			lua_getglobal(lua_State, "Vector3");
			lua_getfield(lua_State, -1, "new");
			lua_pushnumber(lua_State, 6);
			lua_pushnumber(lua_State, 6);
			lua_pushnumber(lua_State, 6);
			lua_pcall(lua_State, 3, 1, 0);
		}
		else {
			int Character = GetCharacter(In.at(1));
			std::string plr = *GetName(Character);
			string asset1 = "rbxassetid://9419831";
			string asset2 = "rbxassetid://9419827";

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, plr.c_str());
			lua_getfield(lua_State, -1, "Torso");

			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "SpecialMesh");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, asset1.c_str());
			lua_setfield(lua_State, -2, "MeshId");

			lua_pushstring(lua_State, asset2.c_str());
			lua_setfield(lua_State, -2, "TextureId");

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, plr.c_str());
			lua_getfield(lua_State, -1, "Head");
			lua_pushnumber(lua_State, 1);
			lua_setfield(lua_State, -2, "Transparency");

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, plr.c_str());
			lua_getfield(lua_State, -1, "Head");
			lua_getfield(lua_State, -1, "face");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, plr.c_str());
			lua_getfield(lua_State, -1, "Left Arm");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, plr.c_str());
			lua_getfield(lua_State, -1, "Right Arm");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, plr.c_str());
			lua_getfield(lua_State, -1, "Left Leg");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, plr.c_str());
			lua_getfield(lua_State, -1, "Right Leg");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, plr.c_str());
			lua_getfield(lua_State, -1, "Torso");
			lua_getfield(lua_State, -1, "Mesh");
			lua_getglobal(lua_State, "Vector3");
			lua_getfield(lua_State, -1, "new");
			lua_pushnumber(lua_State, 6);
			lua_pushnumber(lua_State, 6);
			lua_pushnumber(lua_State, 6);
			lua_pcall(lua_State, 3, 1, 0);
		}
	}


	else if (In.at(0) == "kickplrwmsg") {
		try {
			if (In.at(1) == "me") {
				string string;
				for (size_t i = 1; i < In.size(); i++) {
					if (i < (In.size() - 2)) {
						string.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 2)) {
						string.append(In.at(i));
					}
				}
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Kick");
				lua_pushvalue(lua_State, -2);
				lua_pushstring(lua_State, string.c_str());
				lua_pcall(lua_State, 2, 0, 0);
			}
			else {

				int Character = GetCharacter(In.at(1));
				string string;
				for (size_t i = 1; i < In.size(); i++) {
					if (i < (In.size() - 2)) {
						string.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 2)) {
						string.append(In.at(i));
					}
				}
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Kick");
				lua_pushvalue(lua_State, -2);
				lua_pushstring(lua_State, string.c_str());
				lua_pcall(lua_State, 2, 0, 0);
			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "shutdownplr") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Kick");
				lua_pushvalue(lua_State, -2);
				lua_pushstring(lua_State, "This game has shutdown");
				lua_pcall(lua_State, 2, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Kick");
				lua_pushvalue(lua_State, -2);
				lua_pushstring(lua_State, "This game has shutdown");
				lua_pcall(lua_State, 2, 0, 0);
			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "setshirt") {
		try {
			if (In.at(1) == "me") {
				std::string Shirt = "http://www.roblox.com/asset/?id=" + In.at(2);
				string LocalPlayerName = *PlayerName;

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, LocalPlayerName.c_str());
				lua_getfield(lua_State, -1, "Shirt");
				lua_pushstring(lua_State, Shirt.c_str());
				lua_setfield(lua_State, -2, "ShirtTemplate");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);

				std::string Shirt = "http://www.roblox.com/asset/?id=" + In.at(2);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_pushstring(lua_State, Shirt.c_str());
				lua_setfield(lua_State, -2, "ShirtTemplate");

			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "setpants") {
		try {
			if (In.at(1) == "me") {
				std::string Shirt = "http://www.roblox.com/asset/?id=" + In.at(2);
				string LocalPlayerName = *PlayerName;

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, LocalPlayerName.c_str());
				lua_getfield(lua_State, -1, "Pants");
				lua_pushstring(lua_State, Shirt.c_str());
				lua_setfield(lua_State, -2, "PantsTemplate");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);

				std::string Pants = "http://www.roblox.com/asset/?id=" + In.at(2);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Pants");
				lua_pushstring(lua_State, Pants.c_str());
				lua_setfield(lua_State, -2, "PantsTemplate");

			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "rshirt") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Shirt");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Shirt");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {

		}
	}
	lua_emptystack(lua_State);

}

void DoLuaCommand2(std::string Cmd) {
	std::vector<std::string> In = split(Cmd);

	if (In.at(0) == "trump") {
		try {

			std::string Bk = "http://www.roblox.com/asset/?id=295682046";
			std::string Dn = "http://www.roblox.com/asset/?id=295682046";
			std::string Ft = "http://www.roblox.com/asset/?id=295682046";
			std::string Lf = "http://www.roblox.com/asset/?id=295682046";
			std::string Rt = "http://www.roblox.com/asset/?id=295682046";
			std::string Up = "http://www.roblox.com/asset/?id=295682046";
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "Sky");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, Bk.c_str());
			lua_setfield(lua_State, -2, "SkyboxBk");

			lua_pushstring(lua_State, Dn.c_str());
			lua_setfield(lua_State, -2, "SkyboxDn");

			lua_pushstring(lua_State, Ft.c_str());
			lua_setfield(lua_State, -2, "SkyboxFt");

			lua_pushstring(lua_State, Lf.c_str());
			lua_setfield(lua_State, -2, "SkyboxLf");

			lua_pushstring(lua_State, Rt.c_str());
			lua_setfield(lua_State, -2, "SkyboxRt");

			lua_pushstring(lua_State, Up.c_str());
			lua_setfield(lua_State, -2, "SkyboxUp");

			lua_pushstring(lua_State, "Sky");
			lua_setfield(lua_State, -2, "Name");

			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "bighead") {
		try {
			if (In.size() == 2) {
				if (In.at(1) == "me") {
					lua_getglobal(lua_State, "game");
					lua_getfield(lua_State, -1, "Players");
					lua_getfield(lua_State, -1, "LocalPlayer");
					lua_getfield(lua_State, -1, "Character");
					lua_getfield(lua_State, -1, "Head");
					lua_getfield(lua_State, -1, "Mesh");
					lua_getglobal(lua_State, "Vector3");
					lua_getfield(lua_State, -1, "new");
					lua_pushstring(lua_State, "3");
					lua_pushstring(lua_State, "3");
					lua_pushstring(lua_State, "3");
					lua_pcall(lua_State, 3, 1, 0);
					lua_setfield(lua_State, -3, "Scale");
				}
				else {
					int Character = GetCharacter(In.at(1));
					std::string PlayerName = *GetName(Character);
					lua_getglobal(lua_State, "game");
					lua_getfield(lua_State, -1, "Players");
					lua_getfield(lua_State, -1, PlayerName.c_str());
					lua_getfield(lua_State, -1, "Character");
					lua_getfield(lua_State, -1, "Head");
					lua_getfield(lua_State, -1, "Mesh");
					lua_getglobal(lua_State, "Vector3");
					lua_getfield(lua_State, -1, "new");
					lua_pushstring(lua_State, "3");
					lua_pushstring(lua_State, "3");
					lua_pushstring(lua_State, "3");
					lua_pcall(lua_State, 3, 1, 0);
					lua_setfield(lua_State, -3, "Scale");
				}
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "smallhead") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getfield(lua_State, -1, "Mesh");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "0.75");
				lua_pushstring(lua_State, "0.75");
				lua_pushstring(lua_State, "0.75");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Scale");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getfield(lua_State, -1, "Mesh");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "0.75");
				lua_pushstring(lua_State, "0.75");
				lua_pushstring(lua_State, "0.75");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Scale");
			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "punish") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Lighting");
				lua_setfield(lua_State, -3, "Parent");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Lighting");
				lua_setfield(lua_State, -3, "Parent");
			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "unpunish") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Lighting");
				lua_getfield(lua_State, -1, (const char*)PlayerName);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_setfield(lua_State, -3, "Parent");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Lighting");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_setfield(lua_State, -3, "Parent");
			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "giant") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "HumanoidRootPart");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "HumanoidRootPart");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "3");
				lua_pushstring(lua_State, "8");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "normalsizehead") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "2");
				lua_pushstring(lua_State, "1");
				lua_pushstring(lua_State, "1");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getglobal(lua_State, "Vector3");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "2");
				lua_pushstring(lua_State, "1");
				lua_pushstring(lua_State, "1");
				lua_pcall(lua_State, 3, 1, 0);
				lua_setfield(lua_State, -3, "Size");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "c00lkid") {
		try {

			std::string Bk = "http://www.roblox.com/asset/?id=179832364";
			std::string Dn = "http://www.roblox.com/asset/?id=179832364";
			std::string Ft = "http://www.roblox.com/asset/?id=179832364";
			std::string Lf = "http://www.roblox.com/asset/?id=179832364";
			std::string Rt = "http://www.roblox.com/asset/?id=179832364";
			std::string Up = "http://www.roblox.com/asset/?id=179832364";
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "Sky");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, Bk.c_str());
			lua_setfield(lua_State, -2, "SkyboxBk");

			lua_pushstring(lua_State, Dn.c_str());
			lua_setfield(lua_State, -2, "SkyboxDn");

			lua_pushstring(lua_State, Ft.c_str());
			lua_setfield(lua_State, -2, "SkyboxFt");

			lua_pushstring(lua_State, Lf.c_str());
			lua_setfield(lua_State, -2, "SkyboxLf");

			lua_pushstring(lua_State, Rt.c_str());
			lua_setfield(lua_State, -2, "SkyboxRt");

			lua_pushstring(lua_State, Up.c_str());
			lua_setfield(lua_State, -2, "SkyboxUp");

			lua_pushstring(lua_State, "Sky");
			lua_setfield(lua_State, -2, "Name");

			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "rhumanoid") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "fencing") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Backpack");
			lua_getfield(lua_State, -1, "Foil");
			lua_getfield(lua_State, -1, "Handle");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "SelectionBox");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Backpack");
			lua_getfield(lua_State, -1, "Foil");
			lua_getfield(lua_State, -1, "Handle");
			lua_setfield(lua_State, -7, "Adornee");
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Backpack");
			lua_getfield(lua_State, -1, "Foil");
			lua_getfield(lua_State, -1, "Handle");
			lua_pushstring(lua_State, "false");
			lua_setfield(lua_State, -2, "CanCollide");
			lua_getglobal(lua_State, "Vector3");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "1");
			lua_pushstring(lua_State, "1");
			lua_pushstring(lua_State, "30");
			lua_pcall(lua_State, 3, 1, 0);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Backpack");
			lua_getfield(lua_State, -1, "Foil");
			lua_getfield(lua_State, -1, "Handle");
			lua_pushvalue(lua_State, -8);
			lua_setfield(lua_State, -2, "Size");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "crash") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushnumber(lua_State, HUGE * 0);
				lua_setfield(lua_State, -2, "HipHeight");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushnumber(lua_State, HUGE * 0);
				lua_setfield(lua_State, -2, "HipHeight");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "chat") {
		try {
			if (In.at(1) == "me") {
				std::string Message;
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "GetService");
				lua_pushvalue(lua_State, -2);
				lua_pushstring(lua_State, "Players");
				lua_pcall(lua_State, 2, 1, 0);
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getglobal(lua_State, "Chat");
				lua_getfield(lua_State, -1, "Chat");
				lua_pushvalue(lua_State, -2);
				lua_pushvalue(lua_State, -5);
				lua_pushstring(lua_State, In.at(2).c_str());
				lua_pushstring(lua_State, "Blue");
				lua_pcall(lua_State, 4, 0, 0);
			}
			else {
				std::string Message;
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "GetService");
				lua_pushvalue(lua_State, -2);
				lua_pushstring(lua_State, "Players");
				lua_pcall(lua_State, 2, 1, 0);
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getglobal(lua_State, "Chat");
				lua_getfield(lua_State, -1, "Chat");
				lua_pushvalue(lua_State, -2);
				lua_pushvalue(lua_State, -5);
				lua_pushstring(lua_State, In.at(2).c_str());
				lua_pushstring(lua_State, "Blue");
				lua_pcall(lua_State, 4, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "ghumanoid") {
		try {
			if (In.at(1) == "me") {
				CreateCharacterInstance("LocalPlayer", "Humanoid");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateCharacterInstance(PlayerName, "Humanoid");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "removearms") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}


	else if (In.at(0) == "removelegs") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}


	else if (In.at(0) == "apocbadges") {
		try {
			if (In.at(1) == "me") {
				ApocBadges("LocalPlayer");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				ApocBadges(PlayerName);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "doritos") {
		try {

			std::string Bk = "http://www.roblox.com/asset/?id=291951964";
			std::string Dn = "http://www.roblox.com/asset/?id=291951964";
			std::string Ft = "http://www.roblox.com/asset/?id=291951964";
			std::string Lf = "http://www.roblox.com/asset/?id=291951964";
			std::string Rt = "http://www.roblox.com/asset/?id=291951964";
			std::string Up = "http://www.roblox.com/asset/?id=291951964";
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "Sky");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, Bk.c_str());
			lua_setfield(lua_State, -2, "SkyboxBk");

			lua_pushstring(lua_State, Dn.c_str());
			lua_setfield(lua_State, -2, "SkyboxDn");

			lua_pushstring(lua_State, Ft.c_str());
			lua_setfield(lua_State, -2, "SkyboxFt");

			lua_pushstring(lua_State, Lf.c_str());
			lua_setfield(lua_State, -2, "SkyboxLf");

			lua_pushstring(lua_State, Rt.c_str());
			lua_setfield(lua_State, -2, "SkyboxRt");

			lua_pushstring(lua_State, Up.c_str());
			lua_setfield(lua_State, -2, "SkyboxUp");

			lua_pushstring(lua_State, "Sky");
			lua_setfield(lua_State, -2, "Name");

			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "nohead") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "allowreset") {
		try {
			if (In.at(1) == "me") {
				AllowReset("LocalPlayer");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				AllowReset(PlayerName);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "disablereset") {
		try {
			if (In.at(1) == "me") {
				NoReset("LocalPlayer");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				NoReset(PlayerName);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "mountaindew") {
		try {

			std::string Bk = "http://www.roblox.com/asset/?id=19969558";
			std::string Dn = "http://www.roblox.com/asset/?id=19969558";
			std::string Ft = "http://www.roblox.com/asset/?id=19969558";
			std::string Lf = "http://www.roblox.com/asset/?id=19969558";
			std::string Rt = "http://www.roblox.com/asset/?id=19969558";
			std::string Up = "http://www.roblox.com/asset/?id=19969558";
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "Sky");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, Bk.c_str());
			lua_setfield(lua_State, -2, "SkyboxBk");

			lua_pushstring(lua_State, Dn.c_str());
			lua_setfield(lua_State, -2, "SkyboxDn");

			lua_pushstring(lua_State, Ft.c_str());
			lua_setfield(lua_State, -2, "SkyboxFt");

			lua_pushstring(lua_State, Lf.c_str());
			lua_setfield(lua_State, -2, "SkyboxLf");

			lua_pushstring(lua_State, Rt.c_str());
			lua_setfield(lua_State, -2, "SkyboxRt");

			lua_pushstring(lua_State, Up.c_str());
			lua_setfield(lua_State, -2, "SkyboxUp");

			lua_pushstring(lua_State, "Sky");
			lua_setfield(lua_State, -2, "Name");

			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "foggy") {

		LightingUpdate("FogEnd", "0");
	}

	else if (In.at(0) == "clone") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Archivable");

				lua_getfield(lua_State, -1, "Clone");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 1, 0);

				lua_getglobal(lua_State, "workspace");
				lua_pushvalue(lua_State, -1);
				lua_setfield(lua_State, -3, "Parent");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Archivable");

				lua_getfield(lua_State, -1, "Clone");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 1, 0);

				lua_getglobal(lua_State, "workspace");
				lua_pushvalue(lua_State, -1);
				lua_setfield(lua_State, -3, "Parent");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "broccoliobama") {
		try {

			std::string Bk = "http://www.roblox.com/asset/?id=33321837";
			std::string Dn = "http://www.roblox.com/asset/?id=33321837";
			std::string Ft = "http://www.roblox.com/asset/?id=33321837";
			std::string Lf = "http://www.roblox.com/asset/?id=33321837";
			std::string Rt = "http://www.roblox.com/asset/?id=33321837";
			std::string Up = "http://www.roblox.com/asset/?id=33321837";
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "Sky");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, Bk.c_str());
			lua_setfield(lua_State, -2, "SkyboxBk");

			lua_pushstring(lua_State, Dn.c_str());
			lua_setfield(lua_State, -2, "SkyboxDn");

			lua_pushstring(lua_State, Ft.c_str());
			lua_setfield(lua_State, -2, "SkyboxFt");

			lua_pushstring(lua_State, Lf.c_str());
			lua_setfield(lua_State, -2, "SkyboxLf");

			lua_pushstring(lua_State, Rt.c_str());
			lua_setfield(lua_State, -2, "SkyboxRt");

			lua_pushstring(lua_State, Up.c_str());
			lua_setfield(lua_State, -2, "SkyboxUp");

			lua_pushstring(lua_State, "Sky");
			lua_setfield(lua_State, -2, "Name");

			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

		}

		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "suicide") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Humanoid");
			lua_pushstring(lua_State, "0");
			lua_setfield(lua_State, -2, "Health");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "hillary") {
		try {

			std::string Bk = "http://www.roblox.com/asset/?id=36552402";
			std::string Dn = "http://www.roblox.com/asset/?id=36552402";
			std::string Ft = "http://www.roblox.com/asset/?id=36552402";
			std::string Lf = "http://www.roblox.com/asset/?id=36552402";
			std::string Rt = "http://www.roblox.com/asset/?id=36552402";
			std::string Up = "http://www.roblox.com/asset/?id=36552402";
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "Sky");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, Bk.c_str());
			lua_setfield(lua_State, -2, "SkyboxBk");

			lua_pushstring(lua_State, Dn.c_str());
			lua_setfield(lua_State, -2, "SkyboxDn");

			lua_pushstring(lua_State, Ft.c_str());
			lua_setfield(lua_State, -2, "SkyboxFt");

			lua_pushstring(lua_State, Lf.c_str());
			lua_setfield(lua_State, -2, "SkyboxLf");

			lua_pushstring(lua_State, Rt.c_str());
			lua_setfield(lua_State, -2, "SkyboxRt");

			lua_pushstring(lua_State, Up.c_str());
			lua_setfield(lua_State, -2, "SkyboxUp");

			lua_pushstring(lua_State, "Sky");
			lua_setfield(lua_State, -2, "Name");

			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "pepe") {
		try {

			std::string Bk = "http://www.roblox.com/asset/?id=244905904";
			std::string Dn = "http://www.roblox.com/asset/?id=244905904";
			std::string Ft = "http://www.roblox.com/asset/?id=244905904";
			std::string Lf = "http://www.roblox.com/asset/?id=244905904";
			std::string Rt = "http://www.roblox.com/asset/?id=244905904";
			std::string Up = "http://www.roblox.com/asset/?id=244905904";
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "Sky");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, Bk.c_str());
			lua_setfield(lua_State, -2, "SkyboxBk");

			lua_pushstring(lua_State, Dn.c_str());
			lua_setfield(lua_State, -2, "SkyboxDn");

			lua_pushstring(lua_State, Ft.c_str());
			lua_setfield(lua_State, -2, "SkyboxFt");

			lua_pushstring(lua_State, Lf.c_str());
			lua_setfield(lua_State, -2, "SkyboxLf");

			lua_pushstring(lua_State, Rt.c_str());
			lua_setfield(lua_State, -2, "SkyboxRt");

			lua_pushstring(lua_State, Up.c_str());
			lua_setfield(lua_State, -2, "SkyboxUp");

			lua_pushstring(lua_State, "Sky");
			lua_setfield(lua_State, -2, "Name");

			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "volume") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, "Sound");
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_setfield(lua_State, -2, "Volume");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "weirdify") {
		try {
			if (In.at(1) == "me") {
				LeftArm("LocalPlayer", "SpecialMesh");
				LeftLeg("LocalPlayer", "SpecialMesh");
				CreateCharacterInstance("LocalPlayer", "SpecialMesh");
				RightArm("LocalPlayer", "SpecialMesh");
				RightLeg("LocalPlayer", "SpecialMesh");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				LeftArm(PlayerName.c_str(), "SpecialMesh");
				LeftLeg(PlayerName.c_str(), "SpecialMesh");
				CreateCharacterInstance(PlayerName.c_str(), "SpecialMesh");
				RightArm(PlayerName.c_str(), "SpecialMesh");
				RightLeg(PlayerName.c_str(), "SpecialMesh");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}

	}


	else if (In.at(0) == "tp") {
		try {
			if (In.at(1) == "me") {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "HumanoidRootPart");
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "CFrame");
				lua_setfield(lua_State, -6, "CFrame");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				int Character2 = GetCharacter(In.at(2));
				std::string PlayerName2 = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "HumanoidRootPart");
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Workspace");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "CFrame");
				lua_setfield(lua_State, -6, "CFrame");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "goto") {
		try {
			int Character = GetCharacter(In.at(1));
			std::string PlayerName = *GetName(Character);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, PlayerName.c_str());
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "HumanoidRootPart");
			lua_getfield(lua_State, -1, "CFrame");
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "HumanoidRootPart");
			lua_pushvalue(lua_State, -5);
			lua_setfield(lua_State, -2, "CFrame");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "clearlighting") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getfield(lua_State, -1, "ClearAllChildren");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "bring") {
		try {
			int Character = GetCharacter(In.at(1));
			std::string PlayerName = *GetName(Character);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "HumanoidRootPart");
			lua_getfield(lua_State, -1, "CFrame");
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, PlayerName.c_str());
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "HumanoidRootPart");
			lua_pushvalue(lua_State, -5);
			lua_setfield(lua_State, -2, "CFrame");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "faceid") {
		try {
			if (In.at(1) == "me") {
				std::string Texture = "http://www.roblox.com/asset/?id=" + In.at(2);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getfield(lua_State, -1, "face");

				lua_pushstring(lua_State, Texture.c_str());
				lua_setfield(lua_State, -2, "Texture");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				std::string Texture = "http://www.roblox.com/asset/?id=" + In.at(2);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getfield(lua_State, -1, "face");

				lua_pushstring(lua_State, Texture.c_str());
				lua_setfield(lua_State, -2, "Texture");
			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "tpgame") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "TeleportService");
			lua_getfield(lua_State, -1, "Teleport");
			lua_pushvalue(lua_State, -2);
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_pcall(lua_State, 2, 0, 0);
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "aeffect") {
		try {
			if (In.at(1) == "me") {
				ForceField("LocalPlayer", "ForceField");
				CreateCharacterInstance("LocalPlayer", "Fire");
				CreateCharacterInstance("LocalPlayer", "Sparkles");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				ForceField(PlayerName, "ForceField");
				CreateCharacterInstance(PlayerName, "Fire");
				CreateCharacterInstance(PlayerName, "Sparkles");
			}
		}
		catch (std::exception) {

		}
	}
	lua_emptystack(lua_State);
}


void DoLuaCommand(std::string Cmd) {
	std::vector<std::string> In = split(Cmd);

	if (In.at(0) == "newtool") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Backpack");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "Tool");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, "0");
				lua_setfield(lua_State, -2, "TextureId");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Backpack");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "Tool");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, "0");
				lua_setfield(lua_State, -2, "TextureId");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "removeface") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getfield(lua_State, -1, "face");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getfield(lua_State, -1, "face");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "particle") {
		try {
			if (In.at(1) == "me") {
				std::string texture = "rbxassetid://" + In.at(2);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "ParticleEmitter");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, texture.c_str());
				lua_setfield(lua_State, -2, "Texture");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				std::string texture = "rbxassetid://" + In.at(2);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "ParticleEmitter");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, texture.c_str());
				lua_setfield(lua_State, -2, "Texture");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}



	else if (In.at(0) == "cameralock") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_pushstring(lua_State, "0");
				lua_setfield(lua_State, -2, "CameraMaxZoomDistance");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_pushstring(lua_State, "0");
				lua_setfield(lua_State, -2, "CameraMaxZoomDistance");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "camerahalflock") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_pushstring(lua_State, "16");
				lua_setfield(lua_State, -2, "CameraMaxZoomDistance");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_pushstring(lua_State, "16");
				lua_setfield(lua_State, -2, "CameraMaxZoomDistance");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "cameraunlock") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_pushstring(lua_State, "32");
				lua_setfield(lua_State, -2, "CameraMaxZoomDistance");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_pushstring(lua_State, "32");
				lua_setfield(lua_State, -2, "CameraMaxZoomDistance");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}



	else if (In.at(0) == "ragdoll") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "PlatformStand");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "PlatformStand");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "unragdoll") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, false);
				lua_setfield(lua_State, -2, "PlatformStand");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, false);
				lua_setfield(lua_State, -2, "PlatformStand");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "unselect") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "SelectionBox");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "SelectionBox");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "select") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "SelectionBox");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushvalue(lua_State, -4);
				lua_setfield(lua_State, -2, "Adornee");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "SelectionBox");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushvalue(lua_State, -4);
				lua_setfield(lua_State, -2, "Adornee");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "exit") {
		exit(0);
	}


	else if (In.at(0) == "ghosthead") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "unchar") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "CanLoadCharacterAppearance");
			}
			else {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, In.at(1).c_str());
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "CanLoadCharacterAppearance");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "rechar") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "CanLoadCharacterAppearance");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "CanLoadCharacterAppearance");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "vishead") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}


	else if (In.at(0) == "normalsunpos") {

		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_pushstring(lua_State, "41.733");
			lua_setfield(lua_State, -2, "GeographicLatitude");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "sunpos") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_setfield(lua_State, -2, "GeographicLatitude");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "btools") {
		try {
			if (In.at(1) == "me") {


				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Backpack");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "HopperBin");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, "2");
				lua_setfield(lua_State, -2, "BinType");


				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Backpack");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "HopperBin");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, "3");
				lua_setfield(lua_State, -2, "BinType");


				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Backpack");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "HopperBin");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, "4");
				lua_setfield(lua_State, -2, "BinType");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);


				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Backpack");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "HopperBin");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, "2");
				lua_setfield(lua_State, -2, "BinType");


				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Backpack");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "HopperBin");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, "3");
				lua_setfield(lua_State, -2, "BinType");


				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Backpack");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "HopperBin");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushstring(lua_State, "4");
				lua_setfield(lua_State, -2, "BinType");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "skybox") {
		try {

			std::string Bk = "http://www.roblox.com/asset/?id=" + In.at(1);
			std::string Dn = "http://www.roblox.com/asset/?id=" + In.at(1);
			std::string Ft = "http://www.roblox.com/asset/?id=" + In.at(1);
			std::string Lf = "http://www.roblox.com/asset/?id=" + In.at(1);
			std::string Rt = "http://www.roblox.com/asset/?id=" + In.at(1);
			std::string Up = "http://www.roblox.com/asset/?id=" + In.at(1);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "Sky");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);

			lua_pushstring(lua_State, Bk.c_str());
			lua_setfield(lua_State, -2, "SkyboxBk");

			lua_pushstring(lua_State, Dn.c_str());
			lua_setfield(lua_State, -2, "SkyboxDn");

			lua_pushstring(lua_State, Ft.c_str());
			lua_setfield(lua_State, -2, "SkyboxFt");

			lua_pushstring(lua_State, Lf.c_str());
			lua_setfield(lua_State, -2, "SkyboxLf");

			lua_pushstring(lua_State, Rt.c_str());
			lua_setfield(lua_State, -2, "SkyboxRt");

			lua_pushstring(lua_State, Up.c_str());
			lua_setfield(lua_State, -2, "SkyboxUp");

			lua_pushstring(lua_State, "Sky");
			lua_setfield(lua_State, -2, "Name");

			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "rskybox") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getfield(lua_State, -1, "ClearAllChildren");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "enableturns") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "AutoRotate");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "AutoRotate");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "disableturns") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "AutoRotate");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "AutoRotate");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "name") {
		try {
			if (In.at(1) == "me") {
				string string;
				for (size_t i = 1; i < In.size(); i++) {
					if (i < (In.size() - 1)) {
						string.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 1)) {
						string.append(In.at(i));
					}
				}

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Archivable");
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_pushstring(lua_State, string.c_str());
				lua_setfield(lua_State, -2, "Name");
			}
			else {

				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				string string;
				for (size_t i = 1; i < In.size(); i++) {
					if (i < (In.size() - 1)) {
						string.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 1)) {
						string.append(In.at(i));
					}
				}
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Archivable");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_pushstring(lua_State, string.c_str());
				lua_setfield(lua_State, -2, "Name");
			}
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "jump") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Jump");
			}

			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Jump");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}


	else if (In.at(0) == "newspawn") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "SpawnLocation");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 1, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl; //new hax
		}
	}


	else if (In.at(0) == "invishead") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "vishead") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "ghost") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, "0.6");
				lua_setfield(lua_State, -2, "Transparency");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}

	}

	else if (In.at(0) == "invis") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Transparency");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}

	}

	else if (In.at(0) == "vis") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, 0);
				lua_setfield(lua_State, -2, "Transparency");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "showname") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushvalue(lua_State, 0);
				lua_setfield(lua_State, -2, "DisplayDistanceType");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushvalue(lua_State, 0);
				lua_setfield(lua_State, -2, "DisplayDistanceType");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "hidename") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushvalue(lua_State, 2);
				lua_setfield(lua_State, -2, "DisplayDistanceType");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushvalue(lua_State, 2);
				lua_setfield(lua_State, -2, "DisplayDistanceType");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "sit") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Sit");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Sit");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "pitch") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, "Sound");
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_setfield(lua_State, -2, "Pitch");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "dialog") {
		try {
			if (In.at(1) == "me") {
				CreateCharacterInstance("LocalPlayer", "Dialog");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateCharacterInstance(PlayerName, "Dialog");

			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "blur") {
		try {
			NewLighting("BlurEffect");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "bloom") {
		try {
			NewLighting("BloomEffect");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "fling") {
		try {
			if (In.at(1) == "me") {
				CreateCharacterInstance("LocalPlayer", "BodyPosition");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateCharacterInstance(PlayerName, "BodyPosition");

			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}

	}

	else if (In.at(0) == "disableanimate") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Animate");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Disabled");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Animate");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Disabled");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "enableanimate") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Animate");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Enabled");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Animate");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Enabled");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "disableregen") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Health");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Disabled");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Health");
				lua_pushstring(lua_State, "1");
				lua_setfield(lua_State, -2, "Disabled");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "enableregen") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Health");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Disabled");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Health");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Disabled");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "newteam") {
		try {
			string string;
			for (size_t i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					string.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					string.append(In.at(i));
				}
			}
			NewTeam(string.c_str());
		}
		catch (std::exception)
		{
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "outlines") {
		try {
			if (In.at(1) == "on") {

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Lighting");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Outlines");
			}

			else if (In.at(1) == "disable") {

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Lighting");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Outlines");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "unlock") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");
			}
			else
			{

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, In.at(1).c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, In.at(1).c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, In.at(1).c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, In.at(1).c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, In.at(1).c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, In.at(1).c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Locked");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "lock") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");
			}
			else
			{
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Arm");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Left Leg");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Arm");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Right Leg");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Locked");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "freeze") {
		try {
			if (In.at(1) == "me") {

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Anchored");
			}
			else
			{
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "true");
				lua_setfield(lua_State, -2, "Anchored");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "thaw") {
		try {
			if (In.at(1) == "me") {

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Anchored");
			}
			else
			{
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "false");
				lua_setfield(lua_State, -2, "Anchored");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "fogcolor") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Color3");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_pushstring(lua_State, In.at(2).c_str());
			lua_pushstring(lua_State, In.at(3).c_str());
			lua_pcall(lua_State, 3, 1, 0);
			lua_setfield(lua_State, -3, "FogColor");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "ambient") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getglobal(lua_State, "Color3");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_pushstring(lua_State, In.at(2).c_str());
			lua_pushstring(lua_State, In.at(3).c_str());
			lua_pcall(lua_State, 3, 1, 0);
			lua_setfield(lua_State, -3, "Ambient");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "watercolor") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, "Terrain");
			lua_getglobal(lua_State, "Color3");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_pushstring(lua_State, In.at(2).c_str());
			lua_pushstring(lua_State, In.at(3).c_str());
			lua_pcall(lua_State, 3, 1, 0);
			lua_setfield(lua_State, -3, "WaterColor");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "gravity") {
		try {

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_setfield(lua_State, -2, "Gravity");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "normalgravity") {
		try {

			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_pushstring(lua_State, "196.2");
			lua_setfield(lua_State, -2, "Gravity");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "naked") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Shirt");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Pants");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

			}
			else {

				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Shirt");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Pants");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "fov") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, "Camera");
			lua_pushvalue(lua_State, std::stof(In.at(1).c_str()));
			lua_setfield(lua_State, -2, "FieldOfView");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "speedy") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getfield(lua_State, -1, "Humanoid");
			lua_pushstring(lua_State, "230");
			lua_setfield(lua_State, -2, "WalkSpeed");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "ws") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, In.at(2).c_str());
				lua_setfield(lua_State, -2, "WalkSpeed");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, In.at(2).c_str());
				lua_setfield(lua_State, -2, "WalkSpeed");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "charapp") {
		try {
			if (In.at(1) == "me") {
				ChangeChar("LocalPlayer", In.at(2).c_str());
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				ChangeChar(PlayerName, In.at(2).c_str());
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}




	else if (In.at(0) == "health") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, In.at(2).c_str());
				lua_setfield(lua_State, -2, "MaxHealth");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, In.at(2).c_str());
				lua_setfield(lua_State, -2, "MaxHealth");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "god") {
		try {
			if (In.at(1) == "me") {

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushnumber(lua_State, HUGE_VAL);
				lua_setfield(lua_State, -2, "MaxHealth");

				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushnumber(lua_State, HUGE_VAL);
				lua_setfield(lua_State, -2, "Health");

			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushnumber(lua_State, HUGE_VAL);
				lua_setfield(lua_State, -2, "MaxHealth");


				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushnumber(lua_State, HUGE_VAL);
				lua_setfield(lua_State, -2, "Health");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "sgod") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "1000000000");
				lua_setfield(lua_State, -2, "MaxHealth");

			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "1000000000");
				lua_setfield(lua_State, -2, "MaxHealth");

			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "ungod") {
		try {
			if (In.size() == 2) {
				if (In.at(1) == "me") {
					lua_getglobal(lua_State, "game");
					lua_getfield(lua_State, -1, "Players");
					lua_getfield(lua_State, -1, "LocalPlayer");
					lua_getfield(lua_State, -1, "Character");
					lua_getfield(lua_State, -1, "Humanoid");
					lua_pushstring(lua_State, "100");
					lua_setfield(lua_State, -2, "MaxHealth");
				}
				else {
					int Character = GetCharacter(In.at(1));
					std::string PlayerName = *GetName(Character);
					lua_getglobal(lua_State, "game");
					lua_getfield(lua_State, -1, "Players");
					lua_getfield(lua_State, -1, In.at(1).c_str());
					lua_getfield(lua_State, -1, "Character");
					lua_getfield(lua_State, -1, "Humanoid");
					lua_pushstring(lua_State, "100");
					lua_setfield(lua_State, -2, "MaxHealth");

				}
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "hipheight") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, In.at(2).c_str());
				lua_setfield(lua_State, -2, "HipHeight");

			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, In.at(2).c_str());
				lua_setfield(lua_State, -2, "HipHeight");

			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "nojump") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "0");
				lua_setfield(lua_State, -2, "JumpPower");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "0");
				lua_setfield(lua_State, -2, "JumpPower");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "jpower") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, In.at(2).c_str());
				lua_setfield(lua_State, -2, "JumpPower");

			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				stringstream sstrm(In.at(2));
				string Value;
				sstrm >> Value;
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, Value.c_str());
				lua_setfield(lua_State, -2, "JumpPower");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "removetools") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Backpack");
				lua_getfield(lua_State, -1, "ClearAllChildren");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Backpack");
				lua_getfield(lua_State, -1, "ClearAllChildren");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "stopmusic") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, "Sound");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "clubpenguinearrape") {
		try {
			Music("335816974");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "purge") {
		try {
			Music("188390178");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "ohbabyatriple") {
		try {
			Music("184420199");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "allahakbar") {
		try {
			Music("446824013");
		}
		catch (std::exception) {

		}
	}
	else if (In.at(0) == "rickrollrape") {
		try {
			Music("290536819");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "allahakbartrap") {
		try {
			Music("385672619");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "rickroll") {
		try {
			Music("2027611");
		}
		catch (std::exception) {

		}
	}


	else if (In.at(0) == "shutthefuckup") {
		try {
			Music("510431603");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "music") {
		try {
			if (In.size() == 2) {
				Music(In.at(1));
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}
	else if (In.at(0) == "kill") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "0");
				lua_setfield(lua_State, -2, "Health");

			}
			else
			{
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Humanoid");
				lua_pushstring(lua_State, "0");
				lua_setfield(lua_State, -2, "Health");

			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "clearteams") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Teams");
			lua_pcall(lua_State, 2, 1, 0);
			lua_getfield(lua_State, -1, "ClearAllChildren");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "changestat") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "leaderstats");
				lua_getfield(lua_State, -1, In.at(2).c_str());
				lua_pushstring(lua_State, In.at(3).c_str());
				lua_setfield(lua_State, -2, "Value");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "leaderstats");
				lua_getfield(lua_State, -1, In.at(2).c_str());
				lua_pushstring(lua_State, In.at(3).c_str());
				lua_setfield(lua_State, -2, "Value");

			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "cylhead") {
		try {
			if (In.at(1) == "me") {
				CreateHeadPart("LocalPlayer", "CylinderMesh");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateHeadPart(PlayerName, "CylinderMesh");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "blockhead") {
		try {
			if (In.at(1) == "me") {
				CreateHeadPart("LocalPlayer", "BlockMesh");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateHeadPart(PlayerName, "BlockMesh");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "peahead") {
		try {
			if (In.at(1) == "me") {
				CreateHeadPart("LocalPlayer", "SpecialMesh");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateHeadPart(PlayerName, "SpecialMesh");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "destroy") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, In.at(1).c_str());
			lua_getfield(lua_State, -1, In.at(2).c_str());
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "freecam") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "HumanoidRootPart");
				lua_getfield(lua_State, -1, "RootJoint");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "HumanoidRootPart");
				lua_getfield(lua_State, -1, "RootJoint");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "normalhead") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getfield(lua_State, -1, "Mesh");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Head");
				lua_getfield(lua_State, -1, "Mesh");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "unspin") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "BodyAngularVelocity");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "BodyAngularVelocity");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "unblur") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getfield(lua_State, -1, "Blur");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "unbloom") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_getfield(lua_State, -1, "Bloom");
			lua_getfield(lua_State, -1, "Destroy");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "nodialog") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "Dialog");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "Dialog");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}



	else if (In.at(0) == "removeparticle") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "ParticleEmitter");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_pushstring(lua_State, "ParticleEmitter");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "unfire") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "Fire");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "Fire");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "unsparkles") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "Sparkles");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "Sparkles");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "unsmoke") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "Smoke");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "Smoke");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "explode") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "Explosion");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "Explosion");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "unff") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "ForceField");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "ForceField");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}


	else if (In.at(0) == "ff") {
		if (In.at(1) == "me") {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "LocalPlayer");
			lua_getfield(lua_State, -1, "Character");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "ForceField");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 0, 0);
		}
		else {
			int Character = GetCharacter(In.at(1));
			std::string PlayerName = *GetName(Character);
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, PlayerName.c_str());
			lua_getfield(lua_State, -1, "Character");
			lua_getglobal(lua_State, "Instance");
			lua_getfield(lua_State, -1, "new");
			lua_pushstring(lua_State, "ForceField");
			lua_pushvalue(lua_State, -4);
			lua_pcall(lua_State, 2, 0, 0);
		}
	}

	else if (In.at(0) == "gotospawn") {
		try {
			if (In.at(1) == "me") {
				CreateCharacterInstance("LocalPlayer", "BodyPosition");
			}
			else {
				CreateCharacterInstance(In.at(1), "BodyPosistion");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "unlight") {
		try {
			if (In.at(1) == "me") {
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, "LocalPlayer");
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "SpotLight");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);

			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "Players");
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getfield(lua_State, -1, "SpotLight");
				lua_getfield(lua_State, -1, "Destroy");
				lua_pushvalue(lua_State, -2);
				lua_pcall(lua_State, 1, 0, 0);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "light") {
		try {
			if (In.at(1) == "me") {
				CreateCharacterInstance("LocalPlayer", "SpotLight");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateCharacterInstance(PlayerName, "SpotLight");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "spin") {
		try {
			if (In.at(1) == "me") {
				CreateCharacterInstance("LocalPlayer", "BodyAngularVelocity");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateCharacterInstance(PlayerName, "BodyAngularvelocity");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "fire") {
		try {
			if (In.at(1) == "me") {
				CreateCharacterInstance("LocalPlayer", "Fire");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				lua_getglobal(lua_State, "game");
				lua_getfield(lua_State, -1, "GetService");
				lua_pushvalue(lua_State, -2);
				lua_pushstring(lua_State, "Players");
				lua_pcall(lua_State, 2, 1, 0);
				lua_getfield(lua_State, -1, PlayerName.c_str());
				lua_getfield(lua_State, -1, "Character");
				lua_getfield(lua_State, -1, "Torso");
				lua_getglobal(lua_State, "Instance");
				lua_getfield(lua_State, -1, "new");
				lua_pushstring(lua_State, "Fire");
				lua_pushvalue(lua_State, -4);
				lua_pcall(lua_State, 2, 1, 0);
				lua_pushnumber(lua_State, 30);
				lua_setfield(lua_State, -2, "Size");
				lua_emptystack(lua_State);
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "sparkles") {
		try {
			if (In.at(1) == "me") {
				CreateCharacterInstance("LocalPlayer", "Sparkles");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateCharacterInstance(PlayerName, "Sparkles");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "smoke") {
		try {
			if (In.at(1) == "me") {
				CreateCharacterInstance("LocalPlayer", "Smoke");
			}
			else {
				int Character = GetCharacter(In.at(1));
				std::string PlayerName = *GetName(Character);
				CreateCharacterInstance(PlayerName, "Smoke");
			}
		}
		catch (std::exception) {
			std::cout << Error << std::endl;

		}
	}

	else if (In.at(0) == "printidentity") {
		try {
			PrintIdentity();
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}

	}

	else if (In.at(0) == "time") {

		lua_getglobal(lua_State, "game");
		lua_getfield(lua_State, -1, "Lighting");
		lua_pushstring(lua_State, In.at(1).c_str());
		lua_setfield(lua_State, -2, "TimeOfDay");
	}

	else if (In.at(0) == "day") {

		lua_getglobal(lua_State, "game");
		lua_getfield(lua_State, -1, "Lighting");
		lua_pushstring(lua_State, "14");
		lua_setfield(lua_State, -2, "TimeOfDay");

	}


	else if (In.at(0) == "dawn") {

		lua_getglobal(lua_State, "game");
		lua_getfield(lua_State, -1, "Lighting");
		lua_pushstring(lua_State, "18");
		lua_setfield(lua_State, -2, "TimeOfDay");

	}

	else if (In.at(0) == "night") {

		lua_getglobal(lua_State, "game");
		lua_getfield(lua_State, -1, "Lighting");
		lua_pushstring(lua_State, "0");
		lua_setfield(lua_State, -2, "TimeOfDay");

	}

	else if (In.at(0) == "night") {

		lua_getglobal(lua_State, "game");
		lua_getfield(lua_State, -1, "Lighting");
		lua_pushstring(lua_State, "18");
		lua_setfield(lua_State, -2, "TimeOfDay");

	}

	else if (In.at(0) == "dark") {

		lua_getglobal(lua_State, "game");
		lua_getfield(lua_State, -1, "Lighting");
		lua_pushstring(lua_State, "0");
		lua_setfield(lua_State, -2, "Brightness");

	}

	else if (In.at(0) == "bright") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_pushstring(lua_State, "5");
			lua_setfield(lua_State, -2, "Brightness");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "brightness") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Lighting");
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_setfield(lua_State, -2, "Brightness");
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "fogend") {
		try {
			LightingUpdate("FogEnd", In.at(1));
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}
	else if (In.at(0) == "nofog") {
		LightingUpdate("FogEnd", "9999999999");
	}

	else if (In.at(0) == "fogstart") {

		LightingUpdate("FogStart", In.at(1));
	}

	else if (In.at(0) == "print") {
		try {
			string string;
			for (size_t i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					string.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					string.append(In.at(i));
				}
			}
			Print(string.c_str());
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "warn") {
		try {
			string string;
			for (size_t i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					string.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					string.append(In.at(i));
				}
			}
			Warn(string.c_str());
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "wavespeed") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, "Terrain");
			lua_pushstring(lua_State, In.at(1).c_str());
			lua_setfield(lua_State, -2, "WaterWaveSpeed");
		}
		catch (std::exception) {

		}
	}

	else if (In.at(0) == "wavesize") {

		lua_getglobal(lua_State, "game");
		lua_getfield(lua_State, -1, "Workspace");
		lua_getfield(lua_State, -1, "Terrain");
		lua_pushstring(lua_State, In.at(1).c_str());
		lua_setfield(lua_State, -2, "WaterWaveSize");

	}


	else if (In.at(0) == "clearterrain") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, "Terrain");
			lua_getfield(lua_State, -1, "Clear");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);

		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "shutdown") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Players");
			lua_getfield(lua_State, -1, "ClearAllChildren");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}

	else if (In.at(0) == "clearworkspace") {
		try {
			lua_getglobal(lua_State, "game");
			lua_getfield(lua_State, -1, "Workspace");
			lua_getfield(lua_State, -1, "ClearAllChildren");
			lua_pushvalue(lua_State, -2);
			lua_pcall(lua_State, 1, 0, 0);
		}
		catch (std::exception) {
			std::cout << Error << std::endl;
		}
	}
	lua_emptystack(lua_State);
}

DWORD WINAPI LCPipe(PVOID lvpParameter)
{
	HANDLE hPipe;
	char buffer[1024];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\LCPipe"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				luaC(buffer);
				//}
			}
		}
		DisconnectNamedPipe(hPipe);
	}
}


void CheckForBlackListedUsers() {
	//VMProtectDecryptStringA("storedogshitnigger");

	if (*PlayerName == "GalaxyFencer") {
		MessageBoxA(0, "Our systems automatically has detected that you are a blacklisted user.", "Blacklisted", MB_OK);
		exit(0);
	}

	else {
	}

	//VMProtectEnd();
}

#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <WinINet.h>
#include <algorithm>
#include <random>
#include <typeinfo>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#define TH32CS_SNAPPROCESS 0x00000002
bool ProcessRunning(const char* name)
{
	HANDLE SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (SnapShot == INVALID_HANDLE_VALUE)
		return false;

	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(SnapShot, &procEntry))
		return false;

	do
	{
		if (strcmp((const char *)procEntry.szExeFile, name) == 0)
			return true;
	} while (Process32Next(SnapShot, &procEntry));

	return false;
}

bool IsProcessRunning(const wchar_t *processName)
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!wcsicmp((wchar_t*)entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}

vector<string> SplitString(string str, char delimiter) {
	vector<string> tokens;
	stringstream ss(str);
	string tok;

	while (getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}


void DoHotKeysCommands(std::string Command) {
	try {
		DoConsoleCommands(Command);
		DoLuaCommand(Command);
		DoLuaCommand2(Command);
		DoLuaCommand3(Command);
	}
	catch (...) {

	}
}

void HotKeys_Loop() {
	do {
		if (hotkeybools = true) {
			if (GetAsyncKeyState(VK_F2)) {
				DoHotKeysCommands("ff me");
			}

			else if (GetAsyncKeyState(VK_F3)) {
				DoHotKeysCommands("btools me");
			}

			else if (GetAsyncKeyState(VK_F4)) {
				DoHotKeysCommands("god me");
			}

			else if (GetAsyncKeyState(VK_F5)) {
				DoHotKeysCommands("speedy");
			}

			else if (GetAsyncKeyState(VK_F6)) {
				DoHotKeysCommands("invis me");
			}

			else if (GetAsyncKeyState(VK_F7)) {
				DoHotKeysCommands("vis me");
			}
		}
	} while (true);
}

void DoCommands(std::string Command) {
	try {
		auto Context = (int*(__cdecl*)
			())getaddy(0x59a8c0);
		*(int*)Context() = 6;
		DoConsoleCommands(Command);
		DoLuaCommand(Command);
		DoLuaCommand2(Command);
		DoLuaCommand3(Command);
	}
	catch (...) {

	}
}

/*int LocalPlayerChatted(DWORD luaState) {
CallCheck_Disable();
std::string chatMsg = lua_tolstring(luaState, -2, NULL);

if (chatMsg.substr(0, 1) == ";") {
chatMsg = chatMsg.substr(1, string::npos);
DoCommands(chatMsg);
}
return 0;
}*/

/*void chathookmeme()
{
auto Ret = Returncheck();
CallCheck_Disable();
lua_getglobal(lua_State, "game");
lua_getfield(lua_State, -1, "Players");
lua_getfield(lua_State, -1, "LocalPlayer");
lua_getfield(lua_State, -1, "Chatted");
lua_getfield(lua_State, -1, "connect");
lua_pushvalue(lua_State, -2);
lua_pushcclosure(lua_State, (int)LocalPlayerChatted, 0);
lua_pcall(lua_State, 2, 0, 0);
Ret();
} */

void Init() {
	//VMProtectBeginVirtualization("Main");
	DWORD OldPerm;

	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &OldPerm);
	VirtualProtect(&MessageBoxA, 5, PAGE_EXECUTE_READWRITE, &OldPerm);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	MoveWindow(ConsoleHandle, 500, 500, 565, 500, TRUE);
	SetWindowLong(ConsoleHandle, GWL_STYLE,
		GetWindowLong(ConsoleHandle, GWL_STYLE) & ~WS_MAXIMIZEBOX);
	SetConsoleTitleA("ZERO v2");

	//Auth();

	std::cout << blue << "Welcome to ZERO v2..." << white << std::endl;

	std::cout << blue << "We are making a big update soon for you guys, stay tuned!" << white << std::endl;

	std::cout << green << "Updated and fixed by Deniality. :)(OLDEST)" << white << std::endl;

	std::cout << green << "Updated and fixed by rakion99. :)(NEW)" << white << std::endl;

	Sleep(2500);

	Scan();

	std::cout << blue << "ZERO v2 Development Team" << white << std::endl;
	std::cout << green << "Captain Rex - Owner and Developer" << white << std::endl;
	std::cout << blue << "Deniality - Co-Owner Developer/Successor of Project Zero" << white << std::endl;
	std::cout << red << "bditt - Developer" << white << std::endl;
	std::cout << blue << "Ice Bear - Developer \r\n" << white << std::endl;


	std::cout << blue << "shoutout to my mommy" << white << std::endl;

	std::cout << "\r\n";

	std::cout << blue << "lua_State: " << hex << lua_State << white << std::endl;

	std::cout << "Welcome " << blue << *PlayerName << white << ", ZERO v2 has loaded.\r\n";

	std::cout << blue << "Type 'cmds' for list of commands.\r\n" << white << std::endl;

	int FilteringEnabled = GetFilteringEnabled(Workspace);
	if (!FilteringEnabled) {
		std::cout << blue << "Filtering is disabled.\r\n" << white << std::endl;
	}
	else {
		std::cout << red << "Filtering is enabled. Most commands WILL not replicate.\r\n" << white << std::endl;
	}
	//chathookmeme();

	//std::cout << blue << "ChatHook has loaded successfully. The prefix is ';'. (BUGGY, DO NOT USE YET)" << std::endl;

	while (true) {
		try {
			std::cout << blue << "> " << white;
			std::string Z = Input();
			DoCommands(Z);
		}
		catch (std::exception) {
			std::cout << red << Error << white << std::endl;
		}
	}
	//VMProtectEnd();
}
