#include <common_includes.hpp>

#include "utilities/io.hpp"

#include "memory/hook.hpp"

#include <managers/managers.hpp>

#include <managers/roblox/environment/environment.hpp>
#include <managers/roblox/memcheck/memcheck.hpp>
#include <managers/roblox/boundary/boundary.hpp>
#include <managers/roblox/trustcheck/trustcheck.hpp>

#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/luacode.h>
#include <ExceptionHandler/ExceptionHandler.hpp>

#include <roblox/shuffle.hpp>

void enable_luau_flags( )
{
	for ( auto flag = Luau::FValue<bool>::list; flag; flag = flag->next )
		if ( std::strncmp( flag->name, "Luau", 4 ) )
			flag->value = true;
}

void entry( )
{
	enable_luau_flags( );
	using namespace utilities::io;

	initiate( "Hydrogen" );
	
	log( "Initializing..." );

	roblox::memcheck::initiate( );

	log( "Memcheck hooked" );

	roblox::boundary::initiate( );

	log( "Boundary patched" );

	////roblox::trustcheck::initiate( );

	log( "Trustcheck patched" );

	managers::mgrs.interface_mgr.initiate( );

	log( "Renderer hooked" );

	managers::mgrs.roblox_mgr.initiate( );

	log( "Scheduler binded" );

	std::printf("%p\n", managers::mgrs.roblox_mgr.globals.state);

	std::printf("%i\n", lua_getglobal(managers::mgrs.roblox_mgr.globals.state, "game"));

	managers::mgrs.roblox_mgr.setup_main_thread( );

	managers::mgrs.roblox_mgr.set_state_identity( 8 );

	roblox::environment::initiate( );

	log( "Environment setup" );

	log( log_options_t::log, "Initialized!" );
}

bool __stdcall DllMain( HMODULE mod, DWORD reason, void* )
{
	if ( reason == DLL_PROCESS_ATTACH )
	{
		setup_ehandler( mod );

		char dll_path[ MAX_PATH ];
		GetModuleFileNameA( mod, dll_path, MAX_PATH );

		const auto path = std::filesystem::path( dll_path ).parent_path( ) / "workspace";
		if ( !std::filesystem::exists( path ) )
			std::filesystem::create_directory( path );

		roblox::environment::core::workspace_path = path;
		
		std::thread{ entry }.detach( );
	}
	return true;
}