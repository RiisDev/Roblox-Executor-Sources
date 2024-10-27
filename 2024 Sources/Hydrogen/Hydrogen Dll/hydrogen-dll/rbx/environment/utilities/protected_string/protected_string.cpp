#include "protected_string.hpp"

#include <sol/sol.hpp>

#include <dependencies/zstd/zstd.h>
#include <dependencies/zstd/common/xxhash.h>

#include "../thread/thread.hpp"

using c_protected_string
	= rbx::environment::utilities::c_protected_string;

std::string c_protected_string::decompress_bytecode( const std::string& source )
{
	constexpr const char bytecode_magic[] = "RSB1";

	std::string input = source;

	std::uint8_t hash_bytes[ 4 ];
	memcpy( hash_bytes, &input[ 0 ], 4 );

	for ( auto i = 0u; i < 4; ++i )
	{
		hash_bytes[ i ] ^= bytecode_magic[ i ];
		hash_bytes[ i ] -= i * 41;
	}

	for ( size_t i = 0; i < input.size( ); ++i )
		input[ i ] ^= hash_bytes[ i % 4 ] + i * 41;

	XXH32( &input[ 0 ], input.size( ), 42 );

	std::uint32_t data_size;
	memcpy( &data_size, &input[ 4 ], 4 );

	std::vector<std::uint8_t> data( data_size );

	ZSTD_decompress( &data[ 0 ], data_size, &input[ 8 ], input.size( ) - 8 );

	return std::string( reinterpret_cast< char* >( &data[ 0 ] ), data_size );
}

std::optional< std::string > c_protected_string::get_script_bytecode( lua_State* L, std::uint8_t* script )
{
	const auto virtual_function_table
		= *reinterpret_cast< std::uintptr_t** >( script );

	const auto request_code 
		= reinterpret_cast< base_script::types::request_code_t > 
			( virtual_function_table[ base_script::offs::request_code ] );

	const auto code = request_code( script );

	if ( code )
	{
		if ( const auto protected_string = *reinterpret_cast< std::uintptr_t* >( code + base_script::offs::protected_string ) )
		{
			return decompress_bytecode( *reinterpret_cast<
				std::string* >( protected_string + base_script::offs::bytecode ) );
		}
	}

	return std::nullopt;
}
