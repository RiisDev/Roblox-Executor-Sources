#pragma once

#include <dependencies/common_includes.hpp>

const auto text = ud::segment_t { ".text" };
const auto rdata = ud::segment_t { ".rdata" };
const auto vmpx = ud::segment_t { ".vmpx" };
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
		: function { function }, convention { convention } { }

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


namespace managers
{
	class offset_manager_t
	{
	public:
		struct
		{
			std::uint16_t primitive = 0xD8;
			std::uint16_t world = 0x184;

			/* Cannot require a non-RobloxScript module from a RobloxScript */
			std::int16_t require_flag = 0x194;

			/* Script threadref shit */
			std::ptrdiff_t thread_node = 356;
			std::ptrdiff_t node_first = 4;
			std::ptrdiff_t node_next = 12;
			std::ptrdiff_t state_off1 = 16;
			std::ptrdiff_t state_off2 = 4;

			/*Extraspace*/
			std::int16_t extraspace_script = 0x30;
		} offsets;

		struct
		{
			std::uintptr_t xorconst = [ ] ( ) -> std::uintptr_t
			{
				if ( const auto res = text.find_pattern< std::uintptr_t >( "BB ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B F0 56" ) )
					return *reinterpret_cast< std::uintptr_t* >( *res + 1 );

				return 0;
			}( );

			std::uintptr_t place_launcher = [ ] ( ) -> std::uintptr_t
			{
				if ( const auto res = text.find_pattern< std::uintptr_t >( "8B 0D ?? ?? ?? ?? 85 C9 74 06 8B 01 6A 01 FF 10 8B 4D F4 64 89 0D 00 00 00 00" ) )
					return *reinterpret_cast< std::uintptr_t* >( *res + 2 );

				return 0;
			}( );

			std::uintptr_t boundary = [ ] ( ) -> std::uintptr_t
			{
				if ( const auto res = text.find_pattern< std::uintptr_t >( "A1 ?? ?? ?? ?? ?? ?? 3B 0D ?? ?? ?? ?? 72 ?? A1 ?? ?? ?? ?? 2B D0 3B 15 ?? ?? ?? ??" ) )
					return *reinterpret_cast< std::uintptr_t* >( *res + 9 );

				return 0;
			}( );

			std::uintptr_t trustcheck = *text.find_pattern< std::uintptr_t >( "55 8B EC 6A FF 64 A1 00 00 00 00 68 ?? ?? ?? ?? 50 64 89 25 00 00 00 00 81 EC 8C 01 00 00" );

			// "settings()"
			function_caller_t< void > push_instance
			{
				[ ]( ) -> std::uintptr_t
				{
				   if ( const auto res = text.find_pattern< std::uintptr_t >( "E8 ?? ?? ?? ?? 8B 4D E8 83 C4 08 85 C9 74 05 E8 ?? ?? ?? ?? 8B 4D F4 B8 01 00 00 00 5F 64 89 0D 00 00 00 00 5E 8B E5 5D C3 68" ) )
					   return ud::calculate_relative< std::uint32_t >( *res, 5, 1 );

				   return 0;
				}( ),
				convention_t::cdecl_t
			};

			void( __thiscall* report_touch )( std::uintptr_t, std::uintptr_t, std::uintptr_t, std::uintptr_t ) =
				reinterpret_cast< decltype( report_touch ) >(
					[ ] ( ) -> std::uintptr_t
					{
						if ( const auto res = text.find_pattern< std::uintptr_t >( "55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 ?? ?? ?? ?? 50 64 89 25 ?? ?? ?? ?? 83 EC 1C 8B 55 08 C7 45 ?? ?? ?? ?? ??" ) )
							return *res;

						return 0;
					}( )
						);

			//$Script
			int( __thiscall* scriptcontext_resume )( void*, void*, int, int, int ) = reinterpret_cast< decltype( scriptcontext_resume ) >( base + 0x48a820);

			//$index
			std::uintptr_t( __thiscall* get_property )( std::uintptr_t, std::uintptr_t ) =
			reinterpret_cast< decltype( get_property ) >(
				[ ] ( ) -> std::uintptr_t
				{
					//if ( const auto res = text.find_pattern< std::uintptr_t >( "E8 ?? ?? ?? ?? 85 C0 0F 84 27 03 00 00" ) )
					//    return ud::calculate_relative< std::uint32_t >( *res, 5, 1 );

					return base + 0x54ff90;
				}( )
			);

			std::uintptr_t* property_table = [ ] ( ) -> std::uintptr_t*
			{
				//if ( const auto res = text.find_pattern< std::uintptr_t >( "8B 04 8D ?? ?? ?? ?? 8B D0" ) )
				//    return *reinterpret_cast< std::uintptr_t** >( *res + 3 );

				return reinterpret_cast< std::uintptr_t* >( base + 0x36d3ef0);
			}( );

			void( __fastcall* fire_click_detector )( std::uintptr_t, float, std::uintptr_t ) =
				reinterpret_cast< decltype( fire_click_detector ) >(
					[ ] ( ) -> std::uintptr_t
					{
						if ( const auto res = text.find_pattern< std::uintptr_t >( "55 8B EC 83 EC 10 8B 45 04 53 56 57" ) )
							return *res;

						return 0;
					}( )
			);

			function_caller_t< std::int32_t > luau_load
			{
				[ ]( ) -> std::uintptr_t
				{
					if ( const auto res = text.find_pattern< std::uintptr_t >( "E8 ?? ?? ?? ?? 8B 7D D8 83 C4 0C 89 45 E4" ) )
						return *res + *reinterpret_cast< std::uintptr_t* >( *res + 1 ) + 5;

					return 0;
				}( ),
				convention_t::robloxcall_t
			};

			/*
			v10 = "avatar/defaultShirt.rbxm";
				if ( a2 != 11 )
				  v10 = "avatar/defaultPants.rbxm";
			*/
			//void(__fastcall* blocking_load_instances)(void*, std::string*, std::vector<std::uint64_t>*) =
			//	reinterpret_cast<decltype(blocking_load_instances)>(base + 0x3b4310);
		} roblox;

		struct
		{
			void( __thiscall* luau_execute )( struct lua_State* ) =
				reinterpret_cast< decltype( luau_execute ) >(
					[ ] ( ) -> std::uintptr_t
					{
						if ( const auto res = text.find_pattern< std::uintptr_t >( "55 8B EC 83 E4 F8 80 79 ?? 00 74 09 E8 9F 00 00 00 8B E5 5D C3" ) )
							return *res;

						return 0;
					}( )
			);
		} luau;
	};
}

namespace offsets
{
	namespace task_scheduler
	{
		const auto task_scheduler_ptr = base + 0x39cabc4;
		constexpr auto jobs_start = 0x134;
		constexpr auto jobs_end = 0x138;
	}
}