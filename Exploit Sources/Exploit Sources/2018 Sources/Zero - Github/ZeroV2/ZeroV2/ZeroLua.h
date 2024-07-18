#pragma once
//#include "ReturnCheck.h"
#include "Scan.h"
#include "RBX.h"
#include "Main.h"
#include <string>


using namespace std;

DWORD getaddy(DWORD address) {
	DWORD Base = (DWORD)GetModuleHandleA("RobloxPlayerBeta.exe");
	return address - 0x400000 + Base;
}

DWORD Decrypt_ptr(DWORD Pointer) {
	return (*(DWORD*)((char*)Pointer + *(DWORD*)Pointer))--;
}

//void Disable_Callcheck() {
//	*(int *)((char *)getaddy(0x1525E1C) + (DWORD)aslr(0x1525E1C)) = 0;
//}

int lua_State;

bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;
	return (*szMask) == NULL;
}

DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i<dwLen; i++)
		if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);
	return 0;
}

DWORD unprotect(DWORD addr)
{
	BYTE* tAddr = (BYTE*)addr;

	/* Calcualte the size of the function
	In theory this will run until it hits the next
	functions prolog. It assumes all calls are aligned to
	16 bytes. (grazie katie)
	*/
	do
	{
		tAddr += 16;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	/* Allocate memory for the new function */
	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	/* Copy the function to the newly allocated memory */
	memcpy(nFunc, (void*)addr, funcSz);

	BYTE* pos = (BYTE*)nFunc;
	BOOL valid = false;
	do
	{
		/* Check for the return check with the sig:
		72 ?? A1 ?? ?? ?? ?? 8B
		If the sig matches replace the the jb with a jmp.
		*/
		if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
			*(BYTE*)pos = 0xEB;

			DWORD cByte = (DWORD)nFunc;
			do
			{
				/* Check if the current byte is a call if it is,
				calculate the new relative call(s).
				*(->E8 + 1) = originalFunction - nextInstruction

				oFuncPos - Position of call in original function
				= originalFunction + (->E8 - newFunction)

				oFuncAddr - Original call location
				= oFuncPos + rel32Offset + sizeof(call)

				relativeAddr - New relative address
				= oFuncAddr - ->E8 - sizeof(call)

				Since we are not using a disassembler we assume
				that if we hit a E8 byte which is properly aligned
				it is a relative call.
				For a small amount of compensation I skip location
				of the call, since it is possible to have the byte
				E8 inside of it.
				*/
				if (*(BYTE*)cByte == 0xE8)
				{
					DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
					DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

					if (oFuncAddr % 16 == 0)
					{
						DWORD relativeAddr = oFuncAddr - cByte - 5;
						*(DWORD*)(cByte + 1) = relativeAddr;

						/* Don't check rel32 */
						cByte += 4;
					}
				}

				cByte += 1;
			} while (cByte - (DWORD)nFunc < funcSz);

			valid = true;
		}
		pos += 1;
	} while ((DWORD)pos < (DWORD)nFunc + funcSz);

	/* This function has no return check, let's not waste memory */
	if (!valid)
	{
		VirtualFree(nFunc, funcSz, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}


DWORD SigScan(const char *AoB, char *Mask) {
	return FindPattern((DWORD)GetModuleHandleA("RobloxPlayerBeta.exe"), 0xF00000, (PBYTE)AoB, Mask);
}

#define lua_pop(L,n)				 lua_settop(L, -(n)-1)

typedef void(__stdcall *Lua_getfield)(int lua_State, int idx, const char *k);
Lua_getfield lua_getfield = (Lua_getfield)unprotect(getaddy(0x73DB40));
typedef int(__cdecl *Lua_pcall)(int lua_State, int naArgs, int nresults, int errfunc);
Lua_pcall lua_pcall2 = (Lua_pcall)unprotect(getaddy(0x73e910));
typedef void(__fastcall *Lua_setfield)(int lua_State, int idx, const char *k);
Lua_setfield lua_setfield2 = (Lua_setfield)unprotect(getaddy(0x73F8E0));
typedef void(__cdecl *Lua_pushstring)(int lua_State, const char *s);
Lua_pushstring lua_pushstring = (Lua_pushstring)getaddy(0x73EEE0);
typedef void(__fastcall *Lua_pushvalue)(int lua_State, int idx);
Lua_pushvalue lua_pushvalue = (Lua_pushvalue)unprotect(getaddy(0x73EFD0));
typedef void(__cdecl *Lua_settop)(int lua_State, int n);
Lua_settop lua_settop = (Lua_settop)unprotect(getaddy(0x73fcc0));
typedef void(__stdcall *Lua_pushnumber)(int lua_State, double number);
Lua_pushnumber lua_pushnumber = (Lua_pushnumber)unprotect(getaddy(0x73EE50));
typedef int(__cdecl *Lua_getmetatable)(int lua_State, int idx);
Lua_getmetatable lua_getmetatable = (Lua_getmetatable)unprotect(getaddy(0x73DE10));


void lua_pcall(int lst, int nargs, int nresults, int errfunc) {
	//Bypass RawrCheck(?)
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getaddy(0x736cc7)), "\xEB", 1, 0);
	lua_pcall2(lst, nargs, nresults, errfunc);
	//Restore RawrCheck(?)
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getaddy(0x736cc7)), "\x74", 1, 0);
}

void lua_setfield(int lua_State, int idx, const char *k)
{
	lua_pushvalue(lua_State, idx);
	if (lua_getmetatable(lua_State, -1)) {
		lua_getfield(lua_State, -1, "__newindex");
		lua_pushvalue(lua_State, -3);
		lua_pushstring(lua_State, k);
		lua_pushvalue(lua_State, -6);
		lua_pcall(lua_State, 3, 0, 0);
		lua_pop(lua_State, 3);
	}
	else {
		lua_pop(lua_State, 1);
		lua_setfield2(lua_State, idx, k);
	}
}

