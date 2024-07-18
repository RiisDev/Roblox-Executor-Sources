#define _CRT_SECURE_NO_WARNINGS //msvc doesn't like freopen
#include <iostream>
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

#define _CRT_SECURE_NO_WARNINGS //msvc doesn't like freopen

const char *Commands =
"===PLAYER COMMANDS====\n"
"#kill [p] - Kills [p]\r\n"
"#god [p] - Gods [p]\r\n"
"#ungod [p] - Disables god from the [p]\r\n"
"#ws [p] [n] - Set's [p]'s walkspeed to [n]\r\n"
"#punish [p] - Punishes the [p]\r\n"
"#unpunish [p] - Disable's the punish from the [p]\r\n"
"#jumppower [p] [n] - Set's [p]'s jumppower to [n]\r\n"
"#loopkill [p] - Loopkills the [p]\r\n"
"#kick [p] - Kicks [p]\r\n"
"#ban [p] - Bans [p]\r\n"
"#btools [p] - Give's [p] pbstools.\r\n"
"#freeze [p] - Freezez the [p]\r\n"
"#thaw [p] - Thaw's the [p]\r\n"
"#sethealth [p] [n] - Set's [p]'s health to [n]\r\n"
"#stealtools [p] - Steals the tools from [p]\r\n"
"#sanic [p] - Plays a sanic music while you're running fast [p]\r\n"
"#swimgod [p] - Enables swimgod to the [p]\r\n"
"#unswimgod [p] - Disables swimgod from the [p]\r\n"
"\n"
"====SERVER COMMANDS====\n"
"#fogstart [n] - Set's the fogstart to [n]\r\n"
"#fogend [n] - Set's the fog to [n]\r\n"
"#fogcolor [r] [g] [b] - Set's the fogcolor to [n] [n] [n]\r\n"
"#shutdown - Shutsdown the server\r\n"
"#clearallchildren - Clears all the children from workspace\r\n"
"#killall - Kills all the players\r\n"
"#loopkillall - Loopkills all players\r\n"
"#music [n] - Set's music to [n]\r\n"
"#playjohncena - Plays john cena\r\n"
"#time [n] - Set's time to [n]\r\n"
"#rteams - Remove's all teams\r\n"
"#rteam - Removes a team at a time\r\n"
"#rainbow on / off - Turns rainbow on / off\r\n"
"#seizure on / off - Turns seizure on / off\r\n"
"\n"
"====MISC COMMANDS====\n"
"#faceless [p] - Remove's [p]'s face\r\n"
"#unfaceless [p] - Disables the removal from the [p]'s face\r\n"
"#topless [p] - Make's [p] topless\r\n"
"#untopless [p] - Make's [p] untopless from the removal.\r\n"
"#naked [p] - Make's [p] naked\r\n"
"#fixchar [p] - Fixes the [p]'s char from the removal of naked [p].\r\n"
"#dpants [p] - Remove's [p]'s pants and makes it lowless.\r\n"
"\n"
"====STATE COMMANDS====\n"
"#noclip - Toggles noclip\r\n"
"#airwalk - Toggles airwalk\r\n"
"#swim - Toggles swim\r\n"
"#jesusfly - Toggles jesusfly\r\n"
"#superjump - Toggles superjump\r\n"
"#ragdoll - Toggles ragdoll\r\n"
"#disable - Disables all state commands\r\n"
"\n"
"\n"
;

