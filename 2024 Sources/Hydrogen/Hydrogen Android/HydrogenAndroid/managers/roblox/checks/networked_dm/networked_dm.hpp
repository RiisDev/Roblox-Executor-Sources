#pragma once

#include <managers/memory/memory.hpp>
#include <managers/roblox/offsets.hpp>
#include <sys/mman.h>

namespace roblox::checks::networked_dm
{
	inline void bypass()
	{
		const auto address = managers::memory_mgr.rebase(offsets::networked_dm_check);

		const auto page_sz = sysconf(_SC_PAGESIZE);

		const auto page_addr = reinterpret_cast<void*>(address & -page_sz);

		mprotect(page_addr, page_sz, PROT_EXEC | PROT_READ | PROT_WRITE);

		*reinterpret_cast<std::uint16_t*>(address) = 0xbf00; //nop

		mprotect(page_addr, page_sz, PROT_READ | PROT_EXEC);
	}
}