#pragma once

#include <rbx/rbx.hpp>

namespace rbx
{
	class c_task_scheduler
	{
	public:
		[[ nodiscard ]] task_scheduler::structs::job_t*
			operator[]( const std::string_view name ) const;
	} const g_task_scheduler;
}