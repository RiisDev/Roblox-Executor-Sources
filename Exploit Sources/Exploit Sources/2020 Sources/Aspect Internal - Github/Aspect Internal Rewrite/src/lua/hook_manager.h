#pragma once
#include "../../libs/sol2/sol.hpp"
#include <map>
#include <set>

struct hook_t
{
	std::string unique;
	sol::protected_function func;
};

class hookmanager_t
{
public:
	inline void register_hook(std::string name, std::string unique, sol::protected_function func)
	{
		hooks[name].push_back(hook_t{ unique, func });
	};

	inline void unregister_hook(std::string name, std::string unique)
	{
		//for (auto& x : hooks[name])
			//if (x.unique == unique)
				//;// hooks[name].erase(x);

	};

	inline auto get_hooks(std::string name)
	{
		return hooks[name];
	};

	std::map < std::string, std::vector < hook_t > > hooks;
};