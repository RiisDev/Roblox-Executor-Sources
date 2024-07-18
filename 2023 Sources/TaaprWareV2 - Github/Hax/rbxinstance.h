#pragma once

#include <string>
#include <vector>
#include "addresses.h"

namespace instance {
	objects::instance* FindFirstChild(objects::instance* parent, std::string target_child_name, bool recursive = false, std::vector<objects::instance*> ignore_list = {});
	objects::instance* FindFirstChildOfClass(objects::instance* parent, std::string target_class_name, bool recursive = false, std::vector<objects::instance*> ignore_list = {});
	std::vector<objects::instance*> GetChildren(objects::instance* parent, std::string target_class_name);
	std::string GetFullName(objects::instance* inst);
}
objects::instance* get_datamodel();
objects::instance* get_scriptcontext();
std::vector<std::shared_ptr<objects::job>> get_jobs();