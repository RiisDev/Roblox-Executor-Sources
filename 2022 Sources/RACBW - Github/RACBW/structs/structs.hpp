#pragma once

#include <cstdint>

struct scan_container_t
{
	enum class status_t : std::int32_t
	{
		queued = -1,
		scanning,
		whitelisted,
		finished
	} status;

	std::uint32_t address, size;
};