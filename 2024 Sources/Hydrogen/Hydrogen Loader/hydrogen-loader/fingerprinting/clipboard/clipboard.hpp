#pragma once
#include "../shared.hpp"

#include <optional>
#include <string>

namespace fingerprinting
{
	class clipboard_t : public element_manager_t
	{
		static std::optional< std::string > get_clipboard( );

	public:
		clipboard_t( );
	};
}