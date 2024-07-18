#pragma once

/* Includes */

#include <SDKDDKVer.h>
#include <windows.h>
#include <string>
#include <sstream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <cctype>
#include <fstream>
#include <Wininet.h>
#include <thread>
#include <locale>
#include <ImageHlp.h>

#include "Memory.h"
#include "RBX.h"

#pragma comment(lib, "wininet")

/* Defines */

#define ScanAddress(Address) (Address - 0x400000 + (DWORD)GetModuleHandle("RobloxPlayerBeta.exe"))

using namespace std;

static int L;
static int DataModel, Workspace, Players, ScriptContext;
static std::string* PlayerName; 
static int PlayerInstance;
static std::string ChatHookPrefix = ";";
static bool Whitelisted;
static bool FilteringProperty;

int ScriptContextVFTable = ScanAddress(0x1189C20);
int XORCONST_Flag = ScanAddress(0x13D8060);
int IdentityFlag = ScanAddress(0x150CB84);

int GETFIELD_ADDRESS = Unprotect(
SignatureScanner::Scan("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x83\xC4\x08\x8B\xCA\x8B\xF8\x8D\x59\x01\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\xFF\x76\x10", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx????xxx")
);

int SETFIELD_ADDRESS = Unprotect(
SignatureScanner::Scan("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x83\xC4\x08\x8B\xCA\x8B\xF8\x8D\x59\x01\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\x89\x45\xF0", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx????xxx")
);

int PCALL_ADDRESS = Unprotect(
SignatureScanner::Scan("\x55\x8B\xEC\x8B\x45\x14\x83\xEC\x08\x53\x56", "xxxxxxxxxxx")
);

int PUSHSTRING_ADDRESS = Unprotect(
SignatureScanner::Scan("\x55\x8B\xEC\x8B\x55\x0C\x85\xD2\x75\x0D", "xxxxxxxxxx")
);

int PUSHVALUE_ADDRESS = Unprotect(
SignatureScanner::Scan("\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\x75\x08\x56\xE8\x00\x00\x00\x00\x8B\x56\x10\x83\xC4\x08", "xxxxxxxxxxxx????xxxxxx")
);

int SETTOP_ADDRESS = Unprotect(
SignatureScanner::Scan("\x55\x8B\xEC\x8B\x4D\x0C\x8B\x55\x08\x85\xC9\x78\x35", "xxxxxxxxxxxxx")
);