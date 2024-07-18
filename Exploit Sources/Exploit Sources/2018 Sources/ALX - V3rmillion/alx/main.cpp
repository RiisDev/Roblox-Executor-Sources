#include "main.h"
#include "helper.h"
#include "crc64.h"
#include "aobscan.h"
#include "form.h"

using namespace std;

DWORD ROBLOX_BASE;
DWORD ALX_BASE;
DWORD GameInstance;
DWORD LocalPlayer;
string LocalPlayerName;
BOOLEAN IsGameBlacklisted = false;

RLua* rLua;
YAML::Node ConfigFile;

DWORD CurrentMusicInstance = 0;

const char* COMMAND_LIST[] = {
	"kill [p] - Kills [p].\r\n",
	"ff [p] - Gives a forcefield to [p].\r\n",
	"unff [p] - Removes all forcefields from [p].\r\n",
	"cmds - Lists all available commands.\r\n",
	"sethealth [p] [n] - Sets [p]'s health to [n].\r\n",
	"setmaxhealth [p] [n] - Sets [p]'s max health to [n].\r\n",
	"god [p] - Gods [p].\r\n",
	"ungod [p] - Ungods [p].\r\n",
	"tp [p] [p] - Teleports [p] to [p].\r\n",
	"music [id] - Plays [id].\r\n",
	"btools [p] - Gives btools to [p].\r\n",
	"playerdist [p] - Displays a list of players ([p]) and their distances from the localplayer.\r\n",
	"kick [p] - Removes [p] from the game.\r\n",
	"ws [p] [n] - Sets [p]'s walkspeed to [n].\r\n",
	"invisible [p] - Makes [p] invisible.\r\n",
	"visible [p] - Makes [p] visible.\r\n",
	"charapp [p] [n] - Sets [p]'s appearance to another player whos id is [n].\r\n",
	"fire [p], unfire [p] - Gives [p] fire, Removes fire from [p].\r\n",
	"smoke [p], unsmoke [p] - Gives [p] smoke, Removes smoke from [p].\r\n",
	"sparkles [p], unsparkles [p] - Gives [p] sparkles, Removes sparkles from [p].\r\n",
	"tpwp [p] [s] - Teleports [p] to waypoint [wpname].\r\n",
	"remwp [s] - Removes [s] (Use 'savecfg' to save).\r\n",
	"setwp [s] - Sets a waypoint '[s]' at your coordinates. (Use 'savecfg' to save).\r\n",
	"savecfg - Saves data to the configuration file.\r\n",
	"unlockall - Unlocks everything in Workspace.\r\n",
	"freeze - Freezes [p].\r\n",
	"unfreeze - Thaws [p].\r\n",
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == 1) {
		DisableThreadLibraryCalls(hModule);
		athread begin(Initiate, NULL);
	}
	return 1;
}

bool PEntrySort(PEntry i, PEntry j) { return (i.pDistance) < (j.pDistance); }

void HandleCommand_r(const string& command);
void HandleCommand(string command) {
	try {
		HandleCommand_r(command);
	}
	catch (...) {
		ConsoleOutput("Oops, I crashed when trying to execute the command '%s'.\r\n", command.c_str());
	}
}

