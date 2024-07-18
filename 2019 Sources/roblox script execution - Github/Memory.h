#pragma once
#include "Includes.h"

namespace Memory {

	bool Compare(const char* pData, const char* bMask, const char* szMask)
	{
		while (*szMask) {
			if (*szMask != '?') {
				if (*pData != *bMask) return 0;
			}
			++szMask, ++pData, ++bMask;
		}
		return 1;
	}

	DWORD Scan(const char* vftable)  // Credits goes to tepig
	{
		MEMORY_BASIC_INFORMATION BasicMemoryInformation = {};
		SYSTEM_INFO SystemInformation = {};
		GetSystemInfo(&SystemInformation);
		DWORD StartingMemorySearchPosition = (DWORD)SystemInformation.lpMinimumApplicationAddress;
		DWORD MaximumSearchBoundary = (DWORD)SystemInformation.lpMaximumApplicationAddress;
		do {
			while (VirtualQuery((void*)StartingMemorySearchPosition, &BasicMemoryInformation, sizeof(BasicMemoryInformation))) {
				if ((BasicMemoryInformation.Protect & PAGE_READWRITE) && !(BasicMemoryInformation.Protect & PAGE_GUARD)) {
					for (DWORD Key = (DWORD)(BasicMemoryInformation.BaseAddress); ((Key - (DWORD)(BasicMemoryInformation.BaseAddress) < BasicMemoryInformation.RegionSize)); ++Key) {
						if (Compare((const char*)Key, vftable, "xxxx")) {
							return Key;
						};
					};
				};
				StartingMemorySearchPosition += BasicMemoryInformation.RegionSize;
			};
		} while (StartingMemorySearchPosition < MaximumSearchBoundary);
		return 0x0;
	}
}