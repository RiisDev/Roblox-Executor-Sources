#pragma once
#include "../shared.hpp"

#include <smbios/smbios.h>

namespace fingerprinting
{
	namespace smbios = fi::smbios;

	class smbios_t : public element_manager_t
	{
		smbios::smbios_parser parser;

		void serialize_all( );

	public:

		smbios_t( );
	};
}