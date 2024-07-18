#include <thread>
#include <Windows.h>

#include "structs/structs.hpp"

#include "utilities/scan.hpp"
#include "utilities/io.hpp"
#include "utilities/hook.hpp"

void __stdcall hook_stub( std::uintptr_t inst )
{
	const auto rule_set_vector_start = *reinterpret_cast< std::uintptr_t* >( inst + 0x6C );
	const auto rule_set_vector_end = *reinterpret_cast< std::uintptr_t* >( inst + 0x6C + sizeof( std::uintptr_t ) );

	const auto size = rule_set_vector_end - rule_set_vector_start;

	const auto length = size / sizeof( rule_t );

	const auto dll_info_start = *reinterpret_cast< std::uintptr_t* >( inst + 0x18 );

	const auto dll_info = reinterpret_cast< dll_info_t* >( dll_info_start + 0x21C );
	const auto allocation_info = reinterpret_cast< allocation_info_t* >( dll_info_start + 0x234 );

	if ( dll_info->base )
		utilities::io::log( "[RACL] -> Analyzing DLL: %ls | %p | %x\n\n", dll_info->path, dll_info->base, dll_info->size );
	else
		utilities::io::log( "[RACL] -> Analyzing Allocation: %p | %x\n\n", allocation_info->base, allocation_info->size );

	for ( auto i = 0u; i < length; ++i )
	{
		const auto rule_struct = reinterpret_cast< rule_t* >( rule_set_vector_start + i * sizeof( rule_t ) );

		utilities::io::log( "[RACL] -> Rule Set: %s\n", rule_struct->rule_name.c_str( ) );
	}

	utilities::io::log( "\n" );
}

std::uintptr_t old = 0;
__declspec( naked ) void stub( )
{
	std::uintptr_t mf_ecx;

	__asm 
	{
		push ecx
		mov ecx, [ebp + 0x30]
		mov mf_ecx, ecx
		pop ecx

        pushad
	}

	hook_stub( mf_ecx );

	__asm
	{
		popad
		jmp old
	}
}

void entry( )
{
	utilities::io::initiate( "RACL - gogo1000, ozzy, 0x90, CompiledCode" );
    
	if ( const auto ac = find_ac( ) )
	{
		utilities::io::log( "[RACL] -> scan_module_for_known_signatures:	0x%X\n\n", ac );

		old = tramp_hook( ac, reinterpret_cast< std::uintptr_t >( &stub ), 7 );	

		// lazy fix:
		std::uint8_t jump[] = { 0xB8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
		*reinterpret_cast< std::uintptr_t* >( jump + 1 ) = ac + 0x18;
		
		std::memcpy( reinterpret_cast< void* >( old + 0x18 ), jump, sizeof( jump ) );
	}
}

bool __stdcall DllMain( void*, DWORD reason, void* )
{
	if ( reason == DLL_PROCESS_ATTACH )
		std::thread{ entry }.detach( );

	return true;
}
