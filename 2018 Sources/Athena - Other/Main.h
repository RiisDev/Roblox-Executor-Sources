#pragma once
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#include <sstream>
#include <vector>
#include <iterator>
#include <fcntl.h>
#include <io.h>
#include <TlHelp32.h>
#include <fstream>
#include <wininet.h>
#include <Iphlpapi.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <urlmon.h>
#include <DbgHelp.h>
#include <Iphlpapi.h>
#include <wininet.h>
#include <stdlib.h> 
#include <conio.h>
#include <Psapi.h>
#include <windef.h>
#include <limits>
#include <iomanip>
#include <locale>
#include <utility>
#include <codecvt>
#include <TlHelp32.h>
#include <functional>
#include <typeinfo>
#include <typeinfo.h>
#include <iphlpapi.h>
#include <Assert.h>
#include <cmath>
#include <math.h>
#include <winsock.h>
#include <random>
#include <Richedit.h>
#include <CommCtrl.h>
#include <lmcons.h>


#pragma comment(lib, "lua5.1.lib")

#define aslr(x) (x - 0x400000 + (DWORD)GetModuleHandle(NULL))

#extern "C" {
  #include "Lua/lua.hpp"
}

int ScriptContext;
int L;

int Scan(DWORD mode, char* content, char* mask)
{
	DWORD PageSize;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	PageSize = si.dwPageSize;
	MEMORY_BASIC_INFORMATION mi;
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
				return addr;
			}
		}
	}
}

std::vector<std::string> SplitString(const char *str, char c)
{
	std::vector<std::string> result;
	do {
		const char *begin = str;
		while (*str != c && *str) {
			str++;
		}
		result.push_back(std::string(begin, str));
	} while (0 != *str++);
	return result;
}

std::string getURL(const char* URL) {
	DeleteUrlCacheEntryA(URL);
	HINTERNET interwebs = InternetOpenA("lol", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL, NULL, NULL, NULL, NULL);
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
			return rtn;
		}
	}
	InternetCloseHandle(interwebs);
	return rtn;
}

// lol lua for encryption i am smart
string encode(string toencode, int keychar) {
	string encodesource = getURL("http://tacticalbfg.com/Athena/encryption.lua");
	lua_State* luaS = luaL_newstate();
	lua_State* LuaS = lua_newthread(luaS);
	luaL_openlibs(LuaS);
	luaL_dostring(LuaS, encodesource.c_str());
	lua_getglobal(LuaS, "Encode");
	lua_pushstring(LuaS, toencode.c_str());
	lua_pushnumber(LuaS, keychar);
	lua_pcall(LuaS, 2, 1, 0);
	string RealSource = lua_tostring(LuaS, -1);

	lua_settop(LuaS, 0);
	lua_close(LuaS);

	return RealSource;
}

string decode(string todecode, int keychar) {
	string encodesource = getURL("http://tacticalbfg.com/Athena/encryption.lua");
	lua_State* luaS = luaL_newstate();
	lua_State* LuaS = lua_newthread(luaS);
	luaL_openlibs(LuaS);
	luaL_dostring(LuaS, encodesource.c_str());
	lua_getglobal(LuaS, "Decode");
	lua_pushstring(LuaS, todecode.c_str());
	lua_pushnumber(LuaS, keychar);
	lua_pcall(LuaS, 2, 1, 0);
	string RealSource = lua_tostring(LuaS, -1);

	lua_settop(LuaS, 0);
	lua_close(LuaS);

	return RealSource;
}
