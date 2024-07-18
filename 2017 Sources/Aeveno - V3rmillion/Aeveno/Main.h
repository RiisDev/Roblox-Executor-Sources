/*
README:
Please note: This is just a tiny simulator for people who want to make exploits.
This exploit is semi-working, which it will execute the function, then ROBLOX will shutdown the game, However, the function will be already replicated to the server with the changes,
After the changes, ROBLOX will shutdown, to see it actually "replicates" just join the back the exact same (server/game).
If you want to buy one my exploit's source code (Lithium) which still works to day with some updating in addresses. It also uses Lua C. It goes for (10$) PayPal.
Please contact me on Discord If you are interested in purchasing. https://discord.gg/M47gF8j and contact me.
All side from this, If you just take this without knowing what it does personally, it won't help you in the long run. Please credit me if you desire to implement
these functions into your exploit.

Thank You!
- x∂єηιαℓιту
*/

#pragma once
#include "InstanceInteract.h"
#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include "Memory.h"

#pragma warning (disable: 4996) //Disables the warning 4996

std::string Input()
{
	std::string Aeveno;
	getline(std::cin, Aeveno); //Get Line
	return Aeveno; //Returns the String
}

std::vector<std::string> split(std::string String) {
	std::vector<std::string> Element;
	std::stringstream SString(String);
	std::istream_iterator<std::string> Begin(SString);
	std::istream_iterator<std::string> End;
	std::vector<std::string> VirtualString(Begin, End);
	return VirtualString;
}


void DoCommand(std::string Command)
{
	std::vector<std::string> In = split(Command); //In.at

	if (In.at(0) == "cmds")
	{
		printf("cmds - Shows the list of commands.\r\n");
		printf("fogend [n] - Sets the fogend to [n].\r\n");
		printf("fogstart [n] - Sets the fogstart to [n].\r\n");
		printf("fogcolor [n] [n] [n] - Sets the fogcolor to [n] [n] [n].\r\n");
		printf("hidename [p] - Hides [p]'s name.\r\n");
		printf("showname [p] - Shows [p]'s name.\r\n");
		printf("exit - Exits the exploit.\r\n");
	}

	else if (In.at(0) == "fogend")
	{
		if (In.size() == 2)
		{
			float Num = ::atof(In.at(1).c_str()); //Num = Number
			SetFogStart(Lighting, Num); //Sets the FogStart Value
			SetFogEnd(Lighting, Num + 15); //Sets the FogEnd to its Number + 15
		}
	}

	else if (In.at(0) == "fogstart")
	{
		if (In.size() == 2)
		{
			float Num = ::atof(In.at(1).c_str()); //Num = Number
			SetFogStart(Lighting, Num); //Sets the FogStart Value
		}
	}

	else if (In.at(0) == "fogcolor")
	{
		if (In.size() == 4)
		{
			float No1 = ::atof(In.at(1).c_str());
			float No2 = ::atof(In.at(2).c_str());
			float No3 = ::atof(In.at(3).c_str());
			SetFogColor(Lighting, No1, No2, No3); //Set FogColor Values
		}
	}
	else if (In.at(0) == "hidename")
	{
		if (In.size() == 2)
		{
			int Character = GetCharacter(In.at(1));
			if (Character != 0)
			{
				int Humanoid = FindFirstClass(Character, "Humanoid");
				if (Humanoid != 0)
				{
					SetDisplayDistanceType(Humanoid, 2); //Set's DisplayDistanceType to 2
				}

			}
		}
	}

	else if (In.at(0) == "showname")
	{
		if (In.size() == 2)
		{
			int Character = GetCharacter(In.at(1));
			if (Character != 0)
			{
				int Humanoid = FindFirstClass(Character, "Humanoid");
				if (Humanoid != 0)
				{
					SetDisplayDistanceType(Humanoid, 0); //Set's DisplayDistanceType to 0
				}

			}
		}
	}

	else if (In.at(0) == "exit")
	{
		exit(1); //Exits
	}

}

void Main()
{
	DWORD OldPerm, OldPermMsg, Base;

	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &OldPerm);
	VirtualProtect(&MessageBoxA, 5, PAGE_EXECUTE_READWRITE, &OldPerm);
	*(BYTE*)(&FreeConsole) = 0xC3; //FreeConsole aka ConsoleCheck Bypass
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow(); //GetConsoleWindow
	MoveWindow(ConsoleHandle, 500, 500, 565, 500, TRUE);
	SetConsoleTitle(L"Aeveno V1 - By Deniality"); //Console Title

	printf("Aeveno V1 - A semi-working exploit. When you do a command, it will update to the server and it will shutdown.\r\nIt actually works when you rejoin.\r\n");

	Sleep(2500);

	printf("Scanning..\r\n");

	DWORD DataModelA = aslr(0x01198FBC); //ScriptContext Vftable
	ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&DataModelA, "xxxx"); //Memory Scanner
	printf("ScriptContext.. OK\r\n");
	DataModel = GetParent(ScriptContext); //ScriptContext is a child in the DataModel, Getting the Parent of ScriptParent which is "game" or DataModel
	printf("DataModel.. OK\r\n");
	Workspace = FindFirstClass(DataModel, "Workspace"); //Finds Workspace
	printf("Workspace.. OK\r\n");
	Lighting = FindFirstClass(DataModel, "Lighting"); //Finds Lighting
	printf("Lighting.. OK\r\n");
	Players = FindFirstClass(DataModel, "Players"); //Finds Players
	printf("Players.. OK\r\n"); 
	LocalPlayer = GetLocalPlayer(Players); //Find LocalPlayer
	printf("LocalPlayer.. OK\r\n");
	printf("Aeveno has loaded.\r\nType 'cmds' for the list of commands.\r\n");

	int FilteringEnabled = GetFilteringEnabled(Workspace); //Get Filtering Enabled

	if (!FilteringEnabled)
	{
		printf("Filtering is disabled.\r\n"); //Filtering is Disabled
	}
	else
	{
		printf("Filtering is enabled.\r\n"); //Filtering is Enabled
	}

	while (1)
	{
		std::cout << "> "; //PlaceHolder 
		std::string I = Input();
		DoCommand(I);
	}

}