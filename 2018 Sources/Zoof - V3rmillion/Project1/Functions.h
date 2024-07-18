#pragma once
#include "Definitions.h"
#include "Locations.h"
#include "Retcheck.h"

typedef void(__cdecl* tlua_getfield)(DWORD L, int idx, const char* k);
typedef void(__cdecl* tlua_setfield)(DWORD rL, int idx, const char* k);
typedef void(__cdecl* tlua_pushvalue)(DWORD rL, int idx);
typedef void(__cdecl* tlua_pushcclosure)(DWORD rL, lua_CFunction fn, int n); //not implemented
typedef const char*(__cdecl* tlua_tolstring)(DWORD rL, int idx, size_t s);
typedef double(__cdecl* tlua_tonumber)(DWORD rL, int idx);
typedef bool(__cdecl* tlua_toboolean)(DWORD rL, int idx);
typedef int(__cdecl* tlua_call)(DWORD rL, int args, int results);
typedef void(__cdecl* tlua_settop)(DWORD rL, int top);
typedef void(__cdecl* tlua_pushstring)(DWORD rL, const char* k);
typedef void(__cdecl* tlua_pushlstring)(DWORD rL, const char* k, size_t s);
typedef void(__cdecl* tlua_pushnil)(DWORD rL);
typedef void(__cdecl* tlua_pushnumber)(DWORD rL, double n);
typedef void(__cdecl* tlua_pushboolean)(DWORD rL, bool b);
typedef int(__cdecl* tlua_getmetatable)(DWORD rL, int idx);
typedef void*(__cdecl* tlua_touserdata)(DWORD rL, int idx);
typedef int(__cdecl* tlua_type)(DWORD rL, int idx);
typedef int(__cdecl* tlua_next)(DWORD rL, int idx);
typedef void(__cdecl* tlua_rawseti)(DWORD rL, int idx, int n);
typedef int(__cdecl* tluaL_ref)(DWORD rL, int t);
typedef void(__cdecl* tlua_rawgeti)(DWORD rL, int idx, int n);
typedef DWORD(__cdecl* tlua_newthread)(DWORD rL); //not implemented
typedef int(__cdecl* tluaL_getmetafield)(DWORD rL, int obj, const char *e);
typedef void(__cdecl* tlua_createtable)(DWORD rL, int narr, int nrec);

tlua_getfield rlua_getfield = (tlua_getfield)unprotect(loc_getfield);
tlua_setfield arlua_setfield = (tlua_setfield)unprotect(loc_setfield);
tlua_pushvalue rlua_pushvalue = (tlua_pushvalue)unprotect(loc_pushvalue);
//tlua_pushcclosure rlua_pushcclosure = (tlua_pushcclosure)unprotect(loc_pushcclosure);
tlua_tolstring rlua_tolstring = (tlua_tolstring)unprotect(loc_tolstring);
tlua_tonumber rlua_tonumber = (tlua_tonumber)unprotect(loc_tonumber);
tlua_toboolean rlua_toboolean = (tlua_toboolean)unprotect(loc_toboolean);
tlua_call rlua_call = (tlua_call)unprotect(loc_call);
tlua_settop rlua_settop = (tlua_settop)unprotect(loc_settop);
tlua_pushstring rlua_pushstring = (tlua_pushstring)unprotect(loc_pushstring);
tlua_pushnil rlua_pushnil = (tlua_pushnil)unprotect(loc_pushnil);
tlua_pushnumber rlua_pushnumber = (tlua_pushnumber)unprotect(loc_pushnumber);
tlua_pushboolean rlua_pushboolean = (tlua_pushboolean)unprotect(loc_pushboolean);
tlua_getmetatable rlua_getmetatable = (tlua_getmetatable)unprotect(loc_getmetatable);
tlua_touserdata rlua_touserdata = (tlua_touserdata)unprotect(loc_touserdata);
tlua_type rlua_type = (tlua_type)unprotect(loc_type);
tlua_next rlua_next = (tlua_next)unprotect(loc_next);
tlua_rawseti rlua_rawseti = (tlua_rawseti)unprotect(loc_rawseti);
tluaL_ref rluaL_ref = (tluaL_ref)unprotect(loc_ref);
tlua_rawgeti rlua_rawgeti = (tlua_rawgeti)unprotect(loc_rawgeti);
//tlua_newthread rlua_newthread = (tlua_newthread)unprotect(loc_newthread);
tluaL_getmetafield rluaL_getmetafield = (tluaL_getmetafield)unprotect(loc_getmetafield);
tlua_createtable rlua_createtable = (tlua_createtable)unprotect(loc_createtable);

