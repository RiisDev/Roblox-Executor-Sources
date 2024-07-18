#pragma once

#include <Windows.h>

namespace Memory {
	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask);
	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask);
	int Scan(DWORD mode, char* content, char* mask);
	int QuickScan(DWORD Mode, char* content, char* mask);
}