#pragma once

#include "../Luau/lua.h"
#include "../Luau/lobject.h"

namespace managers::closures::wraps {
    struct wrapped_luacl_t {
        int luacl_ref;
        // originally supposed to have more info
    };

    // Handler functions, declared for external use
    int wrapped_lua_handlerK(lua_State* ls, int status);
    int wrapped_lua_handler(lua_State* ls);
    int wrapped_lua_handlerNY(lua_State* ls);

    // Wraps a CClosure or LClosure and pushes the result to the stack
    auto wrap_closure(lua_State* ls, Closure* cl, bool yields_allowed) -> void;
}