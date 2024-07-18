#pragma once
#pragma warning(suppress : 4002)

#include "lua.h"
#include "lcommon.h"

typedef union GCObject GCObject;

#define CommonHeader \
     VM_SHUFFLE3(uint8_t tt, uint8_t marked, uint8_t memcat);

typedef struct GCheader
{
    CommonHeader;
} GCheader;

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
    int extra[LUA_EXTRA_SIZE];
    int tt;
} TValue;

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

#define checkconsistency(obj) LUAU_ASSERT(!iscollectable(obj) || (ttype(obj) == (obj)->value.gc->gch.tt))

#define checkliveness(g, obj) LUAU_ASSERT(!iscollectable(obj) || ((ttype(obj) == (obj)->value.gc->gch.tt) && !isdead(g, (obj)->value.gc)))

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

#define setobj2s setobj

#define setobjt2t setobj

#define setobj2t setobj

#define setobj2n setobj

#define setttype(obj, tt) (ttype(obj) = (tt))

#define iscollectable(o) (ttype(o) >= LUA_TSTRING)

typedef TValue* StkId;

typedef struct TString
{
    CommonHeader;

    int16_t atom;

    TString* next;

    vmvalue3<unsigned int> hash;
    vmvalue4<unsigned int> len;

    char data[1];
} TString;

#define getstr(ts) (ts)->data
#define svalue(o) getstr(tsvalue(o))

typedef struct Udata
{
    CommonHeader;

    uint8_t tag;

    int len;

    vmvalue4<struct Table*> metatable;

    union
    {
        char data[1];
        L_Umaxalign dummy;
    };
} Udata;


typedef struct Proto
{
    CommonHeader;

    VM_SHUFFLE8(
    vmvalue2 <TValue*> k,
    vmvalue2 <Instruction*> code,
    vmvalue2 <struct Proto**> p,
    vmvalue2 <uint8_t*> lineinfo,
    vmvalue2 <int*> abslineinfo,
    vmvalue2 <struct LocVar*> locvars,
    vmvalue2 <TString**> upvalues,
    vmvalue2 <TString*>source,
    );

    vmvalue3<TString*> debugname;
    vmvalue4<uint8_t*> debuginsn;

    const Instruction* codeentry;
    void* execdata;
    uintptr_t exectarget;

    GCObject* gclist;

    VM_SHUFFLE9(
    int sizecode,
    int sizep,
    int sizelocvars,
    int sizeupvalues,
    int sizek,
    int sizelineinfo,
    int linegaplog2,
    int linedefined,
    int bytecodeid,
    );

    VM_SHUFFLE4(
    uint8_t nups,
    uint8_t numparams,
    uint8_t is_vararg,
    uint8_t maxstacksize
    );
} Proto;


typedef struct LocVar
{
    TString* varname;
    int startpc;
    int endpc;
    uint8_t reg;
} LocVar;

typedef struct UpVal
{
    CommonHeader;
    uint8_t markedopen;

    TValue* v;
    union
    {
        TValue value;
        struct
        {

            struct UpVal* prev;
            struct UpVal* next;

            struct UpVal* threadnext;
        } open;
    } u;
} UpVal;

#define upisopen(up) ((up)->v != &(up)->u.value)

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
            vmvalue4<lua_CFunction> f;
            vmvalue2<lua_Continuation> cont;
            vmvalue3<const char*> debugname;
            TValue upvals[1];
        } c;

        struct
        {
            vmvalue4<struct Proto*> p;
            TValue uprefs[1];
        } l;
    };
} Closure;

#define iscfunction(o) (ttype(o) == LUA_TFUNCTION && clvalue(o)->isC)
#define isLfunction(o) (ttype(o) == LUA_TFUNCTION && !clvalue(o)->isC)

typedef struct TKey
{
    ::Value value;
    int extra[LUA_EXTRA_SIZE];
    unsigned tt : 4;
    int next : 28;
} TKey;

typedef struct LuaNode
{
    TValue val;
    TKey key;
} LuaNode;

#define setnodekey(L, node, obj) \
    { \
        LuaNode* n_ = (node); \
        const TValue* i_o = (obj); \
        n_->key.value = i_o->value; \
        memcpy(n_->key.extra, i_o->extra, sizeof(n_->key.extra)); \
        n_->key.tt = i_o->tt; \
        checkliveness(L->global, i_o); \
    }

#define getnodekey(L, obj, node) \
    { \
        TValue* i_o = (obj); \
        const LuaNode* n_ = (node); \
        i_o->value = n_->key.value; \
        memcpy(i_o->extra, n_->key.extra, sizeof(i_o->extra)); \
        i_o->tt = n_->key.tt; \
        checkliveness(L->global, i_o); \
    }

typedef struct Table
{
    CommonHeader;

    VM_SHUFFLE5(
    uint8_t tmcache,
    uint8_t readonly,
    uint8_t safeenv,
    uint8_t lsizenode,
    uint8_t nodemask8
    );

    int sizearray;
    union
    {
        int lastfree;
        int aboundary;
    };

    VM_SHUFFLE4(
    vmvalue4<struct Table*> metatable,
    vmvalue4<TValue*> array,
    vmvalue4<LuaNode*> node,
    GCObject* gclist
    );
} Table;

#define lmod(s, size) (check_exp((size & (size - 1)) == 0, (cast_to(int, (s) & ((size)-1)))))

#define twoto(x) ((int)(1 << (x)))
#define sizenode(t) (twoto((t)->lsizenode))

#define luaO_nilobject (TValue*)_nilobject

#define ceillog2(x) (luaO_log2((x)-1) + 1)

LUAI_FUNC int luaO_log2(unsigned int x);
LUAI_FUNC int luaO_rawequalObj(const TValue* t1, const TValue* t2);
LUAI_FUNC int luaO_rawequalKey(const TKey* t1, const TValue* t2);
LUAI_FUNC int luaO_str2d(const char* s, double* result);
LUAI_FUNC const char* luaO_pushvfstring(lua_State* L, const char* fmt, va_list argp);
LUAI_FUNC const char* luaO_pushfstring(lua_State* L, const char* fmt, ...);
LUAI_FUNC const char* luaO_chunkid(char* buf, size_t buflen, const char* source, size_t srclen);


struct live_thread_ref
{
    int unk_0;
    lua_State* state;
    int state_id;
    int unk_1;
};


struct thread_ref
{
    live_thread_ref* ref;
    thread_ref(lua_State* L)
    {
        ref = new live_thread_ref;
        ref->unk_0 = 0;
        ref->state = L;
        lua_pushthread(L);
        ref->state_id = lua_ref(L, -1);
        lua_settop(L, 0);

        ref->unk_1 = 0;
    }
};


namespace Defines {
    using robloxFunctionType = std::intptr_t(__cdecl*)(std::intptr_t L);

    using scheduler_t = DWORD(__cdecl*)();
    static scheduler_t getTaskScheduler = reinterpret_cast<scheduler_t>(_getscheduler);

    using print_t = uintptr_t(__cdecl*)(int type, const char* source);
    static print_t stdOut = reinterpret_cast<print_t>(_print);
}