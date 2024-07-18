#pragma once
#include <vector>
#include <queue>
#include "../offsets/offsets.hpp"

extern std::queue<std::string> srcs;

namespace hyperion::taskscheduler
{
	int scheduler_hook(int gcjob);

	static int rstate;

	void init_script_scheduler(int rbstate);
	void unlock_fps(const double fps);
	double get_fps_cap();
	int get_script_context();
	std::vector<std::shared_ptr<hyperion::structs::job>> get_all_jobs();
	std::shared_ptr<hyperion::structs::job> get_job_by_name(const std::string& name);
}
