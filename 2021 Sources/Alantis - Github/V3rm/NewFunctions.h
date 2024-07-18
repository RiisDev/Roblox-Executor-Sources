#pragma once
#include <WinInet.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "Rlua.h"
#include "DETOURS\detours.h"
#pragma comment(lib, "wininet.lib")

extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lobject.h"
#include "Lua/llimits.h"
}

using namespace std;

union r_Value
{
	void* gc;
	void* p;
	double n;
	bool b;
};

typedef struct r_TValue
{
	r_Value value;
	BYTE tt;
	BYTE shit;
};

std::string GetDLLPath(const char* Addon) //Apppon
{
	HMODULE hModule = GetModuleHandleA(Adresses::name);
	if (hModule == NULL)
		return "";
	char buffer[MAX_PATH + 1];
	GetModuleFileNameA(hModule, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos).append(Addon);
}

std::string DownloadURL(const char* URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	std::string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL, NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			std::string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	std::string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

static int DownloadString2(lua_State* L) {
	print("Called!");
	int args = lua_gettop(L) - 1;
	print("Args Ready! String: " << lua_tostring(L, 1));
	std::string url;
	url = DownloadURL(lua_tostring(L, -1));
	print("Retrieved URL!");
	lua_pushstring(L, url.c_str());
	print("Sent String!");
	return 1;
}

static int DownloadString(DWORD L) {
	int args = r_lua_gettop(L);
	print("Args: " << to_string(args));
	std::string url;
	if (args == 2) {
		url = DownloadURL(rlua_tostring(L, -2));
	}
	else if (args == 1) {
		url = DownloadURL(rlua_tostring(L, -1));
	}
	r_lua_pushstring(L, url.c_str());
	return 1;
}

int executeFunction(DWORD Ls) {
	const char* script = rlua_tostring(Ls, -1);
	ExecuteScript(RL, L, script);
	return 1;
}

int ShowConsole(DWORD L) {
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);

	return 1;
}

int HideConsole(DWORD L) {
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	return 1;
}

int copyclipboard(DWORD L) {
	string name = rlua_tostring(L, -1);
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, name.size());
	if (!hg) {
		CloseClipboard();
	}
	memcpy(GlobalLock(hg), name.c_str(), name.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
	return 1;
}

std::string dex = R"(--[[ Dex Loader by cirmolddry#2299 ]] 
local Dex = game:GetService('InsertService'):LoadLocalAsset('rbxassetid://2063968168')
math.randomseed(tick())

