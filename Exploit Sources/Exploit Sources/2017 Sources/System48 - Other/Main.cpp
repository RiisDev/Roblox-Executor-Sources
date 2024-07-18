#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <Tlhelp32.h>
#include <algorithm>
#include <CommCtrl.h>
#include <Richedit.h>
#include <WinInet.h>
#include <conio.h>
#include <WinUser.h>

#include "CLua.h"
#include "Memory.h"
#include "VMProtectSDK.h"

/*
If you're honestly interested in learning, take a read.
Please, kindly leave if you're only here to steal code for your own use.
*/

using namespace std;

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "wininet.lib")

#define CLIENT_VERSION "3.2.6"
#define DEBUG_MODE 1
#define SCRIPTCONTEXT 0x1186B4C

//toolbar
#define EXIT (WM_APP + 100)
#define CREDITS (WM_APP + 102)
#define MINIMIZE (WM_APP + 103)

#define LOCALGODMODE (WM_APP + 103)
#define LOCALHEAL (WM_APP + 104)
#define LOCALFORCEFIELD (WM_APP + 105)
#define LOCALBTOOLS (WM_APP + 106)
#define LOCALGUESTCHAT (WM_APP + 1001)

#define TIME_DAY (WM_APP + 107)
#define TIME_NIGHT (WM_APP + 108)

#define CLEARALLCHILDREN (WM_APP + 110)

#define APOCBADGES (WM_APP + 113)

#define FILTERINGCHECK (WM_APP + 111)
#define STOPMUSIC (WM_APP + 112)

//firstrow
#define FORCEFIELD (WM_APP + 201)
#define WALKSPEED (WM_APP + 202)
#define UNFORCEFIELD (WM_APP + 203)
#define KICK (WM_APP + 204)
#define EXPLODE (WM_APP + 205)
#define KILL (WM_APP + 206)
#define SPARKLES (WM_APP + 207)
#define UNSPARKLES (WM_APP + 208)
#define INVISIBLE (WM_APP + 209)
#define VISIBLE (WM_APP + 210)
#define BRING (WM_APP + 211)
#define GOTO (WM_APP + 212)
#define GOD (WM_APP + 213)
#define UNGOD (WM_APP + 214)
//newrow
#define FREEZE (WM_APP + 215)
#define UNFREEZE (WM_APP + 216)
#define RTOOLS (WM_APP + 217)
#define BTOOLS (WM_APP + 218)
#define FIRE (WM_APP + 219)
#define UNFIRE (WM_APP + 220)
#define SMOKE (WM_APP + 221)
#define UNSMOKE (WM_APP + 222)
//newrow
#define SECRETGOD (WM_APP + 223)
#define UNSECRETGOD (WM_APP + 224)
#define BIGHEAD (WM_APP + 225)
#define WATCH (WM_APP + 226)
#define UNWATCH (WM_APP + 227)


#define LOCALGODMODE (WM_APP + 301)
#define LOCALHEAL (WM_APP + 302)

#define FILTERINGCHECK (WM_APP + 409)
#define STOPMUSIC (WM_APP + 410)

#define REMOVE_FOG (WM_APP + 500)
#define RED_FOG (WM_APP + 501)
#define BLUE_FOG (WM_APP + 502)
#define GREEN_FOG (WM_APP + 503)
#define BLACK_FOG (WM_APP + 504)
#define WHITE_FOG (WM_APP + 505)
#define RAINBOW_FOG (WM_APP + 506)
#define TOGGLEDL (WM_APP + 507)

#define UNANCHORWS (WM_APP + 608)
#define UNLOCKWS (WM_APP + 609)

//value commands
#define WALKSPEED (WM_APP + 701)
#define JUMPPOWER (WM_APP + 702)
#define FOGEND (WM_APP + 703)
#define SOUND (WM_APP + 704)
#define SKYBOX (WM_APP + 705)
#define PARTICLES (WM_APP + 706)

//Get the global state, using the address is plain out disgraceful (this isnt working, its a very simple fix doe)
int GetGlobalState(DWORD* Instance, DWORD Index) {
	DWORD *v2 = Instance;
	DWORD v3 = Index;

	return v2[14 * v3 * 41] - (DWORD)&v2[14 * v3 + 41];
}

HINSTANCE HInstance;

HANDLE hProc = GetCurrentProcess();

HWND MainWindow;
HWND NameField;
HWND WalkSpeedField;
HWND JumpPowerField;
HWND FogEndField;
HWND SoundField;
HWND SkyboxField;
HWND ParticleField;


DWORD DataModel;
DWORD Players;
DWORD LocalPlayer;
DWORD Workspace;

string NameEntered;

LONG RobloxWndProc;
HWND RobloxWindow;

bool DynamicOn = true;

