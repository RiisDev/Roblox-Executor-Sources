#pragma once

#include <Windows.h>

namespace Rosin {
	namespace Scanning {

		void PlaceJMP(BYTE *Address, DWORD jumpTo, DWORD length)
		{
			DWORD dwOldProtect, dwBkup, dwRelAddr;

			VirtualProtect(Address, length, PAGE_EXECUTE_READWRITE, &dwOldProtect);

			dwRelAddr = (DWORD)(jumpTo - (DWORD)Address) - 5;

			*Address = 0xE9;

			*((DWORD*)(Address + 0x1)) = dwRelAddr;

			for (DWORD x = 0x5; x < length; x++)
				*(Address + x) = 0x90;

			VirtualProtect(Address, length, dwOldProtect, &dwBkup);
		}

		void CloseOtherDLLThreads() {
			HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
			if (h != INVALID_HANDLE_VALUE) {
				THREADENTRY32 te;
				te.dwSize = sizeof(te);
				if (Thread32First(h, &te)) {
					do {
						if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID)) {
							if (te.th32ThreadID != GetCurrentThreadId() && te.th32OwnerProcessID == GetCurrentProcessId()) {
								HANDLE thread = ::OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
								if (thread != NULL) {
									TerminateThread(thread, 0);
									CloseHandle(thread);
								}
							}
						}
						te.dwSize = sizeof(te);
					} while (Thread32Next(h, &te));
				}
				CloseHandle(h);
			}
		}

		DWORD PlaceJMPk(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen) //places a jmp at pAddress to dwJumpTo, returns jumpback address
		{
			DWORD dwOldProtect, dwBkup, dwRelAddr;

			VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

			dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;

			*pAddress = 0xE9;

			*((DWORD *)(pAddress + 0x1)) = dwRelAddr;

			for (DWORD x = 0x5; x < dwLen; x++)
				*(pAddress + x) = 0x90;

			VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);
			return ((DWORD)pAddress + dwLen);
		}

		bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
		{
			for (; *szMask; ++szMask, ++pData, ++bMask)
				if (*szMask == 'x' && *pData != *bMask) return 0;
			return (*szMask) == NULL;
		}

		DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
		{
			for (int i = 0; i<(int)dwLen; i++)
				if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
			return 0;
		}

		int Scan(DWORD mode, char* content, char* mask)
		{
			HANDLE CurrentThread = GetCurrentThread();
			int StartPriority = GetThreadPriority(CurrentThread);
			SetThreadPriority(CurrentThread, THREAD_PRIORITY_TIME_CRITICAL);

			DWORD PageSize;
			SYSTEM_INFO si;
			GetSystemInfo(&si);
			PageSize = si.dwPageSize;
			MEMORY_BASIC_INFORMATION mi;
			//Modules::GetBaseInst()->SetThreadActive(false);
			for (DWORD lpAddr = 0; lpAddr<0x7FFFFFFF; lpAddr += PageSize)
			{
				DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
				if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
				if (mi.Type == MEM_MAPPED) continue;
				if (mi.Protect == mode)
				{
					int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
					if (addr != 0)
					{
						SetThreadPriority(CurrentThread, StartPriority);
						//Modules::GetBaseInst()->SetThreadActive(true);
						return addr;
					}
				}
			}
			SetThreadPriority(CurrentThread, StartPriority);
			//Modules::GetBaseInst()->SetThreadActive(true);
			return 0;
		}
	}
}