void Console(char* title) {
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

std::string Input() {
	std::string wotthefuck;
	getline(std::cin, wotthefuck);
	return wotthefuck;
}

std::string IntToHex(int Val) {
	std::stringstream stream;
	stream << std::hex << Val;
	return stream.str();
}

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
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





namespace Roblox {
	int DataModel;
	int Workspace;
	int Lighting;
	int Players;
	int Camera;
	int Head;
	int LogService;
	int CurrentPlayer;
	int State;
	int Teams;
	bool Changing;
	int antiscript = 0;
	bool Seizure = false;
	bool Night = false;
	bool FogStart = false;
	bool FogEnd = false;
	std::string* PlayerName;

	int WalkSpeed_Address = 0x6D0910;
	typedef void(__thiscall * setwalkspeed)(int self, float speed);
	setwalkspeed SetWalkSpeed = (setwalkspeed)WalkSpeed_Address;

	int MaxHealth_Address = 0x6CFD90;
	typedef void(__thiscall * setmaxhealth)(int self, float health);
	setmaxhealth SetMaxHealth = (setmaxhealth)MaxHealth_Address;

	typedef void(__thiscall *setparent)(int self, int parent);
	setparent setParent = (setparent)SetParent;

	int SetMinutesAfterMidnight_Address = 0x6DC0A0;
	typedef void(__thiscall * setminutesaftermidnight)(void* lighting, double aidss);
	setminutesaftermidnight SetMinutesAfterMidnight = (setminutesaftermidnight)SetMinutesAfterMidnight_Address;

	int SetFogEnd_Address = 0x6DBF80;
	typedef void(__thiscall * setfoge)(void* lighting, float aidss);
	setfoge SetFogEnd = (setfoge)SetFogEnd_Address;

	int SetFogStart_Address = 0x6DBFB0;
	typedef void(__thiscall * setfogs)(void* lighting, float aidss);
	setfogs SetFogStart = (setfogs)SetFogStart_Address;

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


	void DoLoop() {
		//VMProtectBeginUltra("SEXYdsf");
		do {


			if (Seizure == true && Lighting != 0) {

				if (Night == false) {
					SetMinutesAfterMidnight((void*)Lighting, 24 * 60.0);
					Night = true;
				}
				else {
					SetMinutesAfterMidnight((void*)Lighting, 8 * 60.0);
					Night = false;
				}
			}
		} while (true);
		//	VMProtectEnd();
	}

	void FogLoop() {
		do {
			if (FogStart == true && Lighting != 0) {
				if (FogEnd == false) {
					float Fog = 0;
					Roblox::SetFogEnd((void*)Lighting, Fog);
					FogEnd = true;
				}
			}
			else {
				float Fog = 15;
				SetFogStart((void*)Lighting, Fog);
				FogEnd = false;
			}
		} while (true);
	}



	int FindFirstChild_PartOf(int Instance, std::string Name) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (strstr(GetName(*(int*)i)->c_str(), Name.c_str()))
			{
				return *(int*)i;
			}
		}
		return 0;
	}


	int GetLocalPlayer(int Plr) {
		return *(int*)(Plr + 0x16C);
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

	int GetCharacter(std::string Name) {


		int Character = FindFirstChild_PartOf(Workspace, Name);
		if (Character != 0) {
			return Character;
		}
		else
		{
			return 0;
		}
	}

	int GetPlayer(std::string Name) {
		int Player = FindFirstChild_PartOf(Players, Name);
		if (Player != 0) {
			return Player;
		}
		else
		{
			return 0;
		}
	}

	std::vector<int> GetChildren(int Instance) {

		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		std::vector<int> Children;
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			Children.push_back(*(int*)i);
		}
		return Children;

	}
}

int State;
bool Changing = false;


void SetHumanoidState(int self, int state) {
	int sPtr = *(int*)(self + 0x2DC);
	if (sPtr) {
		*(BYTE*)(self + state + 0x229) = 1;
		*(int*)(sPtr + 0x20) = state;
	}
}

void doState() {
	if (Changing == true) {
		int LocalCharacter = Roblox::GetCharacter(*Roblox::PlayerName);
		if (LocalCharacter != 0) {
			int Humanoid = Roblox::FindFirstClass(LocalCharacter, "Humanoid");
			if (Humanoid != 0) {
				SetHumanoidState(Humanoid, State);
			}
		}
	}
}

char* mfm;

void Loop() {
	do {
		__try {
			doState();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			Sleep(1); // do absolutely fucking nothing
		}
	} while (true);

}

int ScriptContextVftable;
int ScriptContext = 0;

bool PasswordWhitelist() {
	cout << "Initaiting... OK! " << endl;
	cout << "Authorizing in progress... " << endl;
	std::string password;
	std::cin >> password;
	if (password == "creator") {
		int NUU = Roblox::CurrentPlayer;
		std::string* CurrentName = Roblox::GetName(NUU);
		cout << "Welcome, " << "" << CurrentName << endl;
	}
	else {
		exit(0);
	}
	return 1;
}

