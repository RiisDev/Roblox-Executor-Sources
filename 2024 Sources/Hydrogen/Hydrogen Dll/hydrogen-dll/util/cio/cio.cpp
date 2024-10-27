#include "cio.hpp"

void util::c_cio::open( )
{
	if ( !console_state )
	{
		::ShowWindow( ::GetConsoleWindow( ), SW_RESTORE );

		console_state ^= 1;
	}
}

void util::c_cio::close( )
{
	if ( console_state )
	{
		::ShowWindow( ::GetConsoleWindow( ), SW_HIDE );

		console_state ^= 1;
	}
}

void util::c_cio::set_color( int attribute )
{
	auto console = GetStdHandle( STD_OUTPUT_HANDLE );

	SetConsoleTextAttribute( console, attribute );
}

void util::c_cio::default_pattern( )
{
	logger->set_pattern( "[%^HYDROGEN%$] %v" );
}

void util::c_cio::init_console( )
{
	auto free_console = &FreeConsole;

	DWORD op;
	VirtualProtect( free_console, 1, PAGE_EXECUTE_READWRITE, &op );
	*reinterpret_cast< std::uint8_t* >( &FreeConsole ) = 0xC3;
	VirtualProtect( free_console, 1, op, &op );

	AllocConsole( );
	std::freopen( "conin$", "r", stdin );
	std::freopen( "conout$", "w", stdout );
	std::freopen( "conout$", "w", stderr );

	SetConsoleTitleA( "[hydrogen]" ); 

	if ( !debug )
		::ShowWindow( ::GetConsoleWindow( ), SW_HIDE );
}

std::shared_ptr<spdlog::logger> util::c_cio::log( )
{
	return logger;
}