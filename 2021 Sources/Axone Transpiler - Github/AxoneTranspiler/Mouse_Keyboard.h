#pragma once
#include <Windows.h>
#include <WinInet.h>
#include <string>
#include <WinUser.h>
#include <string>
#include <Shlwapi.h>
#include <fstream>
#include <filesystem>
#include "CallCheck Bypass.h"

HWND RobloxWindow;



int Mouse1Press(int rL) {
	if (GetForegroundWindow() == RobloxWindow)
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);

	return 0;
}

int Mouse1Release(int rL) {
	if (GetForegroundWindow() == RobloxWindow)
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	return 0;
}

int Mouse1Click(int rL) {
	if (GetForegroundWindow() == RobloxWindow)
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	return 0;
}

int Mouse2Press(int rL) {
	if (GetForegroundWindow() == RobloxWindow)
		mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);

	return 0;
}

int Mouse2Release(int rL) {
	if (GetForegroundWindow() == RobloxWindow)
		mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);

	return 0;
}

int Mouse2Click(int rL) {
	if (GetForegroundWindow() == RobloxWindow)
		mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);

	return 0;
}


void RegisterMouseKeyboard() {

	r_luaL_register(RL, "mouse1press", reinterpret_cast<int>(Mouse1Press));
	r_luaL_register(RL,"mouse1release", reinterpret_cast<int>(Mouse1Release));
	r_luaL_register(RL, "mouse1click", reinterpret_cast<int>(Mouse1Click));
	r_luaL_register(RL, "mouse2press", reinterpret_cast<int>(Mouse2Press));
	r_luaL_register(RL,"mouse2release", reinterpret_cast<int>(Mouse2Release));
	r_luaL_register(RL, "mouse2click", reinterpret_cast<int>(Mouse2Click));

}