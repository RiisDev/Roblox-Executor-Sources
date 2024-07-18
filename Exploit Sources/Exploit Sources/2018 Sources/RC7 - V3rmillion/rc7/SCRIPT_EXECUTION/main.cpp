#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <WinINet.h>
#include <algorithm>
#include <random>
#include <typeinfo>
#include <fcntl.h>
#include <io.h>
#include <TlHelp32.h>
#include "Form.h"
#include "Memcheck.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// Memcheck: 0x53B200

#include "detours.h"



#pragma comment(lib, "detours.lib")



DWORD base = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");
DWORD getaddy(DWORD addr) {
	return (addr - 0x400000 + base);
}

typedef void(__thiscall  * servers)();
servers memcheck = (servers)getaddy(0x5241F0);

void PushInstance(lua_State *L, DWORD addy) {
	lua_newtable(L);
	lua_pushnumber(L, addy);
	lua_setfield(L, -2, "__address");

	lua_getglobal(L, "robject");
	lua_setmetatable(L, -2);
}

namespace Memory {
	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask) return 0;
		return (*szMask) == NULL;
	}

	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
	{
		for (int i = 0; i<(int)dwLen; i++)
			if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
		return 0;
	}

	int Scan(DWORD mode, char* content, char* mask)
	{
		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		for (DWORD lpAddr = 0; lpAddr<0x7FFFFFFF; lpAddr += PageSize)
		{
			DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
			if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
			if (mi.Type == MEM_MAPPED) continue;
			if (mi.Protect == mode)
			{
				int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
				if (addr != 0)
				{
					return addr;
				}
			}
		}
	}
}

int DataModel;
int Workspace;
int Players;
int Lighting;
int LocalPlayer;
std::string* PlayerName;

int GetParent(int Instance) {
	return *(int*)(Instance + 0x34);
}

std::string* GetName(int Instance) {
	return (std::string*)(*(int*)(Instance + 0x28));
}

int FindFirstChild_PartOf(int Instance, std::string Name) {
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

int FindFirstChild(int Instance, std::string Name) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		std::string* CurrentName = GetName(*(int*)i);
		CurrentName->resize(Name.size());
		if (*CurrentName == Name)
		{
			return *(int*)i;
		}
	}
	return 0;
}

typedef int(__thiscall *getClassA)(int self);
getClassA GetClassImpl = (getClassA)0x513150;

const char* GetClass(int self)
{
	//return (const char*)GetClassImpl(self);
	return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
}

typedef int(__thiscall *clrC)(int Instance);
clrC ClearAllChildren = (clrC)0xE3F2C0;

int FindFirstClass(int Instance, const char* Name)
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}

int GetLocalPlayer(int Plr) {
	return *(int*)(Plr + 0x16C);
}

int aGetCharacter(std::string Name) {
	if (Name == "me") {
		int Character = FindFirstChild_PartOf(Workspace, *PlayerName);
		if (Character != 0) {
			return Character;
		}
		else {
			return 0;
		}
	}
	int Character = FindFirstChild_PartOf(Workspace, Name);
	if (Character != 0) {
		return Character;
	}
	else {
		return 0;
	}
}

int aGetPlayer(std::string Name) {
	int Player = FindFirstChild_PartOf(Players, Name);
	if (Player != 0) {
		return Player;
	}
	else {
		return 0;
	}
}

std::vector<int> GetChildren(int Instance) {
	int StartOfChildren = *(int*)(Instance + 0x2C);
	int EndOfChildren = *(int*)(StartOfChildren + 4);
	std::vector<int> Children;
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		Children.push_back(*(int*)i);
	}

	return Children;
}

std::string toLower(const char* str) {
	std::string newStr = std::string(str);
	std::transform(newStr.begin(), newStr.end(), newStr.begin(), ::tolower);
	return newStr;
}

int GetCharacter(std::string name) {
	std::string lowerName = toLower(name.c_str());
	if (name == "me") {
		return FindFirstChild_PartOf(Workspace, *PlayerName);
	}
	DWORD StartOfChildren = *(DWORD*)(Workspace + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		std::string* ThingName = GetName(*(int*)i);
		std::string lowerT = toLower(ThingName->c_str());
		if (lowerT.find(lowerName) != std::string::npos) {
			return *(int*)i;
		}
	}
	return 0;
}