vector<string> SplitString(string str, char delimiter) {
	vector<string> tokens;
	stringstream ss(str);
	string tok;

	while (getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}

int getParent(DWORD Instance) {
	//snip
}

const char* getClassName(DWORD Instance) {
	//snip
}

string getName(DWORD Instance) {
	string *wat;
	//snip
}

vector<int> getChildren(DWORD Instance) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	vector<int> Children;
	for (DWORD i = *(DWORD*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		Children.push_back(*(DWORD*)i);
	}
	return Children;
}

DWORD findFirstClass(DWORD Instance, string ClassName) {
	vector<int> InstanceChildren = getChildren(Instance);
	for (int c = 0; c < InstanceChildren.size(); c++) {
		if (getClassName(InstanceChildren[c]) == ClassName) {
			return InstanceChildren[c];
		}
	}
	return 0;
}

bool Commands() {
	"test123";
}

void ChatCommands(string Message);

int GetChatMessage(int luaState) {
	Disable_Callcheck();
	const char* chatMsg = tostring(luaState, 1);
	vector<string> prefix = SplitString(chatMsg, ';');
	for (int a = 0; a < prefix.size(); a++) {
		ChatCommands(prefix[a]);
	}
	return 0;
}

vector<int> GetPlayers(string Name) {
	vector<int> P;
	vector<string> Arguments = SplitString(Name, ',');

	//snip
}

vector<int> GetPlayers2(string Name) {
	//snip
}

void ChatCommands(string Message) {
	vector<string> Arguments = SplitString(Message, ' '); //Chathook commands function

	if (Arguments.size() > 0) {

		if (Arguments.at(0) == "ff" || Arguments.at(0) == "Ff") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, Player.c_str());
				pushstring(State, "LockFirstPerson");
				setfield(State, -2, "CameraMode");
			}
		}

		if (Arguments.at(0) == "ThirdPerson" || Arguments.at(0) == "thirdperson") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, Player.c_str());
				pushstring(State, "Classic");
				setfield(State, -2, "CameraMode");
			}
		}

		if (Arguments.at(0) == "sparkles" || Arguments.at(0) == "Sparkles" || Arguments.at(0) == "sp" || Arguments.at(0) == "Sp") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

				string wat = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=" + Arguments.at(2);
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

		if (Arguments.at(0) == "ws" || Arguments.at(0) == "Ws" || Arguments.at(0) == "speed" || Arguments.at(0) == "Speed" || Arguments.at(0) == "walkspeed" || Arguments.at(0) == "Walkspeed") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			vector<int> Players2 = GetPlayers(Arguments.at(2));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);

				for (int i = 0; i < Players2.size(); i++) {
					string PlayerName2 = getName(Players2[i]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (DWORD a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);
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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

				string duck1 = "rbxassetid://9419831";
				string duck2 = "rbxassetid://9419827";
				string noTshit = "rbxassetid://1";

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

				//getglobal(State, "game");
				//getfield(State, -1, "Workspace");
				//getfield(State, -1, PlayerName.c_str());
				//getfield(State, -1, "Shirt Graphic");
				//pushstring(State, noTshit.c_str());
				//setfield(State, -2, "Graphic");
			}
		}


		if (Arguments.at(0) == "crash" || Arguments.at(0) == "Crash") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, PlayerName.c_str());
				getfield(State, -1, "Character");
				getfield(State, -1, "Humanoid");
				pushnumber(State, HUGE * 0);
				setfield(State, -2, "HipHeight");
				clearstack(State);
			}
		}

		if (Arguments.at(0) == "unlock" || Arguments.at(0) == "Unlock") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);

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

		if (Arguments.at(0) == "watch" || Arguments.at(0) == "Watch") {
			vector<int> Players = GetPlayers2(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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

		if (Arguments.at(0) == "unwatch" || Arguments.at(0) == "Unwatch") {
			vector<int> Players = GetPlayers2(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);

				string lol = "rbxassetid://" + Arguments.at(2);
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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);
				string asg = "http://www.roblox.com/asset/?id=" + Arguments.at(2);
				string p = asg;
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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);
				string asg = "http://www.roblox.com/asset/?id=" + Arguments.at(2);
				string p = asg;
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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);

				string Stat = Arguments.at(2);
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



		if (Arguments.at(0) == "admin") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, PlayerName.c_str());
				getfield(State, -1, "Chatted");
				getfield(State, -1, "connect");
				pushvalue(State, -2);
				pushcfunction(State, GetChatMessage);
				pcall(State, 2, 0, 0);
				clearstack(State);

				//	game.Players:Chat("System48 rules!")

			}
		}

		if (Arguments.at(0) == "bigfire") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int i = 0; i < Players.size(); i++) {
				string PlayerName = getName(Players[i]);

				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, PlayerName.c_str());
				getfield(State, -1, "Character");
				getfield(State, -1, "HumanoidRootPart");

				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "Fire");
				pushvalue(State, -4);
				pcall(State, 2, 1, 0);

				pushnumber(State, 30);
				setfield(State, -2, "Size");
				pcall(State, 1, 0, 0);
				clearstack(State);
			}
		}

		if (Arguments.at(0) == "chat" || Arguments.at(0) == "Chat") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int c = 0; c < Players.size(); c++) {
				string PlayerName = getName(Players[c]);
				string Text = Arguments.at(2);

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
				pushstring(State, Message.substr(Arguments.at(0).length() + Arguments.at(1).length() + 2).c_str());
				pushstring(State, "Blue");
				pcall(State, 4, 0, 0);
				clearstack(State);
			}
		}

		if (Arguments.at(0) == "dialog" || Arguments.at(0) == "Dialog") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);
				string Text = Arguments.at(2);
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
			vector<int> Wrkspace = getChildren(Workspace);
			for (int c = 0; c < Wrkspace.size(); c++) {
				string Children = getName(Wrkspace[c]);

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
			vector<int> Wrkspace = getChildren(Workspace);
			for (int c = 0; c < Wrkspace.size(); c++) {
				string Children = getName(Wrkspace[c]);

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
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

				string wat = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=1";
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

		if (Arguments.at(0) == "noob" || Arguments.at(0) == "Noob") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string Player = getName(Players[a]);

				string wat = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=59977935";
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

		if (Arguments.at(0) == "sethealth" || Arguments.at(0) == "Sethealth") {
			vector<int> Players = GetPlayers(Arguments.at(1));
			for (int a = 0; a < Players.size(); a++) {
				string PlayerName = getName(Players[a]);
				float Value = atof(Arguments.at(2).c_str());
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, PlayerName.c_str());
				getfield(State, -1, "Character");
				getfield(State, -1, "Humanoid");
				pushnumber(State, Value);
				setfield(State, -2, "MaxHealth");
				clearstack(State);
			}
		}



	}
}

void Kill(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "BreakJoints");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}
}

void ForceField(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "ForceField");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);

		}
	}
}

void UnForceField(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "ForceField");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);

		}
	}
}

void SecretGod(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, 9999999999);
			setfield(State, -2, "MaxHealth");
			clearstack(State);

		}
	}
}

void Watch(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Humanoid");

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, "CurrentCamera");
			pushvalue(State, -4);
			setfield(State, -2, "CameraSubject");
			clearstack(State);
		}
	}
}

void UnWatch(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);
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
}

void Bighead(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushnumber(State, 0);
			setfield(State, -2, "Shape");
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}
}

void UnGodMode(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, 100);
			setfield(State, -2, "MaxHealth");
			clearstack(State);
		}
	}
}

bool AddSparkles = true;