void HandleCommand_r(const string& command) {
	if (!IsGameBlacklisted) {
		vector<string> args = GetArguments(command);
		vector<DWORD> players = GetPlayersFromMessage(GameInstance, args);
		string commandName = args.front(); args.erase(args.begin());

		ConsoleOutput("\r\n>%s\r\n", command.c_str());

		if (commandName == "kill") {
			float newHealth = 0;
			for (int i = 0; i < players.size(); ++i)
			if (int pChar = rLua->getCharacterFromPlayer(players[i]))
			if (int pHum = rLua->getChildByName(pChar, "Humanoid"))
				rLua->setProperty(pHum, "Health", "float", &newHealth);
		}
		else if (commandName == "test") {
			char* sList[3] = { "Workspace", "Axstin", "Head" };
			int pHead = rLua->getInstanceInHierarchy(GameInstance, sList, 3);
			if (pHead) {
				CFrame* headCF = rLua->getObjectCFrame(pHead);
				headCF->position.y = headCF->position.y + 10;
				float newVal = 0.5;
				for (int i = 0; i < 500; ++i) {
					int nInst = createInstance(*rLua, "Part", rLua->getChildByName(GameInstance, "Workspace"));
					rLua->setObjectCFrame(nInst, headCF);
					rLua->setProperty(nInst, "Transparency", "float", &newVal);
					Sleep(100);
				}
			}
		}
		else if (commandName == "ff") {
			for (int i = 0; i < players.size(); ++i)
			if (int pChar = rLua->getCharacterFromPlayer(players[i]))
				createInstance(*rLua, "ForceField", pChar);
		}
		else if (commandName == "unff") {
			for (int i = 0; i < players.size(); ++i) {
				if (int pChar = rLua->getCharacterFromPlayer(players[i])) {
					vector<DWORD> pCharChildren = rLua->getChildren(pChar);
					for (int j = 0; j < pCharChildren.size(); j++)
					if (rLua->getClassName(pCharChildren[j]) == "ForceField")
						rLua->destroyInstance(pCharChildren[j]);
				}
			}
		}
		else if (commandName == "cmds") {
			ConsoleOutput("\r\n");
			for (int i = 0; i < _countof(COMMAND_LIST); ++i)
				ConsoleOutput("%s", COMMAND_LIST[i]);
			ConsoleOutput("\r\n");
		}
		else if (commandName == "sethealth") {
			float newHealth = 0;

			try {
				newHealth = stod(args.back());
			}
			catch (std::invalid_argument e) {
				newHealth = -1;
			}

			args.pop_back();
			players = GetPlayersFromMessage(GameInstance, args);

			if (newHealth != -1)
			for (int i = 0; i < players.size(); ++i)
			if (int pChar = rLua->getCharacterFromPlayer(players[i]))
			if (int pHum = rLua->getChildByName(pChar, "Humanoid"))
				rLua->setProperty(pHum, "Health", "float", &newHealth);
		}
		else if (commandName == "ws") {
			float newWS = 0;

			try {
				newWS = stod(args.back());
			}
			catch (std::invalid_argument e) {
				newWS = -1;
			}

			args.pop_back();
			players = GetPlayersFromMessage(GameInstance, args);

			if (newWS != -1)
			for (int i = 0; i < players.size(); ++i)
			if (int pChar = rLua->getCharacterFromPlayer(players[i]))
			if (int pHum = rLua->getChildByName(pChar, "Humanoid"))
				rLua->setProperty(pHum, "WalkSpeed", "float", &newWS);
		}
		else if (commandName == "setmaxhealth") {
			float newHealth = 0;

			try {
				newHealth = stod(args.back());
			}
			catch (std::invalid_argument e) {
				newHealth = -1;
			}

			args.pop_back();
			players = GetPlayersFromMessage(GameInstance, args);

			if (newHealth != -1)
			for (int i = 0; i < players.size(); ++i)
			if (int pChar = rLua->getCharacterFromPlayer(players[i]))
			if (int pHum = rLua->getChildByName(pChar, "Humanoid"))
				rLua->setProperty(pHum, "MaxHealth", "float", &newHealth);
		}
		else if (commandName == "god") {
			float newHealth = NAN;
			for (int i = 0; i < players.size(); ++i)
			if (int pChar = rLua->getCharacterFromPlayer(players[i]))
			if (int pHum = rLua->getChildByName(pChar, "Humanoid"))
				rLua->setProperty(pHum, "MaxHealth", "float", &newHealth);
		}
		else if (commandName == "ungod") {
			float newHealth = 100;
			for (int i = 0; i < players.size(); ++i)
			if (int pChar = rLua->getCharacterFromPlayer(players[i]))
			if (int pHum = rLua->getChildByName(pChar, "Humanoid"))
				rLua->setProperty(pHum, "MaxHealth", "float", &newHealth);
		}
		else if (commandName == "tp") {
			if (args.size() > 1) {
				vector<string> targetSL;
				targetSL.push_back(args.back());

				vector<DWORD> targetL = GetPlayersFromMessage(GameInstance, targetSL);
				if (targetL[0] != NULL) {
					args.pop_back();
					players = GetPlayersFromMessage(GameInstance, args);
					DWORD target = targetL[0];

					target = rLua->getCharacterFromPlayer(target);
					if (target) {
						target = rLua->getChildByName(target, "HumanoidRootPart");
						if (target) {
							CFrame* targetCF = rLua->getObjectCFrame(target);

							for (int i = 0; i < players.size(); ++i)
							if (int pChar = rLua->getCharacterFromPlayer(players[i]))
							if (int pHRT = rLua->getChildByName(pChar, "HumanoidRootPart")) {
								rLua->setObjectCFrame(pHRT, targetCF);
							}
						}
					}
				}
				else {
					ConsoleOutput("Target '%s' doesn't exist!\r\n", args.back());
				}
			}
			else {
				ConsoleOutput("Invalid number of arguments.\r\n");
			}
		}
		else if (commandName == "music") {
			int workSpace = rLua->getChildByName(GameInstance, "Workspace");
			if (workSpace) {
				string fullID = "rbxassetid://" + args.back();
				bool isLooped = 1;

				int sound = rLua->getChildByName(workSpace, "amusic");
				if (!sound) {
					sound = createInstance(*rLua, "Sound", workSpace);
					rLua->setProperty(sound, "Name", "string", "amusic");
					rLua->setProperty(sound, "Looped", "bool", &isLooped);
				}

				rLua->stopSoundInstance((void*)sound);
				rLua->setProperty(sound, "SoundId", "string", (void*)fullID.c_str());
				rLua->playSoundInstance((void*)sound);
			}
		}
		else if (commandName == "kick") {
			for (int i = 0; i < players.size(); ++i)
				rLua->destroyInstance(players[i]);
		}
		else if (commandName == "btools") {
			for (int i = 0; i < players.size(); ++i) {
				if (int pBackpack = rLua->getChildByName(players[i], "Backpack")) {
					for (int j = 0; j < 4; ++j) {
						int nTool = createInstance(*rLua, "HopperBin", pBackpack);
						int hopType = j + 1;
						rLua->setProperty(nTool, "BinType", "enum", &hopType);
					}
					Sleep(50);
				}
			}
		}
		else if (commandName == "playerdist") {
			ConsoleOutput("\r\n");

			vector<PEntry> pEntries;

			if (int lChar = rLua->getCharacterFromPlayer(LocalPlayer)) {
				if (int lHRT = rLua->getChildByName(lChar, "HumanoidRootPart")) {
					Vector3 lpPos = rLua->getObjectCFrame(lHRT)->position;

					for (int i = 0; i < players.size(); ++i) {
						if (players[i] == LocalPlayer) continue;

						if (int pChar = rLua->getCharacterFromPlayer(players[i])) {
							if (int pHRT = rLua->getChildByName(pChar, "HumanoidRootPart")) {
								Vector3 pPos = rLua->getObjectCFrame(pHRT)->position;

								float x, y, z;
								x = abs(pPos.x - lpPos.x); x *= x;
								y = abs(pPos.y - lpPos.y); y *= y;
								z = abs(pPos.z - lpPos.z); z *= z;

								x = sqrt(x + y + z);

								PEntry pEntry;
								pEntry.pName = rLua->getName(players[i]);
								pEntry.pDistance = x;

								pEntries.push_back(pEntry);
							}
						}
					}

					sort(pEntries.begin(), pEntries.end(), PEntrySort);

					for (int i = 0; i < pEntries.size(); ++i) {
						ConsoleOutput("%s - %f\r\n", (*pEntries[i].pName).c_str(), pEntries[i].pDistance);
					}
				}
				else { ConsoleOutput("Failed to get LocalPlayer's position.\r\n"); }
			}
			else { ConsoleOutput("Failed to get LocalPlayer's character.\r\n"); }

			ConsoleOutput("\r\n");
		}
		else if (commandName == "invisible") {
			float newTrans = 1;
			for (int i = 0; i < players.size(); ++i) {
				if (int pChar = rLua->getCharacterFromPlayer(players[i])) {
					vector<DWORD> pCharChildren = rLua->getChildren(pChar);
					for (int j = 0; j < pCharChildren.size(); ++j) {
						if (rLua->getClassName(pCharChildren[j]) == "Part") {
							rLua->setProperty(pCharChildren[j], "Transparency", "float", &newTrans);
						}
					}
				}
			}
		}
		else if (commandName == "visible") {
			float newTrans = 0;
			for (int i = 0; i < players.size(); ++i) {
				if (int pChar = rLua->getCharacterFromPlayer(players[i])) {
					vector<DWORD> pCharChildren = rLua->getChildren(pChar);
					for (int j = 0; j < pCharChildren.size(); ++j) {
						if (rLua->getClassName(pCharChildren[j]) == "Part" && *rLua->getName(pCharChildren[j]) != "HumanoidRootPart") {
							rLua->setProperty(pCharChildren[j], "Transparency", "float", &newTrans);
						}
					}
				}
			}
		}
		else if (commandName == "charapp") {
			string id = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=" + args.back(); args.pop_back();
			players = GetPlayersFromMessage(GameInstance, args);
			for (int i = 0; i < players.size(); ++i) {
				rLua->setProperty(players[i], "CharacterAppearance", "string", (void*)id.c_str());
			}
		}
		else if (commandName == "fire") {
			for (int i = 0; i < players.size(); ++i) {
				DWORD character = rLua->getCharacterFromPlayer(players[i]);
				if (character) {
					DWORD HRT = rLua->getChildByName(character, "HumanoidRootPart");
					if (HRT) {
						createInstance(*rLua, "Fire", HRT);
					}
				}
			}
		}
		else if (commandName == "smoke") {
			for (int i = 0; i < players.size(); ++i) {
				DWORD character = rLua->getCharacterFromPlayer(players[i]);
				if (character) {
					DWORD HRT = rLua->getChildByName(character, "HumanoidRootPart");
					if (HRT) {
						createInstance(*rLua, "Smoke", HRT);
					}
				}
			}
		}
		else if (commandName == "sparkles") {
			for (int i = 0; i < players.size(); ++i) {
				DWORD character = rLua->getCharacterFromPlayer(players[i]);
				if (character) {
					DWORD HRT = rLua->getChildByName(character, "HumanoidRootPart");
					if (HRT) {
						createInstance(*rLua, "Sparkles", HRT);
					}
				}
			}
		}
		else if (commandName == "unfire") {
			for (int i = 0; i < players.size(); ++i) {
				DWORD character = rLua->getCharacterFromPlayer(players[i]);
				if (character) {
					DWORD HRT = rLua->getChildByName(character, "HumanoidRootPart");
					if (HRT) {
						rLua->clearInstanceByClass(HRT, "Fire");
					}
				}
			}
		}
		else if (commandName == "unsmoke") {
			for (int i = 0; i < players.size(); ++i) {
				DWORD character = rLua->getCharacterFromPlayer(players[i]);
				if (character) {
					DWORD HRT = rLua->getChildByName(character, "HumanoidRootPart");
					if (HRT) {
						rLua->clearInstanceByClass(HRT, "Smoke");
					}
				}
			}
		}
		else if (commandName == "unsparkles") {
			for (int i = 0; i < players.size(); ++i) {
				DWORD character = rLua->getCharacterFromPlayer(players[i]);
				if (character) {
					DWORD HRT = rLua->getChildByName(character, "HumanoidRootPart");
					if (HRT) {
						rLua->clearInstanceByClass(HRT, "Sparkles");
					}
				}
			}
		}
		else if (commandName == "listchildren") {
			ConsoleOutput("\r\n");
			char* sList[10];

			for (int i = 0; i < 10; ++i) {
				if (i == args.size())
					break;

				sList[i] = (char*)args[i].data();
			}

			DWORD inst = rLua->getInstanceInHierarchy(GameInstance, sList, args.size());

			if (inst) {
				vector<DWORD> children = rLua->getChildren(inst);

				for (int i = 0; i < children.size(); ++i) {
					ConsoleOutput("%X - %s - %s\r\n", children[i], (*rLua->getName(children[i])).c_str(), rLua->getClassName(children[i]).c_str());
				}
			}
			else {
				cout << "error: couldn't find instance.\n";
			}
		}
		else if (commandName == "tpwp") {
			if (ConfigFile != 0) {
				if (args.size() > 1) {
					string waypoint = args.back(); args.pop_back();
					vector<DWORD> people = GetPlayersFromMessage(GameInstance, args);

					if (ConfigFile["waypoints"][waypoint]) {
						YAML::Node selectedWaypoint = ConfigFile["waypoints"][waypoint];
						for (int i = 0; i < people.size(); ++i) {
							DWORD character = rLua->getCharacterFromPlayer(people[i]);
							if (character) {
								DWORD HRT = rLua->getChildByName(character, "HumanoidRootPart");
								if (HRT) {
									rLua->setObjectCFrame(HRT, selectedWaypoint["x"].as<float>(), selectedWaypoint["y"].as<float>(), selectedWaypoint["z"].as<float>());
								}
							}
						}
					}
					else {
						ConsoleOutput("Invalid waypoint.\r\n");
					}
				}
				else {
					ConsoleOutput("Invalid number of arguments.\r\n");
				}
			}
			else {
				ConsoleOutput("This command is unavailable due to failure to load the config file.\r\n");
			}
		}
		else if (commandName == "setwp") {
			if (ConfigFile != 0) {
				if (args.size() == 1) {
					string wpName = args.back();
					if (!ConfigFile["waypoints"][wpName]) {
						DWORD player = rLua->getLocalPlayer(GameInstance);
						if (player) {
							DWORD character = rLua->getCharacterFromPlayer(player);
							if (character) {
								DWORD HRT = rLua->getChildByName(character, "HumanoidRootPart");
								if (HRT) {
									CFrame* hrtCFrame = rLua->getObjectCFrame(HRT);
									if (hrtCFrame) {
										YAML::Node selectedWaypoint = ConfigFile["waypoints"][wpName];
										selectedWaypoint["x"] = hrtCFrame->position.x;
										selectedWaypoint["y"] = hrtCFrame->position.y;
										selectedWaypoint["z"] = hrtCFrame->position.z;

										ConsoleOutput("Waypoint '%s' has been succesfully set at %f, %f, %f.\r\n", wpName.c_str(), hrtCFrame->position.x, hrtCFrame->position.y, hrtCFrame->position.z);
									}
									else { ConsoleOutput("Failed to retrieve Torso position.\r\n"); }
								}
								else { ConsoleOutput("Failed to get your Torso.\r\n"); }
							}
							else {
								ConsoleOutput("Failed to get your character.\r\n");
							}
						}
						else {
							ConsoleOutput("Failed to find localplayer.\r\n");
						}
					}
					else {
						ConsoleOutput("That waypoint already exists. Use 'remwp' to remove.\r\n");
					}
				}
			}
			else {
				ConsoleOutput("This command is unavailable due to failure to load the config file.\r\n");
			}
		}
		else if (commandName == "remwp") {
			if (ConfigFile != 0) {
				if (args.size() > 0) {
					YAML::Node selectedWaypoint = ConfigFile["waypoints"][args[0]];
					if (selectedWaypoint) {
						ConfigFile["waypoints"].remove(args[0]);
						ConsoleOutput("Waypoint successfully removed.\r\n");
					}
					else {
						ConsoleOutput("Invalid waypoint name.\r\n");
					}
				}
			}
			else {
				ConsoleOutput("This command is unavailable due to failure to load the config file.\r\n");
			}
		}
		else if (commandName == "savecfg") {
			SaveConfigurationFile("config.yaml", ConfigFile);
		}
		else if (commandName == "unlockall") {
			rLua->unlockChildren(rLua->getChildByName(GameInstance, "Workspace"));
		}
		else if (commandName == "freeze") {
			byte anchored = 1;
			for (int i = 0; i < players.size(); ++i) {
				DWORD character = rLua->getCharacterFromPlayer(players[i]);
				if (character) {
					DWORD HRT = rLua->getChildByName(character, "Torso");
					if (HRT) {
						rLua->setProperty(HRT, "Anchored", "bool", &anchored);
					}
				}
			}
		}
		else if (commandName == "unfreeze") {
			byte anchored = 0;
			for (int i = 0; i < players.size(); ++i) {
				DWORD character = rLua->getCharacterFromPlayer(players[i]);
				if (character) {
					DWORD HRT = rLua->getChildByName(character, "Torso");
					if (HRT) {
						rLua->setProperty(HRT, "Anchored", "bool", &anchored);
					}
				}
			}
		}
		else {
			ConsoleOutput("Oops, the command '%s' doesn't exist!\r\n", commandName.c_str());
		}
	}
 else {
	 ConsoleOutput("This game has been blacklisted and Alx has been disabled, sorry!\r\n");
 }
}

