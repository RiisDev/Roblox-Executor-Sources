#include "wraps.hpp"

#include "../Luau/lapi.h"
#include "../Luau/lstate.h"
#include "../Luau/lgc.h"
#include "../Luau/lfunc.h"
#include "../Luau/lualib.h"
#include "closures.hpp"
#include <vector>
#include <memory>
#include "../Roblox.hpp"
#include "../Compiler/include/Compiler.h"
#include "../Utils.hpp"


// wrappedlcl.debugname, wrapped_luacl_t
//std::map< const char*, std::shared_ptr< managers::closures::wraps::wrapped_luacl_t > > wrapped_lua_cls;
std::vector< std::shared_ptr< managers::closures::wraps::wrapped_luacl_t > > wrapped_lua_cls;

int managers::closures::wraps::wrapped_lua_handlerK(lua_State* ls, int status)
{
    if (status) return 0;
    return lua_gettop(ls);
}

int managers::closures::wraps::wrapped_lua_handler(lua_State* ls)
{
    const auto nargs = lua_gettop(ls);
    const auto wrapped_cl = reinterpret_cast<const wrapped_luacl_t*>((const char*)curr_func(ls)->c.debugname);

    // get the lua closure
    lua_getref(ls, wrapped_cl->luacl_ref);
    if (!lua_isfunction(ls, -1))
    {
        luaL_error(ls, ("Fucking hell, the newcclosure failed to get the ref. heres some debug info: ref %i"), wrapped_cl->luacl_ref);
    }
    lua_insert(ls, 1);

    // maintain yieldable invariant (baseCcalls <= nCcalls)
    ls->baseCcalls++;
    const auto res = lua_pcall(ls, nargs, LUA_MULTRET, 0);
    ls->baseCcalls--;

    // yielding means we need to propagate yield; resume will call continuation function later
    if (res == LUA_OK && (ls->status == LUA_YIELD || ls->status == LUA_BREAK))
    {
        return -1; // -1 is a marker for yielding from C
    }
    else if (res >= LUA_ERRRUN && res < LUA_BREAK)
    {
        // throw the error
        lua_error(ls);
    }

    return lua_gettop(ls);
}

int managers::closures::wraps::wrapped_lua_handlerNY(lua_State* ls)
{
    const auto nargs = lua_gettop(ls);
    const auto wrapped_cl = reinterpret_cast<const wrapped_luacl_t*>((const char*)curr_func(ls)->c.debugname);

    // get the lua closure
    lua_getref(ls, wrapped_cl->luacl_ref);
    if (!lua_isfunction(ls, -1))
    {
        luaL_error(ls, ("failed to get closure ref\n %s"), wrapped_cl->luacl_ref);
    }
    lua_insert(ls, 1);

    const auto res = lua_pcall(ls, nargs, LUA_MULTRET, 0);

    if (res >= LUA_ERRRUN && res < LUA_BREAK)
    {
        if (std::strcmp(lua_tolstring(ls, -1, NULL), ("attempt to yield across metamethod/C-call boundary")) == 0)
        {
            return lua_yield(ls, LUA_MULTRET);
        }

        // throw the error
        lua_error(ls);
    }

    return lua_gettop(ls);
}

auto managers::closures::wraps::wrap_closure(lua_State* ls, Closure* cl, bool yields_allowed) -> void
{
    const auto type = get_closure_type(cl);

    // these shouldn't exist :vomit:
    if (type == c_closure || type == marked_c_closure)
    {
        // wrapped C closure
        // we wanna give the closure a separate env
        lua_newtable(ls);
        {
            lua_newtable(ls); // Metatable
            {
                lua_pushvalue(ls, LUA_GLOBALSINDEX);
                lua_setfield(ls, -2, ("__index"));

                lua_setreadonly(ls, -1, true);
            }
            lua_setmetatable(ls, -2);

            setclvalue(ls, ls->top, cl) ls->top++;
            lua_setfield(ls, -2, ("wrappedC"));
        }

        auto bytecode = Utils::compress_bytecode(Luau::compile("return wrappedC(...)"));

        if (Defs::rbx_luavmload((__int64)ls, &bytecode, xorstr_("wrap"), 0) != LUA_OK)
        {
            std::string err = lua_tostring(ls, -1);
            lua_pop(ls, 1);
            luaL_error(ls, ("error while wrapping C Closure to lua: %s"), err);
        }

        Closure* pClosure = const_cast<Closure*>(static_cast<const Closure*>(lua_topointer(ls, -1)));
        ProtoThings::raise_proto(pClosure->l.p);

        mark_closure(clvalue(luaA_toobject(ls, -1)));
    }
    else if (type == l_closure || type == marked_proto)
    {
        // Thanks Table!
        const auto ref = lua_ref(ls, 1);
        const auto wrapped_cl = std::make_shared< wrapped_luacl_t >(wrapped_luacl_t{ ref });

        wrapped_lua_cls.push_back(wrapped_cl);

        // wrapped lua closure
        if (yields_allowed)
        {
            lua_pushcclosurek(ls, wrapped_lua_handler, reinterpret_cast<const char*>(wrapped_cl.get()), 0, wrapped_lua_handlerK);
        }
        else
        {
            lua_pushcclosure(ls, wrapped_lua_handlerNY, reinterpret_cast<const char*>(wrapped_cl.get()), 0);
        }

        const auto wcc = clvalue(luaA_toobject(ls, -1));

        wcc->isC = wrapped_l_closure;
    }
    else
    {
        luaL_error(ls, "%s", ("not implemented"));
    }
}
