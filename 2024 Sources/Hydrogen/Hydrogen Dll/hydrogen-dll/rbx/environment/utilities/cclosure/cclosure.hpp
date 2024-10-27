#pragma once

#include <dependencies/luau/lua.h>
#include <lobject.h>

#include <unordered_map>
#include <optional>

namespace rbx::environment::utilities
{
	enum closure_type_t
	{
		cclosure,
		lclosure,
		newcclosure,
	};

	class c_cclosure
	{
	public:

		static std::optional< Closure* > get_closure( Closure* closure );

		static void clone_function( lua_State* L, int i );
		static closure_type_t get_closure_type( Closure* c );

		static std::int32_t handler( lua_State* L );

		static void add( Closure* closure, Closure* lclosure );

		static void convert( Closure* closure, Closure* lclosure );

		static std::int32_t wrap( lua_State* L );
	} inline g_cclosure;
}

