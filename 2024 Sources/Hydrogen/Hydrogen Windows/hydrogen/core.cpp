#include <iostream>
#include <thread>

#include <Windows.h>

#include <common_includes.hpp>

#include <roblox/scheduler/scheduler.hpp>
#include <roblox/roblox.hpp>

extern "C" __declspec( dllexport ) void entry( )
{
    std::thread( [ ]( ) 
    {
        std::this_thread::sleep_for( std::chrono::seconds( 5 ) );

        roblox::rbx_manager_t::get( ).initiate( );
    } ).detach( );
}

bool __stdcall DllMain( void*, const std::uint32_t reason, void* )
{
    //if ( reason == DLL_PROCESS_ATTACH )
    //{
    //    std::thread( [ ] ( )
    //        {
    //            auto free_console = &FreeConsole;

    //            DWORD op;
    //            VirtualProtect( free_console, 1, PAGE_EXECUTE_READWRITE, &op );
    //            *reinterpret_cast< std::uint8_t* >
    //                ( &FreeConsole ) = 0xC3;
    //            VirtualProtect( free_console, 1, op, &op );

    //            AllocConsole( );
    //            std::freopen( "conin$", "r", stdin );
    //            std::freopen( "conout$", "w", stdout );
    //            std::freopen( "conout$", "w", stderr );

    //            SetConsoleTitleA( "[hydrogen]" );

    //            roblox::rbx_manager_t::get( ).initiate( );
    //        } ).detach( );
    //}
    return true;
}