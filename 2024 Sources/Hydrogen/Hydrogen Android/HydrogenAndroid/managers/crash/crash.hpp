#pragma once

#include <dependencies/common_includes.hpp>

namespace managers
{
    class crash_manager_t
    {
    public:
        crash_manager_t( ) = default;

        static void log_backtrace( );

        void setup( );
    };

    extern crash_manager_t crash_mgr;
}

