#include "environment.hpp"

#include <managers/managers.hpp>

#include <dependencies/Luau/lua.h>

#include "core/core.hpp"
#include "debug_lib/debug.hpp"
#include "console/console.hpp"
#include "drawing/drawing.hpp"
#include "cache/cache.hpp"
#include "crypt/crypt.hpp"

void roblox::environment::initiate( )
{
	auto& roblox_manager = managers::mgrs.roblox_mgr;

	const auto register_function = [ & ] ( lua_CFunction function, const char* name, int nups = 0 )
	{
		roblox_manager.pushcclosure( roblox_manager.globals.thread, function, nups );
		lua_setglobal( roblox_manager.globals.thread, name );
	};

	const auto register_member = [ & ] ( lua_CFunction function, const char* name, int nups = 0 )
	{
		roblox_manager.pushcclosure( roblox_manager.globals.thread, function, nups );
		lua_setfield( roblox_manager.globals.thread, -2, name );
	};

	drawing::initiate( );

	register_function( core::loadstring, "loadstring" );
	register_function( core::load_file, "loadfile" );
	register_function( core::get_function_address, "get_function_address" );
	register_function( core::getgenv, "getgenv" );
	register_function( core::getrenv, "getrenv" );
	register_function( core::getrawmetatable, "getrawmetatable" );
	register_function( core::setrawmetatable, "setrawmetatable" );
	register_function( core::getreg, "getreg" );
	register_function( core::setreadonly, "setreadonly" );
	register_function( core::isreadonly, "isreadonly" );
	register_function( core::is_executor_closure, "checkclosure" );
	register_function( core::is_executor_closure, "isexecutorclosure" );
	register_function( core::is_executor_closure, "is_hydrogen_closure" );
	register_function( core::is_executor_closure, "isourclosure" );
	register_function( core::identifyexecutor, "identifyexecutor" );
	register_function( core::islclosure, "islclosure" );
	register_function( core::iscclosure, "iscclosure" );
	register_function( core::clonefunction, "clonefunction" );
	register_function( core::newcclosure, "newcclosure" );
	register_function( core::hookfunction, "hookfunction" );
	register_function( core::messagebox, "messagebox" );
	register_function( core::test, "test" );
	register_function( core::getgc, "getgc" );
	register_function( core::getsenv, "getsenv" );
	register_function( core::get_namecall_method, "getnamecallmethod" );
	register_function( core::set_namecall_method, "setnamecallmethod" );
	register_function( core::checkcaller, "checkcaller" );
	register_function( core::hookmetamethod, "hookmetamethod" );
	register_function( core::getconnections, "getconnections" );
	register_function( core::firesignal, "firesignal" );
	register_function( core::setclipboard, "setclipboard" );
	register_function( core::get_calling_script, "getcallingscript" );
	register_function( core::get_script_bytecode, "getscriptbytecode" );
	register_function( core::get_script_closure, "getscriptclosure" );
	register_function( core::get_script_hash, "getscripthash" );
	register_function( core::fire_touch_interest, "firetouchinterest" );
	register_function( core::fire_click_detector, "fireclickdetector" );
	register_function( core::get_thread_identity, "getthreadidentity" );
	register_function( core::set_thread_identity, "setthreadidentity" );

	register_function( core::cloneref, "cloneref" );
	register_function( core::get_instances, "getinstances" );
	register_function( core::get_nil_instances, "getnilinstances" );
	register_function( core::get_scripts, "getscripts" );

	register_function( core::get_loaded_modules, "getloadedmodules" );
	register_function( core::get_running_scripts, "getrunningscripts" );

	register_function( core::getobjects, "GetObjects" );
	register_function( core::getobjects, "getobjects" );
	register_function( core::httpget, "HttpGet" );

	register_function( core::lz4compress, "lz4compress" );
	register_function( core::lz4decompress, "lz4decompress" );

	register_function( core::setfpscap, "setfpscap" );

	register_function( core::iswindowactive, "iswindowactive" );
	register_function( core::iswindowactive, "isrbxactive" );
	register_function( core::keypress, "keypress" );
	register_function( core::keyrelease, "keyrelease" );

	register_function( core::mouse1click, "mouse1click" );
	register_function( core::mouserelease1, "mouse1release" );
	register_function( core::mousepress1, "mouse1press" );

	register_function( core::mouse2click, "mouse2click" );
	register_function( core::mouserelease2, "mouse2release" );
	register_function( core::mousepress2, "mouse2press" );

	register_function( core::mousescroll, "mousescroll" );
	register_function( core::mousemoverel, "mousemoverel" );
	register_function( core::mousemoveabs, "mousemoveabs" );

	register_function( core::make_readonly, "makereadonly" );
	register_function( core::make_readonly, "make_readonly" );
	register_function( core::make_writeable, "makewriteable" );
	register_function( core::make_writeable, "make_writeable" );
	register_function( core::is_readonly, "is_readonly" );
	register_function( core::is_writeable, "is_writeable" );

	register_function( core::request, "request" );

	register_function( core::set_scriptable, "setscriptable" );

	register_function( core::is_scriptable, "isscriptable" );

	register_function( core::get_hidden_property, "gethiddenproperty" );
	register_function( core::set_hidden_property, "sethiddenproperty" );

	lua_createtable( roblox_manager.globals.thread, 0, 0 );
	{
		register_member( debug::get_upvalue, "getupvalue" );
		register_member( debug::get_upvalues, "getupvalues" );
		register_member( debug::set_upvalue, "setupvalue" );

		register_member( debug::get_proto, "getproto" );
		register_member( debug::get_protos, "getprotos" );

		register_member( debug::get_constant, "getconstant" );
		register_member( debug::get_constants, "getconstants" );
		register_member( debug::set_constant, "setconstant" );

		register_member( debug::get_info, "getinfo" );

		register_member( debug::get_stack, "getstack" );
		register_member( debug::set_stack, "setstack" );
	}
	lua_setglobal( roblox_manager.globals.thread, "debug" );

	lua_createtable( roblox_manager.globals.thread, 0, 0 );
	{
		register_member( cache::invalidate, "invalidate" );
		register_member( cache::replace, "replace" );
		register_member( cache::is_cached, "iscached" );
	}
	lua_setglobal( roblox_manager.globals.thread, "cache" );

	lua_createtable( roblox_manager.globals.thread, 0, 0 );
	{
		register_member( crypt::base_64_decode, "base64decode" );
		register_member( crypt::base_64_encode, "base64encode" );

		register_member( crypt::generatekey, "generatekey" );
		register_member( crypt::generatebytes, "generatebytes" );

		register_member( crypt::encrypt, "encrypt" );
		register_member( crypt::decrypt, "decrypt" );

		register_member( crypt::hash, "hash" );
	}
	lua_setglobal( roblox_manager.globals.thread, "crypt" );

	register_function( console::rconsoleinput, "rconsoleinput" );
	register_function( console::rconsoleprint, "rconsoleprint" );
	register_function( console::rconsoleprint, "printconsole" );
	register_function( console::rconsoleerr, "rconsoleerr" );
	register_function( console::rconsoleinfo, "rconsoleinfo" );
	register_function( console::rconsolewarn, "rconsolewarn" );
	register_function( console::rconsolename, "rconsolename" );
	register_function( console::rconsolename, "rconsolesettitle" );
	register_function( console::rconsoleclear, "rconsoleclear" );
	register_function( console::rconsoleshow, "rconsoleshow" );
	register_function( console::rconsolehide, "rconsolehide" );
	register_function( console::rconsoleshow, "rconsolecreate" );
	register_function( console::rconsolehide, "rconsoledestroy" );

	register_function( core::append_file, "appendfile" );
	register_function( core::write_file, "writefile" );
	register_function( core::read_file, "readfile" );
	register_function( core::delete_file, "delfile" );
	register_function( core::list_files, "listfiles" );
	register_function( core::is_file, "isfile" );
	register_function( core::is_folder, "isfolder" );
	register_function( core::make_folder, "makefolder" );
	register_function( core::delete_folder, "delfolder" );

	const auto state = roblox_manager.globals.thread;
	const auto hook_game_metamethod = [ state ] ( const char* const metamethod, const lua_CFunction to ) -> lua_CFunction
	{
		lua_getglobal( state, "game" );
		lua_getmetatable( state, -1 );
		lua_getfield( state, -1, metamethod );

		const lua_CFunction original = clvalue( index2addr( state, -1 ) )->c.f;
		clvalue( index2addr( state, -1 ) )->c.f = to;

		lua_pop( state, 3 );

		return original;
	};

	lua_getglobal( state, "game" );
	core::datamodel = lua_touserdata( state, -1 );
	lua_pop( state, 1 );

	core::hwnd = FindWindowW( nullptr, L"Roblox" );

	core::namecall_original = hook_game_metamethod( "__namecall", &core::namecall_hook );
	core::index_original = hook_game_metamethod( "__index", &core::index_hook );

	lua_getglobal( roblox_manager.globals.thread, "require" );
	core::old_require = lua_tocfunction( roblox_manager.globals.thread, -1 );

	register_function( core::require, "require" );

	lua_getglobal( roblox_manager.globals.thread, "debug" );

	lua_pushnil( roblox_manager.globals.thread );

	while ( lua_next( roblox_manager.globals.thread, -2 ) )
	{
		const auto key = lua_tostring( roblox_manager.globals.thread, -2 );

		lua_setglobal( roblox_manager.globals.thread, key );
	}
}
