#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <rbx/environment/utilities/instance/instance.hpp>

#include <filesystem>

namespace rbx::environment
{
	class c_cache
	{
		/*
			<void> cache.replace(<Instance> obj, <Instance> t_obj)  
			Replace obj in the instance cache with t_obj.
		*/
		static void replace( instance_t obj, instance_t t_obj, sol::this_state L );

		/*
			<void> cache.invalidate(<Instance> obj)  
			Invalidate obj's cache entry, forcing a re-cache upon the next lookup.
		*/
		static void invalidate( instance_t obj, sol::this_state L );

		/*
			<bool> cache.iscached(<Instance> obj)
			Returns true if obj is currently cached within the registry.
		*/
		static bool iscached( instance_t obj, sol::this_state L );

		/*
			<Instance> cloneref(<Instance> obj)
			Returns a copy of the Instance reference to object. This is useful for managing an Instance without directly referencing it.
		*/
		static sol::object cloneref( instance_t obj, sol::this_state L );

		/*
			<bool> compareinstances(<Instance> obj, t_obj)
			Returns whether objects obj and t_obj both reference the same Instance.
		*/
		static bool compareinstances( instance_t obj, instance_t t_obj, sol::this_state L );
	public:

		void init( lua_State* L ) const;
	};
}