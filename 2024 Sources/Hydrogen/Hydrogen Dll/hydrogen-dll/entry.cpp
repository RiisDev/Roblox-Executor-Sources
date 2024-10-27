#include <chrono>
#include <iostream>
#include <minhook/minhook.hpp>
#include <rbx/environment/environment.hpp>
#include <rbx/task_scheduler/task_scheduler.hpp>

#include <ui/ui.hpp>

#include <renderer/renderer.hpp>

extern "C" __declspec( dllexport ) void entry( )
{
	std::thread( [ ] 
	{
		using namespace std::chrono_literals;

		MH_Initialize( );

		//std::this_thread::sleep_for( 5s );
		rbx::g_environment.init( 
			rbx::g_task_scheduler[ "WaitingHybridScriptsJob" ] );

		//MH_EnableHook( MH_ALL_HOOKS );

		g_renderer.init( );

		ui::g_ui.create( );

	} ).detach( );
}

bool __stdcall DllMain( void*, const std::uint32_t rsn, void* )
{
	if ( rsn == DLL_PROCESS_ATTACH )
	{
		//MH_Initialize( );

		//rbx::environment::c_env::preinit( );

		//MH_EnableHook( MH_ALL_HOOKS );
	}

	return true;
}