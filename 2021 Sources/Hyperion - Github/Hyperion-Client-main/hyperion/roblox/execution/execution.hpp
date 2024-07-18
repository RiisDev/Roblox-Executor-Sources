#pragma once
#include "../state/state.hpp"

namespace hyperion::execution
{
	void execute_script(int rstate, const std::string& script, bool spawn, bool compiler);
}