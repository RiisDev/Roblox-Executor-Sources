#pragma once
#include <Windows.h>
#include "lua.h"
#include "Addresses_1.h"

DWORD dynamicbase;
bool bCompare3(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;
	return (*szMask) == NULL;
}

DWORD FindPattern3(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i<dwLen; i++)
		if (bCompare3((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);
	return 0;
}

DWORD SigScan3(const char *AoB, char *Mask) {
	return FindPattern3((DWORD)GetModuleHandleA("RobloxPlayerBeta.exe"), 0xF00000, (PBYTE)AoB, Mask);
}


auto Returncheck()
{
	int Retcheck = RReturnCheckFunction_address;
	int RetFlag1 = RReturnFlag1_address;
	int RetFlag2 = RReturnFlag2_address;

	int flag1_val = *(int*)(RetFlag1);
	int flag2_val = *(int*)(RetFlag2);
	DWORD nOldProtect;
	BYTE OldJump = *(BYTE*)(Retcheck);
	VirtualProtect((LPVOID)(Retcheck), 1, PAGE_EXECUTE_READWRITE, &nOldProtect);
	*(BYTE*)(Retcheck) = 0xC3;
	return [&]() {
		*(int*)RetFlag1 = flag1_val;
		*(int*)RetFlag2 = flag2_val;
		*(BYTE*)(Retcheck) = OldJump;
		VirtualProtect((PVOID)(Retcheck), 1, nOldProtect, &nOldProtect);
	};
}

