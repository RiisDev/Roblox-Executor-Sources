#include <thread>
#include <Windows.h>
#include <Psapi.h>

#include <DbgHelp.h>
#pragma comment(lib, "DbgHelp.lib")

#include "structs/structs.hpp"
#include "utilities/scan.hpp"
#include "utilities/io.hpp"
#include "utilities/hook.hpp"
#include "utilities/trust.hpp"

void __stdcall detect(std::uintptr_t mod)
{
	const auto get_roblox_handle = [ ] ( ) -> HANDLE
	{
		DWORD proc_id;
		GetWindowThreadProcessId( FindWindowA( nullptr, "Roblox" ), &proc_id );
		
		return OpenProcess( PROCESS_ALL_ACCESS, FALSE, proc_id );
		
	}; 	static auto roblox_handle = get_roblox_handle( );
	
	const auto scan = reinterpret_cast< scan_container_t* >( mod );

	if ( scan->status == scan_container_t::status_t::queued )
	{
		const auto to_copy_sz = min( 0x1000, scan->size );

		std::uint8_t* buff = new std::uint8_t [ to_copy_sz ];
		ReadProcessMemory( roblox_handle, reinterpret_cast< void* >( scan->address ), buff, to_copy_sz, nullptr );

		if ( const auto nt_header = ImageNtHeader( buff ) )
		{
			if ( nt_header->Signature == 0x4550 )
			{
				if ( !is_signed( roblox_handle, scan->address ) )
				{
					utilities::io::log( "[RACBW] -> Setting unsigned module status to whitelisted\n\n" );

					scan->status = scan_container_t::status_t::whitelisted;
				}
			}
		}

		delete [ ] buff;
	}
	
	utilities::io::log( 
		"[RACBW] -> status: %i	|	address: 0x%X	|	size: 0x%X\n\n", 
		scan->status, 
		scan->address, 
		scan->size 
	);
}

std::uintptr_t old = 0;
__declspec( naked ) void stub( )
{
	std::uintptr_t mf_edi;

	__asm 
	{
		mov mf_edi, edi
        pushad
	}

    detect( mf_edi );

	__asm
	{
		popad
		jmp old
	}
}

void entry( )
{
	utilities::io::initiate( "RACBW - gogo1000, 0x90, iivillian, ozzy" );
    
	if ( const auto ac = find_ac( ) )
	{
		utilities::io::log( "[RACBW] -> add_to_map:	0x%X\n\n", ac );

		old = tramp_hook( ac, reinterpret_cast< std::uintptr_t >( &stub ), 6 );
	}
}

bool __stdcall DllMain( void*, DWORD reason, void* )
{
	if ( reason == DLL_PROCESS_ATTACH )
		std::thread{ entry }.detach( );

	return true;
}