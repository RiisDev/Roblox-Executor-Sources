#include "crash.hpp"

#include <iostream>
#include <iomanip>
#include <unwind.h>
#include <dlfcn.h>
#include <sstream>

#include "../managers.hpp"

#define sizeofa(array) sizeof(array) / sizeof(array[0])

struct state
{
    void** current;
    void** end;
};

static _Unwind_Reason_Code unwind_callback( struct _Unwind_Context* context, void* arg )
{
    state* st = static_cast< state* >( arg );
    uintptr_t pc = _Unwind_GetIP( context );
    if ( pc ) {
        const void* addr = reinterpret_cast< void* >( pc );
        const char* symbol = "";

        Dl_info info;
        if ( dladdr( addr, &info ) && info.dli_sname ) {
            symbol = info.dli_sname;
        }

        std::ostringstream os;
        os << "  #" << std::setw( 2 ) << st->end - st->current << ": " << addr << " (" <<
            reinterpret_cast< void* >( reinterpret_cast< std::uintptr_t >( addr ) - managers::memory_mgr.get_module_base( "libroblox.so" )) << ") " << symbol << "\n";
        managers::utility_mgr.log<ANDROID_LOG_ERROR>( "%s", os.str( ).c_str( ) );

        if ( st->current == st->end ) {
            return _URC_NORMAL_STOP;
        }
        else {
            *st->current++ = reinterpret_cast< void* >( pc );
        }
    }
    return _URC_NO_REASON;
}

void handler( int sig ) {
    managers::utility_mgr.log<ANDROID_LOG_ERROR>( "Signal %d catched.", sig );
    managers::utility_mgr.log<ANDROID_LOG_ERROR>( "A crash has occured. Printing backtrace to logcat." );

    managers::crash_manager_t::log_backtrace( );

	exit( EXIT_FAILURE );
}

void managers::crash_manager_t::log_backtrace( )
{
    char buffer[ 30 ];
    state s = { reinterpret_cast< void** >( &buffer ), reinterpret_cast< void** >( &buffer ) + sizeofa( buffer ) };
    _Unwind_Backtrace( unwind_callback, &s );
}

void managers::crash_manager_t::setup( )
{
	signal( SIGSEGV, handler );	
}
