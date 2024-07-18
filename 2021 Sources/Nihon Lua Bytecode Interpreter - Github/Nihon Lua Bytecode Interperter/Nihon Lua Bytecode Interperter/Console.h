#include "Lua.h"
#include "Settings.h"

inline std::ostream& ConsoleColorWhite(std::ostream& s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, 7 | FOREGROUND_BLUE);
	return s;
}
inline std::ostream& ConsoleColorBlue(std::ostream& s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, 1 | FOREGROUND_INTENSITY);
	return s;
}
inline std::ostream& ConsoleColorPurple(std::ostream& s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	return s;
}
inline std::ostream& ConsoleColorRed(std::ostream& s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
	return s;
}
inline std::ostream& ConsoleColorYellow(std::ostream& s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	return s;
}

int ShowConsole(int rL) {
	::ShowWindow(GetConsoleWindow(), SW_SHOW);
	return 1;
}
int HideConsole(int rL) {
	::ShowWindow(GetConsoleWindow(), SW_HIDE);
	return 1;
}