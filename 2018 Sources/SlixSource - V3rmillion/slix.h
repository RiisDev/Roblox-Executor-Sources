#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

namespace Slix {

	int DataModel = 0;
	int HopperBin = 0;
	int Instance = 0;
	int Lighting = 0;
	int Workspace = 0;
	int Players = 0;
	int State = 18;

	std::string *PlayerName;
#ifdef MODE_PROP_DESCRIPTOR
	#define	FogEnd_PropDescriptor (DWORD*)0x16404D4
	#define	FogColor_PropDescriptor (DWORD*)0x164046C
	#define FogStart_PropDescriptor (DWORD*)0x16404A0
	#define DisplayDistanceType_PropDescriptor (DWORD*)0x163FA98

#endif
	typedef void(__thiscall *settime)(void* self, double time);
#ifndef MODE_PROP_DESCRIPTOR
	typedef void(__thiscall *FogEnd)(void* self, float value);
	typedef void(__thiscall *FogStart)(void* self, float value);
	typedef void(__thiscall *FogColor)(void* self, float RGB[3]);
	typedef void(__thiscall *DisplayDistance)(int self, int value);
#endif
	typedef void(__thiscall *Spd)(int self, float val);
	typedef const char* (__cdecl *lua_LoadString)(int lua_State, int a2, const char * LuaCode);
	typedef void(__thiscall *breakJoints)(void* self);
	typedef void(__thiscall *rProp)(int self, DWORD* r);
	typedef void(__thiscall *setParent)(int parent, int newparent);
	typedef void(__thiscall *getChildren)(void* self);
	typedef void(__thiscall *setValue)(int self, float value);
	typedef void(__thiscall *clrChildren)(void* self);
	typedef void(__thiscall *changeName)(int self, std::string Name);
	typedef void(__thiscall *respawn)(int self);
	typedef void(__thiscall *transParency)(int self, float trans);
	typedef void(__thiscall *FOV)(int self, float fov);


	static lua_LoadString lua_loadstring;
	int luaState = 0;


	FOV SetFOV = (FOV)0x5D5A60;
	transParency SetTrans = (transParency)0x8D94A0;
	respawn Respawn = (respawn)0x6E5FD0;
	clrChildren ClrAllChild = (clrChildren)0x511960;
	setValue SetValue = (setValue)0xE6E2F0;
	getChildren GetChildren = (getChildren)0x513040;
	settime SetTimeAfterMidnight = (settime)0x6DA0C0;
#ifndef MODE_PROP_DESCRIPTOR
	FogEnd SetFogE = (FogEnd)0x6D9FA0;
	FogColor SetFogC = (FogColor)0x6D9F30;
	FogStart SetFogS = (FogStart)0x6D9FD0;
	DisplayDistance SetDisplayDist = (DisplayDistance)0x6CD6C0;
#endif
	breakJoints BreakJoints = (breakJoints)0x5D8BE0;
	setParent SetParent = (setParent)0x4125E0;
	Spd SetWalkSpeed = (Spd)0x6CEA70;
	Spd SetJumpPower = (Spd)0x6CDB80;
	Spd TakeDmg = (Spd)0x6CEFE0;
	Spd SetMaxHealth = (Spd)0x6CDFA0;
	Spd SetHealth = (Spd)0x6CD9C0;
	rProp ReplicateProp = (rProp)0x8F80B0;

#ifdef MODE_PROP_DESCRIPTOR
	void SetFogEnd(int Lighting, float value)
	{
		*(float *)(Lighting + 392) = value;
		ReplicateProp(Lighting, FogEnd_PropDescriptor);
	}

	void SetFogColor(int Lighting, float R, float G, float B)
	{
		*(float*)(Lighting + 376) = R;
		*(float*)(Lighting + 380) = G;
		*(float*)(Lighting + 384) = B;
		ReplicateProp(Lighting, FogColor_PropDescriptor);
	}
	void SetFogStart(int Lighting, float value) {
		*(float*)(Lighting + 388) = value;
		ReplicateProp(Lighting, FogStart_PropDescriptor);
	}
	void SetDisplayDistanceType(int Humanoid, int Type)
	{
		int* Location = (int*)(Humanoid + 484);
		if (*Location != Type) {
			*Location = Type;
			ReplicateProp(Humanoid, DisplayDistanceType_PropDescriptor);
		}
	}
#endif
	void SetHumanoidState(int self, int state) {
		int sPtr = *(int*)(self + 0x2DC);
		if (sPtr) {
			*(BYTE*)(self + state + 0x229) = 1;
			*(int*)(sPtr + 0x20) = state;
		}
	}

	int GetParent(int Instance) {
		return *(int*)(Instance + 0x34);
	}

	std::string* GetName(int Instance) {
		return (std::string*)(*(int*)(Instance + 0x28));
	}

	const char* GetClass(int self)
	{
		return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
	}

	int FindFirstChild(int Instance, std::string Name) {
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

	int FindFirstChildd(int Instance, std::string Name) {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			std::string* CurrentName = GetName(*(int*)i);
			CurrentName->resize(Name.size());
			if (*CurrentName == Name)
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


	int GetCharacter(std::string Name) {

		if (Name == "me") {
			int Character = FindFirstChild(Workspace, *PlayerName);
			if (Character != 0) {
				return Character;
			}
			else
			{
				return 0;
			}
		}

		int Character = FindFirstChild(Workspace, Name);
		if (Character != 0) {
			return Character;
		}
		else
		{
			return 0;
		}
	}
	int GetPlayer(std::string Name) {
		int Player = FindFirstChild(Players, Name);
		if (Player != 0) {
			return Player;
		}
		else
		{
			return 0;
		}
	}

	int GetService(int Service) {
		return *(int*)(Service + 0x2F4);
	}
}