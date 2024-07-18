#pragma once
#include "../../state/state.hpp"
#include <unordered_map>

namespace hyperion::bypasses::callcheck
{
	static int key = 50;

	extern std::unordered_map<int, int> closure_map;

	extern std::unordered_map<int, StkId> newcclosure_map;

	int function_handler(std::uintptr_t rstate);

	LONG __stdcall exception_handler(PEXCEPTION_POINTERS exception);

	void start_callcheck_bypass(state rstate);

	void push_function(state rstate, std::uintptr_t address, int upvals); 
}

