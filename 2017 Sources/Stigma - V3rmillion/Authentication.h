#pragma once
//#include "VMProtectSDK.h"
#include <winInet.h>
#include "Update.h"

#pragma comment(lib, "wininet.lib")

using namespace std;

void setJb(DWORD addr) {
	DWORD oldProtection;
	VirtualProtect((LPVOID)addr, 0x05, PAGE_EXECUTE_READWRITE, &oldProtection);
	*(char*)addr = 0xEB;
	VirtualProtect((LPVOID)addr, 0x05, oldProtection, &oldProtection);
}
void restoreJb(DWORD addr) {
	DWORD oldProtection;
	VirtualProtect((void*)addr, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
	*(char*)addr = 0x72;
	VirtualProtect((void*)addr, 5, oldProtection, &oldProtection);
}

namespace Memory {
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
}


namespace lua {
	typedef int(__thiscall *GlobalState)(DWORD ScriptContext, int idx);
	GlobalState rbGetGlobalState = (GlobalState)aobscan::scan("\x55\x8B\xEC\x56\x57\x6A\x05\xFF\x75\x08\x8B\xF9\x33\xF6", "xxxxxxxxxxxxxx");

	typedef void*(__cdecl *Lua_getfield)(int lua_State, int idx, const char *k);
	Lua_getfield rblua_getfield = (Lua_getfield)aobscan::scan("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C", "xxxxxxxxxxxxxxx");

	typedef int(__cdecl *pCall)(int lua_State, int naArgs, int nresults, int errfunc);
	pCall rblua_pcall = (pCall)aobscan::scan("\x55\x8B\xEC\x8B\x45\x14\x83\xEC\x08\x53\x56\x57\x8B\x7D\x08\x85\xC0\x75", "xxxxxxxxxxxxxxxxxx");

	typedef void*(__cdecl *Lua_setfield)(int lua_State, int idx, const char *k);
	Lua_setfield rblua_setfield = (Lua_setfield)aobscan::scan("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x8B\xCA\x83\xC4\x08\x8B\xF8\x8D\x59\x01\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\x89\x45\xF0\x8B\x46\x10\x83\xE8\x10\x50", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxx");

	typedef void*(__cdecl *Lua_pushvalue)(int lua_State, int idx);
	Lua_pushvalue rblua_pushvalue = (Lua_pushvalue)aobscan::scan("\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\x75\x08\x56\xE8\x00\x00\x00\x00\x8B\x08\x8B\x56\x10", "xxxxxxxxxxxx????xxxxx");

	typedef void(__cdecl *Lua_pushnumber)(int lua_State, double number);
	Lua_pushnumber rblua_pushnumber = (Lua_pushnumber)aobscan::scan("\x55\x8B\xEC\x8B\x55\x08\x66\x0F\x00\x00\x00\x00\x00\x00\xF2\x0F\x10\x45\x0C", "xxxxxxxx??????xxxxx");

	typedef int(__cdecl *Lua_newthread)(int lua_State);
	Lua_newthread rblua_newthread = (Lua_newthread)aobscan::scan("\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x51\x56\x8B\x75\x08\x57\x8B\x4E", "xxxxxx????xx????xxxx????xxxxxxxx");

	typedef const char *(__cdecl *lua_ToLString)(int lua_State, int idx, size_t *len);
	lua_ToLString rblua_tolstring = (lua_ToLString)aobscan::scan("\x55\x8B\xEC\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\xF8\x83\xC4\x08\x83\x7F\x08\x04\x0F", "xxxxxxxxxxxxx????xxxxxxxxxx");

	typedef void*(__cdecl *Lua_settop)(int lua_State, int n);
	Lua_settop rblua_settop = (Lua_settop)aobscan::scan("\x55\x8B\xEC\x8B\x4D\x0C\x8B\x55\x08\x85\xC9\x78\x00\x8B\x42\x1C", "xxxxxxxxxxxx?xxx");

	typedef bool(__cdecl *Lua_pushboolean)(int lua_State, int a1);
	Lua_pushboolean rblua_pushboolean = (Lua_pushboolean)aobscan::scan("\x55\x8B\xEC\x8B\x55\x08\x33\xC0\x39\x45\x0C\x8B\x4A\x10\x0F\x95\xC0", "xxxxxxxxxxxxxxxxx");

	typedef int(__cdecl *rblua_pushcclosuret)(int lua_State, lua_CFunction fn, int a3);
	rblua_pushcclosuret rblua_pushcclosure = (rblua_pushcclosuret)aobscan::scan("\x55\x8B\xEC\x56\x8B\x75\x08\x8B\x4E\x08\x8B\x44\x31\x60\x3B\x44", "xxxxxxxxxxxxxxxx");

	typedef int(__cdecl *rblua_pushcclosurets)(int lua_State, void* fn, int a3);
	rblua_pushcclosurets rlua_pushcclosure = (rblua_pushcclosurets)aobscan::scan("\x55\x8B\xEC\x56\x8B\x75\x08\x8B\x4E\x08\x8B\x44\x31\x60\x3B\x44", "xxxxxxxxxxxxxxxx");

	typedef void*(__cdecl *Lua_pushnil)(int lua_State);
	Lua_pushnil rblua_pushnil = (Lua_pushnil)aobscan::scan("\x55\x8B\xEC\x8B\x55\x08\x8B\x42\x10\xC7\x40\x08\x00\x00\x00", "xxxxxxxxxxxx???");



	typedef int(__thiscall *ns)(int a1, int a2);
	ns openstate = (ns)aobscan::scan("\x55\x8B\xEC\x56\x57\x6A\x05\xFF\x75\x08\x8B\xF9\x33\xF6", "xxxxxxxxxxxxxx");
	DWORD ScriptContextVFTable = ScriptContextAddress;
	int ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVFTable, "xxxx");

	int GetLuaState() {
		int LuaState = *((DWORD*)ScriptContext + 41) - (DWORD)((char*)ScriptContext + 164);
		return LuaState;
	}


	int lua_State = lua::GetLuaState();

	/*__asm {
	push eax
	push ecx
	push 2
	mov ecx, SC
	call ggs
	mov lua_State, eax
	pop ecx
	pop eax
	}*/

	/*int rGetLuaState() {
	int x = *((DWORD *)ScriptContext + 14 * 0 + 41) - (DWORD)((char *)ScriptContext + 56 * 0 + 164);
	return x;
	}*/

	//int lua_State = rGetLuaState();

	/*int getState() {
	int LuaState = *((DWORD*)ScriptContext + 41) - (DWORD)((char*)ScriptContext + 164);
	return LuaState;
	}

	int lua_State = getState();*/
}

void rblua_getfield(int ls, int idx, const char *k) {
	setJb((int)lua::rblua_getfield + 0x62);
	lua::rblua_getfield(ls, idx, k);
	restoreJb((int)lua::rblua_getfield + 0x62);
}
void rblua_pcall(int ls, int nar, int nre, int nfunc) {
	setJb((int)lua::rblua_pcall + 0x77);
	lua::rblua_pcall(ls, nar, nre, nfunc);
	restoreJb((int)lua::rblua_pcall + 0x77);
}
void rblua_setfield(int ls, int idx, const char *k) {
	setJb((int)lua::rblua_setfield + 0x66);
	void* sf = lua::rblua_setfield(ls, idx, k);
	restoreJb((int)lua::rblua_setfield + 0x66);
}
void rblua_pushvalue(int ls, int v) {
	setJb((int)lua::rblua_pushvalue + 0x3A);
	lua::rblua_pushvalue(ls, v);
	restoreJb((int)lua::rblua_pushvalue + 0x3A);
}

void rblua_pushnumber(int a, double ac) {
	setJb((int)lua::rblua_pushnumber + 0x39);
	lua::rblua_pushnumber(a, ac);
	restoreJb((int)lua::rblua_pushnumber + 0x39);
}
void rblua_pushboolean(int ls, int a) {
	setJb((int)lua::rblua_pushboolean + 0x2E);
	lua::rblua_pushboolean(ls, a);
	restoreJb((int)lua::rblua_pushboolean + 0x2E);
}

typedef void*(__cdecl *Lua_pushstring)(int lua_State, const char *s);
Lua_pushstring rbdlua_pushstring = (Lua_pushstring)aobscan::scan("\x55\x8B\xEC\x8B\x55\x0C\x85\xD2\x75\x0D\xFF\x75\x08", "xxxxxxxxxxxxx");

typedef void(__cdecl *Lua_pushobject)(int lua_State, DWORD pObj, DWORD z);
Lua_pushobject rlua_pushobject = (Lua_pushobject)adr(0x4937A);

void rblua_getglobal(int luaState, const char* k) {
	return rblua_getfield(luaState, -10002, k);
};

void rblua_pushstring(int ls, const char *s) {
	setJb((int)rbdlua_pushstring + 0x67);
	rbdlua_pushstring(ls, s);
	restoreJb((int)rbdlua_pushstring + 0x67);
}
void rblua_settop(int ls, int n) {
	setJb((int)lua::rblua_settop + 0x5B);
	lua::rblua_settop(ls, n);
	restoreJb((int)lua::rblua_settop + 0x5B);
}



void rblua_pop(int luaState, int n) {

	rblua_settop(luaState, -(n)-1);
};

void rblua_emptystack(int luaState)
{
	rblua_settop(luaState, 0);
};

void pushcclosure(int State, lua_CFunction fn, int n) {
	callcheck();
	setJb((int)lua::rblua_pushcclosure + 0x6B);
	setJb((int)lua::rblua_pushcclosure + 0xE3);
	lua::rblua_pushcclosure(State, fn, n);
	restoreJb((int)lua::rblua_pushcclosure + 0x6B);
	restoreJb((int)lua::rblua_pushcclosure + 0xE3);
}



void rblua_pushcclosure(int State, void* fn, int n) {
	callcheck();
	setJb((int)lua::rlua_pushcclosure + 0x6B);
	setJb((int)lua::rlua_pushcclosure + 0xE3);
	lua::rlua_pushcclosure(State, fn, n);
	restoreJb((int)lua::rlua_pushcclosure + 0x6B);
	restoreJb((int)lua::rlua_pushcclosure + 0xE3);

}

void* rblua_pushnil(int lua_State) {
	setJb((int)lua::rblua_pushnil + 0x24);
	void* pn = lua::rblua_pushnil(lua_State);
	restoreJb((int)lua::rblua_pushnil + 0x24);
	return pn;
}

const char* rblua_tostring(int luaState, int idx) {
	return lua::rblua_tolstring(luaState, idx, NULL);
};

typedef int(__cdecl *Lua_gettop)(int lua_State);
Lua_gettop rblua_gettop = (Lua_gettop)aobscan::scan("\x55\x8B\xEC\x8B\x4D\x08\x8B\x41\x10\x2B\x41\x1C\xC1\xF8\x04", "xxxxxxxxxxxxxxx");

typedef int(__cdecl *Lua_type)(int lua_State, int a1, BOOL IsNil);
Lua_type rblua_type = (Lua_type)aobscan::scan("\x55\x8B\xEC\xFF\x75\x0C\xFF\x75\x08\xE8\x00\x00\x00\x00\x83\xC4\x08\x3D\x00\x00\x00\x00\x75\x00\x83\xC8\xFF", "xxxxxxxxxx????xxxx????x?xxx");

typedef double(__cdecl *Lua_tonumber)(int lua_State, int idx);
Lua_tonumber rlua_tonumber = (Lua_tonumber)aobscan::scan("\x55\x8B\xEC\x8B\x55\x08\x33\xC0\x39\x45\x0C\x8B", "xxxxxxxxxxxx");

typedef void(__cdecl *Lua_getmetatable)(int lua_State, int index);
Lua_getmetatable rlua_getmetatable = (Lua_getmetatable)aobscan::scan("\x55\x8B\xEC\x56\x57\xFF\x75\x0C\x8B\x7D\x08\x57\xE8\x00\x00\x00\x00\x8B\x50\x08\x8B\xCA\x83\xC4\x08\x83\xE9\x07\x74\x19\x49\x74\x0F", "xxxxxxxxxxxxx????xxxxxxxxxxxxxxxx");

typedef int(__cdecl *Lua_toboolean)(int lua_State, int index);
Lua_toboolean rlua_toboolean = (Lua_toboolean)adr(0x400);//aobscan::scan("\xE8\xCD\x96\x00\x00\x83\xC4\x0C\x5B\x5F\x5E\x5D\xC3\xBB\x28\x08", "xxx??xxxxxxxxxxx");

typedef int(__cdecl *Lua_setreadonly)(int lua_State, int index, int idx);
Lua_setreadonly rlua_setreadonly = (Lua_setreadonly)adr(0x4E5F00);

auto Returnchecks()
{
	int rccheck_function = RReturnCheckFunction_address;
	int flag1 = RReturnFlag1_address;
	int flag2 = RReturnFlag2_address;

	int flag1_val = *(int*)(flag1);
	int flag2_val = *(int*)(flag2);
	DWORD nOldProtect;
	BYTE OldJump = *(BYTE*)(rccheck_function);
	VirtualProtect((LPVOID)(rccheck_function), 1, PAGE_EXECUTE_READWRITE, &nOldProtect);
	*(BYTE*)(rccheck_function) = 0xC3;
	return [&]() {
		*(int*)flag1 = flag1_val;
		*(int*)flag2 = flag2_val;
		*(BYTE*)(rccheck_function) = OldJump;
		VirtualProtect((PVOID)(rccheck_function), 1, nOldProtect, &nOldProtect);
	};
}



void *rblua_getmetatable(int lua_State, int index) {
	auto rt_unhook = Returnchecks();
	rlua_getmetatable(lua_State, index);
	rt_unhook();
	return 0;
}

int rblua_toboolean(int lua_State, int index) {
	auto rt_unhook = Returnchecks();
	rlua_toboolean(lua_State, index);
	rt_unhook();
	return 0;
}

void rblua_pushobject(int State, DWORD pObj, DWORD z) {
	auto rt_unhook = Returnchecks();
	rlua_pushobject(State, pObj, z);
	rt_unhook();
}

typedef int(__cdecl *Lua_setmetatable)(int lua_State, int index);
Lua_setmetatable rlua_setmetatable = (Lua_setmetatable)aobscan::scan("\x55\x8B\xEC\x53\x56\x57\xFF\x75\x0C\x8B\x7D\x08\x57\xE8", "xxxxxxxxxxxxxx");

typedef int(__cdecl *Lua_newuserdata)(int lua_State, int index);
Lua_newuserdata rlua_newuserdata = (Lua_newuserdata)aobscan::scan("\x55\x8B\xEC\x56\x8B\x75\x08\x57\x8B\x4E\x08\x8B\x44\x31\x60\x3B\x44\x31\x54\x72\x09\x56\xE8\x00\x00\x00\x00\x83\xC4\x04\x8B\x46\x0C", "xxxxxxxxxxxxxxxxxxxxxxx");

int rblua_setmetatable(int lua_State, int index) {
	auto rt_unhook = Returnchecks();
	int p = rlua_setmetatable(lua_State, index);
	rt_unhook();
	return p;
}

int rblua_newuserdata(int lua_State, int index) {
	auto rt_unhook = Returnchecks();
	int p = rlua_newuserdata(lua_State, index);
	rt_unhook();
	return p;
}

int rblua_setreadonly(int lua_State, int index, int idx) {
	setJb((int)rlua_setreadonly + 0x39);
	int p = rlua_setreadonly(lua_State, index, idx);
	restoreJb((int)rlua_setreadonly + 0x39);
	return p;
}

double rblua_tonumber(int luaState, int idx) {
	auto rt_unhook = Returnchecks();
	int p = rlua_tonumber(luaState, idx);
	rt_unhook();
	return p;
}

inline DWORD rblua_indx2adr(int r_lua_State, int idx)
{
	DWORD result = 0;
	if (idx > 0)
	{
		if ((unsigned int)(16 * idx + *(DWORD*)(r_lua_State + 28) - 16) < *(DWORD*)(r_lua_State + 16))
			result = 16 * idx + *(DWORD *)(r_lua_State + 28) - 16;
	}
	else if (idx > LUA_REGISTRYINDEX)
		result = *(DWORD*)(r_lua_State + 16) + 16 * idx;
	return result;
}

namespace Auth {
	char file[999998];
	unsigned long readIFILE;

	char* GetResponse(string Link)
	{
		HINTERNET hOpen, hURL;
		hOpen = InternetOpen(TEXT("InetURL/1.0"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

		if (!hOpen) TerminateProcess(GetCurrentProcess(), NULL);

		hURL = InternetOpenUrl(hOpen, Link.c_str(), NULL, 0, 0, 0);
		if (!hURL)
		{
			TerminateProcess(GetCurrentProcess(), NULL);
		}

		if (!InternetReadFile(hURL, file, 999997, &readIFILE))
		{
			TerminateProcess(GetCurrentProcess(), NULL);
		}
		return file;
	}

	std::string replaceAll(std::string subject, const std::string& search,
		const std::string& replace) {
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}

	char convertNum(char num) {
		if (num == '1') return '5';
		if (num == '2') return '4';
		if (num == '3') return '3';
		if (num == '4') return '7';
		if (num == '5') return '2';
		if (num == '6') return '9';
		if (num == '7') return '8';
		if (num == '8') return '0';
		if (num == '9') return '1';
		if (num == '0') return '6';
		return num;
	}

	std::string protectId(int int_id) {
		std::string id = std::to_string(int_id);
		std::string output = "";
		int length = std::strlen(id.c_str());
		for (int x = 0; x < length; x++) {
			char num = id.at(x);
			output += convertNum(num);
		}
		return output;
	}



	std::string DownloadURL(const char* URL) {
		HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
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
				std::string p = replaceAll(rtn, "|n", "\r\n");
				return p;
			}
		}
		InternetCloseHandle(interwebs);
		std::string p = replaceAll(rtn, "|n", "\r\n");
		return p;
	}
}

void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory, unsigned char includePath) // thx stackoverflow
{
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;


		out.push_back(includePath ? full_file_name : file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
}

#include <Psapi.h>

int FileCheck(int pid, std::vector<std::string> list) {
	int count = 0;
	HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);

	if (proc) {
		char pathRaw[MAX_PATH];
		if (GetModuleFileNameEx(proc, NULL, pathRaw, MAX_PATH)) {
			std::string path = pathRaw;
			path = path.substr(0, path.find_last_of("\\/")); // remove file name

			std::vector<std::string> files;
			GetFilesInDirectory(files, path, false);

			for (std::vector<std::string>::iterator i = files.begin(); i != files.end(); i++) {
				for (std::vector<std::string>::iterator j = list.begin(); j != list.end(); j++) {
					if (*i == *j) count++;
				}
			}
		}
	}

	CloseHandle(proc);

	return count;
}

