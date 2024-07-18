#pragma once
#include <cstdint>
#include "../state/state.hpp"

namespace hyperion::globals
{
	extern uintptr_t datamodel;
	extern std::uintptr_t gstate;
	extern std::uintptr_t rstate;
	extern std::string lasterror;
}