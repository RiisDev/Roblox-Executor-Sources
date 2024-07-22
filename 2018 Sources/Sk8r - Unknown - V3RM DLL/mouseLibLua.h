#pragma once
#include <windows.h>
#include "Lua\lua.hpp"

namespace mouse_funcs {

	INPUT Inputs[1] = { 0 };

	int leftButtonDown(lua_State *L) {
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int leftButtonUp(lua_State *L) {
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int rightButtonDown(lua_State *L) {
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int rightButtonUp(lua_State *L) {
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int leftButtonPress(lua_State *L) {
		float n = lua_tonumber(L, -1);
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
		SendInput(1, Inputs, sizeof(INPUT));
		Sleep(n);
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int rightButtonPress(lua_State *L) {
		float n = lua_tonumber(L, -1);
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
		SendInput(1, Inputs, sizeof(INPUT));
		Sleep(n);
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
		SendInput(1, Inputs, sizeof(INPUT));
		return 0;
	}

	int mouseScroll(lua_State *L) {
		int amount = lua_tonumber(L, -1);
		Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_HWHEEL;
		Inputs[0].mi.mouseData = amount;
		SendInput(1, Inputs, sizeof(INPUT));
		Inputs[0].mi.mouseData = NULL;
		return 0;
	}

	int moveMouseRelative(lua_State *L) {
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
	void setup(lua_State *L) {

		Inputs[0].type = INPUT_MOUSE;
		lua_register(L, "leftpress", leftButtonDown);
		lua_register(L, "leftrelease", leftButtonUp);
		lua_register(L, "rightpress", rightButtonDown);
		lua_register(L, "rightRelease", rightButtonUp);
		lua_register(L, "leftclick", leftButtonPress);
		lua_register(L, "rightclick", rightButtonPress);
		lua_register(L, "mousescroll", mouseScroll);
		lua_register(L, "mousemoverel", moveMouseRelative);
	}

}

namespace keyboard_funcs {

	INPUT Inputs[1] = { 0 };

	int keyPress(lua_State *L) {
		WORD key = lua_tonumber(L, -1);
		Inputs[0].ki.wVk = key;
		SendInput(1, Inputs, sizeof(INPUT));
		Inputs[0].ki.wVk = NULL;
		return 0;
	}

	int keyRelease(lua_State *L) {
		WORD key = lua_tonumber(L, -1);
		Inputs[0].ki.wVk = key;
		Inputs[0].mi.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, Inputs, sizeof(INPUT));
		Inputs[0].mi.dwFlags = NULL;
		Inputs[0].ki.wVk = NULL;
		return 0;
	}

	int keyTap(lua_State *L) {
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

	void setup(lua_State *L) {
		Inputs[0].type = INPUT_KEYBOARD;
		lua_register(L, "keypress", keyPress);
		lua_register(L, "keyrelease", keyRelease);
		lua_register(L, "keytap ", keyTap);
	}

}