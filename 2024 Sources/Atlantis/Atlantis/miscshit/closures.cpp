#include "closures.hpp"
#include "../Luau/lapi.h"
#include "../Luau/lstate.h"
#include "../Luau/lgc.h"
#include "../Luau/lfunc.h"
#include "../Luau/lualib.h"
#include "wraps.hpp"
#include "errors.hpp"
#include "../Roblox.hpp"

auto managers::closures::pushcclosure(lua_State* ls, lua_CFunction f, const char* name, int nups, lua_Continuation k) -> Closure*
{
    lua_pushcclosurek(ls, f, name, nups, k);
    const auto cl = clvalue(luaA_toobject(ls, -1));

    mark_closure(cl);

    errors::add_handled_closure(f, name);


    return cl;
    //LOGD("managers::closures::pushcclosure(%p, %p, %s, %i, %p)", ls, f, name, nups, k);
}

auto mark_protos(Proto* p) -> void
{
    p->linedefined = managers::closures::marked_proto;

    for (auto i = 0; i < p->sizep; i++)
    {
        mark_protos(p->p[i]);
    }
}

auto managers::closures::mark_closure(Closure* cl) -> void
{
    if (cl->isC)
    {
        cl->isC = marked_c_closure;
    }
    else
    {
        // Mark the protos for this closure
        const auto p = cl->l.p;

        mark_protos(p);
    }
}

auto managers::closures::clone_closure(lua_State* ls, Closure* src) -> bool
{
    const auto type = get_closure_type(src);

    if (type == wrapped_l_closure)
    {
        // wrapped lua closure
        if (src->c.f == wraps::wrapped_lua_handler)
        {
            lua_pushcclosurek(ls, wraps::wrapped_lua_handler, src->c.debugname, 0, wraps::wrapped_lua_handlerK);
        }
        else
        {
            lua_pushcclosure(ls, wraps::wrapped_lua_handlerNY, src->c.debugname, 0);
        }

        const auto wcc = clvalue(luaA_toobject(ls, -1));
        wcc->isC = wrapped_l_closure;

        return true;
    }

    if (type == c_closure)
    {
        luaC_checkGC(ls)

            Closure* clone = luaF_newCclosure(ls, src->nupvalues, src->env);
        clone->c.debugname = (const char*)src->c.debugname;

        for (int i = 0; i < src->nupvalues; i++)
        {
            setobj2n(ls, &clone->c.upvals[i], &src->c.upvals[i])
        }

        clone->c.cont = (lua_Continuation)src->c.cont;
        clone->c.f = (lua_CFunction)src->c.f;
        clone->env = (Table*)src->env;

        luaC_threadbarrier(ls) setclvalue(ls, ls->top, clone) ls->top++;
        return true;
    }

    if (type == marked_c_closure)
    {
        pushcclosure(ls, src->c.f, src->c.debugname, 0, nullptr);
        return true;
    }

    if (type == l_closure || type == marked_proto)
    {
        setclvalue(ls, ls->top, src) ls->top++;

        lua_clonefunction(ls, -1);
        lua_remove(ls, -2);

        const auto clone = clvalue(luaA_toobject(ls, -1));
        clone->env = (Table*)src->env;

        ProtoThings::raise_proto(clone->l.p);

        if (type == marked_proto)
        {
            mark_protos(clone->l.p);
        }

        return true;
    }

    return false;
}

auto managers::closures::get_closure_type(Closure* cl) -> closure_types
{
    if (cl->isC)
    {
        if (cl->isC == marked_c_closure)
        {
            return marked_c_closure;
        }

        if (cl->isC == wrapped_l_closure)
        {
            return wrapped_l_closure;
        }

        return c_closure;
    }
    else if (!cl->isC)
    {
        if (cl->l.p->linedefined == marked_proto)
        {
            return marked_proto;
        }

        return l_closure;
    }

    // unreachable code
    return unidentified;
}

auto managers::closures::is_marked_closure(Closure* cl) -> bool
{
    const auto type = get_closure_type(cl);

    if (type != l_closure && type != c_closure && type != unidentified)
    {
        return true;
    }

    return false;
}