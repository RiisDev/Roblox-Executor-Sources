#pragma once
#include "../shared.hpp"

#include <filesystem>
#include <optional>

namespace fingerprinting
{
	class visual_studio_t : public element_manager_t
	{
		static std::optional< std::pair< std::filesystem::path, std::size_t > > get_projects_info_path( );
		static std::optional< json_t > get_projects_json( );

	public:
		visual_studio_t( );
	};
}