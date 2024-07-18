#pragma once

#include <windows.h>
#include "MemoryLocations.h"

#ifndef clout
#define clout

DWORD storeFlag1, storeFlag2, storeFunc, lpflOldProtect;

inline void BypassRet() {
	storeFlag1 = *(long*)location_RETFLAG1;
	storeFunc = *(long*)location_RETFUNC;
	storeFlag2 = *(long*)location_RETFLAG2;
	VirtualProtect((void*)location_RETFUNC, 1, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)location_RETFUNC, "\xC2", 1);
}

inline void RestoreRet() {
	*(long*)location_RETFLAG1 = storeFlag1;
	*(long*)location_RETFUNC = storeFunc;
	*(long*)location_RETFLAG2 = storeFlag2;
	VirtualProtect((void*)location_RETFUNC, 1, lpflOldProtect, &lpflOldProtect);
}

#endif

//yeah this is basically the same method as louka's but it's the best way i could think of anyways