#pragma once

#include "SDK.h"

//THANKS 3ds

#define BLACK "@@BLACK@@"
#define BLUE "@@BLUE@@"
#define GREEN "@@GREEN@@"
#define CYAN "@@CYAN@@"
#define RED "@@RED@@"
#define MAGENTA "@@MAGENTA@@"
#define BROWN "@@BROWN@@"
#define LIGHT_GRAY "@@LIGHT_GRAY@@"
#define DARK_GRAY "@@DARK_GRAY@@"
#define LIGHT_BLUE "@@LIGHT_BLUE@@"
#define LIGHT_GREEN "@@LIGHT_GREEN@@"
#define LIGHT_CYAN "@@LIGHT_CYAN@@"
#define LIGHT_RED "@@LIGHT_RED@@"
#define LIGHT_MAGENTA "@@LIGHT_MAGENTA@@"
#define YELLOW "@@YELLOW@@"
#define WHITE "@@WHITE@@"

class Console
{
public:
	Console(std::string Name)
	{
		DWORD oldProtect;
		VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
		*(BYTE*)(&FreeConsole) = 0xC3;
		VirtualProtect(&FreeConsole, 1, oldProtect, &oldProtect);
		AllocConsole();
		SetConsoleTitle(Name.c_str());
		freopen("CONOUT$", "w", stdout);
		freopen("CONIN$", "r", stdin);
		HWND ConsoleHandle = GetConsoleWindow();
		SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		ShowWindow(ConsoleHandle, SW_NORMAL);
	}

	Console operator<<(std::string str) const
	{
		if (str == BLACK) { setcolor(0); }
		else if (str == BLUE) { setcolor(1); }
		else if (str == GREEN) { setcolor(2); }
		else if (str == CYAN) { setcolor(3); }
		else if (str == RED) { setcolor(4); }
		else if (str == MAGENTA) { setcolor(5); }
		else if (str == BROWN) { setcolor(6); }
		else if (str == LIGHT_GRAY) { setcolor(7); }
		else if (str == DARK_GRAY) { setcolor(8); }
		else if (str == LIGHT_BLUE) { setcolor(9); }
		else if (str == LIGHT_GREEN) { setcolor(10); }
		else if (str == LIGHT_CYAN) { setcolor(11); }
		else if (str == LIGHT_RED) { setcolor(12); }
		else if (str == LIGHT_MAGENTA) { setcolor(13); }
		else if (str == YELLOW) { setcolor(14); }
		else if (str == WHITE) { setcolor(15); }
		else
		{
			std::cout << str;
		}
		return *this;
	}

	static void setcolor(int color)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}
};