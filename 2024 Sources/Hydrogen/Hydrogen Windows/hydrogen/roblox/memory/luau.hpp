#pragma once

#include <cstdint>
#include <emmintrin.h>
#include <Windows.h>
#include <iostream>

#include <type_traits>

#include "offsets.hpp"

#define LUAVM_SHUFFLE_COMMA ,
#define LUAVM_SHUFFLE_SPLIT ;

//shuffle5 "charpattern" -> pushlstring -> luaC_step
//shuffle8 "charpattern" -> pushlstring -> luaC_step -> gcstep -> sweepgco -> freeobj
//shuffle8 linedefined linegaplog2 "=[C]" 
//shuffle6 invalid argument #%d to '%s' (%s)

#define LUAVM_SHUFFLE2(sep, a0, a1) a0 sep a1 
#define LUAVM_SHUFFLE3(sep, a0, a1, a2) a2 sep a0 sep a1 
#define LUAVM_SHUFFLE4(sep, a0, a1, a2, a3) a2 sep a1 sep a0 sep a3
#define LUAVM_SHUFFLE5(sep, a0, a1, a2, a3, a4) a2 sep a4 sep a3 sep a1 sep a0// // 
#define LUAVM_SHUFFLE6(sep, a0, a1, a2, a3, a4, a5) a1 sep a5 sep a3 sep a0 sep a4 sep a2
#define LUAVM_SHUFFLE7(sep, a0, a1, a2, a3, a4, a5, a6) a6 sep a3 sep a1 sep a4 sep a5 sep a0 sep a2
#define LUAVM_SHUFFLE8(sep, a0, a1, a2, a3, a4, a5, a6, a7) a5 sep a3 sep a6 sep a1 sep a7 sep a0 sep a4 sep a2 
#define LUAVM_SHUFFLE9(sep, a0, a1, a2, a3, a4, a5, a6, a7, a8) a7 sep a1 sep a6 sep a8 sep a3 sep a4 sep a0 sep a5 sep a2 

#define sc_globalstate vmvalue4_t

#define table_array vmvalue1_t//
#define table_metatable vmvalue1_t //
#define table_node vmvalue1_t //
#define tstring_hash vmvalue2_t//
#define proto_debugname vmvalue2_t
#define gs_ttname vmvalue2_t
#define ls_stacksize vmvalue2_t //
#define gs_tmname vmvalue2_t  //
#define proto_source vmvalue3_t//
#define proto_member vmvalue3_t//
#define cclosure_cont vmvalue3_t
#define dbginsn vmvalue1_t 
#define ls_globalstate vmvalue2_t//
#define closure_debugname vmvalue4_t
#define tstring_len vmvalue1_t//
#define lclosure_p vmvalue1_t 
#define cclosure_f vmvalue1_t
#define userdata_metatable vmvalue1_t//

#define d_addr roblox::off::dummynode
#define nilobject_addr roblox::off::nilobject//

/*
vmvalue1_t = add
vmvalue2_t = inverse_sub
vmvalue3_t = xor
vmvalue4_t = sub
*/

template< typename t >
struct vmvalue1_t
{
    operator const t( ) const
    {
        return ( t ) ( ( ( uintptr_t ) ( value ) -( uintptr_t ) ( this ) ) );
    }

    void operator=( const vmvalue1_t& val )
    {
        value = ( t ) ( ( uintptr_t ) ( this ) + ( uintptr_t ) ( val.operator const t( ) ) );
    }


    void operator=( const t& val )
    {
        value = ( t ) ( ( uintptr_t ) ( this ) + ( uintptr_t ) ( val ) );
    }

    const t operator->( ) const
    {
        return operator const t( );
    }

private:
    t value;
};

template< typename t >
struct vmvalue2_t//inversesub
{
    operator const t( ) const
    {
        return ( t ) ( ( uintptr_t ) ( this ) - ( ( uintptr_t ) ( value ) ) );
    }

    void operator=( const vmvalue2_t& val )
    {
        value = ( t ) ( ( uintptr_t ) ( this ) - ( uintptr_t ) ( val.operator const t( ) ) );
    }

    void operator=( const t& val )
    {
        value = ( t ) ( ( uintptr_t ) ( this ) - ( uintptr_t ) ( val ) );
    }

    const t operator->( ) const
    {
        return operator const t( );
    }

private:
    t value;
};

template< typename t >
struct vmvalue3_t
{
    operator const t( ) const
    {
        return ( t ) ( ( uintptr_t ) ( this ) ^ ( ( uintptr_t ) ( value ) ) );
    }

    void operator=( const vmvalue3_t& val )
    {
        value = ( t ) ( ( uintptr_t ) ( val.operator const t( ) ) ^ ( uintptr_t ) ( this ) );
    }

    void operator=( const t& val )
    {
        value = ( t ) ( ( uintptr_t ) ( val ) ^ ( uintptr_t ) ( this ) );
    }

    const t operator->( ) const
    {
        return operator const t( );
    }

private:
    t value;
};

template< typename t >
struct vmvalue4_t //sub
{
    operator const t( ) const
    {
        return ( t ) ( ( uintptr_t ) ( this ) + ( uintptr_t ) ( value ) );
    }

    void operator=( const vmvalue4_t& val )
    {
        value = ( t ) ( ( uintptr_t ) ( val.operator const t( ) ) - ( uintptr_t ) ( this ) );
    }

    void operator=( const t& val )
    {
        value = ( t ) ( ( uintptr_t ) ( val ) -( uintptr_t ) ( this ) );
    }

    const t operator->( ) const
    {
        return operator const t( );
    }

private:
    t value;
};
