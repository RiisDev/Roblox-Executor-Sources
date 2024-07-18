#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>




DWORD Roblox;





DWORD unprotect(DWORD addr)
{
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

bool QuickCompare(const BYTE *pData, const BYTE *bMask, const char *szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask) return 0;
	return (*szMask) == NULL;
}

int QuickFindPattern(int dwAddress, int dwLen, BYTE *bMask, const char *szMask) {
	for (std::size_t i = 0; i < (int)dwLen; i++)
		if (QuickCompare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
	return 0;
}
int Scan(DWORD mode, char* content, char* mask)
{
	//VMProtectBeginUltra("Scan");
	DWORD PageSize;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	PageSize = si.dwPageSize;
	MEMORY_BASIC_INFORMATION mi;
	for (DWORD lpAddr = 0; lpAddr < 0x7FFFFFFF; lpAddr += PageSize)
	{
		DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
		if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
		if (mi.Type == MEM_MAPPED) continue;
		if (mi.Protect == mode)
		{
			int addr = QuickFindPattern(lpAddr, PageSize, (PBYTE)content, mask);
			if (addr != 0)
			{
				return addr;
			}
		}
	}
	//VMProtectEnd();
}


namespace SignatureScanner {

	int Scan(const char* arrayofbytes, const char* mask) {
		for (int i = (int)GetModuleHandle(NULL); i <= 0xF000000; ++i) {
			if (QuickCompare((BYTE*)i, (BYTE*)arrayofbytes, mask))
				return i;
		}
		return NULL;
	};

}





#define R_LUA_REGISTRYINDEX 0xFFFFD8F0
#define ASLR(OFFSET) (OFFSET - 0x400000 + (DWORD)GetModuleHandle(NULL))

long base(long a) {
	return (a - 0x400000 + (DWORD)GetModuleHandle("RobloxPlayerBeta.exe"));
}

typedef int(__stdcall* rgetfield)(DWORD rL, int idx, const char *k);
rgetfield r_lua_getfield = (rgetfield)unprotect(SignatureScanner::Scan("\x55\x8B\xEC\x8B\x4D\x0C\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\x85\xC9\x7E\x18\x8B\x46\x1C\xBF\x00\x00\x00\x00\xC1\xE1\x04\x83\xC0\xF0\x03\xC1\x3B\x46\x0C\x0F\x42\xF8\xEB\x09\x51\x56\xE8\x00\x00\x00\x00\x8B\xF8\x8B\x55\x10\x8B\xCA\x8D\x59\x01\x0F\x1F\x40\x00\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x8B\xCE\xE8\x00\x00\x00\x00\x6A\x01", "xxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxx????xx"));



typedef char*(__cdecl* rtolstring)(DWORD rL, int idx, size_t *size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(SignatureScanner::Scan("\x55\x8B\xEC\x56\x8B\x75\x08\x57\x8B\x7D\x0C\x85\xFF\x7E\x1A", "xxxxxxxxxxxxxxx")));


typedef int(__cdecl* pcall)(DWORD rL, DWORD, DWORD, DWORD);
pcall r_lua_pcall = (pcall)unprotect(SignatureScanner::Scan("\x55\x8B\xEC\x8B\x4D\x14\x83\xEC\x08\x57", "xxxxxxxxxx"));

typedef void(__stdcall* a_pushstring)(DWORD ls, const char* k);
a_pushstring r_lua_pushstring = (a_pushstring)unprotect(SignatureScanner::Scan("\x55\x8B\xEC\x8B\x55\x0C\x85\xD2\x75\x0F", "xxxxxxxxxx"));


void r_lua_getglobal(int L, const char* k)
{
	r_lua_getfield(L, -10002, k);
}
