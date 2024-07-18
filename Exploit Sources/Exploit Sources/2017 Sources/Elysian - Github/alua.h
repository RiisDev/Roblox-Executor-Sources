#ifndef ALUA_H
#define ALUA_H
#pragma once

#include <string>

int luaA_execute(std::string buffer);
int luaA_executeraw(std::string buffer, std::string source);
void luaA_executeTK();
int luaA_init(int scontext);
int luaA_setinit(int b);

#define addfunction(s, f) 	r_lua_pushcclosure(r_L, f, 0); \
							r_lua_setfield(r_L, -10002, s);

typedef void*(__cdecl *SetThreadIdentity_Def)(int r_lua_State, int context, int inst, int ext);
typedef int(__cdecl *r_lua_setfield_Def)(int r_lua_State, int idx, const char* k);

extern r_lua_setfield_Def		r_lua_setfield;
extern SetThreadIdentity_Def	SetThreadIdentity;

#endif