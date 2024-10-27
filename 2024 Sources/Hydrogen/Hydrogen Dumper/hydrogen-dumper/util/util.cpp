#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include "../mem/mem.hpp"
#include "util.hpp"

void util::open_console( const char* const name )
{
	mem::c_writeable free_console( &FreeConsole );
	*reinterpret_cast< std::uint8_t* >( &FreeConsole ) = 0xC3;

	AllocConsole( );
	std::freopen( "conin$",  "r", stdin );
	std::freopen( "conout$", "w", stdout );
	std::freopen( "conout$", "w", stderr );

	SetConsoleTitleA( name );
}