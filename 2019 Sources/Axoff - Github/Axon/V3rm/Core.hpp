#include "LuaWrapepr.hpp"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <fstream>
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")


void WrapRBX(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	wrap(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
}

namespace Scanner {
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

	DWORD Scan()
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
						DWORD Bruh = x(Address_ScriptContext);
						if (Compare((const char*)i, (char*)& Bruh, "xxxx"))
							return i;
					}
				}
				Start += MBI.RegionSize;
			}
		} while (Start < End);
		return 0;
	}
}

BOOL WINAPI CreateConsole(std::string name)
{
	DWORD nOldProtect;
	if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(FreeConsole) = 0xC3;
	if (!VirtualProtect(FreeConsole, 1, nOldProtect, &nOldProtect))
		return FALSE;
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	SetWindowPos(ConsoleHandle, HWND_TOPMOST, 50, 20, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(ConsoleHandle, 1);
	SetConsoleTitle(name.c_str());

}

std::string ReplaceAll(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string DownloadURL(std::string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = ReplaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = ReplaceAll(rtn, "|n", "\r\n");
	return p;
}

/*Execute*/
void Execute(std::string Script) {
	Script = "spawn(function() script = Instance.new('LocalScript')\r\n" + Script + "\r\nend)";
	if (Script.find(".Name = \"Dex\"") == std::string::npos) {
		Script = ReplaceAll(Script, "game:GetObjects", "GetObjects");
		Script = ReplaceAll(Script, "game:HttpGet", "HttpGet");
	}
	if (luaL_loadstring(LS, Script.c_str())) {
		return;
	}
	else {
		lua_pcall(LS, 0, 0, 0);
	}
}