#pragma once
#include <Windows.h>
#include "Psapi.h"

using namespace std;

MODULEINFO GetModuleInfo(char *szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0) return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask) return 0;
	return (*szMask) == NULL;
}
/*
DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
{
for (int i = 0; i<(int)dwLen; i++)
if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
return 0;
}
*/
DWORD FindPatternA(char *module, char *arrayofbytes, char *szMask) {
	MODULEINFO mInfo = GetModuleInfo(module);
	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;
	DWORD lengthofpattern = (DWORD)strlen(szMask);

	for (DWORD i = 0; i < size - lengthofpattern; i++) {
		bool found = true;
		for (DWORD justgay = 0; justgay < lengthofpattern; justgay++) {
			found &= szMask[justgay] == '?' || arrayofbytes[justgay] == *(char*)(base + i + justgay);
		}

		if (found) {
			return base + i;
		}
	}
	return NULL;
}

DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char* szMask)
{
	__try
	{
		for (int i = 0; i<(int)dwLen; i++)
			if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//MessageBox(NULL, "Failed", "Error", MB_OK);
	}

	return 0x00400000;
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
