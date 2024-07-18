#define _CRT_SECURE_NO_WARNINGS //msvc doesn't like freopen
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>

#include "lua.hpp"

extern "C" {
#include "lauxlib.h"
#include "luaconf.h"
#include "lua.h"
}

using namespace std;

int rbxlua_State;

typedef int(__thiscall * opens)(void* self);
opens OpenState = (opens)0x60F460; // gets the rbxlua_state.






int DataModel;
int Workspace;
int Players;
int Lighting;
int State;
bool Changing;
int rState = 0;
bool rainbow = false;
std::string* PlayerName;

void PushInstance(lua_State *L, DWORD addy) {
	lua_newtable(L);
	lua_pushnumber(L, addy);
	lua_setfield(L, -2, "__address");

	lua_getglobal(L, "robject");
	lua_setmetatable(L, -2);
}







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

const char* GetClass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
}

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

int GetCharacter(std::string Name) {
	if (Name == "me") {
		int Character = FindFirstChild_PartOf(Workspace, *PlayerName);
		if (Character != 0) {
			return Character;
		}
		else
		{
			return 0;
		}
	}
	int Character = FindFirstChild_PartOf(Workspace, Name);
	if (Character != 0) {
		return Character;
	}
	else
	{
		return 0;
	}
}
int GetPlayer(std::string Name) {
	int Player = FindFirstChild_PartOf(Players, Name);
	if (Player != 0) {
		return Player;
	}
	else
	{
		return 0;
	}
}

//_______________________________________________________________________________________________

/// STRUCT INTERACTION FUNCTIONS

void SetHumanoidState(int self, int state) {
	int sPtr = *(int*)(self + 0x2DC);
	if (sPtr) {
		*(BYTE*)(self + state + 0x229) = 1;
		*(int*)(sPtr + 0x20) = state;
	}
}

static int LuaFindFirstChild(lua_State* L) {
	const char* Name = lua_tostring(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, -1, "__address");
	int rInstance = lua_tonumber(L, -1);
	lua_pop(L, 2);
	int Instance = FindFirstChild(rInstance, Name);
	if (Instance != 0) {
		PushInstance(L, Instance);
		return 1;
	}
	else {
		return 0;
	}
}

static int GetChildren(lua_State* L) {
	lua_getfield(L, -1, "__address");
	int Instance = lua_tonumber(L, -1);
	lua_pop(L, 2);
	lua_newtable(L);
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		PushInstance(L, *(int*)i);
		lua_setfield(L, -2, std::to_string(i).c_str());
	}
	return 1;
}


static int Index(lua_State* L) {
	std::string Key = lua_tostring(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, -1, "__address");
	int AddressLoc = lua_tonumber(L, -1);
	lua_pop(L, 2);

	int* userdata = (int*)lua_newuserdata(L, 8);
	*userdata = AddressLoc;
	*(userdata + 1) = 0;
	lua_getfield(L, -10000, "Object");
	lua_setmetatable(L, -2);

	if (Key == "GetChildren" || Key == "getChildren" || Key == "children") {
		lua_pushcfunction(L, GetChildren);
		return 1;
	}
	else if (Key == "FindFirstChild" || Key == "findFirstChild") {
		lua_pushcfunction(L, LuaFindFirstChild);
		return 1;
	}
	else if (Key == "Parent") {
		PushInstance(L, GetParent(AddressLoc));
		return 1;
	}
	else {
		lua_getfield(L, -1, Key.c_str());
		int a = lua_type(L, -1);
		switch (lua_type(L, -1)) {
		case LUA_TNIL: {
			return 0;
		}
		case 2:
		case LUA_TNUMBER: {
			const char* str = lua_tolstring(L, -1, NULL);
			lua_pushnumber(L, ::atof(str));
			return 1;
		}
		case LUA_TSTRING: {
			const char* str = lua_tolstring(L, -1, NULL);
			lua_pushstring(L, str);
			return 1;
		}
		case 8: {
			int instance = FindFirstChild(AddressLoc, Key);
			PushInstance(L, instance);
			return 1;
		}
		}
	}
}

static int Tick(lua_State *L) {
	lua_pushnumber(L, std::time(0));
	return 1;
}

static int PrintIdentity(lua_State *L) {
	lua_getfield(L, 5000, "printidentity");
	int ArgCount = 0;
	if (lua_gettop(L) != 0) {
		for (int x = lua_gettop(L); x != 0; x -= 1) {
			switch (lua_type(L, x * -1)) {
			case LUA_TNIL: {
				lua_pushnil(L);
				break;
			}
			case LUA_TNUMBER: {
				double no = lua_tonumber(L, x * -1);
				lua_pushnumber(L, no);
				break;
			}
			case LUA_TSTRING: {
				const char* str = lua_tostring(L, x * -1);
				lua_pushstring(L, str);
				break;
			}
			case LUA_TTABLE: {
				lua_getfield(L, -1, "__address");
				if (!lua_isnoneornil(L, -1)) {
					int* userdata = (int*)lua_newuserdata(L, 8);
					*userdata = lua_tonumber(L, x * -1);
					*(userdata + 1) = 0;
					lua_getfield(L, -10000, "Object");
					lua_setmetatable(L, -2);
				}
				else {
					lua_pop(L, 1);
					lua_pushstring(L, "table: #NULL");
				}
				break;
			}
			default:
			{
				const char* str = lua_tostring(L, x * -1);
				lua_pushstring(L, str);
				break;
			}
			}
			ArgCount += 1;
		}
	}
	lua_pcall(L, ArgCount, 0, 0);
	return 0;
}

