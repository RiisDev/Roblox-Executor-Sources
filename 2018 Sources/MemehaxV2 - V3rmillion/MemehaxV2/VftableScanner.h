#include <windows.h>
#include "BasicMemory.h"

#ifndef goteem
#define goteem

inline long Scan(char* content)
{
	MEMORY_BASIC_INFORMATION meminfo;
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	long PageSize = (long)SystemInfo.dwPageSize;
	long Start = (long)SystemInfo.lpMinimumApplicationAddress;
	long End = (long)SystemInfo.lpMaximumApplicationAddress;
	do {
		while (VirtualQuery((void*)Start, &meminfo, PageSize) == sizeof(meminfo)) {
			if ((meminfo.Protect == PAGE_READWRITE))  {
				long ScannedAddress = FindPattern(Start, PageSize, (PBYTE)content, "xxxx");
				if (ScannedAddress != 0) return ScannedAddress;
			}
			Start += PageSize;
		}
	} while (Start <= End);
}

#endif