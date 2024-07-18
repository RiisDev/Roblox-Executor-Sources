#include <Windows.h>
#include <string>
#include <istream>
#include <iterator>
#include <ostream>
#include <iostream>
#include "VMProtectSDK.h"

using namespace std;

void toClipboard(HWND hwnd, const std::string &s);

int main()
{
	VMProtectBeginUltra("HWID Grabber");

	HW_PROFILE_INFO hwProfileInfo;

	std::string HWID;

	if (GetCurrentHwProfile(&hwProfileInfo) != NULL) {
		HWID = hwProfileInfo.szHwProfileGuid;
	}

	SetConsoleTitleA("Flame | Unique Key Grabber");

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "[";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	std::cout << "*";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "]";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	std::cout << " Successfully copied your unique key to the clipboard!\n";

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "[";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	std::cout << "*";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << "]";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	std::cout << " Send your unique key to Aero to get whitelisted.\n";


	size_t Length = strlen(HWID.c_str());

	HWND hwnd = GetDesktopWindow();
	toClipboard(hwnd, HWID);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);

	system("pause");
	return 0;
	VMProtectEnd();
}

void toClipboard(HWND hwnd, const std::string &s) {
	VMProtectBeginMutation("function");
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
	VMProtectEnd();
}

