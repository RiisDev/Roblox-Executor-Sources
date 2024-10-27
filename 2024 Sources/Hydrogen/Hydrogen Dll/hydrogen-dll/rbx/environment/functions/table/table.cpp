#include "table.hpp"

#include <rbx/environment/utilities/utilities.hpp>

std::int32_t rbx::environment::c_table::getrawmetatable( lua_State* L )
{
	c_utilities::e_is_types( L, { { 1, {"table", "userdata" } } } );

	lua_getmetatable( L, 1 );

	return 1;
}

std::int32_t rbx::environment::c_table::setrawmetatable( lua_State* L )
{
	c_utilities::e_is_types( L, { { 1, {"table", "userdata" } }, 
		{ 2, { "table" } } } );

	lua_setmetatable( L, -2 );

	return 0;
}

std::int32_t rbx::environment::c_table::setreadonly( lua_State* L )
{
	c_utilities::e_is_types( L, { { 1, { "table", "userdata" } }, 
		{ 2, { "boolean", "number" } } } );

	bool state = lua_isboolean( L, 2 ) ? lua_toboolean( L, 2 ) : lua_tonumber( L, 2 ) != 0;

	lua_setreadonly( L, 1, state );

	return 0;
}

std::int32_t rbx::environment::c_table::isreadonly( lua_State* L )
{
	c_utilities::e_is_types( L, { { 1, {"table", "userdata" } } } );

	lua_pushboolean( L, lua_getreadonly( L, 1 ) );

	return 1;
}

void rbx::environment::c_table::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	const auto register_fn = [ & ] ( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	register_fn( getrawmetatable, { "getrawmetatable" } );
	register_fn( setrawmetatable, { "setrawmetatable" } );
	register_fn( setreadonly, { "setreadonly" } );
	register_fn( isreadonly, { "isreadonly" } );
}