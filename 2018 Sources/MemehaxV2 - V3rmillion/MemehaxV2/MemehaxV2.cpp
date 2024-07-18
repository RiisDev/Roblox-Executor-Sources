#include <windows.h>
#include <iostream>
#include <C:\Program Files (x86)\VMProtect Ultimate\Include\C\VMProtectSDK.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <istream>
#include "LocalScriptState.h"
#include "LuaCFunctions.h"
#include "MemoryLocations.h"
#include "VftableScanner.h"
#include "CommandParser.h"
#include "CustomFunctions.h"
#include "BasicMemory.h"
#include "LuaCParser.h"
#include "ContextElevation.h"
#include "Metamethods.h"
#include "Functions.h"
#include "stdafx.h"

extern "C" {
#include "Lua-5.1.5\lua.h"
#include "Lua-5.1.5\lua.hpp"
#include "Lua-5.1.5\lualib.h"
#include "Lua-5.1.5\lauxlib.h"
#include "Lua-5.1.5\luaconf.h"
}

/*
if you're looking at this, please know that i'm not an expert reverse engineer or anything.
this is just a hobby and none of this code could exist without the community laying out the groundwork.
although i understand the code and wrote/rewrote pretty much all of it, i still couldn't have started and achieved without help.
i encourage all of you to not keep knowledge to yourself, not boast over it, not make fun of others for lack of it (*cough* Most rain members and Eternal), etc.
through thick and thin, thank you all for helping me achieve my dream. enjoy this gift.
ps: i left a couple of coding mistakes on purpose. if you want to learn this stuff, it's for the best. 
psps: first person to update and release gets a custom role in meme chat and +2 rep.
- @tepig#3337
*/


unsigned long State, *ScriptContext, storeRet1, storeRet2, storeRet3, Jump, Protect;


std::vector<std::string> Parse(std::string input)
{
	std::string a;
	std::vector<std::string> vstr;
	std::istringstream str(input);
	while (str >> a) {
		vstr.push_back(a);
	}
	return vstr;
}

std::vector<string> ParseByChar(string str, char delimeter) {
	std::vector<string> tokens;
	std::stringstream ss(str);
	string token;

	while (std::getline(ss, token, delimeter)) {
		tokens.push_back(token);
	}

	return tokens;
}

char buff[1024];
DWORD dwRead;

HANDLE ScriptPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\ScriptPipe"),
	PIPE_ACCESS_DUPLEX,
	PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
	PIPE_UNLIMITED_INSTANCES,
	9999,
	9999,
	NMPWAIT_USE_DEFAULT_WAIT,
	NULL);

HANDLE CmdPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\CmdPipe"),
	PIPE_ACCESS_DUPLEX,
	PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
	PIPE_UNLIMITED_INSTANCES,
	9999,
	9999,
	NMPWAIT_USE_DEFAULT_WAIT,
	NULL);

HANDLE LuaCPipe = CreateNamedPipeA(TEXT("\\\\.\\pipe\\LuaCPipe"),
	PIPE_ACCESS_DUPLEX,
	PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
	PIPE_UNLIMITED_INSTANCES,
	9999,
	9999,
	NMPWAIT_USE_DEFAULT_WAIT,
	NULL);

int opencustomlibs(lua_State* L) {
	lua_newtable(L);
	lua_pushcfunction(L, Instance_new);
	lua_setfield(L, -2, "new");
	lua_setglobal(L, "Instance");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_pushcfunction(L, Vector3_new);
	lua_setfield(L, -2, "new");
	lua_setglobal(L, "Vector3");
	lua_settop(L, 0);
	lua_getglobal(L, "Vector3");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_pushcfunction(L, Color3_new);
	lua_setfield(L, -2, "new");
	lua_setglobal(L, "Color3");
	lua_settop(L, 0);
	lua_getglobal(L, "Color3");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_pushstring(L, "deadass should already know that this is locked");
	lua_setfield(L, -2, "__metatable");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushcfunction(L, GameIndex);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, GameNewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, GameCall);
	lua_setfield(L, -2, "__call");
	lua_setmetatable(L, -2);
	lua_setglobal(L, "game");
	lua_settop(L, 0);
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushcfunction(L, WorkspaceIndex);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, WorkspaceNewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, WorkspaceCall);
	lua_setfield(L, -2, "__call");
	lua_setmetatable(L, -2);
	lua_setglobal(L, "workspace");
	lua_settop(L, 0);
	lua_pushcfunction(L, PrintIdentity);
	lua_setglobal(L, "printidentity");
	lua_settop(L, 0);
	lua_pushcfunction(L, Warn);
	lua_setglobal(L, "warn");
	lua_settop(L, 0);
	lua_pushcfunction(L, Print);
	lua_setglobal(L, "print");
	lua_settop(L, 0);
	lua_pushcfunction(L, Wait);
	lua_setglobal(L, "wait");
	lua_settop(L, 0);
	lua_pushcfunction(L, LuaCAPI);
	lua_setglobal(L, "luac");
	lua_settop(L, 0);
	return 1;
}

void Exe(std::string input) {
	lua_State* luaS = luaL_newstate();
	lua_State* LuaS = lua_newthread(luaS);
	luaL_openlibs(LuaS);
	opencustomlibs(LuaS);
	std::string total;
	luaL_dostring(LuaS, input.c_str());
	lua_settop(LuaS, 0);
	lua_close(LuaS);
}

inline void LuaCPipeThread() {
	while (LuaCPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(LuaCPipe, NULL) != FALSE)
		{
			while (ReadFile(LuaCPipe, buff, sizeof(buff) - 1, &dwRead, NULL) != FALSE)
			{
				buff[dwRead] = '\0';

				if (strlen(buff) != NULL) {
					std::istringstream meme(buff);
					std::string line;
					while (std::getline(meme, line))
						LuaCExecute(line);
				}
			}
		}
		DisconnectNamedPipe(LuaCPipe);
	}
} //straight outta google

inline void ScriptPipeThread() {
	while (ScriptPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(ScriptPipe, NULL) != FALSE)
		{
			while (ReadFile(ScriptPipe, buff, sizeof(buff) - 1, &dwRead, NULL) != FALSE)
			{
				buff[dwRead] = '\0';

				if (strlen(buff) != NULL) {
					Exe(buff);
				}
			}
		}
		DisconnectNamedPipe(ScriptPipe);
	}
}

inline void CmdPipeThread() {
	while (CmdPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(CmdPipe, NULL) != FALSE)
		{
			while (ReadFile(CmdPipe, buff, sizeof(buff) - 1, &dwRead, NULL) != FALSE)
			{
				buff[dwRead] = '\0';

				if (strlen(buff) != NULL) {
					Commands(buff);
				}
			}
		}
		DisconnectNamedPipe(CmdPipe);
	}
}

int main()
{
		ScriptContext = (PDWORD)Scan((PCHAR)&location_scriptcontextvftable);
		State = LocalScriptState(ScriptContext);
		ChangeContext();
		std::string welcome = "Welcome!";
		MessageBox(0, welcome.c_str(), "Memehax", 0);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ScriptPipeThread, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LuaCPipeThread, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CmdPipeThread, NULL, NULL, NULL);
    return 0;
}

