#include "memcheck.hpp"

#include <DbgHelp.h>
#include <utilities/io.hpp>
#include <managers/roblox/structures/task_scheduler/task_scheduler.hpp>
#include <managers/managers.hpp>

const auto core_hasher_start = base + 0x2f26a0;
const auto core_hasher_end = core_hasher_start + 0x51;
std::uintptr_t cloned_text;

std::uintptr_t address_reading;
__declspec( naked ) void core_hasher_hook( )
{
	/*
	*(_DWORD *)(ebp - 16) = 0x1594FE2D * __ROL4__(*(_DWORD *)(ebp - 16) + 0x344B5409 * (ebx + 4 - *(_DWORD *)(ebx + 4)), 0x11);
	*(_DWORD *)(ebp - 12) = 0xCBB4ABF7 * __ROL4__(*(_DWORD *)(ebp - 12) + 0x1594FE2D * ((ebx + 8) ^ *(_DWORD *)(ebx + 8)), 0xD);
	*/

	__asm
	{
		mov address_reading, ebx;

		sub esp, 0x1000;
		pushad;
	}

	if ( address_reading >= text.start && address_reading <= text.end )
		address_reading = cloned_text + ( address_reading - text.start );

	__asm
	{
		popad;

		push edi
		mov edi, address_reading
		mov eax, [ edi ]									// read
		pop edi

		add eax, ebx
		imul eax, eax, 0x1594FE2D
		add eax, edi
		rol eax, 0x13
		imul edi, eax, 0xCBB4ABF7
		lea eax, [ ebx + 0x04 ]

		push edi
		mov edi, address_reading
		sub eax, [ edi + 0x04 ]								// read
		pop edi

		add ebx, 0x08
		imul eax, eax, 0x344B5409
		add eax, [ ebp - 0x10 ]
		rol eax, 0x11
		imul eax, eax, 0x1594FE2D
		mov [ ebp - 0x10 ], eax

		push edi
		mov edi, address_reading
		add edi, 0x08
		mov eax, [ edi ]									// read
		pop edi

		xor eax, ebx
		add ebx, 0x04
		imul eax, eax, 0x1594FE2D
		add eax, [ ebp - 0x0C ]
		rol eax, 0x0D
		imul eax, eax, 0xCBB4ABF7
		mov [ ebp - 0x0C ], eax

		push edi
		mov edi, address_reading
		add edi, 0x0C
		mov eax, [ edi ]									// read
		pop edi

		add esp, 0x1000;
		jmp core_hasher_end;
	}
}

void roblox::memcheck::initiate( )
{
	const auto hook = [ ]( const std::uintptr_t from, const void* const to )
	{
		DWORD old_protection;
		VirtualProtect( reinterpret_cast< void* >( from ), 0x5, PAGE_EXECUTE_READWRITE, &old_protection );

		*reinterpret_cast< std::uint8_t* >( from ) = 0xE9;
		*reinterpret_cast< std::uintptr_t* >( from + 0x1 ) = reinterpret_cast< std::uintptr_t >( to ) - from - 0x5;

		VirtualProtect( reinterpret_cast< void* >( from ), 0x5, old_protection, &old_protection );
	};

	cloned_text = reinterpret_cast< std::uintptr_t >( std::memcpy( VirtualAlloc( nullptr, text.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE ), reinterpret_cast< void* >( text.start ), text.size ) );
	utilities::io::log( utilities::io::log_options_t::debug, "Cloned Text: 0x%p", cloned_text );

	const auto us14116 = reinterpret_cast< std::uintptr_t >( roblox::structures::task_scheduler_t::get( )->get_job_by_name( "US14116" ) );
	utilities::io::log( utilities::io::log_options_t::debug, "US14116: 0x%p", us14116 );

	if ( us14116 )
	{
		const auto client_replicator = *reinterpret_cast< std::uintptr_t* >( us14116 + 0x128 );
		utilities::io::log( utilities::io::log_options_t::debug, "Client Replicator: 0x%p", client_replicator );

		//"55 8B EC 51 53 56 8B 75 08 57 8B F9 6A" xref
		auto& dynamic_checker_key = *reinterpret_cast< std::uint32_t* >( client_replicator + 0x2F24);
		utilities::io::log( utilities::io::log_options_t::debug, "Dynamic Checker Key: 0x%X", dynamic_checker_key );

		dynamic_checker_key = 0;
		utilities::io::log( utilities::io::log_options_t::debug, "Disabled Dynamic Checker: 0x%X", dynamic_checker_key );

		hook( core_hasher_start, &core_hasher_hook );
		utilities::io::log( utilities::io::log_options_t::debug, "Hooked Core Hasher: 0x%p", core_hasher_start );
	}
}