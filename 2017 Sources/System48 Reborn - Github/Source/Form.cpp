#include "stdafx\stdafx.h"
#include "form.h"
#include "system48.h"
#include "Includes/SysIncludes.h"
#include "ChathookCommands.h"
#include "Memory\Memory.h"
#include "mem.h"

/* --- Creation of main window form --- */

bool DynamicOn = false;
bool ForceField = false;
bool Fire = false;
bool Smoke = false;
bool Sparkles = false;
bool Invis = false;
bool NoClipped = false;

bool GodMode = false;
bool WalkSpeed = false;

bool GuestChatEnabled = false;

using namespace CLua;

HMENU PopupSystem48Menu;
HMENU PopupLightingMenu;
HMENU PopupWebUtilMenu;
HMENU PopupQuickCommandMenu;

HFONT SysFont = CreateFont(15, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Sans Serif"));

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case UI_INPUT:
		{
			if (HIWORD(wParam) == EN_MAXTEXT)
			{
				char cText[500];
				SendMessage((HWND)lParam, WM_GETTEXT, 500, (LPARAM)cText);

				if (strcmp(cText, "") == 0)
					break;

				SendMessage((HWND)lParam, WM_SETTEXT, NULL, (LPARAM)"");

				HandleCommand(cText);
				break;
			}
			break;
		}

		case UI_EXIT:
		{
			if (MessageBox(NULL, "Are you sure you want to exit System48?", "Exit", MB_ICONWARNING | MB_OKCANCEL) == IDOK)
				system("taskkill /f /im RobloxPlayerBeta.exe");
			break;
		}

		case UI_CREDITS:
		{
			MessageBox(NULL, SYSTEM48_CREDITS, "Credits", MB_ICONINFORMATION | MB_OK);
			break;
		}

		case UI_MINIMIZE:
		{
			ShowWindow(hwnd, SW_MINIMIZE);
			break;
		}

		case UI_RSPLORER:
		{
			MessageBox(NULL, "RSplorer is not avaliable yet!", "Error", MB_ICONERROR | MB_OK);
			break;
		}

		/* "Web-Utilities" Toolbar */

		case UI_HELPPAGE:
		{
			system("start https://timberman.xyz/azul/help.html");
			break;
		}

		case UI_WLCHANGE:
		{
			system("start https://timberman.xyz/azul/wlchange.php");
			break;
		}

		case UI_FAQ:
		{
			system("start https://timberman.xyz/azul/faq.html");
			break;
		}

		case UI_DISCORD:
		{
			system("start https://discord.gg/uHh58rW");
			break;
		}

		case UI_TRIAL:
		{
			system("start https://timberman.xyz/azul/buysystem48.php");
			break;
		}

		
		/* "Quick Commands" Toolbar */

		case LOCALFORCEFIELD:
		{
			if (ForceField == false)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "ForceField");
				pushvalue(State, -4);
				pcall(State, 2, 0, 0);
				clearstack(State);
				ForceField = true;

				sys->Print(RGB(0, 0, 0), "\nToggle ForceField = ");
				sys->Print(RGB(0, 155, 0), "ON");

				break;
			}

			else if (ForceField == true)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getfield(State, -1, "ForceField");
				getfield(State, -1, "Destroy");
				pushvalue(State, -2);
				pcall(State, 1, 0, 0);
				clearstack(State);
				ForceField = false;

				sys->Print(RGB(0, 0, 0), "\nToggle ForceField = ");
				sys->Print(RGB(255, 0, 0), "OFF");

				break;
			}
		}

		case LOCALFIRE:
		{
			if (Fire == false)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getfield(State, -1, "Head");
				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "Fire");
				pushvalue(State, -4);
				pcall(State, 2, 0, 0);
				clearstack(State);

				sys->Print(RGB(0, 0, 0), "\nToggle Fire = ");
				sys->Print(RGB(0, 155, 0), "ON");

				Fire = true;
				break;
			}

			else if (Fire == true)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getfield(State, -1, "Head");
				getfield(State, -1, "Fire");
				getfield(State, -1, "Destroy");
				pushvalue(State, -2);
				pcall(State, 1, 0, 0);
				clearstack(State);

				sys->Print(RGB(0, 0, 0), "\nToggle Fire = ");
				sys->Print(RGB(255, 0, 0), "OFF");

				Fire = false;
				break;
			}
		}

		case LOCALSMOKE:
		{
			if (Smoke == false)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getfield(State, -1, "Torso");
				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "Smoke");
				pushvalue(State, -4);
				pcall(State, 2, 0, 0);
				clearstack(State);
				Smoke = true;

				sys->Print(RGB(0, 0, 0), "\nToggle Smoke = ");
				sys->Print(RGB(0, 155, 0), "ON");

				break;
			}
			else if (Smoke == true)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getfield(State, -1, "Torso");
				getfield(State, -1, "Smoke");
				getfield(State, -1, "Destroy");
				pushvalue(State, -2);
				pcall(State, 1, 0, 0);
				clearstack(State);
				Smoke = false;

				sys->Print(RGB(0, 0, 0), "\nToggle Smoke = ");
				sys->Print(RGB(255, 0, 0), "OFF");

				break;
			}
		}

		case LOCALGODMODE:
		{
			if (GodMode == false)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getfield(State, -1, "Humanoid");
				pushnumber(State, HUGE_VAL);
				setfield(State, -2, "MaxHealth");
				pcall(State, 1, 0, 0);
				clearstack(State);
				GodMode = true;

				sys->Print(RGB(0, 0, 0), "\nToggle GodMode = ");
				sys->Print(RGB(0, 155, 0), "ON");

				break;
			}

			else if (GodMode == true)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getfield(State, -1, "Humanoid");
				pushnumber(State, 100);
				setfield(State, -2, "MaxHealth");
				pcall(State, 1, 0, 0);
				clearstack(State);

				sys->Print(RGB(0, 0, 0), "\nToggle GodMode = ");
				sys->Print(RGB(255, 0, 0), "OFF");

				break;
			}
		}

		case BTOOLS:
		{
			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 1);
			setfield(State, -2, "BinType");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 2);
			setfield(State, -2, "BinType");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 3);
			setfield(State, -2, "BinType");

			getglobal(State, "game");
			getfield(State, -1, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Backpack");
			getglobal(State, "Instance");
			getfield(State, -1, "new");
			pushstring(State, "HopperBin");
			pushvalue(State, -4);
			pcall(State, 2, 1, 0);
			pushnumber(State, 4);
			setfield(State, -2, "BinType");
			clearstack(State);
			break;
		}

		case GUESTCHAT:
		{
			if (GuestChatEnabled == false) {
				getglobal(State, "game");
				getfield(State, -1, "Chat");
				getfield(State, -1, "ClientChatModules");
				getfield(State, -1, "CommandModules");
				getfield(State, -1, "SwallowGuestChat");
				getfield(State, -1, "Destroy");
				pushvalue(State, -2);
				pcall(State, 1, 1, 0);
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "PlayerGui");
				getfield(State, -1, "Chat");
				getfield(State, -1, "Destroy");
				pushvalue(State, -2);
				pcall(State, 1, 1, 0);
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "PlayerScripts");
				getfield(State, -1, "ChatScript");
				getfield(State, -1, "Clone");
				pushvalue(State, -2);
				pcall(State, 1, 1, 0);
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "PlayerScripts");
				pushvalue(State, -1);
				setfield(State, -6, "Parent");
				clearstack(State);
				Sleep(150);

				//Give chathook to the guest.
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Chatted");
				getfield(State, -1, "connect");
				pushvalue(State, -2);
				pushcfunction(State, GetChatMessage);
				pcall(State, 2, 0, 0);
				clearstack(State);
				GuestChatEnabled = true;
				break;
			}
			else {
				sys->Print(RGB(255, 0, 0), "\nYou already currently have guest chat enabled!");
				break;
			}
		}

		case UNANCHORWS:
		{
			std::vector<int> Wrkspace = getChildren(Workspace);
			for (int c = 0; c < Wrkspace.size(); c++)
			{
				std::string Children = getName(Wrkspace[c]);
				getglobal(State, "Workspace");
				getfield(State, -1, Children.c_str());
				getfield(State, -1, "IsA");
				pushvalue(State, -2);
				pushstring(State, "BasePart");
				pcall(State, 2, 1, 0);
				if (toboolean(State, -1) == true)
				{
					pop(State, 1);
					pushnil(State);
					setfield(State, -2, "Anchored");
				}
				clearstack(State);
				break;
			}
			break;
		}
		break;

		case UNLOCKWS:
		{
			std::vector<int> Wrkspace = getChildren(Workspace);
			for (int c = 0; c < Wrkspace.size(); c++)
			{
				std::string Children = getName(Wrkspace[c]);

				getglobal(State, "Workspace");
				getfield(State, -1, Children.c_str());
				getfield(State, -1, "IsA");
				pushvalue(State, -2);
				pushstring(State, "BasePart");
				pcall(State, 2, 1, 0);
				if (toboolean(State, -1) == true)
				{
					pop(State, 1);
					pushnil(State);
					setfield(State, -2, "Locked");
				}
				clearstack(State);
				break;
			}
			break;
		}
		break;

		case FENCINGREACH:
		{
			getglobal(State, "game");
			getfield(State, -1, "PlaceId");
			double PlaceID = tonumber(State, -1);
			if (PlaceID == 12109643)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Backpack");
				getfield(State, -1, "Foil");
				getfield(State, -1, "Handle");
				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "SelectionBox");
				pushvalue(State, -4);
				pcall(State, 2, 1, 0);
				pushvalue(State, -3);
				setfield(State, -2, "Adornee");
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Backpack");
				getfield(State, -1, "Foil");
				getfield(State, -1, "Handle");
				getglobal(State, "Vector3");
				getfield(State, -1, "new");
				pushnumber(State, 1);
				pushnumber(State, 1);
				pushnumber(State, 30);
				pcall(State, 3, 1, 0);
				setfield(State, -3, "Size");
				clearstack(State);
				sys->Print(RGB(0, 150, 0), "\nFencing Reach is enabled!");
				break;
			}
			else {
				sys->Print(RGB(255, 0, 0), "\nYou're currently not in Fencing!");
				break;
			}
		}

		case RECRUITMENTREACH:
		{
			getglobal(State, "game");
			getfield(State, -1, "PlaceId");
			double PlaceID = tonumber(State, -1);
			if (PlaceID == 6194809)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Backpack");
				getfield(State, -1, "_classic");
				getfield(State, -1, "Handle");
				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "SelectionBox");
				pushvalue(State, -4);
				pcall(State, 2, 1, 0);
				pushvalue(State, -3);
				setfield(State, -2, "Adornee");
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Backpack");
				getfield(State, -1, "_classic");
				getfield(State, -1, "Handle");
				getglobal(State, "Vector3");
				getfield(State, -1, "new");
				pushnumber(State, 1);
				pushnumber(State, 1);
				pushnumber(State, 30);
				pcall(State, 3, 1, 0);
				setfield(State, -3, "Size");
				clearstack(State);
				sys->Print(RGB(0, 150, 0), "\nReruitment Plaza reach is enabled!");
				break;
			}

			else {
				sys->Print(RGB(255, 0, 0), "\nYou're currently not in the Recruitment Plaza!");
				break;
			}

		}

		case QUICKRESPAWN:
		{
			getglobal(State, "Vector3");
			getfield(State, -1, "new");
			pushnumber(State, 0);
			pushnumber(State, -1000000);
			pushnumber(State, 0);
			pcall(State, 3, 1, 0);
			getglobal(State, "Players");
			getfield(State, -1, "LocalPlayer");
			getfield(State, -1, "Character");
			getfield(State, -1, "MoveTo");
			pushvalue(State, -2);
			pushvalue(State, -5);
			pcall(State, 2, 0, 0);
			clearstack(State);
			break;
		}

		case QUICKWS:
		{
			if (WalkSpeed == false)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getfield(State, -1, "Humanoid");
				pushnumber(State, 100);
				setfield(State, -2, "WalkSpeed");
				pcall(State, 1, 0, 0);
				clearstack(State);
				WalkSpeed = true;

				sys->Print(RGB(0, 0, 0), "\nToggle WalkSpeed = ");
				sys->Print(RGB(0, 150, 0), "ON");
				break;
			}
			else if (WalkSpeed == true)
			{
				getglobal(State, "game");
				getfield(State, -1, "Players");
				getfield(State, -1, "LocalPlayer");
				getfield(State, -1, "Character");
				getfield(State, -1, "Humanoid");
				pushnumber(State, 16);
				setfield(State, -2, "WalkSpeed");
				pcall(State, 1, 0, 0);
				clearstack(State);
				WalkSpeed = false;
				sys->Print(RGB(0, 0, 0), "\nToggle WalkSpeed = ");
				sys->Print(RGB(255, 0, 0), "OFF");

				break;
			}
		}

		case UNCLEBILL:
		{
				getglobal(State, "game");
				getfield(State, -1, "Workspace");
				getglobal(State, "Instance");
				getfield(State, -1, "new");
				pushstring(State, "Sound");
				pushvalue(State, -4);
				pcall(State, 2, 1, 0);
				pushstring(State, ("rbxassetid://666951532"));
				setfield(State, -2, "SoundId");
				pushstring(State, "System48Music");
				setfield(State, -2, "Name");
				pushnumber(State, 1);
				setfield(State, -2, "Volume");

				pushstring(State, "true");
				setfield(State, -2, "Playing");
				pushvalue(State, -2);
				pcall(State, 1, 0, 0);
				clearstack(State);

				getglobal(State, "game");
				getfield(State, -1, "Lighting");
				pushnumber(State, 40);
				setfield(State, -2, "FogEnd");
				clearstack(State);

				getglobal(State, "game");
				getfield(State, -1, "Lighting");
				getglobal(State, "Color3");
				getfield(State, -1, "new");
				pushnumber(State, 255);
				pushnumber(State, 0);
				pushnumber(State, 0);
				pcall(State, 3, 1, 0);
				setfield(State, -3, "FogColor");
				clearstack(State);

				getglobal(State, "game");
				getfield(State, -1, "Lighting");
				pushnumber(State, 0);
				setfield(State, -2, "TimeOfDay");
				clearstack(State);
				break;
		}


		/* "Lighting" Toolbar */

		case TIME_DAY:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			pushnumber(State, 12);
			setfield(State, -2, "TimeOfDay");
			clearstack(State);
		}
		break;

		case TIME_NIGHT:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			pushnumber(State, 0);
			setfield(State, -2, "TimeOfDay");
			clearstack(State);
		}
		break;

		case REMOVE_FOG:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			pushnumber(State, 100000000);
			setfield(State, -2, "FogEnd");
			clearstack(State);
		}
		break;

		case BLACK_FOG:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			getglobal(State, "Color3");
			getfield(State, -1, "new");
			pushnumber(State, 0);
			pushnumber(State, 0);
			pushnumber(State, 0);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "FogColor");
			clearstack(State);
		}
		break;

		case WHITE_FOG:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			getglobal(State, "Color3");
			getfield(State, -1, "new");
			pushnumber(State, 255);
			pushnumber(State, 255);
			pushnumber(State, 255);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "FogColor");
			clearstack(State);
		}
		break;

		case RED_FOG:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			getglobal(State, "Color3");
			getfield(State, -1, "new");
			pushnumber(State, 255);
			pushnumber(State, 0);
			pushnumber(State, 0);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "FogColor");
			clearstack(State);
		}
		break;

		case BLUE_FOG:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			getglobal(State, "Color3");
			getfield(State, -1, "new");
			pushnumber(State, 0);
			pushnumber(State, 0);
			pushnumber(State, 255);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "FogColor");
			clearstack(State);
		}
		break;

		case GREEN_FOG:
		{
			getglobal(State, "game");
			getfield(State, -1, "Lighting");
			getglobal(State, "Color3");
			getfield(State, -1, "new");
			pushnumber(State, 0);
			pushnumber(State, 128);
			pushnumber(State, 0);
			pcall(State, 3, 1, 0);
			setfield(State, -3, "FogColor");
			clearstack(State);
		}
		break;

		case TOGGLEDL:
		{
			if (DynamicOn == true)
			{
				getglobal(State, "game");
				getfield(State, -1, "Lighting");
				pushboolean(State, false);
				setfield(State, -2, "GlobalShadows");
				clearstack(State);
				DynamicOn = false;
				sys->Print(RGB(0, 0, 0), "\nToggle Dynamic Lighting = ");
				sys->Print(RGB(0, 150, 0), "ON");
			}
			else
			{
				getglobal(State, "game");
				getfield(State, -1, "Lighting");
				pushboolean(State, true);
				setfield(State, -2, "GlobalShadows");
				clearstack(State);
				DynamicOn = true;
				sys->Print(RGB(0, 0, 0), "\nToggle Dynamic Lighting = ");
				sys->Print(RGB(255, 0, 0), "OFF");
			}
		}
		break;
		default:;
		}
	}

	case WM_DESTROY:
		//system("taskkill /f /im RobloxPlayerBeta.exe");
		break;

	case WM_QUIT:
		system("taskkill /f /im RobloxPlayerBeta.exe");
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 1;
}

