/*
* File Name: util.h
* Author(s): Quad Core
*
* Miscellaneous utilities library for ROBLOX hacks
*/

#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <sstream>

#include "globals.h"

using namespace std;

namespace rbx_Instance {
	inline DWORD rbx_GetParent(DWORD Instance) {
		__asm {
			mov ecx, Instance
				mov eax, [ecx + Parent_off]
		}
	}

	inline DWORD rbx_GetLocalPlayer(DWORD Instance) {
		__asm {
			mov ecx, Instance
				mov eax, [ecx + LocalPlayer_off]
		}
	}

	inline const char* rbx_GetClassName(DWORD Instance) {
		__asm {
			mov ecx, Instance
				mov eax, [ecx]
				call dword ptr[eax + ClassName_off]
		}
	}

	inline string rbx_GetName(DWORD Instance) {
		string* Name;
		__asm {
			mov ecx, Instance
				mov eax, [ecx + Name_off]
				mov Name, eax
		}
		return Name->c_str();
	}

	vector<INT> rbx_GetChildren(DWORD Instance) {
		DWORD StartOfChildren = *(DWORD*)(Instance + Start_of_children_off);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + End_of_children_off);
		vector<INT> Children;
		for (INT i = *(INT*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			Children.push_back(*(INT*)i);
		}
		return Children;
	}

	DWORD rbx_FindFirstChild(DWORD Instance, string Child) {
		vector<INT> InstanceChildren = rbx_GetChildren(Instance);
		for (INT c = 0; c < InstanceChildren.size(); c++) {
			if (rbx_GetClassName(InstanceChildren[c]) == Child) {
				return InstanceChildren[c];
			}
		}
		return 0;
	}

	DWORD rbx_GetService(const char* Service) {
		return rbx_FindFirstChild(DataModel, Service); // disgusting
	}
}

void WriteReturn(DWORD addr) {
	DWORD oldP;
	VirtualProtect((PVOID)addr, 1, 0x40, &oldP);
	memset((PVOID)addr, 0xC3, 1);
	VirtualProtect((PVOID)addr, 1, oldP, &oldP);
}

vector<string> SplitString(string str, char delimiter) {
	vector<string> tokens;
	stringstream ss(str);
	string tok;

	while (getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}

void SetThreadIdentity() {
	while (1) {
		DWORD Identity = ScanAddress(0x150AA74);
		__asm {
			mov ecx, Identity
				mov eax, 7
				mov[ecx], eax
		}
	}
}

void StartConsole(const char* Title) {
	WriteReturn((DWORD)FreeConsole);
	AllocConsole();
	SetConsoleTitle(Title);
	freopen("conout$", "w", stdout);
	freopen("conin$", "r", stdin);
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 900, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}

void SubTitle(const char* Message) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	print(Message);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}