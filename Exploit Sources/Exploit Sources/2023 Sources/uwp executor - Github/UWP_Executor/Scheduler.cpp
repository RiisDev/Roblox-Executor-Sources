#include <string>
#include <stdexcept>

#include "Scheduler.hpp"
#include "Roblox.hpp"

uintptr_t Scheduler::get_job(const char* job_name) noexcept {
	// Get the scheduler.
	const auto scheduler = Roblox::get_scheduler();

	// Get the start and end address of the job-vector.
	auto jobs_start = *reinterpret_cast<uintptr_t*>(scheduler + 0x134);
	const auto jobs_end = *reinterpret_cast<uintptr_t*>(scheduler + 0x138);

	// Loop through the job pointers (each 8 bytes apart).
	for (; jobs_start < jobs_end; jobs_start += 8u) {
		const auto job = *reinterpret_cast<uintptr_t*>(jobs_start);

		// If the name matches, return the job.
		if (*reinterpret_cast<std::string*>(job + 0x10) == job_name)
			return job;
	}

	// Couldn't find the job, return null.
	return NULL;
}

uintptr_t Scheduler::get_script_context() {
	// Get the WaitingHybridScriptsJob.
	const auto job = Scheduler::get_job("WaitingHybridScriptsJob");

	// If it couldn't find the job, throw an exception.
	if (job == NULL)
		throw std::runtime_error("Couldn't find WaitingHybridScriptsJob");

	// Return the script context.
	return *reinterpret_cast<uintptr_t*>(job + 0x138);
}
