#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/stopwatch.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <fstream>
#include <memory>

namespace util
{
    class c_cio
    {
        const bool debug = true;
        bool console_state = debug;
        std::shared_ptr< spdlog::logger > logger = nullptr;

    public:
        void init_console( );
        void open( );
        void close( );
        void set_color( int attribute );
        void default_pattern( );

        std::shared_ptr<spdlog::logger> log( );

        c_cio( )
        {
            logger = spdlog::stdout_color_mt( "Injector" );
            logger->set_level( spdlog::level::debug );
            logger->set_pattern( "[%^HYDROGEN%$] %v" );

            spdlog::set_error_handler( [ ] ( const auto& err ) {
                std::ofstream file( "spdlog-injector-error.log" );

                file << err;
                } );
        }

    } inline g_cio;
}