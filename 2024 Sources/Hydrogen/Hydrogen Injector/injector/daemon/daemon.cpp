#include <string_view>
#include <udlib/ud.hpp>
#include <util/util.hpp>
#include "daemon.hpp"

bool daemon::c_daemon::is_daemon( )
{
	const auto peb = ud::get_peb( );
	const auto& clp = peb->ProcessParameters->CommandLine;

	std::wstring_view params( clp.Buffer, clp.Length );

	return params.starts_with( L"\\??" );
}

void daemon::c_daemon::init( )
{
	util::logger( )->info( "initializing daemon controller" );
}
