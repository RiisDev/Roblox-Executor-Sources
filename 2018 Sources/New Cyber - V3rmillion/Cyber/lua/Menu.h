#include <Windows.h>
#include "Addresses.h"



#define ok_Pointer (0x8064 + 1)
#define BToolsOn_Pointer (0x8064 + 2)
#define ok_Pointer (0x8064 + 1)

#define luaState rL

bool BTools;

bool FeCheck;

LRESULT CALLBACK NewWindowProcedure(HWND Window, UINT Message, WPARAM Parameter1, LPARAM Parameter2);
WNDPROC PreviousWindowProcedure = (WNDPROC)SetWindowLongPtr(FindWindowW(NULL, L"ROBLOX"), GWL_WNDPROC, (LONG_PTR)&NewWindowProcedure);

LRESULT CALLBACK NewWindowProcedure(HWND Window, UINT Message, WPARAM Parameter1, LPARAM Parameter2) {
	if (Message == WM_COMMAND) {
		switch (Parameter1) {
		case BToolsOn_Pointer:
			if (!BTools) {
				rlua_getglobal(luaState, "game");

				rlua_getfield(luaState, -1, "GetService");

				rlua_pushvalue(luaState, -2);

				rlua_pushstring(luaState, "Players");

				rlua_pcall(luaState, 2, 1, 0);

				rlua_getfield(luaState, -1, "LocalPlayer");

				rlua_getfield(luaState, -1, "Backpack");

				rlua_getglobal(luaState, "Instance");

				rlua_getfield(luaState, -1, "new");

				rlua_pushstring(luaState, "HopperBin");

				rlua_pushvalue(luaState, -4);

				rlua_pcall(luaState, 2, 1, 0);

				rlua_pushnumber(luaState, 1); // go ahead

				rlua_setfield(luaState, -2, "BinType");


				rlua_getglobal(luaState, "game");
				rlua_getfield(luaState, -1, "GetService");
				rlua_pushvalue(luaState, -2);
				rlua_pushstring(luaState, "Players");
				rlua_pcall(luaState, 2, 1, 0);
				rlua_getfield(luaState, -1, "LocalPlayer");
				rlua_getfield(luaState, -1, "Backpack");
				rlua_getglobal(luaState, "Instance");
				rlua_getfield(luaState, -1, "new");
				rlua_pushstring(luaState, "HopperBin");
				rlua_pushvalue(luaState, -4);
				rlua_pcall(luaState, 2, 1, 0);
				rlua_pushnumber(luaState, 3);
				rlua_setfield(luaState, -2, "BinType");
				rlua_getglobal(luaState, "game");
				rlua_getfield(luaState, -1, "GetService");
				rlua_pushvalue(luaState, -2);
				rlua_pushstring(luaState, "Players");
				rlua_pcall(luaState, 2, 1, 0);
				rlua_getfield(luaState, -1, "LocalPlayer");
				rlua_getfield(luaState, -1, "Backpack");
				rlua_getglobal(luaState, "Instance");
				rlua_getfield(luaState, -1, "new");
				rlua_pushstring(luaState, "HopperBin");
				rlua_pushvalue(luaState, -4);
				rlua_pcall(luaState, 2, 1, 0);
				rlua_pushnumber(luaState, 4);
				rlua_setfield(luaState, -2, "BinType");
			};
			break;
		}
	}
	return CallWindowProc(PreviousWindowProcedure, Window, Message, Parameter1, Parameter2);
}


HMENU JBOkDropdown;
HMENU BToolsDropdown;

DWORD WINAPI menu() {



	HMENU ExploitDropdownMenu = CreateMenu();
	HMENU GamesDropdownMenu = CreateMenu();
	HMENU MorphDropdownMenu = CreateMenu();
	HMENU MiscellaneousDropdown = CreateMenu();
	HMENU BToolsDropdownMenu = CreateMenu();
	HMENU EspDropdownMenu = CreateMenu();
	HMENU InsertDropdownMenu = CreateMenu();

	BToolsDropdown = CreateMenu();
	JBOkDropdown = CreateMenu();
	/* menus */
	AppendMenu(ExploitDropdownMenu, (MF_STRING | MF_POPUP), (UINT_PTR)MiscellaneousDropdown, "QuickTools");

	//AppendMenu(ExploitDropdownMenu, (MF_STRING | MF_POPUP), (UINT_PTR)GamesDropdownMenu, "Misc");

	/* games */

	//AppendMenu(GamesDropdownMenu, (MF_STRING | MF_POPUP), (UINT_PTR)JBOkDropdown, "FeCheck");
	//AppendMenu(JBOkDropdown, (MF_STRING | MF_POPUP), ok_Pointer, "test");

	/* BTOOLS */

	AppendMenu(MiscellaneousDropdown, (MF_STRING | MF_POPUP), (UINT_PTR)BToolsDropdown, "Btools");
	AppendMenu(BToolsDropdown, (MF_STRING | MF_POPUP), BToolsOn_Pointer, "On");

	return 0x1;
}


