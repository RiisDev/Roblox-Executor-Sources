#pragma once
#include <lua.h>

namespace roblox
{
	namespace env
	{
		void init( lua_State* const state );
	}
	namespace fn
	{
		void set_thread_identity( lua_State* state, std::size_t identity );
		void set_system_identity( lua_State* state, std::size_t identity );
		std::size_t get_thread_identity( lua_State* state );
	}
}