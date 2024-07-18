#pragma once
#include "addresses.h"
#include "Luau/include/lua.h"

//What the Fuck? You arent using the luau files for this?
//IF YOU WANT TO SKID FUNCTIONS GO ALL THE WAY BOTTOM OF THIS FILE
//skidded structures from roblox
typedef union GCObject GCObject;
#define cast_to(t, exp) ((t)(exp))
#define cast_int(i) cast_to(int, (i))

#define cast_num(i) cast_to(double, (i))

#define CommonHeader \
     uint8_t marked = 0; uint8_t memcat = 1; uint8_t tt = 2

typedef struct GCheader
{
    CommonHeader;
} GCheader;

const char* const luaT_typenames[] = {
    // ORDER TYPE
    "nil",
    "boolean",


    "userdata",
    "number",
    "vector",

    "string",


    "table",
    "function",
    "userdata",
    "thread",
};

typedef union
{
    GCObject* gc;
    void* p;
    double n;
    int b;
    float v[2];
} Value;

typedef struct lua_TValue
{
    Value value;
    int extra[1];
    int tt;
} TValue;

typedef TValue* StkId; // index to stack elements
//speed...GET OFF THE DRUGS...
//INCOMING: DISGUSTING DEFINITIONSS

#define setobj(L, obj1, obj2) \
    { \
        const TValue* o2 = (obj2); \
        TValue* o1 = (obj1); \
        *o1 = *o2; \
    }

#define rbx_increment() uintptr_t* top_new = reinterpret_cast<uintptr_t*>(rl + top); *top_new -= 16;//decrement the top ptr of lua state

#define setnilvalue(obj) ((obj)->tt = LUA_TNIL)

#define setnvalue(obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.n = (x); \
        i_o->tt = LUA_TNUMBER; \
    }

#if LUA_VECTOR_SIZE == 4
#define setvvalue(obj, x, y, z, w) \
    { \
        TValue* i_o = (obj); \
        float* i_v = i_o->value.v; \
        i_v[0] = (x); \
        i_v[1] = (y); \
        i_v[2] = (z); \
        i_v[3] = (w); \
        i_o->tt = LUA_TVECTOR; \
    }
#else
#define setvvalue(obj, x, y, z, w) \
    { \
        TValue* i_o = (obj); \
        float* i_v = i_o->value.v; \
        i_v[0] = (x); \
        i_v[1] = (y); \
        i_v[2] = (z); \
        i_o->tt = LUA_TVECTOR; \
    }
#endif

#define setpvalue(obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.p = (x); \
        i_o->tt = LUA_TLIGHTUSERDATA; \
    }

#define setbvalue(obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.b = (x); \
        i_o->tt = LUA_TBOOLEAN; \
    }

#define setsvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TSTRING; \
    }

#define setuvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TUSERDATA; \
    }

#define setthvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TTHREAD; \
    }

#define setclvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TFUNCTION; \
    }

#define sethvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TTABLE; \
    }

#define setptvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TPROTO; \
    }

#define setupvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TUPVAL; \
    }

#define setobj(L, obj1, obj2) \
    { \
        const TValue* o2 = (obj2); \
        TValue* o1 = (obj1); \
        *o1 = *o2; \
    }

//inlined functions rewritten for luau REALLY zesty
/*
if you want to write your own, let me get you started
pushnil is one of the easiest ones, since Tvalue is already provided
you just need to get some 'top' and base but heres a starter:

*reinterpret_cast<TValue**>(L + top)
*reinterpret_cast<TValue**>(L + base)
USE that for ALL of your L->top or L->base
*/

void rbx_settop(uintptr_t L, int idx)
{
    TValue* TOP = *reinterpret_cast<TValue**>(L + top);
    TValue* BASE = *reinterpret_cast<TValue**>(L + base);

    if (idx >= 0)
    {
        while (TOP < BASE + idx)
            setnilvalue(TOP++);

       TOP = BASE + idx;
    }
    else
    {
        TOP += idx + 1;
    }
}

void rbx_pushobject(uintptr_t L, const TValue* o)
{
    TValue* TOP = *reinterpret_cast<TValue**>(L + top);
    TValue* BASE = *reinterpret_cast<TValue**>(L + base);

    setobj(L, TOP, o);
    rbx_increment();
}

uintptr_t rbx_newthread(uintptr_t L) {
    L = 0;//closestate(real)
    return L;//IN YOUR DREAMS SKID
}

int rbx_absindex(uintptr_t L, int idx)
{
    TValue* TOP = *reinterpret_cast<TValue**>(L + top);
    TValue* BASE = *reinterpret_cast<TValue**>(L + base);

    return idx > 0 || lua_ispseudo(idx) ? idx : cast_int(TOP - BASE) + idx + 1;
}

const char* rbx_typename(uintptr_t L, int t)
{
    return (t == LUA_TNONE) ? "no value" : luaT_typenames[t];
}

void rbx_pushnil(uintptr_t L)
{
    TValue* TOP = *reinterpret_cast<TValue**>(L + top);

    setnilvalue(TOP);
    rbx_increment();
}

void rbx_pushnumber(uintptr_t L, double n)
{
    TValue* TOP = *reinterpret_cast<TValue**>(L + top);

    setnvalue(TOP, n);
    rbx_increment();
}

void rbx_pushinteger(uintptr_t L, int n)
{
    TValue* TOP = *reinterpret_cast<TValue**>(L + top);

    setnvalue(TOP, cast_num(n));
    rbx_increment();
}

void rbx_pushunsigned(uintptr_t L, unsigned u)
{
    TValue* TOP = *reinterpret_cast<TValue**>(L + top);

    setnvalue(TOP, cast_num(u));
    rbx_increment();
}

void rbx_pushboolean(uintptr_t L, int b)
{
    TValue* TOP = *reinterpret_cast<TValue**>(L + top);

    setbvalue(TOP, (b != 0)); // ensure that true is 1
    rbx_increment();
}

void rbx_pushlightuserdata(uintptr_t L, void* p)
{
    TValue* TOP = *reinterpret_cast<TValue**>(L + top);

    setpvalue(TOP, p);
    rbx_increment();
}