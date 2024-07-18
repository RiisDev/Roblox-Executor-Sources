#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <Richedit.h>
#include <CommCtrl.h>
#include <WinInet.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <algorithm>
#include <iterator>
#include <ios>
#include <iosfwd>
#include <intrin.h>

void CBP(const char* Title) {
	DWORD superskiddoisachink;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &superskiddoisachink);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}


namespace Memory2
{


	// Credits to Austin

	bool compare(const char* location, const char* pattern, const char* mask)
	{
		for (; *mask; ++pattern, ++mask, ++location)
		{
			if (*mask == 'x' && *location != *pattern)
			{
				return false;
			}
		}
		return true;
	}
	uintptr_t scan(const char* pattern, const char* mask, uintptr_t start, uintptr_t end)
	{
		for (; start <= end; ++start)
		{
			if (compare((char*)start, (char*)pattern, mask))
			{
				return start;
			}
		}
		return 0;
	}
	uintptr_t scan(const char* pattern, const char* mask)
	{
		MODULEINFO moduleInfo;
		if (GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &moduleInfo, sizeof(moduleInfo)))
			return scan(pattern, mask, (uintptr_t)moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage + (uintptr_t)moduleInfo.lpBaseOfDll);
		return 0;
	}

	DWORD dwFindPattern(unsigned char* pData, unsigned int end_addr, const unsigned char* pat, const char* msk)
	{
		const unsigned char* end = (unsigned char*)end_addr - strlen(msk);
		int num_masks = ceil((float)strlen(msk) / (float)16);
		int masks[32]; //32*16 = enough masks for 512 bytes
		memset(masks, 0, num_masks * sizeof(int));
		for (int i = 0; i < num_masks; ++i)
			for (int j = strnlen(msk + i * 16, 16) - 1; j >= 0; --j)
				if (msk[i * 16 + j] == 'x')
					masks[i] |= 1 << j;

		__m128i xmm1 = _mm_loadu_si128((const __m128i*) pat);
		__m128i xmm2, xmm3, mask;
		for (; pData != end; _mm_prefetch((const char*)(++pData + 64), _MM_HINT_NTA)) {
			if (pat[0] == pData[0]) {
				xmm2 = _mm_loadu_si128((const __m128i*) pData);
				mask = _mm_cmpeq_epi8(xmm1, xmm2);
				if ((_mm_movemask_epi8(mask) & masks[0]) == masks[0]) {
					for (int i = 1; i < num_masks; ++i) {
						xmm2 = _mm_loadu_si128((const __m128i*) (pData + i * 16));
						xmm3 = _mm_loadu_si128((const __m128i*) (pat + i * 16));
						mask = _mm_cmpeq_epi8(xmm2, xmm3);
						if ((_mm_movemask_epi8(mask) & masks[i]) == masks[i]) {
							if ((i + 1) == num_masks)
								return (DWORD)pData;
						}
						else goto cont;
					}
					return (DWORD)pData;
				}
			}cont:;
		}
		return NULL;
	}



	uintptr_t scan(char* pattern, size_t start = 0x400000, size_t end = 0xffffffff)
	{
		MEMORY_BASIC_INFORMATION mbi;
		char* buffer = reinterpret_cast<char*>(malloc(1024));
		while (start < end)
		{
			VirtualQueryEx(GetCurrentProcess(), (void*)start, &mbi, sizeof(mbi));
			if ((mbi.AllocationProtect & 238) && !(mbi.Protect & 257) && (mbi.State & 4096) && (mbi.Protect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)))
			{
				for (char* i = (char*)mbi.BaseAddress; i < (char*)mbi.BaseAddress + mbi.RegionSize;) {
					SIZE_T bytesRead;
					ReadProcessMemory(GetCurrentProcess(), i, buffer, 1024, &bytesRead);
					for (int pad = 0; pad < bytesRead; pad++)
					{
						if (*(uintptr_t*)(buffer + pad) == *(uintptr_t*)pattern) {
							return (uintptr_t)(i + pad);
						}
					}
					i += bytesRead;
				}
			}
			start += mbi.RegionSize;
		}
		return 0;

	}
}



