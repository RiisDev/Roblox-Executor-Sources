#pragma once
#include "../shared.hpp"

namespace fingerprinting
{
	class general_t : public element_manager_t
	{
		json_t general_data;

		void serialize_account_name( );
		void serialize_directory_info( );

	public:
		general_t( );
	};
}