#include <vector>
#include <string>

#include "blackoutlua.h"
#include "util.h"

using namespace std;
using namespace rbx_Instance;

/* - Command list - */

const char* Commands[] = {
	"\n[X] ff [p] - Gives forcefield to [p].\n"
	"[X] unff [p] - Removes forcefield from [p].\n"
	"[X] sparkles [p] - Gives sparkles to [p].\n"
	"[X] unsparkles [p] - Removes sparkles from [p].\n"
	"[X] fire [p] - Gives fire to [p].\n"
	"[X] unfire [p] - Removes sparkles from [p].\n"
	"[X] smoke [p] - Gives smoke to [p].\n"
	"[X] unsmoke [p] - Removes smoke from [p].\n"
	"[X] btools [p] - Gives building tools to [p].\n"
	"[X] charapp [p] [id] - Changes [p]'s appearance to [id].\n"
	"[X] uncharapp [p] - Changes [p]'s appearance back to their default appearance.\n"
	"[X] ws [p] [n] - Changes [p]'s walkspeed to [n].\n"
	"[X] jp [p] [n] - Changes [p]'s jumppower to [n].\n"
	"[X] lockfp [p] - Locks [p]'s camera to first person.\n"
	"[X] unlockfp [p] - Unlocks [p]'s camera from first person.\n"
	"[X] skybox [id] - Sets skybox to [id].\n"
	"[X] rskybox - Removes all skyboxes in workspace.\n"
	//"[X] music [id] - Plays [id] in workspace.\n"
	"[X] stopmusic - Stops all music playing in workspace.\n"
	//"[X] noob [p] - Sets [p]'s appearance to a noob.\n"
	//"[X] unnoob [p] - Sets [p]'s appearance back to normal.\n"
	"[X] god [p] - Sets [p]'s maxhealth to a high value.\n"
	"[X] ungod [p] - Sets [p]'s maxhealth to the default value.\n"
	"[X] nojump [p] - Doesn't allow [p] to jump.\n"
	"[X] rejump [p] - Allows [p] to jump.\n"
	"[X] kill [p] - Kills [p].\n"
	"[X] rh [p] - Removes [p]'s humanoid.\n"
	"[X] punish [p] - Removes [p]'s character.\n"
	"[X] unpunish [p] - Returns [p]'s character.\n"
	"[X] time [n] - Sets time to [n] hours.\n"
	"[X] fog [n] - Sets fogend to [n].\n"
	"[X] freeze [p] - Anchors [p] and sets reflectance to 1.\n"
	"[X] thaw [p] - Unanchors [p] and sets reflectance to 0.\n"
	"[X] invis [p] - Sets [p]'s transparency to 1.\n"
	"[X] vis [p] - Sets [p]'s transparency to 0.\n"
	"[X] change [p] [s] [v] - Changes [p]'s stat [s] to value [v].\n"
	"[X] bighead [p] - Makes [p]'s head big.\n"
	"[X] peanuthead [p] - Makes [p]'s head small.\n"
	"[X] particles [p] [id] - Gives [p] a particle emitter and sets texture to [id].\n"
	"[X] unparticles [p] - Removes [p]'s particle emitter.\n"
	"[X] tp [p1] [p2] - Teleports [p1] to [p2].\n"
	"[X] to [p] - Teleports localplayer to [p].\n"
	"[X] bring [p] - Teleports [p] to localplayer.\n"
	//"[X] crash [p] - Freezes [p]'s client.\n"
	//"[X] lock [p] - Locks [p]'s character parts for building tools.\n"
	//"[X] unlock [p] - Unlocks [p]'s character parts for building tools.\n"
	"[X] shutdown - Removes all players from the server.\n"
	"[X] light [p] - Gives [p] a light on their torso.\n"
	"[X] unlight [p] - Removes the light from [p]'s torso.\n"
	"[X] bigfire [p] - Gives fire to [p] with increased size.\n"
	"[X] unbigfire [p] - Removes [p]'s bigfire.\n"
	"[X] chat [p] [s] - Makes [p] chat [s].\n"
	"[X] rtools [p] - Unequips and clears tools from [p]'s backpack.\n"
	"[X] stun [p] - Platformstands [p].\n"
	"[X] rshirt [p] - Removes [p]'s shirt.\n"
	"[X] rpants [p] - Removes [p]'s pants.\n"
	"[X] explode [p] - Explodes [p].\n"
	"[X] watch [p] - Forces you to watch [p].\n"
	"[X] unwatch [p] - Forces you to unwatch [p].\n"
	"[X] clone [p] - Clones [p] and parent sets to workspace.\n"
	"[X] name [p] [s] - Names [p]'s character to [s].\n"
	"[X] duck [p] - Turns [p] into a duck.\n"
	"[X] hidename [p] - Hides [p]'s name and health bar.\n"
	"[X] showname [p] - Shows [p]'s name and health bar.\n"
	"[X] fling [p] - Flings [p] off the map.\n"
	"[X] respawn [p] - Respawns [p].\n"
	"[X] baseplate - Creates a new baseplate in workspace.\n"
	"[X] unanchorws - Unanchors all parts in workspace.\n"
	"[X] unlockws - Unlocks all parts in workspace.\n"
	//"[X] tptool - Gives localplayer a teleport tool.\n"
	"[X] fogcolor [r] [g] [b] - Sets the fogcolor to [r] [g] [b].\n"
	"[X] ambient [r] [g] [b] - Sets the ambient to [r] [g] [b].\n"

	"\n\nGame specific commands:\n"
	"\n[X] money [p] [n] - Sets [p]'s money on driveblox ultimate to [n].\n"
	"[X] fencingreach - Reach hax on fencing.\n"
	//"[X] finish [map] - Finishes [map] on bhop.\n"
	//"[X] apocstats - Gives localplayer full hunger and thirst on apoc.\n"
};

