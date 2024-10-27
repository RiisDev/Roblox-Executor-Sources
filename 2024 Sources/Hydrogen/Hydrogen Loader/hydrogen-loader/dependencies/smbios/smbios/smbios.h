#pragma once

#ifndef SMBIOS_H
#define SMBIOS_H

#include "structs.h"

#include <unordered_map>
#include <functional>
#include <algorithm>

namespace fi {
	namespace smbios {
		class smbios_parser {
		public:
			smbios_parser( );

			void enum_tables( std::function< void( std::uint8_t table_type, detail::entry_handle entry_handle ) > enum_fn );

			double get_version( );

			template < typename T >
			detail::smbios_table< T > get_table_by_handle( detail::entry_handle handle ) {
				for ( auto& table_type : m_tables ) {
					for ( auto& table_entry : table_type.second ) {
						if ( table_entry.handle == handle )
							return detail::smbios_table< T >( table_entry );
					}
				}

				return { };
			}

		private:
			void parse_smbios( );
			void save_smbios_version( smbios_t* smbios );

			std::uint8_t parse_string_table( std::uint8_t* string_table, detail::table_string_container& out );

			double m_smbios_version = 0;

			std::unordered_map< std::uint8_t, std::vector< detail::smbios_table_entry_t > > m_tables = { };
		};
	} // namespace smbios
} // namespace fi

#endif // SMBIOS_STRUCT_H