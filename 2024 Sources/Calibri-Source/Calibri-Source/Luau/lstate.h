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

// clang-format off
typedef struct stringtable
{
    vm_shuffle3(vm_shuffle_semicolon, TString** hash,
    uint32_t nuse,
    int size);
} stringtable;
// clang-format on

/*
** informations about a call
**
** the general Lua stack frame structure is as follows:
** - each function gets a stack frame, with function "registers" being stack slots on the frame
** - function arguments are associated with registers 0+
** - function locals and temporaries follow after; usually locals are a consecutive block per scope, and temporaries are allocated after this, but
*this is up to the compiler
**
** when function doesn't have varargs, the stack layout is as follows:
** ^ (func) ^^ [fixed args] [locals + temporaries]
** where ^ is the 'func' pointer in CallInfo struct, and ^^ is the 'base' pointer (which is what registers are relative to)
**
** when function *does* have varargs, the stack layout is more complex - the runtime has to copy the fixed arguments so that the 0+ addressing still
*works as follows:
** ^ (func) [fixed args] [varargs] ^^ [fixed args] [locals + temporaries]
**
** computing the sizes of these individual blocks works as follows:
** - the number of fixed args is always matching the `numparams` in a function's Proto object; runtime adds `nil` during the call execution as
*necessary
** - the number of variadic args can be computed by evaluating (ci->base - ci->func - 1 - numparams)
**
** the CallInfo structures are allocated as an array, with each subsequent call being *appended* to this array (so if f calls g, CallInfo for g
*immediately follows CallInfo for f)
** the `nresults` field in CallInfo is set by the caller to tell the function how many arguments the caller is expecting on the stack after the
*function returns
** the `flags` field in CallInfo contains internal execution flags that are important for pcall/etc, see LUA_CALLINFO_*
*/
// clang-format off
typedef struct CallInfo
{
    vm_shuffle4(vm_shuffle_semicolon, StkId base,
    StkId func,
    StkId top,
    const Instruction* savedpc);

    int nresults;
    unsigned int flags;
} CallInfo;
// clang-format on

#define LUA_CALLINFO_RETURN (1 << 0) // should the interpreter return after returning from this callinfo? first frame must have this set
#define LUA_CALLINFO_HANDLE (1 << 1) // should the error thrown during execution get handled by continuation from this callinfo? func must be C
#define LUA_CALLINFO_NATIVE (1 << 2) // should this function be executed using execution callback for native code

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
    void (*disable)(lua_State* L, Proto* proto); // called when function has to be switched from native to bytecode in the debugger
    size_t (*getmemorysize)(lua_State* L, Proto* proto); // called to request the size of memory associated with native part of the Proto
    uint8_t (*gettypemapping)(lua_State* L, const char* str, size_t len); // called to get the userdata type index
};

/*
** `global state', shared by all threads of this state
*/
// clang-format off
typedef struct global_State
{
    stringtable strt;

    lua_Alloc frealloc;
    void* ud;

    uint8_t currentwhite;
    uint8_t gcstate;

    vm_shuffle3(vm_shuffle_semicolon, GCObject* gray,
    GCObject* grayagain,
    GCObject* weak);

    vm_shuffle5(vm_shuffle_semicolon, size_t GCthreshold,
    size_t totalbytes,
    int gcgoal,
    int gcstepmul,
    int gcstepsize);

    struct lua_Page* freepages[LUA_SIZECLASSES];
    struct lua_Page* freegcopages[LUA_SIZECLASSES];
    struct lua_Page* allpages; 
    struct lua_Page* allgcopages;
    struct lua_Page* sweepgcopage; 

    size_t memcatbytes[LUA_MEMORY_CATEGORIES]; 

    vm_shuffle5(vm_shuffle_semicolon, struct lua_State* mainthread,
    UpVal uvhead,
    struct Table* mt[LUA_T_COUNT],
    globalstate_ttname(TString*) ttname[LUA_T_COUNT],
    globalstate_tmname(TString*) tmname[TM_N]);

    TValue pseudotemp;

    TValue registry;
    int registryfree;

    struct lua_jmpbuf* errorjmp;

    uint64_t rngstate;
    uint64_t ptrenckey[4];

    lua_Callbacks cb;

    lua_ExecutionCallbacks ecb;

    void (*udatagc[LUA_UTAG_LIMIT])(lua_State*, void*);
    Table* udatamt[LUA_LUTAG_LIMIT];

    TString* lightuserdataname[LUA_LUTAG_LIMIT];

    GCStats gcstats;

#ifdef LUAI_GCMETRICS
    GCMetrics gcmetrics;
#endif
} global_State;
// clang-format on

/*
** `per thread' state
*/
// clang-format off

struct lua_State
{
    CommonHeader;
    uint8_t status;

    uint8_t activememcat;

    bool isactive;
    bool singlestep; 

    vm_shuffle6(vm_shuffle_semicolon, StkId top,
    StkId base,
    luastate_global(global_State*) global,
    CallInfo* ci,
    StkId stack_last,
    StkId stack);

    //stacklast 8
    //top 16
    //stack 24
    //global 32
    //ci 40
    //base 48

    //a5 d a1 d a6 d a3 d a4 d a2

    CallInfo* end_ci;
    CallInfo* base_ci;

    luastate_stacksize(int) stacksize;
    int size_ci;

    unsigned short nCcalls;
    unsigned short baseCcalls;

    int cachedslot;

    vm_shuffle3(vm_shuffle_semicolon, Table* gt,
    UpVal* openupval,
    GCObject* gclist);

    TString* namecall;

    void* userdata;
};
// clang-format on

/*
** Union of all collectible objects
*/
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
    struct Buffer buf;
};

// macros to convert a GCObject into a specific value
#define gco2ts(o) check_exp((o)->gch.tt == LUA_TSTRING, &((o)->ts))
#define gco2u(o) check_exp((o)->gch.tt == LUA_TUSERDATA, &((o)->u))
#define gco2cl(o) check_exp((o)->gch.tt == LUA_TFUNCTION, &((o)->cl))
#define gco2h(o) check_exp((o)->gch.tt == LUA_TTABLE, &((o)->h))
#define gco2p(o) check_exp((o)->gch.tt == LUA_TPROTO, &((o)->p))
#define gco2uv(o) check_exp((o)->gch.tt == LUA_TUPVAL, &((o)->uv))
#define gco2th(o) check_exp((o)->gch.tt == LUA_TTHREAD, &((o)->th))
#define gco2buf(o) check_exp((o)->gch.tt == LUA_TBUFFER, &((o)->buf))

// macro to convert any Lua object into a GCObject
#define obj2gco(v) check_exp(iscollectable(v), cast_to(GCObject*, (v) + 0))

LUAI_FUNC lua_State* luaE_newthread(lua_State* L);
LUAI_FUNC void luaE_freethread(lua_State* L, lua_State* L1, struct lua_Page* page);
