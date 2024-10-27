#pragma once

#include <dependencies/luau/lua.h>
#include <sol/sol.hpp>

namespace rbx::environment::utilities
{
	class c_protected_string
	{
	public:
		static std::string decompress_bytecode( const std::string& source );

		static std::optional< std::string > get_script_bytecode( lua_State* L, std::uint8_t* script );
	};
}