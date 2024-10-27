#pragma once

#define SPDLOG_USE_STD_FORMAT

#include <memory>
#include <fstream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/stopwatch.h>
#include <spdlog/fmt/bin_to_hex.h>

#include <roblox/memory/offsets.hpp>

inline std::shared_ptr< spdlog::logger > logger( )
{
    static std::shared_ptr< spdlog::logger > dingleton = nullptr;

    if ( !dingleton )
    {
        dingleton = spdlog::stdout_color_mt( "Hydrogen" );
        dingleton->set_level( spdlog::level::debug );
        dingleton->set_pattern( "[%^HYDROGEN%$] %v" );

        spdlog::set_error_handler( [ ]( const auto& err ) {
            std::ofstream file( "spdlog-error.log" );

            file << err;
        } );
    }

    return dingleton;
}