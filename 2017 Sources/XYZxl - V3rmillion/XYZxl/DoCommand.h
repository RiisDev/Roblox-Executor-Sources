#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <cstring>
#include <sstream>
#include <ctype.h>
#include "MainLua.h"
#include "XYZ_DB.h"
//#include "DoLuaC.h"
#include "detours.h"
//#include "form.h"
#include "dllmain.h"

std::string ag90vha9eghzza = VMProtectDecryptStringA("th");

int CrashFix(const std::string& s) {
	bool IsNumber = is_number(s);
	if (IsNumber == true) {
		return int(1);
	}
	return int(0);
}

const char* Commands =
"\r\n--> Server Commands: \r\n"
" # loadgame [gameid] - Loads any uncopylocked game into the current game(buggy)"
" # fog [n] - Sets the fog-start to [n]\r\n"
" # fogcolor [n] [n] [n] - Sets the fogcolor to [n] [n] [n]\r\n"
" # ambient [n] [n] [n] - Sets the ambient to [n] [n] [n]\r\n"
" # brightness [n] - Changes the game brightness to [n]\r\n"
" # fixlighting - Fixes the lighting in the game\r\n"
" # clrfog - Clears the fog\r\n"
" # clrterrain - Clears the terrain in personal servers\r\n"
" # music [id] - Plays the given music [id]\r\n"
" # stopmusic - Stops the currently playing sound/music\r\n"
" # destroygame - Makes the game unplayable\r\n"
" # radmin - Attempts to try to remove server admin & anti-exploits\r\n"
" # time [n] - Set time (in hours) to [n]\r\n"
" # day - Makes the game day time\r\n"
" # night - Makes the game night time\r\n"
" # shutdown - Crashes the server\r\n\r\n"
"--> Player Commands: \r\n"
" # naked [p] - Removes [p]'s clothes\r\n"
" # confuse [p] - Confuses [p] walkspeed by reversing it\r\n"
" # unconfuse [p] - Unconfuses a character if confused\r\n"
" # faceid [p] [decalId] - Changes [p] face to the given decalId\r\n"
" # customhead [p] [n] - Changes [p] head size to the given number\r\n"
" # hatsize [p] [n] - Changes [p] hats size to the given number\r\n"
" # tp [p1] [p2] - Teleports [p1] to [p2]\r\n"
" # goto [p] - Teleports your character to [p]\r\n"
" # bring [p] - Teleports [p] to your character\r\n"
" # skybox [id] - Sets the skybox to the given [id] decal\r\n"
" # rskybox -  Removes the current skybox in the Lighting"
" # btools [p] - Gives [p] the original building tools on ROBLOX\r\n"
" # changestat [p] [stat] [num/string] - Changes [p] [stat] to [num/string] from the leaderstats\r\n"
" # fling [p] - Flings [p] from their location\r\n"
" # camlock [p] - Locks [p]'s camera in first person mode\r\n"
" # camunlock [p] - Unlocks [p]'s camera from first person\r\n"
" # fov [p] [n] - Set [p]'s max zoom out distance to [n]\r\n"
" # char [p] [id] - Will make [p1]'s character appearance the same as the [id]'s appearance\r\n"
//"-- THEY CAN BOTH BE USED THE SAME WAY -- [command above and below this line]\r\n"
" # char [p1] [p2] - Will make [p1]'s character appearance the same as [p2]'s character appearance\r\n"
" # stealstat [p] [statname] - Steals [p]'s [statName] and swaps your [statName] with [p]'s\r\n"
" # rlimbs [p] - Removes [p]'s limbs\r\n"
" # rarms [p] - Removes [p]'s arms\r\n"
" # rlegs [p] - Removes [p]'s legs\r\n"
" # hidename [p] - Hide player's name and healthbar (Useful for stealth!)\r\n"
" # showname [p] - Show a player's name and healthbar\r\n"
" # jpower [p] [n] - Sets the JumpPower to [n] on [p]\r\n"
" # name [p] [string] - Names [p] [string]\r\n"
" # unname [p] - Renames [p] back to his old username\r\n"
" # ws [p] [n] - Sets the target [p] walkspeed to [n]\r\n"
" # jump [p] - Forces [p] to jump\r\n"
" # smallhead [p] - Makes [p]'s head small\r\n"
" # bighead [p] - Makes [p]'s head big\r\n"
" # clone [p] - Clones [p] and sets the clone in the workspace\r\n"
" # sit [p] - Forces [p] to sit\r\n"
" # stools [p] - Steals [p]'s tools and puts them in your backpack!\r\n"
" # shats [p] - Steals [p]'s hats and gives you them!\r\n"
" # rtools [p] - Clears the [p] backpack from tools\r\n"
" # heal [p] - Heals [p] from any damage taken\r\n"
" # health [p] [n] - Sets [p]'s MaxHealth to [n]\r\n"
" # sgod [p] - Semi-gods [p] instead of fully godding\r\n"
" # god [p] - Gods the target [p]\r\n"
" # ungod [p] - Ungods the target [p]\r\n"
" # kill [p] - Kills the target [p]\r\n"
" # blockhead [p] - Turns [p]'s head into a block\r\n"
" # unhuman [p] - Doesn't allow [p] to be able to reset on the roblox gui\r\n"
" # human [p] - Allows [p] to once again be able to reset on the gui\r\n"
" # light [p] - Lights up [p]\r\n"
" # unlight [p] - Removes the light from [p]\r\n"
" # ff [p] - Gives a forcefield to [p]\r\n"
" # unff [p] - Removes the target [p]'s forcefield\r\n"
" # fire [p] - Gives [p] fire in their torso\r\n"
" # unfire [p] - Removes the target [p]'s torso fire\r\n"
" # sparkles [p] - Gives [p] sparkles in their torso\r\n"
" # unsparkles [p] - Removes the target [p]'s torso sparkles\r\n"
" # smoke [p] - Gives [p] sparkles in their torso\r\n"
" # unsmoke [p] - Removes the target [p]'s torso smoke\r\n"
" # invisible [p] - Turns [p] invisible!\r\n"
" # visible [p] - Turns back [p] visible\r\n"
" # ghost [p] - Turns [p] into a ghost :O\r\n"
" # kick [p] - Makes [p] nilled from the players\r\n"
" # freeze [p] - Freezes the target [p]\r\n"
" # thaw [p] - Thaws the target [p]\r\n"
" # punish [p] - Punishes the target [p]\r\n"
" # unpunish [p] - Unpunishes the target [p]\r\n"
" # view [p] - Views [p]'s character\r\n"
" # unview - Sets your view back to your character\r\n"
" # nojump [p] - Doesn't allow [p] to be able to jump\r\n"
" # lock [p] - Locks [p] so people can't use btools on the target\r\n"
" # unlock [p] - Unlocks [p] allowing people to use btools such as delete tool on [p]\r\n"
//" # sword [p] - Gives [p] a sword\r\n"
//" # vacuum [p] - Gives [p] a vacuum :)!\r\n"
//" # car [p] - Gives [p] a car\r\n"
//" # carpet [p] - Gives [p] a flying carpet\r\n"
//" # insert [p] - Gives [p] the old InsertTool\r\n"
//" # resize [p] - Gives [p] a resize tool\r\n"
" # lolwot [p] - Supah secret command!!1\r\n\r\n"
"--> Local Commands: \r\n"
" # noclip - Allows you to go through bricks\r\n"
" # swim - Allows you to swim-fly while holding the space-bar\r\n"
" # jesusfly - Allows you to fly backwards when holding space-bar\r\n"
" # airwalk - Allows you to walk in mid air\r\n"
" # superjump - Allows you to jump even higher!\r\n"
" # ragdoll - Makes you flop around when you've respawned\r\n"
" # normal - Toggles off all of the above abilities\r\n"
" # cmds - Shows the list of the commands\r\n"
" # clear - Clears the console\r\n"
" # exit - Exits the console and the game\r\n\r\n"
"--> MISC Commands: \r\n"
" # loop [commandName] - Loops the command you type (you can only loop 1 thing otherwise it will stop the first thing looped and go to the second)\r\n"
"    [EXAMPLE: loop ws others -100]\r\n";
//"-------------------------------------------------------\r\n\n"
//"--> Addons Commands [for specific games]: \r\n"
//" > Twisted Murderer | GameId: 177052655 <\r\n"
//"  # tm [msg] - When a game has started you run this to display a global message\r\n"
//" > Apocalypse Rising | GameId: 1600503 <\r\n"
//"  # apocbadges - Gives the localplayer all the game badges after he rejoins the game\r\n";

bool isFF = false;
bool ContextHooked = false;
bool Authenticated = false;
std::string prefix = ";";
std::vector<DWORD> Holder1 = {};

double PatchedWalkSpeed(int self) { // prevent walkspeed detection from client
	return 16;
}

double PatchedJumpPower(int self) { // prevent jmppower detection from client
	return 50;
}

DWORD* PatchedIdentity() { // To change the SecurityScriptContext
	return rbxaddy((DWORD*)0x16A1830);//0x1630BB8;
									  // Find "Focused" then click the sub_NUMBERS that look like this:
									  // "push    offset sub_E46960" then go to the very last dword in that function and that should be it
}

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

void wait(int seconds) {
	Sleep(seconds * 1000); //there you go
}

void cmdLoopJump();
void cmdLoopSit();
void cmdLoop1();
void cmdLoop2();
extern void DoCommand(const std::string& cmd);

int SecurityContext = 0;

void RunLua(std::string source) {
	//std::thread run(&ExecuteScript, source);
	//run.detach();
}

void ClearChildrenSafely(int instance) {
	try {
		std::vector<DWORD> CharacterChildren = GetChildren(instance);
		for (std::size_t j = 0; j < CharacterChildren.size(); j++) {
			if (CharacterChildren[j] > 10000) {
				try {
					std::string ClassType = GetClass(CharacterChildren[j]);
					if (ClassType != "Terrain") {
						if (ClassType != "Camera") {
							if (ClassType != "TerrainRegion") {
								iDestroy(CharacterChildren[j]); Sleep(100);
							}
						}
					}
				}
				catch (std::exception& e) {
					MessageBoxA(0, e.what(), "", 0);
				}
			}
		}
	}
	catch (std::exception& e) {
		MessageBoxA(0, e.what(), "", 0);
	}
}

//bool LocalStateStarted = false;
void ChangeState(int Location) {
	//ConsoleOutput("\r\nLocal commands are temporarily disabled!");
	//if (LocalStateStarted == false) {
		//LocalStateStarted = true;
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Loop, 0, 0, 0);
	//}
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

bool QuickCompare(const BYTE *pData, const BYTE *bMask, const char *szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask) return 0;
	return (*szMask) == NULL;
}

