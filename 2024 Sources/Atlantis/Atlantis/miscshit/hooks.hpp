#pragma once

#include "../Luau/lua.h"
#include "../Luau/lobject.h"
#include <tuple>

namespace managers::hooks {
    enum return_code {
        success = 0,
        fail = 1,
        invalid_closure = 2
    };

    auto hook_C_with_L(lua_State* ls, Closure* src, Closure* dst, bool nomap = false) -> return_code;
    auto hook_C_with_C(lua_State* ls, Closure* src, Closure* dst, bool nomap = false) -> return_code;

    auto hook_L_with_L(lua_State* ls, Closure* src, Closure* dst, bool nomap = false) -> return_code;
    auto hook_L_with_C(lua_State* ls, Closure* src, Closure* dst, bool nomap = false) -> return_code;

    auto is_func_hooked(Closure* cl) -> bool;
    auto restore_function(lua_State* ls, Closure* src) -> return_code;
}