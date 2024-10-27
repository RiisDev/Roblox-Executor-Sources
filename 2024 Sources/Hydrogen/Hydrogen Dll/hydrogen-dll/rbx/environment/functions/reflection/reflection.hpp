#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <variant>

#include <rbx/environment/utilities/instance/instance.hpp>

namespace rbx::environment
{
	class c_reflection
	{
		/*
			<union<function, nil>, <string?>> loadstring(<string> chunk, <string?> chunk_name)
			Loads chunk as a Lua function with optional chunk_name and returns it if compilation is successful. Otherwise, if an error has occurred during compilation, nil followed by the error message will be returned.
		*/
		static std::tuple< sol::object, std::optional< std::string > > loadstring( std::string chunk, std::optional< std::string > chunk_name, sol::this_state L );

		/*
			<bool> checkcaller(<void>)  
			Returns true if the current thread is a Exploit thread. Note: Checkcaller does NOT check the call stack of the function, if you call a game function then it calls out to checkcaller, the result will be true! Be careful.
		*/
		static bool checkcaller( sol::this_state L );

		/*
			<bool> islclosure(<function> f)  
			Returns true if f is an LClosure.
		*/
		static bool islclosure( sol::function f, sol::this_state L );

		/*
			<bool> iscclosure(<function> f)
			Returns true if f is an CClosure.
		*/
		static bool iscclosure( sol::function f, sol::this_state L );

		/*
			<bool> isexecutorclosure(<function> f)
			Returns true if f is a closure made by the executor.
		*/
		static bool isexecutorclosure( sol::function f, sol::this_state L );
	public:

		void init( lua_State* L ) const;
	};
}