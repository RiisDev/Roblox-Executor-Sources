#include "clipboard.hpp"

#include <Windows.h>

std::optional<std::string> fingerprinting::clipboard_t::get_clipboard( )
{
    if ( !OpenClipboard( nullptr ) )
        return std::nullopt;

    const auto handle = GetClipboardData( CF_TEXT );
    if ( handle == INVALID_HANDLE_VALUE )
        return std::nullopt;

    const auto ptr = static_cast< char* >( GlobalLock( handle ) );
    if ( !ptr )
        return std::nullopt;

    const std::string text = ptr;

    GlobalUnlock( handle );
    CloseClipboard( );

    return text;
}

fingerprinting::clipboard_t::clipboard_t( )
{
    auto& text = _object[ "clipboard_text" ];

    if ( const auto clipboard = get_clipboard( ) )
        text = *clipboard;
}
