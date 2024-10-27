#pragma once

#include <roblox/memory/fnhandler.hpp>
#include <roblox/memory/offsets.hpp>

struct lua_State;

namespace managers
{
	struct off_manager_t
	{

		void luau_execute( lua_State* L );
		int luaD_rawrunprotected(lua_State* L, void ( *f )( lua_State* L, void* ud ), void* ud );
		int resume( std::uintptr_t script_context, lua_State* L, int narg );
		static off_manager_t& get( );
	};
}