void Sparkles(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			if (AddSparkles == true) {
				getglobal(State, "game");
				getfield(State, -1, "Workspace");
				getfield(State, -1, NameEntered.c_str());
				getfield(State, -1, "Torso");
				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "Sparkles");
				pushvalue(State, -4);
				pcall(State, 2, 0, 0);
				clearstack(State);
				AddSparkles = false;
			}
			else {
				getglobal(State, "game");
				getfield(State, -1, "Workspace");
				getfield(State, -1, NameEntered.c_str());
				getfield(State, -1, "Torso");
				getfield(State, -1, "Sparkles");
				getfield(State, -1, "Destroy");
				pushvalue(State, -2);
				pcall(State, 1, 0, 0);
				clearstack(State);
				AddSparkles = true;
			}
		}
	}
}

void UnSparkles(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Torso");
			getfield(State, -1, "Sparkles");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}
}

void Bring(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getfield(State, -1, "CFrame");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "HumanoidRootPart");
			pushvalue(State, -6);
			setfield(State, -2, "CFrame");

			cout << "\nTeleported " << NameEntered.c_str() << " to LocalPlayer." << endl;
		}
	}
}

void apocSetStatPath(string statname, string nextpath, int value) {
	getglobal(State, "game");
	getfield(State, -1, "Players");
	getfield(State, -1, "LocalPlayer");
	getfield(State, -1, "playerstats");
	getfield(State, -1, statname.c_str());
	getfield(State, -1, nextpath.c_str());
	pushnumber(State, value);
	setfield(State, -2, "Value");
	clearstack(State);

}

void GoTo(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
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

			cout << "\nTeleported LocalPlayer to " << NameEntered.c_str() << "." << endl;
		}
	}
}

void Invisible(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushnumber(State, 1);
			setfield(State, -2, "Transparency");
			clearstack(State);
		}
	}
}

void Visible(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushnumber(State, 0);
			setfield(State, -2, "Transparency");
			clearstack(State);
		}
	}
}

void Kick(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, HUGE * 0);
			setfield(State, -2, "HipHeight");
			clearstack(State);
		}
	}
}

void Freeze(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushstring(State, "true");
			setfield(State, -2, "Anchored");
			pushnumber(State, 1);
			setfield(State, -2, "Reflectance");
			clearstack(State);
		}
	}
}

void RemoveTools(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");

			getfield(State, -1, "UnequipTools");
			pushvalue(State, -2);
			pcall(State, 1, 1, 0);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Backpack");

			getfield(State, -1, "ClearAllChildren");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}
}

void BuildTools(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 1);
			setfield(State, -2, "BinType");
			clearstack(State);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 2);
			setfield(State, -2, "BinType");
			clearstack(State);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 3);
			setfield(State, -2, "BinType");
			clearstack(State);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
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
}

void Thaw(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Arm");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Arm");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Left Leg");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Right Leg");
			pushstring(State, false);
			setfield(State, -2, "Anchored");
			pushnumber(State, 0);
			setfield(State, -2, "Reflectance");
			clearstack(State);
		}
	}
}

void WalkSpeed(string Command, float NewWalkSpeed) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Humanoid");
			pushnumber(State, NewWalkSpeed);
			setfield(State, -2, "WalkSpeed");
			clearstack(State);
		}
	}
}

void Skybox(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		getglobal(State, "game");
		getfield(State, -1, "Lighting");
		getglobal(State, "Instance");
		getfield(State, -1, "new");
		pushstring(State, "Sky");
		pushvalue(State, -4);
		pcall(State, 2, 1, 0);

		pushstring(State, ("rbxassetid://" + Args.at(0)).c_str());
		setfield(State, -2, "SkyboxBk");

		pushstring(State, ("rbxassetid://" + Args.at(0)).c_str());
		setfield(State, -2, "SkyboxDn");

		pushstring(State, ("rbxassetid://" + Args.at(0)).c_str());
		setfield(State, -2, "SkyboxFt");

		pushstring(State, ("rbxassetid://" + Args.at(0)).c_str());
		setfield(State, -2, "SkyboxLf");

		pushstring(State, ("rbxassetid://" + Args.at(0)).c_str());
		setfield(State, -2, "SkyboxRt");

		pushstring(State, ("rbxassetid://" + Args.at(0)).c_str());
		setfield(State, -2, "SkyboxUp");
		clearstack(State);
	}
}

void ParticleTest(string Command, string ID) {
	vector<string> Arguments = SplitString(Command, ' ');
	if (Arguments.size() > 0) {
		vector<int> PlayerList = GetPlayers(Arguments.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			string lol = "rbxassetid://" + ID;
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
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
}

void StopMusic() {
	getglobal(State, "game");
	getfield(State, -1, "Workspace");
	getfield(State, -1, "System48Music");
	getfield(State, -1, "Destroy");
	pushvalue(State, -2);
	pcall(State, 1, 0, 0);
	clearstack(State);
}

bool FilteringCheck() {
	getglobal(State, "game");
	getfield(State, -1, "Workspace");
	getfield(State, -1, "FilteringEnabled");
	bool Filtering = toboolean(State, -1);
	if (!Filtering) {
		MessageBox(NULL, "\nFilteringEnabled is currently disabled, Changes will be replicated.", "FilteringCheck", MB_OK);
	}
	else {
		MessageBox(NULL, "\nFilteringEnabled is currently Enabled, Changes may not be replicated", "FilteringCheck", MB_OK);
	}
	return Filtering;
	clearstack(State);
}

void JumpPower(string Command, float NewJumpPower) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Humanoid");
			pushnumber(State, NewJumpPower);
			setfield(State, -2, "JumpPower");
			clearstack(State);
		}
	}
}

void Sound(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		getglobal(State, "game");
		getfield(State, -1, "Workspace");
		getglobal(State, "Instance");
		getfield(State, -1, "new");
		pushstring(State, "Sound");
		pushvalue(State, -4);
		pcall(State, 2, 1, 0);

		pushstring(State, ("rbxassetid://" + Args.at(0)).c_str());
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
}

void GodMode(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, HUGE_VAL);
			setfield(State, -2, "MaxHealth");
			clearstack(State);
		}
	}
}

bool AddFire = true;

void Fire(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			if (AddFire == true) {
				getglobal(State, "game");
				getfield(State, -1, "Workspace");
				getfield(State, -1, NameEntered.c_str());
				getfield(State, -1, "Head");
				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "Fire");
				pushvalue(State, -4);
				pcall(State, 2, 0, 0);
				clearstack(State);
				AddFire = false;
			}

			else {
				getglobal(State, "game");
				getfield(State, -1, "Workspace");
				getfield(State, -1, NameEntered.c_str());
				getfield(State, -1, "Head");
				getfield(State, -1, "Fire");
				getfield(State, -1, "Destroy");
				pushvalue(State, -2);
				pcall(State, 1, 0, 0);
				clearstack(State);
				AddFire = true;
			}
		}
	}
}