DWORD QuickFindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, const char *szMask) {
	for (std::size_t i = 0; i<(int)dwLen; i++)
		if (QuickCompare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
	return 0;
}

int QuickScan(DWORD mode, const char* content, const char* mask) {
	VMProtectBeginMutation("ihISfhSFH9g38hdA"); // Hot scan function
	DWORD PageSize;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	PageSize = si.dwPageSize;
	MEMORY_BASIC_INFORMATION mi;
	for (DWORD lpAddr = 0; lpAddr<rbxaddy(0x7FFFFFFF); lpAddr += PageSize) {
		DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
		if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
		if (mi.Type == MEM_MAPPED) continue;
		if (mi.Protect == mode) {
			int addr = QuickFindPattern(lpAddr, PageSize, (PBYTE)content, mask);
			if (addr != 0) {
				VMProtectEnd();
				return addr;
			}
		}
	}
}

typedef DWORD(__thiscall *xxxxxxx)(void* self, int i);

bool localplayerSet = false;
void SetupLocalPlayer(int state) {
	if (localplayerSet != true) {
		rbxlua_getservice(state, "Players");
		if (!rbxlua_isnil(state, -1)) {
			rbxlua_getfield(state, -1, "LocalPlayer");
			rbxlua_setglobal(state, "LocalPlayer");
			localplayerSet = true;
		}
		else {
			clearStack(LocalState);
		}
	}
}

bool localDebounce = false;
void SetLocalHumanoidState(int state) {
	if (LocalState == 0) {
		int StarterState = ((xxxxxxx)(rbxaddy(0x539E00)))((void*)ScriptContext, 6);//rbxlua_newthread(CluaState);
		LocalState = rbxlua_newthread(StarterState);
	}
	if (LocalState != 0 && localDebounce == false) {
		localDebounce = true;
		std::string HumanoidName = *GetName(GetHumanoidFromPlayer(LocalPlayer));
		if (HumanoidName != "0") {
			SetupLocalPlayer(LocalState);
			rbxlua_getglobal(LocalState, "LocalPlayer");
			if (!rbxlua_isnil(LocalState, -1)) {
				rbxlua_getfield(LocalState, -1, "Character");
				if (!rbxlua_isnil(LocalState, -1)) {
					rbxlua_getfield(LocalState, -1, HumanoidName.c_str());//"Humanoid");
					if (!rbxlua_isnil(LocalState, -1)) {
						rbxlua_getfield(LocalState, -1, "ChangeState");
						if (rbxlua_isfunction(LocalState, -1)) {
							rbxlua_pushvalue(LocalState, -2);
							rbxlua_pushnumber(LocalState, double(state));
							rbxlua_pcall(LocalState, 2, 0, 0);
							ConsoleOutput("");
						}
						else {
							clearStack(LocalState);
						}
					}
					else {
						clearStack(LocalState);
					}
				}
				else {
					clearStack(LocalState);
				}
			}
			else {
				clearStack(LocalState);
			}
		}
		localDebounce = false;
	}
}
std::vector<DWORD> RecurseFindGear(int Ins) {
	std::vector<DWORD> Children = GetChildren(Ins);
	for (size_t i = 0; i < Children.size(); i++) {
		DWORD Child = Children[i];
		if (strcmp(GetClass(Child), "Tool") == 0) {
			Holder1.push_back(Child);
		}
		else {
			RecurseFindGear(Child);
		}
	}
	//std::vector<DWORD> ret(Holder);
	//Holder.clear();
	return Holder1;
}

std::string encryptDecrypt(std::string toEncrypt) {
	char key[10] = { '1', '3', '3', '7', 'I', 's', 'B', 'a', 'c', 'k' }; //Any chars will work
	std::string output = toEncrypt;

	for (int i = 0; i < toEncrypt.size(); i++)
		output[i] = toEncrypt[i] ^ key[i % (sizeof(key) / sizeof(char))];

	return output;
}

//unsigned int a_INT(const char* str, int h = 0)
//{
	// DJB Hash function
	// not my code but can't remember where I got it from
//	return !str[h] ? 5381 : (_INT(str, h + 1) * 33) ^ str[h];
//}

constexpr unsigned int _INT(const char* str, int h = 0) {
	return !str[h] ? 5381 : (_INT(str, h + 1) * 33) ^ str[h];
}

int testmest = -8;
bool sploit = false;
void __cdecl DoCommand(const std::string& cmd) {
	if (sploit == false) {
		sploit = true;
		std::vector<std::string> args = GetArguments(cmd);
		std::vector<DWORD> players;
		if (ScriptContext != 0) {
			if (args.size() != NULL) {
				//if (args.size() != NULL) {
				std::string command = args.front(); args.erase(args.begin());
				std::string fixedCommand = to_lowercase(command);
				players = GetPlayersFromMessage(args);
				//rbxlua_pcall(CluaState, 0, -1, 0);

				switch (_INT(fixedCommand.c_str())) {
				case _INT("real"): {
					LoadInAsset(args.back());
				}
				case _INT("clip"): case _INT("noswim"): {
					Changing = false;
					break;
				}
				case _INT("noclip"): {
					ChangeState(11);
					break;
				}
				case _INT("swim"): {
					ChangeState(4);
					break;
				}
				case _INT("superjump"): {
					ChangeState(13);
					break;
				}
				case _INT("airwalk"): {
					ChangeState(10);
					break;
				}
				case _INT("jesusfly"): {
					ChangeState(12);
					break;
				}
				case _INT("ragdoll"): {
					ChangeState(1);
					break;
				}
				case _INT("jumpglitch"): {
					ChangeState(13);
					break;
				}
				case _INT("disable"): {
					Changing = false;
					break;
				}
				case _INT("normal"): {
					Changing = false;
					break;
				}
				case _INT("changestate"): {
					ChangeState(atoi(args.back().c_str()));
					break;
				}
				case _INT("destroygame"): {
					rbxlua_getservice(CluaState, "Workspace");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getfield(CluaState, -1, "ClearAllChildren");
						rbxlua_pushvalue(CluaState, -2);
						rbxlua_pcall(CluaState, 1, 0, 0);
					}
					rbxlua_getservice(CluaState, "Workspace");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getfield(CluaState, -1, "ClearAllChildren");
						rbxlua_pushvalue(CluaState, -2);
						rbxlua_pcall(CluaState, 1, 0, 0);
					}
					//ClearChildrenSafely(Workspace);
					break;
				}
				/*case _INT("clrterrain"): case _INT("clearterrain"): {
					int Terrain = FindFirstClass(Workspace, "Terrain");
					if (Terrain != 0) {
						ClearTerrain(Terrain);
					}
					break;
				}*/
				case _INT("clearstack"): {
					clearStack(CluaState);
					break;
				}
				case _INT("placeme"): {
					rbxlua_getservice(CluaState, "TeleportService");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getfield(CluaState, -1, "Teleport");
						rbxlua_pushvalue(CluaState, -2);
						rbxlua_pushnumber(CluaState, 1818);
						rbxlua_pcall(CluaState, 2, 0, 0);
					}
					break;
				}
				case _INT("jump"): {
					if (args.size() >= 1) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							const char* Target = GetName(players[i])->c_str();
							int Character = GetCharacterFromPlayer(players[i]);
							std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
							if (HumanoidName == "0") {
								if (Character != 0) {
									int Humanoid = FindFirstClass(Character, "Humanoid");
									if (Humanoid != 0) {
										rbxlua_getservice(CluaState, "Players");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, Target);
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "Character");
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
													if (!rbxlua_isnil(CluaState, -1)) {
														rbxlua_pushbool(CluaState, TRUE);
														rbxlua_setfield(CluaState, -2, "Jump"); Sleep(25);
													}
													else {
														clearStack(CluaState);
													}
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("sit"): {
					if (args.size() >= 1) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							std::string Target = *GetName(players[i]);
							int Character = GetCharacterFromPlayer(players[i]);
							if (Character != 0) {
								int Humanoid = FindFirstClass(Character, "Humanoid");
								if (Humanoid != 0) {
									rbxlua_getservice(CluaState, "Players");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, Target.c_str());
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Character");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, GetName(Humanoid)->c_str());
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_pushbool(CluaState, TRUE);
													rbxlua_setfield(CluaState, -2, "Sit"); Sleep(25);
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
							}
						}
					}
					break;
				}
				case _INT("unlock"): {
					if (args.size() >= 1) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							std::string Target = *GetName(players[i]);
							int Character = GetCharacterFromPlayer(players[i]);
							if (Character != 0) {
								std::vector<DWORD> Children = GetChildren(Character);
								for (std::size_t i = 0; i < Children.size(); i++) {
									DWORD Child = Children[i];
									if (strcmp(GetClass(Child), "Part") == 0) {
										std::string PartName = *GetName(Child);
										rbxlua_getservice(CluaState, "Players");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, Target.c_str());
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "Character");
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getfield(CluaState, -1, PartName.c_str());
													if (!rbxlua_isnil(CluaState, -1)) {
														rbxlua_pushbool(CluaState, FALSE);
														rbxlua_setfield(CluaState, -2, "Locked");
														//rbxlua_pcall(CluaState, 1, 0, 0); Sleep(25);
													}
													else {
														clearStack(CluaState);
													}
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("lock"): {
					if (args.size() >= 1) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							std::string Target = *GetName(players[i]);
							int Character = GetCharacterFromPlayer(players[i]);
							if (Character != 0) {
								std::vector<DWORD> Children = GetChildren(Character);
								for (std::size_t i = 0; i < Children.size(); i++) {
									DWORD Child = Children[i];
									if (strcmp(GetClass(Child), "Part") == 0) {
										std::string PartName = *GetName(Child);
										rbxlua_getservice(CluaState, "Players");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, Target.c_str());
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "Character");
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getfield(CluaState, -1, PartName.c_str());
													if (!rbxlua_isnil(CluaState, -1)) {
														rbxlua_pushbool(CluaState, TRUE);
														rbxlua_setfield(CluaState, -2, "Locked");
															//rbxlua_pcall(CluaState, 1, 0, 0); Sleep(25);
													}
													else {
														clearStack(CluaState);
													}
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("freeze"): case _INT("frz"): {
					if (args.size() >= 1) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							int Character = GetCharacterFromPlayer(players[i]);
							if (Character != 0) {
								int Torso = FindFirstChild_PartOf(Character, "Torso");
								if (Torso != 0) {
									SetAnchored(Torso, 1);
								}
							}
						}
					}
					break;
				}
				case _INT("thaw"): case _INT("unfreeze"): {
					if (args.size() >= 1) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							int Character = GetCharacterFromPlayer(players[i]);
							if (Character != 0) {
								int Torso = FindFirstChild_PartOf(Character, "Torso");
								if (Torso != 0) {
									SetAnchored(Torso, 0);
								}
							}
						}
					}
					break;
				}
				case _INT("punish"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);
							if (Character != 0) {
								setParent(Character, ReplicatedStorage);
							}
						}
					}
					break;
				}
				case _INT("unpunish"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//FindFirstPlayer(Lighting, *GetName(players[i]));
							if (Character != 0) {
								setParent(Character, Workspace);
							}
						}
					}
					break;
				}
				case _INT("hidename"): case _INT("stealth"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (TorsoExists(players[i])) {
							double Type = 2;
							std::string Target = *GetName(players[i]);
							std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
							if (HumanoidName != "0") {
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_pushnumber(CluaState, double(Type));
												rbxlua_setfield(CluaState, -2, "DisplayDistanceType");
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("showname"): case _INT("unstealth"): case _INT("unhidename"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (TorsoExists(players[i])) {
							double Type = 0;
							std::string Target = *GetName(players[i]);
							std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
							if (HumanoidName != "0") {
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_pushnumber(CluaState, double(Type));
												rbxlua_setfield(CluaState, -2, "DisplayDistanceType");
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("friday"): {
					/*[3:46:08 PM] Undyne the Undying | Louka: & is the or operator
					[3:46:12 PM] Undyne the Undying | Louka: that means like
					[3:46:14 PM] Undyne the Undying | Louka: "next to"
					[3:46:22 PM] Undyne the Undying | Louka: 0xA & 1
					[3:46:24 PM] Undyne the Undying | Louka: is like
					[3:46:26 PM] Undyne the Undying | Louka: 0xA + 4
					[3:46:28 PM] Undyne the Undying | Louka: if its a dword
					[3:46:34 PM] Undyne the Undying | Louka: if its a word/short its + 2
					[3:46:37 PM] Undyne the Undying | Louka: if its a byte its + 1*/
					*(DWORD*)rbxaddy(0x1730550) = 1337;

					*(DWORD*)rbxaddy(0x13C8B94) = 1;
					*(DWORD*)(rbxaddy(0x13C8B94) + 4) = 1;

					*(DWORD*)rbxaddy(0x13C8A14) = 1;
					*(DWORD*)(rbxaddy(0x13C8A14) + 4) = 1;

					*(BYTE*)rbxaddy(0x13C8B90) = 1;

					*(BYTE*)rbxaddy(0x13C8B44) = 1;
					//*(BYTE*)(rbxaddy(0xE9DD7C)) = 1;//0x90;
					break;
				}
				case _INT("execute"): {
					LuaExecuteString(cmd.substr(8));
					break;
				}
				case _INT("loop"): case _INT("loop1"): {
					loopCommand1 = cmd.substr(5); Sleep(50);
					std::thread loopcmd1(cmdLoop1);
					loopcmd1.detach();
					//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cmdLoop1, 0, 0, 0);
					break;
				}
				case _INT("loop2"): {
					loopCommand2 = cmd.substr(5); Sleep(50);
					std::thread loopcmd2(cmdLoop2);
					loopcmd2.detach();
					//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cmdLoop2, 0, 0, 0);
					break;
				}
				case _INT("fog"): {
					if (CrashFix(args.back()) != 0) {
						int fog = std::stoi(args.at(0));
						std::string fogstr1 = std::to_string(fog);
						std::string fogstr2 = std::to_string(fog + 45);
						rbxlua_getservice(CluaState, "Lighting");
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_pushstring(CluaState, fogstr1.c_str());
							rbxlua_setfield(CluaState, -2, "FogStart");

							rbxlua_pushstring(CluaState, fogstr2.c_str());
							rbxlua_setfield(CluaState, -2, "FogEnd");
						}
						else {
							clearStack(CluaState);
						}
						//float fog_num;
						//try {
						//	fog_num = std::stof(args.at(0));
						//}
						//catch (std::invalid_argument e) {
						//	fog_num = 18;
						//}
						//SetFogStart(Lighting, fog_num);
						//SetFogEnd(Lighting, fog_num + 45);
					}
					ConsoleOutput("Executed Fog Command!\n");
					break;
				}
				case _INT("unlight"): case _INT("nolight"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);
							if (Character != 0) {
								int Torso = FindFirstChild_PartOf(Character, "Torso");
								if (Torso != 0) {
									cDestroy(Torso, "SpotLight");
								}
							}
						}
					}
					break;
				}
				case _INT("unff"): case _INT("noff"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//GetCharacter(*GetName(players[i]));
							if (Character != 0) {
								cDestroy(Character, "ForceField");
							}
						}
					}
					break;
				}
				case _INT("unfire"): case _INT("nofire"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//GetCharacter(*GetName(players[i]));
							if (Character != 0) {
								int Torso = FindFirstChild_PartOf(Character, "Torso");
								if (Torso != 0) {
									cDestroy(Torso, "Fire");
								}
							}
						}
					}
					break;
				}
				case _INT("unsparkles"): case _INT("nosparkles"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//GetCharacter(*GetName(players[i]));
							if (Character != 0) {
								int Torso = FindFirstChild_PartOf(Character, "Torso");
								if (Torso != 0) {
									cDestroy(Torso, "Sparkles");
								}
							}
						}
					}
					break;
				}
				case _INT("unsmoke"): case _INT("nosmoke"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//GetCharacter(*GetName(players[i]));
							if (Character != 0) {
								int Torso = FindFirstChild_PartOf(Character, "Torso");
								if (Torso != 0) {
									cDestroy(Torso, "Smoke");
								}
							}
						}
					}
					break;
				}
				case _INT("blockhead"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);
							if (Character != 0) {
								int Head = FindFirstChild_PartOf(Character, "Head");
								if (Head != 0) {
									cDestroy(Head, "SpecialMesh");
									cDestroy(Head, "CylinderMesh");
									cDestroy(Head, "BlockMesh");
								}
							}
						}
					}
					break;
				}
				case _INT("nojump"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						double speed = 0;
						std::string Target = *GetName(players[i]);
						std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
						if (HumanoidName != "0") {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, double(speed));
										rbxlua_setfield(CluaState, -2, "JumpPower");
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("unnojump"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						double speed = 60;
						std::string Target = *GetName(players[i]);
						std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
						if (HumanoidName != "0") {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, double(speed));
										rbxlua_setfield(CluaState, -2, "JumpPower");
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("stools"): case _INT("stealtools"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] != 0) {
							int Character = GetCharacterFromPlayer(players[i]);
							int LocalBackpack = LocalPlayer_Get("Backpack");
							int vBackpack = FindFirstClass(players[i], "Backpack");
							if (LocalBackpack != 0 && vBackpack != 0) {
								std::vector<DWORD> BackpackChildren = GetChildren(vBackpack);
								for (std::size_t x = 0; x < BackpackChildren.size(); x++) {
									if (players[i] != 0 && vBackpack != 0 && BackpackChildren[x] != 0) {
										std::string ClassType = GetClass(BackpackChildren[x]); Sleep(5);
										if (ClassType == "Tool") {
											setParent(BackpackChildren[x], LocalBackpack);
										}
										else if (ClassType == "HopperBin") {
											setParent(BackpackChildren[x], LocalBackpack);
										}
									}
								}
							}
							if (Character != 0 && players[i] != 0 && vBackpack != 0 && LocalBackpack != 0) {
								int Tool = FindFirstClass(Character, "Tool");
								if (Tool != 0) {
									setParent(Tool, LocalBackpack);
								}
							}
						}
					}
					break;
				}
				case _INT("droppable"): case _INT("geardroppable"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);
							int Backpack = FindFirstClass(players[i], "Backpack");
							if (Backpack > 10000) {
								std::vector<DWORD> BackpackChildren = GetChildren(Backpack);
								for (std::size_t x = 0; x < BackpackChildren.size(); x++) {
									if (BackpackChildren[x] > 10000) {
										std::string ClassType = GetClass(BackpackChildren[x]);
										if (ClassType == "Tool") {
											SetCanBeDropped(BackpackChildren[x], 1); Sleep(25);
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("notdroppable"): case _INT("undroppable"): case _INT("gearundroppable"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);
							int Backpack = FindFirstClass(players[i], "Backpack");
							if (Backpack > 10000) {
								std::vector<DWORD> BackpackChildren = GetChildren(Backpack);
								for (std::size_t x = 0; x < BackpackChildren.size(); x++) {
									if (BackpackChildren[x] > 10000) {
										std::string ClassType = GetClass(BackpackChildren[x]);
										if (ClassType == "Tool") {
											SetCanBeDropped(BackpackChildren[x], 0); Sleep(25);
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("rlimbs"): case _INT("removelimbs"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//GetCharacter(*GetName(players[i]));
							if (Character != 0) {
								std::vector<DWORD> CharacterChildren = GetChildren(Character);
								for (std::size_t j = 0; j < CharacterChildren.size(); j++) {
									std::string ClassType = GetClass(CharacterChildren[j]);
									if (ClassType == "Part") {
										if (*GetName(CharacterChildren[j]) != "Torso") {
											if (*GetName(CharacterChildren[j]) != "Head") {
												if (*GetName(CharacterChildren[j]) != "HumanoidRootPart") {
													//rSetParent(CharacterChildren[j], 0);
													iDestroy(CharacterChildren[j]);
												}
											}
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("rarms"): case _INT("armless"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//GetCharacter(*GetName(players[i]));
							if (Character != 0) {
								rDestroy(Character, "Right Arm"); Sleep(50);
								rDestroy(Character, "Left Arm"); Sleep(50);
							}
						}
					}
					break;
				}
				case _INT("rlegs"): case _INT("legless"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//GetCharacter(*GetName(players[i]));
							if (Character != 0) {
								rDestroy(Character, "Right Leg"); Sleep(50);
								rDestroy(Character, "Left Leg"); Sleep(50);
							}
						}
					}
					break;
				}
				case _INT("breakjoints"): {
					BreakJoints(Workspace);
					break;
				}
				case _INT("rhats"): case _INT("removehats"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//GetCharacter(*GetName(players[i]));
							if (Character != 0) {
								std::vector<DWORD> CharacterChildren = GetChildren(Character);
								for (std::size_t j = 0; j < CharacterChildren.size(); j++) {
									try {
										std::string ClassType = GetClass(CharacterChildren[j]);
										if (ClassType == "Hat") {
											iDestroy(CharacterChildren[j]);
										}
									}
									catch (std::exception&) {
										Sleep(1);
									}
								}
							}
						}
					}
					break;
				}
				case _INT("naked"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] > 1000) {
							int Character = GetCharacterFromPlayer(players[i]);//GetCharacter(*GetName(players[i]));
							if (Character != 0) {
								//int Torso = FindFirstChild_PartOf(Character, "Torso");
								//if (Torso != 0){
								//	cDestroy(Torso, "ShirtGraphic");//loopCheck); // TShirt
								//}
								std::vector<DWORD> CharacterChildren = GetChildren(Character);
								for (std::size_t j = 0; j < CharacterChildren.size(); j++) {
									std::string ClassType = GetClass(CharacterChildren[j]);
									if (ClassType == "Shirt") {
										//*(DWORD*)(CharacterChildren[j] + 0x90) = 0;
										iDestroy(CharacterChildren[j]);
									}
									if (ClassType == "Pants") {
										iDestroy(CharacterChildren[j]);
									}
									//if (ClassType == "CharacterMesh") {
									//	iDestroy(CharacterChildren[j], loopCheck);
									//}
								}
							}
						}
					}
					break;
				}
				case _INT("shutdown"): {
					rbxlua_getservice(CluaState, "Players");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getfield(CluaState, -1, "ClearAllChildren");
						rbxlua_pushvalue(CluaState, -2);
						rbxlua_pcall(CluaState, 1, 0, 0); Sleep(15);
					}
					else {
						clearStack(CluaState);
					}
					break;
				}
				case _INT("clrfog"): case _INT("clearfog"): case _INT("fixlighting"): {
					double fog = 100000;
					rbxlua_getservice(CluaState, "Lighting");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_pushnumber(CluaState, fog);
						rbxlua_setfield(CluaState, -2, "FogEnd");
					}
					else {
						clearStack(CluaState);
					}
					//float start = 100000;
					//SetFogStart(Lighting, start);
					//SetFogEnd(Lighting, start);
					break;
				}
				case _INT("printcam"): { //GetCurrentCamera(Workspace);//
					int Camera = FindFirstClass(Workspace, "Camera");
					if (Camera != 0) {
						ConsoleOutput(std::to_string(GetCameraType(Camera)));
					}
					break;
				}
				case _INT("view"): case _INT("watch"): {
					int Camera = GetCurrentCamera(Workspace);//FindFirstClass(Workspace, "Camera");
					if (Camera != 0) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							if (players[i] > 1000) {
								int Character = GetCharacterFromPlayer(players[i]);
								if (Character != 0) {
									int Humanoid = FindFirstClass(Character, "Humanoid");
									if (Humanoid != 0) {
										SetCameraSubject(Camera, Humanoid);
										SetCameraType(Camera, 3);
									}
								}
							}
						}
					}
					break;
				}
				case _INT("unview"): case _INT("unwatch"): case _INT("fixcamera"): case _INT("fixcam"): {
					int Camera = GetCurrentCamera(Workspace);//FindFirstClass(Workspace, "Camera");
					if (Camera != 0 && LocalPlayer != 0) {
						int Character = GetCharacterFromPlayer(LocalPlayer);
						if (Character != 0) {
							int Humanoid = FindFirstClass(Character, "Humanoid");
							if (Humanoid != 0) {
								SetCameraSubject(Camera, Humanoid);
								if (GetCameraType(Camera) != 5) {
									SetCameraType(Camera, 5);
								}
							}
						}
					}
					break;
				}
				case _INT("fogcolor"): case _INT("colorfog"): {
					if (args.size() == 3) {
						double No1;
						double No2;
						double No3;
						try {
							No1 = std::stof(args.at(0).c_str());
							No2 = std::stof(args.at(1).c_str());
							No3 = std::stof(args.at(2).c_str());
						}
						catch (std::invalid_argument e) {
							No1 = 0;
							No2 = 0;
							No3 = 0;
						}
						rbxlua_getservice(CluaState, "Lighting");
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_getglobal(CluaState, "Color3");
							rbxlua_getfield(CluaState, -1, "new");
							rbxlua_pushnumber(CluaState, double(No1));
							rbxlua_pushnumber(CluaState, double(No2));
							rbxlua_pushnumber(CluaState, double(No3));

							rbxlua_pcall(CluaState, 3, 1, 0);
							rbxlua_setfield(CluaState, -3, "FogColor"); Sleep(20);
							rbxlua_pcall(CluaState, 0, 0, 0);
						}
						else {
							clearStack(CluaState);
						}
						/*float No1 = std::stof(args.at(0).c_str());
						float No2 = std::stof(args.at(1).c_str());
						float No3 = std::stof(args.at(2).c_str());
						SetFogColor(Lighting, No1, No2, No3);*/
					}; Sleep(10);
					break;
				}
				case _INT("ambient"): {
					if (args.size() == 3) {
						double No1;
						double No2;
						double No3;
						try {
							No1 = std::stof(args.at(0).c_str());
							No2 = std::stof(args.at(1).c_str());
							No3 = std::stof(args.at(2).c_str());
						}
						catch (std::invalid_argument e) {
							No1 = 0;
							No2 = 0;
							No3 = 0;
						}
						rbxlua_getservice(CluaState, "Lighting");
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_getglobal(CluaState, "Color3");
							rbxlua_getfield(CluaState, -1, "new");
							rbxlua_pushnumber(CluaState, double(No1));
							rbxlua_pushnumber(CluaState, double(No2));
							rbxlua_pushnumber(CluaState, double(No3));

							rbxlua_pcall(CluaState, 3, 1, 0);
							rbxlua_setfield(CluaState, -3, "Ambient"); Sleep(20);
							rbxlua_pcall(CluaState, 0, 0, 0);
						}
						else {
							clearStack(CluaState);
						}
						/*float No1 = std::stof(args.at(0).c_str());
						float No2 = std::stof(args.at(1).c_str());
						float No3 = std::stof(args.at(2).c_str());
						SetFogColor(Lighting, No1, No2, No3);*/
					}; Sleep(10);
					break;
				}
				case _INT("brightness"): case _INT("gamebrightness"): {
					if (CrashFix(args.back()) != 0) {
						int val = std::stoi(args.at(0));
						std::string newval = std::to_string(val);
						rbxlua_getservice(CluaState, "Lighting");
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_pushstring(CluaState, newval.c_str());
							rbxlua_setfield(CluaState, -2, "Brightness");
						}
						else {
							clearStack(CluaState);
						}
					}
					break;
				}
				case _INT("prefix"): {
					prefix = args.back();
				}
				case _INT("time"): {
					if (CrashFix(args.back()) != 0) {
						int time = std::stof(args.back());
						std::string timestr = std::to_string(time) + "00:00";
						rbxlua_getservice(CluaState, "Lighting");
						//	if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_pushstring(CluaState, timestr.c_str());
						rbxlua_setfield(CluaState, -2, "TimeOfDay");
						//rbxlua_pcall(CluaState, 0, 0, 0);
				//	}
						//SetMinutesAfterMidnight(Lighting, time * (float)60.0);
					}
					break;
				}
				case _INT("day"): {
					int time = 9;
					std::string timestr = std::to_string(time) + "00:00";
					rbxlua_getservice(CluaState, "Lighting");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_pushstring(CluaState, timestr.c_str());
						rbxlua_setfield(CluaState, -2, "TimeOfDay");
					}
					else {
						clearStack(CluaState);
					}
					//SetMinutesAfterMidnight(Lighting, time * (float)60.0);
					break;
				}
				case _INT("night"): {
					int time = 0;
					std::string timestr = std::to_string(time) + "00:00";
					rbxlua_getservice(CluaState, "Lighting");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_pushstring(CluaState, timestr.c_str());
						rbxlua_setfield(CluaState, -2, "TimeOfDay");
						//rbxlua_pcall(CluaState, 0, 0, 0);
					}
					else {
						clearStack(CluaState);
					}
					//SetMinutesAfterMidnight(Lighting, time * (float)60.0);
					break;
				}
				case _INT("radmin"): {
					//std::cout << "Attempting to remove admin..\n";
					//cDestroy(Workspace, "Script");
					rDestroy(Workspace, "Admin");
					rDestroy(Workspace, "Scripth");
					rDestroy(Workspace, "Anti");
					rDestroy(Workspace, "Exploit");
					rDestroy(Workspace, "EISS");
					rDestroy(Workspace, "Ban");
					rDestroy(Workspace, "Commands");
					rDestroy(Workspace, "Orb");
					rDestroy(Workspace, "Hax");
					rDestroy(Workspace, "Kohl");
					//rDestroy(Workspace, "Kohl", loopCheck);
					break;
				}
				case _INT("shats"): case _INT("stealhats"): {
					int LocalCharacter = GetCharacterFromPlayer(LocalPlayer);
					if (LocalCharacter != NULL) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							if (players[i] > 1000) {
								int Character = GetCharacterFromPlayer(players[i]);
								if (Character != 0) {
									std::vector<DWORD> CharacterChildren = GetChildren(Character);
									for (std::size_t j = 0; j < CharacterChildren.size(); j++) {
										std::string ClassType = GetClass(CharacterChildren[j]);
										if (ClassType == "Hat") {
											setParent(CharacterChildren[j], LocalCharacter);
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("sff"): case _INT("stealff"): case _INT("stealforcefield"): {
					int LocalCharacter = GetCharacterFromPlayer(LocalPlayer);
					if (LocalCharacter != NULL) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							if (players[i] > 1000) {
								int Character = GetCharacterFromPlayer(players[i]);
								if (Character != 0) {
									std::vector<DWORD> CharacterChildren = GetChildren(Character);
									for (std::size_t j = 0; j < CharacterChildren.size(); j++) {
										std::string ClassType = GetClass(CharacterChildren[j]);
										if (ClassType == "ForceField") {
											setParent(CharacterChildren[j], LocalCharacter);
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("stealstat"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (players[i] != 0) {
							std::vector<DWORD> PlayersChildren = GetChildren(players[i]);
							for (std::size_t j = 0; j < PlayersChildren.size(); j++) {
								std::string oldya = *GetName(PlayersChildren[j]);
								const char* statName = oldya.c_str();
								if (std::strstr(statName, "stat") || std::strstr(statName, "data") || std::strstr(statName, "supercool")) {
									int index = PlayersChildren[j];
									std::string statName = GetChildName(index, args.back());
									ConsoleOutput(statName);
									int stat = FindFirstChild_PartOf(index, statName);
									if (stat != 0) {
										int local_leaderstats = LocalPlayer_Find((*GetName(index)).c_str());
										if (local_leaderstats != 0) {
											int localStat = FindFirstChild_PartOf(local_leaderstats, statName);
											if (localStat != 0) {
												setParent(stat, local_leaderstats);
												setParent(localStat, index);
											}
											else {
												setParent(stat, local_leaderstats);
											}
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("services"): {
					std::cout << "\nScriptContext: " << ScriptContext << "\n";
					std::cout << "\nWorkspace: " << Workspace << "\n";
					std::cout << "\nLighting: " << Lighting << "\n";
					std::cout << "\nPlayers: " << Players << "\n";
					std::cout << "\nLocalPlayer: " << GetLocalPlayer(Players) << "\n";
					break;
				}
				case _INT("kill"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							if (GetCharacterFromPlayer(players[i]) > 0) {
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "BreakJoints");
											if (rbxlua_isfunction(CluaState, -1)) {
												rbxlua_pushvalue(CluaState, -2);
												rbxlua_pcall(CluaState, 1, 0, 0); Sleep(25);
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("chat"): case _INT("forcechat"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						std::string FixedString;
						for (std::size_t n = 0; n < (args.size() - 1); ++n) {
							if (n > 0)
								FixedString = FixedString + " " + args.at(n + 1);
							else
								FixedString = FixedString + args.at(n + 1);
						}
						rbxlua_getservice(CluaState, "Chat");
						if (Target != FixedString) {
							rbxlua_getservice(CluaState, "Players");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, Target.c_str());// std::cout << "1";
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Character");
									if (!rbxlua_isnil(CluaState, -1)) {
										//rbxlua_setglobal(CluaState, "PartToChatFrom");
										rbxlua_getservice(CluaState, "Chat");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Chat");
											if (rbxlua_isfunction(CluaState, -1)) {
												rbxlua_pushvalue(CluaState, -1);
												rbxlua_pushvalue(CluaState, -3);
												//rbxlua_getglobal(CluaState, "PartToChatFrom");
												rbxlua_pushstring(CluaState, "worked!!!");//FixedString.c_str());
												rbxlua_pcall(CluaState, 3, 0, 0); Sleep(5);
												ConsoleOutput("fully ran!");
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
										//rbxlua_pushstring(CluaState, FixedString.c_str());
										//rbxlua_setfield(CluaState, -2, "Name"); Sleep(15);
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("localchat"): {
					if (args.size() > 0) {
						std::string FixedString;
						for (std::size_t n = 0; n < (args.size()); ++n) {
							if (n > 0)
								FixedString = FixedString + " " + args.at(n);
							else
								FixedString = FixedString + args.at(n);
						}
						rbxlua_getservice(CluaState, "Players");
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_getfield(CluaState, -1, "Chat");
							rbxlua_pushvalue(CluaState, -2);
							rbxlua_pushstring(CluaState, FixedString.c_str());
							rbxlua_pcall(CluaState, 2, 0, 0);
						}
						else {
							clearStack(CluaState);
						}
					}
					break;
				}
				case _INT("fencingreach"):
				{
					DWORD LuaState = rbxlua_newthread(luaState);
					rbxlua_getservice(LuaState, "Players");
					rbxlua_getfield(LuaState, -1, "LocalPlayer");
					rbxlua_getfield(LuaState, -1, "Backpack");
					rbxlua_getfield(LuaState, -1, "Foil");
					rbxlua_getfield(LuaState, -1, "Handle");

					rbxlua_getglobal(LuaState, "Instance");
					rbxlua_getfield(LuaState, -1, "new");
					rbxlua_pushstring(LuaState, "SelectionBox");
					rbxlua_pushvalue(LuaState, -4);
					rbxlua_pcall(LuaState, 2, 1, 0);

					rbxlua_pushvalue(LuaState, -3);
					rbxlua_setfield(LuaState, -2, "Adornee");

					rbxlua_getglobal(LuaState, "game");
					rbxlua_getfield(LuaState, -1, "Players");
					rbxlua_getfield(LuaState, -1, "LocalPlayer");
					rbxlua_getfield(LuaState, -1, "Backpack");
					rbxlua_getfield(LuaState, -1, "Foil");
					rbxlua_getfield(LuaState, -1, "Handle");

					rbxlua_getglobal(LuaState, "Vector3");
					rbxlua_getfield(LuaState, -1, "new");
					rbxlua_pushstring(LuaState, "3");
					rbxlua_pushstring(LuaState, "9");
					rbxlua_pushstring(LuaState, "3");
					rbxlua_pcall(LuaState, 3, 1, 0);
					rbxlua_setfield(LuaState, -3, "Size");
				}
				case _INT("setupchat"): {
					rbxlua_getservice(CluaState, "Players");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getfield(CluaState, -1, "Chat");
						rbxlua_setglobal(CluaState, "ChatPlease");
					}
					break;
				}
				case _INT("chatspam"): {
					if (args.size() >= 1) {
						std::string FixedString;
						for (std::size_t n = 0; n < (args.size() - 1); ++n) {
							if (n > 0)
								FixedString = FixedString + " " + args.at(n + 1);
							else
								FixedString = FixedString + args.at(n + 1);
						}
						int amount;
						try {
							amount = std::stoi(args.at(0));
						}
						catch (std::exception) {
							amount = 1;
						}
						catch (...) {
							amount = 1;
						}
						ConsoleOutput(args.at(0));
						for (int n = 0; n < amount; ++n) {
							rbxlua_getglobal(CluaState, "ChatPlease");
							//if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_pushvalue(CluaState, -2);
							rbxlua_pushstring(CluaState, FixedString.c_str());
							rbxlua_pcall(CluaState, 2, 0, 0); Sleep(100);
							//}
							clearStack(CluaState);
						}
					}
					break;
				}
				case _INT("guiservice"): case _INT("gs"): {
					std::string ok = args.back();
					rbxlua_getservice(CluaState, "GuiService");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getfield(CluaState, -1, "OpenBrowserWindow");
						//rbxlua_getfield(CluaState, -1, "SetErrorMessage");
						rbxlua_pushvalue(CluaState, -2);
						rbxlua_pushstring(CluaState, ok.c_str());//"http://www.roblox.com/");
						rbxlua_pcall(CluaState, 2, 0, 0);
					}; Sleep(100);
					break;
				}
				/*case _INT("timeout"): {
					//if (CrashFix(args.back()) != 0) {
					std::cout << "timeout ran!\n";
					double timeout = stod(args.back());
					//}
					break;
				}*/
				case _INT("exit"): {
					exit(-1);
					break;
				}
				case _INT("commands"): case _INT("cmds"): {
					MessageBoxA(0, Commands, "Cmds", 0);
					break;
				}
				case _INT("clear"): case _INT("clr"): {
					ClearConsoleOutput();
					if (DoesConsoleExist() == TRUE) {
						system("CLS");
					}
					break;
				}
				case _INT("reset"): {
					Sleep(500);
					break;
				}
				case _INT("break"): {
					Sleep(500);
					break;
				}
				case _INT("stop"): {
					Sleep(500);
					break;
				}
				case _INT("kick"): case _INT("nil"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Player = *GetName(players[i]);
						if (Player != "0") {
							if (Player == ProtectWaffle) {
								if (*GetName(LocalPlayer) == ProtectWaffle) {
									rbxlua_getservice(CluaState, "Players");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, Player.c_str());
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Destroy");
											if (rbxlua_isfunction(CluaState, -1)) {
												rbxlua_pushvalue(CluaState, -2);
												rbxlua_pcall(CluaState, 1, 0, 0); Sleep(10);
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
							}
							else {
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Player.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Destroy");
										if (rbxlua_isfunction(CluaState, -1)) {
											rbxlua_pushvalue(CluaState, -2);
											rbxlua_pcall(CluaState, 1, 0, 0); Sleep(10);
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("httpget"): {
					if (args.size() >= 1) {
						ConsoleOutput(std::string(HttpGet(args.at(0))));
					}
					break;
				}
				case _INT("ht"): {
					if (args.size() >= 1) {
						ConsoleOutput(std::string(HttpGetAsync(args.at(0))));
					}
					break;
				}
				/*case _INT("explode"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							if (TorsoExists(players[i])) {
								////rbxlua_emptystack(CluaState);
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Torso");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_setfield(CluaState, -1, "Position");

												rbxlua_getglobal(CluaState, "Instance");
												rbxlua_getfield(CluaState, -1, "new");
												rbxlua_pushstring(CluaState, "Explosion");
												rbxlua_pushvalue(CluaState, -5);
												rbxlua_pcall(CluaState, 2, 1, 0);

												rbxlua_pushvalue(CluaState, -5);
												rbxlua_setfield(CluaState, -2, "Position"); Sleep(20);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}*/
				case _INT("ff"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							if (TorsoExists(players[i])) {
								////rbxlua_emptystack(CluaState);
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getglobal(CluaState, "Instance");
											rbxlua_getfield(CluaState, -1, "new");
											rbxlua_pushstring(CluaState, "ForceField");
											rbxlua_pushvalue(CluaState, -4);
											rbxlua_pcall(CluaState, 2, 0, 0);
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("tms"): {
					try {
						testmest = std::stoi(args.at(0));
					}
					catch (std::invalid_argument e) {}
				}
				case _INT("disconnect"): case _INT("ds"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							if (TorsoExists(players[i])) {
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "HumanoidRootPart");// std::cout << "2";
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "CFrame");// std::cout << "3";
												//rbxlua_setglobal(CluaState, "DisconnectingCFrame");
												if (!rbxlua_isnil(CluaState, -1)) { 
													rbxlua_getglobal(CluaState, "Instance");
													rbxlua_getfield(CluaState, -1, "new");
													rbxlua_pushstring(CluaState, "SkateboardPlatform");
													rbxlua_pushvalue(CluaState, -6);
													rbxlua_pcall(CluaState, 2, 0, 0);

													rbxlua_getglobal(CluaState, "Instance");
													rbxlua_getfield(CluaState, -1, "new");
													rbxlua_pushstring(CluaState, "SkateboardPlatform");
													rbxlua_pushvalue(CluaState, -7);
													rbxlua_pcall(CluaState, 2, 1, 0);

													//rbxlua_getglobal(CluaState, "DisconnectingCFrame");
													rbxlua_pushvalue(CluaState, -4);// std::cout << "7";
													rbxlua_setfield(CluaState, -2, "CFrame"); Sleep(30);
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("sparkles"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							if (TorsoExists(players[i])) {
								////rbxlua_emptystack(CluaState);
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Torso");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getglobal(CluaState, "Instance");
												rbxlua_getfield(CluaState, -1, "new");
												rbxlua_pushstring(CluaState, "Sparkles");
												rbxlua_pushvalue(CluaState, -4);
												rbxlua_pcall(CluaState, 2, 0, 0);
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("smoke"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							if (TorsoExists(players[i])) {
								////rbxlua_emptystack(CluaState);
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Torso");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getglobal(CluaState, "Instance");
												rbxlua_getfield(CluaState, -1, "new");
												rbxlua_pushstring(CluaState, "Smoke");
												rbxlua_pushvalue(CluaState, -4);
												rbxlua_pcall(CluaState, 2, 0, 0);
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("fire"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							if (TorsoExists(players[i])) {
								////rbxlua_emptystack(CluaState);
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Torso");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getglobal(CluaState, "Instance");
												rbxlua_getfield(CluaState, -1, "new");
												rbxlua_pushstring(CluaState, "Fire");
												rbxlua_pushvalue(CluaState, -4);
												rbxlua_pcall(CluaState, 2, 0, 0);
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("fling"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							int Torso = TorsoExists(players[i]);
							if (Torso) {
								if (!FindFirstClass(Torso, "BodyForce")) {
									rbxlua_getservice(CluaState, "Players");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, Target.c_str());
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Character");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "Torso");
												if (!rbxlua_isnil(CluaState, -1)) {

													rbxlua_getglobal(CluaState, "Instance");
													rbxlua_getfield(CluaState, -1, "new");
													rbxlua_pushstring(CluaState, "BodyForce");
													rbxlua_pushvalue(CluaState, -4);
													rbxlua_pcall(CluaState, 2, 1, 0);
													rbxlua_getglobal(CluaState, "Vector3");
													rbxlua_getfield(CluaState, -1, "new");
													rbxlua_pushstring(CluaState, "25000"); // 99999
													rbxlua_pushstring(CluaState, "25000"); // 99999
													rbxlua_pushstring(CluaState, "25000"); // 99999

													rbxlua_pcall(CluaState, 3, 1, 0);
													rbxlua_setfield(CluaState, -3, "force"); Sleep(20);
													//rbxlua_pcall(CluaState, 0, 0, 0); 
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
							}
						}
					}
					break;
				}
				case _INT("customhead"): case _INT("headsize"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						double CustomSize;
						try {
							CustomSize = std::stod(args.back());
						}
						catch (std::invalid_argument e) {
							CustomSize = 1;
						}
						if (Target != "0") {
							if (TorsoExists(players[i])) {
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Head");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "Mesh");
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getglobal(CluaState, "Vector3");
													rbxlua_getfield(CluaState, -1, "new");
													rbxlua_pushnumber(CluaState, double(CustomSize));
													rbxlua_pushnumber(CluaState, double(CustomSize));
													rbxlua_pushnumber(CluaState, double(CustomSize));
													rbxlua_pcall(CluaState, 3, 1, 0);

													rbxlua_setfield(CluaState, -3, "Scale"); Sleep(10);
													//rbxlua_pcall(CluaState, 0, 0, 0); 
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("hatsize"): case _INT("hatssize"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						double CustomSize;
						try {
							CustomSize = std::stod(args.back());
						}
						catch (std::invalid_argument e) {
							CustomSize = 1;
						}
						int Character = GetCharacterFromPlayer(players[i]);
						if (Character > 1000) {
							std::vector<DWORD> PlayersChildren = GetChildren(Character);
							for (std::size_t j = 0; j < PlayersChildren.size(); j++) {
								std::string ClassType = GetClass(PlayersChildren[j]);
								if (ClassType == "Hat") {
									std::string HatName = *GetName(PlayersChildren[j]);
									if (Target != "0") {
										if (TorsoExists(players[i])) {
											rbxlua_getservice(CluaState, "Players");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, Target.c_str());
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getfield(CluaState, -1, "Character");
													if (!rbxlua_isnil(CluaState, -1)) {
														rbxlua_getfield(CluaState, -1, HatName.c_str());//"Head");
														if (!rbxlua_isnil(CluaState, -1)) {
															rbxlua_getfield(CluaState, -1, "Handle");
															if (!rbxlua_isnil(CluaState, -1)) {
																rbxlua_getfield(CluaState, -1, "Mesh");
																if (!rbxlua_isnil(CluaState, -1)) {
																	rbxlua_getglobal(CluaState, "Vector3");
																	rbxlua_getfield(CluaState, -1, "new");
																	rbxlua_pushnumber(CluaState, double(CustomSize));
																	rbxlua_pushnumber(CluaState, double(CustomSize));
																	rbxlua_pushnumber(CluaState, double(CustomSize));
																	rbxlua_pcall(CluaState, 3, 1, 0);

																	rbxlua_setfield(CluaState, -3, "Scale"); Sleep(10);
																	//rbxlua_pcall(CluaState, 0, 0, 0); 
																}
																else {
																	clearStack(CluaState);
																}
															}
															else {
																clearStack(CluaState);
															}
														}
														else {
															clearStack(CluaState);
														}
													}
													else {
														clearStack(CluaState);
													}
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("bighead"): case _INT("fathead"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							if (TorsoExists(players[i])) {
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Head");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "Mesh");
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getglobal(CluaState, "Vector3");
													rbxlua_getfield(CluaState, -1, "new");
													rbxlua_pushstring(CluaState, "3");
													rbxlua_pushstring(CluaState, "3");
													rbxlua_pushstring(CluaState, "3");
													rbxlua_pcall(CluaState, 3, 1, 0);

													rbxlua_setfield(CluaState, -3, "Scale"); Sleep(15);
													//rbxlua_pcall(CluaState, 0, 0, 0); 
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("smallhead"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							if (TorsoExists(players[i])) {
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Head");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "Mesh");
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getglobal(CluaState, "Vector3");
													rbxlua_getfield(CluaState, -1, "new");
													rbxlua_pushnumber(CluaState, double(0.75));
													rbxlua_pushnumber(CluaState, double(0.75));
													rbxlua_pushnumber(CluaState, double(0.75));
													rbxlua_pcall(CluaState, 3, 1, 0);

													rbxlua_setfield(CluaState, -3, "Scale"); Sleep(10);
													//rbxlua_pcall(CluaState, 0, 0, 0);
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("luapbs"): {
					int PSRRank = 128;
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						*(BYTE *)(players[i] + 176) = 1;

						rbxlua_getservice(CluaState, "Players");
						rbxlua_getfield(CluaState, -1, Target.c_str());
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_pushnumber(CluaState, double(PSRRank));
							rbxlua_setfield(CluaState, -2, "PersonalServerRank");
							//rbxlua_pcall(CluaState, 0, 0, 0);
						}
						else {
							std::cout << "Player not found..." << std::endl;
						}
						/*std::string Target = *GetName(players[i]);
						if (Target != "0") {
						rbxlua_getservice(CluaState, "Players");
						if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getfield(CluaState, -1, Target.c_str());
						if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_pushnumber(CluaState, 128);
						rbxlua_setfield(CluaState, -2, "PersonalServerRank");
						rbxlua_pcall(CluaState, 1, 0, 0);
						}
						}
						clearStack(CluaState);
						}*/
					}
					break;
				}
				case _INT("char"): case _INT("charapp"): {
					if (is_number(args.back()) == true) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							std::string Target = *GetName(players[i]);
							if (Target != "0") {
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										std::string CharApp1 = (std::string)"http://assetgame.roblox.com/Asset/CharacterFetch.ashx?userId=" + (std::string)args.back() + (std::string)"&placeId=0";
										rbxlua_pushstring(CluaState, CharApp1.c_str());
										rbxlua_setfield(CluaState, -2, "CharacterAppearance");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
						ConsoleOutput("[x] To make the appearance change of the player(s) you chose they must reset or must you kill them.\n");
					}
					else {
						std::string newCharacterAppearance = "0";
						std::string PersonWithStuff = GetPlayerName(args.back());
						if (PersonWithStuff != "0") {
							rbxlua_getservice(CluaState, "Players");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, PersonWithStuff.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "CharacterAppearance");
									newCharacterAppearance = rbxlua_tostring(CluaState, -1);
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
							for (std::size_t i = 0; i < players.size(); ++i) {
								std::string Target = *GetName(players[i]);
								if (Target != "0" && PersonWithStuff != "0") {
									if (newCharacterAppearance != "0") {
										rbxlua_getservice(CluaState, "Players");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, Target.c_str());
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_pushstring(CluaState, newCharacterAppearance.c_str());
												rbxlua_setfield(CluaState, -2, "CharacterAppearance");
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
								}
							}
						}
						ConsoleOutput("[x] To make the appearance change of the player(s) you chose they must reset or must you kill them.\n");
					}
					break;
				}
				case _INT("lockcam"): case _INT("camlock"): case _INT("fp"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							rbxlua_getservice(CluaState, "Players");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, Target.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_pushnumber(CluaState, 1);
									rbxlua_setfield(CluaState, -2, "CameraMaxZoomDistance");
									//rbxlua_pcall(CluaState, 0, 0, 0);
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("unlockcam"): case _INT("camunlock"): case _INT("uncamlock"): case _INT("unfp"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							rbxlua_getservice(CluaState, "Players");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, Target.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_pushnumber(CluaState, 400);
									rbxlua_setfield(CluaState, -2, "CameraMaxZoomDistance");
									//rbxlua_pcall(CluaState, 0, 0, 0);
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("camdist"): case _INT("camdistance"): case _INT("fov"): {
					if (CrashFix(args.back()) != 0) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							std::string Target = *GetName(players[i]);
							if (Target != "0") {
								float dist = std::stof(args.back());
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, dist);
										rbxlua_setfield(CluaState, -2, "CameraMaxZoomDistance");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("lolwot"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							rbxlua_getservice(CluaState, "Players");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, Target.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									std::string CharApp2 = (std::string)"http://assetgame.roblox.com/Asset/CharacterFetch.ashx?userId=1311&placeId=0";
									rbxlua_pushstring(CluaState, CharApp2.c_str());
									rbxlua_setfield(CluaState, -2, "CharacterAppearance");
									//rbxlua_pcall(CluaState, 0, 0, 0);
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					ConsoleOutput("[x] To make the appearance change of the player(s) you chose they must reset or you must kill them.\n");
					break;
				}
				case _INT("rtools"): case _INT("cleartools"): case _INT("removetools"): case _INT("notools"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							rbxlua_getservice(CluaState, "Players");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, Target.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Backpack");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "ClearAllChildren");
										if (rbxlua_isfunction(CluaState, -1)) {
											rbxlua_pushvalue(CluaState, -2);
											rbxlua_pcall(CluaState, 1, 0, 0);
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}; Sleep(30);
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("printidentity"): {
					rbxlua_getglobal(CluaState, "printidentity");
					rbxlua_pcall(CluaState, 0, 0, 0);
					break;
				}
				case _INT("check"): {
				
				}
				case _INT("globaltest"): {
					rbxlua_getservice(CluaState, "ContentProvider");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getfield(CluaState, -1, "Preload");
						if (rbxlua_isfunction(CluaState, -1)) {
							rbxlua_pushvalue(CluaState, -2);
							rbxlua_pushstring(CluaState, "http://assetgame.roblox.com/asset?id=66416616");
							rbxlua_pcall(CluaState, 2, 0, 0);
						}
					}
					clearStack(CluaState);
					break;
				}
										 /*case _INT("cookies"): {
										 rbxlua_getservice(CluaState, "CookieService");
										 rbxlua_getfield(CluaState, -1, "GetCookieValue");
										 rbxlua_pushvalue(CluaState, -2);
										 rbxlua_pushstring(CluaState, "rbx_evt_ftp");
										 rbxlua_pcall(CluaState, 2, 1, 0);

										 std::string cookie = rbxlua_tostring(CluaState, -1);
										 std::cout << cookie;
										 Sleep(2500);
										 break;
										 }*/
				case _INT("changestat"): case _INT("statchange"): {
					if (args.size() >= 2) {
						for (std::size_t i = 0; i < players.size(); ++i) {
							std::string FixedString;
							std::string TestString;
							int TargetPlayer = players[i];
							std::string Target = *GetName(players[i]);
							for (std::size_t n = 0; n < (args.size() - 2); ++n) {
								if (n > 0) {
									FixedString = FixedString + " " + args.at(n + 2);
								}
								else {
									FixedString = FixedString + args.at(n + 2);
								}
							}
							std::vector<DWORD> PlayersChildren = GetChildren(players[i]);
							for (std::size_t j = 0; j < PlayersChildren.size(); j++) {
								std::string leaderboardType = to_lowercase(*GetName(PlayersChildren[j]));
								std::string leaderboardTypeNormal = *GetName(PlayersChildren[j]);
								const char* holderName = leaderboardType.c_str();
								if (std::strstr(holderName, "stat") || std::strstr(holderName, "data") || std::strstr(holderName, "supercool") || std::strstr(holderName, "leaderboard")) {
									int leaderstats = FindFirstChild_lower(TargetPlayer, leaderboardType); Sleep(10);
									if (leaderstats != 0) {
										//std::string statName = GetChildName(leaderstats, args.at(1), 0);
										int stat = FindFirstChild_lower(leaderstats, args.at(1));//statName);
										if (stat != 0) {
											rbxlua_getservice(CluaState, "Players");
											rbxlua_getfield(CluaState, -1, Target.c_str());
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, leaderboardTypeNormal.c_str());
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getfield(CluaState, -1, (*GetName(stat)).c_str());
													if (!rbxlua_isnil(CluaState, -1)) {
														rbxlua_pushstring(CluaState, FixedString.c_str());
														rbxlua_setfield(CluaState, -2, "Value"); Sleep(25);
													}
													else {
														clearStack(CluaState);
													}
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("loadgame"): {
					std::string msg = args.back();
					LoadGame(msg); Sleep(5000);
					break;
				}
				case _INT("loadasset"): {
					std::string msg = args.back();
					LoadInAsset(msg);
				}
				case _INT("print"): {
					// http://i.imgur.com/I9ehK7V.png // http://i.imgur.com/oUdXbW2.png
					// 0 is white, 1 is blue, 2 is oragen, 3 is red
					//std::string msg = args.back();
					//int num = 0;
					//for (int x = 0; x != 5; x++) {
					//rbxlua_emptystack(CluaState);
					//rbxlua_settop(CluaState, 0);
					//rbxlua_pushstring(CluaState, msg.c_str());
					//rbxlua_printf(CluaState, &num);
					//rbxlua_emptystack(CluaState);
					//}
					std::string msg = args.back();
					rbxlua_getglobal(CluaState, "print");
					rbxlua_pushstring(CluaState, msg.c_str());
					rbxlua_pcall(CluaState, 1, 0, 0);
					break;
				}
				case _INT("goto"): case _INT("to"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Finished = 0;
						std::string Target1 = *GetName(LocalPlayer);
						std::string Target2 = *GetName(players[i]);
						if (Target1 != Target2) {
							if (Target1 != "0" && Target2 != "0") {
								if (TorsoExists(players[i]) && TorsoExists(LocalPlayer)) {
									rbxlua_getservice(CluaState, "Players");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, Target2.c_str());// std::cout << "1";
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Character");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "HumanoidRootPart");// std::cout << "2";
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getfield(CluaState, -1, "CFrame");// std::cout << "3";
																							//////////////////////////////////////
													rbxlua_getservice(CluaState, "Players");
													if (!rbxlua_isnil(CluaState, -1)) {
														rbxlua_getfield(CluaState, -1, Target1.c_str());// std::cout << "5";
														if (!rbxlua_isnil(CluaState, -1)) {
															rbxlua_getfield(CluaState, -1, "Character");
															if (!rbxlua_isnil(CluaState, -1)) {
																rbxlua_getfield(CluaState, -1, "HumanoidRootPart");// std::cout << "6";
																if (!rbxlua_isnil(CluaState, -1)) {
																	rbxlua_pushvalue(CluaState, -5);// std::cout << "7";
																	rbxlua_setfield(CluaState, -2, "CFrame"); Sleep(15);
																	//rbxlua_pcall(CluaState, 0, 0, 0);
																}
																else {
																	clearStack(CluaState);
																}
															}
															else {
																clearStack(CluaState);
															}
														}
														else {
															clearStack(CluaState);
														}
													}
													else {
														clearStack(CluaState);
													}
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
							}
						}
					}
					break;
				}
				case _INT("bring"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Finished = 0;
						std::string Target1 = *GetName(players[i]);
						std::string Target2 = *GetName(LocalPlayer);
						if (TorsoExists(players[i]) && TorsoExists(LocalPlayer)) {
							if (Target1 != Target2) {
								if (Target1 != "0" && Target2 != "0") {
									rbxlua_getservice(CluaState, "Players");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, Target2.c_str());// std::cout << "1";
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Character");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "HumanoidRootPart");// std::cout << "2";
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getfield(CluaState, -1, "CFrame");// std::cout << "3";
																							//////////////////////////////////////
													if (!rbxlua_isnil(CluaState, -1)) {
														rbxlua_getservice(CluaState, "Players");
														if (!rbxlua_isnil(CluaState, -1)) {
															rbxlua_getfield(CluaState, -1, Target1.c_str());// std::cout << "5";
															if (!rbxlua_isnil(CluaState, -1)) {
																rbxlua_getfield(CluaState, -1, "Character");
																if (!rbxlua_isnil(CluaState, -1)) {
																	rbxlua_getfield(CluaState, -1, "HumanoidRootPart");// std::cout << "6";
																	if (!rbxlua_isnil(CluaState, -1)) {
																		rbxlua_pushvalue(CluaState, -5);// std::cout << "7";
																		rbxlua_setfield(CluaState, -2, "CFrame"); Sleep(25);
																	}
																	else {
																		clearStack(CluaState);
																	}
																}
																else {
																	clearStack(CluaState);
																}
															}
															else {
																clearStack(CluaState);
															}
														}
														else {
															clearStack(CluaState);
														}
													}
													else {
														clearStack(CluaState);
													}
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
							}
						}
					}
					break;
				}
				case _INT("tp"): case _INT("teleport"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Finished = 0;
						std::string Target1 = *GetName(players[i]);
						std::string Target2 = GetPlayerName(args.back());
						int T2 = GetPlayer(Target2);
						if (TorsoExists(players[i]) && TorsoExists(T2)) {
							if (Target1 != Target2) {
								if (Target1 != "0" && Target2 != "0") {
									rbxlua_getservice(CluaState, "Players");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, Target2.c_str());// std::cout << "1";
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Character");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getfield(CluaState, -1, "HumanoidRootPart");// std::cout << "2";
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_getfield(CluaState, -1, "CFrame");// std::cout << "3";
																							//////////////////////////////////////
													if (!rbxlua_isnil(CluaState, -1)) {
														rbxlua_getservice(CluaState, "Players");
														if (!rbxlua_isnil(CluaState, -1)) {
															rbxlua_getfield(CluaState, -1, Target1.c_str());// std::cout << "5";
															if (!rbxlua_isnil(CluaState, -1)) {
																rbxlua_getfield(CluaState, -1, "Character");
																if (!rbxlua_isnil(CluaState, -1)) {
																	rbxlua_getfield(CluaState, -1, "HumanoidRootPart");// std::cout << "6";
																	if (!rbxlua_isnil(CluaState, -1)) {
																		rbxlua_pushvalue(CluaState, -5);// std::cout << "7";
																		rbxlua_setfield(CluaState, -2, "CFrame"); Sleep(25);
																	}
																	else {
																		clearStack(CluaState);
																	}
																}
																else {
																	clearStack(CluaState);
																}
															}
															else {
																clearStack(CluaState);
															}
														}
														else {
															clearStack(CluaState);
														}
													}
													else {
														clearStack(CluaState);
													}
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
							}
						}
					}
					break;
				}
				case _INT("rskybox"): case _INT("removeskybox"): {
					int Skybox = FindFirstClass(Lighting, "Sky");
					if (Skybox != 0) {
						cDestroy(Lighting, "Sky"); Sleep(10);
					}
					break;
				}
				case _INT("skybox"): case _INT("setskybox"): {
					int Skybox = FindFirstClass(Lighting, "Sky");
					if (Skybox != 0) {
						cDestroy(Lighting, "Sky"); Sleep(10);
					}

					////rbxlua_emptystack(CluaState);
					rbxlua_getservice(CluaState, "Lighting");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getglobal(CluaState, "Instance");
						rbxlua_getfield(CluaState, -1, "new");
						rbxlua_pushstring(CluaState, "Sky");
						rbxlua_pushvalue(CluaState, -4);
						rbxlua_pcall(CluaState, 2, 1, 0); Sleep(15);
						rbxlua_pushstring(CluaState, ("http://assetgame.roblox.com/Asset/?id=" + args.back()).c_str());
						rbxlua_setfield(CluaState, -2, "SkyboxBk");

						rbxlua_pushstring(CluaState, ("http://assetgame.roblox.com/Asset/?id=" + args.back()).c_str());
						rbxlua_setfield(CluaState, -2, "SkyboxDn");

						rbxlua_pushstring(CluaState, ("http://assetgame.roblox.com/Asset/?id=" + args.back()).c_str());
						rbxlua_setfield(CluaState, -2, "SkyboxFt");

						rbxlua_pushstring(CluaState, ("http://assetgame.roblox.com/Asset/?id=" + args.back()).c_str());
						rbxlua_setfield(CluaState, -2, "SkyboxLf");

						rbxlua_pushstring(CluaState, ("http://assetgame.roblox.com/Asset/?id=" + args.back()).c_str());
						rbxlua_setfield(CluaState, -2, "SkyboxRt");

						rbxlua_pushstring(CluaState, ("http://assetgame.roblox.com/Asset/?id=" + args.back()).c_str());
						rbxlua_setfield(CluaState, -2, "SkyboxUp");
					}
					else {
						clearStack(CluaState);
					}
					break;
				}
				case _INT("clone"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						int Character = GetCharacterFromPlayer(players[i]);
						if (Character != 0) {
							rbxlua_getservice(CluaState, "Players");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, Target.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Character");
									if (!rbxlua_isnil(CluaState, -1)) {
										if (CheckIfArchivable(Character) != TRUE) {
											rbxlua_pushbool(CluaState, TRUE);
											rbxlua_setfield(CluaState, -2, "Archivable"); Sleep(10);
										}

										rbxlua_getfield(CluaState, -1, "Clone");
										if (rbxlua_isfunction(CluaState, -1)) {
											rbxlua_pushvalue(CluaState, -2);
											rbxlua_pcall(CluaState, 1, 1, 0); Sleep(5); // -3
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getglobal(CluaState, "workspace");
												if (!rbxlua_isnil(CluaState, -1)) {
													rbxlua_pushvalue(CluaState, -1);
													rbxlua_setfield(CluaState, -3, "Parent"); Sleep(20);
													//rbxlua_pcall(CluaState, 0, 0, 0); 
												}
												else {
													clearStack(CluaState);
												}
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("pokemon"): {
					std::string Target = *GetName(LocalPlayer);
					if (Target != "" && Target != "0") {
						rbxlua_getservice(CluaState, "Players");
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Backpack");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "NoJump/Reset");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushbool(CluaState, TRUE);
										rbxlua_setfield(CluaState, -2, "Disabled"); Sleep(10);
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
						else {
							clearStack(CluaState);
						}
					}
					break;
				}
				case _INT("changeteams"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (args.size() >= 2) {
							std::string newTeam;
							try {
								newTeam = args.at(1);
							}
							catch (std::invalid_argument e) {
								break;
							}
							int TeamCheck = FindFirstChild_lower(Teams, newTeam);
							if (TeamCheck) {
								ConsoleOutput("Team found! ");
								std::string Target = *GetName(players[i]);
								std::string TeamName = *GetName(TeamCheck);

								ConsoleOutput(TeamName);
								const char* brickColor = "";

								rbxlua_getservice(CluaState, "Teams");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, TeamName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "TeamColor");
										brickColor = rbxlua_tostring(CluaState, -1);
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
								ConsoleOutput(brickColor);
								ConsoleOutput("----");
								if (brickColor != "") {
									rbxlua_getservice(CluaState, "Players");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, Target.c_str());
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getglobal(CluaState, "BrickColor");
											rbxlua_getfield(CluaState, -1, "new");
											rbxlua_pushstring(CluaState, newTeam.c_str());

											rbxlua_pcall(CluaState, 1, 1, 0);
											rbxlua_setfield(CluaState, -3, "TeamColor"); Sleep(20);
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}

								ConsoleOutput(brickColor);
							}
						}
					}
					break;
				}
				case _INT("jpower"): case _INT("jumppower"): case _INT("jp"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (args.size() >= 2) {
							float power;
							try {
								power = std::stof(args.at(1));
							}
							catch (std::invalid_argument e) {
								power = 50;
							}
							std::string Target = *GetName(players[i]);
							std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
							if (HumanoidName != "0") {
								rbxlua_getservice(CluaState, "Players");
								rbxlua_getfield(CluaState, -1, Target.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Character");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_pushnumber(CluaState, double(power));
											rbxlua_setfield(CluaState, -2, "JumpPower");
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("confuse"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						double speed = -16;
						std::string Target = *GetName(players[i]);
						std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
						if (HumanoidName != "0") {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, double(speed));
										rbxlua_setfield(CluaState, -2, "WalkSpeed");
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("unconfuse"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						double speed = 16;
						std::string Target = *GetName(players[i]);
						std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
						if (HumanoidName != "0") {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, double(speed));
										rbxlua_setfield(CluaState, -2, "WalkSpeed");
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("ws"): case _INT("walkspeed"): case _INT("speed"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (args.size() >= 2) {
							float speed;
							try {
								speed = std::stof(args.at(1));
							}
							catch (std::invalid_argument e) {
								speed = 16;
							}
							std::string Target = *GetName(players[i]);
							std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, double(speed));
										rbxlua_setfield(CluaState, -2, "WalkSpeed");
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("health"): case _INT("sethealth"): case _INT("hp"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						if (args.size() >= 2) {
							float health;
							try {
								health = std::stof(args.at(1));
							}
							catch (std::invalid_argument e) {
								health = 100;
							}
							std::string Target = *GetName(players[i]);
							std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
							if (HumanoidName != "0") {
								rbxlua_getservice(CluaState, "Players");
								rbxlua_getfield(CluaState, -1, Target.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Character");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_pushnumber(CluaState, double(health));
											rbxlua_setfield(CluaState, -2, "MaxHealth");

											rbxlua_pushnumber(CluaState, double(health));
											rbxlua_setfield(CluaState, -2, "Health");
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				case _INT("heal"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						float health = 10000000;
						std::string Target = *GetName(players[i]);
						std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
						if (HumanoidName != "0") {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, double(health));
										rbxlua_setfield(CluaState, -2, "Health");
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("sgod"): case _INT("semigod"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						float health = 10000000;
						std::string Target = *GetName(players[i]);
						std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
						if (HumanoidName != "0") {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, double(health));
										rbxlua_setfield(CluaState, -2, "MaxHealth");

										rbxlua_pushnumber(CluaState, double(health));
										rbxlua_setfield(CluaState, -2, "Health");
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("god"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						float health = (float)HUGE_VAL;
						std::string Target = *GetName(players[i]);
						std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
						if (HumanoidName != "0") {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, double(health));
										rbxlua_setfield(CluaState, -2, "MaxHealth");

										rbxlua_pushnumber(CluaState, double(health));
										rbxlua_setfield(CluaState, -2, "Health");
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("ungod"): case _INT("nogod"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						float health = 100;
						std::string Target = *GetName(players[i]);
						std::string HumanoidName = *GetName(GetHumanoidFromPlayer(players[i]));
						if (HumanoidName != "0") {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, HumanoidName.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, double(health));
										rbxlua_setfield(CluaState, -2, "MaxHealth");

										rbxlua_pushnumber(CluaState, double(health));
										rbxlua_setfield(CluaState, -2, "Health");
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("bypass"): {
					AttemptToBypassAntiHax = TRUE;
					break;
				}
				case _INT("nobypass"): {
					AttemptToBypassAntiHax = FALSE;
					break;
				}
				case _INT("name"): case _INT("setname"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						std::string FixedString;
						for (std::size_t n = 0; n < (args.size() - 1); ++n) {
							if (n > 0)
								FixedString = FixedString + " " + args.at(n + 1);
							else
								FixedString = FixedString + args.at(n + 1);
						}
						if (Target != FixedString) {
							rbxlua_getservice(CluaState, "Players");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, Target.c_str());// std::cout << "1";
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Character");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushstring(CluaState, FixedString.c_str());
										rbxlua_setfield(CluaState, -2, "Name"); Sleep(15);
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("unname"): case _INT("dename"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Target = *GetName(players[i]);
						rbxlua_getservice(CluaState, "Players");
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_getfield(CluaState, -1, Target.c_str());// std::cout << "1";
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_pushstring(CluaState, Target.c_str());
									rbxlua_setfield(CluaState, -2, "Name"); Sleep(15);
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
						else {
							clearStack(CluaState);
						}
					}
					break;
				}
				case _INT("stopmusic"): case _INT("stopsound"): case _INT("nomusic"): case _INT("nosound"): {
					int sound = FindFirstClass(Workspace, "Sound");
					if (sound != 0) {
						std::string soundName = *GetName(sound);
						rbxlua_getservice(CluaState, "Workspace");
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_getfield(CluaState, -1, soundName.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Destroy");
								if (rbxlua_isfunction(CluaState, -1)) {
									rbxlua_pushvalue(CluaState, -2);
									rbxlua_pcall(CluaState, 1, 0, 0); Sleep(5);
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
						else {
							clearStack(CluaState);
						}
					}
					break;
				}
				case _INT("music"): case _INT("sound"): case _INT("play"): {
					int sound = FindFirstClass(Workspace, "Sound");
					if (sound != 0) {
						std::string soundName = *GetName(sound);
						rbxlua_getservice(CluaState, "Workspace");
						if (!rbxlua_isnil(CluaState, -1)) {
							rbxlua_getfield(CluaState, -1, soundName.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Stop");
								if (rbxlua_isfunction(CluaState, -1)) {
									rbxlua_pushvalue(CluaState, -2);
									rbxlua_pcall(CluaState, 1, 0, 0); Sleep(10);
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
						else {
							clearStack(CluaState);
						}
					}
					rbxlua_getservice(CluaState, "Workspace");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getglobal(CluaState, "Instance");
						rbxlua_getfield(CluaState, -1, "new");
						rbxlua_pushstring(CluaState, "Sound");
						rbxlua_pushvalue(CluaState, -4);
						rbxlua_pcall(CluaState, 2, 1, 0);

						rbxlua_pushstring(CluaState, ("rbxassetid://" + args.back()).c_str());
						rbxlua_setfield(CluaState, -2, "SoundId");

						rbxlua_pushstring(CluaState, "");
						rbxlua_setfield(CluaState, -2, "Name");

						rbxlua_pushnumber(CluaState, double(1));
						rbxlua_setfield(CluaState, -2, "Volume");

						rbxlua_getfield(CluaState, -1, "Play");
						rbxlua_pushvalue(CluaState, -2);
						rbxlua_pcall(CluaState, 1, 0, 0); Sleep(15);

						rbxlua_getfield(CluaState, -1, "Looped");
						rbxlua_pushbool(CluaState, TRUE);
						rbxlua_pcall(CluaState, 1, 0, 0); Sleep(15);
					}
					else {
						clearStack(CluaState);
					}
					break;
				}
				case _INT("waffles"): {
					rbxlua_getservice(CluaState, "Workspace");
					if (!rbxlua_isnil(CluaState, -1)) {
						rbxlua_getglobal(CluaState, "Instance");
						rbxlua_getfield(CluaState, -1, "new");
						rbxlua_pushstring(CluaState, "Sound");
						rbxlua_pushvalue(CluaState, -4);
						rbxlua_pcall(CluaState, 2, 1, 0);

						rbxlua_pushstring(CluaState, "rbxassetid://142995017");
						rbxlua_setfield(CluaState, -2, "SoundId");
						//rbxlua_pcall(CluaState, 0, 0, 0);

						rbxlua_pushstring(CluaState, "");
						rbxlua_setfield(CluaState, -2, "Name");
						//rbxlua_pcall(CluaState, 0, 0, 0);

						rbxlua_pushnumber(CluaState, double(1));
						rbxlua_setfield(CluaState, -2, "Volume");
						//rbxlua_pcall(CluaState, 0, 0, 0);

						rbxlua_getfield(CluaState, -1, "Play");
						rbxlua_pushvalue(CluaState, -2);
						rbxlua_pcall(CluaState, 1, 0, 0);
					}
					else {
						clearStack(CluaState);
					}
					break;
				}
				case _INT("btools"): case _INT("givebtools"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						//int Player = players[i];
						std::string Target = *GetName(players[i]);
						if (Target != "0") {
							rbxlua_getservice(CluaState, "Players");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Backpack");
									if (!rbxlua_isnil(CluaState, -1)) {

										rbxlua_getglobal(CluaState, "Instance");
										rbxlua_getfield(CluaState, -1, "new");
										rbxlua_pushstring(CluaState, "HopperBin");
										rbxlua_pushvalue(CluaState, -4);
										rbxlua_pcall(CluaState, 2, 1, 0);
										rbxlua_pushnumber(CluaState, double(2));
										rbxlua_setfield(CluaState, -2, "BinType");
										//rbxlua_pcall(CluaState, 0, 0, 0);

										rbxlua_getglobal(CluaState, "Instance");
										rbxlua_getfield(CluaState, -1, "new");
										rbxlua_pushstring(CluaState, "HopperBin");
										rbxlua_pushvalue(CluaState, -6);
										rbxlua_pcall(CluaState, 2, 1, 0);
										rbxlua_pushnumber(CluaState, double(3));
										rbxlua_setfield(CluaState, -2, "BinType");
										//rbxlua_pcall(CluaState, 0, 0, 0);

										rbxlua_getglobal(CluaState, "Instance");
										rbxlua_getfield(CluaState, -1, "new");
										rbxlua_pushstring(CluaState, "HopperBin");
										rbxlua_pushvalue(CluaState, -8);
										rbxlua_pcall(CluaState, 2, 1, 0);
										rbxlua_pushnumber(CluaState, double(4));
										rbxlua_setfield(CluaState, -2, "BinType");
										//rbxlua_pcall(CluaState, 0, 0, 0);

									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					break;
				}
				case _INT("faceid"): case _INT("face"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Player = players[i];
						double TransparencyOfParts = 1;
						std::string Target = *GetName(Player);
						std::string DecalId = args.back();
						std::string FullStuff = std::string("rbxassetid://") + std::string(DecalId);
						if (Player != 0) {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Head");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "face");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_pushstring(CluaState, FullStuff.c_str());
											rbxlua_setfield(CluaState, -2, "Texture");
											//rbxlua_pcall(CluaState, 0, 0, 0);
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("unhuman"): case _INT("unresetable"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Player = players[i];
						std::string Target = *GetName(Player);
						if (Player != 0) {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Humanoid");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushstring(CluaState, "sd9jr2oPdasfmff39fasl31ik05I");
										rbxlua_setfield(CluaState, -2, "Name");
									}
								}
							}
						}
					}
					break;
				}
				case _INT("human"): case _INT("resetable"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Player = players[i];
						std::string Target = *GetName(Player);
						int Character = GetCharacterFromPlayer(Player);
						if (Player != 0 && Character != 0) {
							int Humanoid = FindFirstClass(Character, "Humanoid");
							if (Humanoid != 0) {
								std::string humanName = *GetName(Humanoid);
								rbxlua_getservice(CluaState, "Players");
								rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Character");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, humanName.c_str());
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_pushstring(CluaState, "Humanoid");
											rbxlua_setfield(CluaState, -2, "Name");
										}
									}
								}
							}
						}
					}
					break;
				}
				case _INT("invisible"): case _INT("invis"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Player = players[i];
						double TransparencyOfParts = 1;
						std::string Target = *GetName(Player);
						if (Player != 0) {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Head");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
										//////////////////////////////////////
										rbxlua_getfield(CluaState, -1, "face");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_pushnumber(CluaState, TransparencyOfParts);
											rbxlua_setfield(CluaState, -2, "Transparency");
											//rbxlua_pcall(CluaState, 0, 0, 0);
										}
									}
									rbxlua_getfield(CluaState, -3, "Torso");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -4, "Right Arm");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -5, "Left Arm");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -6, "Right Leg");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -7, "Left Leg");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									/*rbxlua_getfield(CluaState, -8, "FakeHead");
									if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_pushnumber(CluaState, TransparencyOfParts);
									rbxlua_setfield(CluaState, -2, "Transparency");
									rbxlua_pcall(CluaState, 0, 0, 0);
									//////////////////////////////////////
									rbxlua_getfield(CluaState, -2, "face");
									if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_pushnumber(CluaState, TransparencyOfParts);
									rbxlua_setfield(CluaState, -2, "Transparency");
									rbxlua_pcall(CluaState, 0, 0, 0);
									}
									}*/
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					clearStack(CluaState);
					break;
				}
				case _INT("ghost"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Player = players[i];
						double TransparencyOfParts = 0.8;
						std::string Target = *GetName(Player);
						if (Player != 0) {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Head");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
										//////////////////////////////////////
										rbxlua_getfield(CluaState, -1, "face");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_pushnumber(CluaState, TransparencyOfParts);
											rbxlua_setfield(CluaState, -2, "Transparency");
											//rbxlua_pcall(CluaState, 0, 0, 0);
										}
									}
									rbxlua_getfield(CluaState, -3, "Torso");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -4, "Right Arm");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -5, "Left Arm");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -6, "Right Leg");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -7, "Left Leg");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									/*rbxlua_getfield(CluaState, -8, "FakeHead");
									if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_pushnumber(CluaState, TransparencyOfParts);
									rbxlua_setfield(CluaState, -2, "Transparency");
									rbxlua_pcall(CluaState, 0, 0, 0);
									//////////////////////////////////////
									rbxlua_getfield(CluaState, -2, "face");
									if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_pushnumber(CluaState, TransparencyOfParts);
									rbxlua_setfield(CluaState, -2, "Transparency");
									rbxlua_pcall(CluaState, 0, 0, 0);
									}
									}*/
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					clearStack(CluaState);
					break;
				}
				case _INT("visible"): case _INT("vis"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Player = players[i];
						double TransparencyOfParts = 0;
						std::string Target = *GetName(Player);
						if (Player != 0) {
							rbxlua_getservice(CluaState, "Players");
							rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "Character");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, "Head");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
										//////////////////////////////////////
										rbxlua_getfield(CluaState, -1, "face");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_pushnumber(CluaState, TransparencyOfParts);
											rbxlua_setfield(CluaState, -2, "Transparency");
											//rbxlua_pcall(CluaState, 0, 0, 0);
										}
									}
									rbxlua_getfield(CluaState, -3, "Torso");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -4, "Right Arm");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -5, "Left Arm");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -6, "Right Leg");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									rbxlua_getfield(CluaState, -7, "Left Leg");
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_pushnumber(CluaState, TransparencyOfParts);
										rbxlua_setfield(CluaState, -2, "Transparency");
										//rbxlua_pcall(CluaState, 0, 0, 0);
									}
									/*rbxlua_getfield(CluaState, -8, "FakeHead");
									if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_pushnumber(CluaState, TransparencyOfParts);
									rbxlua_setfield(CluaState, -2, "Transparency");
									rbxlua_pcall(CluaState, 0, 0, 0);
									//////////////////////////////////////
									rbxlua_getfield(CluaState, -2, "face");
									if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_pushnumber(CluaState, TransparencyOfParts);
									rbxlua_setfield(CluaState, -2, "Transparency");
									rbxlua_pcall(CluaState, 0, 0, 0);
									}
									}*/
								}
								else {
									clearStack(CluaState);
								}
							}
							else {
								clearStack(CluaState);
							}
						}
					}
					clearStack(CluaState);
					break;
				}
				case _INT("car"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						std::string Username = *GetName(players[i]);
						if (Username != "0") {
							//int FoldersFound = 0;
							//InsertService = FindFirstClass(DataModel, "InsertService");

							rbxlua_getservice(CluaState, "InsertService");
							if (!rbxlua_isnil(CluaState, -1)) {
								rbxlua_getfield(CluaState, -1, "LoadAssetVersion");//"LoadAsset");
								rbxlua_pushvalue(CluaState, -2);
								rbxlua_pushnumber(CluaState, double(253519495));
								rbxlua_pcall(CluaState, 2, 0, 0);
								/*if (InsertService != 0) {
								int FoldersFound2 = 0;
								int Sword = 0;
								int Tool = TemporaryGetTool("DriveBloxUltimateCar");

								FoldersFound2 = 0;
								if (Sword == 0) {
								Tool = TemporaryGetTool("DriveBloxUltimateCar");
								}

								if (Tool != 0) {
								int Player = GetPlayer(Username);
								if (Player != 0) {
								int Backpack = FindFirstClass(Player, "Backpack");
								if (Backpack != 0) {
								setParent(Tool, Backpack);
								}
								}
								}
								}*/
							}
							//lua_emptystack(lua_State);
							clearStack(CluaState);
						}
					}
					break;
				}
				case _INT("light"): {
					for (std::size_t i = 0; i < players.size(); ++i) {
						int Player = players[i];
						std::string Target = *GetName(Player);
						if (Player != 0) {
							if (TorsoExists(players[i])) {
								////rbxlua_emptystack(CluaState);
								rbxlua_getservice(CluaState, "Players");
								if (!rbxlua_isnil(CluaState, -1)) {
									rbxlua_getfield(CluaState, -1, Target.c_str());
									if (!rbxlua_isnil(CluaState, -1)) {
										rbxlua_getfield(CluaState, -1, "Character");
										if (!rbxlua_isnil(CluaState, -1)) {
											rbxlua_getfield(CluaState, -1, "Torso");
											if (!rbxlua_isnil(CluaState, -1)) {
												rbxlua_getglobal(CluaState, "Instance");
												rbxlua_getfield(CluaState, -1, "new");
												rbxlua_pushstring(CluaState, "SpotLight");
												rbxlua_pushvalue(CluaState, -4);
												rbxlua_pcall(CluaState, 2, 1, 0);

												//rbxlua_pushnumber(CluaState, 180);
												//rbxlua_setfield(CluaState, -2, "Angle");

												rbxlua_pushnumber(CluaState, double(20));
												rbxlua_setfield(CluaState, -2, "Range");
												//rbxlua_pcall(CluaState, 0, 0, 0);

												rbxlua_pushnumber(CluaState, double(100));
												rbxlua_setfield(CluaState, -2, "Brightness"); Sleep(50);
												//rbxlua_pcall(CluaState, 0, 0, 0);

												//rbxlua_pushnumber(CluaState, 10000);
												//rbxlua_setfield(CluaState, -2, "Face");

												//rbxlua_pushnumber(CluaState, 25);
												//rbxlua_setfield(CluaState, -2, "Heat");
											}
											else {
												clearStack(CluaState);
											}
										}
										else {
											clearStack(CluaState);
										}
									}
									else {
										clearStack(CluaState);
									}
								}
								else {
									clearStack(CluaState);
								}
							}
						}
					}
					break;
				}
				default: {
					ConsoleOutput("--> Invalid command <--\r\n"); Sleep(10);
					break;
				}
				}
			}
		}
		sploit = false;
	}
}

/*
else if (command == "luafp") {
for (std::size_t i = 0; i < players.size(); ++i) {
try {
std::string Target = *GetName(players[i]);
//rbxlua_emptystack(CluaState);
rbxlua_getglobal(CluaState, "game");
rbxlua_getfield(CluaState, -1, "Players");
rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_pushnumber(CluaState, 1);
rbxlua_setfield(CluaState, -2, "CameraMaxZoomDistance");
}
}
catch (std::exception e) {
Sleep(10); //std::cout << " - fp breakpoint recovered \n"; Sleep(1);
}
}
}*/
/*
else if (command == "unfp") {
for (std::size_t i = 0; i < players.size(); ++i) {
std::string Target = *GetName(players[i]);
//rbxlua_emptystack(CluaState);
rbxlua_getglobal(CluaState, "game");
rbxlua_getfield(CluaState, -1, "Players");
rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_pushnumber(CluaState, 400);
rbxlua_setfield(CluaState, -2, "CameraMaxZoomDistance");
}
}
}
*/
/*
else if (command == "charapp") {
for (std::size_t i = 0; i < players.size(); ++i) {
try {
std::string Target = *GetName(players[i]);
std::string newUserId = args.back().c_str();
//std::cout << newUserId;
std::string Template = "http://assetgame.roblox.com/Asset/CharacterFetch.ashx?userId=" + newUserId;
//rbxlua_emptystack(CluaState);
rbxlua_getglobal(CluaState, "game");
rbxlua_getfield(CluaState, -1, "Players");
rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_pushstring(CluaState, (const char *)Template.c_str());
rbxlua_setfield(CluaState, -2, "CharacterAppearance");
}; Sleep(50);
////rbxlua_emptystack(CluaState);
}
catch (std::exception e) {
Sleep(10); //std::cout << " - charapp breakpoint recovered \n"; Sleep(1);
}
}
}*/
/*
else if (command == "apocbadges") {
//std::cout << "0";
int Player = GetPlayer(PlayerName);
std::string Target = GetPlayerName(PlayerName, 0);
//std::cout << "1";
int FixedValue;
if (Player != 0) {
//std::cout << "2";
int playerstats = FindFirstChild_PartOf(Player, "playerstats");
if (playerstats != 0) {
//std::cout << "3";
//rbxlua_emptystack(CluaState);
rbxlua_getglobal(CluaState, "game");
rbxlua_getfield(CluaState, -1, "Players");
rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
rbxlua_getfield(CluaState, -1, "playerstats");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_getfield(CluaState, -1, "Days");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_pushnumber(CluaState, 100);
rbxlua_setfield(CluaState, -2, "Value");
}
}
//rbxlua_emptystack(CluaState);
rbxlua_getglobal(CluaState, "game");
rbxlua_getfield(CluaState, -1, "Players");
rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
rbxlua_getfield(CluaState, -1, "playerstats");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_getfield(CluaState, -1, "ZombieKill");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_getfield(CluaState, -1, "Military");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_pushnumber(CluaState, 100);
rbxlua_setfield(CluaState, -2, "Value");
}
}
}
//rbxlua_emptystack(CluaState);
rbxlua_getglobal(CluaState, "game");
rbxlua_getfield(CluaState, -1, "Players");
rbxlua_getfield(CluaState, -1, (const char *)Target.c_str());
rbxlua_getfield(CluaState, -1, "playerstats");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_getfield(CluaState, -1, "PlayerKill");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_getfield(CluaState, -1, "Bandit");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_pushnumber(CluaState, 100);
rbxlua_setfield(CluaState, -2, "Value");
}
}
}
}
}
}

else if (command == "tm") {
std::string FixedString;
int stringSize = args.size() - 2;
if (stringSize > 0) {
if (stringSize == 1) {
std::string part1 = args.back().c_str();
std::string part2 = args.at(2).c_str();
FixedString = part1 + " " + part2;
}
else if (stringSize == 2) {
std::string part1 = args.back().c_str();
std::string part2 = args.at(2).c_str();
std::string part3 = args.at(3).c_str();
FixedString = part1 + " " + part2 + " " + part3;
}
else if (stringSize == 3) {
std::string part1 = args.back().c_str();
std::string part2 = args.at(2).c_str();
std::string part3 = args.at(3).c_str();
std::string part4 = args.at(4).c_str();
FixedString = part1 + " " + part2 + " " + part3 + " " + part4;
}
else if (stringSize == 4) {
std::string part1 = args.back().c_str();
std::string part2 = args.at(2).c_str();
std::string part3 = args.at(3).c_str();
std::string part4 = args.at(4).c_str();
std::string part5 = args.at(5).c_str();
FixedString = part1 + " " + part2 + " " + part3 + " " + part4 + " " + part5;
}
else if (stringSize == 5) {
std::string part1 = args.back().c_str();
std::string part2 = args.at(2).c_str();
std::string part3 = args.at(3).c_str();
std::string part4 = args.at(4).c_str();
std::string part5 = args.at(5).c_str();
std::string part6 = args.at(6).c_str();
FixedString = part1 + " " + part2 + " " + part3 + " " + part4 + " " + part5 + " " + part6;
}
else if (stringSize == 6) {
std::string part1 = args.back().c_str();
std::string part2 = args.at(2).c_str();
std::string part3 = args.at(3).c_str();
std::string part4 = args.at(4).c_str();
std::string part5 = args.at(5).c_str();
std::string part6 = args.at(6).c_str();
std::string part7 = args.at(7).c_str();
FixedString = part1 + " " + part2 + " " + part3 + " " + part4 + " " + part5 + " " + part6 + " " + part7;
}
}
else {
FixedString = args.back().c_str();
}
////rbxlua_emptystack(CluaState); Sleep(1);
rbxlua_getglobal(CluaState, "workspace");
rbxlua_getfield(CluaState, -1, "GameInProgress");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_getfield(CluaState, -1, "Mode");
if (!rbxlua_isnil(CluaState, -1)) {
rbxlua_pushstring(CluaState, (const char *)FixedString.c_str());
rbxlua_setfield(CluaState, -2, "Value");
}
}
}
*/

void cmdLoopJump() {
	do {
		try {
			HandleCommand(loopCommand1); Sleep(450);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "", 0); Sleep(350);
		}
	} while (true);
}

void cmdLoopSit() {
	do {
		try {
			HandleCommand(loopCommand1); Sleep(450);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "", 0); Sleep(450);
		}
	} while (true);
}

void cmdLoop1() {
	do {
		try {
			HandleCommand(loopCommand1); Sleep(1550);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "", 0); Sleep(1200);
		}
		catch (...) {
			//MessageBoxA(0, "A unexpected error has occured!", "", 0); Sleep(1200);
			Sleep(25);
		}
	} while (true);
}

void cmdLoop2() {
	do {
		try {
			HandleCommand(loopCommand2); Sleep(2000);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "", 0); Sleep(1200);
		}
		catch (...) {
			//MessageBoxA(0, "A unexpected error has occured!", "", 0); Sleep(1200);
			Sleep(25);
		}
	} while (true);
}