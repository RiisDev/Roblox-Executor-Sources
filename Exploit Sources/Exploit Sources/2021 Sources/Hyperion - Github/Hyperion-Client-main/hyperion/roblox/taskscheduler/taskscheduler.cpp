#include "taskscheduler.hpp"
#include "../execution/execution.hpp"

#include <Windows.h>

#include "../vmthook/vmthook.hpp"
#include "../environment/environment.hpp"

vmthook* vt_hook;
std::queue<std::string> srcs = {};
bool hooked = false;
bool hook_namecall = false;

int hyperion::taskscheduler::scheduler_hook(int gcjob)
{
	if (!hooked)
	{
		hooked = true;
		hyperion::environment::init_index_hook(rstate);
	}
	if (!hook_namecall)
	{
		hook_namecall = true;
		hyperion::environment::init_namecall_hook(rstate); //no u until u fix the damn yield
	}

	if (!srcs.empty())
	{
		hyperion::execution::execute_script(rstate, srcs.front(), true, false);
		srcs.pop();
	}

	return 0;
}

const std::string& load = "script = Instance.new'LocalScript' script.Disabled = true script.Name = 'hyp' script.Parent = nil";

void hyperion::taskscheduler::init_script_scheduler(int rbstate)
{
	rstate = rbstate;
	std::shared_ptr<hyperion::structs::job> waiting_scripts = hyperion::taskscheduler::get_job_by_name("WaitingScriptJob");
    uintptr_t vtable = reinterpret_cast<uintptr_t>(waiting_scripts.get());

    vt_hook = new vmthook(reinterpret_cast<void*>(vtable));
    vt_hook->hook_func(static_cast<void*>(scheduler_hook), 1);
	hooked = false;
	hook_namecall = false;
	srcs.push(load);
}

void hyperion::taskscheduler::unlock_fps(const double fps)
{
	std::uintptr_t task_scheduler = hyperion::functions::get_task_scheduler();
	const double limit = 1.0 / fps;

	*reinterpret_cast<double*>(task_scheduler + 0x110) = limit;
}

double hyperion::taskscheduler::get_fps_cap()
{
	std::uintptr_t task_scheduler = hyperion::functions::get_task_scheduler();
	const double limit = 1.0 / *reinterpret_cast<double*>(task_scheduler + 0x110);

	return limit;
}

int hyperion::taskscheduler::get_script_context()
{
	std::shared_ptr<hyperion::structs::job> scripts_job = hyperion::taskscheduler::get_job_by_name("WaitingScriptJob");

	int sc = *reinterpret_cast<int*>(reinterpret_cast<int>(scripts_job.get()) + 0x130);

	std::cout << std::hex << sc << std::endl;

	return sc;
}

//TODO: find better method to detect when jobs are created
std::vector<std::shared_ptr<hyperion::structs::job>> hyperion::taskscheduler::get_all_jobs()
{
	std::uintptr_t task_scheduler = hyperion::functions::get_task_scheduler();
	//std::cout << std::hex << task_scheduler << std::endl;
	while (*reinterpret_cast<int*>(task_scheduler) < 160) //when task scheduler start, it runs a timer
		Sleep(100);
	//std::cout << "finished" << std::endl;

	std::vector<std::shared_ptr<hyperion::structs::job>> jobs;
	hyperion::functions::get_jobs(&jobs);

	return jobs;
}

std::shared_ptr<hyperion::structs::job> hyperion::taskscheduler::get_job_by_name(const std::string& name)
{
	std::vector<std::shared_ptr<hyperion::structs::job>> jobs = hyperion::taskscheduler::get_all_jobs();

	for (int i = 0; i < jobs.size(); i++)
	{
		if (jobs[i]->name == name)
		{
			return jobs[i];
		}
	}
}
