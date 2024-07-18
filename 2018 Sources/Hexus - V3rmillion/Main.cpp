#include "LuaWrapper.h"
#include "Retcheck.h"

#include <Windows.h>
#include <sstream>
#include <TlHelp32.h>

#define VERSION "1.0"
#define DbgPrint __noop
#define Error MessageBoxA

#define GetGlobalStateIndex(SC, idx) (int*)(SC[14 * idx + 41] - (DWORD)&SC[14 * idx + 41])
#define ASLR(x) (x + (int)GetModuleHandle(0))

#define R_LUA_REGISTRYINDEX 0xFFFFD8F0

#define NEWTHREAD 0x146280//
#define PUSHSTRING 0x146AE0//
#define PUSHNUMBER 0x146A60//
#define PCALL 0x146580//
#define SETMETATABLE 0x147580//
#define NEWUSERDATA 0x146360//
#define TOLSTRING 0x1479B0//
#define TOBOOLEAN 0x1478F0//
#define TOUSERDATA 0x147DB0//
#define SETFIELD 0x1473F0//
#define PUSHVALUE 0x146BA0//
#define PUSHCCLOSURE 0x1466B0//
#define TONUMBER 0x147C60//
#define GETMETAFIELD 0x141680//
#define TYPE 0x147DE0//
#define SETTOP 0x147780//
#define RAWGETI 0x146D80//
#define REF 0x141B10//
#define NEXT 0x146410
#define GETTABLE 0x145CB0//
#define REMOVE 0x147070//
#define SETTHREADIDENTITY 0x133D90

#define R_LUA_TNIL 0
#define R_LUA_TLIGHTUSERDATA 1
#define R_LUA_TNUMBER 2
#define R_LUA_TBOOLEAN 3
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 5
#define R_LUA_TFUNCTION 6
#define R_LUA_TTABLE 7
#define R_LUA_TUSERDATA 8
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10

Retcheck ret;

typedef void*(__cdecl* rgetfield)(int*, int, const char*);
rgetfield r_getfield;

typedef int*(__cdecl* rnewthread)(int*);
rnewthread r_newthread = (rnewthread)ret.unprotect((BYTE*)ASLR(NEWTHREAD));

typedef void(__cdecl* rpushstring)(int*, const char*);
rpushstring r_pushstring = (rpushstring)ret.unprotect((BYTE*)ASLR(PUSHSTRING));

typedef void(__cdecl* rpushnumber)(int*, double);
rpushnumber r_pushnumber = (rpushnumber)ret.unprotect((BYTE*)ASLR(PUSHNUMBER));

typedef int(__cdecl* rpcall)(int*, int, int, int*);
rpcall r_pcall = (rpcall)ret.unprotect((BYTE*)ASLR(PCALL));

typedef void(__cdecl* rsetmetatable)(int*, int);
rsetmetatable r_setmetatable = (rsetmetatable)ret.unprotect((BYTE*)ASLR(SETMETATABLE));

typedef void*(__cdecl* rnewuserdata)(int*, int);
rnewuserdata r_newuserdata = (rnewuserdata)ret.unprotect((BYTE*)ASLR(NEWUSERDATA));

typedef const char*(__cdecl* rtolstring)(int*, int, int);
rtolstring r_tolstring = (rtolstring)ret.unprotect((BYTE*)ASLR(TOLSTRING));

#define r_tostring(L, idx) r_tolstring(L, idx, 0)

typedef bool(__cdecl* rtoboolean)(int*, int);
rtoboolean r_toboolean = (rtoboolean)ret.unprotect((BYTE*)ASLR(TOBOOLEAN));

typedef int(__cdecl* rtouserdata)(int*, int);
rtouserdata r_touserdata = (rtouserdata)ret.unprotect((BYTE*)ASLR(TOUSERDATA));

typedef void(__cdecl* rsetfield)(int*, int, const char*);
rsetfield r_setfield = (rsetfield)ret.unprotect((BYTE*)ASLR(SETFIELD));

typedef void(__cdecl* rpushvalue)(int*, int);
rpushvalue r_pushvalue = (rpushvalue)ret.unprotect((BYTE*)ASLR(PUSHVALUE));

typedef void(__cdecl* rpushcclosure)(int*, int, int);
rpushcclosure r_pushcclosure = (rpushcclosure)ret.unprotect((BYTE*)ASLR(PUSHCCLOSURE));

#define r_pushcfunction(L, f) r_pushcclosure(L, f, 0);

typedef double(__cdecl* rtonumber)(int*, int);
rtonumber r_tonumber = (rtonumber)ret.unprotect((BYTE*)ASLR(TONUMBER));

typedef void(__cdecl* rgetmetafield)(int*, int, const char*);
rgetmetafield r_getmetafield = (rgetmetafield)ret.unprotect((BYTE*)ASLR(GETMETAFIELD));

