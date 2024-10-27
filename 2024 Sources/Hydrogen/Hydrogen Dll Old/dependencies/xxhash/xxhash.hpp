#pragma once

#include <Windows.h>

//**************************************
// CPU Feature Detection
//**************************************
// Little Endian or Big Endian ?
// You can overwrite the #define below if you know your architecture endianess
#if defined(FORCE_NATIVE_FORMAT) && (FORCE_NATIVE_FORMAT==1)
// Force native format. The result will be endian dependant.
#  define XXH_BIG_ENDIAN 0
#elif defined (__GLIBC__)
#  include <endian.h>
#  if (__BYTE_ORDER == __BIG_ENDIAN)
#     define XXH_BIG_ENDIAN 1
#  endif
#elif (defined(__BIG_ENDIAN__) || defined(__BIG_ENDIAN) || defined(_BIG_ENDIAN)) && !(defined(__LITTLE_ENDIAN__) || defined(__LITTLE_ENDIAN) || defined(_LITTLE_ENDIAN))
#  define XXH_BIG_ENDIAN 1
#elif defined(__sparc) || defined(__sparc__) \
   || defined(__ppc__) || defined(_POWER) || defined(__powerpc__) || defined(_ARCH_PPC) || defined(__PPC__) || defined(__PPC) || defined(PPC) || defined(__powerpc__) || defined(__powerpc) || defined(powerpc) \
   || defined(__hpux)  || defined(__hppa) \
   || defined(_MIPSEB) || defined(__s390__)
#  define XXH_BIG_ENDIAN 1
#endif

#if !defined(XXH_BIG_ENDIAN)
// Little Endian assumed. PDP Endian and other very rare endian format are unsupported.
#  define XXH_BIG_ENDIAN 0
#endif


//**************************************
// Compiler-specific Options & Functions
//**************************************
#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

// Note : under GCC, it may sometimes be faster to enable the (2nd) macro definition, instead of using win32 intrinsic
#if defined(_WIN32)
#  define XXH_rotl32(x,r) _rotl(x,r)
#else
#  define XXH_rotl32(x,r) ((x << r) | (x >> (32 - r)))
#endif

#if defined(_MSC_VER)     // Visual Studio
#  define XXH_swap32 _byteswap_ulong
#elif GCC_VERSION >= 403
#  define XXH_swap32 __builtin_bswap32
#else
static inline unsigned int XXH_swap32( unsigned int x ) {
    return  ( ( x << 24 ) & 0xff000000 ) |
        ( ( x << 8 ) & 0x00ff0000 ) |
        ( ( x >> 8 ) & 0x0000ff00 ) |
        ( ( x >> 24 ) & 0x000000ff );
}
#endif



//**************************************
// Constants
//**************************************
#define PRIME32_1   2654435761U
#define PRIME32_2   2246822519U
#define PRIME32_3   3266489917U
#define PRIME32_4    668265263U
#define PRIME32_5    374761393U

#define MODINV32_1  244002641U
#define MODINV32_2 3066638151U
#define MODINV32_3 2828982549U
#define MODINV32_4 2701016015U
#define MODINV32_5 2923004241U

//**************************************
// Macros
//**************************************
#define XXH_LE32(p)  (XXH_BIG_ENDIAN ? XXH_swap32(*(unsigned int*)(p)) : *(unsigned int*)(p))



//****************************
// Simple Hash Functions
//****************************

static unsigned int XXH32( const void* input, int len, unsigned int seed )
{
#if 0
    // Simple version, good for code maintenance, but unfortunately slow for small inputs
    void* state = XXH32_init( seed );
    XXH32_feed( state, input, len );
    return XXH32_result( state );
#else

    const unsigned char* p = ( const unsigned char* ) input;
    const unsigned char* const bEnd = p + len;
    unsigned int h32;

    if ( len >= 16 )
    {
        const unsigned char* const limit = bEnd - 16;
        unsigned int v1 = seed + PRIME32_1 + PRIME32_2;
        unsigned int v2 = seed + PRIME32_2;
        unsigned int v3 = seed + 0;
        unsigned int v4 = seed - PRIME32_1;

        do
        {
            v1 += XXH_LE32( p ) * PRIME32_2; v1 = XXH_rotl32( v1, 13 ); v1 *= PRIME32_1; p += 4;
            v2 += XXH_LE32( p ) * PRIME32_2; v2 = XXH_rotl32( v2, 13 ); v2 *= PRIME32_1; p += 4;
            v3 += XXH_LE32( p ) * PRIME32_2; v3 = XXH_rotl32( v3, 13 ); v3 *= PRIME32_1; p += 4;
            v4 += XXH_LE32( p ) * PRIME32_2; v4 = XXH_rotl32( v4, 13 ); v4 *= PRIME32_1; p += 4;
        } while ( p <= limit );

        h32 = XXH_rotl32( v1, 1 ) + XXH_rotl32( v2, 7 ) + XXH_rotl32( v3, 12 ) + XXH_rotl32( v4, 18 );
    }
    else
    {
        h32 = seed + PRIME32_5;
    }

    h32 += ( unsigned int ) len;

    while ( p <= bEnd - 4 )
    {
        h32 += XXH_LE32( p ) * PRIME32_3;
        h32 = XXH_rotl32( h32, 17 ) * PRIME32_4;
        p += 4;
    }

    while ( p < bEnd )
    {
        h32 += ( *p ) * PRIME32_5;
        h32 = XXH_rotl32( h32, 11 ) * PRIME32_1;
        p++;
    }

    h32 ^= h32 >> 15;
    h32 *= PRIME32_2;
    h32 ^= h32 >> 13;
    h32 *= PRIME32_3;
    h32 ^= h32 >> 16;

    return h32;

#endif
}