vector<DWORD> GetPlayersFromMessage(DWORD gameInstance, vector<string> argList) {
	vector<DWORD> rtn;
	string input;

	for (int i = 0; i < argList.size(); ++i) {
		input = argList[i];

		if (input == "me") {
			rtn.push_back(rLua->getLocalPlayer(gameInstance));
		}
		else if (input == "all") {
			rtn = rLua->getPlayers(gameInstance);
		}
		else if (input == "others") {
			vector<DWORD> players = rLua->getPlayers(gameInstance);
			for (int i = 1; i < players.size(); ++i) {
				rtn.push_back(players[i]);
			}
		}
		else {
			vector<DWORD> players = rLua->getPlayers(gameInstance);
			for (int i = 0; i < players.size(); ++i)
			if (strstr((*rLua->getName(players[i])).c_str(), input.data()))
				rtn.push_back(players[i]);
		}
	}
	return rtn;
}

int main() {
	// 83 EC ?? 53 56 8B D9 57 89 5D E4 E8 - game instance
	// 8B 3D ?? ?? ?? ?? 8B D7 8B 47 04 80 78 0D 00 75 - for createinstance
	// look for getcframe
	// look for int, bool, float, double, string funcs
	// look for unksub

	FreeConsole();

	ConsoleOutput("Welcome, %s.\r\n\r\n", LocalPlayerName.c_str());
	ConsoleOutput("Scanning... ");

	try {
		rLua->initiate(GameInstance);
	}
	catch (char* e) {
		ConsoleOutput("%s\r\n", e);
	}

	ScanResults scanResults = rLua->getInitResults();

	if (scanResults.failCount > 0) {
		ConsoleOutput("\r\nFailed to scan %d objects.\r\n", scanResults.failCount);
		for (int i = 0; i < scanResults.failCount; ++i)
			ConsoleOutput("%d, ", scanResults.failList[i]);

		ConsoleOutput("\r\n");
	} 
	else
		ConsoleOutput("OK\r\n");

	ConsoleOutput("Loading configuration file... ");
	BOOL cfgResult = 0;
	try {
		cfgResult = LoadConfigurationFile("config.yaml", ConfigFile);
	}
	catch (YAML::Exception e) {};

	if (cfgResult) {
		ConsoleOutput("OK\r\n");
	}
	else {
		ConsoleOutput("ERROR\r\nSome commands and features may be unavailable.\r\n");
	}

	/*try {
		DWORD placeId = *(DWORD *)(GameInstance + 0x0E14);
		if (placeId == 1600503) {
			ConsoleOutput("This game has been blacklisted and Alx has been disabled, sorry!\r\n");
			IsGameBlacklisted = true;
		}
	}
	catch (...) {

	}*/

	return 0;
}

