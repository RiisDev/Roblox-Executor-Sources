#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <string.h>

using namespace std;

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

std::string Input() {
	std::string ass;
	getline(std::cin, ass);
	return ass;
}

DWORD base = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");
DWORD getaddy2(int address) {
	return (address - 0x400000 + base);
}

void ConsoleHacks()
{
	DWORD nothing;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nothing);
	*(BYTE*)(&FreeConsole) = 0xC3;
}

namespace Memory {
	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask) return 0;
		return (*szMask) == NULL;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
	{
		for (int i = 0; i<(int)dwLen; i++)
			if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}

	int Scan(DWORD mode, char* content, char* mask)
	{

		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		for (DWORD lpAddr = 0; lpAddr<0x7FFFFFFF; lpAddr += PageSize)
		{
			DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
			if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
			if (mi.Type == MEM_MAPPED) continue;
			if (mi.Protect == mode)
			{
				int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
				if (addr != 0)
				{
					return addr;
				}
			}
		}

	}


}

int DataModel;
int Workspace;
int Lighting;
int Players;
int lua_State;

int GetParent(int Instance) {
	return *(int*)(Instance + 0x34);
}
std::string* GetName(int Instance) {
	return (std::string*)(*(int*)(Instance + 0x28) - 0x4);
}

const char* GetClass(int self)
{

	return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
}

int FindFirstChild_PartOf(int Instance, std::string Name) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (strstr(GetName(*(int*)i)->c_str(), Name.c_str()))
		{
			if (*(int*)i == 0)
			{

			}
			else {
				return *(int*)i;
			}
		}

	}

}


std::vector<int> GetChildren(int Instance) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	std::vector<int> Children;
	for (int i = *(int*)StartOfChildren; i < EndOfChildren; i += 8)
	{
		Children.push_back(*(int*)i);
	}
	return Children;
}

int GetCharFromPlayer(int Instance) {
	return *(int*)(Instance + 0x48);
}

std::vector<int> GetAllPlayerCharacters(int ParentOfChars, std::string omitted = "none") {
	std::vector<int> PlayersVector = GetChildren(Players);
	std::vector<int> CharList;
	for (int i = 0; i < PlayersVector.size(); i++) {
		if (*GetName(PlayersVector[i]) != omitted) {
			int Character = FindFirstChild_PartOf(ParentOfChars, *GetName(PlayersVector[i]));
			if (Character > 150) {
				CharList.push_back(Character);
			}
		}
	}
	return CharList;
}

int GetLocalPlayer(int Plr) {
	return *(int*)(Plr + 340);
}

