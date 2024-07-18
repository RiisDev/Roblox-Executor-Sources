#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include "HttpGet.h"
#include <stdio.h>
#include <synchapi.h>
#include <chrono>
#include <ctime>    
#include <tlhelp32.h>
#include "Drawing.h"
#include <iostream>
#include <string.h>
#include <Tlhelp32.h>
#include <queue>
#include <process.h>
#include "LuauSerializer.h"
#include "LuauTranspiler.h"
#include "Retcheck.h"
#include "CallCheck Bypass.h"
#include <winbase.h>

#include "Rlua.h"

extern "C" {
#include <iostream>
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lobject.h"
#include "Lua/llimits.h"
}
using namespace std;
const char* ExploitName = "Aqua LuaU";
std::string GetClassNaMee(DWORD Instance)
{
	if (Instance == 0)
	{
		BoundsCheckInstance();
		return **(string**)(*(DWORD*)(instance_ptr + 12) + 4);
	}
	return **(std::string**)(*(DWORD*)(Instance + 12) + 4);
}
std::string site = ("http://setup.roblox.com/version");
void TeleportHandler() {
	while (true)
	{
		if (getdatamodel2_addr != ReturnDataModel()) {
			Sleep(1);
		}
		Sleep(1);
	}
}
void VEH_Bypass() {
	HMODULE mod = GetModuleHandleA("ntdll.dll");
	DWORD old;
	DWORD KIException = cast(DWORD, GetProcAddress(mod, "KiUserExceptionDispatcher"));
	VirtualProtect((LPVOID)KIException, 1, PAGE_EXECUTE_READWRITE, &old);
	for (int i = 0; i < 24; i++) {
		*(char*)(KIException + i) = 0x90;
	}
	VirtualProtect((LPVOID)KIException, 1, old, &old);
}
bool IsNotNumber(int idx)
{
	TValue n;
	const TValue* o = index2adr(L, idx);
	return ttype(o) == LUA_TNUMBER || (((o) = VToNumber(o, &n)) != NULL);
}
void ConsoleBypass(const char* Name)
{
	DWORD AAAAAAAAAAAAAAA;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &AAAAAAAAAAAAAAA);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Name);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}
std::string Script;
bool ExecuteScript(DWORD RS, lua_State* L) {
	DWORD xD = r_lua_newthread(RS);
	std::string Bytecode = LuauTranspiler::compile(lua_open(), Script);
	std::string ExecuteStr;
	Script = replaceAll(Script, "game:HttpGet", "HttpGet");
	Script = replaceAll(Script, "game:HttpGetAsync", "HttpGet");
	Script = replaceAll(Script, "game:HttpGet", "game:HttpGet");
	Script = replaceAll(Script, "game:GetObjects", "GetObjects");
	Script = "spawn(function() script = Instance.new('LocalScript') script.Disabled = true; script.Parent = nil\r\n" + Script + "\r\nend)";
	//Script = replaceAll(Script, "game:HttpGet", "HttpGet");
	lua_getglobal(L, "string");
	lua_getfield(L, -1, "dump");
	if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@Aqua LuaU"))
	{
		const std::string ERR = lua_tostring(L, -1);
		lua_close(L);
		throw std::exception(ERR.c_str());
	}
	r_lua_deserialize(xD, ExploitName, LuauTranspiler::compile(lua_open(), Script).c_str(), LuauTranspiler::compile(lua_open(), Script).size());
	lua_pcall(L, 0, 0, 0);
	r_lua_spawn(xD);

	void* UD = lua_touserdata(L, 1);
	lua_pushvalue(L, 1);
	lua_gettable(L, LUA_REGISTRYINDEX);
	if (!lua_isnil(L, -1)) {
		int Reference = lua_tointeger(L, -1);
		r_lua_settop(RL, 0);
		return 1;
	}
	lua_pop(L, 1);
	return true;
}
template <class T>
static void write(void* lpTarget, T value)
{
	DWORD dwOldProtection;
	VirtualProtect(lpTarget, sizeof(T), PAGE_READWRITE, &dwOldProtection);
	*reinterpret_cast<T*>(lpTarget) = value;
	VirtualProtect(lpTarget, sizeof(T), dwOldProtection, &dwOldProtection);
}

union r_Value
{
	void* gc;
	void* p;
	double n;
	bool b;
};

typedef struct r_TValue
{
	r_Value value;
	byte tt;
	byte shit;
};

