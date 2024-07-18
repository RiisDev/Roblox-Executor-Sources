#include "wrap_lua.h"
#include "Globals.h"
#include "Memory.h"


int RobloxLuaState;



/*
/*

retcheck - 0x59af70
lua_getfield(3 args) - 0x732da0
lua_getfield(4 args) - 0x732dc0
lua_setfield - 0x734af0
lua_tolstring - 0x735110
lua_settop - 0x734ed0
lua_toboolean - 0x735050
lua_pushvalue - 0x734210
lua_pushnumber - 0x7340c0
lua_pushstring - 0x734150
lua_pcall - 0x733b80
lua_pushnil - 0x734050
lua_next - 0x733a00
lua_pushboolean - 0x733c50
lua_tonumber - 0x7353e0
lua_pushcclosure - 0x733cd0
luaD_precall - 0x72c160
lua_checkstack - 0x7327c0
lua_createtable - 0x7329e0
lua_insert - 0x733390
lua_newthread - 0x733870
lua_newuserdata - 0x733950
lua_pushfstring - 0x733e00
lua_pushthread - 0x734190
lua_rawgeti - 0x734420
lua_rawseti - 0x734600
lua_rawset - 0x7344b0
lua_rawget - 0x734370
lua_remove - 0x734720
lua_setfenv - 0x734a00
lua_setmetatable - 0x734ca0
lua_getmetatable - 0x733090
lua_xmove - 0x735590
ScriptContextVFTable - 0x6bb00e
lua_close - 0x7d9fe0
lua_touserdata - 0x72ad20
ContextLevel - 0x5a8180
lua_call - 0x732730
ScriptContextVFTable2 - 0x6b9d26
lua_type - 0x735540
luaS_newlstr - 0x72ef50
luaV_gettable - 0x731f20
luaV_settable - 0x732190
lua_resume - 0x7348d0
rawrjz - 0x72c4c7
retflag1 - 0x166D990
retflag2 - 0x1670D5C
retfunc - 0x166D960
luaF_newCclosure - 0x72CCE0
XORCONSTANT  - 0x15829E0
index2adr - 0x72AD20

*/


int rlua_newthread(DWORD rL)
{
	return ((int(__cdecl*)(DWORD))(aslr(0x733870)))(rL);
}

int rlua_type(DWORD rL, int size)
{
	return ((int(__cdecl*)(DWORD, int))(aslr(0x735540)))(rL, size);
}

DWORD rlua_index2adr(DWORD rL, size_t index)
{
	return ((int(__cdecl*)(DWORD, int))(aslr(0x72AD20)))(rL, index);
}

int rlua_settop(DWORD rL, int idx)
{
	return ((int(__cdecl*)(DWORD, int))(aslr(0x734ed0)))(rL, idx);
}

int rlua_resume(DWORD rL, int nAarg)
{
	return ((int(__cdecl*)(DWORD, int))(aslr(0x7348d0)))(rL, nAarg);
}

int rlua_pcall(DWORD rL, int nAarg, int blank, int errfunc)
{
	return ((int(__cdecl*)(DWORD, int, int, int))(aslr(0x733b80)))(rL, nAarg, blank, errfunc);
}

int rlua_pushcclosure(DWORD rL, int fn(), int callnothing)
{
	return ((int(__cdecl*)(DWORD, int(), int))(aslr(0x733cd0)))(rL, fn, callnothing);
}

int rlua_getmetatable(DWORD rL, int idx)
{
	return ((int(__cdecl*)(DWORD, int))(aslr(0x733090)))(rL, idx);
}

int rlua_rawseti(DWORD rL, int sizeidx, int mainidx)
{
	return ((int(__cdecl*)(DWORD, int, int))(aslr(0x734600)))(rL, sizeidx, mainidx);
}

char* rlua_tolstring(DWORD rL, int idx, int ok)
{
	return ((char*(__cdecl*)(DWORD, int, int))(aslr(0x735110)))(rL, idx, ok);
}

int rlua_toboolean(DWORD rL, bool idx)
{
	return ((bool(__cdecl*)(DWORD, bool))(aslr(0x735050)))(rL, idx);
}

int rlua_pushnil(DWORD rL)
{
	return ((int(__cdecl*)(DWORD))(aslr(0x734050)))(rL);
}

int rlua_newuserdata(DWORD rL, int idx)
{
	return ((int(__cdecl*)(DWORD, int))(aslr(0x733950)))(rL, idx);
}

int rlua_setmetatable(DWORD rL, int idx)
{
	return ((int(__cdecl*)(DWORD rL, int))(aslr(0x734ca0)))(rL, idx);
}

void rlua_getfield(DWORD rL, int idx, const char* key)
{
	((void(__cdecl*)(DWORD, int, const char*))(aslr(0x732da0)))(rL, idx, key);
}

void rlua_setfield(DWORD rL, int idx, const char* key)
{
	((void(__cdecl*)(DWORD, int, const char*))(aslr(0x734af0)))(rL, idx, key);
}

int rlua_next(DWORD rL, int idx)
{
	return ((int(__cdecl*)(DWORD, int))(aslr(0x733a00)))(rL, idx);
}

int rlua_rawgeti(DWORD rL, int mainidx, int lastidx)
{
	return ((int(__cdecl*)(DWORD, int, int))(aslr(0x733A90)))(rL, mainidx, lastidx);
}

double rlua_tonumber(DWORD rL, int idx)
{
	return ((double(__cdecl*)(DWORD, int))(aslr(0x7353e0)))(rL, idx);
}

void rlua_pushvalue(DWORD rL, int idx)
{
	((void(__cdecl*)(DWORD, int))(aslr(0x734210)))(rL, idx);
}

double rlua_pushnumber(DWORD rL, double idx)
{
	return ((double(__cdecl*)(DWORD, double))(aslr(0x7340c0)))(rL, idx);
}

void rlua_pushstring(DWORD rL, const char* s)
{
	((void(__cdecl*)(DWORD, const char*))(aslr(0x734150)))(rL, s);
}

bool rlua_pushboolean(DWORD rL, bool idxlast)
{
	return ((bool(__cdecl*)(DWORD, bool))(aslr(0x733c50)))(rL, idxlast);
}

int rlua_gettop(DWORD rL)
{
	return ((int(__cdecl*)(DWORD))(aslr(0x7332C0)))(rL);
}

static void rlua_getglobal(DWORD rL, const char* s)
{
	return rlua_getfield(rL, -10002, s);
}

int rlua_getmetafield(int lua_State, int idx, const char *k) //!
{
	typedef int(__cdecl *Lua_getmetafield)(int lua_State, int idx, const char *k);
	return ((Lua_getmetafield)(Memory::scan2("\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\x75\x08\x56\xE8\x00\x00\x00\x00\x83\xC4\x08\x85\xC0\x74\x57", "xxxxxxxxxxxx????xxxxxxx")))(lua_State, idx, k);
}
