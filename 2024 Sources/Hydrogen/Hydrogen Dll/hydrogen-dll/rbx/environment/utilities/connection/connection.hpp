#pragma once

#include <dependencies/luau/lua.h>
#include <sol/sol.hpp>

namespace rbx::environment::utilities
{
	inline std::int32_t connection_tag = 0;

	class c_connection
	{
	public:
		static void set_connection_tag( lua_State* L );
		static std::int32_t get_connection_tag( );

		static std::uint8_t* get_weak_function_ref( std::uint8_t* connection );

		static lua_State* get_connections_thread( std::uint8_t* connection );
		static sol::function get_connections_function( std::uint8_t* connection );
	};
}