static int NewIndex(lua_State* L) {
	int Type = lua_type(L, -1);

	std::string Key = lua_tostring(L, -2);

	lua_getfield(L, -3, "__address");
	int AddressLoc = lua_tonumber(L, -1);
	lua_pop(L, 1);

	int* userdata = (int*)lua_newuserdata(L, 8);
	*userdata = AddressLoc;
	*(userdata + 1) = 0;
	lua_getfield(L, -10000, "Object");
	lua_setmetatable(L, -2);

	switch (Type) {
	case LUA_TNUMBER: {
		double no = lua_tonumber(L, -1);
		lua_pushnumber(L, no);
		lua_setfield(L, -2, Key.c_str());
		return 0;
	}
	case LUA_TSTRING: {
		const char* str = lua_tostring(L, -1);
		lua_pushstring(L, str);
		lua_setfield(L, -2, Key.c_str());
		return 0;
	}
	case LUA_TBOOLEAN: {
		bool a = lua_toboolean(L, -1);
		if (a) {
			lua_pushnumber(L, 1);
		}
		else {
			lua_pushnil(L);
		}
		lua_setfield(L, -2, Key.c_str());
		return 0;
	}
	case LUA_TTABLE: {
		lua_getfield(L, -1, "__address");
		if (!lua_isnil(L, -1)) {
			int inst = lua_tonumber(L, -1);
			lua_pop(L, 1);
			int* userdata = (int*)lua_newuserdata(L, 8);
			*userdata = inst;
			*(userdata + 1) = 0;
			lua_getfield(L, -10000, "Object");
			lua_setmetatable(L, -2);
			lua_setfield(L, -2, Key.c_str());
		}
		else {
			lua_pop(L, 1);
		}
		return 0;
	}
	}
}

void Init(lua_State* L) {
	lua_newtable(L);
	lua_pushcfunction(L, Index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, NewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_setglobal(L, "robject");

	PushInstance(L, DataModel);
	lua_setglobal(L, "game");

	PushInstance(L, Workspace);
	lua_setglobal(L, "workspace");

	PushInstance(L, Workspace);
	lua_setglobal(L, "Workspace");
}

const char *Commands =
"- noclip - Walk through walls.\r\n"
"- swim - Swim in the air. Like fly.\r\n"
"- superjump - Jump in the air.\r\n"
"- airwalk - Walk in the air.\r\n"
"- jesusfly - Fly on drugs.\r\n"
"- ragdoll - Absolutely useless but fun.\r\n"
"- disable - Disable any command you activated.\r\n"
"- ws [n] - Set your walkspeed to [n].\r\n"
"- time [n] - Set time (in hours) to [n]\r\n"
"- restart - If you crashed, just type this.\r\n"
"- loadstring [s] - Execute script.\r\n";

void Console(char* title) {
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}

std::string IntToHex(int Val) {
	std::stringstream stream;
	stream << std::hex << Val;
	return stream.str();
}

std::string Input() {
	std::string wotthefuck;
	getline(std::cin, wotthefuck);
	return wotthefuck;
}

void doState() {
	if (Changing == true) {
		int LocalCharacter = GetCharacter(*PlayerName);
		if (LocalCharacter != 0) {
			int Humanoid = FindFirstClass(LocalCharacter, "Humanoid");
			if (Humanoid != 0) {
				SetHumanoidState(Humanoid, State);
			}
		}
	}
}

int ScriptContextVftable;
int ScriptContext;

namespace Memory 

void Start() {
	Console("SGExploitV3 made by SG JHawk");
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	cout << "Scanning... " << endl;
	ScriptContextVftable = 0xF7CBE4;
	ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
	DataModel = GetParent(ScriptContext); // lol
	Workspace = FindFirstClass(DataModel, "Workspace");
	Lighting = FindFirstClass(DataModel, "Lighting");
	Players = FindFirstClass(DataModel, "Players");
	int LocalPlayer = GetLocalPlayer(Players);
	PlayerName = GetName(LocalPlayer);
	rbxlua_State = OpenState((void*)ScriptContext);
	cout << "Lua State: " << IntToHex(rbxlua_State) << std::endl;
	cout << "Seller and Reseller is SonOfAvunit and Peb" << endl;
	while (true) {
		std::string i = Input();
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		Init(L);
		try {
			int err = luaL_dostring(L, i.c_str());
			if (err) {
				const char* errmsg = lua_tostring(L, -1);
				std::string err = "ERROR: ";
				err += errmsg;
				std::cout << err.c_str() << std::endl;
				lua_pop(L, 1);
			}
		}
		catch (std::exception e) {
			std::string err = "C ERROR: ";
			err += e.what();
			std::cout << err.c_str() << std::endl;
		}
		lua_close(L);
	}
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Start, 0, 0, 0); //lets go
	}
	return 1;
}


