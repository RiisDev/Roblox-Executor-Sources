#pragma once

#include <sstream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <iomanip>
#include <iostream>
#include "../writer/writer.hpp"
#include "../instruction_conversion/instruction_conversion.hpp"

namespace hyperion::conversion
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