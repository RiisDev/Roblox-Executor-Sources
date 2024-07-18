#include "globals.h"
#include "Bridge.h"
#include "XorS.h"
#include "Console.h"
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
#include <Psapi.h>
#include <windows.h>
#pragma comment(lib, "wininet.lib")
using namespace std;

using Bridge::m_rL;
using Bridge::m_L;

void PushGlobal(DWORD rL, lua_State* L, const char* s)
{
	r_lua_getglobal(rL, s);
	Bridge::push(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
}

static int UserDataGC(lua_State *Thread) {
	void *UD = lua_touserdata(Thread, 1);
	int GarbageCount;
	if (m_rL) {
		r_lua_rawgeti(m_rL, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(m_rL, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
			//printf(("GC: " + std::to_string(GarbageCount) + "\n").c_str());
		}
	}
	return 0;
}

int Lua_WrapGlobal(lua_State* L) {
	std::string globalName = lua_tostring(L, -1);
	PushGlobal(m_rL, m_L, globalName.c_str());
	return 1;
}

static int GlobalsIndex(lua_State* Thread) {
	int RThread = r_lua_newthread(m_rL);
	r_lua_getglobal(RThread, lua_tostring(Thread, 2));
	if (r_lua_type(RThread, -1) > R_LUA_TNIL) {
		Bridge::push(RThread, Thread, -1);
		return 1;
	}
	return 0;
}

std::string replace_all(std::string subject, const std::string& search, const std::string& replace) {
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
			std::string p = replace_all(rtn, "|n", "\r\n");
			return p;
		}
	}

	InternetCloseHandle(interwebs);
	std::string p = replace_all(rtn, "|n", "\r\n");
	return p;
}

namespace Tools {
	using namespace std;
	string replaceAll(string subject, const string& search,
		const string& replace) {
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != string::npos) {
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}
}


void ExecuteScript(std::string Script) {
	//////////////////////////////////////////////////////////////////////////////////////
	XorS(start, "spawn(function()\r\n");                                                //  
	XorS(start1, "\r\nend)");                                                           //
	Script = start.decrypt() + Script + start1.decrypt();                              //
	XorS(game1, "Game:");                                                               //
	XorS(game2, "game:");                                                               //  
	Script = replace_all(Script, game1.decrypt(), game2.decrypt());                     //
	//////////////////////////////////////////////////////////////////////////////////////
	XorS(gobjexec, "GetObjects");                                                       //
	XorS(httpgetexec, "HttpGet");                                                       // 
	XorS(gobjexec1, "game:GetObjects");                                                 //
	XorS(httpgetexec1, "game:HttpGet");                                                 //
	Script = replace_all(Script, gobjexec1.decrypt(), gobjexec.decrypt());              //
	Script = replace_all(Script, httpgetexec1.decrypt(), httpgetexec.decrypt());        //
	//////////////////////////////////////////////////////////////////////////////////////
	XorS(execcore, "game.Players.LocalPlayer:WaitForChild(\"PlayerGui\")");             //
	XorS(execcore1, "game.CoreGui");                                                    //
	//////////////////////////////////////////////////////////////////////////////////////
	Script = replace_all(Script, execcore.decrypt(), execcore1.decrypt());              //
	//////////////////////////////////////////////////////////////////////////////////////
	XorS(execcore2, "game.Players.LocalPlayer.PlayerGui");                              //
	XorS(execcore3, "game.CoreGui");                                                    //
	//////////////////////////////////////////////////////////////////////////////////////
	Script = replace_all(Script, execcore2.decrypt(), execcore3.decrypt());             //
	//////////////////////////////////////////////////////////////////////////////////////
	XorS(execraincustomhttp, "Raindrop:DownloadString");                                //
	Script = replace_all(Script, execraincustomhttp.decrypt(), "HttpGet");              //
	//////////////////////////////////////////////////////////////////////////////////////
	XorS(execkvixscustomhttp, "Yoink:DownloadString");                                  //
	Script = replace_all(Script, execkvixscustomhttp.decrypt(), "HttpGet");

	Script.c_str();

	if (luaL_loadstring(m_L, Script.c_str())) {
		std::string Error = lua_tostring(m_L, -1);
		XorS(execwarn, "warn");
		r_lua_getglobal(m_rL, execwarn.decrypt());
		r_lua_pushstring(m_rL, Error.c_str());
		r_lua_pcall(m_rL, 1, 0, 0);
		r_lua_settop(m_rL, 0);
	}
	else {
		lua_pcall(m_L, 0, 0, 0);
	}
	UserDataGC(m_L);
}


