#pragma once

#include <dependencies/luau/lua.h>
#include <sol/sol.hpp>
#include <rbx/rbx_structs.hpp>

namespace rbx::environment::utilities
{
	class c_thread
	{
	public:
		static bool is_thread_foreign( lua_State* L );
	
		template< typename T >
		static T get_threads_script( const lua_State* L )
		{
			return *reinterpret_cast< T* >(
				reinterpret_cast< std::uintptr_t >( L->userdata ) + rbx::extra_space::offs::script );
		}

		static std::unordered_map< std::shared_ptr< void* >, sol::table > get_scripts_with_thread( lua_State* L );

		static bool is_script_running( lua_State* L, std::uint8_t* script )
		{
			const auto scripts = get_scripts_with_thread( L );

			for ( const auto [key, value] : scripts )
			{
				if ( reinterpret_cast< std::uint8_t* >( key.get( ) ) == script )
					return true;
			}

			return false;
		}

		static void set_thread_identity( lua_State* L, std::uint32_t identity )
		{
			*reinterpret_cast< std::size_t* >(
				reinterpret_cast< std::uint8_t* >( L->userdata ) +
					extra_space::offs::identity ) = identity;

			*reinterpret_cast< std::uintptr_t* >(
				*reinterpret_cast< std::uintptr_t* >(
					__readfsdword( 0x2C ) ) + extra_space
						::offs::system_thread_identity ) = identity;
		}

		static std::uint32_t get_thread_identity( lua_State* L )
		{
			return *reinterpret_cast< std::size_t* >(
				reinterpret_cast< std::uint8_t* >( L->userdata ) +
					extra_space::offs::identity );
		}
	};
}