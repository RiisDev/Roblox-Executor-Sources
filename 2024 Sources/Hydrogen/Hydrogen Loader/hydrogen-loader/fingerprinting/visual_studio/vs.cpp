#include "vs.hpp"

#include <pugixml.hpp>

#include <fstream>
#include <iostream>

std::optional< std::pair< std::filesystem::path, std::size_t > > fingerprinting::visual_studio_t::get_projects_info_path( )
{
	char* temp;

	std::size_t len;

	auto err = _dupenv_s( &temp, &len, "APPDATA" );
	std::unique_ptr< char > value { temp };

	if ( err )
		return std::nullopt;

	const auto directory = std::filesystem::path( value.get( ) ).parent_path( ) / "Local/Microsoft/VisualStudio";

	if ( std::filesystem::exists( directory  ) )
	{
		std::size_t max_size = 0;
		std::filesystem::path file;

		for ( const auto& path : std::filesystem::recursive_directory_iterator( directory ) )
		{
			if ( path.path( ).filename( ) == "ApplicationPrivateSettings.xml" )
			{
				if ( path.file_size( ) > max_size )
				{
					max_size = path.file_size( );
					file = path.path( );
				}
			}
		}

		if ( max_size == 0 )
			return std::nullopt;

		return std::pair{ file, max_size };
	}

	return std::nullopt;
}

std::optional< fingerprinting::json_t > fingerprinting::visual_studio_t::get_projects_json( )
{
	const auto file = get_projects_info_path( );

	if ( !file )
		return std::nullopt;

	const auto& [ path, size ] = *file;

	pugi::xml_document doc;

	const auto status = doc.load_file( path.c_str( ) );
	if ( !status )
		return std::nullopt;

	try
	{
		for ( const auto& data : doc.child( "content" ).child( "indexed" ).children( ) )
		{
			const std::string name = data.attribute( "name" ).value( );

			if ( name == "CodeContainers.Offline" )
			{
				json_t result;

				const auto unfiltered = json_t::parse( data.child_value( "value" ) );
				for ( const auto& elem : unfiltered )
				{
					const auto& project_props = elem[ "Value" ];
					const auto& local_props = project_props[ "LocalProperties" ];

					const std::filesystem::path project_path = local_props[ "FullPath" ].get< std::string >( );
					
					json_t object;
					object[ "project_path" ] = project_path.string( );
					object[ "is_local" ] = project_props[ "IsLocal" ];
					object[ "last_accessed" ] = project_props[ "LastAccessed" ];
					object[ "is_favorite" ] = project_props[ "IsFavorite" ];
					object[ "remote_location" ] = project_props[ "Remote" ];

					result[ project_path.filename( ).string( ) ] = object;
				}

				return result;
			}
		}
	}
	catch ( ... )
	{
		return std::nullopt;
	}

	return std::nullopt;
}

fingerprinting::visual_studio_t::visual_studio_t( )
{
	auto& projects = _object[ "visual_studio_projects" ];

	if ( const auto data = get_projects_json( ) )
		projects = *data;
}