int GetPlayer(std::string name) {
	std::string lowerName = toLower(name.c_str());
	if (name == "me") {
		return FindFirstChild_PartOf(Players, *PlayerName);
	}
	DWORD StartOfChildren = *(DWORD*)(Players + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		std::string* ThingName = GetName(*(int*)i);
		std::string lowerT = toLower(ThingName->c_str());
		if (ThingName->find(name) != std::string::npos) {
			return *(int*)i;
		}
	}
	return 0;
}

int GetPerson(std::string person)
{
	if (person == "Guest ")
	{
		int p = FindFirstChild_PartOf(Workspace, person);
		return p;


	}
	else
	{
		int p = FindFirstChild_PartOf(Workspace, person);
		return p;
	}
}


void SetHumanoidState(int self, int state) {
	int statePtr = *(int*)(self + 0x2DC);
	if (statePtr) {
		*(BYTE*)(self + state + 0x229) = 1; // lets try it?? ok and later im going to try and suck my own DICK
		*(int*)(statePtr + 0x20) = state;
	}
}


int GetFilteringEnabled(int Workspace) {
	return *(BYTE*)(Workspace + 0x1FB);
}

void Init(lua_State* L) {
	PushInstance(L, DataModel);
	lua_setglobal(L, "game");

	PushInstance(L, Workspace);
	lua_setglobal(L, "workspace");

	PushInstance(L, Workspace);
	lua_setglobal(L, "Workspace");
}


BOOL rainbow = false;

// o lol :)
int rState = 0;



bool isFF = false;
bool ContextHooked = false;
bool isSP = false;
bool isFIRE = false;
bool isSMOKE = false;
bool isGOD = false;

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

void ExecuteScript(std::string str) {
	std::vector<std::string> In = split(str);
	lua_State* L = luaL_newstate();
	L = lua_open();
	try {
		int err = luaL_loadstring(L, In.at(1).c_str());
		if (err) {
			const char* errmsg = lua_tostring(L, -1);
			std::string err = "ERROR: ";
			err += errmsg;
			Angel::Form::GetBaseInst()->RawOutputText(err.c_str());
			lua_pop(L, 1);
		}
	}
	catch (std::exception e) {
		std::string err = "C ERROR: ";
		err += e.what();
		Angel::Form::GetBaseInst()->RawOutputText(err.c_str());
	}
	lua_close(L);
}

void rektretcheck() {
	return;
}

void Main() {
	new Angel::Form("ANGEL_FORM_MAIN", "Seven", (Angel::OnLoad)[](Angel::Form* arg1)->void {

		Angel::Form::GetBaseInst()->RawOutputText("Seven v2.1.4", RGB(0, 100, 0));
		Sleep(550);
		Angel::Form::GetBaseInst()->RawOutputText("\r\nChecking for Updates... No Update is an Avaiabale\n\n");
		Angel::Form::GetBaseInst()->RawOutputText("\r\nAuthorizing... ");
		Sleep(450);
		Angel::Form::GetBaseInst()->RawOutputText("Loading Lua File...\n\n");
		DWORD ScriptContextPtr = getaddy(0x108777C);
		DWORD ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextPtr, "xxxx");
		DataModel = GetParent(ScriptContext);
		lua_State* L = luaL_newstate();
		L = lua_open(L);
		Init(L);
		luaL_openlibs(L);
		if (luaL_dofile(L, "C:\\Users\\Oruix\\Documents\\Seven2\\Seven2\\autoexec\\init.lua")) {
			std::string str = lua_tostring(L, -1);
			Angel::Form::GetBaseInst()->RawOutputText("Failed to execute: " + str + "\r\n", RGB(255, 0, 0));
		}
		else {
			std::string a = lua_tostring(L, -1);
			lua_getglobal(L, "doExec");
			lua_pushvalue(L, -2);
			lua_pcall(L, 2, 0, 0);
			Angel::Form::GetBaseInst()->RawOutputText("Executed File: " + a + "\r\n");
		}
	});
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Main, NULL, 0, 0);
		break;
	case DLL_THREAD_ATTACH:  break;
	case DLL_THREAD_DETACH:  break;
	case DLL_PROCESS_DETACH: break;
	}
	return TRUE;
}