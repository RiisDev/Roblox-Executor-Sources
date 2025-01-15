// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
// This code is based on Lua 5.x implementation licensed under MIT License; see lua_LICENSE.txt for details
#pragma once

#include "lobject.h"

/*
 * WARNING: if you change the order of this enumeration,
 * grep "ORDER TM"
 */
// clang-format off
typedef enum
{ 
    vm_shuffle7(vm_shuffle_comma, TM_INDEX,
    TM_NEWINDEX,
    TM_MODE,
    TM_NAMECALL,
    TM_CALL,
    TM_ITER,
    TM_LEN),

    //a7 d a5 d a6 d a3 d a1 d a2 d a4

    TM_EQ,
    
    vm_shuffle8(vm_shuffle_comma, TM_ADD,
    TM_SUB,
    TM_MUL,
    TM_DIV,
    TM_IDIV,
    TM_MOD,
    TM_POW,
    TM_UNM),

    //a6 d a4 d a1 d a2 d a8 d a5 d a7 d a3

    vm_shuffle5(vm_shuffle_comma, TM_LT,
    TM_LE,
    TM_CONCAT,
    TM_TYPE,
    TM_METATABLE),

    TM_N // number of elements in the enum
} TMS;
// clang-format on

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
