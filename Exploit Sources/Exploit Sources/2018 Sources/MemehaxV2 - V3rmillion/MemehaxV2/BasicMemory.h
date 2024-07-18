#include <windows.h>

#ifndef layers
#define layers

inline long ASLR(long Address) {
	return (Address - 0x400000 + (DWORD)GetModuleHandle(0));
}

inline bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask) {
		if (*szMask == 'x' && *pData != *bMask) { return 0; }
	}
	return (*szMask) == 0;
}

inline long FindPattern(long dwAddress, long dwLen, BYTE *bMask, char *szMask)
{
	for (long i = 0; i < dwLen; i++) {
		if (Compare((BYTE*)(dwAddress + i), bMask, szMask)) { return (long)(dwAddress + i); }
	}
	return 0;
}

#endif