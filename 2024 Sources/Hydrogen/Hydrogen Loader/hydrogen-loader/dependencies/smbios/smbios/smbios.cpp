#include "smbios.h"

#include <Windows.h>

namespace fi {
	namespace smbios {
		smbios_parser::smbios_parser( ) {
			parse_smbios( );
		}
		
		void smbios_parser::enum_tables( std::function< void( std::uint8_t table_type, detail::entry_handle entry_handle ) > enum_fn ) {
			for ( auto table_entries : m_tables ) {
				//						Container of tables with the same type
				for ( auto type_table : table_entries.second  ) {
					//			Table type				Formatted section pointer
					enum_fn( table_entries.first, type_table.handle );
				}
			}
		}

		double smbios_parser::get_version( ) {
			return m_smbios_version;
		}

		void smbios_parser::parse_smbios( ) {
			auto size = GetSystemFirmwareTable( 'RSMB', 0, nullptr, 0 );

			if ( !size )
				throw std::exception( "smbios::parse_smbios: failed to get SMBIOS table size" );

			void* buffer = malloc( size );

			if ( !buffer )
				throw std::exception( "smbios::parse_smbios: failed to allocate buffer" );

			if ( GetSystemFirmwareTable( 'RSMB', 0, buffer, size ) != size )
				throw std::exception( "smbios::parse_smbios: GetSystemFirmwareTable failed" );

			auto smbios = reinterpret_cast< smbios_t* >( buffer );

			save_smbios_version( smbios );

			if ( smbios->length <= sizeof( smbios_t ) )
				throw std::exception( "smbios::parse_smbios: SMBIOS table has no entries" );

			// First table
			detail::entry_handle handle = 0;
			auto table_header = reinterpret_cast< table_header_t* >( std::uintptr_t( buffer ) + sizeof( smbios_t ) );
			do {
				std::uint8_t* string_table = reinterpret_cast< std::uint8_t* >( std::uintptr_t( table_header ) + table_header->length );

				// Parse string table
				detail::table_string_container table_strings = { };
				auto string_table_size = parse_string_table( string_table, table_strings );

				// Add the table into our parsed tables
				// Some tables have multiple entries, which is why we use a container
				m_tables[ table_header->type ].emplace_back( table_header, handle, table_strings );

				// Go to next entry
				table_header = reinterpret_cast< table_header_t* >( string_table + string_table_size );

				// Increase the handle
				handle++;
			} while ( table_header->type != table_types::end_of_table || table_header->length != 4 );

			free( buffer );
		}

		void smbios_parser::save_smbios_version( smbios_t* smbios ) {
			char version_string[ 8 ] = { 0 };
			snprintf( version_string, std::size( version_string ), "%i.%i", smbios->major_version, smbios->minor_verion );

			m_smbios_version = std::stod( version_string );
		}

		std::uint8_t smbios_parser::parse_string_table( std::uint8_t* string_table, detail::table_string_container& out ) {
			std::string current_string = "";
			std::uint8_t size = 0, string_count = 1;

			for ( ; ; size++, string_table++ ) {
				if ( *string_table )
					out[ string_count ].push_back( *string_table );
				else // String end is marked with a null byte
					string_count++;

				// End of structure is marked by 2x null bytes (end-of-string + additional null terminator)
				if ( ( *string_table | *( string_table + 1 ) ) == 0 )
					break;
			}

			return size + 2; // Account for the 2 extra bytes
		}
	} // namespace smbios
} // namespace fi