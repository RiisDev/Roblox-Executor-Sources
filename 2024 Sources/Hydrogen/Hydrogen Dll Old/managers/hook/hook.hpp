#pragma once

#include <dependencies/common_includes.hpp>

struct job_t;

namespace managers
{
    template<typename fn_def>
    class hook_t
    {
    public:
        fn_def original_fn;

        void hook_virtual_function( std::pair<std::uintptr_t, void*> addresses, std::pair<std::size_t, std::size_t> info )
        {
            const auto [object_address, function_address] = addresses;
            const auto [index, sz] = info;

            auto virtual_function_table = new void* [ sz ];

            std::memcpy( virtual_function_table, *reinterpret_cast< std::uintptr_t** >( object_address ), sz * sizeof( void* ) );

            original_fn = reinterpret_cast< fn_def >( virtual_function_table [ index ] );

            virtual_function_table [ index ] = function_address;

            *reinterpret_cast< std::uintptr_t* >( object_address ) = reinterpret_cast< std::uintptr_t >( virtual_function_table );
        }

        template<typename... args_t>
        auto call_original( args_t... args ) -> std::invoke_result_t<fn_def, args_t...>
        {
            return original_fn( args... );
        }
    };

    class hook_manager_t
    {
    public:
        struct
        {
            hook_t < std::uint32_t( __fastcall* )( const job_t* job, std::uintptr_t, const std::uintptr_t stats )> on_waiting_script_job_step;
        } hooks;
    };
}