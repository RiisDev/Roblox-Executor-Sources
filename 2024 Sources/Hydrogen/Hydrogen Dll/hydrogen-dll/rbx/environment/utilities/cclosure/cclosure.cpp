#include "cclosure.hpp"

#include <lualib.h>

#include <lgc.h>
#include <lfunc.h>
#include <lapi.h>
#include <lstate.h>
#include <lvm.h>

#include <rbx/environment/utilities/storage/storage.hpp>

using c_cclosure
    = rbx::environment::utilities::c_cclosure;
using closure_type_t
    = rbx::environment::utilities::closure_type_t;

closure_type_t c_cclosure::get_closure_type( Closure* c )
{
    if ( !c->isC )
        return closure_type_t::lclosure;
    else if ( c->c.f == handler )
        return closure_type_t::newcclosure;
    else
        return closure_type_t::cclosure;
}

void c_cclosure::clone_function( lua_State* L, int i )
{
    luaL_checktype( L, i, LUA_TFUNCTION );

    const auto orig = clvalue( luaA_toobject( L, i ) );

    const auto clone_c = [ & ] ( ) -> Closure*
    {
        lua_checkstack( L, 1 );

        const auto clone = luaF_newCclosure( L , orig->nupvalues, orig->env );
        clone->c.f = orig->c.f;
        clone->c.cont = orig->c.cont;
        clone->c.debugname = orig->c.debugname;
        for ( int i = 0; i < orig->nupvalues; i++ )
            setobj( L, &clone->c.upvals[ i ], &orig->c.upvals[ i ] );
        setclvalue( L, L->top, clone );
        api_check( L, L->top < L->ci->top );
        L->top++;

        return clone;
    };

    switch ( get_closure_type( orig ) )
    {
        case closure_type_t::cclosure:
        {
            clone_c( );
            return;
        }
        case closure_type_t::newcclosure:
        {
            const auto closure = get_closure( orig );

            if ( closure )
            {
                lua_checkstack( L, 1 );

                lua_pushcclosure( L, handler, nullptr, 0 );

                lua_ref( L, -1 );

                g_newcclosure_cache.add( clvalue( luaA_toobject( L, -1 ) ), *closure );
            }
            else
                luaL_error( L, "newcclosure clone failed\n" );

            return;
        }
        case closure_type_t::lclosure:
        {
            luaD_checkstack( L, 1 );

            lua_clonefunction( L, i );

            const auto closure = clvalue( luaA_toobject( L, -1 ) );
            g_closure_cache.add( closure );

            return;
        }
    }
}


std::optional< Closure* > c_cclosure::get_closure( Closure* closure )
{
    return g_newcclosure_cache.get( closure );
}

std::int32_t rbx::environment::utilities::c_cclosure::handler( lua_State* L )
{
    const auto arg_count = lua_gettop( L );

    const auto closure = g_newcclosure_cache.get( clvalue( L->ci->func ) );

    if ( !closure )
        luaL_error( L, "Internal newcclosure error" );

    setclvalue( L, L->top, *closure );
    L->top++;

    lua_insert( L, 1 );

    if ( lua_pcall( L, arg_count, LUA_MULTRET, 0 ) ) // TODO: error handler
    {
        if ( lua_isstring( L, -1 ) )
        {
            const auto err = lua_tostring( L, -1 );

            if( !std::strcmp( lua_tostring( L, -1 ), "attempt to yield across metamethod/C-call boundary" ) )
                return lua_yield( L, 0 );

            luaL_error( L, err );
        }
        else
            luaL_error( L, "Unknown internal newcclosure error" );
    }

    return lua_gettop( L );
}

void rbx::environment::utilities::c_cclosure::add( Closure* closure, Closure* lclosure )
{
    g_newcclosure_cache.add( closure, lclosure );
}

void rbx::environment::utilities::c_cclosure::convert( Closure* closure, Closure* lclosure )
{
    closure->c.f = handler;

    g_newcclosure_cache.add( closure, lclosure );
}

int c_cclosure::wrap( lua_State* L )
{
    luaL_checktype( L, 1, LUA_TFUNCTION );
   
    lua_ref( L, 1 );

    lua_pushcclosure( L, handler, nullptr, 0 );

    lua_ref( L, -1 );

    g_newcclosure_cache.add( clvalue( luaA_toobject( L, -1 ) ), clvalue( luaA_toobject( L, 1 ) ) );

    return 1;
}