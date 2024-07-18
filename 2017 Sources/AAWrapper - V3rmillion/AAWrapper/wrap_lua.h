#pragma once
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "luaconf.h"
}

int rlua_newthread(DWORD rL);
void rlua_pushstring(DWORD rL, const char* s);
double rlua_pushnumber(DWORD rL, double idx);
int rlua_pushfthread(DWORD rL);
extern DWORD rlua_pushobject(DWORD rL, int pobject, int tableobj);
void rlua_getfield(DWORD rL, int idx, const char* key);
void rlua_setfield(DWORD rL, int idx, const char* key);
void rlua_pushvalue(DWORD rL, int idx);
int rlua_call(DWORD rL, int nAarg, int errfunc);
int rlua_pushcclosure(DWORD rL, int fn(), int callnothing);
int rlua_pcall(DWORD rL, int nAarg, int blank, int errfunc);
int rlua_resume(DWORD rL, int nAarg);
int rlua_getmetatable(DWORD rL, int idx);
int rlua_type(DWORD rL, int idx);
int rlua_settop(DWORD rL, int idx);
char* rlua_tolstring(DWORD rL, int idx, int ok);
double rlua_tonumber(DWORD rL, int idx);
bool rlua_pushboolean(DWORD rL, bool idxlast);
int rlua_gettop(DWORD rL);
void rlua_getglobal(DWORD rL, const char* s);
int rlua_getmetafield(int lua_State, int idx, const char *k);
int rlua_pushnil(DWORD rL);
int rlua_toboolean(DWORD rL, bool idx);
int rlua_next(DWORD rL, int idx);
int rlua_rawgeti(DWORD rL, int mainidx, int lastidx);

extern int RobloxLuaState;
extern int RobloxLuaState2;

BOOL ExecuteString(string Source);

#define R_LUA_REGISTRYINDEX 0xFFFFD8F0

#define RLUA_TNIL			0
#define RLUA_TLIGHTUSERDATA	1
#define RLUA_TNUMBER		2
#define RLUA_TBOOLEAN		3
#define RLUA_TSTRING		4
#define RLUA_TTHREAD		5
#define RLUA_TFUNCTION		6
#define RLUA_TTABLE			7
#define RLUA_TUSERDATA		8
#define RLUA_TPROTO			9
#define RLUA_TUPVAL			10
#define rlua_pop(RL,n) rlua_settop(RL, -(n)-1)
#define rlua_tostring(L,i)	rlua_tolstring(L, (i), NULL)