#pragma once
#include <stdexcept>

static DWORD ROBLOX_BASE = (DWORD)GetModuleHandle(NULL);//DWORD ROBLOX_BASE = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");
#define rbxaddy(x) (x - 0x400000 + ROBLOX_BASE)
#define rbxdword(x) (x - 0x400000 + ROBLOX_BASE)

//int UserId_1_ADDR = rbxaddy(0x259DA30); // int
//int UserId_2_ADDR = rbxaddy(0x2146028); // string
//////////////////////////////////////
//static int aBreakJoints = rbxaddy(0x61F280);
#define RReturnCheckFunction_address                    rbxaddy(0x592500)
#define RReturnFlag1_address                            rbxaddy(0x1570740)
#define RReturnFlag2_address                            rbxaddy(0x15727AC)
/*static int aClearTerrain = rbxaddy(0x8626E0);
static int aSetTimeout = rbxaddy(0x545FB0);
static int aUnequipTools = rbxaddy(0x6E7200);*/

//static void BreakJoints(int Instance);

typedef void(__thiscall *mls)(int self, float c);
typedef void(__thiscall *zfunction)(void* self);
typedef void(__thiscall *functiondouble)(void* self, double a1);
typedef void(__thiscall *functionint)(void* self, double a1);
typedef void(__thiscall *ints)(int self, int a1);
typedef void(__thiscall *intstring)(int self, std::string a1);
typedef void(__thiscall *intbool)(int self, bool a1);
typedef void(__thiscall *intself)(int self);

//static mls _setMaxZoomDistance = (mls)aCameraMaxZoomDistance;

//static functiondouble _SetTimeout = (functiondouble)aSetTimeout;

typedef void(__thiscall *settime)(int self, float a2);
//static settime _SetMinutesAfterMidnight = (settime)aSetMinutesAfterMidnight;

//static functionint setval = (functionint)valsub;

//static zfunction _BreakJoints = (zfunction)aBreakJoints;
/*static zfunction _ClearTerrain = (zfunction)aClearTerrain;
static zfunction _UnequipTools = (zfunction)&aUnequipTools;*/

typedef DWORD(__cdecl *potatoX2)(DWORD self, DWORD cake, const char* message, ...);

static intself _PrintIdentity = (intself)0x50FAC0;//0x510700;//0x4ECDE0;
static potatoX2 WarnMessage = (potatoX2)0x569BF0;//0x4ECDE0;

static void PrintIdentity(int num) {
	try {
		_PrintIdentity(num);
	}
	catch (std::exception& e) {
		MessageBoxA(0, e.what(), "ERROR", 0);
	}
	catch (...) {
		MessageBoxA(0, "A unexpected error has occured!", "", 0);
	}
}

/*static void UnequipTools(int Humanoid) {
	if (Humanoid > 1000) {
		try {
			_UnequipTools((void*)Humanoid); Sleep(5);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "ERROR", 0);
		}
		catch (...) {}
	}
}*/


// TERRAIN

/*static void ClearTerrain(int Terrain) {
	if (Terrain > 1000) {
		try {
			_ClearTerrain((void*)Terrain);
		}
		catch (std::invalid_argument e) {
			MessageBoxA(0, e.what(), "Invalid argument!", 0);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "ERROR", 0);
		}
		catch (...) {
			MessageBoxA(0, "A unexpected error has occured!", "", 0);
		}
	}
}*/

/*static void BreakJoints(int Instance) {
	if (Instance > 1000) {
		try {
			_BreakJoints((void*)Instance);
		}
		catch (std::invalid_argument e) {
			MessageBoxA(0, e.what(), "Invalid argument!", 0);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "ERROR", 0);
		}
		catch (...) {
			MessageBoxA(0, "A unexpected error has occured!", "", 0);
		}
	}
}*/

/*static void SetTimeout(int self, double seconds) {
	if (self > 1000) {
		try {
			_SetTimeout((void*)self, seconds);
		}
		catch (std::exception& e) {
			MessageBoxA(0, e.what(), "ERROR", 0);
		}
		catch (...) {
			MessageBoxA(0, "A unexpected error has occured!", "", 0);
		}
	}
}*/