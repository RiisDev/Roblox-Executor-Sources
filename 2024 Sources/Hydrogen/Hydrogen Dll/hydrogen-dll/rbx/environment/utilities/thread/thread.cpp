#include "thread.hpp"

#include <rbx/environment/environment.hpp>

#include <sol/sol.hpp>

using c_thread
	= rbx::environment::utilities::c_thread;

bool c_thread::is_thread_foreign( lua_State* L )
{
	return L->global != g_environment.get_roblox_state( )->global;
}

std::unordered_map< std::shared_ptr< void* >, sol::table > c_thread::get_scripts_with_thread( lua_State* L )
{
	std::unordered_map< std::shared_ptr< void* >, sol::table > scripts;

	sol::state_view sv{ L };

	for ( const auto& [i, v] : sv.lua_registry( ) )
	{
		if ( v.is< sol::thread >( ) )
		{
			if ( !v.pointer( ) )
				continue;

			const auto thread = v.as< sol::thread >( ).thread_state( );

			if ( !thread )
				continue;

			const auto script = get_threads_script< std::shared_ptr< void* > >( thread );

			if ( script )
			{
				if ( !scripts.contains( script ) )
					scripts[ script ] = sol::state_view{ thread }.lua_globals( );
			}
		}
	}

	return scripts;
}