#include "Globals.hpp"
#include "Libs/Drawing/Drawing.hpp"

namespace Fluxus {
	namespace Globals {
		HWND RBXHWND = nullptr;
		HWND RobloxHWND() {
			if (RBXHWND == nullptr) {
				RBXHWND = Fluxus::Drawing::GetRobloxHWND();
			}
			return RBXHWND;
		}
		
		std::string Path = "";
		std::string AutoExec = "";

		void SetWorkspacePath(std::string PathStr) {
			Path = PathStr;
		}

		std::string GetWorkspacePath() {
			return Path;
		}

		void SetAutoExec(std::string PathStr) {
			AutoExec = PathStr;
		}

		std::string GetAutoExec() {
			return AutoExec;
		}

		std::ostream& yellow(std::ostream& s)
		{
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hStdout, 6 | FOREGROUND_INTENSITY);
			return s;
		}

		std::ostream& red(std::ostream& s)
		{
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hStdout, 4 | FOREGROUND_INTENSITY);
			return s;
		}

		std::ostream& white(std::ostream& s)
		{
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hStdout, 7 | FOREGROUND_BLUE);
			return s;
		}

		std::ostream& blue(std::ostream& s)
		{
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hStdout, 1 | FOREGROUND_INTENSITY);
			return s;
		}

		std::ostream& green(std::ostream& s)
		{
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hStdout, 2 | FOREGROUND_INTENSITY);
			return s;
		}

		std::ostream& purple(std::ostream& s)
		{
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hStdout, 5 | FOREGROUND_INTENSITY);
			return s;
		}

		std::ostream& grey(std::ostream& s)
		{
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hStdout, 8 | FOREGROUND_INTENSITY);
			return s;
		}

		std::ostream& pink(std::ostream& s)
		{
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hStdout, 4 | 7 | FOREGROUND_INTENSITY);
			return s;
		}

		std::ostream& etc(std::ostream& s)
		{
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hStdout, 4 | 5 | FOREGROUND_INTENSITY);
			return s;
		}

	}
}