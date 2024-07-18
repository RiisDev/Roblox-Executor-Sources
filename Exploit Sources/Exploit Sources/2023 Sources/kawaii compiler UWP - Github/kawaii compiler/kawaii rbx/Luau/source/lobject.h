// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
// This code is based on Lua 5.x implementation licensed under MIT License; see lua_LICENSE.txt for details
#pragma once

#include "../include/lua.h"
#include "lcommon.h"
#include <Windows.h>
#include <iostream>


#define LUAVM_SHUFFLE4(a1, a2, a3, a4) a4; a2; a3; a1;
#define LUAVM_SHUFFLE5(a1, a2, a3, a4, a5) a3; a5; a4; a2; a1;
#define LUAVM_SHUFFLE6(a1, a2, a3, a4, a5, a6) a1; a3; a2; a6; a5; a4;
#define LUAVM_SHUFFLE3(a1, a2, a3) a2; a3; a1;


template <typename T> struct vmvalue1_t
{
public:
    operator const T() const
    {
        return (T)((uintptr_t)storage - (uintptr_t)this);
    }

    void operator=(const T& value)
    {
        storage = (T)((uintptr_t)value + (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }
private:
    T storage;
};

template <typename T> struct vmvalue2_t
{
public:
    operator const T() const
    {
        return (T)((uintptr_t)this - (uintptr_t)storage);
    }

    void operator=(const T& value)
    {
        storage = (T)((uintptr_t)this - (uintptr_t)value);
    }

    const T operator->() const
    {
        return operator const T();
    }
private:
    T storage;
};

template <typename T> struct vmvalue3_t
{
public:
    operator const T() const
    {
        return (T)((uintptr_t)storage ^ (uintptr_t)this);
    }

    void operator=(const T& value)
    {
        storage = (T)((uintptr_t)value ^ (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }
private:
    T storage;
};

template <typename T> struct vmvalue4_t
{
public:
    operator const T() const
    {
        return (T)((uintptr_t)this + (uintptr_t)storage);
    }

    void operator=(const T& value)
    {
        storage = (T)((uintptr_t)value - (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }
private:
    T storage;
};


static int ASLR(uintptr_t addy)
{
    return addy - 0x400000 + reinterpret_cast<uintptr_t>(GetModuleHandleA(0));
}


typedef union GCObject GCObject;

/*
** Common Header for all collectible objects (in macro form, to be included in other objects)
*/
// clang-format off
#define CommonHeader \
     uint8_t marked = 0; uint8_t memcat = 1; uint8_t tt = 2
// clang-format on

/*
** Common header in struct form
*/
typedef struct GCheader
{
    CommonHeader;
} GCheader;

/*
** Union of all Lua values
*/
typedef union
{
    GCObject* gc;
    void* p;
    double n;
    int b;
    float v[2]; // v[0], v[1] live here; v[2] lives in TValue::extra
} Value;

/*
** Tagged Values
*/

typedef struct lua_TValue
{
    Value value;
    int extra[LUA_EXTRA_SIZE];
    int tt;
} TValue;

// Macros to test type
#define ttisnil(o) (ttype(o) == LUA_TNIL)
#define ttisnumber(o) (ttype(o) == LUA_TNUMBER)
#define ttisstring(o) (ttype(o) == LUA_TSTRING)
#define ttistable(o) (ttype(o) == LUA_TTABLE)
#define ttisfunction(o) (ttype(o) == LUA_TFUNCTION)
#define ttisboolean(o) (ttype(o) == LUA_TBOOLEAN)
#define ttisuserdata(o) (ttype(o) == LUA_TUSERDATA)
#define ttisthread(o) (ttype(o) == LUA_TTHREAD)
#define ttislightuserdata(o) (ttype(o) == LUA_TLIGHTUSERDATA)
#define ttisvector(o) (ttype(o) == LUA_TVECTOR)
#define ttisupval(o) (ttype(o) == LUA_TUPVAL)

// Macros to access values
#define ttype(o) ((o)->tt)
#define gcvalue(o) check_exp(iscollectable(o), (o)->value.gc)
#define pvalue(o) check_exp(ttislightuserdata(o), (o)->value.p)
#define nvalue(o) check_exp(ttisnumber(o), (o)->value.n)
#define vvalue(o) check_exp(ttisvector(o), (o)->value.v)
#define tsvalue(o) check_exp(ttisstring(o), &(o)->value.gc->ts)
#define uvalue(o) check_exp(ttisuserdata(o), &(o)->value.gc->u)
#define clvalue(o) check_exp(ttisfunction(o), &(o)->value.gc->cl)
#define hvalue(o) check_exp(ttistable(o), &(o)->value.gc->h)
#define bvalue(o) check_exp(ttisboolean(o), (o)->value.b)
#define thvalue(o) check_exp(ttisthread(o), &(o)->value.gc->th)
#define upvalue(o) check_exp(ttisupval(o), &(o)->value.gc->uv)

#define l_isfalse(o) (ttisnil(o) || (ttisboolean(o) && bvalue(o) == 0))

/*
** for internal debug only
*/
#define checkconsistency(obj) LUAU_ASSERT(!iscollectable(obj) || (ttype(obj) == (obj)->value.gc->gch.tt))

#define checkliveness(g, obj) LUAU_ASSERT(!iscollectable(obj) || ((ttype(obj) == (obj)->value.gc->gch.tt) && !isdead(g, (obj)->value.gc)))

// Macros to set values
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
        checkliveness(L->global, i_o); \
    }

#define setuvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TUSERDATA; \
        checkliveness(L->global, i_o); \
    }

#define setthvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TTHREAD; \
        checkliveness(L->global, i_o); \
    }

#define setclvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TFUNCTION; \
        checkliveness(L->global, i_o); \
    }

#define sethvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TTABLE; \
        checkliveness(L->global, i_o); \
    }

#define setptvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TPROTO; \
        checkliveness(L->global, i_o); \
    }

