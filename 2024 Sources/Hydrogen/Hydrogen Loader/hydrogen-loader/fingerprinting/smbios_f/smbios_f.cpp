#include "smbios_f.hpp"

void fingerprinting::smbios_t::serialize_all( )
{
	json_t array;

	parser.enum_tables( [ this, &array ] ( auto table_type, auto handle ) { 
		const auto version = parser.get_version( );

		if ( table_type == smbios::system_information )
		{
			try
			{
				json_t data;

				auto info = parser.get_table_by_handle< smbios::system_information_t >( handle );

				auto& ver = data[ "version" ];
				auto& manufacturer = data[ "manufacturer" ];
				auto& product_name = data[ "product_name" ];
				auto& serial = data[ "serial" ];

				auto& sku = data[ "sku_number" ];
				auto& system_family = data[ "system_family" ];

				if ( version >= 2.4 )
				{
					sku = info[ info->id_sku_number ];
					system_family = info[ info->id_family ];
				}

				if ( version >= 2 )
				{
					manufacturer = info[ info->id_manufacturer ];
					ver = info[ info->id_version ];
					product_name = info[ info->id_product_name ];
					serial = info[ info->id_serial_number ];
				}

				array.push_back( data );
			}
			catch ( ... )
			{
				// ...
			}
		}
	} );

	_object[ "smbios" ] = array;
}

fingerprinting::smbios_t::smbios_t( )
{
	serialize_all( );
}
