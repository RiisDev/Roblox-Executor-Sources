#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <string_view>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/stopwatch.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <udlib/ud.hpp>
#include "../mem/writable/writable.hpp"
#include "util.hpp"

void util::open_console( )
{
	mem::c_writeable free_console( &FreeConsole );

	*reinterpret_cast< std::uint8_t* >
		( &FreeConsole ) = 0xC3;

	AllocConsole( );
	std::freopen( "conin$", "r", stdin );
	std::freopen( "conout$", "w", stdout );
	std::freopen( "conout$", "w", stderr );

	SetConsoleTitleA( "[hydrogen] -> injector" );
}

std::shared_ptr<spdlog::logger> util::logger( )
{
    static std::shared_ptr< spdlog::logger > dingleton = nullptr;

    if ( !dingleton )
    {
        dingleton = spdlog::stdout_color_mt( "Injector" );
        dingleton->set_level( spdlog::level::debug );
        dingleton->set_pattern( "[%^HYDROGEN-INJ%$] %v" );

        spdlog::set_error_handler( [ ] ( const auto& err ) {
            std::ofstream file( "spdlog-injector-error.log" );

            file << err;
            } );
    }

    return dingleton;
}
