#include "hooks.hpp"

#include "closures.hpp"
#include "../Luau/lapi.h"
#include "../Luau/lualib.h"
#include <unordered_map>
#include "../Luau/lstate.h"
#include "../Luau/lgc.h"

static std::unordered_map< Closure*, Closure* > hooked_funcs = { };

auto managers::hooks::hook_C_with_L(lua_State* ls, Closure* src, Closure* dst, bool nomap) -> return_code
{
    const auto src_type = closures::get_closure_type(src);
    const auto dst_type = closures::get_closure_type(dst);

    if (dst_type > closures::l_closure) return fail;

    if (src_type >= closures::c_closure)
    {
        lua_getglobal(ls, ("newcclosure"));
        {
            setclvalue(ls, ls->top, dst) ls->top++;
            lua_pushboolean(ls, false); // default: no yields
        }
        lua_call(ls, 2, 1);
        if (!lua_isfunction(ls, -1))
        {
            luaL_error(ls, "%s", "failed to create a new C closure");
        }

        auto ncc = clvalue(luaA_toobject(ls, -1));
        lua_pop(ls, 1);

        hook_C_with_C(ls, src, ncc, nomap);

        return success;
    }

    return fail;
}

auto managers::hooks::hook_C_with_C(lua_State* ls, Closure* src, Closure* dst, bool nomap) -> return_code
{
    const auto src_type = closures::get_closure_type(src);
    const auto dst_type = closures::get_closure_type(dst);

    if (!nomap)
    {
        if (!closures::clone_closure(ls, src)) return invalid_closure;
        hooked_funcs[src] = clvalue(luaA_toobject(ls, -1));
    }

    // ( Normal CC or Marked CC ) with ( Normal CC or Marked CC )
    if ((src_type == closures::c_closure || src_type == closures::marked_c_closure) && (dst_type == closures::c_closure || dst_type == closures::marked_c_closure))
    {
        src->c.f = (lua_CFunction)dst->c.f;
        //  src->stacksize = dst->stacksize;
       // src->env = (Table*)dst->env;
        src->c.cont = (lua_Continuation)dst->c.cont;
        //src->memcat = dst->memcat;

        // Clear the upvalues.
        for (int i = 0; i < src->nupvalues; ++i)
        {
            src->c.upvals[i].tt = LUA_TNIL;
        }

        // Now pass those upvalues.
        for (int i = 0; i < dst->nupvalues; i++)
        {
            setobj2n(ls, &src->c.upvals[i], &dst->c.upvals[i])
        }

        src->nupvalues = dst->nupvalues;

        return success;
    }

    // Wrapped Lua CL with Wrapped Lua CL
    if (src_type == closures::wrapped_l_closure && dst_type == closures::wrapped_l_closure)
    {
        src->c.debugname = (const char*)dst->c.debugname;
        return success;
    }

    // ( Normal CC or Marked CC ) with Wrapped Lua CL
    if ((src_type == closures::c_closure || src_type == closures::marked_c_closure) && dst_type == closures::wrapped_l_closure)
    {
        src->c.debugname = (const char*)dst->c.debugname;
        src->c.f = (lua_CFunction)dst->c.f;
        //    src->stacksize = dst->stacksize;
       // src->env = (Table*)dst->env;
        src->c.cont = (lua_Continuation)dst->c.cont;
        //src->memcat = dst->memcat;

        // Clear the upvalues.
        for (int i = 0; i < src->nupvalues; ++i)
        {
            src->c.upvals[i].tt = LUA_TNIL;
        }

        // Now pass those upvalues.
        for (int i = 0; i < dst->nupvalues; i++)
        {
            setobj2n(ls, &src->c.upvals[i], &dst->c.upvals[i])
        }

        src->nupvalues = dst->nupvalues;

        return success;
    }

    // Wrapped Lua CL with ( Normal CC or Marked CC )
    if (src_type == closures::wrapped_l_closure && (dst_type == closures::c_closure || dst_type == closures::marked_c_closure))
    {
        src->c.f = (lua_CFunction)dst->c.f;
        //    src->stacksize = dst->stacksize;
       // src->env = (Table*)dst->env;
        src->c.cont = (lua_Continuation)dst->c.cont;
        src->c.debugname = (const char*)dst->c.debugname;
        //src->memcat = dst->memcat;

        // Clear the upvalues.
        for (int i = 0; i < src->nupvalues; ++i)
        {
            src->c.upvals[i].tt = LUA_TNIL;
        }

        // Now pass those upvalues.
        for (int i = 0; i < dst->nupvalues; i++)
        {
            setobj2n(ls, &src->c.upvals[i], &dst->c.upvals[i])
        }

        src->nupvalues = dst->nupvalues;

        return success;
    }

    return fail;
}