#define MSFOUND(o) throw std::exception("malicious item '" o "' found");
#define CHECKPROC(p) strcmp(entry.szExeFile, p) == 0

#include <locale>
#include <utility>
#include <codecvt>
#include <TlHelp32.h>

namespace registry {
	int ReadString(HKEY key, const char* value, std::string& out) {
		int val_sz;
		int val_type;
		char* val;

		if (RegQueryValueEx(key, value, NULL, (LPDWORD)&val_type, NULL, (LPDWORD)&val_sz) != ERROR_SUCCESS)
			return 0;

		if (val_type != REG_SZ || !val_sz)
			return 0;

		val = new (std::nothrow) char[val_sz];
		if (!val) return 0;

		if (RegQueryValueEx(key, value, NULL, NULL, (LPBYTE)val, (LPDWORD)&val_sz) != ERROR_SUCCESS) {
			delete[] val;
			return 0;
		}

		out = val;
		delete[] val;
		return 1;
	}
}

void MaliciousSoftwareCheck(int* flag) {
#define CHECKMALICIOUS
#ifdef CHECKMALICIOUS
	//VMProtectBeginMutation("kek");

	if (FindWindow(NULL, "Fiddler Web Debugger") || FindWindow(NULL, "Fiddler - HTTP Debugging Proxy"))
		MSFOUND("Fiddler_0");

	if (OpenMutex(MUTEX_ALL_ACCESS, false, "Global\\ProxifierRunning"))
		MSFOUND("Proxifier_0")

		PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (CHECKPROC("Fiddler.exe"))
				MSFOUND("Fiddler_1");

			if (FileCheck(entry.th32ProcessID, { "saz.ico", "fiddler.exe.config" }))
				MSFOUND("Fiddler_2");

			if (CHECKPROC("Charles.exe"))
				MSFOUND("Charles_0");

			if (FileCheck(entry.th32ProcessID, { "charles.ico", "Charles.ini" }))
				MSFOUND("Charles_1");

			if (CHECKPROC("cheatengine-i386.exe") || CHECKPROC("cheatengine-x86_64.exe"))
				MSFOUND("CE_0");

			if (FileCheck(entry.th32ProcessID, { "ceregreset.exe", "commonmodulelist.txt", "dbk64.sys", "dbk32.sys", "vehdebug-x86_64.dll", "vehdebug-i386.dll" }))
				MSFOUND("CE_1");

			if (CHECKPROC("Wireshark.exe"))
				MSFOUND("Wireshark_0");

			if (FileCheck(entry.th32ProcessID, { "rawshark.exe", "WinPcap_4_1_3.exe", "dumpcap.exe" }))
				MSFOUND("Wireshark_1");
		}
	}

	CloseHandle(snapshot);

	HKEY io_key = NULL;
	std::string ps;

	HKEY f_key = NULL;
	int fport;
	std::string fp;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Fiddler2\\", NULL, KEY_READ, &f_key) == ERROR_SUCCESS) {
		if (RegQueryValueEx(f_key, "ListenPort", NULL, NULL, (LPBYTE)&fport, NULL) == ERROR_SUCCESS)
			fp = "127.0.0.1:" + std::to_string(fport);
		else
			fp += "127.0.0.1:8888";

		int proxyEnabled = 0;
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\", NULL, KEY_READ, &io_key) == ERROR_SUCCESS &&
			RegQueryValueEx(io_key, "ProxyEnabled", NULL, NULL, (LPBYTE)&proxyEnabled, NULL) == ERROR_SUCCESS &&
			proxyEnabled) {

			if (registry::ReadString(io_key, "ProxyServer", ps) &&
				ps.find(fp) != std::string::npos)
				MSFOUND("proxy (Fiddler/Charles/Other)");
		}
	}

	if (io_key) RegCloseKey(io_key);
	if (f_key) RegCloseKey(f_key);

	*flag = 1;

	//VMProtectEnd();
