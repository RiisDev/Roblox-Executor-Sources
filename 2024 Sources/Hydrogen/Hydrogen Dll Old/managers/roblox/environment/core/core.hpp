#pragma once

#include <Luau/lfunc.h>
#include <Luau/lua.h>

#include <dependencies/common_includes.hpp>

struct lua_State;

namespace roblox::environment::core
{
	std::int32_t loadstring( lua_State* state );

	std::int32_t get_function_address( lua_State* state );

	std::int32_t getgenv( lua_State* state );

	std::int32_t getrenv( lua_State* state );

	std::int32_t getrawmetatable( lua_State* state );

	std::int32_t setrawmetatable( lua_State* state );

	std::int32_t getreg( lua_State* state );

	std::int32_t setreadonly( lua_State* state );

	std::int32_t isreadonly( lua_State* state );

	std::int32_t is_executor_closure ( lua_State* state );

	std::int32_t identifyexecutor( lua_State* state );

	std::int32_t islclosure( lua_State* state );

	std::int32_t iscclosure( lua_State* state );

	std::int32_t clonefunction( lua_State* state );

	std::int32_t newcclosure( lua_State* state );

	std::int32_t hookfunction( lua_State* state );

	std::int32_t get_namecall_method( lua_State* state );

	std::int32_t set_namecall_method( lua_State* state );

	std::int32_t test( lua_State* state );

	std::int32_t messagebox( lua_State* state );

	std::int32_t getgc( lua_State* state );

	std::int32_t getsenv( lua_State* state );

	std::int32_t checkcaller( lua_State* state );

	std::int32_t hookmetamethod( lua_State* state );

	std::int32_t lz4compress( lua_State* state );

	std::int32_t lz4decompress( lua_State* state );

	std::int32_t setfpscap( lua_State* state );

	std::int32_t getconnections( lua_State* state );

	std::int32_t firesignal( lua_State* state );

	std::int32_t request( lua_State* state );

	std::int32_t getobjects( lua_State* state );

	std::int32_t httpget( lua_State* state );

	std::int32_t iswindowactive( lua_State* state );

	std::int32_t keypress( lua_State* state );

	std::int32_t httpget_async( lua_State* state );

	std::int32_t keyrelease( lua_State* state );

	std::int32_t mousepress1( lua_State* state );

	std::int32_t mouserelease1( lua_State* state );

	std::int32_t mouse1click( lua_State* state );

	std::int32_t mousepress2( lua_State* state );

	std::int32_t mouserelease2( lua_State* state );

	std::int32_t mouse2click( lua_State* state );

	std::int32_t mousescroll( lua_State* state );

	std::int32_t mousemoverel( lua_State* state );

	std::int32_t mousemoveabs( lua_State* state );

	std::int32_t make_writeable( lua_State* state );

	std::int32_t make_readonly( lua_State* state );

	std::int32_t is_readonly( lua_State* state );

	std::int32_t is_writeable( lua_State* state );

	std::int32_t setclipboard( lua_State* state );

	std::int32_t get_calling_script( lua_State* state );

	std::int32_t get_script_bytecode( lua_State* state );

	std::int32_t get_script_closure( lua_State* state );

	std::int32_t get_script_hash( lua_State* state );

	std::int32_t fire_touch_interest( lua_State* state );

	std::int32_t fire_click_detector( lua_State* state );

	std::int32_t get_thread_identity( lua_State* state );

	std::int32_t set_thread_identity( lua_State* state );

	inline std::filesystem::path workspace_path;
	std::int32_t read_file( lua_State* state );

	std::int32_t write_file( lua_State* state );

	std::int32_t append_file( lua_State* state );

	std::int32_t list_files( lua_State* state );

	std::int32_t is_file( lua_State* state );

	std::int32_t is_folder( lua_State* state );

	std::int32_t make_folder( lua_State* state );

	std::int32_t delete_folder( lua_State* state );

	std::int32_t delete_file( lua_State* state );

	std::int32_t load_file( lua_State* state );

	std::int32_t cloneref( lua_State* state );

	std::int32_t get_instances( lua_State* state );

	std::int32_t get_nil_instances( lua_State* state );

	std::int32_t get_scripts( lua_State* state );

	std::int32_t get_running_scripts( lua_State* state );

	std::int32_t get_loaded_modules( lua_State* state );

	std::int32_t is_scriptable( lua_State* state );

	std::int32_t set_scriptable( lua_State* state );

	std::int32_t set_hidden_property( lua_State* state );

	std::int32_t get_hidden_property( lua_State* state );

	std::int32_t require( lua_State* state );

	void check_instance_type( lua_State* state, int idx, const char* err, std::function< bool( const char* ) > check );

	extern void* datamodel;
	extern HWND hwnd;
	extern std::int32_t( *old_require )( lua_State* state );

	extern lua_CFunction namecall_original;
	std::int32_t namecall_hook( lua_State* state );

	extern lua_CFunction index_original;
	std::int32_t index_hook( lua_State* state );
}