void lua_getglobal(int lua_State, const char *k) {
	lua_getfield(lua_State, -10002, k);
}

void lua_emptystack(int luaState)
{
	//auto ret = Returncheck();
	lua_settop(lua_State, 0);
	//ret();
};

void patchedret() {
	return;
}

void PrintIdentity() {
	lua_getfield(lua_State, -10002, "printidentity");
	lua_pcall(lua_State, 0, 0, 0);
}

void Print(std::string Input) {

	lua_getglobal(lua_State, "print");
	lua_pushstring(lua_State, Input.c_str());
	lua_pcall(lua_State, 1, 0, 0);

}

void Warn(std::string Input) {

	lua_getglobal(lua_State, "warn");
	lua_pushstring(lua_State, Input.c_str());
	lua_pcall(lua_State, 1, 0, 0);


}

void Music(std::string id) {

	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Workspace");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Sound");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 1, 0);

	lua_pushstring(lua_State, ("rbxassetid://" + id).c_str());
	lua_setfield(lua_State, -2, "SoundId");

	lua_pushstring(lua_State, "Sound");
	lua_setfield(lua_State, -2, "Name");

	lua_pushstring(lua_State, "1");
	lua_setfield(lua_State, -2, "Volume");

	lua_getfield(lua_State, -1, "Play");
	lua_pushvalue(lua_State, -2);
	lua_pcall(lua_State, 1, 0, 0);


}


void ChangeChar(std::string plr, std::string id)
{
	std::string idxd = "https://assetgame.roblox.com/Asset/CharacterFetch.ashx?userId=" + id + "&placeId=0";
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, plr.c_str());
	lua_pushstring(lua_State, idxd.c_str());
	lua_setfield(lua_State, -2, "CharacterAppearanceId");
	lua_pcall(lua_State, 1, 0, 0);
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, plr.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "Torso");
	lua_getfield(lua_State, -1, "Destroy");
	lua_pushvalue(lua_State, -2);
	lua_pcall(lua_State, 1, 0, 0);

}

void ForceField(std::string ye, std::string inst) {
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, inst.c_str());
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);

}

void Crash(std::string ye) {
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Backpack");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Script");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);


	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Backpack");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Script");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);


	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Backpack");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Script");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);


	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Backpack");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Script");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);



	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Backpack");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Script");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);



	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Backpack");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Script");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);


	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Backpack");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Script");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);


	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Backpack");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "HopperBin");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);

}

void CreateCharacterInstance(std::string ye, std::string inst) {
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "Torso");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, inst.c_str());
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);
}


void LeftArm(std::string ye, std::string inst) {
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "Left Arm");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, inst.c_str());
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);

}

void LeftLeg(std::string ye, std::string inst) {
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "Left Leg");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, inst.c_str());
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);

}


void RightArm(std::string ye, std::string inst) {
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "Right Arm");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, inst.c_str());
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);

}

void RightLeg(std::string ye, std::string inst) {
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "Right Leg");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, inst.c_str());
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);

}


void CreateHeadPart(std::string ye, std::string inst) {

	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, ye.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "Head");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, inst.c_str());
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);

}

void NewLighting(std::string inst) {
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Lighting");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, inst.c_str());
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 0, 0);

}

void NewTeam(std::string id) {

	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Teams");
	lua_getglobal(lua_State, "Instance");
	lua_getfield(lua_State, -1, "new");
	lua_pushstring(lua_State, "Team");
	lua_pushvalue(lua_State, -4);
	lua_pcall(lua_State, 2, 1, 0);

	lua_pushstring(lua_State, id.c_str());
	lua_setfield(lua_State, -2, "Name");
	lua_pcall(lua_State, 2, 1, 0);

}

void NoReset(std::string plr) {

	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, plr.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "Humanoid");
	lua_pushstring(lua_State, "NoReset");
	lua_setfield(lua_State, -2, "Name");
}

void ApocBadges(std::string plr) {

	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, plr.c_str());
	lua_getfield(lua_State, -1, "playerstats");
	lua_getfield(lua_State, -1, "Days");
	lua_pushstring(lua_State, "100");
	lua_setfield(lua_State, -2, "Value");

	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, plr.c_str());
	lua_getfield(lua_State, -1, "playerstats");
	lua_getfield(lua_State, -1, "ZombieKill");
	lua_getfield(lua_State, -1, "Military");
	lua_pushstring(lua_State, "100");
	lua_setfield(lua_State, -2, "Value");

	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, plr.c_str());
	lua_getfield(lua_State, -1, "playerstats");
	lua_getfield(lua_State, -1, "PlayerKill");
	lua_getfield(lua_State, -1, "Bandit");
	lua_pushstring(lua_State, "100");
	lua_setfield(lua_State, -2, "Value");

}

void AllowReset(std::string plr) {

	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Players");
	lua_getfield(lua_State, -1, plr.c_str());
	lua_getfield(lua_State, -1, "Character");
	lua_getfield(lua_State, -1, "NoReset");
	lua_pushstring(lua_State, "Humanoid");
	lua_setfield(lua_State, -2, "Name");

}
void LightingUpdate(std::string LightingProperty, std::string value)
{
	lua_getglobal(lua_State, "game");
	lua_getfield(lua_State, -1, "Lighting");
	lua_pushstring(lua_State, value.c_str());
	lua_setfield(lua_State, -2, LightingProperty.c_str());

}