#pragma once
#include <cstdint>
#include <queue> 
#include "RetCheck.h"
#include "addys.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lobject.h"
#include "Lua/lstate.h"
#include "Lua/lfunc.h"
#include "Lua/lopcodes.h"
#include "Lua/lstring.h"
#include "Lua/ldo.h"
#include "Lua/llex.h"
#include "Lua/lvm.h"
}

json js;

using namespace Adresses;

std::queue<std::string> ScriptQueue;

DWORD EmptySpace1 = Identity1; //Identity1
DWORD EmptySpace2 = Identity1; //Identity2

DWORD Add(DWORD func);
typedef int (*lRua_CFunction) (DWORD rL);
#define ASLR(x)(x - 0x400000 + (DWORD)GetModuleHandleA(0))

#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))
#define r_lua_setglobal(rL,s)	r_lua_setfield(rL, LUA_GLOBALSINDEX, (s))
#define print(x) std::cout << " X " << ConsoleColors::White() << "[" << ConsoleColors::LightAqua() << "Atlantis" << ConsoleColors::White() << "] " << ConsoleColors::LightGreen() << x << ConsoleColors::LightAqua() << endl;

#define RLUA_TNIL LUA_TNIL
#define RLUA_TLIGHTUSERDATA LUA_TLIGHTUSERDATA
#define RLUA_TNUMBER LUA_TNUMBER
#define RLUA_TBOOLEAN LUA_TBOOLEAN
#define RLUA_TSTRING LUA_TSTRING
#define RLUA_TTHREAD LUA_TTHREAD
#define RLUA_TFUNCTION LUA_TFUNCTION
#define RLUA_TTABLE LUA_TTABLE
#define RLUA_TUSERDATA LUA_TUSERDATA
#define R_LUA_TPROTO LUA_TPROTO
#define R_LUA_TUPVALUE LUA_TUPVALUE

using namespace std;

DWORD RL;
lua_State* L;
std::string LBI;

DWORD TOP;
DWORD BASE;
DWORD IDENTITY;
DWORD OFFSET;
string SYMBOL;
DWORD READONLY;

namespace ConsoleColors
{
	std::string DarkBlue()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
		return "";
	}
	std::string DarkGreen()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
		return "";
	}
	std::string LightBlue()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
		return "";
	}
	std::string DarkRed()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		return "";
	}
	std::string DarkPurple()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
		return "";
	}
	std::string Orange()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
		return "";
	}
	std::string Grey()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		return "";
	}
	std::string Aqua()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		return "";
	}
	std::string LightGreen()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		return "";
	}
	std::string LightAqua()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		return "";
	}
	std::string LightRed()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		return "";
	}
	std::string LightPurple()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
		return "";
	}
	std::string Yellow()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		return "";
	}
	std::string White()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		return "";
	}
}

std::function<DWORD(DWORD)> r_lua_newthread = nullptr;
std::function<DWORD(DWORD, int, const char*)> r_lua_getfield = nullptr;
std::function<void(DWORD, DWORD)> r_lua_pushvalue = nullptr;
std::function<void(DWORD, const char*)> r_lua_pushstring = nullptr;
std::function<void(DWORD, int, const char*)> r_lua_setfield = nullptr;
std::function<DWORD(DWORD, int, int, int)> r_lua_pcall = nullptr;
std::function<DWORD(int, int, int, int, int)> r_lua_pushcclosure = nullptr;
std::function<void(DWORD, int)> r_lua_settop = nullptr;
std::function<DWORD(DWORD, const char*)> r_lual_error = nullptr;
std::function<DWORD(DWORD, const char*, const char*, size_t)> r_luau_deserialize = nullptr;
std::function<const char* (int, int, int)> r_lua_tolstring = nullptr;
std::function<bool(DWORD, int)> r_lua_toboolean = nullptr;
std::function<DWORD(int, int, int)> r_fireclickdetector = nullptr;
std::function<void(DWORD, int)> r_lua_insert = nullptr;
std::function<double(DWORD, int)> r_lua_tonumber = nullptr;
std::function<void* (DWORD, int)> r_lua_touserdata = nullptr;
std::function<int(DWORD, int)> r_lua_pushboolean = nullptr;
std::function<void* (DWORD, int)> r_lua_getmetatable = nullptr;
std::function<void* (DWORD, int)> r_lua_settable = nullptr;
std::function<double(DWORD, double)> r_lua_pushnumber = nullptr;
std::function<void(DWORD, int, int)> r_lua_createtable = nullptr;
std::function<const char* (DWORD, int, int)> r_lua_getupvalues = nullptr;
std::function<uintptr_t(DWORD, int)> r_lua_topointer = nullptr;
std::function<void(DWORD)> r_lua_spawn = nullptr;
std::function<void(DWORD, const char*, int)> r_lua_pushlstring = nullptr;
std::function<DWORD(DWORD, int)> r_lua_type = nullptr;
std::function<void(DWORD, DWORD, DWORD)> r_lua_xmove = nullptr;
std::function<void(DWORD, DWORD, DWORD)> r_lual_checktype = nullptr;
std::function<lua_Number(int, int)> r_lual_checknumber = nullptr;
std::function<const char* (int, int, unsigned int*)> r_lual_checklstring = nullptr;
std::function<bool(DWORD, DWORD)> r_lua_iscfunction = nullptr;
std::function<void(DWORD, DWORD, DWORD)> r_lua_call = nullptr;
std::function<void(DWORD, int)> r_lua_setmetatable = nullptr;
std::function<void* (DWORD, size_t, int)> r_lua_newuserdata = nullptr;
std::function<DWORD(DWORD, DWORD, const std::string&)> r_findfirstchildofclass = nullptr;
std::function<void(DWORD, int)> r_getdatamodel = nullptr;
std::function<DWORD()> r_getdatamodel2 = nullptr;
std::function<bool(string&, string&, int, bool)> r_flog_setvalue = nullptr;
std::function<bool(const string&, string&, bool)> r_flog_getvalue = nullptr;

