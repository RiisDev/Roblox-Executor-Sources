#pragma once
#include <Windows.h>
#include <WinINet.h>
#include <algorithm>
#include <random>
#include <typeinfo>

#define aslr(x)(x - 0x400000 + (int)GetModuleHandle(0)) //ASLR

typedef void(__thiscall *ReplicateProperty)(int Service, int Property);
ReplicateProperty Replicate = (ReplicateProperty)aslr(0x4A87E0); //Replicate Address

#define FogEnd_Prop aslr(0x157E4A8) //Prop
#define FogStart_Prop aslr(0x157E470) //Prop
#define FogColor_Prop aslr(0x157E438) //Prop
#define DisplayDistanceType_Prop aslr(0x15846B0) //Prop
int ScriptContext;
int DataModel;
int Workspace;
int Lighting;
int Players;
int LocalPlayer;
std::string *PlayerName;


std::string* GetName(int Instance) {
	return (std::string*)(*(int*)(Instance + 0x28)); //Returns Name
}


int GetLocalPlayer(int Player)
{
	return *(int*)(Player + 340); //Returns Local Player
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

const char* GetClass(int Self)
{
	try {
		return (const char*)(*(int(**)(void))(*(int*)Self + 16))(); //Returns Class
	}
	catch (...) {
		printf("Failure"); //Failure
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
				return *(int*)i; //Returns
			}
		}
	}
	catch (...) {
		printf("Failure."); //Failure
	}
}

std::string toLower(const char* str) {
	std::string newStr = std::string(str);
	std::transform(newStr.begin(), newStr.end(), newStr.begin(), ::tolower);
	return newStr;
}

int GetCharacter(std::string Name) {
	std::string lowerName = toLower(Name.c_str());
	if (Name == "me") {
		return FindFirstChild_PartOf(Workspace, *PlayerName);
	}
	DWORD StartOfChildren = *(DWORD*)(Workspace + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		std::string* ThingName = GetName(*(int*)i);
		std::string lowerT = toLower(ThingName->c_str());
		if (lowerT.find(lowerName) != std::string::npos) {
			return *(int*)i;
		}
	}
	return 0;
}

int GetPlayer(std::string Name) {
	std::string lowerName = toLower(Name.c_str());
	if (Name == "me") {
		return FindFirstChild_PartOf(Players, *PlayerName);
	}
	DWORD StartOfChildren = *(DWORD*)(Players + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		std::string* ThingName = GetName(*(int*)i);
		std::string lowerT = toLower(ThingName->c_str());
		if (ThingName->find(Name) != std::string::npos) {
			return *(int*)i;
		}
	}
	return 0;
}

int GetParent(int Instance)
{
	return *(int*)(Instance + 0x34); //Returns Instance's Parent
}

void SetFogEnd(int Lighting, float Fog)
{
	*(float*)(Lighting + 392) = Fog; //FogEnd Offset
	Replicate(Lighting, FogEnd_Prop); //Replicates
}

void SetFogStart(int Lighting, float Fog)
{
	*(float*)(Lighting + 388) = Fog; //FogStart Offset
	Replicate(Lighting, FogStart_Prop); //Replicates
}

void SetFogColor(int Lighting, float R, float G, float B)
{
	*(float*)(Lighting + 376) = R; //R Offset
	*(float*)(Lighting + 380) = G; //G Offset
	*(float*)(Lighting + 384) = B; //B Offset
	Replicate(Lighting, FogColor_Prop); //Replicates
}

void SetDisplayDistanceType(int Humanoid, int Type)
{
	int* Location = (int*)(Humanoid + 500); //Location Offset
	if (*Location != Type) //Checks if the Location does not match the Type
	{
		*Location = Type; //Location = Type
		Replicate(Humanoid, DisplayDistanceType_Prop); //Replicates
	}
}

int GetFilteringEnabled(int Workspace)
{
	return *(BYTE*)(Workspace + 0x25B); //Returns FilteringEnabled OffSet
}