int Verify() {
	DWORD qID1 = 0, qID2 = 0, qID3 = 0;
	
	//printf("Scanning for Game... ");
	DWORD gameInstancePtr = 0xF7A1F8;
	GameInstance = aobscan::scanWritable((char*)&gameInstancePtr, "xxxx");

	if ((GameInstance != 0) && (*rLua->getName(GameInstance) == "Game")) {
		//printf("OK\nFetching required information... ");
		LocalPlayer = rLua->getLocalPlayer(GameInstance);
		if ((LocalPlayer != 0) && (rLua->getClassName(LocalPlayer) == "Player")) {
			LocalPlayerName = *rLua->getName(LocalPlayer);
			string visitScript = DownloadURL("http://www.roblox.com/game/visit.ashx");
			if (visitScript.size() > 0) {

				SIZE_T idLocation1 = visitScript.find("{ \\\"userId\\\" : ");
				SIZE_T idLocation2 = visitScript.find("CharacterFetch.ashx?userId=");

				if (idLocation1 == string::npos || idLocation2 == string::npos)
					return ALXERR + 4;

				string oIDLoc1 = visitScript.substr(idLocation1 + 15, 20);
				string oIDLoc2 = visitScript.substr(idLocation2 + 27, 20);

				try {
					qID1 = stoi(oIDLoc1);
					qID2 = stoi(oIDLoc2);
				}
				catch (std::invalid_argument e) {
					return ALXERR + 5;
				}

				qID3 = *(DWORD*)(LocalPlayer + 0xEC);

				//printf("OK\nAuthenticating... ");
				// ID HANDLING

				__int64* eqIDs = new __int64[3];
				CryptoPP::SHA3(8).CalculateDigest((byte*)eqIDs, (const byte*)&qID1, 4);
				CryptoPP::SHA3(8).CalculateDigest((byte*)eqIDs + 8, (const byte*)&qID2, 4);
				CryptoPP::SHA3(8).CalculateDigest((byte*)eqIDs + 16, (const byte*)&qID3, 4);

				//printf("result: %016I64X\n\n", eqIDs[0] & eqIDs[1] & eqIDs[2]);

				char filePath[MAX_PATH];
				GetFile("alxwl.data", filePath, MAX_PATH);

				FILE* wlFile = fopen(filePath, "rb");
				if (!wlFile) return ALXERR + 6;

				long int wlFileSize = GetFileSize(wlFile);
				if (wlFileSize < 10) return ALXERR + 7;
				if (wlFileSize % 8 != 0) return ALXERR + 8;

				BYTE* wlData = new BYTE[wlFileSize];
				if (fread(wlData, 1, wlFileSize, wlFile) != wlFileSize) return ALXERR + 9;

				fclose(wlFile);

				__int64 eqIDsAnded = eqIDs[0] & eqIDs[1] & eqIDs[2];
				//printf("rh: %016I64X, %016I64X, %016I64X, %016I64X\n", eqIDs[0], eqIDs[1], eqIDs[2], eqIDsAnded);
				//printf("r: %s, %s\n\n", oIDLoc1.c_str(), oIDLoc2.c_str());

				// INTEGRITY CHECK //

				wlFileSize -= 8;
				__int64 fileHash = 0;
				CryptoPP::SHA3(8).CalculateDigest((byte*)&fileHash, wlData, wlFileSize);
				if (fileHash != *(__int64*)(wlData + wlFileSize)) return ALXERR + 8;

				// WHITELIST VERIFICATION //
				
				__int64 nameHash = 0;
				CryptoPP::SHA3(8).CalculateDigest((byte*)&nameHash, (const byte*)LocalPlayerName.c_str(), LocalPlayerName.size() + 1);

				for (int i = 0; i < wlFileSize;) {
					__int64 wlNameHash = *(__int64*)(wlData + i);
					i += 8;

					__int64 wlIDHash = *(__int64*)(wlData + i);
					i += 8;

					//if (nameHash == wlNameHash) {
					//	printf("e1: %016I64X\n", wlNameHash);
					//	printf("e2: %016I64X\n", wlIDHash);
					//	printf("ih: %016I64X\n", eqIDsAnded);
					//	printf("lpn: |%s|\n", LocalPlayerName.c_str());
					//	printf("m: %d\n\n", eqIDsAnded == wlIDHash);
					//}

					if ((nameHash == wlNameHash) && (eqIDsAnded == wlIDHash)) {
						return InitiateWindow();
					}
				}

				return 0;
			}
			return ALXERR + 3;
		}
		return ALXERR + 2;
	}
	return ALXERR + 1;
}

