#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>

#include "addresses/addresses.hpp"

namespace roblox
{
	const auto base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));

	struct instance_t;
	using virtual_function_table_t = void*;

	struct class_descriptor_t
	{
		virtual_function_table_t virtual_function_table;

		std::string& name;

		std::uint8_t pad_0[40];

		std::uint32_t id;
	};

	struct reference_t
	{
		virtual_function_table_t virtual_function_table;

		std::uint8_t pad_0[8];

		instance_t* instance;
	};

	struct instance_t
	{
		virtual_function_table_t vtable;

		std::uintptr_t self;

		reference_t* reference;

		class_descriptor_t* class_descriptor;

		std::uint8_t pad_0[20];

		std::string& name;

		std::vector<std::pair<instance_t*, reference_t*>>& children;

		std::uint8_t pad_1[4];

		instance_t* parent;
	};

	struct shared_string
	{
		std::uint8_t pad_0[16];

		std::string str;
	};
	
	struct protected_string
	{
		shared_string* source;

		std::uint8_t pad_0[4];

		shared_string* bytecode;

		std::vector<std::uint8_t> decompress_bytecode();
	};

	struct local_script_t : instance_t
	{
		std::uint8_t pad_0[328];

		protected_string script;
	};

	struct module_script_t : instance_t
	{
		std::uint8_t pad_0[308];

		protected_string script;
	};

	struct script_context_t : instance_t
	{
		std::uint8_t pad_0[1164];

		std::uintptr_t loaded_modules;

		std::uint8_t pad_1[824];

		std::uintptr_t loaded_scripts;

		std::vector<local_script_t*> get_running_scripts();

		std::vector<module_script_t*> get_loaded_modules();
	};
	
	struct task_scheduler_t
	{
		struct job_t
		{
			virtual_function_table_t functions;

			std::uint8_t pad_0[12];

			std::string name;

			std::uint8_t pad_1[16];

			double time;

			std::uint8_t pad_2[16];

			double time_spend;

			std::uint8_t pad_3[8];

			std::uintptr_t state;
		};

		struct data_model_job_t : job_t
		{
			std::uint8_t pad_0[192];

			script_context_t* script_context;
		};

		std::uint8_t pad_0[308];

		std::vector<std::pair<job_t*, reference_t*>> jobs;

		roblox::script_context_t* get_script_context();

		static task_scheduler_t* get() { return { *reinterpret_cast<task_scheduler_t**>(base + roblox::addresses::task_scheduler) }; };
	};
}