#pragma once
#include "Roblox.h"
#include "LuaWrapper.h"

using LuaWrapper::m_L;

int SetClientName(std::string Name)
{
    HWND FindRobloxWindow = FindWindowExW(NULL, NULL, NULL, L"Roblox");
    HMENU GetRobloxMenu = GetMenu(FindRobloxWindow);
    HMENU CreateMenuBar = CreateMenu();
    SetMenu(FindRobloxWindow, CreateMenuBar);
    std::wstring RobloxToolTip = std::wstring(Name.begin(), Name.end());
    LPCWSTR NewRobloxTitle = RobloxToolTip.c_str();
    SetWindowTextW(FindRobloxWindow, NewRobloxTitle);
}

void ConsoleBypass(const char* Title) {
	DWORD aaaa;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &aaaa);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

std::string GetPlayerName()
{
	lua_getglobal(m_L, "game");
    lua_getfield(m_L, -1, "Players");
	lua_getfield(m_L, -1, "LocalPlayer");
	lua_getfield(m_L, -1, "Name");
	std::string playername = lua_tolstring(m_L, -1, NULL);
	return playername;
}