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
#include "XorS.h"
#pragma comment(lib, "wininet.lib")
#include "VMProtectSDK.h"
#pragma comment(lib,"ws2_32.lib")

using namespace std;

DWORD ScriptContext;
DWORD ScriptContextVFTable = Rebase(0x1A45C58);

void WrapRBX(const char* s)
{
	r_lua_getglobal(RLS, s);
	Wrap(RLS, LS, -1);
	lua_setglobal(LS, s);
	r_lua_pop(RLS, 1);
}

namespace Hook {

	DWORD RLSorg = 0;
	DWORD hookaddr = 0x766410; //gettop addr

	int gettopd(DWORD rState)
	{
		RLSorg = rState;
		return (*(DWORD*)(rState + top) - *(DWORD*)(rState + base)) >> 4;
	}

	void CreateHook() {
		MH_Initialize();
		void* oldhook = (void*)MH_CreateHook((DWORD*)Rebase(hookaddr), gettopd, NULL);
		MH_CreateHook((DWORD*)Rebase(hookaddr), gettopd, (LPVOID*)oldhook);
		MH_EnableHook((DWORD*)Rebase(hookaddr));
		MH_DisableHook((DWORD*)Rebase(hookaddr));

		if (RLSorg == 0) {
			void* oldhook = (void*)MH_CreateHook((DWORD*)Rebase(hookaddr), gettopd, NULL);
			MH_CreateHook((DWORD*)Rebase(hookaddr), gettopd, (LPVOID*)oldhook);
			MH_EnableHook((DWORD*)Rebase(hookaddr));
			while (RLS == 0) { Sleep(1); }
			MH_DisableHook((DWORD*)Rebase(hookaddr));
		}
	}
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
	int args = lua_gettop(L) - 1;
	std::string url;
	if (args == 2) {
		std::cout << lua_tostring(L, -2) << std::endl;
		url = DownloadURL(lua_tostring(L, -2));
	}
	else {
		std::cout << lua_tostring(L, -1) << std::endl;
		url = DownloadURL(lua_tostring(L, -1));
	}
	lua_pushstring(L, url.c_str());
	return 1;
}

