#include "ida.hpp"
 
fingerprinting::ida_history_t fingerprinting::ida_t::get_history( )
{
    ida_history_t result;

    const std::wstring reg_path = L"SOFTWARE\\Hex-Rays\\IDA";

    wr::RegKey key { HKEY_CURRENT_USER, reg_path };

    if ( key.IsValid( ) )
    {
        const auto functor = [ &result, reg_path ] ( const std::wstring& sub ) {
            const auto path = reg_path + L"\\" + sub;

            const wr::RegKey key { HKEY_CURRENT_USER, path };

            for ( const auto& [i, _] : key.EnumValues( ) )
            {
                const auto path = key.TryGetStringValue( i );

                if ( path.IsValid( ) )
                {
                    const auto& value = path.GetValue( );

                    result.history.push_back( std::string( value.begin( ), value.end( ) ) );
                }
            }

            for ( const auto& sub : key.EnumSubKeys( ) )
            {
                const std::string ascii_sub( sub.begin( ), sub.end( ) );
                const wr::RegKey key { HKEY_CURRENT_USER, path + L"\\" + sub };

                for ( const auto& [i, _] : key.EnumValues( ) )
                {
                    const auto path = key.TryGetStringValue( i );

                    if ( path.IsValid( ) )
                    {
                        const auto& value = path.GetValue( );
                        const std::string ascii( value.begin( ), value.end( ) );

                        result.commandlets[ ascii_sub ].push_back( ascii );
                    }
                };
            }
        };

        for ( const auto& sub : key.EnumSubKeys( ) )
        {
            if ( sub.starts_with( L"History" ) )
            {
                functor( sub );
            }
        }
    }

    return result;
}

fingerprinting::ida_t::ida_t( )
{
    const auto history = get_history( );

    json_t data;

    auto& commandlets = data[ "commandlets" ];
    data[ "recently_analyzed_files" ] = history.history;

    for ( const auto& [commandlet, recents] : history.commandlets )
    {
        auto& current = commandlets[ commandlet ];

        for ( const auto& cmd : recents )
            current.push_back( cmd );
    }

    _object[ "ida_history" ] = data;
}
