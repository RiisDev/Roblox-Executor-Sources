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

