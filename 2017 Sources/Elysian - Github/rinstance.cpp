#include <Windows.h>
#include "rinstance.h"

using namespace std;

#define GAME_OFF		0xA38

#define GETNAME_OFF		0x28
#define CLASS_OFF		0x10
#define GETPARENT_OFF	0x34
#define GCHILDREN_OFF	0x2C
#define GETLP_OFF		0x16C
#define GETPLAYERS_OFF	0x150
#define PLACEID_OFF		3604

// clone possibly outdated
// destroy possible outdated

RLua::RLua(int game) {
	gameInstance_raw = game;
	gameInstance = game + GAME_OFF;
}

int RLua::GetGameInstance() {
	return gameInstance;
}

int RLua::Clone(int instance) {
	__int64 buffer = 0;
	__asm {
		mov ecx, instance
			mov eax, [ecx]
			lea edx, buffer
			push edx
			call dword ptr[eax + 14h]
	}

	return (int)buffer;
}

std::string* RLua::GetName(int instance) {
	return (std::string*)(*(int*)(instance + GETNAME_OFF));
};

std::string* RLua::GetClass(int instance) {
	__asm {
		mov ecx, instance;
		mov eax, [ecx];
		call dword ptr[eax + CLASS_OFF];
		add eax, 4;
	}
};

int RLua::GetParent(int instance) {
	return *(int*)(instance + GETPARENT_OFF);
}

std::string RLua::GetFullName(int instance) {
	std::string rtn = "";

	int it = instance;
	do {
		if (it == instance) {
			rtn = *GetName(it);
		}
		else {
			rtn = *GetName(it) + "." + rtn;
		}
	} while (it = GetParent(it));

	return rtn;
}

std::vector<int> RLua::GetInstanceTable(int ptr) {
	vector<int> rtn;

	if (!ptr || !*(int*)ptr)
		return rtn;

	int stop_loc = *(int*)(ptr + 4);
	int it = *(int*)ptr;

	while (it != stop_loc) {
		rtn.push_back(*(int*)it);
		it += 8;
	}

	return rtn;
}

vector<int> RLua::GetChildren(int instance) {
	return GetInstanceTable(*(int*)(instance + GCHILDREN_OFF));
}

int RLua::GetChildByName(int instance, const char* name) {
	vector<int> children = GetChildren(instance);

	for (int i = 0; i < children.size(); ++i) {
		if (*GetName(children[i]) == name) {
			return children[i];
		}
	}

	return 0;
};

int RLua::GetChildByClass(int instance, const char* className) {
	vector<int> children = GetChildren(instance);

	for (int i = 0; i < children.size(); ++i)
		if (*GetClass(children[i]) == className)
			return children[i];

	return 0;
};

int RLua::GetInstanceInHierarchy(char** names, int size) {
	int currInstance = 0;
	for (int i = 0; i < size; ++i) {
		int child = GetChildByName(currInstance ? currInstance : gameInstance, names[i]);
		if (child)
			currInstance = child;
		else {
			currInstance = 0;
			break;
		}
	}
	return currInstance;
};

vector<int> RLua::GetPlayers() {
	vector<int> rtn;
	int players;

	if (gameInstance && (players = GetChildByName(gameInstance, "Players"))) {
		rtn = GetInstanceTable(*(int*)(players + GETPLAYERS_OFF));
	}

	return rtn;
};

int RLua::GetPlayerByName(const char* name) {
	vector<int> players = GetPlayers();

	for (int i = 0; i < players.size(); ++i) {
		if (*GetName(players[i]) == name)
			return players[i];
	}

	return 0;
}

void RLua::Destroy(int instance) {
	if (*(int*)instance != 0) {
		__asm {
			mov ecx, instance
			mov eax, [ecx]
			call dword ptr[eax + 12]
		}
	}
};

int RLua::GetLocalPlayer() {
	int players = GetChildByClass(gameInstance, "Players");
	if (players)
		return *(int*)(players + GETLP_OFF);

	return 0;
}

int RLua::ClearInstanceByName(int instance, const char* name) {
	vector<int> children = GetChildren(instance);
	int rtn = 0;

	for (int i = 0; i < children.size(); ++i) {
		if (*GetName(children[i]) == name) {
			Destroy(children[i]);
			++rtn;
		}
	}

	return rtn;
}

int RLua::ClearInstanceByClass(int instance, const char* cname) {
	vector<int> children = GetChildren(instance);
	DWORD rtn = 0;

	for (int i = 0; i < children.size(); ++i) {
		if (*GetClass(children[i]) == cname) {
			Destroy(children[i]);
			++rtn;
		}
	}

	return rtn;
}

int RLua::GetPlaceId() {
	if (gameInstance_raw) return *(int*)(gameInstance_raw + PLACEID_OFF) ^ **(int**)(gameInstance_raw + PLACEID_OFF);
	return 0;
}