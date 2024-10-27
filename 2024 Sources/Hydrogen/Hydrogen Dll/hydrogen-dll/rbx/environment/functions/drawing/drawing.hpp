#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

namespace rbx::environment
{
	class c_drawing
	{
		/*
			<object> Drawing.new(<string> type)
			Creates a new drawing object with type. Returns the object.
		*/
		static sol::object drawingnew( std::string type, sol::this_state L );

		/*
			<void> cleardrawcache(<void>)
			Destroys every drawing object in the cache. Invalidates references to the drawing objects.
		*/
		static void cleardrawcache( sol::this_state L );

		static sol::object getrenderproperty( sol::userdata obj, std::string_view prop, sol::this_state L );

		static void setrenderproperty( sol::userdata obj, std::string_view prop, sol::object value, sol::this_state L );

		static bool isrenderobj( sol::object obj );
	public:

		void init( lua_State* L ) const;
	};
}