int FindFirstClass(int Instance, const char* Name)
{
	DWORD CHILD_START = *(DWORD*)(Instance + 0x2C);
	DWORD CHILD_END = *(DWORD*)(CHILD_START + 4);

	for (int i = *(int*)CHILD_START; i != CHILD_END; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}

bool humanoidstatehax = false;
int humanoidstatetype;


void Console(char* title) {
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}


void Scan()
{

	DWORD DataModelA = getaddy2(0x1170244);
	DataModel = Memory::Scan(PAGE_READWRITE, (char*)&DataModelA, "xxxx");
}

void Scan1()
{
	Workspace = FindFirstClass(DataModel, "Workspace");
	Players = FindFirstClass(DataModel, "Players");
	Lighting = FindFirstClass(DataModel, "Lighting");
}

void setJb(DWORD addr) {
	DWORD oldProtection;
	VirtualProtect((LPVOID)addr, 0x05, PAGE_EXECUTE_READWRITE, &oldProtection);
	*(char*)addr = 0xEB;
	VirtualProtect((LPVOID)addr, 0x05, oldProtection, &oldProtection);
}

void restoreJb(DWORD addr) {
	DWORD oldProtection;
	VirtualProtect((void*)addr, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
	*(char*)addr = 0x72;
	VirtualProtect((void*)addr, 5, oldProtection, &oldProtection);
}

typedef int(__cdecl *Lua_gettop)(int lua_State);
Lua_gettop lua_gettop = (Lua_gettop)getaddy2(0x53A000);
int omgthatscool = 0x53A000;

typedef void(__cdecl *Lua_getfield)(int lua_State, int index, const char *k);
Lua_getfield lua_getfield = (Lua_getfield)getaddy2((omgthatscool-0x1d0));

typedef void(__cdecl *Lua_settop)(int lua_State, int index);
Lua_settop lua_settop = (Lua_settop)getaddy2((omgthatscool+0x15d0));

typedef void(__cdecl *Lua_pushstring)(int lua_State, const char *s);
Lua_pushstring lua_pushstring = (Lua_pushstring)getaddy2((omgthatscool+0xbb0));

typedef void(__cdecl *Lua_pushvalue)(int lua_State, int index);
Lua_pushvalue lua_pushvalue = (Lua_pushvalue)getaddy2((omgthatscool+0xc70));

typedef int(__cdecl *Lua_pcall)(int lua_State, int nargs, int nresults, int errfunc);
Lua_pcall lua_pcall = (Lua_pcall)getaddy2((omgthatscool+0x650));

typedef void(__cdecl *Lua_setfield)(int lua_State, int index, const char *k);
Lua_setfield lua_setfield = (Lua_setfield)getaddy2((omgthatscool+0x1300));

typedef void(__cdecl *Lua_pushnumber)(int lua_State, double n);
Lua_pushnumber lua_pushnumber = (Lua_pushnumber)getaddy2((omgthatscool+0xb30));

void getfield(int state, int index, const char *k) {
	setJb(getaddy2(0x539E92));
	lua_getfield(state, index, k);
	restoreJb(getaddy2(0x539E92));
}

int OMGYAY = 0x539E92;

void settop(int state, int index) {
	setJb(getaddy2((OMGYAY + 0x1799)));
	lua_settop(state, index);
	restoreJb(getaddy2((OMGYAY + 0x1799)));
}

void pushstring(int state, const char *s) {
	setJb(getaddy2((OMGYAY + 0xd85)));
	lua_pushstring(state, s);
	restoreJb(getaddy2((OMGYAY + 0xd85)));
}

void pushvalue(int state, int index) {
	setJb(getaddy2((OMGYAY + 0xe18)));
	lua_pushvalue(state, index);
	restoreJb(getaddy2((OMGYAY + 0xe18)));
}

void pcall(int state, int nargs, int nresults, int errfunc) {
	setJb(getaddy2((OMGYAY + 0x835)));
	lua_pcall(state, nargs, nresults, errfunc);
	restoreJb(getaddy2((OMGYAY + 0x835)));
}

void setfield(int state, int index, const char *k) {
	setJb(getaddy2((OMGYAY + 0x14d4)));
	lua_setfield(state, index, k);
	restoreJb(getaddy2((OMGYAY + 0x14d4)));
}

void pushnumber(int state, double number) {
	setJb(getaddy2((OMGYAY + 0xcd7)));
	lua_pushnumber(state, number);
	restoreJb(getaddy2((OMGYAY + 0xcd7)));
}

void getglobal(int lua_State, const char *k) {
	return getfield(lua_State, -10002, k);
}


int GetCharacter(std::string noobchar) {
	if (noobchar == "me") {
		int LocalNoob = GetLocalPlayer(Players);
		std::string* asdnoob = GetName(LocalNoob);
		int Character = FindFirstChild_PartOf(Workspace, *asdnoob);
		if (Character > 0) {
			return Character;
		}
		if (Character == 0) {
			return{};
		}
	}
	else if (noobchar != "me" && noobchar != "all" && noobchar != "others") {
		int Character = FindFirstChild_PartOf(Workspace, noobchar);
		if (Character > 0) {
			return Character;
		}
		if (Character == 0) {
			return{};
		}
	}
}


int GetPlayer(std::string noobchar) {
	if (noobchar == "me") {
		int Player = GetLocalPlayer(Players);
		return Player;
	}
	else if (noobchar != "me" && noobchar != "all" && noobchar != "others") {
		int Player = FindFirstChild_PartOf(Players, noobchar);
		if (Player != 0) {
			return Player;
		}
		if (Player == 0) {
			return{};
		}
	}
}

int FE(int Workspace) {
	return *(BYTE*)(Workspace + 595);
}


int main() {
	ConsoleHacks();
	Console("noobxploit v0.2.1 by supersupersnivy97 @V3rmillion");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	cout << "u noob\n";
	cout << "scanning plz wait noob\n";
	Scan();
	Scan1();
	int fuck = GetLocalPlayer(Players);
	string hahanoob = *GetName(fuck);
	vector<string> above = { "balkaranbrar1234", "VisualRape", "CaptainPremiumDoge", "DarkFrozure", "1234rr3", "0199284", "MrSubmot", "lemon4488", "stalkingmark", "dogtraining", "Synxcious", "choco123", "HttpPotatos", "noobxlpoit", "Hairy_Nippless", "iiMG", "DENIZR2D", "igotballsofsteel123", "GodOfBerserk", "monkeyoyo6", "razer225", "razersharp225", "GODman5432", "NarrowRebecca", "1hotshot1", "RatteKoppie", "ZackaryH02", "JustCallMeMerc", "CocaColaHax", "Nessifa", "Scorp1x", "Sukez", "HisHighness", "red_abysm", "invisibletjm" };
	bool ok = false;
	printf("authorizing...\n");
	for (int i = 0; i < above.size(); i++) {
		if (hahanoob == above[i]) {
			ok = true;
		}
	}
	if (ok) {
		printf("authorized.\n");
		int ScriptContext = FindFirstClass(DataModel, "ScriptContext");
		bool FilteringEnabled = FE(Workspace);
		SetConsoleTextAttribute(hConsole, 10);
		printf("done u noob\n");
		Sleep(4000);
		lua_State = *(DWORD*)(ScriptContext + 0x70);
		system("cls");
		SetConsoleTextAttribute(hConsole, 14);
		if (DataModel > 0) {
			printf("datamodel is ok\n");
		}
		if (Workspace > 0) {
			printf("workspace is ok\n");
		}
		if (Lighting > 0) {
			printf("lighting is ok\n");
		}
		if (Players > 0) {
			printf("players is ok\n");
		}
		if (lua_State > 0) {
			printf("lua_state is ok\n");
		}
		if (FilteringEnabled) {
			printf("Filtering is enabled\n");
		}
		else {
			printf("Filtering is disabled\n");
		}
		SetConsoleTextAttribute(hConsole, 15);
		SetConsoleTextAttribute(hConsole, 9);
		getglobal(lua_State, "printidentity");
		pcall(lua_State, 0, 0, 0);
		settop(lua_State, 0);
		getglobal(lua_State, "warn");
		string welcome = "Hello, " + *GetName(GetLocalPlayer(Players)) + "!\nWelcome to noobxploit 0.2.1 made by supersupersnivy97\nCredits to Quad Core, Variaxt, HexaDecimal, Variable, Louka\nSay cmds for a list of commands";
		pushstring(lua_State, welcome.c_str());
		pcall(lua_State, 1, 0, 0);
		settop(lua_State, 0);
		SetConsoleTextAttribute(hConsole, 0xA);
		cout << "Say cmds for a list of commands.\n";
		cout << "(no support for 'others' and 'all' but for 'me' and short names)\n";
		cout << "(if a name starts with a cap letter, make sure you cap it)\n";
		do {
			std::cout << ">";
			std::vector<std::string> In = split(Input());
			if (In.at(0) == "cmds") {
				if (In.size() == 1) {
					cout << "ff [p] - Gives [p] a ForceField\n";
					cout << "unff [p] - Removes [p]'s ForceField\n";
					cout << "fire [p] - Gives [p] Fire\n";
					cout << "unfire [p] - Removes [p]'s Fire\n";
					cout << "sparkles [p] - Gives [p] Sparkles\n";
					cout << "unsparkles [p] - Removes [p]'s Sparkles\n";
					cout << "smoke [p] - Gives [p] Smoke\n";
					cout << "unsmoke [p] - Removes [p]'s Smoke\n";
					cout << "particles [p] [id] - Gives [p] particles with the id [id]\n";
					cout << "unparticles [p] - Removes [p]'s particles\n";
					cout << "ws [p] [n] - Sets [p]'s WalkSpeed to [n]\n";
					cout << "god [p] - Sets [p]'s MaxHealth to HUGE_VAL\n";
					cout << "ungod [p] - Sets [p]'s MaxHealth to 100\n";
					cout << "jp [p] [n] - Sets [p]'s JumpPower to [n]\n";
					cout << "time [n] - Sets the time to [n]\n";
					cout << "btools [p] - Gives [p] building tools\n";
					cout << "makepart - Makes a Part in the Workspace\n";
					cout << "fogend [n] - Sets FogEnd to [n]\n";
					cout << "kill [p] - Kills [p]\n";
					cout << "statchange1 [p] [stat] [new int value] - Sets [p]'s [stat] to [new int value]\n";
					cout << "statchange2 [p] [stat] [new string value] - Sets [p]'s [stat] to [new string value]\n";
					cout << "charapp [p] [id] - Sets [p]'s CharacterAppearance to ..[id]\n";
					cout << "getglobal [name] - Use only if you know what it is\n";
					cout << "getfield [index] [name] - Use only if you know what it is\n";
					cout << "setfield [index] [name] - Use only if you know what it is\n";
					cout << "pushvalue [index] - Use only if you know what it is\n";
					cout << "pushstring [string] - Use only if you know what it is\n";
					cout << "pushnumber [number] - Use only if you know what it is\n";
					cout << "pcall [nargs] [nresults] - Use only if you know what it is\n";
					cout << "settop [index] - Use only if you know what it is\n";
					cout << "gettop - Use only if you know what it is\n";
					cout << "invisible [p] - Makes [p] invisible\n";
					cout << "visible [p] - Makes [p] visible\n";
					cout << "gravity [n] - Sets the gravity to [n] (default is 196.2)\n";
					cout << "print [string] - Prints [string] in the dev console\n";
					cout << "tp [p1] [p2] - Teleports [p1] to [p2]\n";
					cout << "music [id] - Plays the music with the id [id]\n";
					cout << "explode [p] - Blows up [p]\n";
					cout << "punish [p] - Punishes [p]\n";
					cout << "unpunish [p] - Unpunishes [p]\n";
					cout << "sky [id] - Makes a skybox with the id [id]\n";
					cout << "rsky - Removes noobxploit's skybox\n";
				}
			}
			if (In.at(0) == "rsky") {
				if (In.size() == 1) {
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Lighting");
					getfield(lua_State, -1, "noobxploit_sky");
					getfield(lua_State, -1, "remove");
					pushvalue(lua_State, -2);
					pcall(lua_State, 1, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "sky") {
				if (In.size() == 2) {
					string skybox = "rbxassetid://" + In.at(1);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Lighting");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "Sky");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 1, 0);
					pushstring(lua_State, "noobxploit_sky");
					setfield(lua_State, -2, "Name");
					pushstring(lua_State, skybox.c_str());
					setfield(lua_State, -2, "SkyboxBk");
					pushstring(lua_State, skybox.c_str());
					setfield(lua_State, -2, "SkyboxDn");
					pushstring(lua_State, skybox.c_str());
					setfield(lua_State, -2, "SkyboxFt");
					pushstring(lua_State, skybox.c_str());
					setfield(lua_State, -2, "SkyboxLf");
					pushstring(lua_State, skybox.c_str());
					setfield(lua_State, -2, "SkyboxRt");
					pushstring(lua_State, skybox.c_str());
					setfield(lua_State, -2, "SkyboxUp");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "setfield") {
				setfield(lua_State, atoi(In.at(1).c_str()), In.at(2).c_str());
			}
			if (In.at(0) == "punish") {
				if (In.size() == 2) {
					string noob1 = *GetName(GetPlayer(In.at(1)));
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, noob1.c_str());
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Lighting");
					setfield(lua_State, -3, "Parent");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "unpunish") {
				if (In.size() == 2) {
					string noob1 = *GetName(GetPlayer(In.at(1)));
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Lighting");
					getfield(lua_State, -1, noob1.c_str());
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					setfield(lua_State, -3, "Parent");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "music") {
				if (In.size() == 2) {
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "Sound");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 1, 0);
					pushstring(lua_State, ("rbxassetid://" + In.at(1)).c_str());
					setfield(lua_State, -2, "SoundId");
					pushstring(lua_State, "noobxploit_music");
					setfield(lua_State, -2, "Name");
					pushnumber(lua_State, 1);
					setfield(lua_State, -2, "Volume");
					getfield(lua_State, -1, "Play");
					pushvalue(lua_State, -2);
					pcall(lua_State, 1, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "explode") {
				if (In.size() == 2) {
					string fuckingnoob = *GetName(GetPlayer(In.at(1)));
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "Explosion");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 1, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, fuckingnoob.c_str());
					getfield(lua_State, -1, "Torso");
					getfield(lua_State, -1, "Position");
					setfield(lua_State, -6, "Position");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "tp") {
				if (In.size() == 3) {
					string noob1 = *GetName(GetPlayer(In.at(1)));
					string noob2 = *GetName(GetPlayer(In.at(2)));
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, noob1.c_str());
					getfield(lua_State, -1, "HumanoidRootPart");
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, noob2.c_str());
					getfield(lua_State, -1, "Torso");
					getfield(lua_State, -1, "CFrame");
					setfield(lua_State, -6, "CFrame");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "print") {
				string string;
				for (int i = 1; i < In.size(); i++) {
					if (i < (In.size() - 1)) {
						string.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 1)) {
						string.append(In.at(i));
					}
				}
				getglobal(lua_State, "print");
				pushstring(lua_State, string.c_str());
				pcall(lua_State, 1, 0, 0);
				settop(lua_State, 0);
			}
			if (In.at(0) == "gravity") {
				if (In.size() == 2) {
					int newgravity = atoi(In.at(1).c_str());
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					pushnumber(lua_State, newgravity);
					setfield(lua_State, -2, "Gravity");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "gettop") {
				if (In.size() == 1) {
					cout << "Elements on the stack: " << lua_gettop(lua_State) << endl;
				}
			}
			if (In.at(0) == "settop") {
				if (In.size() == 2) {
					settop(lua_State, atoi(In.at(1).c_str()));
				}
			}
			if (In.at(0) == "pcall") {
				if (In.size() == 3) {
					pcall(lua_State, atoi(In.at(1).c_str()), atoi(In.at(2).c_str()), 0);
				}
			}
			if (In.at(0) == "pushnumber") {
				if (In.size() == 2) {
					pushnumber(lua_State, atoi(In.at(1).c_str()));
				}
			}
			if (In.at(0) == "pushstring") {
				if (In.size() == 2) {
					pushstring(lua_State, In.at(1).c_str());
				}
			}
			if (In.at(0) == "pushvalue") {
				if (In.size() == 2) {
					pushvalue(lua_State, atoi(In.at(1).c_str()));
				}
			}
			if (In.at(0) == "getfield") {
				if (In.size() == 3) {
					getfield(lua_State, atoi(In.at(1).c_str()), In.at(2).c_str());
				}
			}
			if (In.at(0) == "getglobal") {
				if (In.size() == 2) {
					getglobal(lua_State, In.at(1).c_str());
				}
			}
			if (In.at(0) == "invisible") {
				if (In.size() == 2) {
					int tepig_is_a_fucking_noob = GetCharacter(In.at(1));
					string tepigs_name = *GetName(tepig_is_a_fucking_noob);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Head");
					pushnumber(lua_State, 1);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Torso");
					pushnumber(lua_State, 1);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Left Arm");
					pushnumber(lua_State, 1);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Left Leg");
					pushnumber(lua_State, 1);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Right Arm");
					pushnumber(lua_State, 1);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Right Leg");
					pushnumber(lua_State, 1);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Head");
					getfield(lua_State, -1, "face");
					pushnumber(lua_State, 1);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "visible") {
				if (In.size() == 2) {
					int tepig_is_a_fucking_noob = GetCharacter(In.at(1));
					string tepigs_name = *GetName(tepig_is_a_fucking_noob);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Head");
					pushnumber(lua_State, 0);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Torso");
					pushnumber(lua_State, 0);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Left Arm");
					pushnumber(lua_State, 0);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Left Leg");
					pushnumber(lua_State, 0);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Right Arm");
					pushnumber(lua_State, 0);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Right Leg");
					pushnumber(lua_State, 0);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, tepigs_name.c_str());
					getfield(lua_State, -1, "Head");
					getfield(lua_State, -1, "face");
					pushnumber(lua_State, 0);
					setfield(lua_State, -2, "Transparency");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "charapp") {
				if (In.size() == 3) {
					int noobhere = GetPlayer(In.at(1));
					string noobnamehere = *GetName(noobhere);
					string goodurl = "http://www.roblox.com/Asset/CharacterFetch.ashx?userId=";
					goodurl.append(In.at(2));
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Players");
					getfield(lua_State, -1, noobnamehere.c_str());
					pushstring(lua_State, goodurl.c_str());
					setfield(lua_State, -2, "CharacterAppearance");
					settop(lua_State, 0);
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Humanoid");
					pushnumber(lua_State, 0);
					setfield(lua_State, -2, "Health");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "statchange1") {
				if (In.size() == 4) {
					int Player = GetPlayer(In.at(1));
					string playername = *GetName(Player);
					int leaderstats = FindFirstChild_PartOf(Player, "leaderstats");
					int stat = FindFirstChild_PartOf(leaderstats, In.at(2));
					string statname = *GetName(stat);
					int newvalue = atoi(In.at(3).c_str());
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Players");
					getfield(lua_State, -1, playername.c_str());
					getfield(lua_State, -1, "leaderstats");
					getfield(lua_State, -1, statname.c_str());
					pushnumber(lua_State, newvalue);
					setfield(lua_State, -2, "Value");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "statchange2") {
				int Player = GetPlayer(In.at(1));
				string playername = *GetName(Player);
				int leaderstats = FindFirstChild_PartOf(Player, "leaderstats");
				int stat = FindFirstChild_PartOf(leaderstats, In.at(2));
				string statname = *GetName(stat);
				string newvalue;
				for (int i = 3; i < In.size(); i++) {
					if (i < (In.size() - 1)) {
						newvalue.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 1)) {
						newvalue.append(In.at(i));
					}
				}
				getglobal(lua_State, "game");
				getfield(lua_State, -1, "Players");
				getfield(lua_State, -1, playername.c_str());
				getfield(lua_State, -1, "leaderstats");
				getfield(lua_State, -1, statname.c_str());
				pushstring(lua_State, newvalue.c_str());
				setfield(lua_State, -2, "Value");
				pcall(lua_State, 1, 0, 0);
				settop(lua_State, 0);
			}
			if (In.at(0) == "ff") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "ForceField");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "fogend") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					float hax = atoi(In.at(1).c_str());
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Lighting");
					pushnumber(lua_State, hax);
					setfield(lua_State, -2, "FogEnd");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "unff") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "ForceField");
					getfield(lua_State, -1, "remove");
					pushvalue(lua_State, -2);
					pcall(lua_State, 1, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "fire") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Torso");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "Fire");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "unfire") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Torso");
					getfield(lua_State, -1, "Fire");
					getfield(lua_State, -1, "remove");
					pushvalue(lua_State, -2);
					pcall(lua_State, 1, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "sparkles") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Torso");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "Sparkles");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "unsparkles") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Torso");
					getfield(lua_State, -1, "Sparkles");
					getfield(lua_State, -1, "remove");
					pushvalue(lua_State, -2);
					pcall(lua_State, 1, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "smoke") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Torso");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "Smoke");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "unsmoke") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Torso");
					getfield(lua_State, -1, "Smoke");
					getfield(lua_State, -1, "remove");
					pushvalue(lua_State, -2);
					pcall(lua_State, 1, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "particles") {
				if (In.size() == 3) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					string texture = "rbxassetid://" + In.at(2);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Torso");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "ParticleEmitter");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 1, 0);
					pushstring(lua_State, texture.c_str());
					setfield(lua_State, -2, "Texture");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "unparticles") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Torso");
					getfield(lua_State, -1, "ParticleEmitter");
					getfield(lua_State, -1, "remove");
					pushvalue(lua_State, -2);
					pcall(lua_State, 1, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "ws") {
				if (In.size() == 3) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					float hax = atoi(In.at(2).c_str());
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Humanoid");
					pushnumber(lua_State, hax);
					setfield(lua_State, -2, "WalkSpeed");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "kill") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Humanoid");
					pushnumber(lua_State, 0);
					setfield(lua_State, -2, "Health");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "god") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Humanoid");
					pushnumber(lua_State, HUGE_VAL);
					setfield(lua_State, -2, "MaxHealth");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "ungod") {
				if (In.size() == 2) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Humanoid");
					pushnumber(lua_State, 100);
					setfield(lua_State, -2, "MaxHealth");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "jp") {
				if (In.size() == 3) {
					int Character = GetCharacter(In.at(1));
					string charname = *GetName(Character);
					float hax = atoi(In.at(2).c_str());
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getfield(lua_State, -1, charname.c_str());
					getfield(lua_State, -1, "Humanoid");
					pushnumber(lua_State, hax);
					setfield(lua_State, -2, "JumpPower");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "time") {
				if (In.size() == 2) {
					string newTimeOfDay = In.at(1);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Lighting");
					pushstring(lua_State, newTimeOfDay.c_str());
					setfield(lua_State, -2, "TimeOfDay");
					pcall(lua_State, 1, 0, 0);
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "btools") {
				if (In.size() == 2) {
					int noob = GetPlayer(In.at(1));
					string noobname = *GetName(noob);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Players");
					getfield(lua_State, -1, noobname.c_str());
					getfield(lua_State, -1, "Backpack");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "HopperBin");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 1, 0);
					pushnumber(lua_State, 2);
					setfield(lua_State, -2, "BinType");
					settop(lua_State, 0);
					Sleep(100);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Players");
					getfield(lua_State, -1, noobname.c_str());
					getfield(lua_State, -1, "Backpack");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "HopperBin");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 1, 0);
					pushnumber(lua_State, 3);
					setfield(lua_State, -2, "BinType");
					settop(lua_State, 0);
					Sleep(100);
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Players");
					getfield(lua_State, -1, noobname.c_str());
					getfield(lua_State, -1, "Backpack");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "HopperBin");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 1, 0);
					pushnumber(lua_State, 4);
					setfield(lua_State, -2, "BinType");
					settop(lua_State, 0);
				}
			}
			if (In.at(0) == "makepart") {
				if (In.size() == 1) {
					getglobal(lua_State, "game");
					getfield(lua_State, -1, "Workspace");
					getglobal(lua_State, "Instance");
					getfield(lua_State, -1, "new");
					pushstring(lua_State, "Part");
					pushvalue(lua_State, -4);
					pcall(lua_State, 2, 0, 0);
					settop(lua_State, 0);
				}
			}
		} while (true);
	}
	else {
		printf("failed\n");
		system("pause");
		exit(0);
	}
}

int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
	}
	return TRUE;
}