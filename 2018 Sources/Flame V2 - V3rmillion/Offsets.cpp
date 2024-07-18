// Offsets.cpp : Defines the exported functions for the DLL application.
//

#include <Windows.h>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <string>

using namespace std;

int DataModel;
int Workspace;
int ScriptContext;

void WriteMemory(DWORD Address, DWORD Instruction) {
	DWORD Protect;
	VirtualProtect((LPVOID)Address, 1, PAGE_EXECUTE_READWRITE, &Protect);
	*(char*)Address = Instruction;
	VirtualProtect((LPVOID)Address, 1, Protect, &Protect);
}


std::vector<std::string> Split(std::string string)
{
	std::vector<std::string> elements;
	std::stringstream start(string);
	std::istream_iterator<std::string> begin(start);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vectorStrings(begin, end);
	return vectorStrings;
}

static int SetParent(int Object, int Parent) {
	if (Object && Parent > 0) {
		*(int*)(Object + 0x34) = Parent;
	}
}

static int GetParent(int Instance) {
	if (Instance > 1000) {
		__try {
			int Parent = *(int*)(Instance + 0x34);
			if (Parent != 0) {
				return Parent;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
	}
	return int(0);
}

static int FindFirstChild(int Instance, std::string Name) {
	try {
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (strstr(GetName(*(int*)i)->c_str(), Name.c_str()))
			{
				return *(int*)i;
			}
		}
		return 0;
	}
	catch (exception) {
		printf("Error!\n");
		Sleep(1);
	}
}

inline void RunCommand(std::string command)
{
	std::vector<std::string> Command = Split(command);

	if (Command.size() > 0) {
		if (Command.at(0) == "setparent") {

		}
	}
}



void Main() {
	DWORD ConsoleCheck = (DWORD)GetProcAddress(GetModuleHandle("Kernel32.dll"), "FreeConsole");
	WriteMemory(ConsoleCheck, 0xC3);

	AllocConsole();

	SetConsoleTitleA("Flame");
	HWND ConsoleHandle = GetConsoleWindow();

	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_SHOW);


	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);

	do {
		std::string ConsoleInput;
		getline(std::cin, ConsoleInput);


	} while (true);
}