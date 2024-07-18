#pragma once
#include "../offsets/offsets.hpp"

namespace hyperion::datamodel
{
	std::uintptr_t get_datamodel();
	std::uintptr_t get_instance();
	void wait_for_gameloaded();
	bool is_gameloaded();
}