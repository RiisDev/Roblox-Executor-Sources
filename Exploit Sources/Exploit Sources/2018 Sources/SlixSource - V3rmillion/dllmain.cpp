#pragma comment(lib,"ws2_32.lib")
#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <limits>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>
#include <fcntl.h>
#include <io.h>
#include <TlHelp32.h>
#include <fstream>
#include <string>

#include "Memory.h"
#include "Slix.h"
#include "whitelist.h"
#define UNICODE


void scanner() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	Slix::DataModel = Memory::Scan(PAGE_READWRITE, "\x7C\x9A\xF7\x00", "xxxx");
	Slix::HopperBin = Memory::Scan(PAGE_READWRITE, "\xDC\x8C\xF9\x00", "xxxx");
	Slix::Instance = Memory::Scan(PAGE_READWRITE, "\x8C\x2B\xF7\x00", "xxxx");
	Slix::Lighting = Slix::FindFirstClass(Slix::DataModel, "Lighting");
	Slix::Workspace = Slix::FindFirstClass(Slix::DataModel, "Workspace");
	Slix::Players = Slix::FindFirstClass(Slix::DataModel, "Players");

	int LocalPlayer = Slix::GetLocalPlayer(Slix::Players);

	std::string *playername = Slix::GetName(Slix::GetLocalPlayer(Slix::Players));
	std::cout << "Hello, " << *playername << std::endl;
	std::cout << "Made by SplitTrix and Nicholas_sanchez5575.\n";
	MessageBoxA(NULL, "Welcome to SlixV0.1", "SlixV0.1", MB_OK);


}


typedef void(__cdecl *retcheckf)();
retcheckf retcheck = (retcheckf)0x53F0E0;

void rektretcheck() {
	return;
}


void doState() {
	std::string *playername = Slix::GetName(Slix::GetLocalPlayer(Slix::Players));
	int LocalCharacter = Slix::GetCharacter(*playername);
	if (LocalCharacter != 0) {
		int Humanoid = Slix::FindFirstClass(LocalCharacter, "Humanoid");
		if (Humanoid != 0) {
			Slix::SetHumanoidState(Humanoid, Slix::State);
		}
	}
}


void Loop() {
	do {
		if (GetAsyncKeyState(VK_RSHIFT)) {
			Slix::State = 18;
			Sleep(100);
		}
		doState();
	} while (Slix::State != 18);

}


std::string Input()
{
	std::string sInput;
	getline(std::cin, sInput);
	return sInput;
}


std::vector<std::string> split(std::string s)
{
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

void failed() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
	std::cout << "Failed!" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
}

void success() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	std::cout << "Successful!" << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
}