void UpdateAddresses(DWORD rL, json jsx) {
	std::string ccv1;
	bool ret1;
	std::string ccv2;
	bool ret2;
	std::string ccv3;
	bool ret3;
	ccv1 = jsx["addresses"]["lua_newthread"]["ccv"];
	ret1 = jsx["addresses"]["lua_newthread"]["retcheck"];
	if (ccv1 == "__cdecl") {
		if (ret1) {
			r_lua_newthread = (DWORD(__cdecl*)(DWORD))(unprotect(ASLR(jsx["addresses"]["lua_newthread"]["addr"])));
		}
		else {
			r_lua_newthread = (DWORD(__cdecl*)(DWORD))(ASLR(jsx["addresses"]["lua_newthread"]["addr"]));
		}
	}
	else if (ccv1 == "__fastcall") {
		if (ret1) {
			r_lua_newthread = (DWORD(__fastcall*)(DWORD))(unprotect(ASLR(jsx["addresses"]["lua_newthread"]["addr"])));
		}
		else {
			r_lua_newthread = (DWORD(__fastcall*)(DWORD))(ASLR(jsx["addresses"]["lua_newthread"]["addr"]));
		}
	}
	else if (ccv1 == "__stdcall") {
		if (ret1) {
			r_lua_newthread = (DWORD(__stdcall*)(DWORD))(unprotect(ASLR(jsx["addresses"]["lua_newthread"]["addr"])));
		}
		else {
			r_lua_newthread = (DWORD(__stdcall*)(DWORD))(ASLR(jsx["addresses"]["lua_newthread"]["addr"]));
		}
	}
	else if (ccv1 == "__thiscall") {
		if (ret1) {
			r_lua_newthread = (DWORD(__thiscall*)(DWORD))(unprotect(ASLR(jsx["addresses"]["lua_newthread"]["addr"])));
		}
		else {
			r_lua_newthread = (DWORD(__thiscall*)(DWORD))(ASLR(jsx["addresses"]["lua_newthread"]["addr"]));
		}
	}

	ccv2 = jsx["addresses"]["lua_pushstring"]["ccv"];
	ret2 = jsx["addresses"]["lua_pushstring"]["retcheck"];
	if (ccv2 == "__cdecl") {
		if (ret2) {
			r_lua_pushstring = (void(__cdecl*)(DWORD, const char*))(unprotect(ASLR(jsx["addresses"]["lua_pushstring"]["addr"])));
		}
		else {
			r_lua_pushstring = (void(__cdecl*)(DWORD, const char*))(ASLR(jsx["addresses"]["lua_pushstring"]["addr"]));
		}
	}
	else if (ccv2 == "__fastcall") {
		if (ret2) {
			r_lua_pushstring = (void(__fastcall*)(DWORD, const char*))(unprotect(ASLR(jsx["addresses"]["lua_pushstring"]["addr"])));
		}
		else {
			r_lua_pushstring = (void(__fastcall*)(DWORD, const char*))(ASLR(jsx["addresses"]["lua_pushstring"]["addr"]));
		}
	}
	else if (ccv2 == "__stdcall") {
		if (ret2) {
			r_lua_pushstring = (void(__stdcall*)(DWORD, const char*))(unprotect(ASLR(jsx["addresses"]["lua_pushstring"]["addr"])));
		}
		else {
			r_lua_pushstring = (void(__stdcall*)(DWORD, const char*))(ASLR(jsx["addresses"]["lua_pushstring"]["addr"]));
		}
	}
	else if (ccv2 == "__thiscall") {
		if (ret2) {
			r_lua_pushstring = (void(__thiscall*)(DWORD, const char*))(unprotect(ASLR(jsx["addresses"]["lua_pushstring"]["addr"])));
		}
		else {
			r_lua_pushstring = (void(__thiscall*)(DWORD, const char*))(ASLR(jsx["addresses"]["lua_pushstring"]["addr"]));
		}
	}

	ccv3 = jsx["addresses"]["lua_type"]["ccv"];
	ret3 = jsx["addresses"]["lua_type"]["retcheck"];
	if (ccv3 == "__cdecl") {
		if (ret3) {
			r_lua_type = (DWORD(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_type"]["addr"])));
		}
		else {
			r_lua_type = (DWORD(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_type"]["addr"]));
		}
	}
	else if (ccv3 == "__fastcall") {
		if (ret3) {
			r_lua_type = (DWORD(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_type"]["addr"])));
		}
		else {
			r_lua_type = (DWORD(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_type"]["addr"]));
		}
	}
	else if (ccv3 == "__stdcall") {
		if (ret3) {
			r_lua_type = (DWORD(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_type"]["addr"])));
		}
		else {
			r_lua_type = (DWORD(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_type"]["addr"]));
		}
	}
	else if (ccv3 == "__thiscall") {
		if (ret3) {
			r_lua_type = (DWORD(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_type"]["addr"])));
		}
		else {
			r_lua_type = (DWORD(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_type"]["addr"]));
		}
	}

	std::string ccv;
	bool ret;
	
	ccv = jsx["addresses"]["lua_tonumber"]["ccv"];
	ret = jsx["addresses"]["lua_tonumber"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_tonumber = (double(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_tonumber"]["addr"])));
		}
		else {
			r_lua_tonumber = (double(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_tonumber"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_tonumber = (double(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_tonumber"]["addr"])));
		}
		else {
			r_lua_tonumber = (double(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_tonumber"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_tonumber = (double(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_tonumber"]["addr"])));
		}
		else {
			r_lua_tonumber = (double(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_tonumber"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_tonumber = (double(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_tonumber"]["addr"])));
		}
		else {
			r_lua_tonumber = (double(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_tonumber"]["addr"]));
		}
	}

    ccv = jsx["addresses"]["lua_tolstring"]["ccv"];
	ret = jsx["addresses"]["lua_tolstring"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_tolstring = (const char*(__cdecl*)(int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_tolstring"]["addr"])));
		}
		else {
			r_lua_tolstring = (const char* (__cdecl*)(int, int, int))(ASLR(jsx["addresses"]["lua_tolstring"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_tolstring = (const char* (__fastcall*)(int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_tolstring"]["addr"])));
		}
		else {
			r_lua_tolstring = (const char* (__fastcall*)(int, int, int))(ASLR(jsx["addresses"]["lua_tolstring"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_tolstring = (const char* (__stdcall*)(int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_tolstring"]["addr"])));
		}
		else {
			r_lua_tolstring = (const char* (__stdcall*)(int, int, int))(ASLR(jsx["addresses"]["lua_tolstring"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_tolstring = (const char* (__thiscall*)(int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_tolstring"]["addr"])));
		}
		else {
			r_lua_tolstring = (const char* (__thiscall*)(int, int, int))(ASLR(jsx["addresses"]["lua_tolstring"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_pushvalue"]["ccv"];
	ret = jsx["addresses"]["lua_pushvalue"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_pushvalue = (void(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_pushvalue"]["addr"])));
		}
		else {
			r_lua_pushvalue = (void(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_pushvalue"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_pushvalue = (void(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_pushvalue"]["addr"])));
		}
		else {
			r_lua_pushvalue = (void(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_pushvalue"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_pushvalue = (void(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_pushvalue"]["addr"])));
		}
		else {
			r_lua_pushvalue = (void(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_pushvalue"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_pushvalue = (void(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_pushvalue"]["addr"])));
		}
		else {
			r_lua_pushvalue = (void(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_pushvalue"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_pushlstring"]["ccv"];
	ret = jsx["addresses"]["lua_pushlstring"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_pushlstring = (void(__cdecl*)(DWORD, const char*, int))(unprotect(ASLR(jsx["addresses"]["lua_pushlstring"]["addr"])));
		}
		else {
			r_lua_pushlstring = (void(__cdecl*)(DWORD, const char*, int))(ASLR(jsx["addresses"]["lua_pushlstring"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_pushlstring = (void(__fastcall*)(DWORD, const char*, int))(unprotect(ASLR(jsx["addresses"]["lua_pushlstring"]["addr"])));
		}
		else {
			r_lua_pushlstring = (void(__fastcall*)(DWORD, const char*, int))(ASLR(jsx["addresses"]["lua_pushlstring"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_pushlstring = (void(__stdcall*)(DWORD, const char*, int))(unprotect(ASLR(jsx["addresses"]["lua_pushlstring"]["addr"])));
		}
		else {
			r_lua_pushlstring = (void(__stdcall*)(DWORD, const char*, int))(ASLR(jsx["addresses"]["lua_pushlstring"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_pushlstring = (void(__thiscall*)(DWORD, const char*, int))(unprotect(ASLR(jsx["addresses"]["lua_pushlstring"]["addr"])));
		}
		else {
			r_lua_pushlstring = (void(__thiscall*)(DWORD, const char*, int))(ASLR(jsx["addresses"]["lua_pushlstring"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_pcall"]["ccv"];
	ret = jsx["addresses"]["lua_pcall"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_pcall = (DWORD(__cdecl*)(DWORD, int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_pcall"]["addr"])));
		}
		else {
			r_lua_pcall = (DWORD(__cdecl*)(DWORD, int, int, int))(ASLR(jsx["addresses"]["lua_pcall"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_pcall = (DWORD(__fastcall*)(DWORD, int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_pcall"]["addr"])));
		}
		else {
			r_lua_pcall = (DWORD(__fastcall*)(DWORD, int, int, int))(ASLR(jsx["addresses"]["lua_pcall"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_pcall = (DWORD(__stdcall*)(DWORD, int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_pcall"]["addr"])));
		}
		else {
			r_lua_pcall = (DWORD(__stdcall*)(DWORD, int, int, int))(ASLR(jsx["addresses"]["lua_pcall"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_pcall = (DWORD(__thiscall*)(DWORD, int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_pcall"]["addr"])));
		}
		else {
			r_lua_pcall = (DWORD(__thiscall*)(DWORD, int, int, int))(ASLR(jsx["addresses"]["lua_pcall"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_settop"]["ccv"];
	ret = jsx["addresses"]["lua_settop"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_settop = (void(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_settop"]["addr"])));
		}
		else {
			r_lua_settop = (void(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_settop"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_settop = (void(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_settop"]["addr"])));
		}
		else {
			r_lua_settop = (void(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_settop"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_settop = (void(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_settop"]["addr"])));
		}
		else {
			r_lua_settop = (void(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_settop"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_settop = (void(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_settop"]["addr"])));
		}
		else {
			r_lua_settop = (void(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_settop"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_newthread"]["ccv"];
	ret = jsx["addresses"]["lua_newthread"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_newthread = (DWORD(__cdecl*)(int))(unprotect(ASLR(jsx["addresses"]["lua_newthread"]["addr"])));
		}
		else {
			r_lua_newthread = (DWORD(__cdecl*)(int))(ASLR(jsx["addresses"]["lua_newthread"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_newthread = (DWORD(__fastcall*)(int))(unprotect(ASLR(jsx["addresses"]["lua_newthread"]["addr"])));
		}
		else {
			r_lua_newthread = (DWORD(__fastcall*)(int))(ASLR(jsx["addresses"]["lua_newthread"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_newthread = (DWORD(__stdcall*)(int))(unprotect(ASLR(jsx["addresses"]["lua_newthread"]["addr"])));
		}
		else {
			r_lua_newthread = (DWORD(__stdcall*)(int))(ASLR(jsx["addresses"]["lua_newthread"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_newthread = (DWORD(__thiscall*)(int))(unprotect(ASLR(jsx["addresses"]["lua_newthread"]["addr"])));
		}
		else {
			r_lua_newthread = (DWORD(__thiscall*)(int))(ASLR(jsx["addresses"]["lua_newthread"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_setfield"]["ccv"];
	ret = jsx["addresses"]["lua_setfield"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_setfield = (void(__cdecl*)(DWORD, int, const char*))(unprotect(ASLR(jsx["addresses"]["lua_setfield"]["addr"])));
		}
		else {
			r_lua_setfield = (void(__cdecl*)(DWORD, int, const char*))(ASLR(jsx["addresses"]["lua_setfield"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_setfield = (void(__fastcall*)(DWORD, int, const char*))(unprotect(ASLR(jsx["addresses"]["lua_setfield"]["addr"])));
		}
		else {
			r_lua_setfield = (void(__fastcall*)(DWORD, int, const char*))(ASLR(jsx["addresses"]["lua_setfield"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_setfield = (void(__stdcall*)(DWORD, int, const char*))(unprotect(ASLR(jsx["addresses"]["lua_setfield"]["addr"])));
		}
		else {
			r_lua_setfield = (void(__stdcall*)(DWORD, int, const char*))(ASLR(jsx["addresses"]["lua_setfield"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_setfield = (void(__thiscall*)(DWORD, int, const char*))(unprotect(ASLR(jsx["addresses"]["lua_setfield"]["addr"])));
		}
		else {
			r_lua_setfield = (void(__thiscall*)(DWORD, int, const char*))(ASLR(jsx["addresses"]["lua_setfield"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_getfield"]["ccv"];
	ret = jsx["addresses"]["lua_getfield"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_getfield = (DWORD(__cdecl*)(DWORD, int, const char*))(unprotect(ASLR(jsx["addresses"]["lua_getfield"]["addr"])));
		}
		else {
			r_lua_getfield = (DWORD(__cdecl*)(DWORD, int, const char*))(ASLR(jsx["addresses"]["lua_getfield"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_getfield = (DWORD(__fastcall*)(DWORD, int, const char*))(unprotect(ASLR(jsx["addresses"]["lua_getfield"]["addr"])));
		}
		else {
			r_lua_getfield = (DWORD(__fastcall*)(DWORD, int, const char*))(ASLR(jsx["addresses"]["lua_getfield"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_getfield = (DWORD(__stdcall*)(DWORD, int, const char*))(unprotect(ASLR(jsx["addresses"]["lua_getfield"]["addr"])));
		}
		else {
			r_lua_getfield = (DWORD(__stdcall*)(DWORD, int, const char*))(ASLR(jsx["addresses"]["lua_getfield"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_getfield = (DWORD(__thiscall*)(DWORD, int, const char*))(unprotect(ASLR(jsx["addresses"]["lua_getfield"]["addr"])));
		}
		else {
			r_lua_getfield = (DWORD(__thiscall*)(DWORD, int, const char*))(ASLR(jsx["addresses"]["lua_getfield"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["spawn"]["ccv"];
	ret = jsx["addresses"]["spawn"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {	
			r_lua_spawn = (void(__cdecl*)(DWORD))(unprotect(ASLR(jsx["addresses"]["spawn"]["addr"])));
		}
		else {
			r_lua_spawn = (void(__cdecl*)(DWORD))(ASLR(jsx["addresses"]["spawn"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_spawn = (void(__fastcall*)(DWORD))(unprotect(ASLR(jsx["addresses"]["spawn"]["addr"])));
		}
		else {
			r_lua_spawn = (void(__fastcall*)(DWORD))(ASLR(jsx["addresses"]["spawn"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_spawn = (void(__stdcall*)(DWORD))(unprotect(ASLR(jsx["addresses"]["spawn"]["addr"])));
		}
		else {
			r_lua_spawn = (void(__stdcall*)(DWORD))(ASLR(jsx["addresses"]["spawn"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_spawn = (void(__thiscall*)(DWORD))(unprotect(ASLR(jsx["addresses"]["spawn"]["addr"])));
		}
		else {
			r_lua_spawn = (void(__thiscall*)(DWORD))(ASLR(jsx["addresses"]["spawn"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["luau_deserialize"]["ccv"];
	ret = jsx["addresses"]["luau_deserialize"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_luau_deserialize = (DWORD(__cdecl*)(DWORD, const char*, const char*, size_t))(unprotect(ASLR(jsx["addresses"]["luau_deserialize"]["addr"])));
		}
		else {
			r_luau_deserialize = (DWORD(__cdecl*)(DWORD, const char*, const char*, size_t))(ASLR(jsx["addresses"]["luau_deserialize"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_luau_deserialize = (DWORD(__fastcall*)(DWORD, const char*, const char*, size_t))(unprotect(ASLR(jsx["addresses"]["luau_deserialize"]["addr"])));
		}
		else {
			r_luau_deserialize = (DWORD(__fastcall*)(DWORD, const char*, const char*, size_t))(ASLR(jsx["addresses"]["luau_deserialize"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_luau_deserialize = (DWORD(__stdcall*)(DWORD, const char*, const char*, size_t))(unprotect(ASLR(jsx["addresses"]["luau_deserialize"]["addr"])));
		}
		else {
			r_luau_deserialize = (DWORD(__stdcall*)(DWORD, const char*, const char*, size_t))(ASLR(jsx["addresses"]["luau_deserialize"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_luau_deserialize = (DWORD(__thiscall*)(DWORD, const char*, const char*, size_t))(unprotect(ASLR(jsx["addresses"]["luau_deserialize"]["addr"])));
		}
		else {
			r_luau_deserialize = (DWORD(__thiscall*)(DWORD, const char*, const char*, size_t))(ASLR(jsx["addresses"]["luau_deserialize"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_getmetatable"]["ccv"];
	ret = jsx["addresses"]["lua_getmetatable"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_getmetatable = (void*(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_getmetatable"]["addr"])));
		}
		else {
			r_lua_getmetatable = (void*(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_getmetatable"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_getmetatable = (void* (__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_getmetatable"]["addr"])));
		}
		else {
			r_lua_getmetatable = (void* (__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_getmetatable"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_getmetatable = (void* (__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_getmetatable"]["addr"])));
		}
		else {
			r_lua_getmetatable = (void* (__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_getmetatable"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_getmetatable = (void* (__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_getmetatable"]["addr"])));
		}
		else {
			r_lua_getmetatable = (void* (__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_getmetatable"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_pushnumber"]["ccv"];
	ret = jsx["addresses"]["lua_pushnumber"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_pushnumber = (double(__cdecl*)(DWORD, double))(unprotect(ASLR(jsx["addresses"]["lua_pushnumber"]["addr"])));
		}
		else {
			r_lua_pushnumber = (double(__cdecl*)(DWORD, double))(ASLR(jsx["addresses"]["lua_pushnumber"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_pushnumber = (double(__fastcall*)(DWORD, double))(unprotect(ASLR(jsx["addresses"]["lua_pushnumber"]["addr"])));
		}
		else {
			r_lua_pushnumber = (double(__fastcall*)(DWORD, double))(ASLR(jsx["addresses"]["lua_pushnumber"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_pushnumber = (double(__stdcall*)(DWORD, double))(unprotect(ASLR(jsx["addresses"]["lua_pushnumber"]["addr"])));
		}
		else {
			r_lua_pushnumber = (double(__stdcall*)(DWORD, double))(ASLR(jsx["addresses"]["lua_pushnumber"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_pushnumber = (double(__thiscall*)(DWORD, double))(unprotect(ASLR(jsx["addresses"]["lua_pushnumber"]["addr"])));
		}
		else {
			r_lua_pushnumber = (double(__thiscall*)(DWORD, double))(ASLR(jsx["addresses"]["lua_pushnumber"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_pushboolean"]["ccv"];
	ret = jsx["addresses"]["lua_pushboolean"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_pushboolean = (int(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_pushboolean"]["addr"])));
		}
		else {
			r_lua_pushboolean = (int(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_pushboolean"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_pushboolean = (int(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_pushboolean"]["addr"])));
		}
		else {
			r_lua_pushboolean = (int(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_pushboolean"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_pushboolean = (int(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_pushboolean"]["addr"])));
		}
		else {
			r_lua_pushboolean = (int(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_pushboolean"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_pushboolean = (int(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_pushboolean"]["addr"])));
		}
		else {
			r_lua_pushboolean = (int(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_pushboolean"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_xmove"]["ccv"];
	ret = jsx["addresses"]["lua_xmove"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_xmove = (void(__cdecl*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lua_xmove"]["addr"])));
		}
		else {
			r_lua_xmove = (void(__cdecl*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lua_xmove"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_xmove = (void(__fastcall*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lua_xmove"]["addr"])));
		}
		else {
			r_lua_xmove = (void(__fastcall*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lua_xmove"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_xmove = (void(__stdcall*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lua_xmove"]["addr"])));
		}
		else {
			r_lua_xmove = (void(__stdcall*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lua_xmove"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_xmove = (void(__thiscall*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lua_xmove"]["addr"])));
		}
		else {
			r_lua_xmove = (void(__thiscall*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lua_xmove"]["addr"]));
		}
	}
	
	ccv = jsx["addresses"]["lual_checktype"]["ccv"];
	ret = jsx["addresses"]["lual_checktype"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lual_checktype = (void(__cdecl*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lual_checktype"]["addr"])));
		}
		else {
			r_lual_checktype = (void(__cdecl*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lual_checktype"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lual_checktype = (void(__fastcall*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lual_checktype"]["addr"])));
		}
		else {
			r_lual_checktype = (void(__fastcall*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lual_checktype"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lual_checktype = (void(__stdcall*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lual_checktype"]["addr"])));
		}
		else {
			r_lual_checktype = (void(__stdcall*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lual_checktype"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lual_checktype = (void(__thiscall*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lual_checktype"]["addr"])));
		}
		else {
			r_lual_checktype = (void(__thiscall*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lual_checktype"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lual_checknumber"]["ccv"];
	ret = jsx["addresses"]["lual_checknumber"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lual_checknumber = (lua_Number(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lual_checknumber"]["addr"])));
		}
		else {
			r_lual_checknumber = (lua_Number(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lual_checknumber"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lual_checknumber = (lua_Number(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lual_checknumber"]["addr"])));
		}
		else {
			r_lual_checknumber = (lua_Number(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lual_checknumber"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lual_checknumber = (lua_Number(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lual_checknumber"]["addr"])));
		}
		else {
			r_lual_checknumber = (lua_Number(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lual_checknumber"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lual_checknumber = (lua_Number(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lual_checknumber"]["addr"])));
		}
		else {
			r_lual_checknumber = (lua_Number(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lual_checknumber"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lual_checklstring"]["ccv"];
	ret = jsx["addresses"]["lual_checklstring"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lual_checklstring = (const char*(__cdecl*)(DWORD, int, unsigned int*))(unprotect(ASLR(jsx["addresses"]["lual_checklstring"]["addr"])));
		}
		else {
			r_lual_checklstring = (const char* (__cdecl*)(DWORD, int, unsigned int*))(ASLR(jsx["addresses"]["lual_checklstring"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lual_checklstring = (const char* (__fastcall*)(DWORD, int, unsigned int*))(unprotect(ASLR(jsx["addresses"]["lual_checklstring"]["addr"])));
		}
		else {
			r_lual_checklstring = (const char* (__fastcall*)(DWORD, int, unsigned int*))(ASLR(jsx["addresses"]["lual_checklstring"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lual_checklstring = (const char* (__stdcall*)(DWORD, int, unsigned int*))(unprotect(ASLR(jsx["addresses"]["lual_checklstring"]["addr"])));
		}
		else {
			r_lual_checklstring = (const char* (__stdcall*)(DWORD, int, unsigned int*))(ASLR(jsx["addresses"]["lual_checklstring"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lual_checklstring = (const char* (__thiscall*)(DWORD, int, unsigned int*))(unprotect(ASLR(jsx["addresses"]["lual_checklstring"]["addr"])));
		}
		else {
			r_lual_checklstring = (const char* (__thiscall*)(DWORD, int, unsigned int*))(ASLR(jsx["addresses"]["lual_checklstring"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_getupvalue"]["ccv"];
	ret = jsx["addresses"]["lua_getupvalue"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_getupvalues = (const char* (__cdecl*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["lua_getupvalue"]["addr"])));
		}
		else {
			r_lua_getupvalues = (const char* (__cdecl*)(DWORD, int, int))(ASLR(jsx["addresses"]["lua_getupvalue"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_getupvalues = (const char* (__fastcall*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["lua_getupvalue"]["addr"])));
		}
		else {
			r_lua_getupvalues = (const char* (__fastcall*)(DWORD, int, int))(ASLR(jsx["addresses"]["lua_getupvalue"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_getupvalues = (const char* (__stdcall*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["lua_getupvalue"]["addr"])));
		}
		else {
			r_lua_getupvalues = (const char* (__stdcall*)(DWORD, int, int))(ASLR(jsx["addresses"]["lua_getupvalue"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_getupvalues = (const char* (__thiscall*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["lua_getupvalue"]["addr"])));
		}
		else {
			r_lua_getupvalues = (const char* (__thiscall*)(DWORD, int, int))(ASLR(jsx["addresses"]["lua_getupvalue"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["fireclickdetector"]["ccv"];
	ret = jsx["addresses"]["fireclickdetector"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_fireclickdetector = (DWORD(__cdecl*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["fireclickdetector"]["addr"])));
		}
		else {
			r_fireclickdetector = (DWORD(__cdecl*)(DWORD, int, int))(ASLR(jsx["addresses"]["fireclickdetector"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_fireclickdetector = (DWORD(__fastcall*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["fireclickdetector"]["addr"])));
		}
		else {
			r_fireclickdetector = (DWORD(__fastcall*)(DWORD, int, int))(ASLR(jsx["addresses"]["fireclickdetector"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_fireclickdetector = (DWORD(__stdcall*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["fireclickdetector"]["addr"])));
		}
		else {
			r_fireclickdetector = (DWORD(__stdcall*)(DWORD, int, int))(ASLR(jsx["addresses"]["fireclickdetector"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_fireclickdetector = (DWORD(__thiscall*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["fireclickdetector"]["addr"])));
		}
		else {
			r_fireclickdetector = (DWORD(__thiscall*)(DWORD, int, int))(ASLR(jsx["addresses"]["fireclickdetector"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_touserdata"]["ccv"];
	ret = jsx["addresses"]["lua_touserdata"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_touserdata = (void*(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_touserdata"]["addr"])));
		}
		else {
			r_lua_touserdata = (void* (__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_touserdata"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_touserdata = (void* (__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_touserdata"]["addr"])));
		}
		else {
			r_lua_touserdata = (void* (__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_touserdata"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_touserdata = (void* (__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_touserdata"]["addr"])));
		}
		else {
			r_lua_touserdata = (void* (__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_touserdata"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_touserdata = (void* (__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_touserdata"]["addr"])));
		}
		else {
			r_lua_touserdata = (void* (__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_touserdata"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_touserdata"]["ccv"];
	ret = jsx["addresses"]["lua_touserdata"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_touserdata = (void* (__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_touserdata"]["addr"])));
		}
		else {
			r_lua_touserdata = (void* (__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_touserdata"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_touserdata = (void* (__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_touserdata"]["addr"])));
		}
		else {
			r_lua_touserdata = (void* (__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_touserdata"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_touserdata = (void* (__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_touserdata"]["addr"])));
		}
		else {
			r_lua_touserdata = (void* (__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_touserdata"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_touserdata = (void* (__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_touserdata"]["addr"])));
		}
		else {
			r_lua_touserdata = (void* (__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_touserdata"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_settable"]["ccv"];
	ret = jsx["addresses"]["lua_settable"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_settable = (void* (__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_settable"]["addr"])));
		}
		else {
			r_lua_settable = (void* (__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_settable"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_settable = (void* (__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_settable"]["addr"])));
		}
		else {
			r_lua_settable = (void* (__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_settable"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_settable = (void* (__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_settable"]["addr"])));
		}
		else {
			r_lua_settable = (void* (__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_settable"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_settable = (void* (__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_settable"]["addr"])));
		}
		else {
			r_lua_settable = (void* (__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_settable"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_createtable"]["ccv"];
	ret = jsx["addresses"]["lua_createtable"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_createtable = (void(__cdecl*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["lua_createtable"]["addr"])));
		}
		else {
			r_lua_createtable = (void(__cdecl*)(DWORD, int, int))(ASLR(jsx["addresses"]["lua_createtable"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_createtable = (void(__fastcall*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["lua_createtable"]["addr"])));
		}
		else {
			r_lua_createtable = (void(__fastcall*)(DWORD, int, int))(ASLR(jsx["addresses"]["lua_createtable"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_createtable = (void(__stdcall*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["lua_createtable"]["addr"])));
		}
		else {
			r_lua_createtable = (void(__stdcall*)(DWORD, int, int))(ASLR(jsx["addresses"]["lua_createtable"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_createtable = (void(__thiscall*)(DWORD, int, int))(unprotect(ASLR(jsx["addresses"]["lua_createtable"]["addr"])));
		}
		else {
			r_lua_createtable = (void(__thiscall*)(DWORD, int, int))(ASLR(jsx["addresses"]["lua_createtable"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_iscfunction"]["ccv"];
	ret = jsx["addresses"]["lua_iscfunction"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_iscfunction = (bool(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_iscfunction"]["addr"])));
		}
		else {
			r_lua_iscfunction = (bool(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_iscfunction"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_iscfunction = (bool(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_iscfunction"]["addr"])));
		}
		else {
			r_lua_iscfunction = (bool(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_iscfunction"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_iscfunction = (bool(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_iscfunction"]["addr"])));
		}
		else {
			r_lua_iscfunction = (bool(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_iscfunction"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_iscfunction = (bool(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_iscfunction"]["addr"])));
		}
		else {
			r_lua_iscfunction = (bool(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_iscfunction"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_topointer"]["ccv"];
	ret = jsx["addresses"]["lua_topointer"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_topointer = (uintptr_t(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_topointer"]["addr"])));
		}
		else {
			r_lua_topointer = (uintptr_t(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_topointer"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_topointer = (uintptr_t(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_topointer"]["addr"])));
		}
		else {
			r_lua_topointer = (uintptr_t(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_topointer"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_topointer = (uintptr_t(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_topointer"]["addr"])));
		}
		else {
			r_lua_topointer = (uintptr_t(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_topointer"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_topointer = (uintptr_t(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_topointer"]["addr"])));
		}
		else {
			r_lua_topointer = (uintptr_t(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_topointer"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_toboolean"]["ccv"];
	ret = jsx["addresses"]["lua_toboolean"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_toboolean = (bool(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_toboolean"]["addr"])));
		}
		else {
			r_lua_toboolean = (bool(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_toboolean"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_toboolean = (bool(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_toboolean"]["addr"])));
		}
		else {
			r_lua_toboolean = (bool(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_toboolean"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_toboolean = (bool(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_toboolean"]["addr"])));
		}
		else {
			r_lua_toboolean = (bool(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_toboolean"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_toboolean = (bool(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_toboolean"]["addr"])));
		}
		else {
			r_lua_toboolean = (bool(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_toboolean"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_call"]["ccv"];
	ret = jsx["addresses"]["lua_call"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_call = (void(__cdecl*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lua_call"]["addr"])));
		}
		else {
			r_lua_call = (void(__cdecl*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lua_call"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_call = (void(__fastcall*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lua_call"]["addr"])));
		}
		else {
			r_lua_call = (void(__fastcall*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lua_call"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_call = (void(__stdcall*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lua_call"]["addr"])));
		}
		else {
			r_lua_call = (void(__stdcall*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lua_call"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_call = (void(__thiscall*)(DWORD, DWORD, DWORD))(unprotect(ASLR(jsx["addresses"]["lua_call"]["addr"])));
		}
		else {
			r_lua_call = (void(__thiscall*)(DWORD, DWORD, DWORD))(ASLR(jsx["addresses"]["lua_call"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_setmetatable"]["ccv"];
	ret = jsx["addresses"]["lua_setmetatable"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_setmetatable = (void(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_setmetatable"]["addr"])));
		}
		else {
			r_lua_setmetatable = (void(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_setmetatable"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_setmetatable = (void(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_setmetatable"]["addr"])));
		}
		else {
			r_lua_setmetatable = (void(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_setmetatable"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_setmetatable = (void(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_setmetatable"]["addr"])));
		}
		else {
			r_lua_setmetatable = (void(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_setmetatable"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_setmetatable = (void(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_setmetatable"]["addr"])));
		}
		else {
			r_lua_setmetatable = (void(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_setmetatable"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_newuserdata"]["ccv"];
	ret = jsx["addresses"]["lua_newuserdata"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_newuserdata = (void*(__cdecl*)(DWORD, size_t, int))(unprotect(ASLR(jsx["addresses"]["lua_newuserdata"]["addr"])));
		}
		else {
			r_lua_newuserdata = (void* (__cdecl*)(DWORD, size_t, int))(ASLR(jsx["addresses"]["lua_newuserdata"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_newuserdata = (void* (__fastcall*)(DWORD, size_t, int))(unprotect(ASLR(jsx["addresses"]["lua_newuserdata"]["addr"])));
		}
		else {
			r_lua_newuserdata = (void* (__fastcall*)(DWORD, size_t, int))(ASLR(jsx["addresses"]["lua_newuserdata"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_newuserdata = (void* (__stdcall*)(DWORD, size_t, int))(unprotect(ASLR(jsx["addresses"]["lua_newuserdata"]["addr"])));
		}
		else {
			r_lua_newuserdata = (void* (__stdcall*)(DWORD, size_t, int))(ASLR(jsx["addresses"]["lua_newuserdata"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_newuserdata = (void* (__thiscall*)(DWORD, size_t, int))(unprotect(ASLR(jsx["addresses"]["lua_newuserdata"]["addr"])));
		}
		else {
			r_lua_newuserdata = (void* (__thiscall*)(DWORD, size_t, int))(ASLR(jsx["addresses"]["lua_newuserdata"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_insert"]["ccv"];
	ret = jsx["addresses"]["lua_insert"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_insert = (void(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_insert"]["addr"])));
		}
		else {
			r_lua_insert = (void(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["lua_insert"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_insert = (void(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_insert"]["addr"])));
		}
		else {
			r_lua_insert = (void(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_insert"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_insert = (void(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_insert"]["addr"])));
		}
		else {
			r_lua_insert = (void(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_insert"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_insert = (void(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["lua_insert"]["addr"])));
		}
		else {
			r_lua_insert = (void(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["lua_insert"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["findfirstchildofclass"]["ccv"];
	ret = jsx["addresses"]["findfirstchildofclass"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_findfirstchildofclass = (DWORD(__cdecl*)(DWORD, int, const std::string&))(unprotect(ASLR(jsx["addresses"]["findfirstchildofclass"]["addr"])));
		}
		else {
			r_findfirstchildofclass = (DWORD(__cdecl*)(DWORD, int, const std::string&))(ASLR(jsx["addresses"]["findfirstchildofclass"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_findfirstchildofclass = (DWORD(__fastcall*)(DWORD, int, const std::string&))(unprotect(ASLR(jsx["addresses"]["findfirstchildofclass"]["addr"])));
		}
		else {
			r_findfirstchildofclass = (DWORD(__fastcall*)(DWORD, int, const std::string&))(ASLR(jsx["addresses"]["findfirstchildofclass"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_findfirstchildofclass = (DWORD(__stdcall*)(DWORD, int, const std::string&))(unprotect(ASLR(jsx["addresses"]["findfirstchildofclass"]["addr"])));
		}
		else {
			r_findfirstchildofclass = (DWORD(__stdcall*)(DWORD, int, const std::string&))(ASLR(jsx["addresses"]["findfirstchildofclass"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_findfirstchildofclass = (DWORD(__thiscall*)(DWORD, int, const std::string&))(unprotect(ASLR(jsx["addresses"]["findfirstchildofclass"]["addr"])));
		}
		else {
			r_findfirstchildofclass = (DWORD(__thiscall*)(DWORD, int, const std::string&))(ASLR(jsx["addresses"]["findfirstchildofclass"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["lua_pushcclosure"]["ccv"];
	ret = jsx["addresses"]["lua_pushcclosure"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_lua_pushcclosure = (DWORD(__cdecl*)(DWORD, int, int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_pushcclosure"]["addr"])));
		}
		else {
			r_lua_pushcclosure = (DWORD(__cdecl*)(DWORD, int, int, int, int))(ASLR(jsx["addresses"]["lua_pushcclosure"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_lua_pushcclosure = (DWORD(__fastcall*)(DWORD, int, int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_pushcclosure"]["addr"])));
		}
		else {
			r_lua_pushcclosure = (DWORD(__fastcall*)(DWORD, int, int, int, int))(ASLR(jsx["addresses"]["lua_pushcclosure"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_lua_pushcclosure = (DWORD(__stdcall*)(DWORD, int, int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_pushcclosure"]["addr"])));
		}
		else {
			r_lua_pushcclosure = (DWORD(__stdcall*)(DWORD, int, int, int, int))(ASLR(jsx["addresses"]["lua_pushcclosure"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_lua_pushcclosure = (DWORD(__thiscall*)(DWORD, int, int, int, int))(unprotect(ASLR(jsx["addresses"]["lua_pushcclosure"]["addr"])));
		}
		else {
			r_lua_pushcclosure = (DWORD(__thiscall*)(DWORD, int, int, int, int))(ASLR(jsx["addresses"]["lua_pushcclosure"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["getdatamodel"]["ccv"];
	ret = jsx["addresses"]["getdatamodel"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_getdatamodel = (void(__cdecl*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["getdatamodel"]["addr"])));
		}
		else {
			r_getdatamodel = (void(__cdecl*)(DWORD, int))(ASLR(jsx["addresses"]["getdatamodel"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_getdatamodel = (void(__fastcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["getdatamodel"]["addr"])));
		}
		else {
			r_getdatamodel = (void(__fastcall*)(DWORD, int))(ASLR(jsx["addresses"]["getdatamodel"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_getdatamodel = (void(__stdcall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["getdatamodel"]["addr"])));
		}
		else {
			r_getdatamodel = (void(__stdcall*)(DWORD, int))(ASLR(jsx["addresses"]["getdatamodel"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_getdatamodel = (void(__thiscall*)(DWORD, int))(unprotect(ASLR(jsx["addresses"]["getdatamodel"]["addr"])));
		}
		else {
			r_getdatamodel = (void(__thiscall*)(DWORD, int))(ASLR(jsx["addresses"]["getdatamodel"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["getdatamodel2"]["ccv"];
	ret = jsx["addresses"]["getdatamodel2"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_getdatamodel2 = (DWORD(__cdecl*)())(unprotect(ASLR(jsx["addresses"]["getdatamodel2"]["addr"])));
		}
		else {
			r_getdatamodel2 = (DWORD(__cdecl*)())(ASLR(jsx["addresses"]["getdatamodel2"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_getdatamodel2 = (DWORD(__fastcall*)())(unprotect(ASLR(jsx["addresses"]["getdatamodel2"]["addr"])));
		}
		else {
			r_getdatamodel2 = (DWORD(__fastcall*)())(ASLR(jsx["addresses"]["getdatamodel2"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_getdatamodel2 = (DWORD(__stdcall*)())(unprotect(ASLR(jsx["addresses"]["getdatamodel2"]["addr"])));
		}
		else {
			r_getdatamodel2 = (DWORD(__stdcall*)())(ASLR(jsx["addresses"]["getdatamodel2"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_getdatamodel2 = (DWORD(__thiscall*)())(unprotect(ASLR(jsx["addresses"]["getdatamodel2"]["addr"])));
		}
		else {
			r_getdatamodel2 = (DWORD(__thiscall*)())(ASLR(jsx["addresses"]["getdatamodel2"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["flog_getvalue"]["ccv"];
	ret = jsx["addresses"]["flog_getvalue"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_flog_getvalue = (bool(__cdecl*)(const string&, string&, bool))(unprotect(ASLR(jsx["addresses"]["flog_getvalue"]["addr"])));
		}
		else {
			r_flog_getvalue = (bool(__cdecl*)(const string&, string&, bool))(ASLR(jsx["addresses"]["flog_getvalue"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_flog_getvalue = (bool(__fastcall*)(const string&, string&, bool))(unprotect(ASLR(jsx["addresses"]["flog_getvalue"]["addr"])));
		}
		else {
			r_flog_getvalue = (bool(__fastcall*)(const string&, string&, bool))(ASLR(jsx["addresses"]["flog_getvalue"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_flog_getvalue = (bool(__stdcall*)(const string&, string&, bool))(unprotect(ASLR(jsx["addresses"]["flog_getvalue"]["addr"])));
		}
		else {
			r_flog_getvalue = (bool(__stdcall*)(const string&, string&, bool))(ASLR(jsx["addresses"]["flog_getvalue"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_flog_getvalue = (bool(__thiscall*)(const string&, string&, bool))(unprotect(ASLR(jsx["addresses"]["flog_getvalue"]["addr"])));
		}
		else {
			r_flog_getvalue = (bool(__thiscall*)(const string&, string&, bool))(ASLR(jsx["addresses"]["flog_getvalue"]["addr"]));
		}
	}

	ccv = jsx["addresses"]["flog_setvalue"]["ccv"];
	ret = jsx["addresses"]["flog_setvalue"]["retcheck"];
	if (ccv == "__cdecl") {
		if (ret) {
			r_flog_setvalue = (bool(__cdecl*)(const string&, const string&, int, bool))(unprotect(ASLR(jsx["addresses"]["flog_setvalue"]["addr"])));
		}
		else {
			r_flog_setvalue = (bool(__cdecl*)(const string&, const string&, int, bool))(ASLR(jsx["addresses"]["flog_setvalue"]["addr"]));
		}
	}
	else if (ccv == "__fastcall") {
		if (ret) {
			r_flog_setvalue = (bool(__fastcall*)(const string&, const string&, int, bool))(unprotect(ASLR(jsx["addresses"]["flog_setvalue"]["addr"])));
		}
		else {
			r_flog_setvalue = (bool(__fastcall*)(const string&, const string&, int, bool))(ASLR(jsx["addresses"]["flog_setvalue"]["addr"]));
		}
	}
	else if (ccv == "__stdcall") {
		if (ret) {
			r_flog_setvalue = (bool(__stdcall*)(const string&, const string&, int, bool))(unprotect(ASLR(jsx["addresses"]["flog_setvalue"]["addr"])));
		}
		else {
			r_flog_setvalue = (bool(__stdcall*)(const string&, const string&, int, bool))(ASLR(jsx["addresses"]["flog_setvalue"]["addr"]));
		}
	}
	else if (ccv == "__thiscall") {
		if (ret) {
			r_flog_setvalue = (bool(__thiscall*)(const string&, const string&, int, bool))(unprotect(ASLR(jsx["addresses"]["flog_setvalue"]["addr"])));
		}
		else {
			r_flog_setvalue = (bool(__thiscall*)(const string&, const string&, int, bool))(ASLR(jsx["addresses"]["flog_setvalue"]["addr"]));
		}
	}

	TOP = jsx["offsets"]["ls_top"];
	BASE = jsx["offsets"]["ls_base"];
	IDENTITY = jsx["offsets"]["Identity2"];
	SYMBOL = jsx["offsets"]["ls_symbol"];
	OFFSET = jsx["offsets"]["ls_offset"];
	READONLY = jsx["offsets"]["ls_readonly"];
}


void rLua_pushcclosure(DWORD rL, int Func, int upvals)
{
	DWORD Func2 = Add(Func);
	r_lua_pushcclosure(rL, Func2, NULL, upvals, NULL);
}

void r_lua_setreadonly(DWORD State, int Index, bool Boolean) {
	*(BYTE*)(r_lua_topointer(State, Index) + READONLY) = Boolean;
}


void r_lua_pushcfunction(DWORD rL, int Func) {
	rLua_pushcclosure(rL, Func, 0);
}

int r_lua_gettop(int rL)
{
	return *(DWORD*)(rL + TOP) - *(DWORD*)(rL + BASE) >> 4;
}


void r_luaL_register(DWORD rL, const char* Name, int fn(DWORD)) {
	r_lua_pushcfunction(RL, (DWORD)fn);
	r_lua_setglobal(RL, Name);
}

uintptr_t ReturnDataModel()
{
	DWORD DMPad[16]{};
	r_getdatamodel(r_getdatamodel2(), (DWORD)DMPad);
	return DMPad[0] + 0xC;
}

DWORD RBX_LuaState(DWORD ScriptContext) {
	if (SYMBOL == "^") {
		return (ScriptContext + OFFSET) ^ *(DWORD*)(ScriptContext + OFFSET);
	}
	else if (SYMBOL == "+") {
		return (ScriptContext + OFFSET) + *(DWORD*)(ScriptContext + OFFSET);
	}
	else if (SYMBOL == "-") {
		return (ScriptContext + OFFSET) - *(DWORD*)(ScriptContext + OFFSET);
	}
	else if (SYMBOL == "*") {
		return (ScriptContext + OFFSET) * *(DWORD*)(ScriptContext + OFFSET);
	}
	else if (SYMBOL == "/") {
		return (ScriptContext + OFFSET) / *(DWORD*)(ScriptContext + OFFSET);
	}
}

DWORD ReturnState(DWORD v2) {
	return *(DWORD*)(v2 + 0xA4) - (DWORD)(v2 + 0xA4);
}

const char* r_lua_getclass(int self)
{
	return (const char*)(*(int(**)(void))(*(int*)self + 16))();
}

int FindChildOfInstance(int Instance, const char* Name)
{
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		if (memcmp(r_lua_getclass(*(int*)i), Name, strlen(Name)) == 0)
		{
			return *(int*)i;
		}
	}
}

DWORD FindChildOfInstance1(DWORD Instance, const char* Child) {
	static DWORD SC;
	r_findfirstchildofclass(Instance, (DWORD)&SC, Child);
	return SC;
}

int rluaL_newmetatable(int ls, const char* name)
{
	r_lua_getfield(ls, LUA_REGISTRYINDEX, name);  /* get registry.name */
	if (r_lua_type(ls, -1) != RLUA_TNIL)  /* name already in use? */
		return 0;  /* leave previous value on top, but return 0 */
	r_lua_pop(ls, 1);
	r_lua_createtable(ls, 0, 0);  /* create metatable */
	r_lua_pushvalue(ls, -1);
	r_lua_setfield(ls, LUA_REGISTRYINDEX, name);  /* registry.name = metatable */
	return 1;
}

auto addrError = unprotect(ASLR(lual_error_addr));
#define ccError lual_error_CCV


auto addrNewUserData = unprotect(ASLR(lua_newuserdata_addr));
#define ccNewUserData lua_newuserdata_CCV

void* rlua_newuserdata(int ls, unsigned int sz)
{
	return ((void* (ccNewUserData*)(int, unsigned int))(addrNewUserData))(ls, sz);
}


int rluaL_error(int ls, const char* reason)
{
	return ((int(ccError*)(int, const char*))(lua_error_addr))(ls, reason);
}

int rluaL_getmetatable(int ls, const char* tname)
{
	return r_lua_getfield(ls, LUA_REGISTRYINDEX, tname);
}

void rluaL_setmetatable(int ls, const char* tname)
{
	rluaL_getmetatable(ls, tname);
	r_lua_setmetatable(ls, -2);
}

const char* rlua_tolstring(int ls, int idx, int len)
{
	return r_lua_tolstring(ls, idx, len);
}

const char* rlua_tostring(int ls, int idx)
{
	return rlua_tolstring(ls, idx, 0);
}

uintptr_t  RLuaNilObject;

#define luaO_nilobject		(&luaO_nilobject_)
typedef TValue* StkId;  /* index to stack elements */

static TValue* index2adr(lua_State* L, int idx) {
	if (idx > 0) {
		TValue* o = L->base + (idx - 1);
		api_check(L, idx <= L->ci->top - L->base);
		if (o >= L->top) return cast(TValue*, luaO_nilobject);
		else return o;
	}
	else if (idx > LUA_REGISTRYINDEX) {
		api_check(L, idx != 0 && -idx <= L->top - L->base);
		return L->top + idx;
	}
	else switch (idx) {  /* pseudo-indices */
	case LUA_REGISTRYINDEX: return registry(L);
	case LUA_ENVIRONINDEX: {
		Closure* func = curr_func(L);
		sethvalue(L, &L->env, func->c.env);
		return &L->env;
	}
	case LUA_GLOBALSINDEX: return gt(L);
	default: {
		Closure* func = curr_func(L);
		idx = LUA_GLOBALSINDEX - idx;
		return (idx <= func->c.nupvalues)
			? &func->c.upvalue[idx - 1]
			: cast(TValue*, luaO_nilobject);
	}
	}
}

const char* GetStr(TString* o) { return (const char*)((uintptr_t)(o)+0x18); };
#define r_setnvalue(obj, x) { TValue *i_o = (obj); i_o->value.n = (x); i_o->tt = RLUA_TNUMBER; }

uintptr_t DXorKey;

double XorDouble(double num)
{
	uint64_t U64_Xor = *reinterpret_cast<uint64_t*>(&num) ^ *reinterpret_cast<uint64_t*>(DXorKey);
	return *reinterpret_cast<double*>(&U64_Xor);
}

const TValue* VToNumber(const TValue* obj, TValue* n)
{
	lua_Number num;
	if (ttype(obj) == RLUA_TNUMBER) return obj;
	if (ttype(obj) == RLUA_TSTRING && luaO_str2d(GetStr(rawtsvalue(obj)), &num)) {
		r_setnvalue(n, XorDouble(num));
		return n;
	}
	else
		return NULL;
}

DWORD RBX_SetIdentity(DWORD LS, int identity) { *(DWORD*)(*(DWORD*)(LS + IDENTITY) + 24) = identity; return 0; }

bool IsNumber(int idx)
{
	TValue n;
	const TValue* o = index2adr(L, idx);
	return ttype(o) == LUA_TNUMBER || (((o) = VToNumber(o, &n)) != NULL);
}

DWORD instance_ptr;

#define SynException(x) (x)
#define ERR_INVALID_INST		1

#define DereferenceSmartPointerInstance(Ptr) (*(DWORD*) (Ptr))
#define BoundsCheckInstance() if (!instance_ptr) throw SynException(ERR_INVALID_INST)


std::string GetInstanceClassName(DWORD Inst)
{
	if (Inst == 0)
	{
		BoundsCheckInstance();
#ifdef _DEBUG
		return std::string(*(const char**)(*(DWORD*)(instance_ptr + 0xC) + 4));
#else
		return **(std::string**)(*(DWORD*)(instance_ptr + 0xC) + 4);
#endif
	}
#ifdef _DEBUG
	return std::string(*(const char**)(*(DWORD*)(Inst + 0xC) + 4));
#else
	return **(std::string**)(*(DWORD*)(Inst + 0xC) + 4);
#endif
}

std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::string logo = R"(   ______     __      __                       __      __           
  /      \   |  \    |  \                     |  \    |  \          
 |  $$$$$$\ _| $$_   | $$  ______   _______  _| $$_    \$$  _______ 
 | $$__| $$|   $$ \  | $$ |      \ |       \|   $$ \  |  \ /       \
 | $$    $$ \$$$$$$  | $$  \$$$$$$\| $$$$$$$\\$$$$$$  | $$|  $$$$$$$
 | $$$$$$$$  | $$ __ | $$ /      $$| $$  | $$ | $$ __ | $$ \$$    \ 
 | $$  | $$  | $$|  \| $$|  $$$$$$$| $$  | $$ | $$|  \| $$ _\$$$$$$\
 | $$  | $$   \$$  $$| $$ \$$    $$| $$  | $$  \$$  $$| $$|       $$
  \$$   \$$    \$$$$  \$$  \$$$$$$$ \$$   \$$   \$$$$  \$$ \$$$$$$$ 
                                                                    
                                          Developed by Nexus42
                                                                    )";

const char* ExploitName = "Atlantis"; //Change this to your exploit name or the name you want.

bool ExecuteScript(DWORD RS, lua_State* L, std::string Script) {
	DWORD xD = r_lua_newthread(RS); // back
	Script = replaceAll(Script, "Game", "game");
	Script = replaceAll(Script, "game:GetObjects", "GetObjects");
	Script = replaceAll(Script, "game:HttpGet", "httpget");
	Script = "GetObjects=function(assetid)local object=game:GetService('InsertService'):LoadLocalAsset(assetid)return{object}end\n" + Script;
	Script = "spawn(function() script = Instance.new('LocalScript') script.Disabled = true; script.Parent = nil\r\n" + Script + "\r\nend)";
	if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@Atlantis"))
	{
		const std::string ERR = lua_tostring(L, -1);
		lua_close(L);
		throw std::exception(ERR.c_str());
	}
	std::string bytes;
	bytes = LuauTranspiler::compile(lua_open(), Script);
	r_luau_deserialize(xD, ExploitName, bytes.c_str(), bytes.size());
	r_lua_spawn(xD);

	return true;
}

const char c_ref1[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
const int c_ref2[16] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15 };

BYTE toHex(char* x) {
	if (lstrlenA(x) < 2) return 0;
	if (x[0] == '?' && x[1] == '?') return 0;
	BYTE b = 0;
	for (int i = 0; i < 16; i++) {
		if (x[0] == c_ref1[i]) b += c_ref2[i] * 16;
		if (x[1] == c_ref1[i]) b += i;
	}
	return b;
}

bool ExecuteBytecode(DWORD RS, lua_State* L, char* bytecode) {
	print("Expect me ded");
	print("Do you rlly WANT me to execute this bytecodes :C")
	print("THERE I FORMATTED IT WHAT ELSE COULD U WANT... oh.. no.................");
	Sleep(500);

	DWORD xD = r_lua_newthread(RS); // back
	print("AHHHHHHHH *torture sounds* AHHHH-");
	lua_getglobal(L, "string");
	lua_getfield(L, -1, "dump");
	//r_lua_setfield(xD, LUA_GLOBALSINDEX, "SCRIPT_VALUE");
	print("AHHHHHHHHHHHHHHHHHHHHHHHHH MY FUCKING FACE ARGGGGGGGGGGGG FUCKING HELP REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE-");
	r_luau_deserialize(xD, ExploitName, (const char*)bytecode, sizeof(bytecode));
	lua_pcall(L, 0, 0, 0);
	print("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! FUCKING HELP IM DYING OVER HERE!");
	r_lua_spawn(xD);
	print(".. is it over?");
	return true;
}