#pragma once

#include <dependencies/common_includes.hpp>

#include <dlfcn.h>

namespace managers
{
    class memory_manager_t
    {
    public:
        std::uintptr_t get_module_base(const char* const name);

        template<typename ret_t = std::uintptr_t>
        ret_t rebase(std::uintptr_t address)
        {
            static const auto module_base = get_module_base("libroblox.so");

            std::uintptr_t addr = module_base + address;

            return *reinterpret_cast<ret_t*>(&addr);
        }

		template<typename ret_t = std::uintptr_t>
        ret_t irebase( std::uintptr_t address )
        {
            static const auto module_base = get_module_base( "libroblox.so" );

			std::uintptr_t addr = address - module_base;

            return *reinterpret_cast< ret_t* >( &addr );
        }
    };

    extern memory_manager_t memory_mgr;
}

