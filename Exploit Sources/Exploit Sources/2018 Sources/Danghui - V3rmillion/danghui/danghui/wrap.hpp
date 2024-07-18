#pragma once

#include "Lua/lua.hpp"
#include "Lua/lobject.h"

lua_State* danghui_openstate();
void danghui_Aexecute(const char* script);
int danghui_int3fnhandler(DWORD rL);
int danghui_wraplfn(lua_State* L, unsigned int rL, LClosure* fn);

typedef enum {
	R_TM_NAMECALL,
	R_TM_INDEX,
	R_TM_GC,
	R_TM_NEWINDEX,
	R_TM_MODE,
	R_TM_EQ,
	R_TM_MUL,
	R_TM_DIV,
	R_TM_ADD,
	R_TM_MOD,
	R_TM_UNM,
	R_TM_SUB,
	R_TM_POW,
	R_TM_CALL,
	R_TM_LEN,
	R_TM_LE,
	R_TM_LT,
	R_TM_CONCAT
} r_TMS;