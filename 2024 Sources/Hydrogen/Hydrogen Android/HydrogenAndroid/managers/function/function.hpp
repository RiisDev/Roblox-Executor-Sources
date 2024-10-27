#pragma once

#include <dependencies/common_includes.hpp>

#include <managers/memory/memory.hpp>
#include <managers/roblox/offsets.hpp>

#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/luacode.h>
#include <Luau/lualib.h>
#include <Luau/lfunc.h>

#include <Luau/Luau/BytecodeBuilder.h>
#include <Luau/Luau/Compiler.h>
#include <dependencies/imgui/imgui.h>



#define DEF_LUAU_FUNC(luau_func) \
    template< typename ... args_t > \
    std::invoke_result_t<decltype(&luau_func), args_t...> luau_func(args_t... args) \
    { \
        static const auto func = reinterpret_cast<decltype(&::luau_func)>(managers::memory_mgr.rebase(offsets::luau_func)); \
        return func(args...); \
    } \

#define DEF_FUNC(name, def) \
    template< typename ... args_t > \
    std::invoke_result_t<def, args_t...> name(args_t... args) \
    { \
        static const auto func = reinterpret_cast<def>(managers::memory_mgr.rebase(offsets::name)); \
        return func(args...); \
    } \

namespace managers
{
    class function_manager_t
    {
    public:
        DEF_LUAU_FUNC( luau_load )

        DEF_FUNC( script_context_resume, int( * )( void*, void*, int, int, int ) )
        DEF_FUNC( get_thread_identity, int* ( * )( ) )
        DEF_FUNC( push_instance, void( * )( lua_State*, void* ) )
        DEF_FUNC( touch_start, void( * )( std::uintptr_t, std::uintptr_t ) )
        DEF_FUNC( touch_end, void( * )( std::uintptr_t, std::uintptr_t ) )
        DEF_FUNC( fire_click_detector, void( * )( std::uintptr_t, float, std::uintptr_t ) )
        DEF_FUNC( fire_proximity_prompt, void( * )( std::uintptr_t ) )
        DEF_FUNC( set_fflag, bool( * )( std::string*, std::string*, int, bool ) )
        DEF_FUNC( get_fflag, bool( * )( std::string*, std::string*, bool ) )

        TValue* index2addr(lua_State* L, int idx)
        {
            if (idx > 0)
            {
                TValue* o = L->base + (idx - 1);
                api_check(L, idx <= L->ci->top - L->base);
                if (o >= L->top)
                    return cast_to(TValue*, luaO_nilobject);
                else
                    return o;
            }
            else if (idx > LUA_REGISTRYINDEX)
            {
                api_check(L, idx != 0 && -idx <= L->top - L->base);
                return L->top + idx;
            }
            else
            {
                return pseudo2addr(L, idx);
            }
        }

        void lua_pushcclosurek(lua_State* L, lua_CFunction fn, const char* debugname, int nup, lua_Continuation cont);
    };

    extern function_manager_t function_mgr;
}

