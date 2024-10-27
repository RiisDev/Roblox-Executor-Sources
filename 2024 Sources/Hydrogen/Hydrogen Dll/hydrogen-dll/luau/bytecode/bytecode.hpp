#pragma once

#include <luau/lua.h>
#include <luau/Luau/BytecodeBuilder.h>

#include <optional>

namespace luau
{
	class c_bytecode : Luau::BytecodeEncoder
	{
		std::uint8_t encodeOp( const std::uint8_t op ) override;

		[[ nodiscard ]] std::string compile( const std::string& script );

	public:
		bool load( lua_State* L, const std::string& script, std::optional< std::string > chunkname );
	} inline g_bytecode;
}