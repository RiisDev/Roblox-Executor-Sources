#pragma once
#include "SDK.h"

int ScriptContext;
int DataModel;
int Players;
int LocalPlayer;
std::string PlayerName;

int getParent(int Instance) {
	return *(int*)(Instance + 0x34);
}

std::string getName(DWORD Instance) {
	std::string *Result;
	__asm {
		mov ecx, Instance
		mov eax, [ecx + 0x28]
		mov Result, eax
	}
	return Result->c_str();
}

const char* getClass(int Self) {
	return (const char*)(*(int(**)(void))(*(int*)Self + 16))();
}

int findFirstClass(int Instance, const char* Name)
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(getClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}

int getService(const char* Name) {
	DWORD StartOfChildren = *(DWORD*)(DataModel + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(getClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}

	return 0;
}

int getLocalPlayer(int Player) {
	return *(int*)(Player + 340);
}

std::vector<std::string> SplitString(std::string str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string tok;

	while (getline(ss, tok, delimiter))
		tokens.push_back(tok);

	return tokens;
}

std::vector<int> getChildren(DWORD Instance) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	std::vector<int> Children;
	for (DWORD i = *(DWORD*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		Children.push_back(*(DWORD*)i);
	}
	return Children;
}

std::vector<int> GetPlayers(std::string Name) {
	std::vector<int> P;
	std::vector<std::string> Arguments = SplitString(Name, ',');

	for (DWORD i = 0; i < Arguments.size(); i++) {
		std::string Input = Arguments[i];
		transform(Input.begin(), Input.end(), Input.begin(), tolower);

		if (Input == "me" || Input == "Me") {
			P.push_back(LocalPlayer);
		}

		else if (Input == "all" || Input == "All") {
			P = getChildren(Players);
		}

		else if (Input == "others" || Input == "Others") {
			std::vector<int> P2 = getChildren(Players);
			for (DWORD i = 0; i < P2.size(); i++) {
				if (getName(P2[i]) != getName(LocalPlayer)) {
					P.push_back(P2[i]);
				}
			}
		}

		else if (Input == "guests" || Input == "Guests" || Input == "guest" || Input == "Guest") {
			std::vector<int> Plrs = getChildren(Players);
			for (int i = 0; i < Plrs.size(); i++) {
				if (getName(Plrs[i]).find("Guest ") != std::string::npos) {
					P.push_back(Plrs.at(i));
				}
			}
		}
		else if (Input == "random" || Input == "Random") {
			std::vector<int> Plrs = getChildren(Players);
			if (Plrs.size() > 0) {
				std::random_shuffle(Plrs.begin(), Plrs.end());
				P.push_back(Plrs[0]);
			}
		}

		else {
			std::vector<int> P2 = getChildren(Players);
			for (DWORD i = 0; i < P2.size(); i++) {
				std::string P2Name = getName(P2[i]);
				transform(P2Name.begin(), P2Name.end(), P2Name.begin(), tolower);
				if (P2Name.substr(0, Input.length()) == Input) {
					P.push_back(P2[i]);
				}
			}
		}
	}
	return P;
}