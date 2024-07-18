#include "main.h"

void open_lua_libs(lua_State *Ls) {
	lua_newtable(Ls);
	lua_pushcfunctioN(Ls, print);
	lua_setglobal(Ls, "print");
	lua_settop(Ls, 0);
	lua_newtable(Ls);
	lua_pushcfunctioN(Ls, warn);
	lua_setglobal(Ls, "warn");
	lua_settop(Ls, 0);
	lua_newtable(Ls);
	lua_pushcfunctioN(Ls, error);
	lua_setglobal(Ls, "error");
	lua_settop(Ls, 0);
	lua_newtable(Ls);
	lua_pushcfunctioN(Ls, pi);
	lua_setglobal(Ls, "printidentity");
	lua_settop(Ls, 0);
	
	lua_newtable(Ls);
	lua_pushcfunctioN(Ls, newinstance);
	lua_setfield(Ls, -2, "new");
	lua_setglobal(Ls, "Instance");
	lua_settop(Ls, 0);
}

void WrapLua(std::string source) {
	lua_State* luaS = luaL_newstate();
	lua_State* LuaS = lua_newthread(luaS);
	*(char*)(L - 0x14) = 6;
	luaL_openlibs(LuaS);
	open_lua_libs(LuaS);
	
	luaL_dostring(LuaS, source.c_str());
	lua_settop(LuaS, 0);
	lua_close(LuaS);

	settop(L, 0);
}