typedef int(__cdecl* rtype)(int*, int);
rtype r_type = (rtype)ret.unprotect((BYTE*)ASLR(TYPE));

typedef void(__cdecl* rsettop)(int*, int);
rsettop r_settop = (rsettop)ret.unprotect((BYTE*)ASLR(SETTOP));

typedef int(__cdecl* rref)(int*, int);
rref r_ref = (rref)ret.unprotect((BYTE*)ASLR(REF));

typedef void(__cdecl* rrawgeti)(int*, int, int);
rrawgeti r_rawgeti = (rrawgeti)ret.unprotect((BYTE*)ASLR(RAWGETI));

typedef int(__cdecl* rnext)(int*, int);
rnext r_next = (rnext)ret.unprotect((BYTE*)ASLR(NEXT));

typedef int(__cdecl* rgettable)(int*, int);
rgettable r_gettable = (rgettable)ASLR(GETTABLE);

typedef void(__cdecl* rremove)(int*, int);
rremove r_remove = (rremove)ret.unprotect((BYTE*)ASLR(REMOVE));

#define r_pop(L, n) r_settop(L, -(n)-1)

void _WriteMem(DWORD Address, char Byte) {
DWORD Protection;
VirtualProtect((LPVOID)Address, 0x5, PAGE_READWRITE, &Protection);
*(char*)Address = Byte;
VirtualProtect((LPVOID)Address, 0x5, Protection, &Protection);
}

int r_gettop(int* _State) {
int State = (int)_State;
return *(DWORD*)(State + 16) - *(DWORD*)(State + 28) >> 4;
}

void r_pushboolean(int* _State, bool boolean) {
int State = (int)_State;
int v2 = *(DWORD*)(State + 16);
*(DWORD*)v2 = boolean;
*(DWORD*)(v2 + 8) = 3;
*(DWORD*)(State + 16) += 16;
}

void r_pushnil(int* _State) {
int State = (int)_State;
int v2 = *(DWORD*)(State + 16);
*(DWORD*)(v2 + 8) = 0;
*(DWORD*)(State + 16) += 16;
}

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
}

#pragma comment(lib, "Lua/lua5.1.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
return _iob;
}

int* GetThreadPointer(lua_State* L) {
lua_getfield(L, -10000, "__thread");
int T = lua_tonumber(L, -1);
lua_pop(L, 1);
return (int*)T;
}

static int LuaErrorNoPermission(lua_State* L) {
luaL_error(L, "You are not allowed to do that!");
return 0;
}

static int UserdataToString(lua_State* L) {
int* T = GetThreadPointer(L);
r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -1));
r_getfield(T, -10002, "tostring");
r_pushvalue(T, -2);
r_pcall(T, 1, 1, 0);
lua_pushstring(L, r_tostring(T, -1));
r_pop(T, 2);
return 1;
}

int* R_GlobalState;
int* R_DataModel;

void UnWrap(lua_State* L, int* T, int idx);
void Wrap(int* T, lua_State* L, int idx);

static int LuaFunction(int* T) {
int Key = r_tonumber(T, lua_upvalueindex(1));
lua_State* L = (lua_State*)(int)r_tonumber(T, lua_upvalueindex(2));
lua_rawgeti(L, -10000, Key);
for (int i = r_gettop(T); i != 0; i--) {
Wrap(T, L , 1);
}
lua_pcall(L, lua_gettop(L) - 1, 0, 0);
return 0;
}

void UnWrap(lua_State* L, int* T, int idx) {
switch (lua_type(L, idx)) {
case LUA_TNIL:
{
r_pushnil(T);
break;
}
case LUA_TSTRING:
{
r_pushstring(T, lua_tostring(L, idx));
break;
}
case LUA_TNUMBER:
{
r_pushnumber(T, lua_tonumber(L, idx));
break;
}
case LUA_TBOOLEAN:
{
r_pushboolean(T, lua_toboolean(L, idx));
break;
}
case LUA_TUSERDATA:
{
int* U = (int*)lua_touserdata(L, idx);
int Key = *U;
int IsRU = (*(int*)(U + 1) == 1);
if (IsRU) {
r_rawgeti(T, R_LUA_REGISTRYINDEX, Key);
}
else {
int* U = (int*)r_newuserdata(T, lua_objlen(L, idx));
*U = *(int*)lua_touserdata(L, idx);
}
break;
}
case R_LUA_TTABLE:
{
r_newtable(T);
lua_pushnil(L);
while (lua_next(L, -2)) {
lua_pushvalue(L, -2);

const char* key = lua_tostring(L, -1);

lua_pushvalue(L, -2);

UnWrap(L, T, -1);
r_setfield(T, -2, key);

lua_pop(L, 2);
}
lua_pop(L, 1);
break;
}
case LUA_TFUNCTION:
{
r_pushnumber(T, lua_ref(L, -10000));
r_pushnumber(T, (int)(int*)L);
r_pushcclosure(T, (int)LuaFunction, 2);
break;
}
default:
{
r_pushstring(T, "Not supported.");
break;
}
}
}

