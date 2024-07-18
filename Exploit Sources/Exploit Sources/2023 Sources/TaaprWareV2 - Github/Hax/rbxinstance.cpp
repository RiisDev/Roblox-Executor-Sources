#include "rbxstring.h"
#include "rbxinstance.h"

namespace instance {
	objects::instance* FindFirstChild(objects::instance* parent, std::string target_child_name, bool recursive, std::vector<objects::instance*> ignore_list) {
		std::vector<objects::instance*> children = *(parent->children);
		int child_num = 0;
		for (objects::instance* instance : children) {
			// child 0: right, child 1: wrong, child 2: right, child 3: wrong, etc
			// This is because roblox uses shared_ptr which has an extra pointer to ref_count which isnt an instance
			if (++child_num % 2) {
				std::string name = *(instance->name);
				if (name == target_child_name && (std::find(ignore_list.begin(), ignore_list.end(), instance) == ignore_list.end())) {
					return instance;
				} else if (recursive) {
					objects::instance* child = instance::FindFirstChildOfClass(instance, target_child_name, recursive);
					if (child && (std::find(ignore_list.begin(), ignore_list.end(), child) == ignore_list.end())) {
						return child;
					}
				}
			}
		}
		return nullptr;
	}
	objects::instance* FindFirstChildOfClass(objects::instance* parent, std::string target_class_name, bool recursive, std::vector<objects::instance*> ignore_list) {
		if (!parent->children) {
			return nullptr;
		}
		std::vector<objects::instance*> children = *(parent->children);
		int child_num = 0;
		for (objects::instance* instance : children) {
			if (++child_num % 2) {
				std::string name = *(instance->class_descriptor->class_name);
				if (name == target_class_name && (std::find(ignore_list.begin(), ignore_list.end(), instance) == ignore_list.end())) {
					return instance;
				} else if (recursive) {
					objects::instance* child = instance::FindFirstChildOfClass(instance, target_class_name, recursive);
					if (child && (std::find(ignore_list.begin(), ignore_list.end(), child) == ignore_list.end())) {
						return child;
					}
				}
			}
		}
		return nullptr;
	}
	std::vector<objects::instance*> GetChildren(objects::instance* parent, std::string target_class_name) {
		return *(parent->children);
	}
	std::string GetFullName(objects::instance* inst) {
		std::string output = *(inst->name);
		while (inst->parent) {
			output = std::string(".").append(output.c_str());
			output = std::string(inst->name->c_str()).append(output.c_str());
			inst = inst->parent;
		}
		return output;
	}
}

objects::instance* get_datamodel() {
	objects::task_scheduler* task_scheduler = functions::rbx_getscheduler();
	for (std::shared_ptr<objects::job> job : task_scheduler->jobs) {
		std::string name = job->name;
		if (name == std::string(XorString("WaitingHybridScriptsJob"))) {
			auto current_waiting_hybrid_script_job = reinterpret_cast<objects::waiting_hybrid_scripts_job*>(job.get());
			return current_waiting_hybrid_script_job->script_context->parent;
		}
	}
	return NULL;
};
objects::instance* get_scriptcontext() {
	objects::task_scheduler* task_scheduler = functions::rbx_getscheduler();
	for (std::shared_ptr<objects::job> job : task_scheduler->jobs) {
		std::string name = job->name;
		if (name == std::string(XorString("WaitingHybridScriptsJob"))) {
			auto current_waiting_hybrid_script_job = reinterpret_cast<objects::waiting_hybrid_scripts_job*>(job.get());
			return current_waiting_hybrid_script_job->script_context;
		}
	}
	return NULL;
};
std::vector<std::shared_ptr<objects::job>> get_jobs() {
	objects::task_scheduler* task_scheduler = functions::rbx_getscheduler();
	return task_scheduler->jobs;
};