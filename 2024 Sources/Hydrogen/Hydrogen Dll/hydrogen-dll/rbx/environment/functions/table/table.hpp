#pragma once


#include <luau/lua.h>
#include <sol/sol.hpp>

namespace rbx::environment
{
	class c_table
	{
		/*
			<table> getrawmetatable(<table> value)  
			Retrieve the metatable of value irregardless of value's metatable's __metatable field. Returns nil if it doesn't exist.
		*/
		static std::int32_t getrawmetatable( lua_State* L );

		/*
			<bool> setrawmetatable(<object> o, <table> mt)
			Sets o's metatable to mt even if the __metatable field exists in o's metatable.
		*/
		static std::int32_t setrawmetatable( lua_State* L );

		/*
			<void> setreadonly(<table> t, <bool> val)  
			Sets t's read-only value to val.
		*/
		static std::int32_t setreadonly( lua_State* L );

		/*
			<bool> isreadonly(<table> t)  
			Returns t's read-only condition.
		*/
		static std::int32_t isreadonly( lua_State* L );

	public:

		void init( lua_State* L ) const;
	};
}