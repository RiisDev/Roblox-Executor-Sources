#include <Windows.h>
#include <cstdint>
#include <filesystem>
#include <minhook/minhook.hpp>
#include "util/util.hpp"
#include "rbx/ac/syscall/syscall.hpp"
#include "rbx/ac/integrity/integrity.hpp"
#include "rbx/ac/boundary/boundary.hpp"
#include "dll/mapper/mapper.hpp"
#include "daemon/daemon.hpp"

__declspec( dllexport ) 
	bool __stdcall DllMain
		( void*, const std::uint32_t reason, void* )
{
	if ( reason == DLL_PROCESS_ATTACH )
	{
		util::open_console( );
		MH_Initialize( );

		if ( !daemon::c_daemon::is_daemon( ) )
		{
			util::logger( )->info( "main roblox process" );

			rbx::ac::g_syscall.init( );
			rbx::ac::g_integrity.init( );
			rbx::ac::g_boundary.init( );

			dll::g_mapper.init( );
		}
		else
		{
			util::logger( )->info( "daemon process" );

			daemon::g_daemon.init( );
		}

		MH_EnableHook( MH_ALL_HOOKS );
	}

	return true;
}
