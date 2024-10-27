#include <span>
#include "task_scheduler.hpp"

using namespace rbx::task_scheduler;
using c_task_scheduler = rbx::c_task_scheduler;

structs::job_t* c_task_scheduler::operator[]
	( const std::string_view name ) const
{
	static const auto task_scheduler 
		= *task_scheduler::offs::self;
	
	const auto start 
		= *reinterpret_cast< structs::job_container_t** >
			( task_scheduler + offs::jobs_start );

	const auto end 
		= *reinterpret_cast< structs::job_container_t** >
			( task_scheduler + offs::jobs_end );

	std::span< structs::job_container_t > 
		jobs( start, ( end - start ) );

	const auto container = std::ranges::find_if( jobs, 
		[ & ]( const structs::job_container_t& c ) 
			{ return c.m_job->m_name == name; } );

	return container->m_job;
}