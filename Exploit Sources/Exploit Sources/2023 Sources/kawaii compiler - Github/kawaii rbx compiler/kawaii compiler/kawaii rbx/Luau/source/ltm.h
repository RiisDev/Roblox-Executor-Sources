// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
// This code is based on Lua 5.x implementation licensed under MIT License; see lua_LICENSE.txt for details
#pragma once

#include "lobject.h"



typedef enum
{
    TM_ITER,
    TM_NEWINDEX,
    TM_LEN,
    TM_MODE,
    TM_INDEX,
    TM_NAMECALL,
    TM_CALL,
    TM_EQ,
    TM_POW,
    TM_SUB,
    TM_UNM,
    TM_MUL,
    TM_ADD,
    TM_DIV,
    TM_MOD,
    TM_CONCAT,
    TM_METATABLE,
    TM_TYPE,
    TM_LE,
    TM_LT,

    TM_N
} TMS;


#define gfasttm(g, et, e) ((et) == NULL ? NULL : ((et)->tmcache & (1u << (e))) ? NULL : luaT_gettm(et, e, (g)->tmname[e]))

#define fasttm(l, et, e) gfasttm(l->global, et, e)
#define fastnotm(et, e) ((et) == NULL || ((et)->tmcache & (1u << (e))))

LUAI_DATA const char* const luaT_typenames[];
LUAI_DATA const char* const luaT_eventname[];

LUAI_FUNC const TValue* luaT_gettm(Table* events, TMS event, TString* ename);
LUAI_FUNC const TValue* luaT_gettmbyobj(lua_State* L, const TValue* o, TMS event);

LUAI_FUNC const TString* luaT_objtypenamestr(lua_State* L, const TValue* o);
LUAI_FUNC const char* luaT_objtypename(lua_State* L, const TValue* o);

LUAI_FUNC void luaT_init(lua_State* L);
