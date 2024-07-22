#pragma once
#include "SDK.h"


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

	DWORD SigScan(const char *AoB, char *Mask) {
		for (DWORD i = (DWORD)GetModuleHandle(NULL); i <= 0xF000000; ++i) {

			if (Memory::Compare((BYTE*)i, (BYTE*)AoB, Mask))
				return i;
		}
		return 0;
	}

	DWORD Scan(char* content)
	{
		SYSTEM_INFO SystemInfo;
		GetSystemInfo(&SystemInfo);
		DWORD PageSize = SystemInfo.dwPageSize;
		MEMORY_BASIC_INFORMATION meminfo;
		DWORD Start = (DWORD)SystemInfo.lpMinimumApplicationAddress;
		DWORD End = (DWORD)SystemInfo.lpMaximumApplicationAddress;
		for (DWORD lpAddress = Start; lpAddress <= End; lpAddress += PageSize)
		{
			VirtualQuery((void*)lpAddress, &meminfo, PageSize);
			if (meminfo.Type == MEM_MAPPED) continue;
			if (meminfo.Protect == PAGE_READWRITE) {
				DWORD Address = FindPattern(lpAddress, PageSize, (PBYTE)content, "xxxx");
				if (Address != NULL)
					return Address;
			}
		}
	}


}