void UnFire(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Head");
			getfield(State, -1, "Fire");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}
}

bool AddSmoke = true;

void Smoke(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			if (AddSmoke == true) {
				getglobal(State, "game");
				getfield(State, -1, "Workspace");
				getfield(State, -1, NameEntered.c_str());
				getfield(State, -1, "Torso");
				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "Smoke");
				pushvalue(State, -4);
				pcall(State, 2, 0, 0);
				clearstack(State);
				clearstack(State);
				AddSmoke = false;
			}
			else {
				getglobal(State, "game");
				getfield(State, -1, "Workspace");
				getfield(State, -1, NameEntered.c_str());
				getfield(State, -1, "Torso");
				getfield(State, -1, "Smoke");
				getfield(State, -1, "Destroy");
				pushvalue(State, -2);
				pcall(State, 1, 0, 0);
				clearstack(State);
				AddSmoke = true;
			}
		}
	}
}

void FogEnd(float Value) {
	getglobal(State, "game");
	getfield(State, -1, "Lighting");
	pushnumber(State, Value);
	setfield(State, -2, "FogEnd");
	clearstack(State);
}

void UnSmoke(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Torso");
			getfield(State, -1, "Smoke");
			getfield(State, -1, "Destroy");
			pushvalue(State, -2);
			pcall(State, 1, 0, 0);
			clearstack(State);
		}
	}
}

