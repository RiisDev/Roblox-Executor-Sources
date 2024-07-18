#include "Main.h"

#include "rlua.h"
extern "C" {
  #include "Lua/lua.hpp"
}

int mb1up(lua_State *Ls) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}
int mb1down(lua_State *Ls) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mb2up(lua_State *Ls) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}
int mb2down(lua_State *Ls) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int mb3up(lua_State *Ls) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
	SendInput(1, &input, sizeof(input));
	return 0;
}
int mb3down(lua_State *Ls) {
	INPUT input;
	input.type = INPUT_MOUSE;
	memset(&input.mi, 0, sizeof(MOUSEINPUT));
	input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
	SendInput(1, &input, sizeof(input));
	return 0;
}

int MB1C(lua_State *Ls) {
	mb1down(Ls);
	Sleep(10);
	mb1up(Ls);
	return 0;
}

int MB2C(lua_State *Ls) {
	mb2down(Ls);
	Sleep(10);
	mb2up(Ls);
	return 0;
}

int MB3C(lua_State *Ls) {
	mb3down(Ls);
	Sleep(10);
	mb3up(Ls);
	return 0;
}

int getreg(lua_State *Ls) {
	lua_pushvalue(L, LUA_REGISTRYINDEX);
  wrapper:PushRbx(Ls, -1);
	return 1;
}

int getgenv(lua_State *Ls) {
	lua_pushvalue(L, LUA_GLOBALSINDEX);
  wrapper:PushRbx(Ls, -1);
	return 1;
}

// austin dont kill me im sorry
int getfflag(lua_State *Ls) {
	getglobal(L, "GlobalSettings");
	getfield(L, -1, "GetFFlag");
	pushstring(L, lua_tostring(Ls, -1));
	pcall(L, 1, 1, 0);
	bool fflag_result = lua::toboolean(L, -1);
	lua_pushboolean(Ls, fflag_result);
	return 1;
}

int CopyString(lua_State *Ls) {
	const char* str = lua_tostring(Ls, -1);

	int len = strlen(str);
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len + 1);
	void *mem_ptr = GlobalLock(hMem);
	memcpy(mem_ptr, str, len + 1);
	GlobalUnlock(hMem);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
	return 0;
}

int geturl(lua_State *Ls) {
	string html = getURL(lua_tostring(Ls, -1));
	lua_pop(Ls, 1);
	lua_pushstring(Ls, html.c_str());
	return 0;
}


static int custom_movemouse(lua_State *Ls) {
	double x = lua_tonumber(Ls, -1);
	double y = lua_tonumber(Ls, -2);

	INPUT mouse;
	mouse.type = INPUT_MOUSE;
	memset(&mouse.mi, 0, sizeof(MOUSEINPUT));
	mouse.mi.dx = x;
	mouse.mi.dy = y;
	mouse.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &mouse, sizeof(INPUT));
	return 0;
}

int getscreenwidth(lua_State *Ls) {
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	lua_pushnumber(Ls, desktop.right);
	lua_pushnumber(Ls, desktop.bottom);
	return 0;
}
static int athenacaller(lua_State *Ls) {
	lua_pushboolean(Ls, true);
	return 1;
}

LUALIB_API int OpenCustomLibs(lua_State *Ls) {
lua_register(Ls, "MouseButton1Click", MB1C);
	lua_register(Ls, "MouseButton2Click", MB2C);
	lua_register(Ls, "MouseButton3Click", MB3C);
	lua_register(Ls, "MouseButton1Up", mb1up);
	lua_register(Ls, "MouseButton1Down", mb1down);
	lua_register(Ls, "MouseButton2Up", mb2up);
	lua_register(Ls, "MouseButton2Down", mb2down);
	lua_register(Ls, "MouseButton3Up", mb3up);
	lua_register(Ls, "MouseButton3Down", mb3down);
	lua_register(Ls, "runcmd", comandmeme);
	lua_register(Ls, "geturl", geturl);
	lua_register(Ls, "copystring", CopyString);
	lua_register(Ls, "setclipboard", CopyString);
	lua_register(Ls, "toclipboard", CopyString);
	lua_register(Ls, "movemouserel", custom_movemouse);

  lua_register(Ls, "getfflag", getfflag);
	lua_register(Ls, "checkcaller", athenacaller);
	lua_register(Ls, "is_athena_caller", athenacaller);
	lua_register(Ls, "getrawmetatable", DBG_getrawmetatable);
	lua_register(Ls, "setrawmetatable", DBG_setrawmetatable);
	lua_register(Ls, "setreadonly", sro);
	lua_register(Ls, "getgenv", getgenv);
	lua_register(Ls, "getreg", getreg);
	lua_register(Ls, "athenaexecute", loadstringMod);
}