auto managers::hooks::hook_L_with_L(lua_State* ls, Closure* src, Closure* dst, bool nomap) -> return_code
{
    auto src_nups = src->nupvalues;
    auto dst_nups = dst->nupvalues;

    if (dst_nups > src_nups)
    {
        luaL_error(ls, ("new closure has more upvalues than old one ( new: %i, old : %i )"), dst_nups, src_nups);
    }

    if (!nomap)
    {
        if (!closures::clone_closure(ls, src)) return invalid_closure;
        hooked_funcs[src] = clvalue(luaA_toobject(ls, -1));
    }

    // Clear the upvalues.
    for (int i = 0; i < src->nupvalues; ++i)
    {
        src->l.uprefs[i].tt = LUA_TNIL;
    }

    // Now pass those upvalues.
    for (int i = 0; i < dst->nupvalues; ++i)
    {
        setobj2n(ls, &src->l.uprefs[i], &dst->l.uprefs[i])
    }

    src->nupvalues = dst->nupvalues;
    src->stacksize = dst->stacksize;
    src->preload = dst->preload;
    //src->memcat = dst->memcat;
    src->env = (Table*)dst->env;
    src->l.p = (Proto*)dst->l.p;

    return success;
}

auto managers::hooks::hook_L_with_C(lua_State* ls, Closure* src, Closure* dst, bool nomap) -> return_code
{
    const auto src_type = closures::get_closure_type(src);
    const auto dst_type = closures::get_closure_type(dst);

    if (dst_type < closures::c_closure) return fail;

    if (src_type == closures::l_closure || src_type == closures::marked_proto)
    {
        lua_getglobal(ls, ("newlclosure"));
        {
            setclvalue(ls, ls->top, dst) ls->top++;
        }
        lua_call(ls, 1, 1);
        if (!lua_isfunction(ls, -1))
        {
            luaL_error(ls, "%s", ("failed to create new lua closure"));
        }

        auto nlc = clvalue(luaA_toobject(ls, -1));
        lua_pop(ls, 1);

        hook_L_with_L(ls, src, nlc, nomap);

        return success;
    }

    return fail;
}

auto managers::hooks::is_func_hooked(Closure* cl) -> bool
{
    const auto hf_map = hooked_funcs.find(cl);
    if (hf_map == hooked_funcs.end())
    {
        return false;
    }

    return true;
}

auto managers::hooks::restore_function(lua_State* ls, Closure* src) -> return_code
{
    const auto hf_map = hooked_funcs.find(src);
    if (hf_map == hooked_funcs.end())
    {
        return fail;
    }

    const auto orig_copy = hf_map->second;

    const auto src_type = closures::get_closure_type(src);
    const auto orig_type = closures::get_closure_type(orig_copy);

    return_code code = fail;
    if (src_type >= closures::c_closure && orig_type >= closures::c_closure)
    {
        code = hook_C_with_C(ls, src, orig_copy, true);
    }
    else if ((src_type == closures::l_closure || src_type == closures::marked_proto) && (orig_type == closures::l_closure || orig_type == closures::marked_proto))
    {
        code = hook_L_with_L(ls, src, orig_copy, true);
    }
    else if (src_type >= closures::c_closure && orig_type == closures::l_closure)
    {
        code = hook_C_with_L(ls, src, orig_copy, true);
    }
    else if ((src_type == closures::l_closure || src_type == closures::marked_proto) && orig_type >= closures::c_closure)
    {
        code = hook_L_with_C(ls, src, orig_copy, true);
    }

    if (code == success)
    {
        hooked_funcs.erase(hf_map);
    }
    return code;
}