#pragma once

#include "../Luau/lua.h"
#include "../Luau/lobject.h"

namespace managers::closures {
    enum closure_types {
        unidentified = -2,
        marked_proto = -1,

        // These 2 must be specifically 0 and 1 due to luau
        l_closure = 0, // 0
        c_closure = 1, // 1

        marked_c_closure = 2, // 2
        wrapped_l_closure = 3, // 3 // LClosures -> CClosures
    };

    // Push a marked closure
    auto pushcclosure(lua_State* ls, lua_CFunction f, const char* name, int nups, lua_Continuation k) -> Closure*;

    // Mark a closure as ours
    auto mark_closure(Closure* cl) -> void;

    // Clone 'cl' and pushes it to the stack, returns false if it failed
    auto clone_closure(lua_State* ls, Closure* src) -> bool;

    // Get closure type
    auto get_closure_type(Closure* cl) -> closure_types;

    // Returns if it is marked as ours.
    auto is_marked_closure(Closure* cl) -> bool;
}