int impl_getgc(DWORD rL)
{
	auto IncludeTables = false;
	IncludeTables = r_lua_toboolean(rL, -1);
	const auto GlobalState = rL + 8 + *(DWORD*)(rL + 8); // fucking global state ofc
	const auto DeadMask = *(BYTE*)(GlobalState + 20);
	auto Object = *(GCObject**)(GlobalState + 48); // to do: define these you can remake rluaC_link with these 
	r_lua_createtable(rL, 0, 0);
	auto n = 1;
	while (Object != nullptr)
	{
		const auto TT = *(BYTE*)((DWORD)Object + 5);

		if ((TT == RLUA_TFUNCTION || (IncludeTables ? TT == RLUA_TTABLE : TT == RLUA_TFUNCTION) || (IncludeTables ? TT == RLUA_TUSERDATA : TT == RLUA_TFUNCTION)) && (*(BYTE*)((DWORD)Object + 4) & 3) & DeadMask)
		{
			r_lua_pushnumber(rL, n++);
			r_TValue* top = *(r_TValue**)(rL + 0xC);
			top->tt = RLUA_TFUNCTION;
			top->value.gc = (GCObject*)(TT);
			*reinterpret_cast<uintptr_t*>(rL + 0xC) += sizeof(r_TValue); // modified specially for it to WORK on fucking getgc, was this that was causing the problem the whole time
			r_lua_settable(rL, -3);
		}
		Object = Object->gch.next;
	}
	return 1;
}
std::string GetMachineID()
{
	std::string ss;
	ss = "Err_StringIsNu";
	UCHAR szFileSys[255],
		szVolNameBuff[255];
	DWORD dwSerial;
	DWORD dwMFL;
	DWORD dwSysFlags;
	int error = 0;

	bool success = GetVolumeInformation(LPCTSTR("C:\\"), (LPTSTR)szVolNameBuff,
		255, &dwSerial,
		&dwMFL, &dwSysFlags,
		(LPTSTR)szFileSys,
		255);
	if (!success) {
		ss = "Err_Not_Elevated";
	}
	std::stringstream errorStream;
	errorStream << dwSerial;
	return std::string(errorStream.str().c_str());
}
int RenderstepHook(DWORD rL)
{
	if (!ScriptQueue.empty()) {
		auto scripte = ScriptQueue.front();
		ExecuteScript(rL, L);
		ScriptQueue.pop();
	}
	return 1;
}
#include "Execution.h" // This is LuaC Send Script Support.
int RenderstepHookC(DWORD rL)
{
	if (!ScriptCQueue.empty()) {
		auto script = ScriptCQueue.front();
		ExecuteCScript(rL, L, script);
		ScriptCQueue.pop();
	}
	return 1;
}
INPUT Inputs[1] = { 0 };
void Execute(const std::string& Script) {
	if (Script != "") {
		ScriptQueue.push(Script);
	}

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
const auto haystack = std::string("haystack");
int exepipelmao() {
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;


	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\AquaWinning"), //This is your pipe name you can change it to whatever you want just make sure it has \\\\.\\pipe\\ at the beginning
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		9999999,
		9999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';

				if (strlen(buffer) != NULL) {
					ScriptQueue.push(buffer);
				}
			}
		}

		DisconnectNamedPipe(hPipe);
	}
}

int exepipelaunched() {
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;


	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\AquaLaunched"), //This is your pipe name you can change it to whatever you want just make sure it has \\\\.\\pipe\\ at the beginning
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		9999999,
		9999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';

				if (strlen(buffer) != NULL) {
					ScriptCQueue.push(buffer);
				}
			}
		}

		DisconnectNamedPipe(hPipe);
	}
}

void SetLevel(DWORD RobloxState, int Level)
{
	int v3 = *(DWORD*)(RobloxState + EmptySpace1);
	*(DWORD*)(v3 + EmptySpace2) = Level;
}

DWORD SetThreadIdentity(DWORD rL, int level)
{
	return *(DWORD*)(*(DWORD*)(rL + Identity2) + 24) = level;
}

int mouse1press_impl(uintptr_t ls) {
	INPUT Inputs[1] = { 0 };
	Inputs->type = INPUT_MOUSE;
	memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}