#endif
}

#define LUAVM_DAX_ME 0x1C6B438 
#define LUAVM_DAX_AO 0x1A7D575
#define LUAVM_DAX_MO 0x1451AFB 

static uint32_t rbxDaxEncodeOp(uint32_t x, uint32_t mulEven, uint32_t addEven, uint32_t mulOdd, uint32_t addOdd)
{
	uint32_t result = 0;
	uint32_t mask = 1;
	for (size_t i = 0; i < 8 * sizeof(uint32_t); ++i)
	{
		uint32_t bitDesired = mask & x;
		uint32_t bitOdd = mask & (result*mulOdd + addOdd);
		uint32_t bitEven = mask & (result*mulEven + addEven);
		if ((bitEven ^ bitOdd) != bitDesired)
		{
			result |= mask;
		}
		mask <<= 1;
	}
	return result;
}


#include <locale>
#pragma comment(lib,"ws2_32.lib")

std::string validp = "";

std::string getUserId() {
	rblua_getglobal(lua::lua_State, "game");
	rblua_getfield(lua::lua_State, -1, "Players");
	rblua_getfield(lua::lua_State, -1, "LocalPlayer");
	rblua_getfield(lua::lua_State, -1, "Name");
	validp = rblua_tostring(lua::lua_State, -1);
	return validp;
	rblua_emptystack(lua::lua_State);
}