/* - Function to interact with all players in the current game instance - */

vector<INT> GetAllPlayers(string Name) {
	vector<INT> ReturnList;
	vector<string> Arguments = SplitString(Name, ',');

	for (INT c = 0; c < Arguments.size(); c++) {
		string Input = Arguments[c];

		if (Input == "me") {
			ReturnList.push_back(LocalPlayer);
		}
		else if (Input == "all") {
			ReturnList = rbx_GetChildren(Players);
		}
		else if (Input == "others") {
			vector<INT> Plrs = rbx_GetChildren(Players);
			for (INT i = 0; i < Plrs.size(); i++) {
				if (rbx_GetName(Plrs[i]) != rbx_GetName(LocalPlayer)) {
					ReturnList.push_back(Plrs[i]);
				}
			}
		}
		else {
			vector<INT> Plrs = rbx_GetChildren(Players);
			for (INT i = 0; i < Plrs.size(); ++i)
				if (strstr((rbx_GetName(Plrs[i])).c_str(), Input.data()))
					ReturnList.push_back(Plrs[i]);
		}
	}
	return ReturnList;
}

/* - Function to handle command input - */

VOID HandleCommands(vector<string> Arguments) {
	string Input = Arguments.at(0); // Ah.
	vector<INT> Players;

	if (Arguments.size() > 1) { // Fixes the issue of crashing when commands don't have more than 1 argument, somewhat disgusting
		Players = GetAllPlayers(Arguments.at(1));
	}

	for (INT c = 0; c < Players.size(); c++) {
		string Player = rbx_GetName(Players[c]);

		if (Input == "ff") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "ForceField");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "unff") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "ForceField");
			rbx_lua_getfield(rbx_L, -1, "Remove");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "sparkles" | Input == "sp") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "Sparkles");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "unsparkles" | Input == "unsp") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getfield(rbx_L, -1, "Sparkles");
			rbx_lua_getfield(rbx_L, -1, "Remove");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "fire") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "Fire");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "unfire") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_getfield(rbx_L, -1, "Fire");
			rbx_lua_getfield(rbx_L, -1, "Remove");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "smoke") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "Smoke");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "unsmoke") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getfield(rbx_L, -1, "Smoke");
			rbx_lua_getfield(rbx_L, -1, "Remove");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "btools") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Backpack");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "HopperBin");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "BinType");
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Backpack");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "HopperBin");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_pushnumber(rbx_L, 2);
			rbx_lua_setfield(rbx_L, -2, "BinType");
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Backpack");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "HopperBin");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_pushnumber(rbx_L, 3);
			rbx_lua_setfield(rbx_L, -2, "BinType");
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Backpack");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "HopperBin");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_pushnumber(rbx_L, 4);
			rbx_lua_setfield(rbx_L, -2, "BinType");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "charapp" | Input == "char") {
			string UserID = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=" + Arguments.at(2);
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_pushstring(rbx_L, UserID.c_str());
			rbx_lua_setfield(rbx_L, -2, "CharacterAppearance");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "BreakJoints");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "uncharapp" | Input == "unchar") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "userId");
			rbx_lua_pushvalue(rbx_L, -3);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_pushvalue(rbx_L, -3);
			rbx_lua_setfield(rbx_L, -2, "CharacterAppearance");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "BreakJoints");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "ws" | Input == "speed" | Input == "walkspeed") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushnumber(rbx_L, atoi(Arguments.at(2).c_str()));
			rbx_lua_setfield(rbx_L, -2, "WalkSpeed");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "jp" | Input == "jpower" | Input == "jumppower") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushnumber(rbx_L, atoi(Arguments.at(2).c_str()));
			rbx_lua_setfield(rbx_L, -2, "JumpPower");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "lockfp") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "CameraMode");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "unlockfp") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "CameraMode");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "noob") {

		}

		if (Input == "unnoob") {

		}

		if (Input == "god") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushnumber(rbx_L, HUGE_VAL);
			rbx_lua_setfield(rbx_L, -2, "MaxHealth");
			rbx_lua_pushnumber(rbx_L, HUGE_VAL);
			rbx_lua_setfield(rbx_L, -2, "Health");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "ungod") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushnumber(rbx_L, 100);
			rbx_lua_setfield(rbx_L, -2, "MaxHealth");
			rbx_lua_pushnumber(rbx_L, 100);
			rbx_lua_setfield(rbx_L, -2, "Health");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "nojump") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "JumpPower");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "rejump") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushnumber(rbx_L, 50);
			rbx_lua_setfield(rbx_L, -2, "JumpPower");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "kill") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "BreakJoints");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "rh" | Input == "rhum") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_getfield(rbx_L, -1, "Remove");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "punish") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "Lighting");
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "Players");
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_pushvalue(rbx_L, -5);
			rbx_lua_setfield(rbx_L, -2, "Parent");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "unpunish") {
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "Players");
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_pushvalue(rbx_L, -5);
			rbx_lua_setfield(rbx_L, -2, "Parent");
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "Players");
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "MakeJoints");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "freeze" | Input == "frz") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_getfield(rbx_L, -1, "Right Arm");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -6);
			rbx_lua_getfield(rbx_L, -1, "Left Arm");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -8);
			rbx_lua_getfield(rbx_L, -1, "Right Leg");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -10);
			rbx_lua_getfield(rbx_L, -1, "Left Leg");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "thaw" | Input == "unfreeze" | Input == "unfrz") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_pushboolean(rbx_L, false);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_pushboolean(rbx_L, false);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_getfield(rbx_L, -1, "Right Arm");
			rbx_lua_pushboolean(rbx_L, false);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -6);
			rbx_lua_getfield(rbx_L, -1, "Left Arm");
			rbx_lua_pushboolean(rbx_L, false);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -8);
			rbx_lua_getfield(rbx_L, -1, "Right Leg");
			rbx_lua_pushboolean(rbx_L, false);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_pushvalue(rbx_L, -10);
			rbx_lua_getfield(rbx_L, -1, "Left Leg");
			rbx_lua_pushboolean(rbx_L, false);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Reflectance");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "invis") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_getfield(rbx_L, -1, "Right Arm");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -6);
			rbx_lua_getfield(rbx_L, -1, "Left Arm");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -8);
			rbx_lua_getfield(rbx_L, -1, "Right Leg");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -10);
			rbx_lua_getfield(rbx_L, -1, "Left Leg");
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "vis") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_getfield(rbx_L, -1, "Right Arm");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -6);
			rbx_lua_getfield(rbx_L, -1, "Left Arm");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -8);
			rbx_lua_getfield(rbx_L, -1, "Right Leg");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_pushvalue(rbx_L, -10);
			rbx_lua_getfield(rbx_L, -1, "Left Leg");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_setfield(rbx_L, -2, "Transparency");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "change" | Input == "changestat" | Input == "stats") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "leaderstats");
			rbx_lua_getfield(rbx_L, -1, Arguments.at(2).c_str());
			rbx_lua_pushstring(rbx_L, Arguments.at(3).c_str());
			rbx_lua_setfield(rbx_L, -2, "Value");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "bighead") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_getfield(rbx_L, -1, "Mesh");
			rbx_lua_getglobal(rbx_L, "Vector3");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushnumber(rbx_L, 3.75);
			rbx_lua_pushnumber(rbx_L, 3.75);
			rbx_lua_pushnumber(rbx_L, 3.75);
			rbx_lua_pcall(rbx_L, 3, 1, 0);
			rbx_lua_setfield(rbx_L, -3, "Scale");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "peanuthead") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_getfield(rbx_L, -1, "Mesh");
			rbx_lua_getglobal(rbx_L, "Vector3");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushnumber(rbx_L, 0.55);
			rbx_lua_pushnumber(rbx_L, 0.55);
			rbx_lua_pushnumber(rbx_L, 0.55);
			rbx_lua_pcall(rbx_L, 3, 1, 0);
			rbx_lua_setfield(rbx_L, -3, "Scale");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "particles") {
			if (Arguments.size() == 2) {
				rbx_lua_getglobal(rbx_L, "game");
				rbx_lua_getfield(rbx_L, -1, "GetService");
				rbx_lua_pushvalue(rbx_L, -2);
				rbx_lua_pushstring(rbx_L, "Players");
				rbx_lua_pcall(rbx_L, 2, 1, 0);
				rbx_lua_getfield(rbx_L, -1, Player.c_str());
				rbx_lua_getfield(rbx_L, -1, "Character");
				rbx_lua_getfield(rbx_L, -1, "Torso");
				rbx_lua_getglobal(rbx_L, "Instance");
				rbx_lua_getfield(rbx_L, -1, "new");
				rbx_lua_pushstring(rbx_L, "ParticleEmitter");
				rbx_lua_pushvalue(rbx_L, -4);
				rbx_lua_pcall(rbx_L, 2, 1, 0);
				rbx_lua_pushnumber(rbx_L, 75);
				rbx_lua_setfield(rbx_L, -2, "VelocitySpread");
				rbx_lua_emptystack(rbx_L);
			}
			else if (Arguments.size() > 2) {
				string ID = "rbxassetid://" + Arguments.at(2);
				rbx_lua_getglobal(rbx_L, "game");
				rbx_lua_getfield(rbx_L, -1, "GetService");
				rbx_lua_pushvalue(rbx_L, -2);
				rbx_lua_pushstring(rbx_L, "Players");
				rbx_lua_pcall(rbx_L, 2, 1, 0);
				rbx_lua_getfield(rbx_L, -1, Player.c_str());
				rbx_lua_getfield(rbx_L, -1, "Character");
				rbx_lua_getfield(rbx_L, -1, "Torso");
				rbx_lua_getglobal(rbx_L, "Instance");
				rbx_lua_getfield(rbx_L, -1, "new");
				rbx_lua_pushstring(rbx_L, "ParticleEmitter");
				rbx_lua_pushvalue(rbx_L, -4);
				rbx_lua_pcall(rbx_L, 2, 1, 0);
				rbx_lua_pushstring(rbx_L, ID.c_str());
				rbx_lua_setfield(rbx_L, -2, "Texture");
				rbx_lua_pushnumber(rbx_L, 75);
				rbx_lua_setfield(rbx_L, -2, "VelocitySpread");
				rbx_lua_emptystack(rbx_L);
			}
		}

		if (Input == "unparticles" | Input == "rparticles" | Input == "removeparticles") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getfield(rbx_L, -1, "ParticleEmitter");
			rbx_lua_getfield(rbx_L, -1, "Remove");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "tp" | Input == "teleport") {
			vector<INT> Players2 = GetAllPlayers(Arguments.at(2));
			for (INT c = 0; c < Players2.size(); c++) {
				string Player2 = rbx_GetName(Players2[c]);
				rbx_lua_getglobal(rbx_L, "game");
				rbx_lua_getfield(rbx_L, -1, "GetService");
				rbx_lua_pushvalue(rbx_L, -2);
				rbx_lua_pushstring(rbx_L, "Players");
				rbx_lua_pcall(rbx_L, 2, 1, 0);
				rbx_lua_getfield(rbx_L, -1, Player2.c_str());
				rbx_lua_getfield(rbx_L, -1, "Character");
				rbx_lua_getfield(rbx_L, -1, "Torso");
				rbx_lua_getfield(rbx_L, -1, "CFrame");
				rbx_lua_pushvalue(rbx_L, -5);
				rbx_lua_getfield(rbx_L, -1, Player.c_str());
				rbx_lua_getfield(rbx_L, -1, "Character");
				rbx_lua_getfield(rbx_L, -1, "HumanoidRootPart");
				rbx_lua_pushvalue(rbx_L, -5);
				rbx_lua_setfield(rbx_L, -2, "CFrame");
				rbx_lua_emptystack(rbx_L);
			}
		}

		if (Input == "to" | Input == "goto") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getfield(rbx_L, -1, "CFrame");
			rbx_lua_pushvalue(rbx_L, -5);
			rbx_lua_getfield(rbx_L, -1, "LocalPlayer");
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "HumanoidRootPart");
			rbx_lua_pushvalue(rbx_L, -5);
			rbx_lua_setfield(rbx_L, -2, "CFrame");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "bring") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, "LocalPlayer");
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getfield(rbx_L, -1, "CFrame");
			rbx_lua_pushvalue(rbx_L, -5);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "HumanoidRootPart");
			rbx_lua_pushvalue(rbx_L, -5);
			rbx_lua_setfield(rbx_L, -2, "CFrame");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "crash") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushnumber(rbx_L, HUGE_VAL * 0);
			rbx_lua_setfield(rbx_L, -2, "HipHeight");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "lock") {

		}

		if (Input == "unlock") {

		}

		if (Input == "light") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "SpotLight");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_pushnumber(rbx_L, 1000);
			rbx_lua_setfield(rbx_L, -2, "Brightness");
			rbx_lua_pushnumber(rbx_L, 30);
			rbx_lua_setfield(rbx_L, -2, "Range");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "unlight" | Input == "rlight" | Input == "removelight") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getfield(rbx_L, -1, "SpotLight");
			rbx_lua_getfield(rbx_L, -1, "Destroy");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "bigfire") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "Fire");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_pushnumber(rbx_L, 30);
			rbx_lua_setfield(rbx_L, -2, "Size");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "unbigfire" | Input == "rbigfire" | Input == "removebigfire") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getfield(rbx_L, -1, "Fire");
			rbx_lua_getfield(rbx_L, -1, "Destroy");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "chat") {
			string Message;
			for (int a = 2; a < Arguments.size(); a++) { // disgusting
				if (a < (Arguments.size() - 1)) {
					Message.append(Arguments.at(a) + ' ');
				}
				if (a == (Arguments.size() - 1)) {
					Message.append(Arguments.at(a));
				}
			}
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "Chat");
			rbx_lua_getfield(rbx_L, -1, "Chat");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushvalue(rbx_L, -5);
			rbx_lua_pushstring(rbx_L, Message.c_str());
			rbx_lua_pushstring(rbx_L, "Blue");
			rbx_lua_pcall(rbx_L, 4, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "rtools" | Input == "removetools") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_getfield(rbx_L, -1, "UnequipTools");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Backpack");
			rbx_lua_getfield(rbx_L, -1, "ClearAllChildren");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "stun") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "PlatformStand");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "rshirt" | Input == "removeshirt") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Shirt");
			rbx_lua_getfield(rbx_L, -1, "Destroy");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "rpants" | Input == "removepants") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Pants");
			rbx_lua_getfield(rbx_L, -1, "Destroy");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "explode") {
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "Explosion");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getfield(rbx_L, -1, "Position");
			rbx_lua_setfield(rbx_L, -7, "Position");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "watch" | Input == "view") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getfield(rbx_L, -1, "CurrentCamera");
			rbx_lua_pushvalue(rbx_L, -3);
			rbx_lua_setfield(rbx_L, -2, "CameraSubject");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "unwatch" | Input == "unview") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, "LocalPlayer");
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getfield(rbx_L, -1, "CurrentCamera");
			rbx_lua_pushvalue(rbx_L, -3);
			rbx_lua_setfield(rbx_L, -2, "CameraSubject");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "clone") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "Archivable");
			rbx_lua_getfield(rbx_L, -1, "Clone");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 1, 0);
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_setfield(rbx_L, -2, "Parent");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "name") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_pushstring(rbx_L, Arguments.at(2).c_str());
			rbx_lua_setfield(rbx_L, -2, "Name");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "duck") {
			string duck1 = "rbxassetid://9419831"; // Mesh
			string duck2 = "rbxassetid://9419827"; // Texture
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_getfield(rbx_L, -1, "Mesh");
			rbx_lua_getfield(rbx_L, -1, "Destroy");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "SpecialMesh");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_pushstring(rbx_L, duck1.c_str());
			rbx_lua_setfield(rbx_L, -2, "MeshId");
			rbx_lua_pushstring(rbx_L, duck2.c_str());
			rbx_lua_setfield(rbx_L, -2, "TextureId");
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_getfield(rbx_L, -1, "face");
			rbx_lua_getfield(rbx_L, -1, "Destroy");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pcall(rbx_L, 1, 0, 0);
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Head");
			rbx_lua_getfield(rbx_L, -1, "Mesh");
			rbx_lua_getglobal(rbx_L, "Vector3");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushnumber(rbx_L, 9);
			rbx_lua_pushnumber(rbx_L, 9);
			rbx_lua_pushnumber(rbx_L, 9);
			rbx_lua_pcall(rbx_L, 3, 1, 0);
			rbx_lua_setfield(rbx_L, -3, "Scale");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "hidename") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushstring(rbx_L, "None");
			rbx_lua_setfield(rbx_L, -2, "DisplayDistanceType");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "showname") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushstring(rbx_L, "Viewer");
			rbx_lua_setfield(rbx_L, -2, "DisplayDistanceType");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "fling") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Torso");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "BodyForce");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getglobal(rbx_L, "Vector3");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushnumber(rbx_L, -9999);
			rbx_lua_pushnumber(rbx_L, 9999);
			rbx_lua_pushnumber(rbx_L, -9999);
			rbx_lua_pcall(rbx_L, 3, 1, 0);
			rbx_lua_setfield(rbx_L, -3, "force");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "respawn") {
			rbx_lua_getglobal(rbx_L, "Vector3");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_pushnumber(rbx_L, -1000000);
			rbx_lua_pushnumber(rbx_L, 0);
			rbx_lua_pcall(rbx_L, 3, 1, 0);
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "MoveTo");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushvalue(rbx_L, -7);
			rbx_lua_pcall(rbx_L, 2, 0, 0);
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "baseplate") {
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getglobal(rbx_L, "Instance");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushstring(rbx_L, "Part");
			rbx_lua_pushvalue(rbx_L, -4);
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getglobal(rbx_L, "Vector3");
			rbx_lua_getfield(rbx_L, -1, "new");
			rbx_lua_pushnumber(rbx_L, 1000);
			rbx_lua_pushnumber(rbx_L, 1);
			rbx_lua_pushnumber(rbx_L, 1000);
			rbx_lua_pcall(rbx_L, 3, 1, 0);
			rbx_lua_setfield(rbx_L, -3, "Size");
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getfield(rbx_L, -1, "Part");
			rbx_lua_pushstring(rbx_L, "Blackout_Baseplate");
			rbx_lua_setfield(rbx_L, -2, "Name");
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getfield(rbx_L, -1, "Blackout_Baseplate");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "Anchored");
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getfield(rbx_L, -1, "Blackout_Baseplate");
			rbx_lua_pushboolean(rbx_L, true);
			rbx_lua_setfield(rbx_L, -2, "Locked");
			rbx_lua_emptystack(rbx_L);
		}

		if (Input == "hipheight") {
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Players");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Player.c_str());
			rbx_lua_getfield(rbx_L, -1, "Character");
			rbx_lua_getfield(rbx_L, -1, "Humanoid");
			rbx_lua_pushnumber(rbx_L, atoi(Arguments.at(2).c_str()));
			rbx_lua_setfield(rbx_L, -2, "HipHeight");
			rbx_lua_emptystack(rbx_L);
		}

	}

	// Commands that don't make a call to GetAllPlayers go here

	if (Input == "cmds" | Input == "help" | Input == "commands") {
		print(*Commands);
	}

	if (Input == "skybox" | Input == "sky") {
		rbx_lua_getglobal(rbx_L, "game");
		rbx_lua_getfield(rbx_L, -1, "Lighting");
		rbx_lua_getglobal(rbx_L, "Instance");
		rbx_lua_getfield(rbx_L, -1, "new");
		rbx_lua_pushstring(rbx_L, "Sky");
		rbx_lua_pushvalue(rbx_L, -4);
		rbx_lua_pcall(rbx_L, 2, 1, 0);
		rbx_lua_pushstring(rbx_L, ("rbxassetid://" + Arguments.at(1)).c_str());
		rbx_lua_setfield(rbx_L, -2, "SkyboxBk");
		rbx_lua_pushstring(rbx_L, ("rbxassetid://" + Arguments.at(1)).c_str());
		rbx_lua_setfield(rbx_L, -2, "SkyboxDn");
		rbx_lua_pushstring(rbx_L, ("rbxassetid://" + Arguments.at(1)).c_str());
		rbx_lua_setfield(rbx_L, -2, "SkyboxFt");
		rbx_lua_pushstring(rbx_L, ("rbxassetid://" + Arguments.at(1)).c_str());
		rbx_lua_setfield(rbx_L, -2, "SkyboxLf");
		rbx_lua_pushstring(rbx_L, ("rbxassetid://" + Arguments.at(1)).c_str());
		rbx_lua_setfield(rbx_L, -2, "SkyboxRt");
		rbx_lua_pushstring(rbx_L, ("rbxassetid://" + Arguments.at(1)).c_str());
		rbx_lua_setfield(rbx_L, -2, "SkyboxUp");
		rbx_lua_emptystack(rbx_L);
	}

	if (Input == "rsky" | Input == "rskybox" | Input == "removesky" | Input == "removeskybox") {
		vector<INT> ass = rbx_GetChildren(Lighting);
		for (INT c = 0; c < ass.size(); c++) {
			string Children = rbx_GetName(ass[c]);
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Lighting");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Children.c_str());
			rbx_lua_getfield(rbx_L, -1, "IsA");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Sky");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			if (rbx_lua_toboolean(rbx_L, -1) == true) {
				rbx_lua_pop(rbx_L, 1);
				rbx_lua_getfield(rbx_L, -1, "Remove");
				rbx_lua_pushvalue(rbx_L, -2);
				rbx_lua_pcall(rbx_L, 1, 0, 0);
			}
			rbx_lua_emptystack(rbx_L);
		}
	}

	if (Input == "music") {

	}

	if (Input == "stopmusic") {
		vector<INT> ass = rbx_GetChildren(Workspace);
		for (INT c = 0; c < ass.size(); c++) {
			string Children = rbx_GetName(ass[c]);
			rbx_lua_getglobal(rbx_L, "game");
			rbx_lua_getfield(rbx_L, -1, "GetService");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Workspace");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			rbx_lua_getfield(rbx_L, -1, Children.c_str());
			rbx_lua_getfield(rbx_L, -1, "IsA");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "Sound");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			if (rbx_lua_toboolean(rbx_L, -1) == true) {
				rbx_lua_pop(rbx_L, 1);
				rbx_lua_getfield(rbx_L, -1, "Remove");
				rbx_lua_pushvalue(rbx_L, -2);
				rbx_lua_pcall(rbx_L, 1, 0, 0);
			}
			rbx_lua_emptystack(rbx_L);
		}
	}

	if (Input == "time") {
		string TimeOfDay = Arguments.at(1);
		rbx_lua_getglobal(rbx_L, "game");
		rbx_lua_getfield(rbx_L, -1, "GetService");
		rbx_lua_pushvalue(rbx_L, -2);
		rbx_lua_pushstring(rbx_L, "Lighting");
		rbx_lua_pcall(rbx_L, 2, 1, 0);
		rbx_lua_pushstring(rbx_L, TimeOfDay.c_str());
		rbx_lua_setfield(rbx_L, -2, "TimeOfDay");
		rbx_lua_emptystack(rbx_L);
	}

	if (Input == "fog") {
		double FogEnd = atof(Arguments.at(1).c_str());
		rbx_lua_getglobal(rbx_L, "game");
		rbx_lua_getfield(rbx_L, -1, "GetService");
		rbx_lua_pushvalue(rbx_L, -2);
		rbx_lua_pushstring(rbx_L, "Lighting");
		rbx_lua_pcall(rbx_L, 2, 1, 0);
		rbx_lua_pushnumber(rbx_L, FogEnd);
		rbx_lua_setfield(rbx_L, -2, "FogEnd");
		rbx_lua_emptystack(rbx_L);
	}

	if (Input == "shutdown") {
		rbx_lua_getglobal(rbx_L, "game");
		rbx_lua_getfield(rbx_L, -1, "GetService");
		rbx_lua_pushvalue(rbx_L, -2);
		rbx_lua_pushstring(rbx_L, "Workspace");
		rbx_lua_pcall(rbx_L, 2, 1, 0);
		rbx_lua_pushnumber(rbx_L, HUGE_VAL * 0);
		rbx_lua_setfield(rbx_L, -2, "Gravity");
		rbx_lua_emptystack(rbx_L);
	}

	if (Input == "finishrace") { // Vehicle simulator, untested
		rbx_lua_getglobal(rbx_L, "game");
		rbx_lua_getfield(rbx_L, -1, "ReplicatedStorage");
		rbx_lua_getfield(rbx_L, -1, "RaceSystem");
		rbx_lua_getfield(rbx_L, -1, "finish_race");
		rbx_lua_getfield(rbx_L, -1, "FireServer");
		rbx_lua_pushvalue(rbx_L, -2);
		rbx_lua_pushstring(rbx_L, "Quarter Mile");
		rbx_lua_pushnumber(rbx_L, 3.5);
		rbx_lua_pcall(rbx_L, 3, 1, 0);
		rbx_lua_emptystack(rbx_L);
	}

	if (Input == "unanchorws") {
		vector<INT> ass = rbx_GetChildren(Workspace);
		for (INT c = 0; c < ass.size(); c++) {
			string Children = rbx_GetName(ass[c]);
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getfield(rbx_L, -1, Children.c_str());
			rbx_lua_getfield(rbx_L, -1, "IsA");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "BasePart");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			if (rbx_lua_toboolean(rbx_L, -1) == true) {
				rbx_lua_pop(rbx_L, 1);
				rbx_lua_pushboolean(rbx_L, false);
				rbx_lua_setfield(rbx_L, -2, "Anchored");
			}
			rbx_lua_emptystack(rbx_L);
		}
	}

	if (Input == "unlockws") {
		vector<INT> ass = rbx_GetChildren(Workspace);
		for (INT c = 0; c < ass.size(); c++) {
			string Children = rbx_GetName(ass[c]);
			rbx_lua_getglobal(rbx_L, "Workspace");
			rbx_lua_getfield(rbx_L, -1, Children.c_str());
			rbx_lua_getfield(rbx_L, -1, "IsA");
			rbx_lua_pushvalue(rbx_L, -2);
			rbx_lua_pushstring(rbx_L, "BasePart");
			rbx_lua_pcall(rbx_L, 2, 1, 0);
			if (rbx_lua_toboolean(rbx_L, -1) == true) {
				rbx_lua_pop(rbx_L, 1);
				rbx_lua_pushboolean(rbx_L, false);
				rbx_lua_setfield(rbx_L, -2, "Locked");
			}
			rbx_lua_emptystack(rbx_L);
		}
	}

	if (Input == "fogcolor") {
		rbx_lua_getglobal(rbx_L, "Color3");
		rbx_lua_getfield(rbx_L, -1, "new");
		rbx_lua_pushnumber(rbx_L, atof(Arguments.at(1).c_str()));
		rbx_lua_pushnumber(rbx_L, atof(Arguments.at(2).c_str()));
		rbx_lua_pushnumber(rbx_L, atof(Arguments.at(3).c_str()));
		rbx_lua_pcall(rbx_L, 3, 1, 0);
		rbx_lua_getglobal(rbx_L, "game");
		rbx_lua_getfield(rbx_L, -1, "Lighting");
		rbx_lua_pushvalue(rbx_L, -3);
		rbx_lua_setfield(rbx_L, -2, "FogColor");
		rbx_lua_emptystack(rbx_L);
	}

	if (Input == "ambient") {
		rbx_lua_getglobal(rbx_L, "Color3");
		rbx_lua_getfield(rbx_L, -1, "new");
		rbx_lua_pushnumber(rbx_L, atof(Arguments.at(1).c_str()));
		rbx_lua_pushnumber(rbx_L, atof(Arguments.at(2).c_str()));
		rbx_lua_pushnumber(rbx_L, atof(Arguments.at(3).c_str()));
		rbx_lua_pcall(rbx_L, 3, 1, 0);
		rbx_lua_getglobal(rbx_L, "game");
		rbx_lua_getfield(rbx_L, -1, "Lighting");
		rbx_lua_pushvalue(rbx_L, -3);
		rbx_lua_setfield(rbx_L, -2, "Ambient");
		rbx_lua_emptystack(rbx_L);
	}

	if (Input == "fencingreach") {
		rbx_lua_getglobal(rbx_L, "game");
		rbx_lua_getfield(rbx_L, -1, "Players");
		rbx_lua_getfield(rbx_L, -1, "LocalPlayer");
		rbx_lua_getfield(rbx_L, -1, "Backpack");
		rbx_lua_getfield(rbx_L, -1, "Foil");
		rbx_lua_getfield(rbx_L, -1, "Handle");
		rbx_lua_getglobal(rbx_L, "Instance");
		rbx_lua_getfield(rbx_L, -1, "new");
		rbx_lua_pushstring(rbx_L, "SelectionBox");
		rbx_lua_pushvalue(rbx_L, -4);
		rbx_lua_pcall(rbx_L, 2, 1, 0);
		rbx_lua_pushvalue(rbx_L, -3);
		rbx_lua_setfield(rbx_L, -2, "Adornee");
		rbx_lua_pushvalue(rbx_L, -3);
		rbx_lua_getglobal(rbx_L, "Vector3");
		rbx_lua_getfield(rbx_L, -1, "new");
		rbx_lua_pushnumber(rbx_L, 1);
		rbx_lua_pushnumber(rbx_L, 1);
		rbx_lua_pushnumber(rbx_L, 30);
		rbx_lua_pcall(rbx_L, 3, 1, 0);
		rbx_lua_setfield(rbx_L, -3, "Size");
		rbx_lua_emptystack(rbx_L);
	}

}