void ExecuteScript1(std::string Script) {
	Script = "spawn(function()script=Instance.new('LocalScript') script.Name = 'Yoink' \r\n" + Script + "\r\nend)"; //change for diff exploits
	Script = replace_all(Script, "Game:", "game:");
	Script = replace_all(Script, "game:GetObjects", "GetObjects");
	Script = replace_all(Script, "game:HttpGetAsync", "HttpGet");
	Script = replace_all(Script, "game:HttpGet", "HttpGet");
	//Script = replace_all(Script, "x.x = x", "error(\'attempt to call a table value'\)");
	Script = replace_all(Script, "while true do end", "error(\'attempt to call a table value'\)");
	Script = replace_all(Script, "local function spawn()end", "error(\'attfempt to call a table value'\)");
	Script = replace_all(Script, "function printidentity()", "print\"level suck my nuts\" end"); //DEEZ NUTS HA GOTTEM
	if (luaL_loadstring(m_L, Script.c_str())) {
		std::string Error = lua_tostring(m_L, -1);
		printf("Error: %s\n", Error);
		//r_lua_getglobal(m_rL, "warn");
		r_lua_pushstring(m_rL, Error.c_str());
		r_lua_pcall(m_rL, 1, 0, 0);
		r_lua_settop(m_rL, 0);
	}
	else {
		lua_pcall(m_L, 0, 0, 0);
	}
	UserDataGC(m_L);
}


