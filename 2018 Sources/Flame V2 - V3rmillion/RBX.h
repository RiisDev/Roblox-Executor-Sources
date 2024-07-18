#pragma once

/* Includes */

#include <Windows.h>
#include <Psapi.h>
#include "VMProtectSDK.h"

/* Defines */

#define MESSAGE_ERROR 3
#define MESSAGE_WARN 2
#define MESSAGE_INFO 1
#define MESSAGE_OUTPUT 0

#define LUA_TNIL 0
#define LUA_TLIGHTUSERDATA 1
#define LUA_TNUMBER 2
#define LUA_TBOOLEAN 3
#define LUA_TSTRING 4
#define LUA_TTHREAD 5
#define LUA_TFUNCTION 6
#define LUA_TTABLE 7
#define LUA_TUSERDATA 8

MODULEINFO GetModuleInfo(char *szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0) return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

int FindPattern(char *module, char *arrayofbytes, char *szMask) {
	MODULEINFO mInfo = GetModuleInfo(module);
	int base = (int)mInfo.lpBaseOfDll;
	int size = (int)mInfo.SizeOfImage;
	int patternLength = (int)strlen(szMask);

	for (int i = 0; i < size - patternLength; i++) {
		bool sigFound = true;
		for (int scan = 0; scan < patternLength; scan++) {
			sigFound &= szMask[scan] == '?' || arrayofbytes[scan] == *(char*)(base + i + scan);
		}
		if (sigFound) {
			return base + i;
		}
	}
	return NULL;
}

namespace EventLog {
	void Log(int logType, const char* logText) {
		VMProtectBeginUltra("Singleton Event Log");
		typedef int(__cdecl *EVENTLOG)(int a1, int a2, int a3, char a4);
		static EVENTLOG LogEvent = (EVENTLOG)(SignatureScanner::Scan("\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83\xEC\x30\x8D\x45\x14", "xxxxxx????xx????xxxx????xxxxxx"));

		typedef int(__cdecl *VARIABLE)();
		static VARIABLE CreateLogVariable = (VARIABLE)(SignatureScanner::Scan("\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x51\x64\xA1\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\x0C\x88\xA1\x00\x00\x00\x00\x3B\x81\x00\x00\x00\x00\x7E\x4F", "xxxxxx????xx????xxxx????xxx????xx????xxxx????xx????xx")); //FindPattern("RobloxPlayerBeta.exe", "\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x51\x64\xA1\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\x0C\x88\xA1\x00\x00\x00\x00\x3B\x81\x00\x00\x00\x00\x7E\x4F", "xxxxxx????xx????xxxx????xxx????xx????xxxx????xx????xx"));
		
		int Variable = CreateLogVariable();

		try {
			LogEvent(Variable, logType, (int)"%s", (int)logText);
		}
		catch (...) {};
		VMProtectEnd();
	}
}
