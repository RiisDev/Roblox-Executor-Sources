#include "scheduler.hpp"

#include <common_includes.hpp>

std::vector< roblox::job_t* > roblox::task_scheduler_t::jobs( ) const
{
    std::vector< job_container_t > references{ *reinterpret_cast< job_container_t** >( std::uintptr_t( get( ) ) + off::scheduler::jobs_begin), *reinterpret_cast< job_container_t** >( std::uintptr_t( get( ) ) + off::scheduler::jobs_end) };
    std::vector< job_t* > result;
    result.reserve( references.size( ) );

    std::for_each( references.begin( ), references.end( ), [ &result ]( const job_container_t& ref ) {
        result.push_back( ref.job );
    } );

    return result;
}

roblox::job_t* roblox::task_scheduler_t::get_job( const std::string_view name ) const
{
    for ( const auto job : jobs( ) )
        if ( job->name == name )
            return job;

    return nullptr;
}

std::vector< roblox::job_t* > roblox::task_scheduler_t::find_jobs( const std::string_view name ) const
{
    std::vector< job_t* > result;

    for ( const auto job : jobs( ) )
        if ( job->name.contains( name ) )
            result.push_back( job );

    return result;
}

roblox::task_scheduler_t* roblox::task_scheduler_t::get( )
{
    static const auto self = *reinterpret_cast< task_scheduler_t** >( off::scheduler::self );
    return self;
}