string replace(string word, string target, string replacement) {
	int len, loop = 0;
	string nword = "", let;
	len = word.length();
	len--;
	while (loop <= len) {
		let = word.substr(loop, 1);
		if (let == target) {
			nword = nword + replacement;
		}
		else {
			nword = nword + let;
		}
		loop++;
	}
	return nword;

}

#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")

char* getMAC() {
	PIP_ADAPTER_INFO AdapterInfo;
	DWORD dwBufLen = sizeof(AdapterInfo);
	char *mac_addr = (char*)malloc(17);

	AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (AdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");

	}
	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {

		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
		if (AdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
		}
	}

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
		do {
			sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
			return mac_addr;

			printf("\n");
			pAdapterInfo = pAdapterInfo->Next;
		} while (pAdapterInfo);
	}
	free(AdapterInfo);
}

std::string GetMac() {
	char data[4096];
	ZeroMemory(data, 4097);
	unsigned long len = 4000;

	PIP_ADAPTER_INFO pinfo = (PIP_ADAPTER_INFO)data;
	char sbuf[20];
	string sret;

	DWORD ret = GetAdaptersInfo(pinfo, &len);
	if (ret != ERROR_SUCCESS)
		return string("**ERROR**");

	for (int k = 0; k < 5; k++) {
		sprintf(sbuf, "%02X-", pinfo->Address[k]);
		sret += sbuf;
	}
	sprintf(sbuf, "%02X", pinfo->Address[5]);
	sret += sbuf;

	return(sret);
}





