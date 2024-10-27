#pragma once
#include "spdlog/common.h"
#include "spdlog/logger.h"

#include <memory>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

inline std::shared_ptr< spdlog::logger > logger( )
{
    static std::shared_ptr< spdlog::logger > singleton = nullptr;

    if ( !singleton )
    {
        singleton = spdlog::stdout_color_mt( "Decompiler-main" );
        singleton->set_level( spdlog::level::debug );
        singleton->set_pattern( "[%^DECOMP%$] %v" );
    }

    return singleton;
}