#pragma once

#include "../Luau/lua.h"
#include <string>

namespace managers::errors {
    // adds a cfunction that gets handled by this manager
    auto add_handled_closure(lua_CFunction cf, std::string_view name) -> void;
}