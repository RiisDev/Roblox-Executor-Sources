#ifndef ALX_VERSION
#include <Windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <algorithm>

#include "rlua.h"

#define ALXERR 200
#define ALXRESTART	100

#define ALX_VERSION		"2"

extern DWORD ROBLOX_BASE;
extern DWORD ALX_BASE;
extern DWORD GameInstance;
extern DWORD LocalPlayer;
extern std::string LocalPlayerName;
extern RLua* rLua;

typedef struct PEntry { std::string* pName; float pDistance; };

void Initiate();
void CreateConsole();
BOOLEAN DoesConsoleExist();
int main();
void HandleCommand(std::string command);
std::vector<DWORD> GetPlayersFromMessage(DWORD gameInstance, std::vector<std::string> argList);
#endif
