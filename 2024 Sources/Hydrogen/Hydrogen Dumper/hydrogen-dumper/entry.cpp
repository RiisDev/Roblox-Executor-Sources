#include <cstdint>
#include <cstdio>
#include "dumper/shuffle/shuffle.hpp"
#include "util/util.hpp"

bool __stdcall DllMain( void*, const std::uint32_t reason, void* )
{
	if ( reason == 1 )
	{
		util::open_console( "[hydrogen] -> dumper" );

		dumper::g_shuffle.dump( );
	}

	return true;
}