#pragma once

#include "lobject.h"
#include "ltm.h"

#define registry(L) (&L->global->registry)

#define EXTRA_STACK 5

#define BASIC_CI_SIZE 8

#define BASIC_STACK_SIZE (2 * LUA_MINSTACK)

typedef struct stringtable
{
    VM_SHUFFLE3(
        TString** hash,
        uint32_t nuse,
        int size
    );
} stringtable;

typedef struct CallInfo
{
    VM_SHUFFLE4(
    StkId base,
    StkId func,
    StkId top,
    const Instruction* savedpc
    );

    int nresults;
    unsigned int flags;
} CallInfo;

#define LUA_CALLINFO_RETURN (1 << 0) 
#define LUA_CALLINFO_HANDLE (1 << 1) 
#define LUA_CALLINFO_NATIVE (1 << 2) 

#define curr_func(L) (clvalue(L->ci->func))
#define ci_func(ci) (clvalue((ci)->func))
#define f_isLua(ci) (!ci_func(ci)->isC)
#define isLua(ci) (ttisfunction((ci)->func) && f_isLua(ci))

struct GCStats
{

    int32_t triggerterms[32] = { 0 };
    uint32_t triggertermpos = 0;
    int32_t triggerintegral = 0;

    size_t atomicstarttotalsizebytes = 0;
    size_t endtotalsizebytes = 0;
    size_t heapgoalsizebytes = 0;

    double starttimestamp = 0;
    double atomicstarttimestamp = 0;
    double endtimestamp = 0;
};

#ifdef LUAI_GCMETRICS
struct GCCycleMetrics
{
    size_t starttotalsizebytes = 0;
    size_t heaptriggersizebytes = 0;

    double pausetime = 0.0;

    double starttimestamp = 0.0;
    double endtimestamp = 0.0;

    double marktime = 0.0;
    double markassisttime = 0.0;
    double markmaxexplicittime = 0.0;
    size_t markexplicitsteps = 0;
    size_t markwork = 0;

    double atomicstarttimestamp = 0.0;
    size_t atomicstarttotalsizebytes = 0;
    double atomictime = 0.0;

    double atomictimeupval = 0.0;
    double atomictimeweak = 0.0;
    double atomictimegray = 0.0;
    double atomictimeclear = 0.0;

    double sweeptime = 0.0;
    double sweepassisttime = 0.0;
    double sweepmaxexplicittime = 0.0;
    size_t sweepexplicitsteps = 0;
    size_t sweepwork = 0;

    size_t assistwork = 0;
    size_t explicitwork = 0;

    size_t propagatework = 0;
    size_t propagateagainwork = 0;

    size_t endtotalsizebytes = 0;
};

struct GCMetrics
{
    double stepexplicittimeacc = 0.0;
    double stepassisttimeacc = 0.0;

    uint64_t completedcycles = 0;

    GCCycleMetrics lastcycle;
    GCCycleMetrics currcycle;
};
#endif


struct lua_ExecutionCallbacks
{
    void* context;
    void (*close)(lua_State* L);
    void (*destroy)(lua_State* L, Proto* proto);
    int (*enter)(lua_State* L, Proto* proto);
    void (*setbreakpoint)(lua_State* L, Proto* proto, int line);
};


typedef struct global_State
{
    stringtable strt;

    lua_Alloc frealloc;
    void* ud;

    uint8_t currentwhite;
    uint8_t gcstate;

    VM_SHUFFLE3(
    GCObject* gray,
    GCObject* grayagain,
    GCObject* weak
    );

    VM_SHUFFLE5(
    size_t GCthreshold,
    size_t totalbytes,
    int gcgoal,
    int gcstepmul,
    int gcstepsize
    );

    struct lua_Page* freepages[LUA_SIZECLASSES];
    struct lua_Page* freegcopages[LUA_SIZECLASSES];
    struct lua_Page* allgcopages;
    struct lua_Page* sweepgcopage;

    size_t memcatbytes[LUA_MEMORY_CATEGORIES];

    VM_SHUFFLE5(
    struct lua_State* mainthread,
    UpVal uvhead,
    struct Table* mt[LUA_T_COUNT],
    vmvalue4<TString*> ttname[LUA_T_COUNT],
    vmvalue4<TString*> tmname[TM_N]
    );

    TValue pseudotemp;

    TValue registry;
    int registryfree;

    struct lua_jmpbuf* errorjmp;

    uint64_t rngstate;
    uint64_t ptrenckey[4];

    lua_Callbacks cb;

    lua_ExecutionCallbacks ecb;

    void (*udatagc[LUA_UTAG_LIMIT])(lua_State*, void*);

    GCStats gcstats;

#ifdef LUAI_GCMETRICS
    GCMetrics gcmetrics;
#endif
} global_State;


struct lua_State
{
    CommonHeader;
    uint8_t status;

    uint8_t activememcat;

    bool isactive;
    bool singlestep;

    VM_SHUFFLE6(
    StkId top,
    StkId base,
    vmvalue1<global_State*> global,
    CallInfo* ci,
    StkId stack_last,
    StkId stack
    );

    CallInfo* end_ci;
    CallInfo* base_ci;

    vmvalue1<int> stacksize;
    int size_ci;

    unsigned short nCcalls;
    unsigned short baseCcalls;

    int cachedslot;

    VM_SHUFFLE3(
    Table* gt,
    UpVal* openupval,
    GCObject* gclist
    );

    TString* namecall;

    void* userdata;
};

union GCObject
{
    GCheader gch;
    struct TString ts;
    struct Udata u;
    struct Closure cl;
    struct Table h;
    struct Proto p;
    struct UpVal uv;
    struct lua_State th;
};

#define gco2ts(o) check_exp((o)->gch.tt == LUA_TSTRING, &((o)->ts))
#define gco2u(o) check_exp((o)->gch.tt == LUA_TUSERDATA, &((o)->u))
#define gco2cl(o) check_exp((o)->gch.tt == LUA_TFUNCTION, &((o)->cl))
#define gco2h(o) check_exp((o)->gch.tt == LUA_TTABLE, &((o)->h))
#define gco2p(o) check_exp((o)->gch.tt == LUA_TPROTO, &((o)->p))
#define gco2uv(o) check_exp((o)->gch.tt == LUA_TUPVAL, &((o)->uv))
#define gco2th(o) check_exp((o)->gch.tt == LUA_TTHREAD, &((o)->th))

#define obj2gco(v) check_exp(iscollectable(v), cast_to(GCObject*, (v) + 0))

LUAI_FUNC lua_State* luaE_newthread(lua_State* L);
LUAI_FUNC void luaE_freethread(lua_State* L, lua_State* L1, struct lua_Page* page);