int mouse1release_impl(uintptr_t ls) {
	INPUT Inputs[1] = { 0 };
	Inputs->type = INPUT_MOUSE;
	memset(&Inputs->mi, 0, sizeof(MOUSEINPUT));
	Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
	SendInput(1, Inputs, sizeof(INPUT));
	return 0;
}
const char* Versionlink = "https://raw.githubusercontent.com/SpeedSterKawaii/AquaAPI/main/Version.txt";
#define log_addy_start 0xB838D
int WINAPI h_MessageBox(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCTSTR lpText,
	_In_opt_ LPCTSTR lpCaption,
	_In_     UINT    uType
) {
	ConsoleBypass("Roblox Crashed [Aqua Softworks, SpeedSterKawaii]");
	printf("[Aqua]: Roblox has Unexpectedly Crashed, No Crash Logs are Being Sent.\n");
	if(!getdatamodel_addr)
	{
		printf("[Aqua]: Aqua Softworks isn't updated, please wait for an update.\n");
	}
	else
	{
		printf("[Aqua]: Aqua Softworks Script Queue has Unexpectedly Stopped Working.\n");
		printf("[Aqua]: Press any character/key to exit the console and Roblox-Player.\n");
		getchar();
		exit(0);
	}
	std::string str_lp = "";
	std::string str_cap = "";
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	if (str_cap == "Roblox Crash") {
		DWORD log_start = log_addy_start + (DWORD)GetModuleHandleA(NULL);
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
		DWORD old;
		for (int i = 0; i < 79; i++) {
			VirtualProtect((LPVOID)(log_start + i), 1, PAGE_EXECUTE_READWRITE, &old);
			*(char*)(log_start + i) = 0x90;
			VirtualProtect((LPVOID)(log_start + i), 1, old, &old);
		}
		str_lp = "Crashed oh shit";
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
	}

	wchar_t* str_lp_res = new wchar_t[4096];
	wchar_t* str_cap_res = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, NULL, str_lp.c_str(), -1, str_lp_res, 4096);
	MultiByteToWideChar(CP_ACP, NULL, str_cap.c_str(), -1, str_cap_res, 4096);
	return MessageBoxW(hWnd, str_lp_res, str_cap_res, uType);
}
void CheckHook() {
	DWORD o;
	VirtualProtect((LPVOID)&MessageBoxA, 1, PAGE_EXECUTE_READWRITE, &o);
	*(char*)(&MessageBoxA) = 0xE9;
	*(DWORD*)((DWORD)&MessageBoxA + 1) = ((DWORD)&h_MessageBox - (DWORD)&MessageBoxA) - 5;
	VirtualProtect((LPVOID)&MessageBoxA, 1, o, &o);
}
#pragma once
#include <string>
#include <algorithm>

std::string RandomString(unsigned int length)
{
	auto RandomCharacters = []() -> char
	{
		const char Characters[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t MaximumLength = (sizeof(Characters) - 1);
		return Characters[rand() % MaximumLength];
	};
	std::string EnryptedString(length, 0);
	std::generate_n(EnryptedString.begin(), length, RandomCharacters);
	return EnryptedString;
}

#define RAND_STR(l) RandomString(l)

static std::string base64_decode(const std::string& in) {
	std::string out;

	std::vector<int> T(256, -1);
	for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

	int val = 0, valb = -8;
	for (unsigned char c : in) {
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
	}
	return out;
}
static int Panic(lua_State* Thread) {
	luaL_error(Thread, "PANIC: unprotected error in call to Lua API. (%s)\n", lua_tostring(Thread, -1));
	throw;
	return 0;
}
int checkcaller_impl(uintptr_t ls) {
	r_lua_pushboolean(ls, TRUE);
	return 1;
}
static int GarbageCollector(DWORD rL)  {
	void* UD = lua_touserdata(L, 1);
	lua_pushvalue(L, 1);
	lua_gettable(L, LUA_REGISTRYINDEX);
	if (!lua_isnil(L, -1)) {
		int Reference = lua_tointeger(L, -1);
		r_lua_settop(RL, 0);
		return 1;
	}
	lua_pop(L, 1);
	return 0;
}

#include <Windows.h>
#include <string>
#include <WinInet.h>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "wininet.lib")
static int GetClipboard(lua_State* LS) {
	if (!OpenClipboard(nullptr)) {
		lua_pushstring(LS, "");
		return 0;
	}
	auto ClipBoardText = GetClipboardData(CF_TEXT);
	lua_pushstring(LS, reinterpret_cast<char*>(ClipBoardText));
	CloseClipboard();
	return 1;
}
BOOL IsPC64()
{
	BOOL b_is_wow64 = FALSE;

	typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fn_is_wow64_process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fn_is_wow64_process)
	{
		if (!fn_is_wow64_process(GetCurrentProcess(), &b_is_wow64))
		{
			return FALSE;
		}
	}
	return b_is_wow64;
}
const int MAX = 26;
const int AX = 26;
string randomnunber(int n)
{
	char alphabet[AX] = { '1', '2', '3', '4', '5', '6', '7',
						  '8', '9' };

	string res = "";
	for (int i = 0; i < n; i++)
		res = res + alphabet[rand() % MAX];

	return res;
}
string printRandomString(int n)
{
	char alphabet[MAX] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g',
						  'h', 'i', 'j', 'k', 'l', 'm', 'n',
						  'o', 'p', 'q', 'r', 's', 't', 'u',
						  'v', 'w', 'x', 'y', 'z' };

	string res = "";
	for (int i = 0; i < n; i++)
		res = res + alphabet[rand() % MAX];

	return res;
}
static int getreg_impl(uintptr_t rL) {
	r_lua_pushvalue(rL, -10000);
	return 1;
}

