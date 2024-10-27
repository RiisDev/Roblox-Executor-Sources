#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <variant>

#include <rbx/environment/utilities/instance/instance.hpp>

namespace rbx::environment
{
	class c_script
	{
		/*
			<table> getsenv(union<LocalScript, ModuleScript> Script)  
			Returns the environment of Script. Errors if the script is not loaded in memory.
		*/
		static sol::table getsenv( instance_t script, sol::this_state L );

		/*
			<union<LocalScript, ModuleScript, nil>> getcallingscript(<void>)  
			Gets the script that is calling this function.
		*/
		static std::int32_t getcallingscript( lua_State* L );

		/*
			<function> getscriptclosure(union<LocalScript, ModuleScript> Script)  
			Gets a bare function from the script passed. Please note this is not the original function of the script and will not have upvalues/environment correctly defined.
		*/
		static sol::function getscriptclosure( instance_t script, sol::this_state L );

		/*
			<string> getscriptbytecode(union<LocalScript, ModuleScript> Script)
			Returns bytecode from script.
		*/
		static std::string getscriptbytecode( instance_t script, sol::this_state L );

		/*
			<string> getscripthash(union<LocalScript, ModuleScript> Script)  
			Returns a SHA384 hash of the scripts bytecode. You can use this to detect changes of a script.
		*/
		static std::string getscripthash( instance_t script, sol::this_state L );
	public:

		void init( lua_State* L ) const;
	};
}