void Commands(std::string cmd) {
	const char* Command_List[]{
		"\r\n -===SERVER COMMANDS===- \r\n",
		"fog[n] - Sets the fogend to [n].\r\n",
		"fogc[n][n][n] - Sets the fogcolor to [n][n][n]\r\n",
		"time[n] - Sets the time to [n]\\r\n",
		"creeps - FogEnd to 120, time 0.\r\n",
		"\r\n -===HUMANOID COMMANDS===- \r\n",
		"speed[p][n] - Sets [p]'s WalkSpeed to [n]\r\n",
		"jumppow[p][n] - Sets [p]'s jumppower to [n].\r\n",
		"sh[p][n] - Sets [p]'s health to [n].\r\n",
		"dmg[p][n] - Damages [p] by [n]\r\n",
		"\r\n -===MISCELLANEOUS COMMANDS===- \r\n",
		"punish[p] - Moves the character to lighting.\r\n",
		"unpunish[p] - Reverses punish.\r\n",
		"hidename[p] - Hides [p]'s name\r\n",
		"showname[p] - Shows [p]'s name\r\n",
		"god[p] - Sets [p]'s maxhealth to a huge value.\r\n",
		"ungod[p] - Sets [p]'s maxhealth to normal(100).\r\n",
		"killall - Kills everyone(may crash).\r\n",
		"kill[p] - Kills that player.\r\n",
		"naked[p] - Removes Shirt and Pants from [p].\r\n",
		"freeze[p] - Freezes [p]\r\n",
		"unfreeze[p] - Unfreezes [p]\r\n",
		"\r\n -===LOCAL COMMANDS===-\r\n",
		"noclip - Allows you to walk through walls.\r\n",
		"ragdoll - Ragdoll\r\n",
		"swim - Makes you look like you swimming in air.\r\n"
		"fly - Hold space. Makes you fly!(unusual)\r\n",
		"unlimitedjump - Hold space to go as high as you want :). (Cant move)\r\n",


	};

	std::vector<std::string> In = split(cmd);

	if (In.size() == 0) { return; }

	else if (In.at(0) == "cmds")
	{
		for (int i = 0; i < _countof(Command_List); i++)
		{
			std::cout << Command_List[i];
		}
		std::cout << std::endl;
	}
	else if (In.at(0) == "noclip") {
		if (In.size() == 1) {
			success();
			Slix::State = 11;
			Loop();
		}
		else {
			failed();
		}
	}
	else if (In.at(0) == "speed") {
		if (In.size() == 3) {
			float speed = std::stof(In.at(2).c_str());
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::SetWalkSpeed(Human, speed);
					success();
				}
			}
			else {
				failed();
			}
		}
	}
	else if (In.at(0) == "punish") {
		if (In.size() == 2) {
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				Slix::SetParent(Char, Slix::Lighting);
				success();
			}
			else {
				failed();
			}
		}
	}
	else if (In.at(0) == "unpunish") {
		if (In.size() == 2) {
			int Char = Slix::FindFirstChild(Slix::Lighting, In.at(1).c_str());
			if (Char != 0) {
				Slix::SetParent(Char, Slix::Workspace);
				success();
			}
			else {
				failed();
			}
		}
	}
	else if (In.at(0) == "jumppow") {
		if (In.size() == 3) {
			float pow = std::stof(In.at(2).c_str());
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::SetJumpPower(Human, pow);
					success();
				}
			}
			else {
				failed();
			}

		}
	}
	else if (In.at(0) == "god") {
		if (In.size() == 2) {
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::SetMaxHealth(Human, HUGE_VAL);
					Sleep(100);
					Slix::SetHealth(Human, HUGE_VAL);
					success();
				}
			}
			else {
				failed();
			}
		}
	}
	else if (In.at(0) == "ungod") {
		if (In.size() == 2) {
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::SetMaxHealth(Human, 100);
					success();
				}
			}
			else {
				failed();
			}
		}
	}
	else if (In.at(0) == "time") {
		if (In.size() == 2) {
			double time = std::atof(In.at(1).c_str());
			Slix::SetTimeAfterMidnight((void*)Slix::Lighting,  time);
			success();
		}else{
			failed();
		}
	}
	else if (In.at(0) == "creeps") {
		if (In.size() == 1) {
			Slix::SetTimeAfterMidnight((void*)Slix::Lighting, 0);
			Slix::SetFogE((void*)Slix::Lighting, 120);
			success();
		}
	}
	else if (In.at(0) == "sh") {
		if (In.size() == 3) {
			float hlth = std::stof(In.at(2).c_str());
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::SetMaxHealth(Human, hlth);
					Slix::SetHealth(Human, hlth);
					success();
				}
			}
			else {
				failed();
			}
		}
		else {
			failed();
		}
	}
	else if (In.at(0) == "kill") {
		if (In.size() == 2) {
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::SetMaxHealth(Human, 0);
					success();
				}
			}
			else {
				failed();
			}
		}
	}
	else if (In.at(0) == "fog") {
		if (In.size() == 2) {
			float FogEnd = std::atof(In.at(1).c_str());
		#ifndef MODE_PROP_DESCRIPTOR
			Slix::SetFogE((void*)Slix::Lighting, FogEnd);
		#endif

		#ifdef MODE_PROP_DESCRIPTOR
			Slix::SetFogEnd(Slix::Lighting, FogEnd);
		#endif
			success();
		}
		else {
			failed();
		}
	}
	else if (In.at(0) == "fogs") {
		if (In.size() == 2) {
			float FogStart = std::atof(In.at(1).c_str());
#ifndef MODE_PROP_DESCRIPTOR
			Slix::SetFogS((void*)Slix::Lighting, FogStart);
#endif

#ifdef MODE_PROP_DESCRIPTOR
			Slix::SetFogStart(Slix::Lighting, FogStart);
#endif
			success();
		}
		else {
			failed();
		}
	}
	else if (In.at(0) == "killall") {
		if (In.size() == 1) {
			Slix::BreakJoints((void*)Slix::Workspace);
			success();
		}
	}
	else if (In.at(0) == "fogc") {
		if (In.size() == 4) {
			float R = std::atof(In.at(1).c_str());
			float G = std::atof(In.at(2).c_str());
			float B = std::atof(In.at(3).c_str());
#ifndef MODE_PROP_DESCRIPTOR
			float RGB[3] = { R, G, B };
			Slix::SetFogC((void*)Slix::Lighting, RGB);
#endif

#ifdef MODE_PROP_DESCRIPTOR
			Slix::SetFogColor(Slix::Lighting, R, G, B);
#endif
			success();
		}
		else {
			failed();
		}
	}
	else if (In.at(0) == "naked") {
		if (In.size() == 2) {
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int shirt = Slix::FindFirstChild(Char, "Shirt");
				if (shirt != 0) {
					int pant = Slix::FindFirstClass(Char, "Pants");
					if (pant != 0) {
						Slix::SetParent(shirt, Slix::Lighting);
						Slix::SetParent(pant, Slix::Lighting);
						success();
					}
				}
				else {
					failed();
				}
			}
			else {
				failed();
			}
		}
	}
	else if (In.at(0) == "removelimbs") {
		if (In.size() == 2) {
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int LA = Slix::FindFirstChild(Char, "Left Arm");
				if (LA != 0) {
					int RA = Slix::FindFirstChild(Char, "Right Arm");
					if (RA != 0) {
						int LL = Slix::FindFirstChild(Char, "Left Leg");
						if (LL != 0) {
							int RL = Slix::FindFirstChild(Char, "Right Leg");
							if (RL != 0) {
								Slix::SetParent(LA, Slix::Lighting);
								Slix::SetParent(RA, Slix::Lighting);
								Slix::SetParent(LL, Slix::Lighting);
								Slix::SetParent(RL, Slix::Lighting);
								success();
							}
							else { failed(); }
						}
						else { failed(); }
					}
					else { failed(); }
				}
				else { failed(); }
			}
			else {
				failed();
			}

		}
	}
	else if (In.at(0) == "dmg") {
		if (In.size() == 3) {
			float dmg = std::stof(In.at(2).c_str());
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::TakeDmg(Human, dmg);
					success();
				}
			}
			else {
				failed();
			}
		}
	}
	else if (In.at(0) == "freeze") {
		if (In.size() == 2) {
			int Character = Slix::GetCharacter(In.at(1).c_str());
			if (Character != 0) {
				int Humanoid = Slix::FindFirstClass(Character, "Humanoid");
				if (Humanoid != 0) {
					Slix::SetJumpPower(Humanoid, 0);
					Slix::SetWalkSpeed(Humanoid, 0);
					success();
				}
			}
			else {
				failed();
			}
		}
	}
	else if (In.at(0) == "unfreeze") {
		if (In.size() == 2) {
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::SetJumpPower(Human, 50);
					Slix::SetWalkSpeed(Human, 16);
				}
			}
		}
	}
	
	else if (In.at(0) == "ragdoll") {
		success();
		Slix::State = 1;
	}
	else if (In.at(0) == "swim") {
		success();
		Slix::State = 4;
	}
	else if (In.at(0) == "fly") {
		success();
		Slix::State = 12;
	}
	else if (In.at(0) == "unlimitedjump") {
		success();
		Slix::State = 14;
	}
	else if (In.at(0) == "hidename") {
		if (In.size() == 2) {
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::SetDisplayDist(Human, 0);
					success();
				}
			}
		}
	}
	else if (In.at(0) == "showname") {
		if (In.size() == 2) {
			int Char = Slix::GetCharacter(In.at(1).c_str());
			if (Char != 0) {
				int Human = Slix::FindFirstClass(Char, "Humanoid");
				if (Human != 0) {
					Slix::SetDisplayDist(Human, 100);
					success();
				}
			}
		}
	}
	else if (In.at(0) == "fov") {
		if (In.size() == 2) {
			float FOV = std::stof(In.at(0).c_str());
			int Camera = Slix::FindFirstClass(Slix::Workspace, "Camera");
			if (Camera != 0) {
				Slix::SetFOV(Camera, FOV);
				success();
			}
		}
	}
}



