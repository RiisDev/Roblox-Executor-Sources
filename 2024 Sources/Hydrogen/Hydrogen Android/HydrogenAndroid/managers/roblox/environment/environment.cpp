#include "environment.hpp"

#include <managers/managers.hpp>

#include <dependencies/Luau/lua.h>

#include <array>

#include "core/core.hpp"
#include "console/console.hpp"
#include "cache/cache.hpp"
#include "debug_lib/debug_lib.hpp"
#include "drawing/drawing.hpp"
#include "security/security.hpp"

void setup_input( lua_State* state ) {
	auto& roblox_manager = managers::roblox_mgr;
	
	auto current_top = lua_gettop( state );

	lua_getglobal( state, "game" );
	lua_getfield( state, -1, "GetService" );
	lua_pushvalue( state, -2 );
	lua_pushstring( state, "CoreGui" );
	if ( lua_pcall( state, 2, 1, 0 ) != 0 ) {
		managers::utility_mgr.log<ANDROID_LOG_ERROR>( "Failed to get CoreGui | %s", lua_tostring( state, -1 ) );
		lua_settop( state, current_top );
		return;
	}
	
	lua_getglobal( state, "Instance" );
	lua_getfield( state, -1, "new" );
	lua_pushstring( state, "TextBox" );
	lua_pushvalue( state, -4 );
	if ( lua_pcall( state, 2, 1, 0 ) != 0 ) {
		managers::utility_mgr.log<ANDROID_LOG_ERROR>( "Failed to instantiate a new RBX::TextBox | %s", lua_tostring( state, -1 ) );
		lua_settop( state, current_top );
		return;
	}

	lua_pushboolean( state, false );
	lua_setfield( state, -2, "ClearTextOnFocus" );	

	lua_pushboolean( state, true );
	lua_setfield( state, -2, "MultiLine" );	

	roblox_manager.textbox_ptr = *reinterpret_cast<const std::uintptr_t*>( lua_topointer( state, -1 ) );
	roblox_manager.textbox_ref = lua_ref( state, -1 );

	lua_pushstring( state, "" );
	lua_setfield( state, -2, "Text" );

	lua_settop( state, current_top );
	managers::utility_mgr.log<ANDROID_LOG_INFO>( "Input successfully setup | RBX::TextBox: %x (ref %d)", roblox_manager.textbox_ptr, roblox_manager.textbox_ref );
}

