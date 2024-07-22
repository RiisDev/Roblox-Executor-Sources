#include "Open.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Lua\llimits.h"
void Utils::Logs(std::string d) { std::cout << d << std::endl; }
#define set(x) (x - 0x400000 + (DWORD)GetModuleHandleA(0))

std::string Utils::consoleName(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

void Utils::OpenConsole()
{
	DWORD oldProt;
	VirtualProtect((LPVOID)FreeConsole, 1, PAGE_EXECUTE_READWRITE, &oldProt);
	*(BYTE*)FreeConsole = 0xC3;
	VirtualProtect((LPVOID)FreeConsole, 1, oldProt, &oldProt);
	AllocConsole();
	const char* actualName = consoleName((rand() % 18) + 3).c_str();
	SetConsoleTitle(actualName);
	FILE* v;
	freopen_s(&v, "CONIN$", "r", stdin);
	freopen_s(&v, "CONOUT$", "w", stdout);
	freopen_s(&v, "CONOUT$", "w", stderr);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
	fclose(v);
}

void Utils::CloseConsole()
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);
}

BOOL WINAPI Utils::Load()
{
	DWORD nOldProtect;
	if (!VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(FreeConsole) = 0xC3;
	if (!VirtualProtect(FreeConsole, 1, nOldProtect, &nOldProtect))
		return FALSE;
	AllocConsole();
}

std::vector<std::string> Utils::split(const std::string &text, char sep) {
	std::vector<std::string> tokens;
	std::size_t start = 0, end = 0;
	while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
	tokens.push_back(text.substr(start));
	return tokens;
}