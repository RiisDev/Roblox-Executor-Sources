#pragma once

#include <managers/memory/memory.hpp>
#include <managers/roblox/offsets.hpp>
#include <sys/mman.h>

namespace roblox::checks::trust_check
{
	inline void bypass()
	{
		constexpr auto nop_sz = 0x3E;

		const auto address = managers::memory_mgr.rebase(offsets::trust_check);

		const auto page_sz = sysconf(_SC_PAGESIZE);

		const auto page_addr = reinterpret_cast<void*>(address & -page_sz);

		mprotect(page_addr, page_sz, PROT_EXEC | PROT_READ | PROT_WRITE);

		std::memset(reinterpret_cast<void*>(address), 0xbf00, nop_sz / 2);

		mprotect(page_addr, page_sz, PROT_READ | PROT_EXEC);
	}
}