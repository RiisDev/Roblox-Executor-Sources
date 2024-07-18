#pragma once

#include <string>

namespace Scheduler {
	uintptr_t get_job(const char* job_name) noexcept;

	uintptr_t get_script_context();
};
