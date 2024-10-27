#include "file.hpp"

#include <fstream>

#include <rbx/environment/utilities/file/file.hpp>

std::string rbx::environment::c_file::readfile( std::string path, sol::this_state L )
{
	//std::printf( "%s\n", path.c_str( ) );

	//if ( utilities::c_file::is_malicious( path ) )
	//	luaL_error( L, "Malicious attempt" );

	std::ifstream stream( workspace_path / path );

	if ( !stream.is_open( ) )
		luaL_error( L, "Failed to open file" );

	std::string output;
	std::copy( std::istreambuf_iterator< char >( stream ), std::istreambuf_iterator< char >( ), std::back_inserter( output ) );

	return output;
}

void rbx::environment::c_file::writefile( std::string path, std::string contents, sol::this_state L )
{
	//std::printf( "%s\n", path.c_str( ) );

	//if ( utilities::c_file::is_malicious( path ) )
	//	luaL_error( L, "Malicious attempt" );

	std::ofstream stream( workspace_path / path, std::ios::beg | std::ios::binary );

	if ( !stream.is_open( ) )
		luaL_error( L, "Failed to open file" );

	stream.write( contents.c_str( ), contents.size( ) );
}

void rbx::environment::c_file::appendfile( std::string path, std::string contents, sol::this_state L )
{
	//std::printf( "%s\n", path.c_str( ) );

	//if ( utilities::c_file::is_malicious( path ) )
	//	luaL_error( L, "Malicious attempt" );

	std::ofstream stream( workspace_path / path, std::ios::end | std::ios::binary | std::ios::app );

	if ( !stream.is_open( ) )
		luaL_error( L, "Failed to open file" );

	stream.write( contents.c_str( ), contents.size( ) );
}

std::tuple< sol::object, std::optional< std::string > > rbx::environment::c_file::loadfile( std::string path, sol::this_state L )
{
	//std::printf( "%s\n", path.c_str( ) );

	sol::state_view sv{ L };

	const auto script = sv[ "readfile" ]( path ).get< std::string >( );

	const auto&[ obj, err ] = sv["loadstring"]( script ).get< std::tuple< sol::object, std::optional< std::string > > >( );

	if ( err || !obj.is< sol::function >( ) )
		return std::make_tuple( sol::nil, err );

	return std::make_tuple( obj, std::nullopt );
}

std::optional< std::string > rbx::environment::c_file::runfile( std::string path, sol::this_state L )
{
	//std::printf( "%s\n", path.c_str( ) );

	sol::state_view sv{ L };

	const auto& [obj, err] = sv[ "loadfile" ]( path ).get< std::tuple< sol::object, std::optional< std::string > > >( );

	if ( err || !obj.is< sol::function >( ) )
		return err;

	sv[ "task" ][ "spawn" ]( obj.as< sol::function >( ) );

	return std::nullopt;
}

sol::table rbx::environment::c_file::listfiles( std::string folder, sol::this_state L )
{
	//std::printf( "%s\n", folder.c_str( ) );

	//if ( utilities::c_file::is_malicious( folder ) )
	//	luaL_error( L, "Malicious attempt" );

	const auto path = workspace_path / folder;

	if ( !std::filesystem::exists( path ) )
		luaL_error( L, "Directory does not exist" );

	std::vector< std::string > files;

	for ( const auto& entry : std::filesystem::directory_iterator( path ) )
		files.push_back( std::string{ folder }.append( "/" ) + entry.path( ).filename( ).string( ) );

	return sol::make_object( L, sol::as_table( files ) );
}

bool rbx::environment::c_file::isfile( std::string path, sol::this_state L )
{
	//std::printf( "%s\n", path.c_str( ) );

	//if ( std::filesystem::exists( workspace_path / path ) && utilities::c_file::is_malicious( path ) )
	//	luaL_error( L, "Malicious attempt" );

	return std::filesystem::exists( workspace_path / path ) && !std::filesystem::is_directory( workspace_path / path );
}

bool rbx::environment::c_file::isfolder( std::string path, sol::this_state L )
{
	//std::printf( "%s\n", path.c_str( ) );

	//if ( std::filesystem::exists( workspace_path / path ) && utilities::c_file::is_malicious( path ) )
	//	luaL_error( L, "Malicious attempt" );

	return std::filesystem::exists( workspace_path / path ) && std::filesystem::is_directory( workspace_path / path );
}

void rbx::environment::c_file::makefolder( std::string path, sol::this_state L )
{
	//std::printf( "%s\n", path.c_str( ) );

	//if ( utilities::c_file::is_malicious( path ) )
	//	luaL_error( L, "Malicious attempt" );

	const auto full = workspace_path / path;
	if ( std::filesystem::exists( full ) )
		return;

	std::filesystem::create_directories( full );
}

void rbx::environment::c_file::delfolder( std::string path, sol::this_state L )
{
	//std::printf( "%s\n", path.c_str( ) );

	//if ( utilities::c_file::is_malicious( path ) )
	//	luaL_error( L, "Malicious attempt" );

	if ( !path.length( ) || path == "." || path == "./" )
		luaL_error( L, "Cannot delete workspace directory" );

	const auto full = workspace_path / path;
	if ( !std::filesystem::exists( full ) )
		luaL_error( L, "Directory does not exist" );

	std::filesystem::remove_all( full );
}

void rbx::environment::c_file::delfile( std::string path, sol::this_state L ) // TODO: fix escape detection
{
	//std::printf( "%s\n", path.c_str( ) );

	//if ( utilities::c_file::is_malicious( path ) )
	//	luaL_error( L, "Malicious attempt" );

	const auto full = workspace_path / path;
	if ( !std::filesystem::exists( full ) )
		luaL_error( L, "file does not exist" );

	std::filesystem::remove( full );
}

void rbx::environment::c_file::init( lua_State* L ) const
{
	workspace_path = std::filesystem::current_path( ) / "Workspace";

	if ( !std::filesystem::exists( workspace_path ) )
		std::filesystem::create_directories( workspace_path );

	sol::state_view sv{ L };

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	register_fn( readfile, { "readfile" } );
	register_fn( writefile, { "writefile" } );
	register_fn( appendfile, { "appendfile" } );
	register_fn( loadfile, { "loadfile" } );
	register_fn( runfile, { "runfile" } );
	register_fn( listfiles, { "listfiles" } );
	register_fn( isfile, { "isfile" } );
	register_fn( isfolder, { "isfolder" } );
	register_fn( makefolder, { "makefolder" } );
	register_fn( delfolder, { "delfolder" } );
	register_fn( delfile, { "delfile" } );
}