static int getenv_impl(uintptr_t rL) {
	r_lua_pushvalue(rL, -10002);
	return 1;
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

static int GlobalsIndex(lua_State* Thread) {
	try {
		int RThread = r_lua_newthread(RL);
		if (!RThread) return luaL_error(Thread, "Roblox's Thread Was Incorrect");
		if (lua_type(Thread, 2) != LUA_TSTRING) return luaL_error(Thread, "Argument 2 Expected String, Got %s", lua_typename(Thread, lua_type(Thread, 1)));
		r_lua_getglobal(RThread, lua_tostring(Thread, 2));
		printf("Wrapping Cached Global: %s", lua_tostring(Thread, 2));
	}
	catch (exception e) {
		return luaL_error(Thread, "%s", e.what());
	}
	catch (...) {
		return luaL_error(Thread, "C++ exception thrown");
	}
	return 0;
}

int getgc_impl(uintptr_t rL)
{
	GCObject* Obj = *(GCObject**)((*(DWORD*)(rL + 16) - (rL + 16)) + 32);
	r_lua_createtable(rL, 0, 0);

	for (int i = 0; Obj != nullptr; i++)
	{
		if ((*(BYTE*)((DWORD)Obj + 4)) == 6 || (*(BYTE*)((DWORD)Obj + 4)) == 7 || (*(BYTE*)((DWORD)Obj + 4)) == 8)
		{
			r_lua_pushnumber(rL, i);
			*(uintptr_t*)((rL)+24) += sizeof(r_TValue);
			r_lua_settable(rL, -3);
		}
		Obj = Obj->gch.next;
	}
	return 1;
}

int isreadonly_impl(uintptr_t rL) {
	return 1;
	r_lua_settop(rL, 0);
	return 0;
}

	int mousemovalbetonmumber(uintptr_t ls) {
		DWORD x = r_lua_tonumber(ls, -1, 0);
		DWORD y = r_lua_tonumber(ls, -2, 0);

		HWND activewindow = GetForegroundWindow();
		DWORD x2 = GetSystemMetrics(SM_CXSCREEN);
		DWORD y2 = GetSystemMetrics(SM_CYSCREEN);

		RECT s;
		GetClientRect(activewindow, &s);
		POINT hit = { s.left, s.top };
		ClientToScreen(activewindow, &hit);

		x2 = x2 - 1;
		y2 = y2 - 1;
		x = (0xFFFF / x2) * (x + hit.x);
		y = (0xFFFF / y2) * (y + hit.y);

		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, NULL, NULL);

		return 0;
	}

	static int setclipboard(DWORD rL) {
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
std::string protectfilename(const char* filename) {
	std::string workstr = filename;

	for (int idx = 0; idx < workstr.length(); idx++) {
		if (!isalnum(workstr[idx]) && workstr[idx] != '-' && workstr[idx] != '_' && workstr[idx] != '.' && workstr[idx] != ' ' && workstr[idx] != '\0') {
			workstr[idx] = '_';
		}
	}

	return workstr;
}
int setreadonly_impl(DWORD rL) {
	r_lua_setreadonly(rL, 1, r_lua_toboolean(rL, 2));
	r_lua_settop(rL, 0);
	return 1;
}

int getObjects_impl(uintptr_t ls) {
	const char* str = r_lua_tolstring(ls, -1, 0);
	r_lua_settop(ls, 0);
	r_lua_getfield(ls, -10002, "game");
	r_lua_getfield(ls, -1, "GetService");
	r_lua_pushvalue(ls, -2);
	r_lua_pushstring(ls, "InsertService");
	r_lua_pcall(ls, 2, 1, 0);
	r_lua_getfield(ls, -1, "LoadLocalAsset");
	r_lua_pushvalue(ls, -2);
	r_lua_pushstring(ls, str);
	r_lua_pcall(ls, 2, 1, 0);
	r_lua_createtable(ls, 0, 0);
	r_lua_pushvalue(ls, -2);
	return 1;
}
void RegisterAquaShit(lua_State* L) {
	lua_register(L, "setclipboard", CopyString);
	lua_register(L, "getclipboard", GetClipboard);
	//lua_register(L, "httpget", HttpGet);
}
void CheckPC()
{
	if (!IsPC64)
	{
		printf("[Aqua]: Windows 7 isn't supported, we are sorry. \n");
		exit(0);
	}
	else
	{
	}
}
using namespace std;
void CheckGame(int rL) {
	r_lua_getglobal(rL, "game");
	r_lua_getfield(rL, -1, "PlaceId");
	double place_id = r_lua_tonumber(NULL, rL, 0);

	if (place_id == 155615604)
	{
		Execute("game.StarterGui:SetCore('SendNotification', {Duration = 10; Title='Prison Life'; Text='Prison Life has Been Detected.'; Icon = 'rbxassetid://6666666'})");
	}

	else if (place_id == 1600503)
	{
	}

	else if (place_id == 192800)
	{
	}

	else if (place_id == 221813725) {
	}
}
int hookfunction(DWORD rL) {
	if (lua_gettop(L) != 2) {
		luaL_error(L, "Aqua hookfunction requires 2 arguments");
		return 0;
	}
	else
	{
		TValue* frstfunc = index2adr(L, 1);
		TValue* secondfunc = index2adr(L, 2);
		frstfunc->value.gc->cl = secondfunc->value.gc->cl;
		return 1;
	}
}
int GetObjects(DWORD rL) {
	const char* str = r_lua_tolstring(rL, -1, 0);
	r_lua_settop(rL, 0);
	r_lua_getfield(rL, -10002, "game");
	r_lua_getfield(rL, -1, "GetService");
	r_lua_pushvalue(rL, -2);
	r_lua_pushstring(rL, "InsertService");
	r_lua_pcall(rL, 2, 1, 0);
	r_lua_getfield(rL, -1, "LoadLocalAsset");
	r_lua_pushvalue(rL, -2);
	r_lua_pushstring(rL, str);
	r_lua_pcall(rL, 2, 1, 0);
	r_lua_createtable(rL, 0, 0);
	r_lua_pushvalue(rL, -2);
	r_lua_rawseti(rL, -2, 1);
	return 1;
}
int getnamecallmethod(lua_State* L)
{

	DWORD namecall_method = *(DWORD*)(RL + 104);

	if (namecall_method) {
		StkId top = *(StkId*)(RL + RL);
		top->tt = R_LUA_TSTRING;
		top->value.gc = (GCObject*)namecall_method;
		*(DWORD*)(RL + RL) += sizeof(StkId);
	}
	return 1;
}

int FireClickDetector(DWORD rL)
{
	int CDectector = *(DWORD*)r_lua_touserdata(rL, 1);

	float Dist = 0.0;
	if (r_lua_gettop(rL) > 1)
		Dist = (float)r_lua_tonumber(rL, 2, 0);

	r_lua_getfield(rL, -10002, "game");
	r_lua_getfield(rL, -1, "GetService");
	r_lua_pushvalue(rL, -2);
	r_lua_pushstring(rL, "Players");
	r_lua_pcall(rL, 2, 1, 0);
	r_lua_getfield(rL, -1, "LocalPlayer");



	DWORD Plr = *(DWORD*)r_lua_touserdata(rL, -1);
	static DWORD FnFire = ASLR(Adresses::fireclickdetector_addr);

	((void(fireclickdetector_CCV*)(DWORD, float, DWORD))FnFire)(CDectector, Dist, Plr);


	return 0;
}
int HttpGet(DWORD rL)
{
	size_t CSize;
	const auto CStr = rluaL_checklstring(rL, 1, &CSize);
	const auto Url = std::string(CStr, CSize);
	const auto Script = DownloadURL(Url.c_str());
	r_lua_pushstring(rL, Script.c_str());
	return 1;
}
int Loadstring(DWORD rL)
{
	size_t ScriptLength;
	std::string Source = rluaL_checklstring(rL, 1, &ScriptLength);
	Execute(Source);
	return 1;
}
int AQUA_LOADED(lua_State* L) {
	lua_pushboolean(L, true);
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

int LuaU_decompile(lua_State*L)
{
	string x;
	int i = 0;

	for (i = 1; i <= 6; i++)
	{
		x.append(randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + randomstringg2(2) + " " + "\n");
	}

	lua_pushstring(L, x.c_str());
	return 1;
}
std::string GetDLLPath(const char* Addon)
{
	HMODULE hModule = GetModuleHandleA("AquaBytecode.dll");
	if (hModule == NULL)
		return "";
	char buffer[MAX_PATH + 1];
	GetModuleFileNameA(hModule, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos).append(Addon);
}
std::function<void(uintptr_t, int, int)> r_luaL_checktype;
int isreadonly_impl(DWORD rL) {
	r(rL, 1, R_LUA_TTABLE);
	const auto Table = r_lua_topointer(rL, -1);
	r_lua_pushboolean(rL, *(BYTE*)(Table + 8));
	return 1;
	r_lua_settop(rL, 0);
	return 0;
}
typedef r_TValue* r_StkId;
int getrawmetatable_impl(DWORD ls) {
	if (r_lua_type(ls, -1) == R_LUA_TUSERDATA || r_lua_type(ls, -1) == R_LUA_TSTRING || r_lua_type(ls, -1) == R_LUA_TTABLE) {
		r_lua_getmetatable(ls, -1);
		return 1;
	}
	return 0;
}
int getnamecallmethod_impl(DWORD rL) {
	r_StkId& topstack = *(r_StkId*)(rL + 24);
	topstack->tt = 5;
	topstack->value.p = (r_TValue*)(*(uintptr_t*)(rL + 100));
	*(uintptr_t*)((rL)+12) += sizeof(r_TValue);
	return 1;
}
static int setmetatable_impl(DWORD rL) {
	int t = lua_type(L, 2);
	luaL_argcheck(L, t == LUA_TNIL || t == LUA_TTABLE, 2,
		"nil or table expected");
	lua_settop(L, 2);
	lua_pushboolean(L, lua_setmetatable(L, 1));
	return 1;
}
int checkcaller_impl(DWORD rL ) {
	r_lua_pushboolean(rL, TRUE);
	return 1;
}
void DeepPainFuckYou(int rL) {
	r_lua_getglobal(rL, "game");
	r_lua_getfield(rL, -1, "PlaceId");
	double place_id = r_lua_tonumber(NULL, rL, 0);

	if (place_id == 155615604)
	{
		Execute("game.StarterGui:SetCore('SendNotification', {Duration = 10; Title='Prison Life'; Text='Prison Life has Been Detected.'; Icon = 'rbxassetid://6666666'})");
	}

	else if (place_id == 1600503)
	{
	}

	else if (place_id == 192800)
	{
	}

	else if (place_id == 221813725) {
	}
}
int setnamecallmethod_impl(DWORD rL) {
	*(DWORD*)(rL + 100) = (uintptr_t)index2adr(L, 1)->value.gc;
	return 1;
}
int SetFFlag(DWORD rL)
{
	const std::string FlagName = lua_tostring(L, 1);
	const std::string SetTo = lua_tostring(L, 2);
	return 0;
}
static int dumpstring(DWORD rL)
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
	return 1;
}
int newcclosure(DWORD rL) {
	if (!lua_iscfunction(L, -1))
	{
		std::cout << "Aqua expected function as argument #1 \n";
		return luaL_argerror(L, -1, "Aqua expected function as argument #1");
	}
	r_lua_pushvalue(RL, 1);
	return 1;
}

int newcclosure_handler() {
	int args = r_lua_gettop(RL);
	r_lua_pushvalue(RL, lua_upvalueindex(1));
	for (int i = 1; i <= args; i++)
		r_lua_pushvalue(RL, i);
	int ah = r_lua_pcall(RL, args, LUA_MULTRET, 0);
	if (ah) {
		return 0;
	}
	return r_lua_gettop(RL) - args;
}
void CheckWhiteList()
{
	DWORD max = 30;
	DWORD DeepPainGoKys;
	DWORD DeepPainIsIDANerd;
	GetVolumeInformation(NULL, NULL, NULL, &DeepPainGoKys, &max, NULL, NULL, NULL);
	if (DeepPainGoKys == 2389665645)
	{
		exit(0);
	}
	else
	{
	}
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
int getconnections(DWORD rL) {

	if (!lua_isuserdata(L, 1) || !checksignal(L, 1))
		return luaL_argerror(L, 1, "signal expected");
	const auto EventInstance = *reinterpret_cast<std::uintptr_t*>(lua_touserdata(L, 1));
	const auto Source = reinterpret_cast<std::weak_ptr<uintptr_t>*>(EventInstance + 4)->lock();
	const auto Signal = reinterpret_cast<uintptr_t(__thiscall*)(uintptr_t*, bool)>(*reinterpret_cast<std::uintptr_t*>(EventInstance + 48))(Source.get(), true);
	return 1;
}
std::string PipeName = ("AquaBytecode");
void CheckInjection()
{
	if (FindWindowW(L"Roblox", NULL) || GetModuleHandleA("EasyExploits.dll") || GetModuleHandleA("krnl.dll") || GetModuleHandleA("csapi.dll") || GetModuleHandleA("exploit-main.dll"))
	{
		while (true)
		{
			Execute("loadstring(game:HttpGet(('https://raw.githubusercontent.com/RandomAdamYT/DarkHub/master/Init'), true))()"); // I would just fuck up the execution.
		}
		MessageBoxA(NULL, "EasyExploits has been Injected, Roblox needs to be Closed.", "EasyExploits Alert.", NULL);
		exit(0);
	}
	else
	{
	}
}

const char* Aqua_TypeName(int t)
{
	switch (t) {
	case R_LUA_TNONE:
		return "no value";
	case R_LUA_TNIL:
		return "nil";
	case R_LUA_TUSERDATA:
		return "touserdata";
	case R_LUA_TLIGHTUSERDATA:
		return "userdata";
	case R_LUA_TNUMBER:
		return "number";
	case R_LUA_TBOOLEAN:
		return "boolean";
	case R_LUA_TSTRING:
		return "string";
	case R_LUA_TTHREAD:
		return "thread";
	case R_LUA_TFUNCTION:
		return "function";
	case R_LUA_TTABLE:
		return "table";
	}
}
void CheckGame()
{
	lua_getglobal(L, "game:IsLoaded");
}
int crash_aqua(DWORD rL)
{
	if (lua_type(L, 1) == R_LUA_TSTRING)
		MessageBoxA(NULL, lua_tostring(L,1), "Aqua Softworks", MB_OK);

	((DWORD(__cdecl*)())nullptr)();
	return 0;
}
int getsenv(DWORD rL)
{
	DWORD LS = 0;

	return 1;
}
int extractisgay(DWORD rL)
{
	lua_pushboolean(L, 1);
	return 0;
}
int AquaWinning(DWORD rL) {
	MessageBoxA(NULL, "Aqua Message Box.", "Aqua Message", MB_TOPMOST);
	return 0;
}
#define LockTable() *(BYTE*)(lua_topointer(-1) + RT_LOCKED) = TRUE
bool checkinstance(DWORD RL, int Index)
{
	lua_getglobal(L, "typeof");
	lua_pushvalue(L, Index);
	lua_pcall(L, 1, 1, 0);
	const auto Check = lua_tostring(L, -1);
	lua_pop(L, 1);
	return !strcmp(Check, "Instance");
}
void Thread() {
	MessageBoxA(NULL, "We are sorry for the inconvenience, but aqua is currently patched. Please DM SpeedSterKawaiiツ#5233 for more Information. Thank you and goodbye.", "Aqua has patched", NULL);
	exit(0);
	auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	ConsoleBypass("[+] Aqua Bytecode - Powered by Imperious Transpiler [+]");
	std::cout << R"(
     
                               _____        __ _                      _        
     /\                       / ____|      / _| |                    | |       
    /  \   __ _ _   _  __ _  | (___   ___ | |_| |___      _____  _ __| | _____ 
   / /\ \ / _` | | | |/ _` |  \___ \ / _ \|  _| __\ \ /\ / / _ \| '__| |/ / __|
  / ____ \ (_| | |_| | (_| |  ____) | (_) | | | |_ \ V  V / (_) | |  |   <\__ \
 /_/    \_\__, |\__,_|\__,_| |_____/ \___/|_|  \__| \_/\_/ \___/|_|  |_|\_\___/
             | |                                                               
             |_|                                                               
    )";
	printf(" \n");
	cout << "[Aqua]: Get_Data_Model " << getdatamodel_addr << "." << endl;
	cout << "[Aqua]: Script_Context " << scriptcontext << "." <<  endl;
	cout << "[Aqua]: Injected_Data " << elapsed_seconds.count() << "." << endl;
	printf("[Aqua]: Please wait a moment. \n");
	printf("[Aqua]: Checking Init Hook. \n");
	CheckHook();
	printf("[Aqua]: Checking Computer. \n");

	CheckPC();
	DWORD DataModel = ReturnDataModel();
	DWORD ScriptContext = FindChildOfInstance(DataModel, "ScriptContext");
	RL = r_lua_newthread(RBX_LuaState(ScriptContext));
	DWORD Thread = r_lua_newthread(RL);
	L = luaL_newstate();
	luaL_openlibs(L);
	printf("[Aqua]: Setting Identity.\n");
	SetThreadIdentity(RL, 7);
	printf("[Aqua]: Identity Set to 7.\n");
	printf("[Aqua]: Bypassing Call-Check. \n");
	BypassCallCheck();
	printf("[Aqua]: Call-Check Bypassed. \n");
	printf("[Aqua]: Registering Functions.\n");
	r_luaL_register(RL, "getgc", impl_getgc);
	r_luaL_register(RL, "HttpGet", HttpGet);
	r_luaL_register(RL, "GetObjects", GetObjects);
	r_luaL_register(RL, "loadstring", Loadstring);
	r_luaL_register(RL, "AQUA_LOADED", IS_AQUA);
	r_luaL_register(RL, "hookfunction", hookfunction);
	r_luaL_register(RL, "fireclickdetector", FireClickDetector);
	r_luaL_register(RL, "isreadonly", isreadonly_impl);
	r_luaL_register(RL, "setreadonly", setreadonly_impl);
	r_luaL_register(RL, "getrawmetatable", getrawmetatable_impl);
	r_luaL_register(RL, "getnamecallmethod", getnamecallmethod_impl);
	r_luaL_register(RL, "garbagecollector", GarbageCollector);
	r_luaL_register(RL, "setrawmetatable", setmetatable_impl);
	r_luaL_register(RL, "checkcaller", checkcaller_impl);
	r_luaL_register(RL, "setnamecallmethod", setnamecallmethod_impl);
	r_luaL_register(RL, "dumpstring", dumpstring);
	r_luaL_register(RL, "newcclosure", newcclosure);
	r_luaL_register(RL, "setfflag", SetFFlag);
	r_luaL_register(RL, "getsenv", getsenv);
	r_luaL_register(RL, "getconnections", getconnections);
	r_luaL_register(RL, "aquawinning", AquaWinning);
	r_luaL_register(RL, "setclipboard", setclipboard);
	r_luaL_register(RL, "extractisgay", extractisgay);
	printf("[Aqua]: Functions Registered.\n");
	r_lua_getfield(RL, LUA_GLOBALSINDEX, "game");
	r_lua_getfield(RL, -1, "GetService");
	r_lua_pushvalue(RL, -2);
	r_lua_pushstring(RL, "RunService");
	r_lua_pcall(RL, 2, 1, 0);
	r_lua_getfield(RL, -1, "RenderStepped");
	r_lua_getfield(RL, -1, "Connect");
	r_lua_pushvalue(RL, -2);
	r_lua_pushcclosure(RL, (DWORD)RenderstepHook, 0);
	r_lua_pcall(RL, 2, 0, 0);
	r_lua_pop(RL, 2);
	getObjects_impl(RL);
	printf("[Aqua]: Configuring DrawingAPI. \n");
	printf("[Aqua]: DrawingAPI Configured.\n");
	printf("[Aqua]: Aqua Softworks Injected.\n");
	Sleep(900);
	CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(exepipelmao), NULL, NULL, NULL);
	//CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(exepipelaunched), NULL, NULL, NULL);
	//::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	//CheckWhiteList();
}
BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved) {
	if (Reason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Thread), NULL, NULL, NULL);
	}
	return TRUE;
}

