#include "security.hpp"

#include <managers/managers.hpp>
#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>
#include <Luau/lmem.h>

#include <managers/function/function.hpp>

#define GENERATE_NAMECALL_BLOCKER(old_method, list) \
		if (*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(state->userdata) + offsets::extraspace_script)) \
			return old_method(state); \
 \
		const std::string method = lua_namecallatom(state, nullptr); \
 \
		if ((lua_isuserdata(state, 1) && lua_touserdata(state, 1))) \
		{ \
			if (should_block(list, method)) \
				return 0; \
		} \
 \
		return old_method(state); \

#define GENERATE_INDEX_BLOCKER(old_method, list) \
\
		if (*reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::uintptr_t>(state->userdata) + offsets::extraspace_script)) \
			return old_method(state); \
 \
		if (ttype(managers::function_mgr.index2addr(state, 2)) == LUA_TSTRING) \
		{ \
			std::string method = lua_tostring(state, 2); \ 
\
			if (should_block(list, method)) \
			return 0; \
		} \
		\
		return old_method(state); \

static const auto should_block = [](auto list, std::string method) -> bool //copy is intentional here
{
	std::transform(method.begin(),
		method.end(),
		method.begin(),
		tolower);

	for (const auto term : list)
	{
		if (method.find(term) != std::string::npos)
			return true;
	}

	return false;
};

std::int32_t roblox::environment::security::browser_service_index_blocker(lua_State* state)
{

	GENERATE_INDEX_BLOCKER(browser_service_index, browser_service_list)
}

std::int32_t roblox::environment::security::browser_service_namecall_blocker(lua_State* state)
{
	GENERATE_NAMECALL_BLOCKER(browser_service_namecall, browser_service_list)
}

std::int32_t roblox::environment::security::gui_service_index_blocker(lua_State* state)
{
	GENERATE_INDEX_BLOCKER(gui_service_index, gui_service_list)
}

std::int32_t roblox::environment::security::gui_service_namecall_blocker(lua_State* state)
{
	GENERATE_NAMECALL_BLOCKER(gui_service_namecall, gui_service_list)
}

std::int32_t roblox::environment::security::marketplace_service_index_blocker(lua_State* state)
{
	GENERATE_INDEX_BLOCKER(marketplace_service_index, marketplace_service_list)
}

std::int32_t roblox::environment::security::marketplace_service_namecall_blocker(lua_State* state)
{
	GENERATE_NAMECALL_BLOCKER(marketplace_service_namecall, marketplace_service_list)
}

std::int32_t roblox::environment::security::messagebus_service_index_blocker(lua_State* state)
{
	GENERATE_INDEX_BLOCKER(messagebus_service_index, messagebus_service_list)
}

std::int32_t roblox::environment::security::messagebus_service_namecall_blocker(lua_State* state)
{
	GENERATE_NAMECALL_BLOCKER(messagebus_service_namecall, messagebus_service_list)
}

std::int32_t roblox::environment::security::auth(lua_State* state)
{
	const std::string key = luaL_checkstring( state, 1 );

	authed = true;

	std::ofstream stream( managers::roblox_mgr.hydrogen_dir / "key", std::ios::beg | std::ios::binary );

	if ( stream.is_open( ) )
	{

		stream.write( key.c_str( ), key.size( ) );

	}

	managers::roblox_mgr.execute_script(
#include <scripts/ui.lua>
	);

	lua_pushnil( managers::roblox_mgr.thread );
	lua_setfield( managers::roblox_mgr.thread, LUA_REGISTRYINDEX, "PEOU2IGKB4VKSBGPYQAD" );

	lua_pushnil( managers::roblox_mgr.thread );
	lua_setfield( managers::roblox_mgr.thread, LUA_REGISTRYINDEX, "TDXP4J2MQBH163PZT1S0" );

	lua_pushnil( managers::roblox_mgr.thread );
	lua_setfield( managers::roblox_mgr.thread, LUA_REGISTRYINDEX, "TDXP4J2MQBH163PZT1S1" );

	managers::function_mgr.lua_pushcclosurek( managers::roblox_mgr.thread, security::run_script, nullptr, 0, nullptr );
	lua_setfield( managers::roblox_mgr.thread, LUA_REGISTRYINDEX, "GTX4CHQYWURD3CR12JF" );

	return 0;
}

std::int32_t roblox::environment::security::get_key( lua_State* state )
{
	std::string base_link = "https://gateway.hydrogen.sh/";
	managers::jni_mgr.set_clipboard( base_link.append( managers::jni_mgr.get_hwid( ) ).c_str( ) );

	return 0;
}

std::int32_t roblox::environment::security::get_hwid( lua_State* state )
{
	lua_pushstring( state, managers::jni_mgr.get_hwid( ).c_str( ) );
	return 1;
}

std::int32_t roblox::environment::security::run_script( lua_State* state )
{
	if ( authed )
	{
		managers::roblox_mgr.execute_script( luaL_checkstring( state, 1 ) );
	}

	return 0;
}

std::int32_t roblox::environment::security::delete_key( lua_State* state )
{

	const auto path = managers::roblox_mgr.hydrogen_dir / "key";
	if ( !fs::exists( path ) )
		return 0;

	fs::remove( path );

	return 0;
}
