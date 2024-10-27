#include <algorithm>
#include <fmt/color.h>
#include <hydrogen-decompiler/common.hpp>
#include <iostream>
#include <string_view>

int main( )
{
    logger( )->info( "I'm a test" );
    logger( )->debug( "another test" );
    logger( )->error( "fuck" );
    logger( )->critical( "SHIT FUCK" );

    constexpr std::string_view some_text = "yo!";

    logger( )->info(
        "{} of string {}",
        fmt::styled( "Size", fmt::emphasis::italic | fmt::emphasis::bold |
                                 fmt::fg( fmt::color::lawn_green ) ),

        fmt::format( fmt::emphasis::bold | fmt::emphasis::italic |
                         fmt::fg( fmt::color::aquamarine ),
                     "0x{:X}", some_text.size( ) ) );

    return 0;
}