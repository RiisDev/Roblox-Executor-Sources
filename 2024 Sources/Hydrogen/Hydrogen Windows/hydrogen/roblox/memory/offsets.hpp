#pragma once
#include <cstdint>

#include <Windows.h>

#include "fnhandler.hpp"

namespace roblox
{
	namespace off
	{
		const auto base = reinterpret_cast< std::uint8_t* >( GetModuleHandleA( nullptr ) );

		template< typename R >
		R rebase( const auto addr )
		{
			return R( base + addr );
		}

		const auto luau_execute = rebase< std::uintptr_t >( 0x1499910 );
		const auto luad_rawrunprotected = rebase< std::uintptr_t >( 0x1493D50 );

		const auto nilobject = rebase< std::uintptr_t >( 0x2487FB0 );
		const auto dummynode = rebase< std::uintptr_t >( 0x2487F90 );

		namespace tls
		{
			constexpr auto identity = 0xC8;
		}

		namespace extraspace
		{
			constexpr auto identity = 0x18;
		}

		namespace scheduler
		{
			const auto self = rebase< std::uintptr_t >( 0x3A47574 );

			constexpr auto jobs_begin = 0x134;
			constexpr auto jobs_end = jobs_begin + sizeof( void* );
		}

		namespace script_context
		{
			const auto resume = rebase< std::uintptr_t >( 0x417F50 );

			// offset to ScriptContext from WaitingScriptsJob
			constexpr auto wsj_offset = 304;
			constexpr auto global_state = 300;
		}
	}
}
