#pragma once
#include "../shared.hpp"

#include <iphlpapi.h>
#pragma comment( lib, "Iphlpapi.lib" )

namespace fingerprinting
{
	struct network_adapter_t
	{
		std::string name;
		std::string desc;
		
		std::string gateway;
		std::string type;

		std::string ip_address;
		std::string mac_address;
	};

	class adapters_t : public element_manager_t
	{
		json_t _data;

		static std::vector< network_adapter_t > get_adapters( );
		static network_adapter_t make_adapter( const IP_ADAPTER_INFO* adapter );

		json_t serialize_adapter( const network_adapter_t adapter );
		
		void serialize_ip( );
		void serialize_all( );

	public:
		adapters_t( );
	};
}