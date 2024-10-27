#include "offsets.hpp"

#include <lua.h>

#include <iostream>
namespace managers
{
	void off_manager_t::luau_execute( lua_State* L )
	{
		( reinterpret_cast< void( __thiscall* )( lua_State* ) >( roblox::off::luau_execute ) )( L );
	}
	int off_manager_t::luaD_rawrunprotected( lua_State* L, void ( *f )( lua_State* L, void* ud ), void* ud )
	{
		std::cout << "Firead\n";
		auto res =( reinterpret_cast< int( __fastcall* )( lua_State*, void ( *f )( lua_State * L, void* ud), void* ud ) >( roblox::off::luad_rawrunprotected))( L, f, ud );

		__asm add esp, 4

		return res;
	}

	int off_manager_t::resume( std::uintptr_t script_context, lua_State* L, int narg )
	{
		struct threadref
		{
			std::uint32_t pad;
			lua_State* state;
		}ref{ 0, L };

		threadref* kek = &ref;

		return (reinterpret_cast<int(__thiscall*)(std::uintptr_t, void*, int, int, int)>(roblox::off::script_context::resume))(script_context, &kek, narg, 0, 0 );
	}

	off_manager_t& off_manager_t::get( )
	{
		static off_manager_t mgr;

		return mgr;
	}
}