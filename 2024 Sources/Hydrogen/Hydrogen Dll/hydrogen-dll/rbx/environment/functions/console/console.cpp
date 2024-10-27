#include "console.hpp"

#include <spdlog/sinks/wincolor_sink.h>

#include <util/cio/cio.hpp>
#include <rbx/rbx_structs.hpp>

#include <rbx/environment/environment.hpp>
#include <rbx/environment/utilities/thread/thread.hpp>

void rbx::environment::c_console::rconsoleprint( std::string_view out, sol::this_state L )
{
	util::g_cio.open( );

	static auto attribute_code = 7;

	const auto check_color_code = [ & ] ( std::string_view str ) // Hi iivillian, you can try to make this less ugly :D
	{
		const std::vector< std::string_view > color_codes = { "BLACK", "BLUE", "GREEN", "CYAN", "RED",
			"MAGENTA", "BROWN", "LIGHT_GRAY", "DARK_GRAY", "LIGHT_BLUE", "LIGHT_GREEN",
			"LIGHT_CYAN", "LIGHT_RED", "LIGHT_MAGENTA", "YELLOW", "WHITE" };

		if ( str.starts_with( "@@" ) && str.ends_with( "@@" ) )
		{
			const auto color_code = str.substr( 2, str.length() - 4 );
			const auto it = std::find( color_codes.begin( ), color_codes.end( ), color_code );

			if ( it != color_codes.end( ) )
				attribute_code = std::distance( color_codes.begin( ), it );
			else
				return false;

			return true;
		}

		return false;
	};

	if ( !check_color_code( out ) )
	{
		util::g_cio.set_color( attribute_code );
		util::g_cio.log( )->log( spdlog::level::level_enum::off, out );
		util::g_cio.set_color( 7 ); //default as colors only work w print
	}
}

void rbx::environment::c_console::rconsoleinfo( std::string_view out, sol::this_state L )
{
	util::g_cio.open( );

	util::g_cio.log( )->set_pattern( "[%^INFO%$] %v" );
	util::g_cio.log( )->info( out );
	util::g_cio.default_pattern( );
}

void rbx::environment::c_console::rconsolewarn( std::string_view out, sol::this_state L )
{
	util::g_cio.open( );

	util::g_cio.log( )->set_pattern( "[%^WARN%$] %v" );
	util::g_cio.log( )->warn( out );
	util::g_cio.default_pattern( );
}

void rbx::environment::c_console::rconsoleerr( std::string_view out, sol::this_state L )
{
	util::g_cio.open( );

	util::g_cio.log( )->set_pattern( "[%^ERROR%$] %v" );
	util::g_cio.log( )->error( out );
	util::g_cio.default_pattern( );
}

void rbx::environment::c_console::rconsoleclear( sol::this_state L )
{
	HANDLE hStdOut;

	hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );

	DWORD mode = 0;
	if ( GetConsoleMode( hStdOut, &mode ) )
	{
		const DWORD originalMode = mode;
		mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

		if ( SetConsoleMode( hStdOut, mode ) )
		{
			DWORD written = 0;
			PCWSTR sequence = L"\x1b[2J";
			SetConsoleCursorPosition( hStdOut, COORD{ 0, 0 } );
			WriteConsoleW( hStdOut, sequence, ( DWORD ) wcslen( sequence ), &written, NULL );
		}

		SetConsoleMode( hStdOut, originalMode );
	}
}

void rbx::environment::c_console::rconsolename( std::string_view out, sol::this_state L )
{
	util::g_cio.open( );

	SetConsoleTitleA( out.data() );
}

std::int32_t rbx::environment::c_console::rconsoleinput( lua_State* L )
{
	util::g_cio.open( );

	const auto identity 
		= utilities::c_thread::get_thread_identity( L );

	lua_pushthread( L );
	const auto refid = lua_ref( L, -1 );

	struct c_storage
	{
		std::uint32_t identity;
		lua_State* L;
		int refid;
		std::string res;
	};

	c_storage* storage = 
		new c_storage{ .identity = identity, 
			.L = L, .refid = refid }; // Prevent destruction of any objects due to exiting scope

	std::thread{
		[ storage ]( )
		{
			std::getline( std::cin, storage->res );

			g_environment.m_yields.insert(
			[ storage ]( )
			{
				utilities::c_thread
					::set_thread_identity( storage->L, storage->identity );

				lua_pushstring( storage->L, storage->res.c_str( ) );

				g_environment.resume( storage->L, 1 );

				lua_unref( storage->L, storage->refid );

				delete storage;
			} );
		}
	}.detach( );

	return lua_yield( L, 1 );
}

void rbx::environment::c_console::printconsole( std::string_view out, double r, double g, double b, sol::this_state L ) // TODO: finish this once we have an internal ui
{
	std::uint8_t br = std::uint8_t( r ),
		bg = std::uint8_t( g ),
		bb = std::uint8_t( b );

	//ui::g_ui.console_out(out, br, bg, bb);
}

void rbx::environment::c_console::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	const auto register_fn = [ & ] ( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	register_fn( rconsoleprint, { "rconsoleprint", "consoleprint" } );
	register_fn( rconsoleinfo, { "rconsoleinfo", "consoleinfo"  } );
	register_fn( rconsolewarn, { "rconsolewarn", "consolewarn" } );
	register_fn( rconsoleerr, { "rconsoleerr", "consoleerr" } );
	register_fn( rconsoleclear, { "rconsoleclear", "consoleclear" } );
	register_fn( rconsolename, { "rconsolename", "rconsolesettitle", "consolesettitle" } );
	register_fn( rconsoleinput, { "rconsoleinput", "consoleinput" } );
}
