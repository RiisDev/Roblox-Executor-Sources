#pragma once
#include <Windows.h>
#include <String>
#include <ios>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <functional>
#include <WinInet.h>
#include <string>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
#include <limits>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdexcept>

#include "RetCheck.h"

namespace Memory {
	///////////////
	// ScriptContext Scanner, Pretty Damn Fast
	// Credits To Whoever Made It (I Believe It Was Louka Or Rakion99)
	///////////////

	bool compare(BYTE* address, BYTE* pattern, BYTE* mask) {
		for (; *mask; address++, pattern++, mask++) {
			if (*mask == 'x' && *address != *pattern) {
				return false;
			}
		}
		return true;
	}

	DWORD Scan(BYTE* aob, BYTE* mask, BYTE prot = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
		MEMORY_BASIC_INFORMATION mbi;
		DWORD j = (DWORD)GetModuleHandle(NULL);
		while (j < 0x7FFFFFFF && VirtualQuery((void*)j, &mbi, sizeof(mbi))) {
			if (!(mbi.Protect & PAGE_GUARD) && (mbi.State & MEM_COMMIT) && (mbi.Protect & prot)) {
				for (DWORD k = (DWORD)mbi.BaseAddress; k < ((DWORD)mbi.BaseAddress + mbi.RegionSize); ++k) {
					if (compare((BYTE*)k, (BYTE*)aob, (BYTE*)mask)) {
						return k;
					}
				}
			}
			j += mbi.RegionSize;
		}
		return 0;
	}
	///////////////
}


int Players = 0;

namespace MemoryFunctions {
	///////////////
	//useless memory functions
	///////////////

	int GetParent(int Instance) {
		try {
			return *(int*)(Instance + 0x34);
		}
		catch (...) {
		}
	}

	const char* GetClass(int self)
	{
		try {
			return (const char*)(*(int(**)(void))(*(int*)self + 16))();
		}
		catch (...) {

		}
	}


	int FindFirstClass(int Instance, const char* Name)
	{
		try {
			DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
			DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

			for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
			{
				if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
				{
					return *(int*)i;
				}
			}
		}
		catch (...) {

		}
	}


	std::string* GetName(int Instance) {
		try {
			return (std::string*)(*(int*)(Instance + 40));
		}
		catch (...) {

		}
	}



	int FindFirstChild_PartOf(int Instance, std::string Name) {
		try {
			DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
			DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
			for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
			{
				if (strstr(GetName(*(int*)i)->c_str(), Name.c_str()))
				{
					if (*(int*)i == 0)
					{

					}
					else {
						return *(int*)i;
					}
				}

			}
		}
		catch (...) {

		}

	}



	int GetLocalPlayer(int Player) {
		try {
			return *(int*)(Player + 196);
		}
		catch (...) {
		}

	}

	std::string to_lowercase(std::string s) {
		std::transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(std::tolower));
		return s;
	}

	int FindFirstPlayer(int Instance, std::string Name) {
		if (Instance > 10000) {
			DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
			if (Name == "me") {
				return GetLocalPlayer(Players);
			}
			else {
				if (StartOfChildren != 0) {
					DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
					if (EndOfChildren != 0) {
						for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
							try {
								if (strstr(to_lowercase(*GetName(*(int*)i)).c_str(), to_lowercase(Name).c_str())) {
									return *(int*)i;
								}
							}
							catch (std::exception) {
								Sleep(1);
							}
							catch (...) {
								Sleep(1);
							}
						}
					}
				}
			}
		}
		return 0;
	}

	DWORD GetCharacterFromPlayer(DWORD player) {
		if (player > 1000) {
			__try {
				return *(DWORD *)(player + 88);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 0;
	};


	int GetCharacter(std::string Name) {
		if (Name != "") {
			if (Name == "me") {
				int Player = GetLocalPlayer(Players);
				if (Player != 0) {
					return GetCharacterFromPlayer(Player);
				}
				else {
					return 0;
				}
			}
			int Player = FindFirstPlayer(Players, Name);
			if (Player != 0) {
				return GetCharacterFromPlayer(Player);
			}
		}
		return 0;
	}

	int GetId(int Player) {
		return *(int*)(Player + 168);
	}

	int GetPlayer(std::string Name) {
		int Player = FindFirstChild_PartOf(Players, Name);
		if (Player != 0) {
			return Player;
		}
		else
		{
			return 0;
		}
	}
}

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



///////
//ETERNALS V3 RETECHECK
///////

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
