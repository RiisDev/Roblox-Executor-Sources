#pragma once

#include <Luau/lfunc.h>
#include <Luau/lua.h>

#include <dependencies/common_includes.hpp>

#include <array>

struct lua_State;

namespace roblox::environment::security
{
	constexpr std::array<const char* const, 6> browser_service_list = {
		"closebrowserwindow",
		"copyauthcookie",
		"emithybrid",
		"javascript",
		"open",
		"sendcommand",
	};

	constexpr std::array<const char* const, 1> gui_service_list = {
		"openbrowserwindow",
	};

	constexpr std::array<const char* const, 2> marketplace_service_list = {
		"prompt",
		"purchase",
	};

	constexpr std::array<const char* const, 2> messagebus_service_list = {
		"getmessageid",
		"publish",
	};

	inline lua_CFunction browser_service_index, browser_service_namecall;
	std::int32_t browser_service_index_blocker(lua_State* state);
	std::int32_t browser_service_namecall_blocker(lua_State* state);

	inline lua_CFunction gui_service_index, gui_service_namecall;
	std::int32_t gui_service_index_blocker(lua_State* state);
	std::int32_t gui_service_namecall_blocker(lua_State* state);

	inline lua_CFunction marketplace_service_index, marketplace_service_namecall;
	std::int32_t marketplace_service_index_blocker(lua_State* state);
	std::int32_t marketplace_service_namecall_blocker(lua_State* state);

	inline lua_CFunction messagebus_service_index, messagebus_service_namecall;
	std::int32_t messagebus_service_index_blocker(lua_State* state);
	std::int32_t messagebus_service_namecall_blocker(lua_State* state);

	std::int32_t auth(lua_State* state);

	std::int32_t get_key( lua_State* state );
	std::int32_t get_hwid( lua_State* state );

	std::int32_t run_script( lua_State* state );

	std::int32_t delete_key( lua_State* state );

	inline bool authed = false;
}