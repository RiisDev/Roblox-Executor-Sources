// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
// This code is based on Lua 5.x implementation licensed under MIT License; see lua_LICENSE.txt for details
#pragma once

#include "lobject.h"
#include "ltm.h"

// registry
#define registry(L) (&L->global->registry)

// extra stack space to handle TM calls and some other extras
#define EXTRA_STACK 5

#define BASIC_CI_SIZE 8

#define BASIC_STACK_SIZE (2 * LUA_MINSTACK)


typedef struct stringtable
{
    LUAVM_SHUFFLE3(
    TString** hash,
    uint32_t nuse,
    int size,
    );
} stringtable;


typedef struct CallInfo
{
    LUAVM_SHUFFLE4(
    StkId top, // 0
    StkId func, // 4
    const Instruction* savedpc, // 8
    StkId base, // 12
    );

    int nresults;
    unsigned int flags;
} CallInfo;



#define LUA_CALLINFO_RETURN (1 << 0) // should the interpreter return after returning from this callinfo? first frame must have this set
#define LUA_CALLINFO_HANDLE (1 << 1) // should the error thrown during execution get handled by continuation from this callinfo? func must be C

#define curr_func(L) (clvalue(L->ci->func))
#define ci_func(ci) (clvalue((ci)->func))
#define f_isLua(ci) (!ci_func(ci)->isC)
#define isLua(ci) (ttisfunction((ci)->func) && f_isLua(ci))

struct GCStats
{
    // data for proportional-integral controller of heap trigger value
    int32_t triggerterms[32] = {0};
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

    double pausetime = 0.0; // time from end of the last cycle to the start of a new one

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

    // specific atomic stage parts
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

    // when cycle is completed, last cycle values are updated
    uint64_t completedcycles = 0;

    GCCycleMetrics lastcycle;
    GCCycleMetrics currcycle;
};
#endif

// Callbacks that can be used to to redirect code execution from Luau bytecode VM to a custom implementation (AoT/JiT/sandboxing/...)
struct lua_ExecutionCallbacks
{
    void* context;
    void (*close)(lua_State* L);                 // called when global VM state is closed
    void (*destroy)(lua_State* L, Proto* proto); // called when function is destroyed
    int (*enter)(lua_State* L, Proto* proto);    // called when function is about to start/resume (when execdata is present), return 0 to exit VM
    void (*setbreakpoint)(lua_State* L, Proto* proto, int line); // called when a breakpoint is set in a function
};



typedef struct global_State
{
    stringtable strt;

    lua_Alloc frealloc;
    void* ud;

    uint8_t currentwhite;
    uint8_t gcstate;

    GCObject* gray;
    GCObject* grayagain;
    GCObject* weak;

    LUAVM_SHUFFLE5(
    size_t GCthreshold,
    size_t totalbytes,
    int gcgoal,
    int gcstepmul,
    int gcstepsize,
    );

    struct lua_Page* freepages[LUA_SIZECLASSES];
    struct lua_Page* freegcopages[LUA_SIZECLASSES];
    struct lua_Page* allgcopages;
    struct lua_Page* sweepgcopage;

    size_t memcatbytes[LUA_MEMORY_CATEGORIES];

    LUAVM_SHUFFLE5(
    struct lua_State* mainthread,
    UpVal uvhead,
    struct Table* mt[LUA_T_COUNT],
    vmvalue4_t<TString*> ttname[LUA_T_COUNT],
    vmvalue4_t<TString*> tmname[TM_N],
    );

    TValue pseudotemp;

    TValue registry;
    int registryfree;

    struct lua_jmpbuf* errorjmp;

    uint64_t rngstate;
    uint64_t ptrenckey[4];

    void (*udatagc[LUA_UTAG_LIMIT])(lua_State*, void*);

    lua_Callbacks cb;

#if LUA_CUSTOM_EXECUTION
    lua_ExecutionCallbacks ecb;
#endif

    GCStats gcstats;

#ifdef LUAI_GCMETRICS
    GCMetrics gcmetrics;
#endif
} global_State;



struct lua_State
{
    CommonHeader;

    uint8_t status; //3
    uint8_t activememcat; //4

    bool isactive; //5
    bool singlestep; //6

    LUAVM_SHUFFLE6(
    StkId top,
    StkId base,
    vmvalue4_t<global_State*> global,
    CallInfo* ci,
    StkId stack_last,
    StkId stack,
    );

    CallInfo* end_ci; //32
    CallInfo* base_ci; //36

    vmvalue4_t<int> stacksize; //40
    int size_ci; //44

    unsigned short nCcalls; //48
    unsigned short baseCcalls; //50

    int cachedslot; //52

    UpVal* openupval; //56
    GCObject* gclist; //60
    Table* gt; //64

    TString* namecall; //68

    void* userdata; //72
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
    struct lua_State th; // thread
};

// macros to convert a GCObject into a specific value
#define gco2ts(o) check_exp((o)->gch.tt == LUA_TSTRING, &((o)->ts))
#define gco2u(o) check_exp((o)->gch.tt == LUA_TUSERDATA, &((o)->u))
#define gco2cl(o) check_exp((o)->gch.tt == LUA_TFUNCTION, &((o)->cl))
#define gco2h(o) check_exp((o)->gch.tt == LUA_TTABLE, &((o)->h))
#define gco2p(o) check_exp((o)->gch.tt == LUA_TPROTO, &((o)->p))
#define gco2uv(o) check_exp((o)->gch.tt == LUA_TUPVAL, &((o)->uv))
#define gco2th(o) check_exp((o)->gch.tt == LUA_TTHREAD, &((o)->th))

// macro to convert any Lua object into a GCObject
#define obj2gco(v) check_exp(iscollectable(v), cast_to(GCObject*, (v) + 0))

LUAI_FUNC lua_State* luaE_newthread(lua_State* L);
LUAI_FUNC void luaE_freethread(lua_State* L, lua_State* L1, struct lua_Page* page);