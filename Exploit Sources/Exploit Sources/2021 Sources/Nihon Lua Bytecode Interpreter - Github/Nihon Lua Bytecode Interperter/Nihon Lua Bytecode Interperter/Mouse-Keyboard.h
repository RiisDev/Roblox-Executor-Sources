#include <Windows.h>
#include <WinInet.h>
#include <string>
#include <WinUser.h>
#include <string>
#include <Shlwapi.h>
#include <fstream>
#include <filesystem>
#include "CallCheck.h"

HWND RobloxWindow;

using namespace CallCheck;

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

	RegisterFunction(rL, reinterpret_cast<int>(Mouse1Press), "mouse1press");
	RegisterFunction(rL, reinterpret_cast<int>(Mouse1Release), "mouse1release");
	RegisterFunction(rL, reinterpret_cast<int>(Mouse1Click), "mouse1click");
	RegisterFunction(rL, reinterpret_cast<int>(Mouse2Press), "mouse2press");
	RegisterFunction(rL, reinterpret_cast<int>(Mouse2Release), "mouse2release");
	RegisterFunction(rL, reinterpret_cast<int>(Mouse2Click), "mouse2click");

}