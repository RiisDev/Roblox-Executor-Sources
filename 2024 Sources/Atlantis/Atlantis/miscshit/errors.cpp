#include "errors.hpp"
#include "../Luau/lobject.h"
#include "../Luau/lstate.h"

#include <map>
#include "../Luau/lualib.h"

// This exists because oxorany messe up func names
std::map< lua_CFunction, std::string > debugnames;

// adds a cfunction that gets handled by this manager
auto managers::errors::add_handled_closure(lua_CFunction cf, std::string_view name) -> void
{
  //  LOG_FUNC_CALL();

    debugnames.insert({ cf, name.data() });
}
/*
// These exist due to some issues with oxorany and luau's macros
auto managers::errors::throw_type(lua_State* ls, int narg, std::string_view expected, bool condition_result) -> void
{
    LOG_FUNC_CALL();

    if ( condition_result ) return;

    // how it should look:
    // invalid type for argument #1 to 'func', expected (number) but got (nil)
    // invalid type for argument #1 to 'func', expected (function or nil) but got (number)
    // invalid type for argument #1 to 'func', expected (<ClickDetector>) but got (<Part>)

    // construct the type we got
    std::string ttypename;
    if (lua_isuserdata(ls, narg))
    {
        const auto tt = rbx::reflection::types::get_lua_type(ls, narg);
        if (tt == PH_O("Instance"))
        {
            const auto classname = rbx::reflection::types::get_class_name(ls, narg);

            // <DataModel>
            ttypename.append(PH_O("<"));
            ttypename.append(classname);
            ttypename.append(PH_O(">"));
        }
    }
    else
    {
        // function
        const auto ntype = lua_type(ls, narg);
        ttypename = lua_typename(ls, ntype);
    }

    luaL_error(ls, PH_O("invalid type for argument #i to '%s', expected (%s) but got (%s)!"),
                           narg, currfuncname(ls), expected.data(), ttypename.c_str());
}
*/
// meant to replace the one in lualib, if there's errors remove that one
// and declare it in lualib.h
const char* currfuncname(lua_State* L)
{
    Closure* cl = L->ci > L->base_ci ? curr_func(L) : nullptr;
    const char* debugname;

    if (cl && cl->isC)
    {
        const auto mapped = debugnames.find(cl->c.f);
        if (mapped == debugnames.end())
        {
            debugname = cl->c.debugname + 0;
        }
        else
        {
            debugname = mapped->second.data();
        }
    }
    else
    {
        debugname = nullptr;
    }

    if (debugname && strcmp(debugname, "__namecall") == 0)
        return L->namecall ? getstr(L->namecall) : nullptr;
    else
        return debugname;
}