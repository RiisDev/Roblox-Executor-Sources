#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <variant>

#include <rbx/environment/utilities/instance/instance.hpp>

namespace rbx::environment
{
	class c_input
	{
		static inline HWND game_window;

		/*
			<bool> iswindowactive(<void>)  
			Returns if the main window is in focus. This must return true for any other mouse/keyboard function to work.
		*/
		static bool iswindowactive( sol::this_state L );

		/*
			<void> keypress(<uint> keycode)  
			Simulates a key press for the specified keycode. Keycodes are listed here https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes.
		*/
		static void keypress( std::uint32_t keycode, sol::this_state L );

		/*
			<void> keyrelease(<uint> key)  
			Releases key on the keyboard. You can access the key values from the link above.
		*/
		static void keyrelease( std::uint32_t keycode, sol::this_state L );

		/*
			<void> mouse1click(<void>)  
			Simulates a full left mouse button press.
		*/
		static void mouse1click( sol::this_state L );

		/*
			<void> mouse1press(<void>)  
			Simulates a left mouse button press without releasing it.
		*/
		static void mouse1press( sol::this_state L );

		/*
			<void> mouse1release(<void>)  
			Simulates a left mouse button release.
		*/
		static void mouse1release( sol::this_state L );

		/*
			<void> mouse2click(<void>)  
			Simulates a full right mouse button press.
		*/
		static void mouse2click( sol::this_state L );

		/*
			<void> mouse2press(<void>)  
			Clicks down on the right mouse button.
		*/
		static void mouse2press( sol::this_state L );

		/*
			<void> mouse2release(<void>)  
			Simulates a right mouse button release.
		*/
		static void mouse2release( sol::this_state L );

		/*
			<void> mousescroll(<number> px)  
			Scrolls the mouse wheel virtually by px pixels.
		*/
		static void mousescroll( double px, sol::this_state L );

		/*
			<void> mousemoverel(<number> x, <number> y)  
			Moves the mouse cursor relatively to the current mouse position by coordinates x and y.
		*/
		static void mousemoverel( double x, double y, sol::this_state L );

		/*
			<void> mousemoveabs(<number> x, <number> y)  
			Move's your mouse to the x and y coordinates in pixels from topleft of the main window.
		*/
		static void mousemoveabs( double x, double y, sol::this_state L );
	public:

		void init( lua_State* L ) const;
	};
}