//#define rlua_pushcfunction(rL, fn)		 (rlua_pushcclosure(rL,fn,0))
#define rlua_clearstack(rL)				 (rlua_settop(rL, 0))
#define rlua_getglobal(rL, k)			 (rlua_getfield(rL, -10002, k))
#define rlua_setglobal(rL, k)			 (rlua_setfield(rL, -10002, k))
#define rlua_pop(L,n)					 (rlua_settop(L, -(n)-1))
#define rlua_isnil(L,n)					 (rlua_type(L, (n)) == 0)
#define rlua_isnone(L,n)			     (rlua_type(L, (n)) == (-1))
#define rlua_isnumber(L,n)				 (rlua_type(L, (n)) == 2)
#define rlua_isbool(L,n)			     (rlua_type(L, (n)) == 3)
#define rlua_isstring(L,n)				 (rlua_type(L, (n)) == 4)
#define rlua_islightuserdata(L,n)		 (rlua_type(L, (n)) == 1)
#define rlua_istable(L,n)				 (rlua_type(L, (n)) == 7)
#define rlua_isuserdata(L,n)		     (rlua_type(L, (n)) == 8)
#define rlua_isfunction(L,n)		     (rlua_type(L, (n)) == 6)
#define rlua_isproto(L,n)				 (rlua_type(L, (n)) == 9)
#define rlua_isthread(L,n)				 (rlua_type(L, (n)) == 5)
#define rlua_isupvalue(L,n)				 (rlua_type(L, (n)) == 10)
#define RLUA_TNONE						 -1
#define RLUA_TNIL						 0
#define RLUA_TLIGHTUSERDATA				 1
#define RLUA_TNUMBER					 2
#define RLUA_TBOOLEAN					 3
#define RLUA_TSTRING					 4
#define RLUA_TTHREAD					 5
#define RLUA_TFUNCTION					 6
#define RLUA_TTABLE						 7
#define RLUA_TUSERDATA					 8
#define RLUA_TPROTO						 9
#define RLUA_TUPVAL						 10


DWORD rlua_gettop(DWORD a1) {
	return (*(DWORD *)(a1 + 12) - *(DWORD *)(a1 + 20)) >> 4;
}

void rlua_setfield(DWORD L, int idx, const char *k) {
	rlua_pushvalue(L, idx);
	if (rlua_getmetatable(L, -1)) {
		rlua_getfield(L, -1, "__newindex");
		rlua_pushvalue(L, -3);
		rlua_pushstring(L, k);
		rlua_pushvalue(L, -6);
		rlua_call(L, 3, 0);
		rlua_pop(L, 3);
	}
	else {
		std::cout << "0\n";
		rlua_pop(L, 1);
		arlua_setfield(L, idx, k);
	}
}

std::string GetLocalPlayerName() {
	rlua_getglobal(rL, "game");
	rlua_getfield(rL, -1, "Players");
	rlua_getfield(rL, -1, "LocalPlayer");
	rlua_getfield(rL, -1, "Name");
	return rlua_tolstring(rL, -1, NULL);
}

void Execute(std::string s) {
	if (luaL_dostring(L, s.c_str())) {
		std::cout << "ERROR: " << lua_tostring(L, -1) << std::endl;
	}
}

void wrap(DWORD rL, lua_State *L, int idx) {
	switch (rlua_type(rL, idx)) {
	case RLUA_TSTRING:
		lua_pushstring(L, rlua_tolstring(rL, idx, NULL));
		break;
	case RLUA_TNUMBER:
		lua_pushnumber(L, rlua_tonumber(rL, idx));
		break;
	case RLUA_TBOOLEAN:
		lua_pushboolean(L, rlua_toboolean(rL, idx));
		break;
	case RLUA_TNIL:
		lua_pushnil(L);
		break;
	case RLUA_TNONE:
		lua_pushnil(L);
		break;
	case RLUA_TTABLE:
		lua_newtable(L);
		rlua_pushnil(rL);
		DWORD start = 0;
		DWORD end = 0;
		start = rlua_gettop(rL);
		while (rlua_next(rL, -2) != 0) {
			rlua_pushvalue(rL, -2);
			const char *name = rlua_tolstring(rL, -1, NULL);
			rlua_pushvalue(rL, -2);
			wrap(rL, L, -1);
			lua_setfield(L, -2, name);
			DWORD end = rlua_gettop(rL);
			rlua_pop(rL, end - start);
		}
		break;
	}
}

void unwrap(lua_State *L, DWORD rL, int idx) {
	switch (lua_type(L, idx)) {
	case LUA_TNIL:
		rlua_pushnil(rL);
		break;
	case LUA_TNONE:
		rlua_pushnil(rL);
		break;
	case LUA_TBOOLEAN:
		rlua_pushboolean(rL, lua_toboolean(L, idx));
		break;
	case LUA_TNUMBER:
		rlua_pushnumber(rL, lua_tonumber(L, idx));
		break;
	case LUA_TSTRING:
		rlua_pushstring(rL, lua_tostring(L, idx));
		break;
	}
}
