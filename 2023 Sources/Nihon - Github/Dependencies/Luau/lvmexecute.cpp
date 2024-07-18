#include "lvm.h"

#include "lstate.h"
#include "ltable.h"
#include "lfunc.h"
#include "lstring.h"
#include "lgc.h"
#include "lmem.h"
#include "ldebug.h"
#include "ldo.h"
#include "lbuiltins.h"
#include "lnumutils.h"
#include "lbytecode.h"

#include <string.h>

LUAU_NOINLINE void luau_callhook(lua_State* L, lua_Hook hook, void* userdata)
{
    ptrdiff_t base = savestack(L, L->base);
    ptrdiff_t top = savestack(L, L->top);
    ptrdiff_t ci_top = savestack(L, L->ci->top);
    int status = L->status;

    // if the hook is called externally on a paused thread, we need to make sure the paused thread can emit Lua calls
    if (status == LUA_YIELD || status == LUA_BREAK)
    {
        L->status = 0;
        L->base = L->ci->base;
    }

    luaD_checkstack(L, LUA_MINSTACK); // ensure minimum stack size
    L->ci->top = L->top + LUA_MINSTACK;
    LUAU_ASSERT(L->ci->top <= L->stack_last);

    // note: the pc expectations of the hook are matching the general "pc points to next instruction"
    // however, for the hook to be able to continue execution from the same point, this is called with savedpc at the *current* instruction
    if (L->ci->savedpc)
        L->ci->savedpc++;

    Closure* cl = clvalue(L->ci->func);

    lua_Debug ar;
    ar.currentline = cl->isC ? -1 : luaG_getline(cl->l.p, pcRel(L->ci->savedpc, cl->l.p));
    ar.userdata = userdata;

    hook(L, &ar);

    if (L->ci->savedpc)
        L->ci->savedpc--;

    L->ci->top = restorestack(L, ci_top);
    L->top = restorestack(L, top);

    // note that we only restore the paused state if the hook hasn't yielded by itself
    if (status == LUA_YIELD && L->status != LUA_YIELD)
    {
        L->status = LUA_YIELD;
        L->base = restorestack(L, base);
    }
    else if (status == LUA_BREAK)
    {
        LUAU_ASSERT(L->status != LUA_BREAK); // hook shouldn't break again

        L->status = LUA_BREAK;
        L->base = restorestack(L, base);
    }
}

int luau_precall(lua_State* L, StkId func, int nresults)
{
    if (!ttisfunction(func))
    {
        luaV_tryfuncTM(L, func);
        // L->top is incremented by tryfuncTM
    }

    Closure* ccl = clvalue(func);

    CallInfo* ci = incr_ci(L);
    ci->func = func;
    ci->base = func + 1;
    ci->top = L->top + ccl->stacksize;
    ci->savedpc = NULL;
    ci->flags = 0;
    ci->nresults = nresults;

    L->base = ci->base;
    // Note: L->top is assigned externally

    luaD_checkstack(L, ccl->stacksize);
    LUAU_ASSERT(ci->top <= L->stack_last);

    if (!ccl->isC)
    {
        // fill unused parameters with nil
        StkId argi = L->top;
        StkId argend = L->base + ccl->l.p->numparams;
        while (argi < argend)
            setnilvalue(argi++); // complete missing arguments
        L->top = ccl->l.p->is_vararg ? argi : ci->top;

        L->ci->savedpc = ccl->l.p->code;

        return PCRLUA;
    }
    else
    {
        lua_CFunction func = ccl->c.f;
        int n = func(L);

        // yield
        if (n < 0)
            return PCRYIELD;

        // ci is our callinfo, cip is our parent
        CallInfo* ci = L->ci;
        CallInfo* cip = ci - 1;

        // copy return values into parent stack (but only up to nresults!), fill the rest with nil
        // TODO: it might be worthwhile to handle the case when nresults==b explicitly?
        StkId res = ci->func;
        StkId vali = L->top - n;
        StkId valend = L->top;

        int i;
        for (i = nresults; i != 0 && vali < valend; i--)
            setobj2s(L, res++, vali++);
        while (i-- > 0)
            setnilvalue(res++);

        // pop the stack frame
        L->ci = cip;
        L->base = cip->base;
        L->top = res;

        return PCRC;
    }
}

void luau_poscall(lua_State* L, StkId first)
{
    // finish interrupted execution of `OP_CALL'
    // ci is our callinfo, cip is our parent
    CallInfo* ci = L->ci;
    CallInfo* cip = ci - 1;

    // copy return values into parent stack (but only up to nresults!), fill the rest with nil
    // TODO: it might be worthwhile to handle the case when nresults==b explicitly?
    StkId res = ci->func;
    StkId vali = first;
    StkId valend = L->top;

    int i;
    for (i = ci->nresults; i != 0 && vali < valend; i--)
        setobj2s(L, res++, vali++);
    while (i-- > 0)
        setnilvalue(res++);

    // pop the stack frame
    L->ci = cip;
    L->base = cip->base;
    L->top = (ci->nresults == LUA_MULTRET) ? res : cip->top;
}