//Execute("local aa=Instance.new('ScreenGui')local ba=Instance.new('ImageLabel') local ca=Instance.new('TextLabel')local da=Instance.new('TextLabel') local _b=Instance.new('TextLabel')aa.Name='Aqua'aa.Parent=game.CoreGui aa.ZIndexBehavior=Enum.ZIndexBehavior.Sibling;ba.Name='Icon'ba.Parent=aa ba.BackgroundColor3=Color3.fromRGB(255,255,255)ba.BackgroundTransparency=1.000 ba.Position=UDim2.new(0.438,0,0.415,0)ba.Size=UDim2.new(0,158,0,159) ba.Image='rbxassetid://6435705350'ba.ScaleType=Enum.ScaleType.Fit;ba.Active=true;ca.Name='AquaLabel' ca.Parent=ba;ca.BackgroundColor3=Color3.fromRGB(255,255,255) ca.BackgroundTransparency=1.000;ca.Position=UDim2.new(0.438,0,0.415,0) ca.Size=UDim2.new(0,200,0,50)ca.Font=Enum.Font.GothamBold;ca.Text='Aqua' ca.TextColor3=Color3.fromRGB(39, 39, 39)ca.TextScaled=true;ca.TextSize=14.000 ca.TextStrokeColor3=Color3.fromRGB(255,255,255)ca.TextStrokeTransparency=0.000;ca.TextWrapped=true;ca.Visible=false da.Name='Softworks'da.Parent=ba;da.BackgroundColor3=Color3.fromRGB(255,255,255) da.BackgroundTransparency=1.000;da.Position=UDim2.new(0.438,0,0.415,0) da.Size=UDim2.new(0,200,0,50)da.Font=Enum.Font.GothamBold;da.Text='Softworks' da.TextColor3=Color3.fromRGB(39, 39, 39)da.TextScaled=true;da.TextSize=14.000 da.TextStrokeColor3=Color3.fromRGB(255,255,255)da.TextStrokeTransparency=0.000;da.TextWrapped=true;da.Visible=false _b.Name='CreditsLabel'_b.Parent=ba;_b.BackgroundColor3=Color3.fromRGB(255,255,255)_b.BackgroundTransparency=1.000 _b.Position=UDim2.new(-0.132911578,0,-2.40880513,0)_b.Size=UDim2.new(0,395,0,25)_b.Font=Enum.Font.GothamBold _b.Text=''_b.TextColor3=Color3.fromRGB(39, 39, 39)_b.TextScaled=true _b.TextSize=14.000;_b.TextStrokeColor3=Color3.fromRGB(255,255,255) _b.TextStrokeTransparency=0.000;_b.TextWrapped=true;_b.Visible=false ba:TweenPosition(UDim2.new(0.361,0,0.415,0),'Out','Quad',0.5,true)wait(1)ca.Visible=true ca:TweenPosition(UDim2.new(1,0,0.138,0),'Out','Quad',0.5,true)wait(1)da.Visible=true da:TweenPosition(UDim2.new(1,0,0.453,0),'Out','Quad',0.5,true)wait(3)_b.Visible=true;wait(1)da.Visible=false;ca.Visible=false _b.Visible=false ba:TweenPosition(UDim2.new(0.438,0,0.415,0),'Out','Quad',0.5,true)wait(1) ba:TweenPosition(UDim2.new(0,0,1.5,0),'In','Quad',0.3,true)wait(1)aa:Destroy()");