bool Authentication() {
	//VMProtectBeginUltra("hotshotqq.com/auth.php?h=");
	typedef unsigned long DWORD;
	getUserId();


	int msflag = 0;
	MaliciousSoftwareCheck(&msflag);
	if (!msflag)
		printf("malicious software/item found");

	HW_PROFILE_INFO hardwareinfo;
	if (GetCurrentHwProfile(&hardwareinfo) != NULL) {
		char* d = getMAC();
		std::string wadawda = d;
		std::string dc = GetMac();


		CHAR szVolumeNameBuffer[12];
		DWORD dwVolumeSerialNumber;
		DWORD dwMaximumComponentLength;
		DWORD dwFileSystemFlags;
		CHAR szFileSystemNameBuffer[10];

		GetVolumeInformation("C:\\", szVolumeNameBuffer, 12, &dwVolumeSerialNumber, &dwMaximumComponentLength, &dwFileSystemFlags, szFileSystemNameBuffer, 10);

		string HWID = hardwareinfo.szHwProfileGuid;

		std::string HVID;
		HVID += replace(HWID, "}", "-");
		HVID += to_string(dwVolumeSerialNumber);
		HVID += "-";
		HVID += replace(d, ":", "-");
		HVID += "-";
		HVID += GetMac();
		HVID += "}";

		std::string baseurl = "http://hotshotqq.website/caches/hwid.php?h=";
		baseurl += HVID;
		baseurl += "&u=";
		baseurl += validp;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		string Response = Auth::DownloadURL(baseurl.c_str());

		if (Response == std::string("xD")) {
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Fail!\n";
			cout << "Your HWID is copied to your clipboard.\n";
			int len = strlen(HVID.c_str());
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
			void *mem_ptr = GlobalLock(hMem);
			memcpy(mem_ptr, HVID.c_str(), len + 1);
			GlobalUnlock(hMem);
			OpenClipboard(NULL);
			EmptyClipboard();
			SetClipboardData(CF_TEXT, hMem);
			CloseClipboard();
			Sleep(999999999999999 * 999999);
		}
		else if (Response == std::string("complete - ") + HVID) {
			SetConsoleTextAttribute(hConsole, 10);
			cout << "Done!\n";
		}
		else {
			SetConsoleTextAttribute(hConsole, 10);
			cout << "Done!\n";
		}
	}
	//VMProtectEnd();
	return true;
}