DWORD WINAPI luapipe(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	XorS(sdfsdfsdf, "\\\\.\\pipe\\ConfiscationPublicApi"); // real pipe (which is hidden and can't be seen)
	std::string("\\\\.\\pipe\\imperiousisaskid"); // fake pipe XDD (which can be seen and tricks people thinking they got the pipe XDDD)
	hPipe = CreateNamedPipe(TEXT(sdfsdfsdf.decrypt()),
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
	XorS(sdfsdfsdf, "\\\\.\\pipe\\ConfiscationPublicApi"); // real pipe (which is hidden and can't be seen)
	std::string("\\\\.\\pipe\\imperiousisaskid"); // fake pipe XDD (which can be seen and tricks people thinking they got the pipe XDDD)
	hPipe = CreateNamedPipe(TEXT(sdfsdfsdf.decrypt()),
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
	XorS(sdfsdfsdf, "\\\\.\\pipe\\ConfiscationPublicApi"); // real pipe (which is hidden and can't be seen)
	std::string("\\\\.\\pipe\\imperiousisaskid"); // fake pipe XDD (which can be seen and tricks people thinking they got the pipe XDDD)
	hPipe = CreateNamedPipe(TEXT(sdfsdfsdf.decrypt()),
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
	GetFile("Confiscation_API_DLL.dll", "", path, MAX_PATH);

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
	GetFile("Confiscation_API_DLL.dll", "", path, MAX_PATH);

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
	r_lua_pushvalue(RLS, LUA_GLOBALSINDEX);
	Wrap(RLS, L, -1);
	return 1;
}

int getreg(lua_State* rL) {
	r_lua_pushvalue(RLS, LUA_REGISTRYINDEX);
	Wrap(RLS, rL, -1);
	return 1;
}

int getrenv(lua_State* L) {
	r_lua_pushvalue(RLS, int(RLS));
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

int __Nemesis(lua_State* L) {
	r_lua_getglobal(RLS, "print");
	r_lua_pushstring(RLS, "Nemesis Bitch");
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
		//str_lp = "Yoink Executor crashed.. but don't worry! No logs were sent to ROBLOX";
		str_lp = "If Yoink continues to crash, feel free to use the DLL Configuration form to change DLLs";
		str_cap = "Yoink Executor has crashed";
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

int error(lua_State* thread) {
	rlua_print(Error, lua_tostring(thread, -1));
	return 1;
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

//int error(lua_State* thread) {
//	std::string errorstr = lua_tostring(thread, -1);
//	int type = lua_tonumber(thread, -2);
//	std::string finalerror = "error('"; finalerror = finalerror + errorstr; finalerror = finalerror + "')";
//	luaL_dostring(thread, finalerror.c_str());
//	return 1;
//}

int newcclosure(lua_State* L) {
	lua_pushvalue(L, 1);
	return 1;
}

int getsenv(lua_State* LS) {
	lua_pushvalue(LS, LUA_ENVIRONINDEX);
	Wrap(RLS, LS, -1);
	return 1;
}

#include "MouseLib.h"

typedef void(__cdecl* unkidentity)(int, int, int);
unkidentity SandboxThread = (unkidentity)Rebase(0x77c090);//Updated 18/10/2019

#include "Auth.h"
int ImperiousSoRetardedHisErrorFunctionBAIT() {
	//if (AUTH()) {
		ConsoleBypass("Nemesis Debug Console | Powered By Imperious | NemesisOS | Unrestricted Execution");
		LS = lua_open();

		std::cout << iblue << "[OutPut] Lua Open" << endl;
		
		//ScriptContext = Memory::Scan();
		//int v48 = 0;
		//int v39 = ScriptContext;
		//RLS = v39 + 56 * v48 + 172 - *(DWORD*)(v39 + 56 * v48 + 172);

		Hook::CreateHook();
		RLS = Hook::RLSorg;

		std::cout << iblue << "[OutPut] Lua Open" << endl;


		*reinterpret_cast<int*>(*reinterpret_cast<int*>(RLS + 136) + 24) = 6; /*forgot i fed you it earlier*/

		std::cout << iblue << "[OutPut] Lua Open" << endl;

		BypassBreakpoints();
		luaL_openlibs(LS);

		luaL_newmetatable(LS, "garbagecollector");
		lua_pushcfunction(LS, UserDataGC);
		lua_setfield(LS, -2, "__gc");
		srand(time(NULL));
		int garbagecollected = std::rand();
		lua_pushvalue(LS, -1);
		lua_setfield(LS, -2, "__index");

		std::cout << ired << "[OutPut]";
		WrapRBX("printidentity");
		std::cout << ipurple << " PrintIdentity" << endl;

		std::cout << ired << "[OutPut]";
		WrapRBX("game");
		WrapRBX("Game");
		std::cout << ipurple << " Game" << endl;

		std::cout << ired << "[OutPut]";
		WrapRBX("workspace");
		WrapRBX("Workspace");
		std::cout << ipurple << " WorkSpace" << endl;

		std::cout << ired << "[OutPut]";
		WrapRBX("Axes");
		std::cout << ipurple << " Axes" << endl;

		std::cout << ired << "[OutPut]";
		WrapRBX("BrickColor");
		WrapRBX("CFrame");
		WrapRBX("Color3");
		WrapRBX("ColorSequence");
		WrapRBX("ColorSequenceKeypoint");
		WrapRBX("NumberRange");
		WrapRBX("NumberSequence");
		WrapRBX("NumberSequenceKeypoint");
		WrapRBX("PhysicalProperties");
		std::cout << ipurple << " PhysicalProperties" << endl;

		WrapRBX("Ray");
		WrapRBX("Rect");
		WrapRBX("Region3");
		WrapRBX("Region3int16");
		WrapRBX("TweenInfo");

		std::cout << ired << "[OutPut]";
		WrapRBX("UDim");
		WrapRBX("UDim2");
		std::cout << ipurple << " UDim's" << endl;


		std::cout << ired << "[OutPut]";
		WrapRBX("Vector2");
		WrapRBX("Vector2int16");
		WrapRBX("Vector3");
		WrapRBX("Vector3int16");
		WrapRBX("Enum");
		WrapRBX("Faces");
		WrapRBX("Instance");
		std::cout << ipurple << " Vectors" << endl;

		std::cout << ired << "[OutPut]";
		WrapRBX("math");
		WrapRBX("warn");
		WrapRBX("typeof");
		WrapRBX("type");
		WrapRBX("spawn");
		WrapRBX("Spawn");
		WrapRBX("print");
		WrapRBX("printidentity");
		std::cout << ipurple << " Spawn" << endl;

		std::cout << ired << "[OutPut]";
		WrapRBX("ypcall");
		WrapRBX("Wait");
		WrapRBX("wait");
		std::cout << ipurple << " Wait's" << endl;

		std::cout << ired << "[OutPut]";
		WrapRBX("delay");
		WrapRBX("Delay");
		WrapRBX("tick");
		WrapRBX("LoadLibrary");
		std::cout << ipurple << " Finishing Up!!" << endl;

		std::cout << ired << "[Lua_Register]";
		lua_register(LS, "HttpGet", custom_httpget);
		lua_register(LS, "DLS", httpgetimpl);
		lua_register(LS, "loadstring", custom_loadstring);
		std::cout << ipurple << " Loadstrings And GetObjects" << endl;

		std::cout << ired << "[Lua_Register]";
		lua_register(LS, "EQ", EQ);
		lua_register(LS, "GC", GC);
		lua_register(LS, "getrawmetatable", getRawMetaTable);
		std::cout << ipurple << " GetRawMetaTable" << endl;

		std::cout << ired << "[Lua_Register]";
		lua_register(LS, "getreg", getreg);
		lua_register(LS, "getrenv", getrenv);
		lua_register(LS, "getgenv", getgenv);
		std::cout << ipurple << " GetStacks" << endl;

		std::cout << ired << "[Lua_Register]";
		lua_register(LS, "setreadonly", setreadonly);
		lua_register(LS, "WriteLog", outputtoconsole);
		lua_register(LS, "copystring", fn_toclipboard);
		lua_register(LS, "setclipboard", fn_toclipboard);
		lua_register(LS, "custom_copyclipboard", fn_toclipboard);
		lua_register(LS, "fn_toclipboard", fn_toclipboard);
		lua_register(LS, "toclipboard", fn_toclipboard);
		lua_register(LS, "toclipboard", fn_toclipboard);
		std::cout << ipurple << " ClipBoards" << endl;

		std::cout << ired << "[Lua_Register]";
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
		std::cout << ipurple << " FULL Debug Lib" << endl;

		std::cout << ired << "[Lua_Register_Custom]";
		lua_register(LS, "getsenv", getsenv);
		lua_setglobal(LS, "_G");
		lua_register(LS, "require", custom_require);
		lua_register(LS, "error", error);
		lua_register(LS, "__Nemesis", __Nemesis);
		std::cout << ipurple << " Done" << endl;
		std::cout << ired << "[Lua_Register]";
		KeyBoardLibs::RegisterKeyBoardLibs(LS);
		MouseLibs::RegisterMouseLibs(LS);
		std::cout << ipurple << " Mouse And KeyPress" << endl;
		std::cout << ired << "[Injected] Nemesis API Has Been Injected Now Closing";
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)cmdpipe, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)luacpipe, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)luapipe, NULL, NULL, NULL);
		::ShowWindow(GetConsoleWindow(), SW_HIDE);
	//	ShowIMGUI(LS);
		luaL_dostring(LS, "game.StarterGui:SetCore('SendNotification', {Duration = 4; Title='Nemesis API'; Text='Attached'})");
		//r_lua_getglobal(RLS, "print");
		//r_lua_pushstring(RLS, "Nemesis Loaded");
		return 1;
	}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	if (Reason == 1) {
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ImperiousSoRetardedHisErrorFunctionBAIT, NULL, NULL, NULL);
	}
	return TRUE;
}