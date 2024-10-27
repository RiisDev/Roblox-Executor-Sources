#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <variant>

#include <rbx/environment/utilities/instance/instance.hpp>

namespace rbx::environment
{
	class c_misc
	{
		/*
			<void> setclipboard(<string> value)  
			Sets value to the clipboard.
		*/
		static void setclipboard( std::string value, sol::this_state L );

		/*
			<string> getnamecallmethod(<void>)
			Returns the namecall method if the function is called in an __namecall metatable hook.
		*/
		static sol::object getnamecallmethod( sol::this_state L );

		/*
			<void> setnamecallmethod(<string> method)
			Sets the current namecall method to the new namecall method. Must be called in a __namecall metatable hook.
		*/
		static void setnamecallmethod( std::string method, sol::this_state L );

		/*
			<uint> messagebox(<string> text, <string> caption, <uint> flags)
			Creates a message box with parameters text, caption and style.
		*/
		static std::int32_t messagebox( lua_State* L );

		/*
			<void> setfpscap(<uint> fps)
			Sets the in-game FPS cap to fps. If fps is 0, the FPS cap is disabled.
		*/
		static void setfpscap( std::uint32_t fps, sol::this_state L );
	public:

		void init( lua_State* L ) const;
	};
}