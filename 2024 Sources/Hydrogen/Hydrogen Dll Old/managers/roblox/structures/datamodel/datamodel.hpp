#pragma once

#include <dependencies/common_includes.hpp>

#include "../instance/instance.hpp"

namespace roblox::structures
{
	struct datamodel_t : public instance_t
	{
		static datamodel_t* get( );
	};
}