#define setupvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = cast_to(GCObject*, (x)); \
        i_o->tt = LUA_TUPVAL; \
        checkliveness(L->global, i_o); \
    }

#define setobj(L, obj1, obj2) \
    { \
        const TValue* o2 = (obj2); \
        TValue* o1 = (obj1); \
        *o1 = *o2; \
        checkliveness(L->global, o1); \
    }

/*
** different types of sets, according to destination
*/

// to stack
#define setobj2s setobj
// from table to same table (no barrier)
#define setobjt2t setobj
// to table (needs barrier)
#define setobj2t setobj
// to new object (no barrier)
#define setobj2n setobj

#define setttype(obj, tt) (ttype(obj) = (tt))

#define iscollectable(o) (ttype(o) >= LUA_TSTRING)

typedef TValue* StkId; // index to stack elements



typedef struct TString
{
    CommonHeader;
    int16_t atom; //4

    TString* next; //8

    vmvalue4_t<unsigned int> hash; //12
    vmvalue2_t<unsigned int> len; //16

    char data[1]; //20
} TString;

#define getstr(ts) (ts)->data
#define svalue(o) getstr(tsvalue(o))


typedef struct Udata
{
    CommonHeader;

    uint8_t tag;

    int len;

    vmvalue2_t<struct Table*> metatable;

    union
    {
        char data[1];
        L_Umaxalign dummy;
    };
} Udata;


typedef struct Proto
{
    CommonHeader;

    vmvalue1_t<int*> abslineinfo; // 4   
    vmvalue1_t<uint8_t*> lineinfo; // 8 
    vmvalue1_t<TString*> source; // 12  
    vmvalue1_t<struct Proto**> p; // 16
    vmvalue1_t<Instruction*> code; // 20     
    vmvalue1_t<TValue*> k; // 24             
    vmvalue1_t<TString**> upvalues; // 28
    vmvalue1_t<struct LocVar*> locvars; // 32 

    vmvalue4_t<TString*> debugname; //36
    vmvalue2_t<uint8_t*> debuginsn; //40

#if LUA_CUSTOM_EXECUTION
    void* execdata;
#endif

    GCObject* gclist; //44

    int sizeupvalues; // 48
    int linegaplog2; // 52
    int linedefined; // 56
    int sizelineinfo; // 60
    int sizecode; // 64
    int bytecodeid; // 68
    int sizek; // 72
    int sizelocvars; // 76
    int sizep; // 80

    uint8_t nups;
    uint8_t numparams;
    uint8_t is_vararg;
    uint8_t maxstacksize;
} Proto;


