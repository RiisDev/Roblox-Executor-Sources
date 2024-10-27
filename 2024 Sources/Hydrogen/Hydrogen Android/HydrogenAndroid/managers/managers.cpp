#include "managers.hpp"

managers::hook_manager_t managers::hook_mgr;
managers::jni_manager_t managers::jni_mgr;
managers::memory_manager_t managers::memory_mgr;
managers::roblox_manager_t managers::roblox_mgr;
managers::function_manager_t managers::function_mgr;
managers::utility_manager_t managers::utility_mgr{ true };
managers::renderer_manager_t managers::renderer_mgr;
managers::crash_manager_t managers::crash_mgr;
managers::drawing_manager_t managers::drawing_mgr;
managers::menu_manager_t managers::menu_mgr;

void managers::initiate()
{
	managers::crash_mgr.setup( );

	managers::roblox_mgr.initiate( );
	managers::renderer_mgr.initiate( );

	managers::utility_mgr.log( "Managers Initiated\n" );
}
