#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>

namespace roblox::drawing
{
	struct base_t;
}

namespace managers
{
	class drawing_manager_t
	{
	public:
		std::vector< roblox::drawing::base_t*> drawing_objects;
	};

	extern drawing_manager_t drawing_mgr;
}
