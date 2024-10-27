#include <luau/Luau/Compiler.h>
#include <rbx/environment/environment.hpp>
#include "bytecode.hpp"

using c_bytecode = luau::c_bytecode;

std::uint8_t c_bytecode::encodeOp( const std::uint8_t op )
{
	return op * 227;
}

std::string c_bytecode::compile( const std::string& script )
{
	return Luau::compile( script, { 1, 1, 0 }, { }, this );
}

bool c_bytecode::load( lua_State* L, const std::string& script, std::optional< std::string > chunkname )
{
	const auto bytecode = compile( script );

	return !luau_load( L, chunkname.value_or( "@Hydrogen" ).c_str( ), bytecode.data( ), bytecode.size( ), 0 );
}