#pragma once

#include <windows.h>
#include "MemoryLocations.h"

#ifndef rawrxd
#define rawrxd

DWORD lpflOldProtect;

void BypassRawr() {
	VirtualProtect((void*)location_RawrJZ, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	//Changes protection to PAGE_EXECUTE_READWRITE so that we can modify the instruction
	memcpy((void*)location_RawrJZ, "\xEB", 1);
	//Changes the instruction to an unconditional short jmp
	VirtualProtect((void*)location_RawrJZ, 5, lpflOldProtect, &lpflOldProtect);
	//Restores original protection
}

void RestoreRawr() {
	VirtualProtect((void*)location_RawrJZ, 5, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	//Changes protection to PAGE_EXECUTE_READWRITE so that we can modify the instruction
	memcpy((void*)location_RawrJZ, "\x74", 1);
	//Changes the instruction back to jz
	VirtualProtect((void*)location_RawrJZ, 5, lpflOldProtect, &lpflOldProtect);
	//Restores original protection
}

#endif