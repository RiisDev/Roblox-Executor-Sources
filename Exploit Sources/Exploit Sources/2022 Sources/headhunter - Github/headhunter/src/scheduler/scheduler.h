#pragma once
#include <Windows.h>
#include <string>
#include <vector>

class scheduler_t
{
private:
	std::uintptr_t taskscheduler = 0;
	std::uintptr_t datamodel = 0;
	std::uintptr_t script_context = 0;
public:
	explicit scheduler_t();
	std::uintptr_t get() const;
	std::uintptr_t get_datamodel() const;
	std::uintptr_t get_script_context() const;
	std::uintptr_t get_global_luastate() const;
	std::vector<std::uintptr_t> get_jobs() const;
	std::uintptr_t get_job_by_name(const std::string& name) const;
	void print_jobs() const;
	void hook_waiting_scripts_job(void* hook, std::uintptr_t& original_func) const;
};