void roblox::environment::initiate()
{
	core::workspace_path = managers::roblox_mgr.hydrogen_dir / "Workspace";

	if (!fs::exists(core::workspace_path))
		fs::create_directory(core::workspace_path);

	auto& roblox_manager = managers::roblox_mgr;

	managers::function_mgr.lua_pushcclosurek( roblox_manager.thread, security::get_key, nullptr, 0, nullptr );
	lua_setfield( roblox_manager.thread, LUA_REGISTRYINDEX, "PEOU2IGKB4VKSBGPYQAD" );

	managers::function_mgr.lua_pushcclosurek( roblox_manager.thread, security::auth, nullptr, 0, nullptr );
	lua_setfield( roblox_manager.thread, LUA_REGISTRYINDEX, "TDXP4J2MQBH163PZT1S0" );

	managers::function_mgr.lua_pushcclosurek( roblox_manager.thread, security::get_hwid, nullptr, 0, nullptr );
	lua_setfield( roblox_manager.thread, LUA_REGISTRYINDEX, "TDXP4J2MQBH163PZT1S1" );

	managers::function_mgr.lua_pushcclosurek( roblox_manager.thread, security::delete_key, nullptr, 0, nullptr );
	lua_setfield( roblox_manager.thread, LUA_REGISTRYINDEX, "TDXP4J2MQBH163PZT1S169" );

	const auto register_function = [&](lua_CFunction function, const char* name, int nups = 0)
	{
		managers::function_mgr.lua_pushcclosurek(roblox_manager.thread, function, nullptr, nups, nullptr);

		lua_setfield(roblox_manager.thread, LUA_GLOBALSINDEX, name);
	};

	const auto register_member = [&](lua_CFunction function, const char* name, int nups = 0)
	{
		managers::function_mgr.lua_pushcclosurek(roblox_manager.thread, function, nullptr, nups, nullptr);

		lua_setfield(roblox_manager.thread, -2, name);
	};

	register_function(security::auth, "hydrogen");

	register_function( core::request, "request" );

	register_function(core::loadstring, "loadstring");

	register_function(core::getgenv, "getgenv");
	register_function(core::getrenv, "getrenv");
	register_function(core::getreg, "getreg");

	register_function(core::setrawmetatable, "setrawmetatable");
	register_function(core::getrawmetatable, "getrawmetatable");

	register_function(core::isreadonly, "isreadonly");
	register_function(core::is_writeable, "iswriteable");
	register_function(core::setreadonly, "setreadonly");
	register_function(core::make_readonly, "makereadonly");
	register_function(core::make_writeable, "makewriteable");

	register_function(core::iscclosure, "iscclosure");
	register_function(core::islclosure, "islclosure");

	register_function(core::get_namecall_method, "getnamecallmethod");
	register_function(core::set_namecall_method, "setnamecallmethod");

	register_function(core::identifyexecutor, "identifyexecutor");
	register_function(core::identifyexecutor, "getexecutorname");

	register_function(core::is_executor_closure, "checkclosure");
	register_function(core::is_executor_closure, "isexecutorclosure");
	register_function(core::is_executor_closure, "is_hydrogen_closure");
	register_function(core::is_executor_closure, "isourclosure");

	register_function(core::get_thread_identity, "getthreadidentity");
	register_function(core::set_thread_identity, "getidentity");
	register_function(core::get_thread_identity, "getthreadcontext");
	register_function(core::set_thread_identity, "setthreadidentity");
	register_function(core::set_thread_identity, "setidentity");
	register_function(core::set_thread_identity, "setthreadcontext");

	register_function(core::checkcaller, "checkcaller");

	register_function(core::newcclosure, "newcclosure");

	register_function(core::getgc, "getgc");

	register_function(core::get_instances, "getinstances");
	register_function(core::get_nil_instances, "getnilinstances");
	register_function(core::get_scripts, "getscripts");

	register_function(core::get_script_from_thread, "getscriptfromthread");
	register_function(core::getsenv, "getsenv");

	register_function(core::get_loaded_modules, "getloadedmodules");
	register_function(core::get_running_scripts, "getrunningscripts");

	register_function(console::rconsoleprint, "rconsoleprint");
	register_function(console::rconsoleprint, "consoleprint");
	register_function(console::rconsoleprint, "printconsole");
	register_function(console::rconsoleerr, "rconsoleerr");
	register_function(console::rconsoleinfo, "rconsoleinfo");
	register_function(console::rconsolewarn, "rconsolewarn");
	register_function(console::rconsolename, "rconsolename");
	register_function(console::rconsolename, "rconsolesettitle");
	register_function(console::rconsolename, "consolesettitle");
	register_function(console::rconsoleclear, "rconsoleclear");
	register_function(console::rconsoleclear, "consoleclear");
	register_function(console::rconsoleshow, "rconsoleshow");
	register_function(console::rconsolehide, "rconsolehide");
	register_function(console::rconsoleshow, "rconsolecreate");
	register_function(console::rconsoleshow, "consolecreate");
	register_function(console::rconsolehide, "rconsoledestroy");
	register_function(console::rconsolehide, "consoledestroy");

	register_function(core::append_file, "appendfile");
	register_function(core::write_file, "writefile");
	register_function(core::read_file, "readfile");
	register_function(core::delete_file, "delfile");
	register_function(core::list_files, "listfiles");
	register_function(core::is_file, "isfile");
	register_function(core::is_folder, "isfolder");
	register_function(core::make_folder, "makefolder");
	register_function(core::delete_folder, "delfolder");
	register_function(core::load_file, "loadfile");

	register_function(core::set_scriptable, "setscriptable");
	register_function(core::is_scriptable, "isscriptable");
	register_function(core::get_hidden_property, "gethiddenproperty");
	register_function(core::set_hidden_property, "sethiddenproperty");
	register_function(core::get_properties, "getproperties");
	register_function(core::get_hidden_properties, "gethiddenproperties");

	register_function(core::is_rbx_active, "isrbxactive");
	register_function(core::is_rbx_active, "isgameactive");

	register_function(core::cloneref, "cloneref");
	register_function(core::compareinstances, "compareinstances");

	register_function(core::setfpscap, "setfpscap");

	register_function(core::lz4compress, "lz4compress");
	register_function(core::lz4decompress, "lz4decompress");

	register_function(core::clonefunction, "clonefunction");

	register_function(core::get_script_bytecode, "getscriptbytecode");
	register_function(core::get_script_bytecode, "dumpstring");
	register_function(core::get_script_closure, "getscriptclosure");
	register_function(core::get_script_hash, "getscripthash");

	register_function(core::hookfunction, "hookfunction");
	register_function(core::hookfunction, "replaceclosure");
	register_function(core::hookmetamethod, "hookmetamethod");

	register_function(core::get_calling_script, "getcallingscript");

	register_function(core::fire_touch_interest, "firetouchinterest");
	register_function(core::fire_click_detector, "fireclickdetector");
	register_function(core::fire_proximity_prompt, "fireproximityprompt");
	register_function(core::get_proximity_prompt_duration, "getproximitypromptduration");
	register_function(core::set_proximity_prompt_duration, "setproximitypromptduration");

	register_function(core::get_fflag, "getfflag");
	register_function(core::set_fflag, "setfflag");

	register_function(core::setclipboard, "setclipboard");

	register_function(core::getconnections, "getconnections");
	register_function(core::firesignal, "firesignal");

	register_function(core::queue_on_teleport, "queue_on_teleport");

	roblox::drawing::initiate();

	lua_createtable(roblox_manager.thread, 0, 0);
	{
		register_member(cache::invalidate, "invalidate");
		register_member(cache::replace, "replace");
		register_member(cache::is_cached, "iscached");
	}
	lua_setglobal(roblox_manager.thread, "cache");

	lua_createtable(roblox_manager.thread, 0, 0);
	{
		register_member(debug::get_upvalue, "getupvalue");
		register_member(debug::get_upvalues, "getupvalues");
		register_member(debug::set_upvalue, "setupvalue");

		register_member(debug::get_proto, "getproto");
		register_member(debug::get_protos, "getprotos");

		register_member(debug::get_constant, "getconstant");
		register_member(debug::get_constants, "getconstants");
		register_member(debug::set_constant, "setconstant");

		register_member(debug::get_info, "getinfo");

		register_member(debug::get_stack, "getstack");
		register_member(debug::set_stack, "setstack");
	}
	lua_setglobal(roblox_manager.thread, "debug");

	lua_getglobal(roblox_manager.thread, "debug");

	lua_pushnil(roblox_manager.thread);

	while (lua_next(roblox_manager.thread, -2))
	{
		const auto key = lua_tostring(roblox_manager.thread, -2);

		lua_setglobal(roblox_manager.thread, key);
	}

	const auto state = roblox_manager.thread;
	const auto hook_game_metamethod = [state](const char* const metamethod, const lua_CFunction to) -> lua_CFunction
	{
		lua_getglobal(state, "game");
		lua_getmetatable(state, -1);
		lua_getfield(state, -1, metamethod);

		const auto original = lua_tocfunction(state, -1);

		reinterpret_cast<Closure*>(const_cast<void*>(lua_topointer(state, -1)))->c.f = to;

		lua_pop(state, 3);

		return original;
	};

	lua_getglobal(state, "game");
	core::datamodel = lua_touserdata(state, -1);
	lua_pop(state, 1);

	core::namecall_original = hook_game_metamethod("__namecall", &core::namecall_hook);
	core::index_original = hook_game_metamethod("__index", &core::index_hook);

	register_function(core::namecall_original, "test");

	lua_getglobal(roblox_manager.thread, "require");
	core::old_require = lua_tocfunction(roblox_manager.thread, -1);

	register_function(core::require, "require");

	register_function(core::getobjects, "GetObjects");

	//setup_input( roblox_manager.thread );

	//const auto hook_service_metamethod = [state](const char* const service, const char* const metamethod, const lua_CFunction to) -> lua_CFunction
	//{
	//	lua_getglobal(state, "game");
	//	lua_getfield(state, -1, "GetService");
	//	lua_getglobal(state, "game");
	//	managers::function_mgr.lua_pushstring(state, service);
	//	lua_pcall(state, 2, 1, 0);

	//	lua_getmetatable(state, -1);
	//	lua_getfield(state, -1, metamethod);

	//	const auto original = lua_tocfunction(state, -1);

	//	reinterpret_cast<Closure*>(const_cast<void*>(lua_topointer(state, -1)))->c.f = to;

	//	lua_pop(state, 3);

	//	return original;
	//};

	//struct blocker_t
	//{
	//	lua_CFunction* old_index_method, *old_namecall_method = nullptr;

	//	lua_CFunction index_method, namecall_method = nullptr;
	//};

	////TODO block promptpurchase

	//std::array<std::pair<const char* const, blocker_t>, 4> blockers = {
	//	{ 
	//		{ 
	//			"BrowserService", 
	//			blocker_t { 
	//				&security::browser_service_index, &security::browser_service_namecall,
	//				security::browser_service_index_blocker, security::browser_service_namecall_blocker
	//			}, 
	//		},
	//		{
	//			"GuiService",
	//			blocker_t {
	//				&security::gui_service_index, &security::gui_service_namecall,
	//				security::gui_service_index_blocker, security::gui_service_namecall_blocker
	//			},
	//		},
	//		{
	//			"MarketplaceService",
	//			blocker_t {
	//				&security::marketplace_service_index, &security::marketplace_service_namecall,
	//				security::marketplace_service_index_blocker, security::marketplace_service_namecall_blocker
	//			},
	//		},
	//		{
	//			"MessageBusService",
	//			blocker_t {
	//				&security::messagebus_service_index, &security::messagebus_service_namecall,
	//				security::messagebus_service_index_blocker, security::messagebus_service_namecall_blocker
	//			},
	//		},
	//	}
	//};

	//for (auto& [name, blocker] : blockers)
	//{
	//	*blocker.old_index_method = hook_service_metamethod(name, "__index", blocker.index_method);
	//	*blocker.old_namecall_method = hook_service_metamethod(name, "__namecall", blocker.namecall_method);
	//}
}
