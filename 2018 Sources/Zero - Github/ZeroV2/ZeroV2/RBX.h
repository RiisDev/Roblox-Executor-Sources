#pragma once
#include "Scan.h"
#include "ZeroLua.h"
#include <Windows.h>
#include <algorithm>
#include <functional>
#include <WinInet.h>
#include <string>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
#include <limits>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <windef.h>
#include "Main.h"
//#include "VMProtectSDK.h"
#include <stdexcept>

int DataModel = 0;
int Workspace = 0;
int Lighting = 0;
int Players = 0;
int ScriptContext = 0;
int State = 0;
int CurrentPlayer;
std::string *PlayerName;
bool IsFogBypassed = FALSE;


int GetParent(int Instance) {
	try {
		return *(int*)(Instance + 0x34);
	}
	catch (...) {
		printf("Couldn't get Parent!");
	}
}

const char* GetClass(int self)
{
	try {
		return (const char*)(*(int(**)(void))(*(int*)self + 16))();
	}
	catch (...) {
		printf("Couldn't get Class!");
	}
}


int FindFirstClass(int Instance, const char* Name)
{
	try {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
			{
				return *(int*)i;
			}
		}
	}
	catch (...) {
		printf("Couldn't get FirstClass!");
	}
}

int GetService(const char* Name) {
	try {
		DWORD StartOfChildren = *(DWORD*)(DataModel + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
			{
				return *(int*)i;
			}
		}

		return 0;
	}
	catch (...) {
		printf("Could not call GetService!");
	}
}

std::string* GetName(int Instance) {
	try {
		return (std::string*)(*(int*)(Instance + 40));
	}
	catch (...) {
		printf("Couldn't get Name!");
	}
}



int FindFirstChild_PartOf(int Instance, std::string Name) {
	try {
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
	catch (...) {
		printf("Couldn't get FirstClassPart_Of!");
	}

}



int GetLocalPlayer(int Player) {
	try {
		return *(int*)(Player + 196);
	}
	catch (...) {
		printf("Couldn't Get Local Player!");
	}

}

std::string to_lowercase(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(std::tolower));
	return s;
}

int FindFirstPlayer(int Instance, std::string Name) {
	if (Instance > 10000) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		if (Name == "me") {
			return GetLocalPlayer(Players);
		}
		else {
			if (StartOfChildren != 0) {
				DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
				if (EndOfChildren != 0) {
					for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
						try {
							if (strstr(to_lowercase(*GetName(*(int*)i)).c_str(), to_lowercase(Name).c_str())) {
								return *(int*)i;
							}
						}
						catch (std::exception) {
							Sleep(1);
						}
						catch (...) {
							Sleep(1);
						}
					}
				}
			}
		}
	}
	return 0;
}

DWORD GetCharacterFromPlayer(DWORD player) {
	if (player > 1000) {
		//if ((int)(((int*)player) + 0x48) > 1000) {
		__try {
			return *(DWORD *)(player + 88);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return 0;
		}
		//}
	}
	return 0;
};


int GetCharacter(std::string Name) {
	if (Name != "") {
		if (Name == "me") {
			int Player = GetLocalPlayer(Players);
			if (Player != 0) {
				return GetCharacterFromPlayer(Player);
			}
			else {
				return 0;
			}
		}
		int Player = FindFirstPlayer(Players, Name);
		if (Player != 0) {
			return GetCharacterFromPlayer(Player);
		}
	}
	return 0;
}

int GetId(int Player) {
	return *(int*)(Player + 168);
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

void RunFogBypass() {
	if (IsFogBypassed == FALSE) {

	}

	if (IsFogBypassed == TRUE) {

	}
}



typedef int*(__cdecl *GetContextLevel)();
GetContextLevel ContextLevel = (GetContextLevel)getaddy(0x59a8c0);

int *ChangeContextLevel() {
	int *Level = ContextLevel();
	if (*Level == 0) {
		*Level = 6;
	}
	return Level;
}

int GetFilteringEnabled(int Workspace) {
	return *(BYTE*)(Workspace + 619);
}

