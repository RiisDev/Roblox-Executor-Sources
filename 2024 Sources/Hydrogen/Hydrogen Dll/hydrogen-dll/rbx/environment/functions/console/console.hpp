#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

namespace rbx::environment
{
	class c_console
	{
		/*
			<void> rconsoleprint(<string> message)
			Prints message into the console. rconsoleprint also supports colors.
		*/
		static void rconsoleprint( std::string_view out, sol::this_state L );

		/*
			<void> rconsoleinfo(<string> message)
			Prints message into the console, with a info text before it.
		*/
		static void rconsoleinfo( std::string_view out, sol::this_state L );

		/*
			<void> rconsolewarn(<string> message)
			Prints message into the console, with a warning text before it.
		*/
		static void rconsolewarn( std::string_view out, sol::this_state L );

		/*
			<void> rconsoleerr(<string> message)
			Prints message into the console, with a error text before it.
		*/
		static void rconsoleerr( std::string_view out, sol::this_state L );

		/*
			<void> rconsoleclear(<void>)
			Clears the console.
		*/
		static void rconsoleclear( sol::this_state L );

		/*
			<void> rconsolename(<string> title)
			Sets the currently allocated console title to title.
		*/
		static void rconsolename( std::string_view out, sol::this_state L );

		/*
			<string> rconsoleinput(<void>)
			Yields until the user inputs information into their console. Returns the input they put in.
		*/
		static std::int32_t rconsoleinput( lua_State* L );

		/*
			<void> printconsole(<string> message, <byte> red, <byte> green, <byte> blue)
			Prints message into the internal and integrated console with RGB value. To view the internal console, 
			enable Internal UI in the Options menu and when in-game press INSERT on your keyboard. 
			The integrated console can be found in the new synapse UI.
		*/
		static void printconsole( std::string_view out, double r, double g, double b, sol::this_state L );

	public:

		void init( lua_State* L ) const;
	};
}