#pragma once
#include "SDK.h"

void CreateConsole() {
	WriteProcessMemory(GetCurrentProcess(), *(LPVOID*)&FreeConsole, "\xc3", 1, NULL);
	AllocConsole();
	SetConsoleTitleA("Raise");
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
}


std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;	
	if (s == "") return elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}	

std::string Input() {
	std::string ass;
	getline(std::cin, ass);
	return ass;
}

void welcomeshit(Console* console) {
	*console << DARK_GRAY;
	*console << ".  o ..                    \n";
	*console << " o . o o.o                 \n";
	*console << "      ...oo                \n";
	*console << WHITE;
	*console << "          []               \n";
	*console << "        __[]__   North v1.1.2\n";
	*console << "     __|_o_o_o\\__         \n";
	*console << "     \\\"\"\"\"\"\"\"\"\"\" /        \n";
	*console << "      \\. ..  .  /          \n";
	*console << BLUE;
	*console << " ^^^^^^^^^^^^^^^^^^^^      \n";
	*console << WHITE << "Authenticating....";
	std::string name = Auth::Auth();
	*console << GREEN << "Success" << WHITE << ", Welcome " << CYAN << name << WHITE << "!\n";
}


std::vector<const char*> getChildren(int userdata) {
	int Instance = *(int*)(userdata + 4);
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	std::vector<const char*> ret;
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8) {
		int ptr = *(int*)i;
		ret.push_back((const char*)(*(int*)(ptr + 0x28)));
	}
	return ret;
}

bool validateCommand(std::vector<std::string> params, int args) {
	if (params.size()-1 < args) return false;
	return true;
}