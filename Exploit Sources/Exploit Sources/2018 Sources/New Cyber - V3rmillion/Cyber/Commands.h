#pragma once
#include <Windows.h>
#include "Addresses.h"
#define state rL

DWORD LeftClick()
{
	return GetAsyncKeyState(VK_LBUTTON) < 0 ? true : false;
}

#define getglobal(l,g)			 rlua_getfield(l, LUA_GLOBALSINDEX, g);

inline void getservice(int state, const char* s)
{
	rlua_getfield(rL, -10002, "game");

	rlua_getfield(rL, -1, "GetService");

	rlua_pushvalue(rL, -2);

	rlua_pushstring(rL, s);

	rlua_pcall(rL, 2, 1, 0);
	
}

bool ClickTP = false;


inline void ClickTeleport() {
	for (;;)
	{
		if (ClickTP != true) ExitThread(0);

		if (GetAsyncKeyState(VK_CONTROL) < 0 && LeftClick() == true) {
			try {
				getservice(state, "Players");
				rlua_getfield(state, -1, "LocalPlayer");
				rlua_getfield(state, -1, "GetMouse");
				rlua_pushvalue(state, -2);
				rlua_pcall(state, 1, 1, 0);
				rlua_getfield(state, -1, "Hit");

				getservice(state, "Players");
				rlua_getfield(state, -1, "LocalPlayer");
				rlua_getfield(state, -1, "Character");
				rlua_getfield(state, -1, "HumanoidRootPart");
				rlua_pushvalue(state, -6);
				rlua_setfield(state, -2, "CFrame");
				Sleep(150);
			}
			catch (std::exception) {}
			catch (...) {};
		}
	}
}

void cmds(std::string cmd) {
	std::vector<std::string> st = gay(cmd, ' ');
	if (st.size() != 0) {
		if (tostring(st.at(0)) == "clicktp") {
			{
				ClickTP = !ClickTP;
				if (ClickTP == true)
				{
					CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ClickTeleport, 0, 0, 0);
				}
			}
		}
	}
}