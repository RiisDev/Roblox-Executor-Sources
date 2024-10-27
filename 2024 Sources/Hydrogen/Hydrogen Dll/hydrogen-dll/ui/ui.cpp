#include "ui.hpp"

#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <resource.h>
#include <Richedit.h>
#include <iostream>

#include <rbx/environment/environment.hpp>

#include <resource.h>

LRESULT CALLBACK wndproc( HWND window, UINT message, WPARAM w_param, LPARAM l_param )
{
    static bool mousedown = false;
    switch ( message )
    {
    case WM_CREATE:
    {
        HBITMAP hImage = ( HBITMAP ) LoadImage( GetModuleHandle( L"hydrogen.dll" ), MAKEINTRESOURCE( IDB_BITMAP1 ), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_SHARED );
        HWND hImageView = CreateWindowEx( NULL, L"STATIC", NULL, SS_BITMAP | WS_VISIBLE | WS_CHILD, 0, 00, 700, 456, window, ( HMENU ) NULL, GetModuleHandle( L"hydrogen.dll" ), NULL );
        SendMessage( hImageView, STM_SETIMAGE, IMAGE_BITMAP, ( LPARAM ) hImage );
        break;
    }
    case WM_COMMAND:
        if ( HIWORD( w_param ) == EN_CHANGE )
        {
            ShowWindow( HWND( l_param ), SW_HIDE );
            ShowWindow( HWND( l_param ), SW_SHOW );
            SetFocus( HWND( l_param ) );
        }
        break;
    case WM_LBUTTONDOWN:
        if ( !mousedown )
        {
            mousedown = true;
            POINT p;
            if ( GetCursorPos( &p ) )
            if ( ScreenToClient( ui::g_ui.get_main_window(), &p ) )
            {
                if ( p.x >= 60 && p.y >= 200 && p.x <= 370 && p.y <= 280 )
                {
                    auto input = ui::g_ui.get_script_input( );
                    std::size_t len = SendMessageA( input, WM_GETTEXTLENGTH, 0, 0 );

                    std::string script_buffer( len, '\0' );
                    SendMessageA( input, WM_GETTEXT, ( WPARAM ) len + 1, ( LPARAM ) script_buffer.data( ) );

                    rbx::g_environment.m_scripts.insert( script_buffer );
                }
            }
        }
        return 0;

    case WM_LBUTTONUP:
        if ( mousedown )
        {
            mousedown = false;
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        break;
    default:
        return DefWindowProc( window, message, w_param, l_param );
        break;
    }

    return 0;
}

HWND ui::c_ui::get_main_window( )
{
    return w_main_window;
}

HWND ui::c_ui::get_script_input( )
{
    return w_script_input;
}

bool ui::c_ui::create()
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndproc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandleA( nullptr );
    wcex.hIcon = LoadIcon( wcex.hInstance, IDI_APPLICATION );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = CreateSolidBrush( RGB( 10, 10, 10 ) );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"DesktopApp";
    wcex.hIconSm = LoadIcon( wcex.hInstance, IDI_APPLICATION );

    if ( !RegisterClassEx( &wcex ) )
    {
        MessageBoxA( NULL, "Call to RegisterClassEx failed!", "Windows Desktop Guided Tour", NULL );
        return false;
    }

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow( );

    GetWindowRect( hDesktop, &desktop );

    LoadLibrary( TEXT( "Msftedit.dll" ) );

    w_main_window = CreateWindowEx( NULL, L"DesktopApp", 
        L"2pacalypse 3.0", WS_EX_PALETTEWINDOW, desktop.right / 3, desktop.bottom / 3, // satanic screen positioning
            700, 456, NULL, NULL, GetModuleHandleA( nullptr ), 
                NULL);

    SetWindowPos( w_main_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
    SetWindowLongA( w_main_window, GWL_STYLE, DS_FIXEDSYS | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION );

    w_script_input = CreateWindowEx( NULL, L"RICHEDIT50W", L"", 
            WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | 
                ES_MULTILINE | WS_VISIBLE | ES_AUTOVSCROLL,
                    20, 60, 380, 130, w_main_window, reinterpret_cast<HMENU>( m_script_input ),
                        GetModuleHandleA( nullptr ), NULL );

    /*b_execute = CreateWindowEx( NULL, L"BUTTON", L"", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        60, 200, 310, 80, window, reinterpret_cast< HMENU >( m_execute ), ( HINSTANCE ) 
            GetModuleHandleA( nullptr ), NULL );*/

    HFONT text_font = CreateFontA( 18, 0, 0, 0, FW_LIGHT, FALSE, FALSE,
        FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
            CLEARTYPE_QUALITY, VARIABLE_PITCH, "Segoe UI" );

    SendMessageA( w_script_input, WM_SETFONT, ( WPARAM ) text_font, MAKELPARAM( TRUE, 0 ) );
    SendMessageA( w_script_input, EM_SETLIMITTEXT, 0x7FFFFFFF, 0 );

    if ( !w_main_window )
    {
        MessageBoxA( nullptr, "Call to CreateWindow failed!", "2pacalypse 3.0", NULL );
        return false;
    }

    ShowWindow( w_main_window, SW_SHOWNORMAL );
    UpdateWindow( w_main_window );

    MSG msg;
    while ( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    return bool(msg.wParam);
}
