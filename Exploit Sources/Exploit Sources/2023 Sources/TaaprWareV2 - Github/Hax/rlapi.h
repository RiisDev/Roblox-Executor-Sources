#pragma once

#include "addresses.h"

#define LUAI_MAXCSTACK 8000
#define LUA_REGISTRYINDEX (-LUAI_MAXCSTACK - 2000)
#define LUA_ENVIRONINDEX (-LUAI_MAXCSTACK - 2001)
#define LUA_GLOBALSINDEX (-LUAI_MAXCSTACK - 2002)
#define lua_upvalueindex(i) (LUA_GLOBALSINDEX - (i))
#define lua_ispseudo(i) ((i) <= LUA_REGISTRYINDEX)
/*
Type 0: nil
Type 1: boolean
Type 2: number
Type 3: vector
Type 4: userdata
Type 5: string
Type 6: thread
Type 7: function
Type 8: userdata
Type 9: table
*/
enum lua_Type {
    LUA_TNIL = 0,     // must be 0 due to lua_isnoneornil
    LUA_TBOOLEAN = 1, // must be 1 due to l_isfalse

    LUA_TNUMBER,
    LUA_TVECTOR,
    LUA_TLIGHTUSERDATA,

    LUA_TSTRING, // all types above this must be value types, all types below this must be GC types - see iscollectable

    LUA_TTHREAD,
    LUA_TFUNCTION,
    LUA_TUSERDATA,
    LUA_TTABLE,

    // values below this line are used in GCObject tags but may never show up in TValue type tags
    LUA_TPROTO,
    LUA_TUPVAL,
    LUA_TDEADKEY,

    // the count of TValue type tags
    LUA_T_COUNT = LUA_TPROTO
};

#define CommonHeader \
     LUAVM_SHUFFLE3(;, uint8_t tt, uint8_t marked, uint8_t memcat)
typedef struct GCheader {
    CommonHeader;
} GCheader;
typedef struct {
    char padding[88];
} GCHeader_size;

// its a UNION not a struct
union GCObject {
    GCheader gch;
    UINT ts; // tstring
    UINT u; // userdata (udata)
    UINT cl; // closure
    UINT h; // table
    UINT p; // proto
    UINT uv; // upvalue (upval)
    UINT th; // thread
    GCHeader_size padding;
};

// its a UNION not a struct
typedef union {
    GCObject* gc;
    double n;
    int b;
    void* p;
    float v[2]; // v[0], v[1] live here; v[2] lives in TValue::extra (USED FOR VECTORS)
} Value;

typedef struct lua_TValue {
    Value value;
    int extra[1]; // For when a vector is being 
    int tt;
} TValue;
typedef TValue* StkId;

extern TValue* luaO_nilobject;

#define api_incr_top(rL) rL_get(StkId, top) += 1;

#define setnilvalue(obj) ((obj)->tt = LUA_TNIL)

#define setnvalue(obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.n = (x); \
        i_o->tt = LUA_TNUMBER; \
    }

#define setvvalue(obj, x, y, z, w) \
    { \
        TValue* i_o = (obj); \
        float* i_v = i_o->value.v; \
        i_v[0] = (x); \
        i_v[1] = (y); \
        i_v[2] = (z); \
        i_o->tt = LUA_TVECTOR; \
    }

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
        i_o->value.gc = (GCObject*)(x); \
        i_o->tt = LUA_TSTRING; \
    }

#define setuvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = (GCObject*)(x); \
        i_o->tt = LUA_TUSERDATA; \
    }

#define setthvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = (GCObject*)(x); \
        i_o->tt = LUA_TTHREAD; \
    }

#define setclvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = (GCObject*)(x); \
        i_o->tt = LUA_TFUNCTION; \
    }

#define sethvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = (GCObject*)(x); \
        i_o->tt = LUA_TTABLE; \
    }

#define setptvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = (GCObject*)(x); \
        i_o->tt = LUA_TPROTO; \
    }

#define setupvalue(L, obj, x) \
    { \
        TValue* i_o = (obj); \
        i_o->value.gc = (GCObject*)(x); \
        i_o->tt = LUA_TUPVAL; \
    }

#define setobj(L, obj1, obj2) \
    { \
        const TValue* o2 = (obj2); \
        TValue* o1 = (obj1); \
        *o1 = *o2; \
    }
// to stack
#define setobj2s setobj
// from table to same table (no barrier)
#define setobjt2t setobj
// to table (needs barrier)
#define setobj2t setobj
// to new object (no barrier)
#define setobj2n setobj

#define setttype(obj, tt) (ttype(obj) = (tt))

#define ttype(o) ((o)->tt)
#define ttisnil(o) (ttype(o) == LUA_TNIL)
#define ttisnumber(o) (ttype(o) == LUA_TNUMBER)
#define iscollectable(o) (ttype(o) >= LUA_TSTRING)
#define ttisstring(o) (ttype(o) == LUA_TSTRING)
#define ttistable(o) (ttype(o) == LUA_TTABLE)
#define ttisfunction(o) (ttype(o) == LUA_TFUNCTION)
#define ttisboolean(o) (ttype(o) == LUA_TBOOLEAN)
#define ttisuserdata(o) (ttype(o) == LUA_TUSERDATA)
#define ttisthread(o) (ttype(o) == LUA_TTHREAD)
#define ttislightuserdata(o) (ttype(o) == LUA_TLIGHTUSERDATA)
#define ttisvector(o) (ttype(o) == LUA_TVECTOR)
#define ttisupval(o) (ttype(o) == LUA_TUPVAL)
#define gcvalue(o) (o)->value.gc
#define pvalue(o) (o)->value.p
#define nvalue(o) (o)->value.n
#define vvalue(o) (o)->value.v
#define tsvalue(o) &(o)->value.gc->ts
#define uvalue(o) &(o)->value.gc->u
#define clvalue(o) &(o)->value.gc->cl
#define hvalue(o) &(o)->value.gc->h
#define bvalue(o) (o)->value.b
#define thvalue(o) &(o)->value.gc->th
#define upvalue(o) &(o)->value.gc->uv
#define l_isfalse(o) (ttisnil(o) || (ttisboolean(o) && bvalue(o) == 0))
#define lmod(s, size) ((int)((s)&((size)-1)))
#define MAXSSIZE (1 << 30)
#define lua_pop(L, n) settop(L, -(n)-1)

