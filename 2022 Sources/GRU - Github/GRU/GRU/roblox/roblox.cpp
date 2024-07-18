#include "roblox.hpp"

std::vector<roblox::local_script_t*> roblox::script_context_t::get_running_scripts()
{
	std::vector<roblox::local_script_t*> ret;

	const auto modules = *reinterpret_cast<std::uintptr_t*>(this->loaded_scripts);
	const auto running_scripts = *reinterpret_cast<std::vector<std::pair<local_script_t*, reference_t*>>*>(modules + 0x40);

	for (const auto& script_ref : running_scripts)
		ret.push_back(script_ref.first);

	return ret;
}

std::vector<roblox::module_script_t*> roblox::script_context_t::get_loaded_modules()
{
	std::vector<roblox::module_script_t*> ret;

	const auto first_module = this->loaded_modules;

	auto current_module = *reinterpret_cast<std::uintptr_t*>(first_module);

	while (first_module != current_module)
	{
		const auto mod = *reinterpret_cast<roblox::module_script_t**>(current_module + 0x10);

		ret.push_back(mod);

		const auto nm = *reinterpret_cast<std::uintptr_t*>(current_module + 8);

		if (*reinterpret_cast<std::uint8_t*>(nm + 13))
		{
			auto i = 0;

			for (i = *reinterpret_cast<std::uintptr_t*>(current_module + 4); !*reinterpret_cast<std::uint8_t*>(i + 13); i = *reinterpret_cast<std::uintptr_t*>(i + 4))
			{
				if (current_module != *reinterpret_cast<std::uintptr_t*>(i + 8))
					break;

				current_module = i;
			}

			current_module = i;
		}
		else
		{
			current_module = *reinterpret_cast<std::uintptr_t*>(current_module + 8);

			for (auto i = *reinterpret_cast<std::uintptr_t*>(nm); !*reinterpret_cast<std::uint8_t*>(i + 13); i = *reinterpret_cast<std::uintptr_t*>(i))
				current_module = i;
		}
	}

	return ret;
}

roblox::script_context_t* roblox::task_scheduler_t::get_script_context()
{
	for (const auto& job : this->jobs)
		if (std::strcmp(job.first->name.c_str(), "LuaGc") == 0)
			return static_cast<roblox::task_scheduler_t::data_model_job_t*>(job.first)->script_context;

	return nullptr;
}

std::vector<std::uint8_t> roblox::protected_string::decompress_bytecode()
{
	auto buffer = this->bytecode->str;

	std::uint8_t hash_key[4]{};
	std::memcpy(hash_key, &buffer[0], 4);

	const auto bytecode_magic = reinterpret_cast<std::uint8_t*>(roblox::base + roblox::addresses::bytecode_magic);

	for (auto i = 0; i < 4; ++i)
		hash_key[i] = (hash_key[i] ^ bytecode_magic[i]) - 41 * i;

	for (auto i = 0; i < buffer.size(); ++i)
		buffer[i] ^= i * 41 + hash_key[i & 3];

	std::uint32_t real_size{};

	std::memcpy(&real_size, &buffer[4], 4);

	const auto zstd_instance = reinterpret_cast<std::uintptr_t(*)(std::uint32_t, std::uint32_t)>(roblox::base + roblox::addresses::create_zstd_instance)(0, 0);

	std::vector<std::uint8_t> destination(real_size);

	const auto destination_source_address = &destination[0];

	const auto compressed_size = buffer.size() - 8;
	const auto compressed_buff = &buffer[8];

	const auto decompress = roblox::base + roblox::addresses::zstd_decompress;

	__asm
	{
		mov ecx, zstd_instance;
		mov edx, destination_source_address;

		push compressed_size;
		push compressed_buff;
		push real_size;

		mov eax, [decompress]
		call eax;

		add esp, 12;
	}

	reinterpret_cast<void(__thiscall*)(std::uintptr_t)>(roblox::base + roblox::addresses::destroy_zstd_instance)(zstd_instance);

	return destination;
}