void RemoveStyle(HWND hwnd, int style)
{
	LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	lExStyle &= ~(style);
	SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
}

BOOL RegisterWindowClass()
{
	WNDCLASSEX nClass;

	nClass.cbSize = sizeof(WNDCLASSEX);
	nClass.style = CS_DBLCLKS;
	nClass.lpfnWndProc = WindowProc;
	nClass.cbClsExtra = 0;
	nClass.cbWndExtra = 0;
	nClass.hInstance = GetModuleHandle(0);
	nClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	nClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	nClass.hCursor = LoadCursor(0, IDC_ARROW);
	nClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	nClass.lpszMenuName = "System48";
	nClass.lpszClassName = "_SYSTEM48";

	if (!RegisterClassEx(&nClass))
		return 0;

	return 1;
}

void System48::Print(COLORREF col, const char* format, ...)
{
	char message[10000];
	memset(message, 0, sizeof(message));
	va_list vl;
	va_start(vl, format);
	vsnprintf_s(message, 10000, format, vl);
	va_end(vl);

	int len = SendMessage(sys->OutputField, WM_GETTEXTLENGTH, NULL, NULL);
	SendMessage(sys->OutputField, EM_SETSEL, len, len);

	CHARFORMAT cfd;
	CHARFORMAT cf;
	SendMessage(sys->OutputField, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cfd);
	memcpy(&cf, &cfd, sizeof(CHARFORMAT));

	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = col;
	cf.dwEffects = 0;

	SendMessage(sys->OutputField, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	SendMessage(sys->OutputField, EM_REPLACESEL, FALSE, (LPARAM)message);
	SendMessage(sys->OutputField, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfd);
}


bool System48::UpdateCheck()
{
     #ifndef NOUPDATE
	/*-- Scan the site for any avaliable updates! --*/
	std::string currentversion = SYSTEM48_VERSION;
	std::string nextversion = SYSTEM48_NEXTVERSION;
	std::string host = DownloadURL("https://buyex7.com/System48/version.php");

	if (nextversion == host) {
		sys->Print(RGB(255, 0, 0), "\nSystem48 has an update avaliable! Version: %s\n", SYSTEM48_NEXTVERSION);
		MessageBox(NULL, "System48 has an update avaliable!", "Update", MB_ICONEXCLAMATION | MB_OK);
		system("start https://1drv.ms/u/s!AsTOHXgRtJ6Fbt9d062YuJtxTik");
		std::exit(0);
	}

	else if (currentversion == host) {
		//continue
	}

	else {
		sys->Print(RGB(255, 0, 0), "Error while fetching data from version host! Continuing with init...\n");
	}

    #endif
	return 0;
}


void CreateToolbarMenu(HMENU hhMenu)
{
	/* Definitions */
	PopupLightingMenu = CreatePopupMenu();
	HMENU hFogSub = CreatePopupMenu();
	HMENU hSubSubMenu = CreatePopupMenu();
	HMENU hSubSub2Menu = CreatePopupMenu();
	HMENU LocalPlayerSub = CreatePopupMenu();
	HMENU DestructiveSub = CreatePopupMenu();
	HMENU OtherSub = CreatePopupMenu();
	HMENU LightingSub = CreatePopupMenu();
	PopupWebUtilMenu = CreatePopupMenu();
	PopupQuickCommandMenu = CreatePopupMenu();


	PopupSystem48Menu = CreatePopupMenu();
	AppendMenu(PopupSystem48Menu, MF_STRING, UI_EXIT, TEXT("Exit"));
	AppendMenu(PopupSystem48Menu, MF_STRING, UI_CREDITS, TEXT("Credits"));
	InsertMenu(PopupSystem48Menu, 2, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
	AppendMenu(PopupSystem48Menu, MF_STRING, UI_RSPLORER, TEXT("RSplorer"));

	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)PopupSystem48Menu, TEXT("System48"));

	//AppendMenu(PopupWebUtilMenu, MF_STRING, UI_TRIAL, TEXT("Purchase System48"));

	AppendMenu(PopupWebUtilMenu, MF_STRING, UI_FAQ, TEXT("Frequently Asked Questions"));
	AppendMenu(PopupWebUtilMenu, MF_STRING, UI_WLCHANGE, TEXT("Whitelist Change"));
	AppendMenu(PopupWebUtilMenu, MF_STRING, UI_HELPPAGE, TEXT("Help Page"));
	AppendMenu(PopupWebUtilMenu, MF_STRING, UI_DISCORD, TEXT("Discord Chat"));

	//AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)PopupWebUtilMenu, TEXT("Web Utilities"));

	AppendMenu(LocalPlayerSub, MF_STRING, LOCALFORCEFIELD, "Toggle ForceField");
	AppendMenu(LocalPlayerSub, MF_STRING, LOCALFIRE, "Toggle Fire");
	AppendMenu(LocalPlayerSub, MF_STRING, LOCALSMOKE, "Toggle Smoke");
	AppendMenu(LocalPlayerSub, MF_STRING, LOCALGODMODE, "Toggle GodMode");
	AppendMenu(LocalPlayerSub, MF_STRING, QUICKWS, "Toggle Set Walkspeed to 100");
	InsertMenu(LocalPlayerSub, 5, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
	AppendMenu(LocalPlayerSub, MF_STRING, BTOOLS, "Give Btools");
	//AppendMenu(LocalPlayerSub, MF_STRING, GUESTCHAT, "Enable Guest Chat");
	 
	AppendMenu(PopupQuickCommandMenu, MF_POPUP, (UINT_PTR)LocalPlayerSub, "LocalPlayer");


	AppendMenu(LightingSub, MF_POPUP, (UINT_PTR)hFogSub, "FogColor");
	AppendMenu(hFogSub, MF_STRING, BLACK_FOG, "FG Black");
	AppendMenu(hFogSub, MF_STRING, WHITE_FOG, "FG White");
	AppendMenu(hFogSub, MF_STRING, RED_FOG, "FG Red");
	AppendMenu(hFogSub, MF_STRING, GREEN_FOG, "FG Green");
	AppendMenu(hFogSub, MF_STRING, BLUE_FOG, "FG Blue");
	InsertMenu(hFogSub, 5, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
	AppendMenu(hFogSub, MF_STRING, REMOVE_FOG, "Remove Fog");


	AppendMenu(LightingSub, MF_POPUP, (UINT_PTR)hSubSub2Menu, "Set Time");
	AppendMenu(hSubSub2Menu, MF_STRING, TIME_DAY, "Time Day");
	AppendMenu(hSubSub2Menu, MF_STRING, TIME_NIGHT, "Time Night");
	InsertMenu(LightingSub, 3, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
	AppendMenu(LightingSub, MF_STRING, TOGGLEDL, "Toggle Dynamic Lighting");

	AppendMenu(PopupQuickCommandMenu, MF_POPUP, (UINT_PTR)LightingSub, "Lighting");

	AppendMenu(OtherSub, MF_STRING, FENCINGREACH, "Enable Fencing Reach");
	AppendMenu(OtherSub, MF_STRING, RECRUITMENTREACH, "Enable RecruitMent Plaza Reach");
	AppendMenu(OtherSub, MF_STRING, UNCLEBILL, "Execute UncleBill Command");

	InsertMenu(PopupQuickCommandMenu, 3, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
	AppendMenu(PopupQuickCommandMenu, MF_POPUP, (UINT_PTR)OtherSub, "Other");

	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)PopupQuickCommandMenu, TEXT("Quick Commands"));
}

HMENU CreateDLLWindowMenu()
{
	HMENU System48Menu;
	System48Menu = CreateMenu();

	if (System48Menu == NULL)
		return FALSE;

	CreateToolbarMenu(System48Menu);
	return System48Menu;
}

void HotKeys() {
	do {
		if (GetAsyncKeyState(VK_F1)) {
			HandleCommand("respawn me");
		}

		else if (GetAsyncKeyState(VK_F2)) {
			if (ForceField == false) {
				HandleCommand("ff me");
				ForceField = true;
			}

			else if (ForceField == true) {
				HandleCommand("unff me");
				ForceField = false;
			}
		}

		else if (GetAsyncKeyState(VK_F3)) {
			if (Sparkles == false) {
				HandleCommand("sparkles me");
				Sparkles = true;
			}

			else if (Sparkles == true) {
				HandleCommand("unsparkles me");
				Sparkles = false;
			}
		}

		else if (GetAsyncKeyState(VK_F4)) {
			if (Fire == false) {
				HandleCommand("fire me");
				Fire = true;
			}

			else if (Fire == true) {
				HandleCommand("unfire me");
				Fire = false;
			}
		}

		else if (GetAsyncKeyState(VK_F5)) {
			HandleCommand("btools me");
		}

		else if (GetAsyncKeyState(VK_F6)) {
			HandleCommand("bring others");
		}

		else if (GetAsyncKeyState(VK_F7)) {
			if (Invis == false) {
				HandleCommand("invisible me");
				Invis = true;
			}

			else if (Invis == true) {
				HandleCommand("visible me");
				Invis = false;
			}
		}

		else if (GetAsyncKeyState(VK_F8)) {
			HandleCommand("unclebill");
		}

		else if (GetAsyncKeyState(VK_F10)) {
			if (NoClipped == false) {
				HandleCommand("noclip");
				NoClipped = true;
			}

			else if (NoClipped == true) {
				HandleCommand("disable");
				NoClipped = false;
			}
		}

		Sleep(100);
	} while (true);
}

int System48::getState(DWORD ScriptContext)
{
	int ls = ScriptContext + 56 * 1 + 164 - *(DWORD *)(ScriptContext + 56 * 1 + 164);
	return ls;
}

#define DEBUG

void System48::SysInit()
{
	/* -- Begin init after whitelist -- */
	VMProtectBeginVirtualization("Sys Function: Scan | Virtualization");

    #ifdef DEBUG
	sys->Print(RGB(0, 0, 0), "Scanning...\n");
    #endif

    #ifndef DEBUG
	sys->Print(RGB(0, 0, 0), "Scanning... ");
    #endif

	if (currentver != site)
	{
		sys->Print(RGB(255, 0, 0), "\nERROR: Roblox update detected!\n");
		ScanFailed = true;
	}

	else
	{
        #ifdef DEBUG
		sys->Print(RGB(0, 0, 0), "Scanning for ScriptContext... ");
        #endif

		try {
			int VFTable = ScanAddress(SCRIPTCONTEXT);
			int* ScriptContext = (int*)Memory::Scan(PAGE_READWRITE, (char*)&VFTable, "xxxx");
			DataModel = getParent((DWORD)ScriptContext);

            #ifdef DEBUG
			Sleep(700);
			sys->Print(RGB(0, 150, 0), "OK!\n");
            #endif

            #ifdef DEBUG
			sys->Print(RGB(0, 0, 0), "Getting services and globalstate... ");
            #endif

			/* -- Define the functions that System48 uses.*/
			Players = getService("Players");
			Workspace = getService("Workspace");
			Lighting = getService("Lighting");
			LocalPlayer = *(DWORD*)(Players + 0x154);

            #ifndef DEBUG
			State = sys->getState((DWORD)ScriptContext);
            #endif

            #ifdef DEBUG
			sys->Print(RGB(0, 0, 0), "GlobalState... ");
			State = sys->getState((DWORD)ScriptContext);
#endif
		}

		catch (std::exception error) {
			sys->Print(RGB(255, 0, 0), "\nERROR: Error during initial scan!\n");
			ScanFailed = true;
		}

		catch (...) {
			sys->Print(RGB(255, 0, 0), "\nERROR: Error during initial scan!\n");
			ScanFailed = true;
		}

        #ifdef DEBUG
		Sleep(1000);
		sys->Print(RGB(0, 150, 0), "OK!\n");
		#endif

        #ifdef DEBUG
		sys->Print(RGB(0, 0, 0), "Initiating chathook... ");
        #endif

        #define DisableChathook

		/* -- Init Chathook -- */
        #ifndef DisableChathook
	    getglobal(State, "game");
		getfield(State, -1, "Players");
		getfield(State, -1, "LocalPlayer");
		getfield(State, -1, "Chatted");
		getfield(State, -1, "connect");
		pushvalue(State, -2);
		pushcfunction(State, GetChatMessage);
		pcall(State, 2, 0, 0);
		clearstack(State);
        #endif

        #ifdef DEBUG
		sys->Print(RGB(0, 150, 0), "OK!\n");
        #endif
        
        #ifdef DEBUG
		sys->Print(RGB(0, 0, 0), "Finishing up... ");
		Sleep(1000);
        #endif

		sys->Print(RGB(0, 0, 200), "Done!\n");
		EnableWindow(sys->InputField, true);
		getFiltering();

		sys->Print(RGB(0, 0, 0), "System48 successfully initialized!\nInput 'cmds' for a list of commands!\n");

        #ifdef DEBUG
		sys->Print(RGB(0, 0, 0), "Debugging is enabled. To remove the junk code, input 'clr' or 'clear'\n");
        #endif

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HotKeys, 0, 0, 0);

        #ifndef DISABLETRIAL
		sys->Print(RGB(0, 0, 0), "Welcome to the offical System48 trial!\n");
        #endif
	}
	VMProtectEnd();
}

DWORD WINAPI Main(LPVOID Param)
{
	VMProtectBeginVirtualization("Major Security function.");
	LoadLibrary("Riched20.dll");

	if (!LoadLibrary("Riched20.dll"))
	{
		MessageBox(NULL, "System48 has failed to load 'Riched20.dll' Please retry.", "Error!", MB_OK);
		throw std::exception("Failed to load UI component");
	}

	UnregisterClass("_SYSTEM48", sys->HInstance);
	RegisterWindowClass();

	HMENU WindowMenu = CreateDLLWindowMenu();
	sys->MainWindow = CreateWindowEx(NULL, "_System48", SYSTEM48_TITLE, WS_SYSMENU | WS_MINIMIZEBOX, UI_POSX, UI_POSY, UI_WIDTH, UI_HEIGHT, 0, WindowMenu, sys->HInstance, 0);

	if (!sys->MainWindow)
	{
		MessageBox(NULL, "Failure to load the main window", "Error!", MB_OK);
		throw std::exception("failed to register window");
	}

	sys->OutputField = CreateWindowEx(NULL, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_READONLY | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_MULTILINE, 12, 12, 480, 189, sys->MainWindow, NULL, sys->HInstance, NULL);
	sys->InputField = CreateWindowEx(0, "EDIT", "", WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VISIBLE, 12, 207, 480, 20, sys->MainWindow, (HMENU)UI_INPUT, sys->HInstance, 0);

	SendMessage(sys->InputField, WM_SETFONT, (WPARAM)SysFont, NULL);
	SendMessage(sys->OutputField, WM_SETFONT, (WPARAM)SysFont, NULL);
	EnableWindow(sys->InputField, false);

	ShowWindow(sys->MainWindow, SW_SHOW);

	RemoveStyle(sys->OutputField, WS_EX_CLIENTEDGE);

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Whitelist::BeginInit, 0, 0, 0);

	SetWindowPos(sys->MainWindow, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOSIZE | SWP_NOMOVE);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	VMProtectEnd();
	return 0;
}

BYTE DllMain(HMODULE Dll, DWORD Reason, PVOID Reserved)
{
	if (Reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(Dll);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, NULL, NULL, NULL);
	}
	return TRUE;
}

System48 lsys;
System48* sys = &lsys;