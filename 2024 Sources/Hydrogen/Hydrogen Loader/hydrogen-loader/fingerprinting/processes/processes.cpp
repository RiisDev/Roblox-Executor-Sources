#include "processes.hpp"

#include <psapi.h>

#pragma comment( lib, "psapi.lib" )

std::vector<PROCESSENTRY32> fingerprinting::processes_t::get_processes( )
{
    std::vector<PROCESSENTRY32> result;

    const std::unique_ptr< void, decltype( &CloseHandle ) > snapshot { CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ), &CloseHandle };

    if ( snapshot.get( ) == INVALID_HANDLE_VALUE )
        return result;

    PROCESSENTRY32 proc;
    proc.dwSize = sizeof( proc );

    if ( !Process32First( snapshot.get( ), &proc ) )
        return result;

    do
        result.push_back( proc );
    while ( Process32Next( snapshot.get( ), &proc ) );

    return result;
}

fingerprinting::json_t fingerprinting::processes_t::serialize_process( const PROCESSENTRY32& proc )
{
    json_t process_body;

    auto& full_path = process_body[ "file_path" ];

    process_body[ "pid" ] = proc.th32ProcessID;
    process_body[ "thread_count" ] = proc.cntThreads;

    full_path = proc.szExeFile;

    if ( const auto handle = OpenProcess( PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, proc.th32ProcessID ); handle != INVALID_HANDLE_VALUE )
    {
        char path[ MAX_PATH ];

        if ( const auto size = GetProcessImageFileNameA( handle, path, MAX_PATH ) )
            full_path = std::string( path, size );


        CloseHandle( handle );
    }

    return process_body;
}

fingerprinting::processes_t::processes_t( )
{
    json_t array;

    for ( const auto& proc : get_processes( ) )
        array.push_back( serialize_process( proc ) );

    _object[ "processes" ] = array;
}