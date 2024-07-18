#pragma once
#include <winternl.h>

inline std::uintptr_t scan( const char* const pattern, const char* const mask, std::uintptr_t start, std::uintptr_t end )
{
	for ( auto at = start; at < end; ++at )
	{
		const auto is_same = [ & ] ( ) -> bool
		{
			for ( auto i = 0u; i < std::strlen( mask ); ++i )
			{
				if ( *reinterpret_cast< std::uint8_t* >( at + i ) != static_cast< std::uint8_t >( pattern [ i ] ) && mask [ i ] != '?' )
					return false;
			}

			return true;
		};

		if ( is_same( ) )
			return at;
	}

	return 0;
}

std::vector< MEMORY_BASIC_INFORMATION > get_allocations( )
{
    std::vector< MEMORY_BASIC_INFORMATION > allocations;

    std::uintptr_t addr = 0;

    MEMORY_BASIC_INFORMATION mbi;

    while ( VirtualQuery( reinterpret_cast< std::uintptr_t* >( addr), &mbi, sizeof( mbi ) ) )
    {
        if ( mbi.State == MEM_COMMIT && mbi.Protect == PAGE_EXECUTE_READ )
            allocations.push_back( mbi );

        addr += mbi.RegionSize;
    }

    return allocations;
}

inline std::uintptr_t find_ac( )
{
    for ( const auto& alloc : get_allocations( ) )
    {
        if ( const auto result = scan( "\x8B\x4D\x30\x85\xC9\x74\x00\x8B\x31", "xxxxxx?xx", reinterpret_cast< std::uintptr_t >( alloc.BaseAddress ), reinterpret_cast< std::uintptr_t >( alloc.BaseAddress ) + alloc.RegionSize ) )
            return result;
    }

    return 0;
}