static int RobloxFunction(lua_State* L) {
int* T = GetThreadPointer(L);
int Key = lua_tonumber(L, lua_upvalueindex(1));
int ArgC = lua_gettop(L);
r_rawgeti(T, R_LUA_REGISTRYINDEX, Key);
for (int i = ArgC; i != 0; i--) {
UnWrap(L, T, i * -1);
}
r_pcall(T, ArgC, LUA_MULTRET, 0);
ArgC = r_gettop(T);
for (int i = ArgC; i != 0; i--) {
Wrap(T, L, i * -1);
}
return ArgC;
}

void Wrap(int* T, lua_State* L, int idx) {
switch (r_type(T, idx)) {
case R_LUA_TNIL:
{
lua_pushnil(L);
r_remove(T, idx);
break;
}
case R_LUA_TSTRING:
{
lua_pushstring(L, r_tostring(T, idx));
r_remove(T, idx);
break;
}
case R_LUA_TNUMBER:
{
lua_pushnumber(L, r_tonumber(T, idx));
r_remove(T, idx);
break;
}
case R_LUA_TBOOLEAN:
{
lua_pushboolean(L, r_toboolean(T, idx));
r_remove(T, idx);
break;
}
case R_LUA_TFUNCTION:
{
lua_pushnumber(L, r_ref(T, R_LUA_REGISTRYINDEX));
lua_pushcclosure(L, RobloxFunction, 1);
break;
}
case R_LUA_TUSERDATA:
{
r_getmetafield(T, -1, "__type");
const char* Type = r_tostring(T, -1);
r_pop(T, 1);
int Key = r_ref(T, R_LUA_REGISTRYINDEX);
int* U = (int*)lua_newuserdata(L, 4);
*U = Key;
*(U + 1) = 1;
lua_getfield(L, -10000, (std::string("R") + (std::string)Type).c_str());
lua_setmetatable(L, -2);
break;
}
case R_LUA_TTABLE:
{
lua_newtable(L);
r_pushnil(T);
while (r_next(T, -2)) {
r_pushvalue(T, -2);

const char* key = r_tostring(T, -1);

r_pushvalue(T, -2);

Wrap(T, L, -1);
lua_setfield(L, -2, key);

r_pop(T, 2);
}
r_pop(T, 1);
break;
}
}
}

void WrapRobloxGlobal(int* T, lua_State* L, const char* Name) {
r_getfield(T, -10002, Name);
Wrap(T, L, -1);
lua_setfield(L, -10002, Name);
}

static int UserdataIndex(lua_State* L) {
try {
int* T = GetThreadPointer(L);

const char* Key = lua_tostring(L, -1);
lua_pop(L, 1);

UnWrap(L, T, 1);
r_getfield(T, -1, Key);
Wrap(T, L, -1);
r_pop(T, 1);
}
catch (std::exception e) {
Error(0, e.what(), "Error", 0);
luaL_error(L, e.what());
}
return 1;
}

static int UserdataNewIndex(lua_State* L) {
try {
int* T = GetThreadPointer(L);
int Type = lua_type(L, -1);
const char* Key = lua_tostring(L, -2);
UnWrap(L, T, -3);
UnWrap(L, T, -1);
r_setfield(T, -2, Key);
r_pop(T, 1);
}
catch (std::exception e) {
luaL_error(L, e.what());
}
return 0;
}

static int Sleep(lua_State* L) {

return 0;
}

static int UserdataMul(lua_State* L) {
int* T = GetThreadPointer(L);

r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -2));
r_getmetafield(T, -1, "__mul");
r_pushvalue(T, -2);
r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -1));
r_pcall(T, 2, 1, 0);
Wrap(T, L, -1);
return 1;
}

static int UserdataAdd(lua_State* L) {
int* T = GetThreadPointer(L);

r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -2));
r_getmetafield(T, -1, "__add");
r_pushvalue(T, -2);
r_rawgeti(T, R_LUA_REGISTRYINDEX, *(int*)lua_touserdata(L, -1));
r_pcall(T, 2, 1, 0);
Wrap(T, L, -1);
return 1;
}

void AddDataType(lua_State* L, const char* Type) {
lua_newtable(L);
lua_pushcfunction(L, UserdataToString);
lua_setfield(L, -2, "__tostring");
lua_pushcfunction(L, UserdataIndex);
lua_setfield(L, -2, "__index");
lua_pushcfunction(L, UserdataNewIndex);
lua_setfield(L, -2, "__newindex");
lua_pushstring(L, "The metatable is locked!");
lua_setfield(L, -2, "__metatable");
lua_pushcfunction(L, UserdataMul);
lua_setfield(L, -2, "__mul");
lua_pushcfunction(L, UserdataAdd);
lua_setfield(L, -2, "__add");
lua_setfield(L, -10000, (std::string("R") + (std::string)Type).c_str());
}

