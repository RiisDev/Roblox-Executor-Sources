#pragma once
#include <cstdint>
#include <emmintrin.h>
#include <Windows.h>
#include <iostream>

#include <managers/managers.hpp>

class LuaSecureDouble
{
private:
    double storage;

public:
    operator const double( ) const
    {
        const auto xorc = *reinterpret_cast< double* >( managers::mgrs.offset_mgr.roblox.xorconst );
        __m128d xmmKey = _mm_load_pd( &xorc );
        __m128d xmmData = _mm_load_sd( &storage );
        __m128d xmmResult = _mm_xor_pd( xmmData, xmmKey );
        return _mm_cvtsd_f64( xmmResult );
    }

    void operator=( const double& value )
    {
        const auto xorc = *reinterpret_cast< double* >( managers::mgrs.offset_mgr.roblox.xorconst );

        __m128d xmmKey = _mm_load_pd( &xorc );
        __m128d xmmData = _mm_load_sd( &value );
        __m128d xmmResult = _mm_xor_pd( xmmData, xmmKey );
        storage = _mm_cvtsd_f64( xmmResult );
    }
};