void Initiate() {
	//if (DEBUG_MODE)
	CreateConsole();

	printf("Initiating...\n");

	generate_table();
	ROBLOX_BASE = (DWORD)GetModuleHandle(NULL);
	HMODULE alxBase = GetModuleHandle("SGExploit V2.dll");
	ALX_BASE = (DWORD)alxBase;

	rLua = new RLua;

	athread mainThread(Verify, NULL);

	int exitCode = mainThread.wait();

	if (!(exitCode == 0 || exitCode == ALXRESTART)) {
		CreateConsole();
		if (exitCode == ALXERR + 8) {
			printf("[ALX] ERROR: Whitelist file 'alxwl.data' is corrupted.\n");
		}
		else {
			printf("[ALX] ERROR: %d\n", exitCode);
		}
		system("pause");
	}

	delete rLua;

	if (exitCode == ALXRESTART) {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		char injectorLoc[MAX_PATH];
		GetFile("alxinject.exe", injectorLoc, MAX_PATH);

		CreateProcess(injectorLoc, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	}

	FreeConsole();
	FreeLibraryAndExitThread(alxBase, 0);
}

void CreateConsole() {
	if (DoesConsoleExist())
		return;

	int hConHandle = 0;
	HANDLE lStdHandle = 0;
	FILE *fp = 0;

	// Allocate a console
	AllocConsole();

	// redirect unbuffered STDOUT to the console
	lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	SetConsoleTitle("SGExploitsV2");
}

BOOLEAN DoesConsoleExist() {
	return GetConsoleWindow() != NULL;
}