bool Start() {
	SetConsoleTitleW(L"Slix");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
	std::string *playername = Slix::GetName(Slix::GetLocalPlayer(Slix::Players));

	std::string user;
	std::string pass;
	std::cout << "User: ";
	std::cin >> user;
	std::cout << "\nPass: ";
	std::cin >> pass;
	


	for (int c = 0; c < _countof(WL::userNames); c++) {
		if (user == WL::userNames[c] && pass == WL::passWords[c]) {
			scanner();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			std::cout << "RSHIFT = Turn off running local command." << std::endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			std::cout << "Type 'cmds' to view a list available commands..." << std::endl;
			do {
				std::cout << ">";
				std::string In = Input();
				Commands(In);
				Loop();
			} while (true);
		}
	}


	scanner();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	std::cout << "RSHIFT = Turn off running local command." << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	std::cout << "Type 'cmds' to view a list available commands..." << std::endl;
	do {
		std::cout << ">";
		std::string In = Input();
		Commands(In);
		Loop();
	} while (true);
	return false;
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	HANDLE ourThread;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		printf("Slix attached..\n");
		LoadLibraryA("Slix.dll");
		ourThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Start, hinstDLL, CREATE_SUSPENDED, 0);
		ResumeThread(ourThread);
		break;

	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
