#pragma once

#include <Windows.h>
#include <string>
#include <memory>

namespace rbx
{

	template < class T = void* >
	inline T o( const std::uintptr_t rva )
	{
		static const auto base 
			= reinterpret_cast< std::uint8_t* >
				( GetModuleHandleA( nullptr ) );

		return T( base + rva );
	}

	namespace player
	{
		namespace offs
		{
			const auto player_id = o< std::uint32_t* >( 0x39a71a0 ); // PlayerId=%llu\n
		}
	}

	namespace luau
	{
		namespace offs
		{
			const auto dummynode = o( 0x24A8E00 );
			const auto nilobject = o( 0x24A8E20 );

			const auto luau_execute 
				= o< void( __thiscall* )( void* ) >( 0x145B2B0 );

			static std::int32_t luad_rawrunprotected ( void* thread, void( *f )( void*, void* ), void* ud  )
			{
				auto ret = o< std::int32_t( __fastcall* )
					( void*, void( * )( void*, void* ), void* ) >
						( 0x1455850 )( thread, f, ud );

				__asm add esp, 4

				return ret;

			};
		}
	}

	namespace click_detector
	{
		namespace offs
		{
			const auto fire_shit
				= o<void( __thiscall* )( void* click_detector, void* player )>( 0x117e620 );

			const auto make_shared
				= o< void( __fastcall* )( void*, void* ) >( 0x3b7310 );

			//const auto fire_and_replicate_shit
			//	= o<void( __thiscall* )( void*, void*, int, void*, void* )>( 0x117fc40 );

			const auto event_thingy
				= o< std::uint8_t** >( 0x3b4feb8 );
		}
	}

	/*
		namespace click_detector
	{
		namespace offs
		{
			const auto fire_shit
				= o<void( __thiscall* )( void*, std::shared_ptr< void* >, int )>( 0x117e500 ); //0x117fc40

			const auto fire_and_replicate_shit
				= o<void( __thiscall* )( void*, void*, int, std::shared_ptr< void* > )>( 0x117fc40 );

			const auto make_shared
				= o< std::shared_ptr< void* >( __fastcall* )( void*, void* ) >( 0x3b7310 );

			const auto event_thingy
				= o< std::uint8_t** >( 0x3b4ffa0 );
		}
	}
	*/

	namespace base_script
	{
		namespace types
		{
			using request_code_t = std::uint8_t*( __thiscall* )( std::uint8_t* );
		}

		namespace offs
		{
			constexpr auto request_code //not the one that checks cache remote, just ask me or ill find it
				= 0x32; // TODO: update this iiv

			constexpr auto protected_string
				= 0x8;

			constexpr auto bytecode
				= 0x10;
		}
	}

	namespace touch_interest
	{
		namespace offs
		{
			// Finalize contacts  0 == start
			const auto touch_started
				= o< void( * )( void*, void* ) >( 0x13C6280 ); // TODO: this got inlined, check it out iiv

			const auto touch_ended 
				= o< void( * )( void*, void* ) >( 0x132FFC0 );
		}
	}

	namespace proximity_prompt
	{
		namespace offs
		{
			constexpr auto duration
				= 0x70;

			const auto fire_proximity_prompt
				= o< void( __thiscall* )( void* ) >( 0x107A200 );
		}
	}

	namespace instance
	{
		namespace offs
		{
			constexpr auto primitive
				= 0xC8;

			constexpr auto class_descriptor
				= 0xC;

			constexpr auto scriptable
				= 0x20;

			constexpr auto parent
				= 0x30;

			const auto compute_network_owner_is_someone_else // newrt & throttling
				= o< bool( __thiscall* )( void* ) >( 0xCFEC50 );

			const auto push_instance
				= o< void( __cdecl* )( void*,
					const std::shared_ptr< void* >& ) >( 0x41DEC0 );

			const auto push_instance_u
				= o< void( __cdecl* )( void*,
					const void* ) >( 0x41DEC0 );

			const auto function_script_slot_call_operator // Maximum event re-entrancy depth (%d) exceeded for %s.%s
				= o< std::int32_t( __thiscall* )( void* ) >( 0x42DD10 );

			const auto set_generic_callback_helper 
				= o< std::int32_t( __cdecl* )( void* ) >( 0x1513bb0 ); // TODO: this got inlined iiv

			const auto property_table = o< std::uintptr_t* >( 0x3950620 );

			const auto get_property
				= o < std::uintptr_t*( __thiscall* )( void*, std::uintptr_t )>( 0x42B040 );
		}
	}

	namespace extra_space
	{
		namespace offs
		{
			constexpr auto identity
				= 0x18;

			constexpr auto system_thread_identity
				= 0x60; // set camera movement mode for touch devices

			constexpr auto script
				= 0x30;
		}
	}

	namespace script_context 
	{
		namespace structs
		{
			struct live_thread_ref_t
			{
				std::int32_t m_id;
				void* m_thread;
			};

			struct thread_ref_t
			{
				live_thread_ref_t* m_live_thread_ref;
			};
		}

		namespace offs
		{
			using resume_t = std::int32_t( __thiscall* )
				( void*, void**, 
					std::int32_t, std::int32_t, std::int32_t );

			const auto resume 
				= o< resume_t >( 0x3A69D0 );

			constexpr auto loaded_modules // Exception thrown while cleaning up Lua: %s TODO: iiv find this pls
				= 0x478;
		}
	}

	namespace task_scheduler
	{
		namespace structs
		{
			struct job_t
			{
				void** m_vftable;
				std::uint8_t pad_0[ 0xC ];
				std::string m_name;
				std::uint8_t pad_1[ 0x10 ];
				double m_time;
				std::uint8_t pad_2[ 0x10 ];
				double m_time_spend;
				std::uint8_t pad_3[ 0x8 ];
				std::uintptr_t m_state;
			};

			struct job_container_t
			{
				job_t* m_job;
				void* m_reference;
			};
		}

		namespace offs
		{
			const auto self = o< std::uint8_t** >( 0x3C49EE8 );

			constexpr auto jobs_start = 0x134;
			constexpr auto jobs_end = jobs_start + sizeof( structs::job_t* );

			constexpr auto script_context = 0x130;
			constexpr auto global_state = 0x114;

			const auto whsj_ctor
				= o< std::uint8_t* >( 0x3c6700 ); // "WaitingHybridScriptsJob"
		}
	}
}