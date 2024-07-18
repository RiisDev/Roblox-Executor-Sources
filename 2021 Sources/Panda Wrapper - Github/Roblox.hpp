#pragma once

extern "C" {
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lopcodes.h"
#include "Lua/lobject.h"
#include "Lua/ldo.h"
#include "Lua/lstate.h"
}

#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 3
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 1
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 7
#define RLUA_TFUNCTION 5
#define RLUA_TTABLE 6
#define RLUA_TUSERDATA 8

typedef int (*rbx_CFunction)(int ls);

class Roblox
{
public:
	enum PrintType {
		Normal = 0,
		Info,
		Warn,
		Error
	};

	class Functions {
	public:
		static char Crash();
		static int GetDatamodel();

		static int rlua_pop(int ls, int a); //nooo the definitions are already done for these, same with the declarations
		static int rlua_type(int ls, int idx);// i just got a notification in google classroom. like its saturday tf
		static int rlua_settop(int ls, int a); //surprise school call from ur teechors my nigga ass biology teacher posted an assignment
		static int rlua_gettop(int ls); //lol niice
		static int rlua_getglobal(int ls, const char* key);//gotta go see abt that
		static int rlua_setglobal(int ls, const char* key); //kk im done with addresses
		static int rlua_getfield(int ls, int idx, const char* key);
		static int rlua_iscfunction(int ls, int idx);
		static void rlua_gettable(int ls, int idx);
		static int rlua_setfield(int ls, int idx, const char* key);
		static int rlua_settable(int ls, int idx);
		static int rlua_setreadonly(int ls, int a, int b);
		static int rlua_setmetatable(int ls, int a);
		static int rlua_getmetatable(int ls, int a);
		static int rluaL_getmetatable(int ls, const char* tname);
		static void rluaL_setmetatable(int ls, const char* tname);
		static int rlua_pushlstring(int ls, const char* val, int len);
		static int rlua_pushcfunction(int ls, rbx_CFunction fn);
		static int rlua_pushcclosure(int ls, rbx_CFunction fn, int n);
		static int rlua_pushnumber(int ls, double num);
		static int rlua_createtable(int ls, int a, int b);
		static int rlua_pushvalue(int ls, int a);
		static int rlua_pushnil(int ls);
		static int rlua_pushboolean(int ls, bool a);
		static void rlua_register(int ls, rbx_CFunction fn, const char* name);
		static const char* rlua_tolstring(int ls, int idx, int len);
		static const char* rlua_tostring(int ls, int idx);
		static bool rlua_toboolean(int ls, int idx);
		static void* rlua_touserdata(int ls, int idx);
		static int rlua_tothread(int ls, int idx);
		static lua_Integer rluaL_checkinteger(int ls, int numarg);
		static const char* rluaL_checklstring(int ls, int a, unsigned int* b);
		static lua_Number rluaL_checknumber(int ls, int numarg);
		static int rluaL_checkoption(int ls, int numarg, const char* def, const char* const* lst);
		static void rluaL_checktype(int ls, int numarg, int t);
		static void rlua_rawseti(int ls, int a, int b);
		static int rlua_call(int ls, int a, int b);
		static int rlua_pcall(int ls, int a, int b, int c);
		static int rlua_next(int ls, int a);
		static void rlua_remove(int ls, int a);
		static int rluaL_error(int ls, const char* reason);
		static int rlua_pushstring(int ls, const char* val);
		static int rlua_newthread(int ls);
		static void* rlua_newuserdata(int ls, unsigned int sz);
		static int rluaL_newmetatable(int ls, const char* name);
		static TValue* rlua_index2adr(int a, int b);

		static void SetContext(int ls, int context);
		static int GetContext(int ls);

		static int deserializer(int ls, const char* name, const char* source, int unk);
		static void spawn(int ls);
	};

	class Instance {
	public:
		static const char* GetClass(int self);
		static int FindFirstClass(int Instance, const char* name);
	};

	static void Initialize();

	static int GetDatamodel();
	static int GetScriptContext();
	static int GetLuaState();
};

