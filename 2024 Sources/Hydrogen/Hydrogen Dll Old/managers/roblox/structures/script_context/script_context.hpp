#pragma once

#include <dependencies/common_includes.hpp>

#include "../datamodel/datamodel.hpp"

namespace roblox::structures
{
	struct script_context_t : public instance_t
	{
		static script_context_t* get( );
	};
}