typedef struct LocVar
{
    TString* varname;
    int startpc;
    int endpc;   // first point where variable is dead
    uint8_t reg; // register slot, relative to base, where variable is stored
} LocVar;

/*
** Upvalues
*/

typedef struct UpVal
{
    CommonHeader;
    uint8_t markedopen; // set if reachable from an alive thread (only valid during atomic)

    // 4 byte padding (x64)

    TValue* v; // points to stack or to its own value
    union
    {
        TValue value; // the value (when closed)
        struct
        {
            // global double linked list (when open)
            struct UpVal* prev;
            struct UpVal* next;

            // thread linked list (when open)
            struct UpVal* threadnext;
        } open;
    } u;
} UpVal;

#define upisopen(up) ((up)->v != &(up)->u.value)

/*
** Closures
*/

typedef struct Closure
{
    CommonHeader;

    uint8_t isC;
    uint8_t nupvalues;
    uint8_t stacksize;
    uint8_t preload;

    GCObject* gclist;
    struct Table* env;

    union
    {
        struct
        {
            vmvalue2_t<lua_CFunction> f;
            vmvalue1_t<lua_Continuation> cont;
            vmvalue3_t<const char*> debugname;
            TValue upvals[1];
        } c;

        struct
        {
            vmvalue2_t<struct Proto*> p;
            TValue uprefs[1];
        } l;
    };
} Closure;

#define iscfunction(o) (ttype(o) == LUA_TFUNCTION && clvalue(o)->isC)
#define isLfunction(o) (ttype(o) == LUA_TFUNCTION && !clvalue(o)->isC)

/*
** Tables
*/

typedef struct TKey
{
    ::Value value;
    int extra[LUA_EXTRA_SIZE];
    unsigned tt : 4;
    int next : 28; // for chaining
} TKey;

typedef struct LuaNode
{
    TValue val;
    TKey key;
} LuaNode;

// copy a value into a key
#define setnodekey(L, node, obj) \
    { \
        LuaNode* n_ = (node); \
        const TValue* i_o = (obj); \
        n_->key.value = i_o->value; \
        memcpy(n_->key.extra, i_o->extra, sizeof(n_->key.extra)); \
        n_->key.tt = i_o->tt; \
        checkliveness(L->global, i_o); \
    }

// copy a value from a key
#define getnodekey(L, obj, node) \
    { \
        TValue* i_o = (obj); \
        const LuaNode* n_ = (node); \
        i_o->value = n_->key.value; \
        memcpy(i_o->extra, n_->key.extra, sizeof(i_o->extra)); \
        i_o->tt = n_->key.tt; \
        checkliveness(L->global, i_o); \
    }


//LUAVM_SHUFFLE4(a1, a2, a3, a4) a4; a2; a3; a1;
typedef struct Table
{
    CommonHeader;

    LUAVM_SHUFFLE5(
    uint8_t tmcache,
    uint8_t readonly,
    uint8_t safeenv,
    uint8_t lsizenode,
    uint8_t nodemask8,
    );

    int sizearray; //8

    union
    {
        int lastfree; //12
        int aboundary; //12
    };

    LUAVM_SHUFFLE4(
    vmvalue2_t<struct Table*> metatable,
    vmvalue2_t<TValue*> array,
    vmvalue2_t<LuaNode*> node,
    GCObject* gclist,
    );

} Table;



#define lmod(s, size) (check_exp((size & (size - 1)) == 0, (cast_to(int, (s) & ((size)-1)))))

#define twoto(x) ((int)(1 << (x)))
#define sizenode(t) (twoto((t)->lsizenode))

#define luaO_nilobject (TValue*)ASLR(0x2882770)

#define ceillog2(x) (luaO_log2((x)-1) + 1)

LUAI_FUNC int luaO_log2(unsigned int x);
LUAI_FUNC int luaO_rawequalObj(const TValue* t1, const TValue* t2);
LUAI_FUNC int luaO_rawequalKey(const TKey* t1, const TValue* t2);
LUAI_FUNC int luaO_str2d(const char* s, double* result);
LUAI_FUNC const char* luaO_pushvfstring(lua_State* L, const char* fmt, va_list argp);
LUAI_FUNC const char* luaO_pushfstring(lua_State* L, const char* fmt, ...);
LUAI_FUNC const char* luaO_chunkid(char* buf, size_t buflen, const char* source, size_t srclen);