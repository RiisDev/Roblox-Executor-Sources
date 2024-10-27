#pragma once
#include <udlib/udlib.hpp>

namespace roblox
{
	const auto text = ud::segment_t{ ".text" };
	const auto rdata = ud::segment_t{ ".rdata" };
	const auto vmpx = ud::segment_t{ ".vmpx" };
	const auto base = reinterpret_cast< std::uintptr_t >( GetModuleHandleA( nullptr ) );

	_declspec( naked ) static void stub( ) { __asm ret }

	enum class convention_t
	{
		cdecl_t,
		fastcall_t,
		stdcall_t,
		robloxcall_t
	};

	template<typename ret_t = void>
	struct function_caller_t
	{
		explicit function_caller_t( std::uintptr_t function, convention_t convention = convention_t::robloxcall_t )
			: function{ function }, convention{ convention } { }

		template<typename... args>
		ret_t operator()( args... arguments ) const
		{
			const auto addr = function;

			static const auto jmp_addr = [ ] ( ) -> std::uintptr_t
			{
				if ( const auto res = text.find_pattern< std::uintptr_t >( "ff e6" ) )
					return *res;

				return 0;
			}( );

			constexpr auto sz = ( sizeof( arguments ) + ... );

			/* Absolutely terrible solution but works */
			constexpr auto fastcall_sz = ( sizeof( arguments ) + ... ) - 8;

			__asm push esi

			switch ( convention )
			{
			case convention_t::cdecl_t:
				reinterpret_cast< ret_t( __cdecl* )( args... ) >( stub )( arguments... );
				_asm sub esp, sz
				break;
			case convention_t::stdcall_t:
				reinterpret_cast< ret_t( __stdcall* )( args... ) >( stub )( arguments... );
				break;
			case convention_t::robloxcall_t:
			case convention_t::fastcall_t:
				reinterpret_cast< ret_t( __fastcall* )( args... ) >( stub )( arguments... );
				break;
			}

			__asm
			{
				mov esi, cleanup
				push jmp_addr
				jmp[ addr ]

				cleanup:
				pop esi
			}

			if ( convention == convention_t::cdecl_t )
			{
				_asm add esp, sz
			}
			else if ( convention == convention_t::robloxcall_t )
			{
				_asm add esp, fastcall_sz
			}
		}

		convention_t convention;
		std::uintptr_t function;
	};

}