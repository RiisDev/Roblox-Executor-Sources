#pragma once
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include <thread>
#include <fstream>
#include <filesystem>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <chrono>
#include <string>
#include <regex>
#include <cmath>
#include "globals.h"
#include "Bridge.h"
#include "Lua/lapi.h"
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
#pragma comment(lib, "wininet.lib")
#include "r_lua.h"
#include <tchar.h>

namespace Keyboard {
	INPUT Inputs[1] = { 0 };

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

	void keyboardlib(lua_State* LS) {
		Inputs[0].type = INPUT_KEYBOARD;

		lua_register(LS, "keypress", KeyPress);
		lua_register(LS, "keyrelease", KeyRelease);
		lua_register(LS, "keytap", KeyTap);

		lua_register(LS, "KeyPress", KeyPress);
		lua_register(LS, "KeyRelease", KeyRelease);
		lua_register(LS, "KeyTap", KeyTap);
	}
}

namespace Mouse {
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

	void mouselib(lua_State* LS) {
		Inputs[0].type = INPUT_MOUSE;
		lua_register(LS, "mousemoverel", MouseMoveRelative);
		lua_register(LS, "MouseMoveRel", MouseMoveRelative);
		lua_register(LS, "mousemoverelative", MouseMoveRelative);
		lua_register(LS, "MouseMoveRelative", MouseMoveRelative);

		lua_register(LS, "MouseScroll", MouseScroll);
		lua_register(LS, "mousescroll", MouseScroll);

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
	}
}
