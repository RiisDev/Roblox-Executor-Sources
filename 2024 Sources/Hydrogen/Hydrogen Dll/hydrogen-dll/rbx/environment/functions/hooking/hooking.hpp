#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

namespace rbx::environment
{
	class c_hooking
	{
		/*
			<function> hookfunction(<function> old, <function> hook)  
			Hooks function old, replacing it with the function hook. The old function is returned, you must use this function in order to call the original function.
		*/
		static int hookfunction( lua_State* L );

		static std::int32_t hookmetamethod( lua_State* L );

		/*
			<function> newcclosure(<function> f)  
			Pushes a new CClosure that invokes function f upon call.
		*/
		static int newcclosure( lua_State* L );

		/*
			<function> clonefunction(<function> f)
			Pushes a new function that contains the content of the original one.
		*/
		static int clonefunction( lua_State* L );

	public:

		void init( lua_State* L ) const;
	};
}