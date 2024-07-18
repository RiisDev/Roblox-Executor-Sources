#include <windows.h>
#include "BasicMemory.h"

#ifndef aiightfam
#define aiightfam

long SigScan(char *pbMask, char *szMask)
{
	return FindPattern((long)GetModuleHandleA(0), 0xF000000, (PBYTE)pbMask, szMask);
}

#endif