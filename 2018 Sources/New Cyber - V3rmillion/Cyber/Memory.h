#pragma once

#include <Windows.h>

#include "Globals.hpp"
#include "Addresses.h"

bool Compare(const char* pData, const char* bMask, const char* szMask) {
	while (*szMask) {
		if (*szMask != '?') {
			if (*pData != *bMask) {
				return 0;
			};
		};
		++szMask, ++pData, ++bMask;
	};
	return 1;
};

DWORD ScanA(const char* vftable) {
	MEMORY_BASIC_INFORMATION MBI = { 0 };
	SYSTEM_INFO SI = { 0 };
	GetSystemInfo(&SI);
	DWORD Start = (DWORD)SI.lpMinimumApplicationAddress;
	DWORD End = (DWORD)SI.lpMaximumApplicationAddress;
	do {
		while (VirtualQuery((void*)Start, &MBI, sizeof(MBI))) {
			if ((MBI.Protect & PAGE_READWRITE) && !(MBI.Protect & PAGE_GUARD)) {
				for (DWORD i = (DWORD)(MBI.BaseAddress); i - (DWORD)(MBI.BaseAddress) < MBI.RegionSize; ++i) {
					if (Compare((const char*)i, vftable, "xxxx"))
						return i;
				};
			};
			Start += MBI.RegionSize;
		};
	} while (Start < End);
	return 0;
};

bool Scan() {
	ScriptContext = (DWORD)(ScanA((char*)&aScriptContext));
	if (!ScriptContext) {
		return false;
	};
	rL = rL_def;
	if (!rL) {
		return false;
	};
	return true;
};