static int CustomGetRawMetaTable(lua_State* L) {
int* T = GetThreadPointer(L);
if (r_getmetatable(T, -1))
Wrap(T, L, -1);
return 0;
}

void RawExecuteScript(int* T, const char* Source) {
r_pop(T, 1);

lua_State* L = luaL_newstate();
luaL_openlibs(L);         

AddDataType(L, "Object");
AddDataType(L, "CFrame");
AddDataType(L, "Vector3");
AddDataType(L, "Vector2");
AddDataType(L, "UDim");
AddDataType(L, "UDim2");
AddDataType(L, "BrickColor");
AddDataType(L, "Color3");
AddDataType(L, "RBXScriptSignal");
AddDataType(L, "RBXScriptConnection");

WrapRobloxGlobal(T, L, "game");
WrapRobloxGlobal(T, L, "Game");
WrapRobloxGlobal(T, L, "Enum");
WrapRobloxGlobal(T, L, "workspace");
WrapRobloxGlobal(T, L, "Workspace");

WrapRobloxGlobal(T, L, "_G");
WrapRobloxGlobal(T, L, "shared");
WrapRobloxGlobal(T, L, "print");
WrapRobloxGlobal(T, L, "printidentity");
WrapRobloxGlobal(T, L, "warn");
WrapRobloxGlobal(T, L, "delay");
WrapRobloxGlobal(T, L, "spawn");
WrapRobloxGlobal(T, L, "elapsedTime");
WrapRobloxGlobal(T, L, "LoadLibrary");
WrapRobloxGlobal(T, L, "PluginManager");
WrapRobloxGlobal(T, L, "require");
WrapRobloxGlobal(T, L, "settings");
WrapRobloxGlobal(T, L, "stats");
WrapRobloxGlobal(T, L, "tick");
WrapRobloxGlobal(T, L, "time");
WrapRobloxGlobal(T, L, "typeof");
WrapRobloxGlobal(T, L, "UserSettings");
WrapRobloxGlobal(T, L, "version");
WrapRobloxGlobal(T, L, "wait");
WrapRobloxGlobal(T, L, "math");


WrapRobloxGlobal(T, L, "Instance");
WrapRobloxGlobal(T, L, "UDim");
WrapRobloxGlobal(T, L, "UDim2");
WrapRobloxGlobal(T, L, "Vector3");
WrapRobloxGlobal(T, L, "Vector2");
WrapRobloxGlobal(T, L, "CFrame");
WrapRobloxGlobal(T, L, "Region3");
WrapRobloxGlobal(T, L, "Region3int16");
WrapRobloxGlobal(T, L, "PhysicalProperties");
WrapRobloxGlobal(T, L, "BrickColor");
WrapRobloxGlobal(T, L, "Vector3int16");
WrapRobloxGlobal(T, L, "Color3");
WrapRobloxGlobal(T, L, "Axes");
WrapRobloxGlobal(T, L, "ColorSequence");
WrapRobloxGlobal(T, L, "ColorSequenceKeypoint");
WrapRobloxGlobal(T, L, "Faces");
WrapRobloxGlobal(T, L, "Rect");
WrapRobloxGlobal(T, L, "TweenInfo");
WrapRobloxGlobal(T, L, "Ray");

lua_pushcclosure(L, CustomGetRawMetaTable, 0);
lua_setfield(L, -10002, "getrawmetatable");

lua_pushnumber(L, (int)T);
lua_setfield(L, -10000, "__thread");

lua_pushcfunction(L, Sleep);
lua_setfield(L, -10002, "wait");

lua_getfield(L, -10002, "os");
lua_pushcfunction(L, LuaErrorNoPermission);
lua_setfield(L, -2, "execute");
lua_pop(L, 1);

luaL_dostring(L, Source);
return 0;
}


namespace LuaWrapper {
void ExecuteScript(std::string Source) {
std::string Buff = "script=Instance.new'LocalScript'\n" + std::string(Source);
int* T = r_newthread(R_GlobalState);
int asd;
r_pop(R_GlobalState, 1);

RawExecuteScript(T, Buff.c_str());
}

void Init(int* SC, int _GETFIELD) {
r_getfield = (rgetfield)ret.unprotect((BYTE*)ASLR(_GETFIELD));
DbgPrint("ScriptContext: %X\n", (int)SC);
R_GlobalState = r_newthread(GetGlobalStateIndex(SC, 1));
R_DataModel = (int*)*(int*)((int)SC + 0x34);
DbgPrint("LuaState: %X\n", R_GlobalState);
}
}