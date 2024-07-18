#include <memory.h>
#include <cstdint>
#include "retcheck.h"
#include <stdio.h>    
#include <psapi.h>
#pragma comment( lib, "psapi.lib" )

/*
Eternals retcheck
Eternals retcheck is fine but it skips some parts...
*/
namespace Retcheck
{
	DWORD Unprotect(DWORD addr, bool mode)
	{
		if (mode)
			return Ret::unprotect<DWORD>((BYTE*)addr);

		BYTE* tAddr = (BYTE*)addr;
		do
		{
			tAddr += 16;
		} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

		DWORD funcSz = tAddr - (BYTE*)addr;

		PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (nFunc == NULL)
			return addr;

		memcpy(nFunc, (void*)addr, funcSz);

		BYTE* pos = (BYTE*)nFunc;
		BOOL valid = false;
		do
		{
			if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
				*(BYTE*)pos = 0xEB;

				DWORD cByte = (DWORD)nFunc;
				do
				{
					if (*(BYTE*)cByte == 0xE8)
					{
						DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
						DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

						if (oFuncAddr % 16 == 0)
						{
							DWORD relativeAddr = oFuncAddr - cByte - 5;
							*(DWORD*)(cByte + 1) = relativeAddr;

							cByte += 4;
						}
					}

					cByte += 1;
				} while (cByte - (DWORD)nFunc < funcSz);

				valid = true;
			}
			pos += 1;
		} while ((DWORD)pos < (DWORD)nFunc + funcSz);

		if (!valid)
		{
			VirtualFree(nFunc, funcSz, MEM_RELEASE);
			return addr;
		}

		return (DWORD)nFunc;
	}
}

/*
Sigscan by Austin
Sigscanner made by austin
making our own soon
*/
namespace sigscan
{
	bool compare(const char* location, const char* aob, const char* mask)
	{
		for (; *mask; ++aob, ++mask, ++location)
		{
			if (*mask == 'x' && *location != *aob)
			{
				return false;
			}
		}

		return true;
	}

	bool compare_reverse(const char* location, const char* aob, const char* mask)
	{
		const char* mask_iter = mask + strlen(mask) - 1;
		for (; mask_iter >= mask; --aob, --mask_iter, --location)
		{
			if (*mask_iter == 'x' && *location != *aob)
			{
				return false;
			}
		}

		return true;
	}

	byte* scan(const char* aob, const char* mask, uintptr_t start, uintptr_t end)
	{
		if (start <= end)
		{
			for (; start <= end; ++start)
			{
				if (compare((char*)start, (char*)aob, mask))
				{
					return (byte*)start;
				}
			}
		}
		else
		{
			for (; start >= end; --start)
			{
				if (compare_reverse((char*)start, (char*)aob, mask))
				{
					return (byte*)start - strlen(mask) - 1;
				}
			}
		}

		return 0;
	};

	byte* scan(const char* module, const char* aob, const char* mask)
	{
		MODULEINFO info;
		if (GetModuleInformation(GetCurrentProcess(), GetModuleHandle(module), &info, sizeof(info)))
			return scan(aob, mask, (uintptr_t)info.lpBaseOfDll, (uintptr_t)info.lpBaseOfDll + info.SizeOfImage);

		return 0;
	}
}

namespace xGladiusScanner {
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
		for (; *mask; ++aob, ++mask, ++location) {
			__try {
				if (*mask == 'x' && *location != *aob)
					return 0;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
	{
		for (int i = 0; i<(int)dwLen; i++)
			if (compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}

	int Scan(DWORD mode, char* content, char* mask)
	{
		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		for (DWORD lpAddr = (DWORD)GetModuleHandle(NULL); lpAddr<0xF000000; lpAddr += PageSize)
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
	}
}

namespace Rakion99Scanner {
	bool Compare(const char* pData, const char* bMask, const char* szMask)
	{
		while (*szMask) {
			__try {
				if (*szMask != '?') {
					if (*pData != *bMask) return 0;
				}
				++szMask, ++pData, ++bMask;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	}

	DWORD Scan(const char* vftableaddress)
	{
		MEMORY_BASIC_INFORMATION MBI = { 0 };
		SYSTEM_INFO SI = { 0 };
		GetSystemInfo(&SI);
		DWORD Start = (DWORD)SI.lpMinimumApplicationAddress;
		DWORD End = (DWORD)SI.lpMaximumApplicationAddress;
		do
		{
			while (VirtualQuery((void*)Start, &MBI, sizeof(MBI))) {
				if ((MBI.Protect & PAGE_READWRITE) && !(MBI.Protect & PAGE_GUARD) && !(MBI.Protect & 0x90))
				{
					for (DWORD i = (DWORD)(MBI.BaseAddress); i - (DWORD)(MBI.BaseAddress) < MBI.RegionSize; ++i)
					{
						if (Compare((const char*)i, vftableaddress, "xxxx"))
							return i;
					}
				}
				Start += MBI.RegionSize;
			}
		} while (Start < End);
		return 0;
	}
}