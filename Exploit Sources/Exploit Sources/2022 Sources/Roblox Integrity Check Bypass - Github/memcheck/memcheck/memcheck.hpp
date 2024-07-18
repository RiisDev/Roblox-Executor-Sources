#pragma once

#include <cstdint>
#include <Windows.h>
#include <string>
#include <vector>

constexpr bool debug = true;

namespace memcheck
{
	const auto base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));

	const std::uintptr_t client_replicator_vtable = base + 0x2691FEC;
	const std::uintptr_t m_base = base + 0x62C88E;
	const auto chunk_start = reinterpret_cast<std::uintptr_t*>(base + 0x2E56038);
	const auto size_start = reinterpret_cast<std::uintptr_t*>(base + 0x2E560B0);
	const std::uintptr_t core_hasher_seed = base + 0x2F182A8;
	const std::uintptr_t seed_address = base + 0x2B9E28C;
	const std::uintptr_t core_hasher = base + 0x22C890;

	using get_task_scheduler_t = std::uintptr_t(*)();
	const auto get_task_scheduler = reinterpret_cast<get_task_scheduler_t>(base + 0x0DBA560);

	struct job_t
	{
		std::uintptr_t* functions;
		byte pad0[12];
		std::string name;
		byte pad1[16];
		double time;
		byte pad2[16];
		double time_spend;
		byte pad3[8];
		std::uintptr_t state;
	};

	struct chunk_t
	{
		std::uintptr_t start;
		std::uint32_t size;
		std::uint32_t hash;
		std::uint32_t core_hash;

		chunk_t() = default;
		chunk_t(std::uintptr_t start, std::uint32_t size, std::uint32_t hash, std::uint32_t core_hash) : start{ start }, size{ size }, hash{ hash }, core_hash{ core_hash } {}
	};

	inline std::uintptr_t get_job(const char* job_name)
	{
		const auto task_scheduler = get_task_scheduler();

		if (!task_scheduler)
			return 0;

		const auto job_start = *reinterpret_cast<std::uintptr_t*>(task_scheduler + 0x134);

		const auto last_element = *reinterpret_cast<std::uintptr_t*>(task_scheduler + 0x138);

		for (auto job = job_start; job != last_element; job += 0x8)
		{
			const auto current_instance = *reinterpret_cast<job_t**>(job);

			if (!current_instance)
				continue;

			if (current_instance->name == job_name)
				return reinterpret_cast<std::uintptr_t>(current_instance);
		}

		return 0;
	}

	inline std::uintptr_t get_client_replicator()
	{
		static std::uintptr_t client_replicator = 0;

		if (client_replicator)
			return client_replicator;

		std::uintptr_t addr = 0;

		MEMORY_BASIC_INFORMATION mbi;

		while (VirtualQuery(reinterpret_cast<std::uint32_t*>(addr), &mbi, sizeof(mbi)))
		{
			if (addr && mbi.AllocationProtect == PAGE_READWRITE && mbi.State == MEM_COMMIT && mbi.RegionSize > 0x15000)
			{
				for (auto c_addr = addr; c_addr < mbi.RegionSize + addr; c_addr += sizeof(std::uintptr_t))
				{
					const auto current_address = *reinterpret_cast<std::uintptr_t*>(c_addr);

					if (current_address == client_replicator_vtable)
					{
						client_replicator = c_addr;

						return client_replicator;
					}
				}
			}

			addr += mbi.RegionSize;
		}

		return 0;
	}

	enum class operation : std::uint8_t
	{
		xor_t = 0x33,
		add_t = 0x03,
		sub_t = 0x2B,
	};

	operation determine_operation(std::uintptr_t silent_hasher);

	std::uintptr_t* populate_hash_list();

	std::uintptr_t get_active_silent_hasher();

	void initiate();
}