DWORD WINAPI input(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\crashpad_5680_QEKFAKDIUDQCWAJB"),
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
			ExecuteScript(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

#include <windows.h>
#include <string>
#include <stdio.h>
using std::string;


#define CRT_SECURE_NO_WARNINGS
#define CRT_SECURE_NO_DEPRICATE

#pragma comment(lib,"ws2_32.lib")

HINSTANCE hInst;
WSADATA wsaData;
void mParseUrl(char* mUrl, string & serverName, string & filepath, string & filename);
SOCKET connectToServer(char* szServerName, WORD portNum);
int getHeaderLength(char* content);
char* readUrl2(char* szUrl, long& bytesReturnedOut, char** headerOut);

#define CRT_SECURE_NO_WARNINGS
#define CRT_SECURE_NO_DEPRICATE

int custom_httpget(lua_State * L) {
	std::string URL = std::string(lua_tostring(L, -1));
	XorS(ticket, "getauthticket");
	if (URL.find(ticket.decrypt()) != string::npos) {
		XorS(httperror, "Auth Ticket Logger detected, Http request canceled.");
		luaL_error(L, httperror.decrypt());
		return 0;
	}
	lua_pushstring(L, std::string(download_url(URL)).c_str());
	return 1;
}

int custom_loadstring(lua_State * L) {
	if (lua_type(L, -1) != LUA_TSTRING) {
		XorS(loadargument_bad, "(Bad Argument) - 'loadstring' only accepts 1 argument (string).");
		luaL_error(L, loadargument_bad.decrypt());
		return 0;
	}
	if (lua_gettop(L) == 0) {
		XorS(loadnewarg, "'loadstring' needs 1 string argument.");
		luaL_error(L, loadnewarg.decrypt());
		return 0;
	}
	ExecuteScript(std::string(lua_tostring(L, -1)));
	return 0;
}

static int Custom_GetObjects(lua_State * L) {
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

string DownloadURL(string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	string rtn;
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
			string p = rpall(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	string p = rpall(rtn, "|n", "\r\n");
	return p;
}

int httpgetimpl(lua_State* LS) {
	lua_pushstring(LS, DownloadURL(lua_tostring(LS, -1)).c_str());
	return 1;
}




int Debug_GetMetaTable(lua_State * L) {
	if (lua_gettop(L) == 0) {
		luaL_error(L, "'debug.getrawmetatable' needs 1 argument.");
		return 0;
	}
	Bridge::push(m_rL, L, 1);
	if (r_lua_getmetatable(m_rL, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Bridge::push(L, m_rL, -1);
	return 1;
}


void ConsoleBypass(const char* Title) {
	DWORD superskiddoisachink;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &superskiddoisachink);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

void r_lua_pushobject(DWORD pRobloxState, TValue *value) {
	auto &topa = *(TValue**)(pRobloxState + top);
	*topa = *value;
	++topa;
}

inline void r_lua_getservice(DWORD state, const char* s)
{
	r_lua_getfield(m_rL, -10002, "game");
	r_lua_getfield(m_rL, -1, "GetService");
	r_lua_pushvalue(m_rL, -2);
	r_lua_pushstring(m_rL, s);
	r_lua_pcall(m_rL, 2, 1, 0);
}

static int GC(lua_State *Thread) {
	void *UD = lua_touserdata(Thread, 1);
	if (m_rL) {
		r_lua_rawgeti(m_rL, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(m_rL, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
		}
	}
	return 0;
}

static int EQ(lua_State *Thread) {
	lua_pushboolean(Thread, (*(DWORD *)lua_touserdata(Thread, 1) == *(DWORD *)lua_touserdata(Thread, 2)));
	return 1;
}

void r_lua_emptystack(DWORD luaState)
{
	r_lua_settop(luaState, 0);
};

void PushCClosure(const void* func, DWORD n) {
	try {
		r_lua_pushcclosure(m_rL, (int)func, n);
	}
	catch (...) {};
}

/*int gettop(int rL) {
	int top = 8; // rL->top offset
	int base = 12; // rL->base offset

	return *(DWORD*)(rL + top) - *(DWORD*)(rL + base) >> 4;
}*/

static int load_aux(lua_State *L, int status) {
	if (status == 0)  /* OK? */
		return 1;
	else {
		lua_pushnil(L);
		lua_insert(L, -2);  /* put before error message */
		return 2;  /* return nil plus error message */
	}
}

static int loadstring(lua_State *L) {
	size_t l;
	const char *s = luaL_checklstring(L, 1, &l);
	const char *chunkname = luaL_optstring(L, 2, s);
	return load_aux(L, luaL_loadbuffer(L, s, l, chunkname));
}

r_TValue* r_gettop(int r_lua_State) {
	return *(r_TValue**)(r_lua_State + 16);
}
int setrawmetatable_impl(lua_State* r_lua_State) {
	if (r_lua_gettop(m_rL) < 2) {
		r_lua_pushboolean(m_rL, 0);
		r_lua_pushstring(m_rL, "2 or more arguments expected");
		return 2;
	}

	r_TValue* top = r_gettop(m_rL);
	r_TValue* obj1 = top - 1;
	r_TValue* obj0 = top - 2;

	if (!(obj0->tt == R_LUA_TTABLE || obj0->tt == R_LUA_TUSERDATA) || !(obj1->tt == R_LUA_TNIL || obj1->tt == R_LUA_TTABLE)) {
		r_lua_pushboolean(m_rL, 0);
		r_lua_pushstring(m_rL, "bad argument types");
		return 2;
	}

	r_lua_setmetatable(m_rL, 1);
	r_lua_pushboolean(m_rL, 1);
	return 1;
}

int getgenv(lua_State *L) {
	lua_pushvalue(L, LUA_GLOBALSINDEX);
	Bridge::push(m_rL, L, -1);
	return 1;
}

int getreg(lua_State *L) {
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	Bridge::push(m_rL, L, -1);
	return 1;
}

int getrenv(lua_State* L) {
	lua_pushvalue(L, int(m_rL));
	Bridge::push(m_rL, L, -1);
	return 1;
}

void lua_setfield(DWORD L, int idx, const char *k) {
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


BOOL WINAPI CONSOLEBYPASS()
{
	DWORD nOldProtect;
	if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(FreeConsole) = 0xC3;
	if (!VirtualProtect(FreeConsole, 1, nOldProtect, &nOldProtect))
		return FALSE;
	AllocConsole();
}

//namespace Hook {
//	void* placeHook(DWORD address, void* hook, bool revert = false) {
//		DWORD oldprot;
//		if (!revert) {
//			void* oldmem = new void*;
//			void* result = new void*;
//			memcpy(oldmem, (void*)address, sizeof(void*) * 4);
//			VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
//			*(char*)address = 0xE9; *(DWORD*)(address + 1) = (DWORD)hook - address - 5;
//			memcpy(result, oldmem, sizeof(void*) * 4);
//			VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
//			return result;
//		}
//		else {
//			VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
//			memcpy((void*)address, hook, sizeof(void*) * 4);
//			VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
//			return NULL;
//		}
//	}
//
//}
//DWORD RLSorg = 0;
//DWORD hookaddr = 0x827290; //gettop addr

int mouse1press_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse1release_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse1click_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2press_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2release_impl(int r_lua_State) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mouse2click_impl(int r_lua_State) {
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

int Mouse1Down(lua_State * m_L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse1Up(lua_State * L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse2Down(lua_State * L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse2Up(lua_State * L) {
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse1Click(lua_State * L) {
	float n = lua_tonumber(L, -1);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	Sleep(n);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int Mouse2Click(lua_State * L) {
	float n = lua_tonumber(L, -1);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	Sleep(n);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int MouseScroll(lua_State * L) {
	int amount = lua_tonumber(L, -1);
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_HWHEEL;
	Inputs[0].mi.mouseData = amount;
	SendInput(1, Inputs, sizeof(INPUT));
	Inputs[0].mi.mouseData = NULL;
	return 0;
}


int custom_copyclipboard(lua_State *Ls) {
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

void toClipboard(const std::string &s) {
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

int fn_toclipboard(lua_State *L) {
	std::string tc = lua_tostring(L, -1);
	toClipboard(tc);
	MessageBoxA(NULL, "Successfully copied to your clipboard!", "Done!", NULL);
	return 0;
}

//int getsenv(lua_State * LS) {
//	lua_pushvalue(m_L, LUA_ENVIRONINDEX);
//	Bridge::push(m_rL, m_L -1);
//	return 1;
//}

//namespace Hook {
//
//	DWORD RLSorg = 0;
//	DWORD hookaddr = 0x825d90; //gettop addr
//
//	int gettopd(DWORD rState)
//	{
//		RLSorg = rState;
//		return (*(DWORD *)(rState + top) - *(DWORD *)(rState + base)) >> 4;
//	}
//
//	void CreateHook() {
//		MH_Initialize();
//		void* oldhook = (void*)MH_CreateHook((DWORD*)x(hookaddr), gettopd, NULL);
//		MH_CreateHook((DWORD*)x(hookaddr), gettopd, (LPVOID*)oldhook);
//		MH_EnableHook((DWORD*)x(hookaddr));
//		MH_DisableHook((DWORD*)x(hookaddr));
//
//		if (RLSorg == 0) {
//			void* oldhook = (void*)MH_CreateHook((DWORD*)x(hookaddr), gettopd, NULL);
//			MH_CreateHook((DWORD*)x(hookaddr), gettopd, (LPVOID*)oldhook);
//			MH_EnableHook((DWORD*)x(hookaddr));
//			while (m_rL == 0) { Sleep(1); }
//			MH_DisableHook((DWORD*)x(hookaddr));
//		}
//	}
//}

DWORD ScriptContext;
DWORD ScriptContextVFTable = x(0x1D0DF3C);

bool CompareData(const char* Data, const char* Pattern, const char* Mask) {
	while (*Mask) {
		if (*Mask != '?') {
			if (*Data != *Pattern) {
				return false;
			};
		};
		++Mask;
		++Data;
		++Pattern;
	};
	return true;
};

struct AOBType {

	string name;

	const char* first;

	const char* second;

};



/*

AOB Scanning

*/

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

	void write(void* address, void* instructions, size_t size) {
		DWORD tbuf;
		VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &tbuf);
		memcpy(address, instructions, size);
		VirtualProtect(address, 1, tbuf, &tbuf);
	}

	bool compare(BYTE* address, BYTE* pattern, BYTE* mask) {
		for (; *mask; address++, pattern++, mask++) {
			if (*mask == 'x' && *address != *pattern) {
				return false;
			}
		}
		return true;
	}

	DWORD scan(BYTE* aob, BYTE* mask, BYTE prot = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
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
}


static int MoveMouse(lua_State *L) {
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

int MouseMoveRelative(lua_State * L) {
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
		str_lp = "Yoink Executor crashed.. but don't worry! No logs were sent to ROBLOX";
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

int KeyPress(lua_State * L) {
	WORD key = lua_tonumber(L, -1);
	Inputs[0].ki.wVk = key;
	SendInput(1, Inputs, sizeof(INPUT));
	Inputs[0].ki.wVk = NULL;
	return 0;
}

int KeyRelease(lua_State * L) {
	WORD key = lua_tonumber(L, -1);
	Inputs[0].ki.wVk = key;
	Inputs[0].mi.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, Inputs, sizeof(INPUT));
	Inputs[0].mi.dwFlags = NULL;
	Inputs[0].ki.wVk = NULL;
	return 0;
}

int KeyTap(lua_State * L) {
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

int Debug_GetRegistry(lua_State * L) {
	r_lua_pushvalue(m_rL, LUA_REGISTRYINDEX);
	return 1;
}

static int Debug_setfenv(lua_State * L) {
	luaL_checktype(L, 2, LUA_TTABLE);
	lua_settop(L, 2);
	if (lua_setfenv(L, 1) == 0)
		luaL_error(L, LUA_QL("setfenv")
			" cannot change environment of given object");
	return 1;
}

static int auxupvalue(lua_State * L, int get) {
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

static int Debug_getupvalue(lua_State * L) {
	return auxupvalue(L, 1);
}

static int Debug_setupvalue(lua_State * L) {
	luaL_checkany(L, 3);
	return auxupvalue(L, 0);
}


static int custom_checkcaller(lua_State * L) {
	r_lua_pushboolean(m_rL, true);
	return 1;
}

int CrashRoblox(lua_State * L) {
	exit(10);
	return 0;
}

static lua_State* getthread(lua_State * L, int* arg) {
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

int setreadonly(lua_State * L) {
	DWORD index2address = (DWORD)r_lua_index2adr(m_rL, -1);
	*(BYTE*)(*(DWORD*)index2address + 10) = lua_toboolean(L, -1);
	return 0;
}
int metatable_handler(lua_State *L)
{
	Bridge::push(L, m_rL, lua_upvalueindex(1));
	const char *key = lua_tostring(L, 2);
	r_lua_getmetatable(m_rL, -1);
	r_lua_setreadonly(m_rL, -1, 0);
	Bridge::push(L, m_rL, 3);
	r_lua_setfield(m_rL, -2, key);

	r_lua_pop(m_rL, 2);
	return 0;
}
int getRawMetaTable(lua_State *L)
{
	Bridge::push(L, m_rL, 1);

	r_lua_getmetatable(m_rL, -1);
	r_lua_setreadonly(m_rL, -1, 0);

	Bridge::push(m_rL, L, -1);

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
	Bridge::push(thread, m_rL, -2);
	*(BYTE *)(*(DWORD *)r_lua_index2adr(m_rL, -1) + 6) = lua_toboolean(thread, -1);
	Bridge::push(m_rL, thread, -1);
	return 0;
}

int createconsole(lua_State* L)
{
	string gay = lua_tostring(L, -1);
	ConsoleBypass(gay.c_str());
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
int print(lua_State*L) {
	string lol = lua_tostring(L, -1);
	r_lua_getglobal(m_rL, "print");
	r_lua_pushstring(m_rL, lol.c_str());
	r_lua_pcall(m_rL, 1, 1, 0);
	r_lua_settop(m_rL, 0);
	return 0;
}

//static int Debug_setlocal(lua_State * L) {
//	int arg;
//	lua_State* L1 = getthread(L, &arg);
//	lua_Debug ar;
//	if (!lua_getstack(L1, luaL_checkint(L, arg + 1), &ar))  /* out of range? */
//		return luaL_argerror(L, arg + 1, "level out of range");
//	luaL_checkany(L, arg + 3);
//	lua_settop(L, arg + 3);
//	lua_xmove(L, L1, 1);
//	lua_pushstring(L, lua_setlocal(L1, &ar, luaL_checkint(L, arg + 2)));
//	return 1;
//}

static void settabss(lua_State * L, const char* i, const char* v) {
	lua_pushstring(L, v);
	lua_setfield(L, -2, i);
}

static void settabsi(lua_State * L, const char* i, int v) {
	lua_pushinteger(L, v);
	lua_setfield(L, -2, i);
}

static void treatstackoption(lua_State * L, lua_State * L1, const char* fname) {
	if (L == L1) {
		lua_pushvalue(L, -2);
		lua_remove(L, -3);
	}
	else
		lua_xmove(L1, L, 1);
	lua_setfield(L, -2, fname);
}

static int Debug_getinfo(lua_State * L) {
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
		if (level > LEVELS1&& firstpart) {
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

//int AntiAxonSpawn(lua_State* LS) {                 i dont want this bs
//	if (lua_gettop(LS) == 1) {
//		if (lua_type(LS, 1) != LUA_TFUNCTION) {
//			std::cout << "Not Func On Spawn!\n";
//			luaL_error(LS, "The function 'spawn' expected a CFunction as argument #1!");
//			return 0;
//		}
//		else
//		{
//			r_lua_getglobal(m_rL, "spawn");
//			Bridge::push(m_rL, LS, -1);
//			lua_setfield(LS, LUA_GLOBALSINDEX, "Confiscation");
//			r_lua_pop(m_rL, 1);
//
//			lua_getglobal(LS, "Confiscation");
//			lua_pushcclosure(LS, lua_tocfunction(LS, -1), 1);
//			lua_pcall(LS, 1, 0, 0);
//			return 1;
//		}
//	}
//	else
//	{
//		luaL_error(LS, "The function 'spawn' only accepts one argument");
//		return 0;
//	}
//}

//int print(lua_State* L) {
//	string LOL = lua_tostring(L, -1);
//	r_lua_getglobal(m_rL, "print");
//	r_lua_pushstring(m_rL, LOL.c_str());
//	r_lua_pcall(m_rL, 1, 1, 0);
//	r_lua_settop(m_rL, 0);
//	return 1;
//}

int something(lua_State * L) {
	luaL_dostring(m_L, "print'hi'");
}

void main()
{
	InitUploadCheckHook();
	CONSOLEBYPASS();
	SetConsoleTitle("Yoink Executor Console");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	SetWindowPos(ConsoleHandle, HWND_TOPMOST, 50, 20, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ShowWindow(ConsoleHandle, 1);
	SetConsoleTitle("Yoink Executor");
	cout << blue << R"(
_____.___.      .__        __
\__  |   | ____ |__| ____ |  | __
 /   |   |/  _ \|  |/    \|  |/ /
 \____   (  <_> )  |   |  \    <
 / ______|\____/|__|___|  /__|_ \
 \/                     \/     \/                                               
)" << '\n';
	std::cout << blue << " [System] Hooking..." << endl;
	ScriptContext = Memory::scan((BYTE*)&ScriptContextVFTable, (BYTE*)"xxxx", PAGE_READWRITE);

	int v51 = 0;
	DWORD v39 = ScriptContext;
	m_rL = v39 + 56 * v51 + 172 + *(DWORD *)(v39 + 56 * v51 + 172);

	m_L = luaL_newstate();
	seti(m_rL, 6);
	Bridge::VehHandlerpush();
	luaL_openlibs(m_L);
	luaL_newmetatable(m_L, "garbagecollector");
	lua_pushcfunction(m_L, UserDataGC);
	lua_setfield(m_L, -2, "__gc");
	lua_pushvalue(m_L, -1);
	lua_pushvalue(m_L, -10000);
	lua_setfield(m_L, -2, "__index");

	std::cout << blue << " [System] PushingGlobals" << endl;
	PushGlobal(m_rL, m_L, "printidentity");
	PushGlobal(m_rL, m_L, "game");
	PushGlobal(m_rL, m_L, "Game");
	PushGlobal(m_rL, m_L, "workspace");
	PushGlobal(m_rL, m_L, "Workspace");
	PushGlobal(m_rL, m_L, "Axes");
	PushGlobal(m_rL, m_L, "BrickColor");
	PushGlobal(m_rL, m_L, "CFrame");
	PushGlobal(m_rL, m_L, "Color3");
	PushGlobal(m_rL, m_L, "ColorSequence");
	PushGlobal(m_rL, m_L, "ColorSequenceKeypoint");
	PushGlobal(m_rL, m_L, "NumberRange");
	PushGlobal(m_rL, m_L, "NumberSequence");
	PushGlobal(m_rL, m_L, "NumberSequenceKeypoint");
	PushGlobal(m_rL, m_L, "PhysicalProperties");
	PushGlobal(m_rL, m_L, "Ray");
	PushGlobal(m_rL, m_L, "Rect");
	PushGlobal(m_rL, m_L, "Region3");
	PushGlobal(m_rL, m_L, "Region3int16");
	PushGlobal(m_rL, m_L, "TweenInfo");
	PushGlobal(m_rL, m_L, "UDim");
	PushGlobal(m_rL, m_L, "UDim2");
	PushGlobal(m_rL, m_L, "Vector2");
	PushGlobal(m_rL, m_L, "Vector2int16");
	PushGlobal(m_rL, m_L, "Vector3");
	PushGlobal(m_rL, m_L, "Vector3int16");
	PushGlobal(m_rL, m_L, "Enum");
	PushGlobal(m_rL, m_L, "Faces");
	PushGlobal(m_rL, m_L, "Instance");
	//PushGlobal(m_rL, m_L, "math");
	PushGlobal(m_rL, m_L, "warn");
	PushGlobal(m_rL, m_L, "typeof");
	PushGlobal(m_rL, m_L, "type");
	PushGlobal(m_rL, m_L, "spawn");
	PushGlobal(m_rL, m_L, "Spawn");
	PushGlobal(m_rL, m_L, "print");
	//PushGlobal(m_rL, m_L, "printidentity");
	PushGlobal(m_rL, m_L, "ypcall");
	PushGlobal(m_rL, m_L, "Wait");
	PushGlobal(m_rL, m_L, "wait");
	PushGlobal(m_rL, m_L, "delay");
	PushGlobal(m_rL, m_L, "Delay");
	PushGlobal(m_rL, m_L, "tick");
	PushGlobal(m_rL, m_L, "LoadLibrary");
	std::cout << " [System] Yoink Globals Wrapped | Please wait while we register your functions" << endl;
	lua_register(m_L, "loadstring", custom_loadstring);
	lua_register(m_L, "GetObjects", Custom_GetObjects);
	//lua_register(m_L, "DLS", httpgetimpl);
	//lua_register(m_L, "DLS", httpgetimpl);
	lua_register(m_L, "DLS", httpgetimpl);
	//lua_register(m_L, "Getobjects", Custom_GetObjects);
	//lua_register(m_L, "loadstring", custom_loadstring);
	lua_register(m_L, "WrapGlobal", Lua_WrapGlobal);
	lua_register(m_L, "GlobalsIndex", GlobalsIndex);

	lua_register(m_L, "EQ", EQ);
	lua_register(m_L, "GC", GC);

	lua_register(m_L, "MouseMove", MoveMouse);
	lua_register(m_L, "mousemoverel", MouseMoveRelative);
	lua_register(m_L, "MouseMoveRel", MouseMoveRelative);

	lua_register(m_L, "mousemoverelative", MouseMoveRelative);
	lua_register(m_L, "MouseMoveRelative", MouseMoveRelative);
	lua_register(m_L, "MouseScroll", MouseScroll);
	lua_register(m_L, "mousescroll", MouseScroll);

	lua_register(m_L, "MouseButton1Click", Mouse1Click);
	lua_register(m_L, "MouseButton1Press", Mouse1Down);
	lua_register(m_L, "MouseButton1Release", Mouse1Up);
	lua_register(m_L, "MouseButton2Click", Mouse2Click);
	lua_register(m_L, "MouseButton2Press", Mouse2Down);
	lua_register(m_L, "MouseButton2Release", Mouse2Up);
	lua_register(m_L, "MouseButton1Down", Mouse1Down);
	lua_register(m_L, "MouseButton1Up", Mouse1Up);
	lua_register(m_L, "MouseButton2Down", Mouse2Down);
	lua_register(m_L, "MouseButton2Up", Mouse2Up);

	lua_register(m_L, "getrawmetatable", getRawMetaTable);
	lua_register(m_L, "setrawmetatable", setrawmetatable_impl);

	lua_register(m_L, "DownloadString", httpgetimpl);

	lua_register(m_L, "copystring", fn_toclipboard);
	lua_register(m_L, "setclipboard", fn_toclipboard);
	lua_register(m_L, "custom_copyclipboard", fn_toclipboard);
	lua_register(m_L, "fn_toclipboard", fn_toclipboard);
	lua_register(m_L, "toclipboard", fn_toclipboard);
	lua_register(m_L, "toclipboard", fn_toclipboard);

	lua_register(m_L, "keypress", KeyPress);
	lua_register(m_L, "keyrelease", KeyRelease);
	lua_register(m_L, "keytap", KeyTap);
	lua_register(m_L, "KeyPress", KeyPress);
	lua_register(m_L, "KeyRelease", KeyRelease);
	lua_register(m_L, "KeyTap", KeyTap);

	lua_register(m_L, "checkcaller", custom_checkcaller);
	lua_register(m_L, "crash__", CrashRoblox);

	lua_register(m_L, "mousemoverel", MouseMoveRelative);
	lua_register(m_L, "MouseMoveRel", MouseMoveRelative);
	lua_register(m_L, "mousemoverelative", MouseMoveRelative);
	lua_register(m_L, "MouseMoveRelative", MouseMoveRelative);
	lua_register(m_L, "MouseButton1Click", Mouse1Click);
	lua_register(m_L, "MouseButton1Press", Mouse1Down);
	lua_register(m_L, "MouseButton1Release", Mouse1Up);
	lua_register(m_L, "MouseButton2Click", Mouse2Click);
	lua_register(m_L, "MouseButton2Press", Mouse2Down);
	lua_register(m_L, "MouseButton2Release", Mouse2Up);
	lua_register(m_L, "MouseButton1Down", Mouse1Down);
	lua_register(m_L, "MouseButton1Up", Mouse1Up);
	lua_register(m_L, "MouseButton2Down", Mouse2Down);
	lua_register(m_L, "MouseButton2Up", Mouse2Up);

	lua_register(m_L, "print", print);
	lua_register(m_L, "CreateConsole", createconsole);
	lua_register(m_L, "WriteLog", outputtoconsole);

	lua_register(m_L, "debug.setmetatable", setrawmetatable_impl);
	lua_register(m_L, "debug.getmetatable", getRawMetaTable);
	lua_register(m_L, "debug.getregistry", Debug_GetRegistry);
	lua_register(m_L, "debug.getupvalue", Debug_getupvalue);
	lua_register(m_L, "debug.setupvalue", Debug_setupvalue);
	/*lua_register(m_L, "debug.setlocal", Debug_setlocal);*/
	lua_register(m_L, "debug.getinfo", Debug_getinfo);
	lua_register(m_L, "debug.setfenv", Debug_setfenv);
	lua_register(m_L, "debug.traceback", Debug_traceback);;

	//lua_register(m_L, "getsenv", getsenv);
	lua_register(m_L, "getreg", getreg);
	lua_register(m_L, "getrenv", getrenv);
	lua_register(m_L, "getgenv", getgenv);

	lua_register(m_L, "setreadonly", setreadonly);

	//lua_register(m_L, "spawn", AntiAxonSpawn);
	//lua_register(m_L, "Spawn", AntiAxonSpawn);

	// bro something terrible happened. im going off. what happened bro

	r_lua_pop(m_rL, 2);
	lua_newtable(m_L);
	lua_setglobal(m_L, "_G");
	std::cout << blue << " [System] Yoink Executor Injected | By Immortal Donkey" << endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
	//luaL_dostring(m_L, "script=Instance.new(\"LocalScript\") script.Name = \"***Yoink***\"");
	luaL_dostring(m_L, "function setreadonly() end isreadonly = function() return false end makewritable = setreadonly setwritable = setreadonly ");
	luaL_dostring(m_L, "function GetObjects(assetId) local obj = game:GetService(\"InsertService\"):LoadLocalAsset(assetId) return { obj }; end");
	luaL_dostring(m_L, "function HttpGet(url, other) return DLS(url) end");
	luaL_dostring(m_L, DownloadURL("https://pastebin.com/raw/CenCCYqb").c_str());
	//luaL_dostring(m_L, "local gmt = getrawmetatable(game)\nlocal old = gmt.__index\n\ngmt.__index = function(self, key)\nif key == \"GetObjects\" or key == \"getobjects\" then\nreturn function(self, id) return GetObjects(id) end\nend\nif key == \"HttpGet\" or key == \"HttpGetAsync\" then\nreturn function(self, url, async) return DLS(url) end\nend\nreturn old(self, key)\nend");

	//luaL_dostring(m_L, "game.StarterGui:SetCore('SendNotification', {Duration = 4; Title='Yoink Executor'; Text='Injected'})");
	std::cout << blue << " [System] Closing in 3 seconds..." << endl;
	Sleep(2000);
	//luaL_dostring(m_L, "game.StarterGui:SetCore('SendNotification', {Duration = 4; Title='Yoink Executor'; Text='Ban Protection Enabled'})");
	Sleep(1000);
	::ShowWindow(GetConsoleWindow(), SW_HIDE);
}

unsigned int ProtectSections(HMODULE Module) {
	MODULEINFO ModuleInfo;
	GetModuleInformation(GetCurrentProcess(), Module, &ModuleInfo, sizeof(ModuleInfo));
	uintptr_t Address = reinterpret_cast<uintptr_t>(Module);
	uintptr_t TermAddress = Address + ModuleInfo.SizeOfImage;
	MEMORY_BASIC_INFORMATION MemoryInfo;


	while (Address < TermAddress) {
		VirtualQuery(reinterpret_cast<void*>(Address), &MemoryInfo, sizeof(MemoryInfo));
		if (MemoryInfo.State == MEM_COMMIT && (MemoryInfo.Protect & (PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))) {
			DWORD OldProtection;
			VirtualProtect(reinterpret_cast<void*>(Address), MemoryInfo.RegionSize, PAGE_EXECUTE_READ, &OldProtection);
		}
		Address = reinterpret_cast<uintptr_t>(MemoryInfo.BaseAddress) + MemoryInfo.RegionSize;
	}

	VirtualQuery(reinterpret_cast<void*>(MemoryInfo.AllocationBase), &MemoryInfo, sizeof(MemoryInfo));
	if (MemoryInfo.State != MEM_COMMIT || !(MemoryInfo.Protect & PAGE_EXECUTE_READ))
		return 0x400;
	return MemoryInfo.RegionSize - 0x400;
}
__forceinline void UnlinkModule(HINSTANCE Module
) {
	unsigned long PEB_DATA = 0;
	_asm {
		pushad;
		pushfd;
		mov eax, fs:[30h]
			mov eax, [eax + 0Ch]
			mov PEB_DATA, eax

			InLoadOrderModuleList :
		mov esi, [eax + 0Ch]
			mov edx, [eax + 10h]

			LoopInLoadOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 18h]
			cmp ecx, Module
			jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList

			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList

			InMemoryOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]

			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, Module
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList

			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList

			InInitializationOrderModuleList :
		mov eax, PEB_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]

			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, Module
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished

			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList

			Finished :
		popfd;
		popad;
	}
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	ProtectSections(Module);
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default: break;
	}

	return TRUE;
}
