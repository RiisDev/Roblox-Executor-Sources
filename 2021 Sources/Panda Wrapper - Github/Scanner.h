#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <string>
#include <tlhelp32.h>
#include <iostream> 
#include <windows.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <ctime>
#include "Rlua.h"
#include "Minhook/MinHook.h"

using namespace std;
#define ScanAddress(addr)(addr - 0x400000 + (int)GetModuleHandle(0))

namespace Scanner {
	namespace ClipBoard {
		void Set(const char* output) {
			const size_t len = strlen(output) + 1;
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
			memcpy(GlobalLock(hMem), output, len);
			GlobalUnlock(hMem);
			OpenClipboard(0);
			EmptyClipboard();
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
		}

		string Get() {
			HANDLE clip = 0;
			if (OpenClipboard(NULL)) {
				clip = GetClipboardData(CF_TEXT);
				CloseClipboard();
			}
			string text = (char*)clip;
			return text;
		}
	}

	namespace RandomString {
		std::string GenerateString(size_t len)
		{
			srand(time(0));
			std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
			int pos;
			while (str.size() != len) {
				pos = ((rand() % (str.size() - 1)));
				str.erase(pos, 1);
			}
			return str;
		}
	}

	namespace Process {
		DWORD GetPID(char* processName)
		{
			char* p = strrchr(processName, '\\');
			if (p)
				processName = p + 1;

			PROCESSENTRY32 processInfo;
			processInfo.dwSize = sizeof(processInfo);

			HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
			if (processesSnapshot == INVALID_HANDLE_VALUE)
				return 0;

			Process32First(processesSnapshot, &processInfo);
			if (!strcmp(processName, processInfo.szExeFile))
			{
				CloseHandle(processesSnapshot);
				return processInfo.th32ProcessID;
			}

			while (Process32Next(processesSnapshot, &processInfo))
			{
				if (!strcmp(processName, processInfo.szExeFile))
				{
					CloseHandle(processesSnapshot);
					return processInfo.th32ProcessID;
				}
			}

			CloseHandle(processesSnapshot);
			return 0;
		}

		void Kill(const char *filename)
		{
			HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
			PROCESSENTRY32 pEntry;
			pEntry.dwSize = sizeof(pEntry);
			BOOL hRes = Process32First(hSnapShot, &pEntry);
			while (hRes)
			{
				if (strcmp(pEntry.szExeFile, filename) == 0)
				{
					HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
						(DWORD)pEntry.th32ProcessID);
					if (hProcess != NULL)
					{
						TerminateProcess(hProcess, 9);
						CloseHandle(hProcess);
					}
				}
				hRes = Process32Next(hSnapShot, &pEntry);
			}
			CloseHandle(hSnapShot);
		}
	}

	namespace AOB {
		bool Check(const BYTE* pd, const BYTE* aob, const char* mask)
		{
			for (; *mask; ++mask, ++pd, ++aob)
				if (*mask != '?' && *pd != *aob)
					return false;

			return (*mask) == NULL;
		}

		DWORD FindPattern(const char* aob, const char* mask)
		{
			for (DWORD ind = (DWORD)GetModuleHandle(0); ind <= 0xFFFFFFF; ++ind) {
				if (Check((BYTE*)ind, (BYTE*)aob, mask))
					return ind;
			}
			return 0x00000000;
		}
	}

	namespace Memory {

		bool Compare(const char* pData, const char* bMask, const char* szMask)
		{
			while (*szMask) {
				if (*szMask != '?') {
					if (*pData != *bMask) return 0;
				}
				++szMask, ++pData, ++bMask;
			}
			return 1;
		}

		DWORD Scan(const char* vftable)  // Credits goes to tepig
		{
			MEMORY_BASIC_INFORMATION BasicMemoryInformation = {};
			SYSTEM_INFO SystemInformation = {};
			GetSystemInfo(&SystemInformation);
			DWORD StartingMemorySearchPosition = (DWORD)SystemInformation.lpMinimumApplicationAddress;
			DWORD MaximumSearchBoundary = (DWORD)SystemInformation.lpMaximumApplicationAddress;
			do {
				while (VirtualQuery((void*)StartingMemorySearchPosition, &BasicMemoryInformation, sizeof(BasicMemoryInformation))) {
					if ((BasicMemoryInformation.Protect & PAGE_READWRITE) && !(BasicMemoryInformation.Protect & PAGE_GUARD)) {
						for (DWORD Key = (DWORD)(BasicMemoryInformation.BaseAddress); ((Key - (DWORD)(BasicMemoryInformation.BaseAddress) < BasicMemoryInformation.RegionSize)); ++Key) {
							if (Compare((const char*)Key, vftable, "xxxx")) {
								return Key;
							};
						};
					};
					StartingMemorySearchPosition += BasicMemoryInformation.RegionSize;
				};
			} while (StartingMemorySearchPosition < MaximumSearchBoundary);
			return 0x0;
		}
	}

	namespace Console {
		void CreateConsole(const char *Name) {
			DWORD consoleOldProtect = 0;
			VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &consoleOldProtect);
			*(UINT*)FreeConsole = 0xC2;
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
			freopen("CONIN$", "r", stdin);
			DWORD cons = (WS_CAPTION | DS_MODALFRAME | WS_MINIMIZEBOX | WS_SYSMENU);
			SetWindowLong(GetConsoleWindow(), GWL_STYLE, cons);
			SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
			SetConsoleTitleA(Name);
		}

		void CloseConsole() {
			ShowWindow(::GetConsoleWindow(), SW_HIDE);
		}
	}

	namespace ReturnCheck {
		DWORD RemoveProtection(DWORD Address) { // Credits goes to Eternal
			BYTE* tAddr = (BYTE*)Address;
			do
			{
				tAddr += 16;
			} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

			DWORD funcSz = tAddr - (BYTE*)Address;

			PVOID nFunc = VirtualAlloc(NULL, funcSz, 0x00001000 | 0x00002000, 0x40);
			if (nFunc == NULL)
				return Address;

			memcpy(nFunc, (void*)Address, funcSz);

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
							DWORD oFuncPos = Address + (cByte - (DWORD)nFunc);
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
				VirtualFree(nFunc, funcSz, 0x00008000);
				return Address;
			}

			return (DWORD)nFunc;
		}
	}

	namespace Hook {
		void* placeHook(DWORD address, void* hook, bool revert = false) {
			DWORD oldprot;
			if (!revert) {
				void* oldmem = new void*;
				void* result = new void*;
				memcpy(oldmem, (void*)address, sizeof(void*) * 4);
				VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
				*(char*)address = 0xE9; *(DWORD*)(address + 1) = (DWORD)hook - address - 5;
				memcpy(result, oldmem, sizeof(void*) * 4);
				VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
				return result;
			}
			else {
				VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
				memcpy((void*)address, hook, sizeof(void*) * 4);
				VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
				return NULL;
			}
		}
	}
}