void Explode(string Command) {
	vector<string> Args = SplitString(Command, ' ');
	if (Args.size() > 0) {
		vector<int> PlayerList = GetPlayers(Args.at(0));
		for (DWORD i = 0; i < PlayerList.size(); i++) {
			NameEntered = getName(PlayerList[i]);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Explosion");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);

			getglobal(State, "game");
			getfield(State, -1, "Workspace");
			getfield(State, -1, NameEntered.c_str());
			getfield(State, -1, "Torso");
			getfield(State, -1, "Position");
			setfield(State, -6, "Position");
			clearstack(State);
		}
	}
}

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case KICK:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Kick(src.c_str());
		}
		break;

		case KILL:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Kill(src.c_str());
		}
		break;

		case FORCEFIELD:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			ForceField(src.c_str());
		}
		break;

		case GOD:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			GodMode(src.c_str());
		}
		break;

		case UNGOD:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			UnGodMode(src.c_str());
		}
		break;

		case FREEZE:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Freeze(src.c_str());
		}
		break;

		case UNFREEZE:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Thaw(src.c_str());
		}
		break;

		case RTOOLS:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			RemoveTools(src.c_str());
		}
		break;

		case BTOOLS:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			BuildTools(src.c_str());
		}
		break;

		case UNFORCEFIELD:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			UnForceField(src.c_str());
		}
		break;

		case EXPLODE:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Explode(src.c_str());
		}
		break;

		case SPARKLES:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Sparkles(src.c_str());
		}
		break;

		case UNSPARKLES:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			UnSparkles(src.c_str());
		}
		break;

		case SMOKE:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Smoke(src.c_str());
		}
		break;

		case UNSMOKE:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			UnSmoke(src.c_str());
		}
		break;


		case FIRE:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Fire(src.c_str());
		}
		break;

		case UNFIRE:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			UnFire(src.c_str());
		}
		break;

		case INVISIBLE:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Invisible(src.c_str());
		}
		break;

		case VISIBLE:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Visible(src.c_str());
		}
		break;

		case GOTO:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			GoTo(src.c_str());
		}
		break;

		case BRING:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Bring(src.c_str());
		}
		break;

		case SECRETGOD:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			SecretGod(src.c_str());
		}
		break;

		case UNSECRETGOD:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			UnGodMode(src.c_str());
		}
		break;

		case BIGHEAD:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Bighead(src.c_str());
		}
		break;

		case WATCH:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			Watch(src.c_str());
		}
		break;

		case UNWATCH:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			UnWatch(src.c_str());
		}
		break;


		case WALKSPEED:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			char buffer2[40];
			GetWindowText(WalkSpeedField, buffer2, sizeof(buffer2));
			float speed = atoi(buffer2);

			WalkSpeed(src.c_str(), speed);
		}
		break;

		case SOUND:
		{
			char buffer[40];
			GetWindowText(SoundField, buffer, sizeof(buffer));
			string id = string(buffer);

			Sound(id);
		}
		break;

		case JUMPPOWER:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			char buffer2[40];
			GetWindowText(JumpPowerField, buffer2, sizeof(buffer2));
			float power = atoi(buffer2);

			JumpPower(src.c_str(), power);
		}
		break;

		case FOGEND:
		{
			char buffer[40];
			GetWindowText(FogEndField, buffer, sizeof(buffer));
			float fogend = atoi(buffer);

			FogEnd(fogend);
		}
		break;

		case SKYBOX:
		{
			char buffer[40];
			GetWindowText(SkyboxField, buffer, sizeof(buffer));
			string id = string(buffer);

			Skybox(id);
		}
		break;

		case PARTICLES:
		{
			char buffer[40];
			GetWindowText(NameField, buffer, sizeof(buffer));
			string src = string(buffer);

			char buffer2[40];
			GetWindowText(ParticleField, buffer2, sizeof(buffer2));
			string id = string(buffer2);

			ParticleTest(src.c_str(), id);
		}
		break;

		case TOGGLEDL:
		{
			if (DynamicOn == true) {
				getglobal(State, "game");
				getfield(State, -1, "Lighting");
				pushnil(State);
				setfield(State, -2, "GlobalShadows");
				clearstack(State);
				DynamicOn = false;
			}
			else {
				getglobal(State, "game");
				getfield(State, -1, "Lighting");
				pushtrue(State);
				setfield(State, -2, "GlobalShadows");
				clearstack(State);
				DynamicOn = true;
			}
		}
		break;

		case LOCALGUESTCHAT:
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
			Sleep(150);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Chatted");
			getfield(State, -1, "connect");
			pushvalue(State, -2);
			pushcfunction(State, GetChatMessage);
			pcall(State, 2, 0, 0);
			clearstack(State);

			MessageBox(NULL, "Thanks to Harkinian#1146 for providing System48 with guest chat!", "Loaded.", MB_OK);
		}
		break;

		case LOCALBTOOLS:
		{
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 1);
			setfield(State, -2, "BinType");
			clearstack(State);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 2);
			setfield(State, -2, "BinType");
			clearstack(State);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 3);
			setfield(State, -2, "BinType");
			clearstack(State);

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
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
		break;

		case LOCALFORCEFIELD:
		{
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "ForceField");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);

			cout << "\nGave ForceField to LocalPlayer." << endl;
		}
		break;

		case TIME_DAY:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			pushnumber(State, 12);
			setfield(State, -2, "TimeOfDay");
			clearstack(State);
		}
		break;

		case LOCALGODMODE:
		{
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, HUGE_VAL);
			setfield(State, -2, "MaxHealth");
			clearstack(State);
		}
		break;

		case LOCALHEAL:
		{
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, 100);
			setfield(State, -2, "MaxHealth");
			clearstack(State);
		}
		break;

		case TIME_NIGHT:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			pushnumber(State, 0);
			setfield(State, -2, "TimeOfDay");
			clearstack(State);
		}
		break;

		case REMOVE_FOG:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			pushnumber(State, 10000);
			setfield(State, -2, "FogEnd");
			clearstack(State);
		}
		break;

		case BLACK_FOG:
		{
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
		break;

		case WHITE_FOG:
		{
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
		break;

		case RED_FOG:
		{
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
		break;

		case BLUE_FOG:
		{
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
		break;

		case GREEN_FOG:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			getglobal(State, "Color3");
			getfield(State, -1, "new");
			pushnumber(State, 0);
			pushnumber(State, 128);
			pushnumber(State, 0);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "FogColor");
			clearstack(State);
		}
		break;

		case RAINBOW_FOG:
		{
			do {
				try {
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
					Sleep(2000);

					getglobal(State, "game");
					getfield(State, -1, "Lighting");
					getglobal(State, "Color3");
					getfield(State, -1, "new");
					pushnumber(State, 0);
					pushnumber(State, 128);
					pushnumber(State, 0);
					pcall(State, 3, 1, 0);
					setfield(State, -3, "FogColor");
					clearstack(State);
					Sleep(2000);

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
					Sleep(2000);
				}
				catch (...) {};
			} while (true);
		}

		case APOCBADGES:
		{
			int result = MessageBox(NULL, "Thanks to Aeka#0892 for providing System48 with this function! Are you in Apoc and ready to execute?", "Credit", MB_YESNO);

			if (result == IDYES) {
				apocSetStatPath("PlayerKill", "Aggressive", 1337);
				apocSetStatPath("PlayerKill", "Bandit", 1337);
				apocSetStatPath("PlayerKill", "Defensive", 1337);
				apocSetStatPath("ZombieKill", "Civilian", 1337);
				apocSetStatPath("ZombieKill", "Military", 1337);
			}
			else {
				//do something?
			}
		}
		break;

		case UNANCHORWS:
		{
			vector<int> Wrkspace = getChildren(Workspace);
			for (int c = 0; c < Wrkspace.size(); c++) {
				string Children = getName(Wrkspace[c]);

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
		break;

		case UNLOCKWS:
		{
			vector<int> Wrkspace = getChildren(Workspace);
			for (int c = 0; c < Wrkspace.size(); c++) {
				string Children = getName(Wrkspace[c]);

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
		break;

		case FILTERINGCHECK:
		{
			FilteringCheck();
		}
		break;

		case STOPMUSIC:
		{
			StopMusic();
		}
		break;

		case EXIT:
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}
		break;

		case CREDITS:
		{
			MessageBox(NULL, "AzuLX and Quad Core - Main Coders\nCustomality - Huge Help with System48V3!\nTimberman - Making the whitelist host!\nYou - Purchasing and using System48!\n", "Credits - System48V3", MB_OK);
		}
		break;

		case MINIMIZE:
		{
			ShowWindow(hwnd, SW_MINIMIZE);
		}
		break;

		}
		break;

	case WM_DESTROY:
		ExitThread(0);
		break;

	case WM_QUIT:
		exit(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL RegisterWindowClass(const char* wClassName) {
	WNDCLASSEX nClass;

	nClass.cbSize = sizeof(WNDCLASSEX);
	nClass.style = CS_DBLCLKS;
	nClass.lpfnWndProc = DLLWindowProc;
	nClass.cbClsExtra = 0;
	nClass.cbWndExtra = 0;
	nClass.hInstance = GetModuleHandle(0);
	nClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	nClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	nClass.hCursor = LoadCursor(0, IDC_ARROW);
	nClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	//nClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	nClass.lpszMenuName = "SYSTEM48";
	nClass.lpszClassName = wClassName;

	if (!RegisterClassEx(&nClass))
		return 0;

	return 1;
}

HMENU hMenuPopupFile;
HMENU hMenuPopupLocalPlayer;
HMENU hMenuPopupLighting;
HMENU hMenuPopupTime;
HMENU hMenuPopupDestruction;
HMENU hMenuPopupState;
HMENU hMenuPopupGameSpecific;
HMENU hMenuPopupExtras;

HMENU hMenubar = CreateMenu();
HMENU hMenu = CreateMenu();
HMENU hSubMenu = CreatePopupMenu();

void CreateToolbarMenu(HMENU hhMenu)
{
	hMenuPopupFile = CreatePopupMenu();
	AppendMenu(hMenuPopupFile, MF_STRING, EXIT, TEXT("Exit"));
	AppendMenu(hMenuPopupFile, MF_STRING, CREDITS, TEXT("Credits"));
	AppendMenu(hMenuPopupFile, MF_STRING, MINIMIZE, TEXT("Minimize Gui"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupFile, TEXT("File"));

	HMENU hSubSubMenu = CreatePopupMenu();
	hMenuPopupLighting = CreatePopupMenu();
	HMENU hSubSub2Menu = CreatePopupMenu();
	HMENU hFogSub = CreatePopupMenu();

	hMenuPopupLocalPlayer = CreatePopupMenu();
	AppendMenu(hMenuPopupLocalPlayer, MF_STRING, LOCALGODMODE, TEXT("Godmode"));
	AppendMenu(hMenuPopupLocalPlayer, MF_STRING, LOCALHEAL, TEXT("Heal"));
	AppendMenu(hMenuPopupLocalPlayer, MF_STRING, LOCALFORCEFIELD, TEXT("ForceField"));
	AppendMenu(hMenuPopupLocalPlayer, MF_STRING, LOCALBTOOLS, TEXT("BTools"));
	AppendMenu(hMenuPopupLocalPlayer, MF_STRING, LOCALGUESTCHAT, TEXT("Guest_Chat"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupLocalPlayer, TEXT("LocalPlayer"));


	hMenuPopupLighting = CreatePopupMenu();
	AppendMenu(hMenuPopupLighting, MF_POPUP, (UINT_PTR)hSubSub2Menu, "Set_Time");
	AppendMenu(hSubSub2Menu, MF_STRING, TIME_DAY, "Time_Day");
	AppendMenu(hSubSub2Menu, MF_STRING, TIME_NIGHT, "Time_Night");

	AppendMenu(hMenuPopupLighting, MF_POPUP, (UINT_PTR)hFogSub, "FogColor");
	AppendMenu(hFogSub, MF_STRING, BLACK_FOG, "FG_Black");
	AppendMenu(hFogSub, MF_STRING, WHITE_FOG, "FG_White");
	AppendMenu(hFogSub, MF_STRING, RED_FOG, "FG_Red");
	AppendMenu(hFogSub, MF_STRING, GREEN_FOG, "FG_Green");
	AppendMenu(hFogSub, MF_STRING, BLUE_FOG, "FG_Blue");
	AppendMenu(hFogSub, MF_STRING, REMOVE_FOG, "Remove_Fog");
	AppendMenu(hMenuPopupLighting, MF_STRING, TOGGLEDL, "Toggle Dynamic Lighting");
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupLighting, TEXT("Lighting"));

	hMenuPopupDestruction = CreatePopupMenu();
	AppendMenu(hMenuPopupDestruction, MF_POPUP, UNANCHORWS, "UnanchorWS");
	AppendMenu(hMenuPopupDestruction, MF_POPUP, UNLOCKWS, "UnlockWS");
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupDestruction, TEXT("Destructive/Mayhem"));


	hMenuPopupGameSpecific = CreatePopupMenu();
	HMENU hApocSub = CreatePopupMenu();
	AppendMenu(hMenuPopupGameSpecific, MF_POPUP, (UINT_PTR)hApocSub, "Apocalypse Rising");
	AppendMenu(hApocSub, MF_STRING, APOCBADGES, "All InGame Badges + Stat Change");

	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupGameSpecific, TEXT("GameSpecific Hacks"));

	hMenuPopupExtras = CreatePopupMenu();

	AppendMenu(hMenuPopupExtras, MF_STRING, FILTERINGCHECK, "FilteringCheck");
	AppendMenu(hMenuPopupExtras, MF_STRING, STOPMUSIC, "Stop Sound");
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupExtras, TEXT("Extras"));






}

HMENU CreateDLLWindowMenu()
{
	HMENU heyMenu;
	heyMenu = CreateMenu();

	if (heyMenu == NULL)
		return FALSE;

	CreateToolbarMenu(heyMenu);
	return heyMenu;
}

BOOL StartMessageLoop() {
	MSG msg;
	BOOL bRet;

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == 0) {
			return 0;
		}
		else if (bRet == -1)
		{
			return 0;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void InterfaceCommands() {

	HFONT TextFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI");

	/*
	KEY:
	(second number)
	+25 for a button under a button
	+50 for a new value based command
	(first number)
	+106 for a new row of buttons
	*/

	HWND KickExecute = CreateWindow("BUTTON", "Kick Plr", WS_VISIBLE | WS_CHILD, 283, 35, 95, 24, MainWindow, (HMENU)KICK, HInstance, 0);

	HWND ForceFieldExecute = CreateWindow("BUTTON", "FF Plr", WS_VISIBLE | WS_CHILD, 283, 60, 95, 24, MainWindow, (HMENU)FORCEFIELD, HInstance, 0);

	HWND UnForceFieldExecute = CreateWindow("BUTTON", "UNFF Plr", WS_VISIBLE | WS_CHILD, 283, 85, 95, 24, MainWindow, (HMENU)UNFORCEFIELD, HInstance, 0);

	HWND ExplodeExecute = CreateWindow("BUTTON", "Xplode Plr", WS_VISIBLE | WS_CHILD, 283, 110, 95, 24, MainWindow, (HMENU)EXPLODE, HInstance, 0);

	HWND KillExecute = CreateWindow("BUTTON", "Kill Plr", WS_VISIBLE | WS_CHILD, 283, 135, 95, 24, MainWindow, (HMENU)KILL, HInstance, 0);

	HWND TPToYouExecute = CreateWindow("BUTTON", "TP Plr->You", WS_VISIBLE | WS_CHILD, 283, 160, 95, 24, MainWindow, (HMENU)BRING, HInstance, 0);

	HWND TPPlayerToUExecute = CreateWindow("BUTTON", "TP You->Plr", WS_VISIBLE | WS_CHILD, 283, 185, 95, 24, MainWindow, (HMENU)GOTO, HInstance, 0);

	HWND InvisibleExecute = CreateWindow("BUTTON", "Plr->Invisible", WS_VISIBLE | WS_CHILD, 283, 210, 95, 24, MainWindow, (HMENU)INVISIBLE, HInstance, 0);

	HWND VisibleExecute = CreateWindow("BUTTON", "Plr->Visible", WS_VISIBLE | WS_CHILD, 283, 235, 95, 24, MainWindow, (HMENU)VISIBLE, HInstance, 0);

	HWND GodExecute = CreateWindow("BUTTON", "Plr->God", WS_VISIBLE | WS_CHILD, 283, 260, 95, 24, MainWindow, (HMENU)GOD, HInstance, 0);

	HWND UnGodExecute = CreateWindow("BUTTON", "Plr->UnGod", WS_VISIBLE | WS_CHILD, 283, 285, 95, 24, MainWindow, (HMENU)UNGOD, HInstance, 0);

	//new row

	HWND FreezeExecute = CreateWindow("BUTTON", "Freeze Plr", WS_VISIBLE | WS_CHILD, 389, 35, 95, 24, MainWindow, (HMENU)FREEZE, HInstance, 0);

	HWND ThawExecute = CreateWindow("BUTTON", "Thaw Plr", WS_VISIBLE | WS_CHILD, 389, 60, 95, 24, MainWindow, (HMENU)UNFREEZE, HInstance, 0);

	HWND RToolsExecute = CreateWindow("BUTTON", "RemoveTools", WS_VISIBLE | WS_CHILD, 389, 85, 95, 24, MainWindow, (HMENU)RTOOLS, HInstance, 0);

	HWND BtoolsExecute = CreateWindow("BUTTON", "BuildTools", WS_VISIBLE | WS_CHILD, 389, 110, 95, 24, MainWindow, (HMENU)BTOOLS, HInstance, 0);

	HWND SparklesExecute = CreateWindow("BUTTON", "+/- Sparkles", WS_VISIBLE | WS_CHILD, 389, 135, 95, 24, MainWindow, (HMENU)SPARKLES, HInstance, 0);

	HWND FireExecute = CreateWindow("BUTTON", "+/- Fire", WS_VISIBLE | WS_CHILD, 389, 160, 95, 24, MainWindow, (HMENU)FIRE, HInstance, 0);

	HWND SmokeExecute = CreateWindow("BUTTON", "+/- Smoke", WS_VISIBLE | WS_CHILD, 389, 185, 95, 24, MainWindow, (HMENU)SMOKE, HInstance, 0);

	HWND CrashExecute = CreateWindow("BUTTON", "Crash Plr", WS_VISIBLE | WS_CHILD, 389, 210, 95, 24, MainWindow, (HMENU)KICK, HInstance, 0);

	HWND SecretGodExecute = CreateWindow("BUTTON", "Plr->SGod", WS_VISIBLE | WS_CHILD, 389, 235, 95, 24, MainWindow, (HMENU)SECRETGOD, HInstance, 0);

	HWND UnSecretGodExecute = CreateWindow("BUTTON", "Plr->UnSGod", WS_VISIBLE | WS_CHILD, 389, 260, 95, 24, MainWindow, (HMENU)UNSECRETGOD, HInstance, 0);

	HWND BigHeadExecute = CreateWindow("BUTTON", "Plr->BigHead", WS_VISIBLE | WS_CHILD, 389, 285, 95, 24, MainWindow, (HMENU)BIGHEAD, HInstance, 0);


	//new row

	HWND WatchExecute = CreateWindow("BUTTON", "Watch->Plr", WS_VISIBLE | WS_CHILD, 495, 35, 95, 24, MainWindow, (HMENU)WATCH, HInstance, 0);

	HWND UnWatchExecute = CreateWindow("BUTTON", "UnWatch->Plr", WS_VISIBLE | WS_CHILD, 495, 60, 95, 24, MainWindow, (HMENU)UNWATCH, HInstance, 0);


	//value commands

	HWND WalkSpeedLabel = CreateWindow("STATIC", "WalkSpeed", WS_VISIBLE | WS_CHILD, 1, 13, 100, 20, MainWindow, NULL, HInstance, NULL);
	WalkSpeedField = CreateWindowEx(0, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 0, 35, 130, 25, MainWindow, 0, HInstance, 0);
	HWND WalkSpeedExecute = CreateWindow("BUTTON", "Change", WS_VISIBLE | WS_CHILD, 128, 35, 75, 24, MainWindow, (HMENU)WALKSPEED, HInstance, 0);

	HWND JumpPowerLabel = CreateWindow("STATIC", "JumpPower", WS_VISIBLE | WS_CHILD, 1, 63, 100, 20, MainWindow, NULL, HInstance, NULL);
	JumpPowerField = CreateWindowEx(0, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 0, 85, 130, 25, MainWindow, 0, HInstance, 0);
	HWND JumpPowerExecute = CreateWindow("BUTTON", "Change", WS_VISIBLE | WS_CHILD, 128, 85, 75, 24, MainWindow, (HMENU)JUMPPOWER, HInstance, 0);

	HWND FogEndLabel = CreateWindow("STATIC", "FogEnd", WS_VISIBLE | WS_CHILD, 1, 113, 100, 20, MainWindow, NULL, HInstance, NULL);
	FogEndField = CreateWindowEx(0, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 0, 135, 130, 25, MainWindow, 0, HInstance, 0);
	HWND FogEndExecute = CreateWindow("BUTTON", "Change", WS_VISIBLE | WS_CHILD, 128, 135, 75, 24, MainWindow, (HMENU)FOGEND, HInstance, 0);

	HWND SoundLabel = CreateWindow("STATIC", "SoundId", WS_VISIBLE | WS_CHILD, 1, 163, 100, 20, MainWindow, NULL, HInstance, NULL);
	SoundField = CreateWindowEx(0, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 0, 185, 130, 25, MainWindow, 0, HInstance, 0);
	HWND SoundExecute = CreateWindow("BUTTON", "Insert", WS_VISIBLE | WS_CHILD, 128, 185, 75, 24, MainWindow, (HMENU)SOUND, HInstance, 0);

	HWND SkyboxLabel = CreateWindow("STATIC", "SkyboxId", WS_VISIBLE | WS_CHILD, 1, 213, 100, 20, MainWindow, NULL, HInstance, NULL);
	SkyboxField = CreateWindowEx(0, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 0, 235, 130, 25, MainWindow, 0, HInstance, 0);
	HWND SkyboxExecute = CreateWindow("BUTTON", "Insert", WS_VISIBLE | WS_CHILD, 128, 235, 75, 24, MainWindow, (HMENU)SKYBOX, HInstance, 0);

	HWND ParticleLabel = CreateWindow("STATIC", "ParticlesId", WS_VISIBLE | WS_CHILD, 1, 263, 100, 20, MainWindow, NULL, HInstance, NULL);
	ParticleField = CreateWindowEx(0, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 0, 285, 130, 25, MainWindow, 0, HInstance, 0);
	HWND ParticleExecute = CreateWindow("BUTTON", "Insert", WS_VISIBLE | WS_CHILD, 128, 285, 75, 24, MainWindow, (HMENU)PARTICLES, HInstance, 0);
}

BOOL CreateInterface() {
	HFONT ConsolasFontCustom = CreateFont(16, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Consolas");

	LoadLibrary("Riched20.dll");

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&icex);

	HInstance = GetModuleHandle(0);
	HMENU hMenu = CreateDLLWindowMenu();

	UnregisterClass("SYSTEM48", HInstance);
	RegisterWindowClass("SYSTEM48");
	MainWindow = CreateWindowEx(0, "SYSTEM48", "System48 Pro " CLIENT_VERSION " - AzuLX & Quad Core", WS_BORDER | WS_OVERLAPPED, 100, 100, 600, 366, 0, hMenu, HInstance, 0);

	HWND NameFieldLabel = CreateWindow("STATIC", "Enter Player:", WS_VISIBLE | WS_CHILD, 283, 3, 130, 20, MainWindow, NULL, HInstance, NULL);
	NameField = CreateWindowEx(0, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER, 373, 0, 130, 25, MainWindow, 0, HInstance, 0);

	InterfaceCommands();

	SetWindowPos(MainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(MainWindow, SW_SHOWNORMAL);

	SendMessage(NameField, WM_SETFONT, (WPARAM)ConsolasFontCustom, 0);
	SendMessage(WalkSpeedField, WM_SETFONT, (WPARAM)ConsolasFontCustom, 0);
	SendMessage(JumpPowerField, WM_SETFONT, (WPARAM)ConsolasFontCustom, 0);
	SendMessage(FogEndField, WM_SETFONT, (WPARAM)ConsolasFontCustom, 0);
	SendMessage(SoundField, WM_SETFONT, (WPARAM)ConsolasFontCustom, 0);
	SendMessage(SkyboxField, WM_SETFONT, (WPARAM)ConsolasFontCustom, 0);
	SendMessage(ParticleField, WM_SETFONT, (WPARAM)ConsolasFontCustom, 0);

	return StartMessageLoop();
}

string GetMachineID()
{

	string ss;
	ss = "Err_StringIsNu";
	UCHAR szFileSys[255],
		szVolNameBuff[255];
	DWORD dwSerial;
	DWORD dwMFL;
	DWORD dwSysFlags;
	int error = 0;

	bool success = GetVolumeInformation(LPCTSTR("C:\\"), (LPTSTR)szVolNameBuff,
		255, &dwSerial,
		&dwMFL, &dwSysFlags,
		(LPTSTR)szFileSys,
		255);
	if (!success) {
		ss = "Err_Not_Elevated";
	}
	stringstream errorStream;
	errorStream << dwSerial;
	return string(errorStream.str().c_str());
}

string replaceAll(string subject, const string& search,
	const string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

string DownloadURL(string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

static std::string base64_decode(const std::string &in) {
	std::string out;

	std::vector<int> T(256, -1);
	for (int i = 0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

	int val = 0, valb = -8;
	for (unsigned char c : in) {
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}

	return out;
}

void execute(string asd) {
	vector<string> Arguments = SplitString(asd, ' ');

	if (Arguments.at(0) == "ff") {
		getglobal(State, "game");
		getfield(State, -1, "Workspace");
		getfield(State, -1, Arguments.at(1).c_str());
		getglobal(State, "Instance");
		getfield(State, -1, "new");
		pushstring(State, "ForceField");
		pushvalue(State, -4);
		pcall(State, 2, 0, 0);
		clearstack(State);
	}
}

int LocalPlayerChatted(DWORD luaState) {
	string chatMsg = tostring(luaState, 1);

	if (chatMsg.substr(0, 1) == ";") {
		chatMsg = chatMsg.substr(1, string::npos);

		execute(chatMsg);
	}

	return 0;
}

int InitilizeSystem48() {
	VMProtectBeginUltra("SysFunc: Initializing Method");
	int VFTable = ScanAddress(SCRIPTCONTEXT);
	DWORD* ScriptContext = (DWORD*)Memory::Scan(PAGE_READWRITE, (char*)&VFTable, "xxxx");

	DataModel = getParent((DWORD)ScriptContext);

	Players = findFirstClass(DataModel, "Players");
	Workspace = findFirstClass(DataModel, "Workspace");
	LocalPlayer = *(DWORD*)(Players + 0x154);
	State = newthread(GetGlobalState(ScriptContext, 1));


	getglobal(State, "game");
	getfield(State, -1, "Players");
	getfield(State, -1, "LocalPlayer");
	getfield(State, -1, "Chatted");
	getfield(State, -1, "connect");
	pushvalue(State, -2);
	pushcfunction(State, GetChatMessage);
	pcall(State, 2, 0, 0);
	clearstack(State);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	VMProtectEnd();
}

void HotKeys() {
	do {
		if (GetAsyncKeyState(VK_F2)) {
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
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
			getfield(State, -1, "LocalPlayer");
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
			getfield(State, -1, "LocalPlayer");
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
			getfield(State, -1, "LocalPlayer");
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
		else if (GetAsyncKeyState(VK_F1)) {
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Humanoid");
			pushnumber(State, HUGE_VAL);
			setfield(State, -2, "MaxHealth");
			clearstack(State);
		}
		else if (GetAsyncKeyState(VK_F3)) {
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "ForceField");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
		else if (GetAsyncKeyState(VK_F4)) {
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Sparkles");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
		else if (GetAsyncKeyState(VK_F5)) {
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Head");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Fire");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
		else if (GetAsyncKeyState(VK_F6)) {
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "ParticleEmitter");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 75);
			setfield(State, -2, "VelocitySpread");
			clearstack(State);
		}
		else if (GetAsyncKeyState(VK_F7)) {
			string PlayerName = "LocalPlayer";

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
		else if (GetAsyncKeyState(VK_F8)) {
			string PlayerName = "LocalPlayer";

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
		else if (GetAsyncKeyState(VK_F9)) {
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "Torso");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "Smoke");
			pushvalue(State, -4);
			pcall(State, 2, 0, 0);
			clearstack(State);
		}
		Sleep(100);
	} while (true);
}

void Whitelist() {
	InitilizeSystem48();
}

int Init() {
	Whitelist();
    #if DEBUG_MODE;
    #else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
    #endif
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CreateInterface, 0, 0, 0);
	HotKeys();
	return 0;
}

BOOLEAN DllMain(HINSTANCE Dll, DWORD Reason, LPVOID Reserved) {
	if (Reason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, 0);
	}
	return TRUE;
}
