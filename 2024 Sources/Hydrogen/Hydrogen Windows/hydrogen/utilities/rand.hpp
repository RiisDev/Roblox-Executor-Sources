#pragma once

#include <random>

#include <string>

namespace utilities::rand
{
    inline std::string generate_random( const std::size_t sz )
    {
        const auto random_generator = [ ] ( ) -> char
        {
            const char charset[ ] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

            static std::mt19937 rg{ std::random_device{}( ) };
            static std::uniform_int_distribution<std::string::size_type> pick( 0, sizeof( charset ) - 2 );

            return charset[ pick( rg ) ];
        };

        std::string str{ };
        str.reserve( sz );

        std::generate_n( str.begin( ), sz, random_generator );
        return str;
    }
}