// Cry about it
#define structptr_get(var_name, type, struct_name, member) ((offsets::struct_name::member##_get((UINT)(var_name + offsets::struct_name::member))))
#define struct_get(var_name, type, struct_name, member) ((type)((UINT)(var_name) + offsets::struct_name::member))
#define structptr_set(var_name, type, struct_name, member, value) (offsets::struct_name::member##_set((UINT)(var_name + offsets::struct_name::member), (UINT)(value)))
#define struct_set(var_name, type, struct_name, member, value) ((type)((UINT)(var_name) + offsets::struct_name::member) = (type)(value))
#define rL_get(type, member) (*reinterpret_cast<type*>(rL + offsets::rL::member))
#define rLapi_func extern

// For rCommonHeader->marked
#define resetbits(x, m) ((x) &= cast_to(uint8_t, ~(m)))
#define setbits(x, m) ((x) |= (m))
#define testbits(x, m) ((x) & (m))
#define bitmask(b) (1 << (b))
#define bit2mask(b1, b2) (bitmask(b1) | bitmask(b2))
#define l_setbit(x, b) setbits(x, bitmask(b))
#define resetbit(x, b) resetbits(x, bitmask(b))
#define testbit(x, b) testbits(x, bitmask(b))
#define set2bits(x, b1, b2) setbits(x, (bit2mask(b1, b2)))
#define reset2bits(x, b1, b2) resetbits(x, (bit2mask(b1, b2)))
#define test2bits(x, b1, b2) testbits(x, (bit2mask(b1, b2)))
#define WHITE0BIT 0
#define WHITE1BIT 1
#define BLACKBIT 2
#define FIXEDBIT 3
#define WHITEBITS bit2mask(WHITE0BIT, WHITE1BIT)
#define iswhite(x) test2bits(structptr_get(x, uint8_t, rCommonHeader, marked), WHITE0BIT, WHITE1BIT)
#define isblack(x) testbit(structptr_get(x, uint8_t, rCommonHeader, marked), BLACKBIT)
#define isgray(x) (!testbits(structptr_get(x, uint8_t, rCommonHeader, marked), WHITEBITS | bitmask(BLACKBIT)))
#define isfixed(x) testbit(structptr_get(x, uint8_t, rCommonHeader, marked), FIXEDBIT)
#define otherwhite(g) (structptr_get(v, uint8_t, g, currentwhite) ^ WHITEBITS)
#define isdead(g, v) ((structptr_get(v, uint8_t, rCommonHeader, marked) & (WHITEBITS | bitmask(FIXEDBIT))) == (otherwhite(g) & WHITEBITS))
#define changewhite(x) (structptr_get(x, uint8_t, rCommonHeader, marked) ^= WHITEBITS)
#define gray2black(x) l_setbit(structptr_get(x, uint8_t, rCommonHeader, marked), BLACKBIT)
#define luaC_white(g) ((uint8_t)(structptr_get(g, uint8_t, rG, currentwhite)) & WHITEBITS)
#define sizestring(len) (offsets::rTString::data + len + 1)
#define luaM_newgco(rL, t, size, memcat) ((t*)(functions::luau::luaM_newgco_(rL, size, memcat)))
#define luaC_init(rL_, o, tt_) \
    { \
        structptr_set(o, uint8_t, rTString, tt, tt_); \
        structptr_set(o, uint8_t, rTString, memcat, structptr_get(rL_, uint8_t, rL, activememcat)); \
        structptr_set(o, uint8_t, rTString, marked, structptr_get(structptr_get(rL_, UINT, rL, global), uint8_t, rG, currentwhite)); \
    }

#define ATOM_UNDEF -32768

const extern types::pseudo2addr pseudo2addr;
rLapi_func int gettop(UINT rL);
rLapi_func void settop(UINT rL, int idx);
rLapi_func TValue* index2addr(UINT rL, int idx);
rLapi_func void pushvalue(UINT rL, int idx);
rLapi_func void pushnil(UINT rL);
rLapi_func void pushnumber(UINT rL, double n);
rLapi_func void pushinteger(UINT rL, int n);
rLapi_func void pushunsigned(UINT rL, unsigned u);
rLapi_func void pushvector(UINT rL, float x, float y, float z);
rLapi_func void pushboolean(UINT rL, int b);
rLapi_func void pushtable(UINT rL, UINT t);
rLapi_func unsigned int luaS_hash(const char* str, size_t len);
rLapi_func void pushlstring(UINT rL, const char* s, size_t len);
rLapi_func void pushstring(UINT rL, const char* s);
rLapi_func UINT luaS_newlstr(UINT rL, const char* str, size_t l);
rLapi_func UINT newlstr(UINT rL, const char* str, size_t l, unsigned int h);
rLapi_func void insert(UINT rL, int idx);
rLapi_func void xmove(UINT from, UINT to, int n);