local charset = {}
for i = 48,  57 do table.insert(charset, string.char(i)) end
for i = 65,  90 do table.insert(charset, string.char(i)) end
for i = 97, 122 do table.insert(charset, string.char(i)) end
function RandomCharacters(length)
  if length > 0 then
    return RandomCharacters(length - 1) .. charset[math.random(1, #charset)]
  else
    return ""
  end
end

Dex.Name = RandomCharacters(math.random(5, 20))
Dex.Parent = game:GetService("CoreGui")
	
local function Load(Obj, Url)
	local function GiveOwnGlobals(Func, Script)
		local Fenv = {}
		local RealFenv = {script = Script}
		local FenvMt = {}
		FenvMt.__index = function(a,b)
			if RealFenv[b] == nil then
				return getfenv()[b]
			else
				return RealFenv[b]
			end
		end
		FenvMt.__newindex = function(a, b, c)
			if RealFenv[b] == nil then
				getfenv()[b] = c
			else
				RealFenv[b] = c
			end
		end
		setmetatable(Fenv, FenvMt)
		setfenv(Func, Fenv)
		return Func
	end
	
	local function LoadScripts(Script)
		if Script.ClassName == "Script" or Script.ClassName == "LocalScript" then
			spawn(GiveOwnGlobals(loadstring(Script.Source, "=" .. Script:GetFullName()), Script))
		end
		for i,v in pairs(Script:GetChildren()) do
			LoadScripts(v)
		end
	end
	
	LoadScripts(Obj)
end

Load(Dex))";

int scriptHub(DWORD Ls) {
	const char* scriptType = rlua_tostring(Ls, -1);

	if (scriptType == "SCRIPT_DEX") {
		ExecuteScript(RL, L, dex);
	}
	return 1;
}

string bytes(string bytecode) { //Deeppain
	string bytecodetoreturn;
	for (int i = 0; i < bytecode.length(); i += 2) {
		char push = (char)(int)strtol(bytecode.substr(i, 2).c_str(), 0, 16);
		bytecodetoreturn.push_back(push);
	}
	return bytecodetoreturn;
}

bool executeBytecode_2(DWORD rL, string bytecode) {
	string bytecodeFormatted = bytes(bytecode);
	DWORD Thread = r_lua_newthread(rL);
	r_luau_deserialize(Thread, "@AtlantisLU", bytecodeFormatted.c_str(), bytecodeFormatted.size());
	r_lua_spawn(Thread);
	return true;
}

int execBytecode(DWORD Ls) {
	const char* bytecodeData = rlua_tostring(Ls, -1);
	r_lua_pushboolean(Ls, executeBytecode_2(RL, bytecodeData));
	return 1;
}

int conIn(DWORD L) {
	const char* write = rlua_tostring(L, -1);

	if (::ShowWindow(GetConsoleWindow(), SW_SHOW)) {
		print(write);
	}

	return 1;
}

std::string asciiart = R"(  ____        _                     _      
 |  _ \      | |                   | |     
 | |_) |_   _| |_ ___  ___ ___   __| | ___ 
 |  _ <| | | | __/ _ \/ __/ _ \ / _` |/ _ \
 | |_) | |_| | ||  __/ (_| (_) | (_| |  __/
 |____/ \__, |\__\___|\___\___/ \__,_|\___|
         __/ |                             
        |___/                              )";

int deserializeHook(int a1, int a2, int a3, int a4) {
	std::cout << " += Dumping Bytecode..." << endl;
	Sleep(15);
	std::cout << " += Bytecode Read:" << endl;
	BYTE* bytecode = new BYTE[a4];
	memcpy((void*)bytecode, (void*)a3, a4);
	int form = 0;
	for (int i = 0; i < form; i++) {
		if (form++ > 512) {
			printf("\n");
			form = 0;
		}
		printf("%02X", bytecode[i]);
	}
	std::cout << " += Bytecode Information: " << endl << "     - Roblox State: " << to_string(a1) << endl << "     - Name: " << to_string(a2) << endl << "     - Bytecode Source: " << to_string(a3) << endl << "     - Size: " << to_string(a4) << endl << " += Bytecode Dumper Developed by Nexus42" << endl << endl;
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(LPVOID&)luau_deserialize_addr, deserializeHook);
	DetourTransactionCommit();
	return r_luau_deserialize(a1, (const char*)a2, (const char*)a3, a4);
}

int dump(DWORD Ls) {
	system("cls");
	system("color e");
	::ShowWindow(GetConsoleWindow(), SW_SHOW);
	std::cout << asciiart << endl << endl;
	print("Initializing Dumper...");
	Sleep(450);
	string script = rlua_tostring(Ls, -1);
	string luaTranspile = LuauTranspiler::compile(lua_open(), script).c_str();
	print("Transpiled!" << endl << endl << luaTranspile.c_str());
	Sleep(450);
	system("color b");
	system("cls");
	std::cout << logo << endl;
	print("Ready!");
	r_lua_pushstring(Ls, luaTranspile.c_str());
	return 1;
}

int getgenv(DWORD LS) {
	r_lua_pushvalue(LS, LUA_GLOBALSINDEX);
	return 1;
}

int getreg(DWORD LS) {
	r_lua_pushvalue(LS, LUA_REGISTRYINDEX);
	return 1;
}

int getrenv(DWORD LS) {
	r_lua_pushvalue(LS, RL);
	return 1;
}

int getfenv(DWORD LS) {
	r_lua_pushvalue(LS, LS);
	return 1;
}

int impl_getgc(DWORD rL)
{
	auto IncludeTables = false;
	IncludeTables = r_lua_toboolean(rL, -1);
	const auto GlobalState = RBX_GlobalState(rL);
	const auto DeadMask = *(BYTE*)(GlobalState + 20);
	auto Object = *(GCObject**)(GlobalState + 48); // to do: define these you can remake rluaC_link with these 
	r_lua_createtable(rL, 0, 0);
	auto n = 1;
	while (Object != nullptr)
	{
		const auto TT = *(BYTE*)((DWORD)Object + 5);

		if ((TT == js["offsets"]["R_LUA_TFUNCTION"] || (IncludeTables ? TT == js["offsets"]["R_LUA_TTABLE"] : TT == js["offsets"]["R_LUA_TFUNCTION"]) || (IncludeTables ? TT == js["offsets"]["R_LUA_TUSERDATA"] : TT == js["offsets"]["R_LUA_TFUNCTION"])) && (*(BYTE*)((DWORD)Object + 4) & 3) & DeadMask)
		{
			r_lua_pushnumber(rL, n++);
			r_TValue* top = *(r_TValue**)(rL + 0xC);
			top->tt = js["offsets"]["R_LUA_TFUNCTION"];
			top->value.gc = (GCObject*)(TT);
			*reinterpret_cast<uintptr_t*>(rL + 0xC) += sizeof(r_TValue); // modified specially for it to WORK on fucking getgc, was this that was causing the problem the whole time
			r_lua_settable(rL, -3);
		}
		Object = Object->gch.next;
	}
	return 1;
}

//My File System B)
namespace fileSystem {
	int readFile(DWORD LS) {
		int args = r_lua_gettop(LS);
		if (args == 0) {
			rluaL_error(LS, "Invalid Arguments");
			return 0;
		}
		string fileName = rlua_tostring(LS, -1);
		string name = GetDLLPath("\\Workspace\\") + fileName;
		string content;
		ifstream rFile;
		rFile.open(name, ios::in | ios::out);
		rFile >> content;
		rFile.close();
		r_lua_pushstring(LS, content.c_str());
		return 1;
	}

	int writeFile(DWORD LS) {
		int args = r_lua_gettop(LS);

		if (args < 2) {
			return rluaL_error(LS, "Invalid Arguments");;
		}
		print("Loaded!");
		string fileName = rlua_tostring(LS, -2);
		string name = GetDLLPath("\\Workspace\\") + fileName;
		string text = rlua_tostring(LS, -1);
		std::wstring ws;
		ws.assign(name.begin(), name.end());
		print("Initialized!");
		HANDLE file = CreateFile(ws.c_str(), GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE) {
			print("Invalid Handle!");
			return rluaL_error(LS, "Invalid Handle");;
		}
		print("Valid Handle!");
		DWORD dw;
		WriteFile(file, text.c_str(), text.size(), &dw, NULL);
		print("Completed!");
		CloseHandle(file);
		return 1;
	}
}

int getsenv(DWORD LS) {
	r_lua_pushvalue(LS, LUA_ENVIRONINDEX);
	return 1;
}

int sysExec(DWORD LS) {
	system(rlua_tostring(LS, -1));
	return 1;
}

int GetRawMeta(DWORD LS) {
	r_lua_getmetatable(LS, -1);
	return 1;
}

namespace zexea {
	//getfflag -
		int getfflag(DWORD rL)
	{
		r_lua_getglobal(rL, "GlobalSettings");
		r_lua_getfield(rL, -1, "GetFFlag");
		r_lua_pushstring(rL, r_lua_tolstring(rL, -1, 0));
		r_lua_pcall(rL, 1, 1, 0);
		bool fflag_result = r_lua_toboolean(rL, -1);
		r_lua_pushboolean(rL, fflag_result);
		return 1;
	}
	//readonly& writeable -
		int make_writeable(DWORD rL)
	{
		r_lua_setreadonly(rL, 1, false);
		return 1;
	}

	int make_readonly(DWORD rL)
	{
		r_lua_setreadonly(rL, 1, true);
		return 1;
	}
	//getupvalues -
		static int getupvalues(DWORD rL)
	{
		if (r_lua_gettop(rL) < 2)
			return rluaL_error(rL, "debug.getupvalues requires only one argument.");
		lua_newtable(L);
		int i = 1;
		while (const char* name = r_lua_getupvalues(rL, -2, i))
		{
			r_lua_pushstring(rL, name);
			r_lua_pushvalue(rL, -2);
			r_lua_settable(rL, -4);
			r_lua_pop(rL, 1);
			i++;
		}
		return 1;
	}
}

int FireClickDetector(DWORD rL)
{
	const uintptr_t Click = *reinterpret_cast<uintptr_t*>(r_lua_touserdata(RL, -1));
	float Distance = 0;
	if (r_lua_gettop(rL) > 1)
		Distance = r_lua_tonumber(rL, 2);
	r_lua_getglobal(RL, "game");
	r_lua_getfield(RL, -1, "GetService");
	r_lua_insert(RL, -2);
	r_lua_pushstring(RL, "Players");
	r_lua_pcall(RL, 2, 1, 0);
	r_lua_getfield(RL, -1, "LocalPlayer");
	const uintptr_t LP = *reinterpret_cast<uintptr_t*>(r_lua_touserdata(RL, -1));
	r_fireclickdetector(Click, Distance, LP);
	r_lua_pop(RL, 2);
	return 0;
}

int getfflag_flog(DWORD rL) {
	if (r_lua_gettop(rL) == 1) {
		r_lual_checktype(rL, -1, js["offsets"]["R_LUA_TSTRING"]);
		string xd;
		r_flog_getvalue(rlua_tostring(rL, -1), xd, true);
		r_lua_pushstring(rL, xd.c_str());
		return 1;
	}
	else {
		r_lua_getglobal(RL, "error");
		r_lua_pushstring(RL, "Invalid Argument Count.");
		r_lua_pcall(RL, 1, 0, 0);
		r_lua_pop(RL, 0);
		return 1;
	}	
}

int setfflag(DWORD rL) {
	string flag = rlua_tostring(rL, -2);
	string value = rlua_tostring(rL, -1);
	bool sex = r_flog_setvalue(flag, value, 63, 0);
	r_lua_pushboolean(rL, sex);
	return 1;
}

int setreadonlyfunc(DWORD rL) {
	r_lual_checktype(rL, -2, js["offsets"]["R_LUA_TTABLE"]);
	r_lual_checktype(rL, -1, js["offsets"]["R_LUA_TBOOLEAN"]);
	r_lua_setreadonly(rL, -2, r_lua_toboolean(rL, -1));
	return 1;
}

int isreadonly(DWORD rL) {
	r_lual_checktype(rL, -1, js["offsets"]["R_LUA_TTABLE"]);
	r_lua_pushboolean(rL, *(BYTE*)(r_lua_topointer(rL, -1) + READONLY));
	return 1;
}

int setreadonly_deep(DWORD rL) {
	r_lual_checktype(rL, -2, js["offsets"]["R_LUA_TTABLE"]);
	r_lual_checktype(rL, -1, js["offsets"]["R_LUA_TBOOLEAN"]);

	bool protection = r_lua_toboolean(rL, 2);

	int table_addr = r_lua_topointer(rL, 1);
	*reinterpret_cast<BYTE*>(table_addr + 10) = protection; /*offset changes each week*/
	return 0;
}

int isreadonly_deep(DWORD rL) {
	r_lual_checktype(rL, -1, js["offsets"]["R_LUA_TTABLE"]);

	int table_addr = r_lua_topointer(rL, 1);
	r_lua_pushboolean(rL, *reinterpret_cast<BYTE*>(table_addr + READONLY)); /*offset changes each week*/
	return 1;
}

void updateFunctions() {
	r_luaL_register(RL, "showConsole", ShowConsole);
	r_luaL_register(RL, "hideConsole", HideConsole);
	r_luaL_register(RL, "httpget", DownloadString);
	r_luaL_register(RL, "loadstring", executeFunction);
	r_luaL_register(RL, "writeConsole", conIn);
	r_luaL_register(RL, "loadScript", scriptHub);
	r_luaL_register(RL, "copy", copyclipboard);
	r_luaL_register(RL, "bytecode", execBytecode);
	r_luaL_register(RL, "dump", dump);
	r_luaL_register(RL, "getgenv", getgenv);
	r_luaL_register(RL, "getreg", getreg);
	r_luaL_register(RL, "getrenv", getrenv);
	r_luaL_register(RL, "getsenv", getsenv);
	r_luaL_register(RL, "getrawmetatable", GetRawMeta);
	r_luaL_register(RL, "fireclickdetector", FireClickDetector);
	r_luaL_register(RL, "setfflag", setfflag);
	r_luaL_register(RL, "isreadonly", isreadonly);
	r_luaL_register(RL, "setreadonly", setreadonlyfunc);

	r_luaL_register(RL, "writefile", fileSystem::writeFile);
	r_luaL_register(RL, "readfile", fileSystem::readFile);

	r_luaL_register(RL, "debug.getupvalues", zexea::getupvalues);
	r_luaL_register(RL, "getfflag", getfflag_flog);
	r_luaL_register(RL, "make_writable", zexea::make_writeable);
	r_luaL_register(RL, "make_readonly", zexea::make_readonly);

	r_luaL_register(RL, "getgc", impl_getgc);
	//r_luaL_register(RL, "system", sysExec);
}