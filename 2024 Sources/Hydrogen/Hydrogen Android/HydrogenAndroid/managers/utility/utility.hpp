#pragma once

#include <dependencies/common_includes.hpp>

namespace managers
{
    class utility_manager_t
    {
        bool debug;

    public:
        utility_manager_t(bool debug);

        template< android_LogPriority log_type_t = ANDROID_LOG_INFO, typename ... t >
        void log(const char* const format, t ... args)
        {
            __android_log_print(log_type_t, "[Hydrogen]", format, args...);
        }

    };

    extern utility_manager_t utility_mgr;
}

