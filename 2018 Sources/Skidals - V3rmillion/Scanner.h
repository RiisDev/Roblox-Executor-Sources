#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include <istream>
#include <iterator>
#include "RLua.h"
#define _CRT_SECURE_NO_WARNINGS
int scriptcontext, robloxluastate;
namespace Memory {
	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask) return 0;
		return (*szMask) == NULL;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, const char *szMask)
	{
		for (int i = 0; i<(int)dwLen; i++)
			if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}

	DWORD FastScan(const char* content)
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
		return 0;
	}
}

void scan() {
	DWORD SContext = aslr(0x111915C);

	scriptcontext = Memory::FastScan((char*)&SContext);
	robloxluastate = *(DWORD *)(scriptcontext + 56 * 1 + 164 ) - (scriptcontext + 56 * 1 + 164);
}