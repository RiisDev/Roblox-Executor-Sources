#pragma once

#include <sstream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <iomanip>
#include <iostream>
#include "writer.h"

#include "LuauTranspiler.h"

namespace pandatranspiler
{
	enum class types : std::uint8_t
	{
		nil = 0,
		bl,
		num,
		str,
		global,
		table
	};

	std::string script_to_bytecode(lua_State* ls, const std::string& script, bool& compiled);
}