bool Nexion_Start() {
	AllocConsole();//create console
	freopen("CONOUT$", "w", stdout); //to use output (cout)
	freopen("CONIN$", "r", stdin); //to use input, useless because I never take input
	Console("Nexion");
		cout << "Scanning for the RBXDATA...\n";
		ScriptContextVftable = 0xF873F4;
		ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
		Roblox::DataModel = Roblox::GetParent(ScriptContext);
		Roblox::Players = Roblox::FindFirstClass(Roblox::DataModel, "Players");
		Roblox::Lighting = Roblox::FindFirstClass(Roblox::DataModel, "Lighting");
		Roblox::Workspace = Roblox::FindFirstClass(Roblox::DataModel, "Workspace");
		Roblox::Teams = Roblox::FindFirstClass(Roblox::DataModel, "Teams");
		Roblox::CurrentPlayer = Roblox::GetLocalPlayer(Roblox::Players);
		Roblox::PlayerName = Roblox::GetName(Roblox::CurrentPlayer);
		cout << "Loading for loops... " << endl;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Loop, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Roblox::FogLoop, NULL, NULL, NULL);
		cout << "done " << endl;
		cout << "ScriptContext: OK\n";
		cout << "DataModel: OK!\n";
		cout << "Workspace: OK!\n";
		cout << "Players: OK!\n";
		cout << "Lighting: OK\n";
		cout << "done\n";
		while (1) {
			std::cout << ">";
			std::vector<std::string> Arguments = split(Input());

			if (Arguments.at(0) == "noclip") {
				State = 11;
				Changing = true;
			}

			else if (Arguments.at(0) == "swim") {
				State = 4;
				Changing = true;
			}

			else if (Arguments.at(0) == "superjump") {
				State = 13;
				Changing = true;
			}

			else if (Arguments.at(0) == "airwalk") {
				State = 10;
				Changing = true;
			}
			else if (Arguments.at(0) == "jesusfly") {
				State = 12;
				Changing = true;
			}

			else if (Arguments.at(0) == "ragdoll") {
				State = 1;
				Changing = true;
			}
			else if (Arguments.at(0) == "slow'd") {
				State = -1;
				Changing = true;
			}
			else if (Arguments.at(0) == "phyH") {
				State = 16;
				Changing = true;

			}
			else if (Arguments.at(0) == "disable") {
				Changing = false;
			}
			else if (Arguments.at(0) == "cmds") {
				cout << Commands;
			}
			else if (Arguments.at(0) == "ws") {
				if (Arguments.size() == 3) {
					float NewSpeed = std::stof(Arguments.at(2).c_str());
					int Player = Roblox::GetCharacter(Arguments.at(1));
					if (Player != 0) {
						int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
						if (Humanoid != 0) {
							Roblox::SetWalkSpeed(Humanoid, NewSpeed);
						}
					}
				}
			}
			else if (Arguments.at(0) == "freeze") {
				int Player = Roblox::GetCharacter(Arguments.at(1));
				if (Player != 0) {
					int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
					if (Humanoid != 0) {
						Roblox::SetWalkSpeed(Humanoid, 0);
					}
				}
			}
			else if (Arguments.at(0) == "thaw") {
				int Player = Roblox::GetCharacter(Arguments.at(1));
				if (Player != 0) {
					int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
					if (Humanoid != 0) {
						Roblox::SetWalkSpeed(Humanoid, 15);
					}
				}
			}
			else if (Arguments.at(0) == "god") {
				if (Arguments.size() == 2) {
					int Player = Roblox::GetCharacter(Arguments.at(1));
					if (Player != 0) {
						int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
						if (Humanoid != 0) {
							Roblox::SetMaxHealth(Humanoid, 99999999999);
							//State = 4;
							//Changing = true;
							//Sleep(300);
							//Changing = false;
						}
					}
					else
					{
						std::cout << "Failed to god " << std::endl;
					}
				}
			}
			else if (Arguments.at(0) == "sethealth") {
				float NewSpeed = std::stof(Arguments.at(2).c_str());
				int Player = Roblox::GetCharacter(Arguments.at(1));
				if (Player != 0) {
					int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
					if (Humanoid != 0) {
						Roblox::SetMaxHealth(Humanoid, NewSpeed);
					}
				}
			}
			else if (Arguments.at(0) == "kill") {
				int Player = Roblox::GetCharacter(Arguments.at(1));
				if (Player != 0) {
					int Humanoid = Roblox::FindFirstClass(Player, "Humanoid");
					if (Humanoid != 0) {
						Roblox::SetMaxHealth(Humanoid, 0);
					}
				}
			}
			else if (Arguments.at(0) == "dpants") {
				if (Arguments.size() == 2) {
					DWORD Characte3r = Roblox::GetCharacter(Arguments.at(1));
					if (Characte3r != 0) {
						DWORD Shir3t = Roblox::FindFirstClass(Characte3r, "Pants");
						if (Shir3t != 0) {
							Roblox::setParent(Shir3t, Roblox::Workspace);
						}
					}
				}
			}
			else if (Arguments.at(0) == "topless")
			{
				if (Arguments.size() == 2) {
					DWORD Characterr = Roblox::GetCharacter(Arguments.at(1));
					if (Characterr != 0) {
						DWORD Shirtt = Roblox::FindFirstClass(Characterr, "Shirt");
						if (Shirtt != 0) {
							Roblox::setParent(Shirtt, Roblox::Workspace);
						}
					}
				}
				else if (Arguments.at(0) == "fogloop") {
					Roblox::FogLoop();
				}
			}
		}
	//}
 return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Nexion_Start, 0, 0, 0);
	}
	return 1;
}