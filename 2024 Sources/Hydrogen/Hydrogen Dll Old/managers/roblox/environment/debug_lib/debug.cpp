#include "debug.hpp"

#include <managers/managers.hpp>

#include <Luau/lstate.h>
#include <Luau/lua.h>
#include <Luau/lapi.h>
#include <Luau/lualib.h>
#include <Luau/lgc.h>
#include <Luau/lfunc.h>

static lua_State* getthread( lua_State* L, int* arg )
{
	if ( lua_isthread( L, 1 ) )
	{
		*arg = 1;
		return lua_tothread( L, 1 );
	}
	else
	{
		*arg = 0;
		return L;
	}
}

std::int32_t roblox::environment::debug::get_upvalue( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isLfunction( state, 1 ) || lua_isnumber( state, 1 ), 1, "Expected lclosure or number" );

	int level;
	if ( lua_isnumber( state, 1 ) )
	{
		level = lua_tointeger( state, 1 );
		luaL_argcheck( state, level >= 0, 1, "level can't be negative" );
	}
	else if ( lua_isfunction( state, 1 ) )
	{
		// convert absolute index to relative index
		level = -lua_gettop( state );
	}
	else
		luaL_argerror( state, 1, "function or level expected" );

	lua_Debug ar;
	if ( lua_getinfo( state, level, "f", &ar ) == 0 )
		luaL_argerror( state, 1, "invalid level" );
	
	const auto closure = *reinterpret_cast< Closure** >( index2addr( state, -1 ) );

	const auto idx = lua_tointeger( state, 2 );

	if ( idx < 1 || idx > closure->nupvalues )
		luaL_error( state, "index out of range" );

	TValue* r = &closure->l.uprefs[ idx - 1 ];
	const auto val = ttisupval( r ) ? upvalue( r )->v : r;

	setobj( state, state->top, val );

	api_incr_top( state );

	return 1;
}

std::int32_t roblox::environment::debug::get_upvalues( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isLfunction( state, 1 ) || lua_isnumber( state, 1 ), 1, "Expected lclosure or number" );

	int level;
	if ( lua_isnumber( state, 1 ) )
	{
		level = lua_tointeger( state, 1 );
		luaL_argcheck( state, level >= 0, 1, "level can't be negative" );
	}
	else if ( lua_isfunction( state, 1 ) )
	{
		// convert absolute index to relative index
		level = -lua_gettop( state );
	}
	else
		luaL_argerror( state, 1, "function or level expected" );

	lua_Debug ar;
	if ( lua_getinfo( state, level, "f", &ar ) == 0 )
		luaL_argerror( state, 1, "invalid level" );

	const auto closure = *reinterpret_cast< Closure** >( index2addr( state, -1 ) );

	lua_createtable( state, 0, 0 );

	for ( auto i = 0u; i < closure->nupvalues; ++i )
	{
		TValue* r = &closure->l.uprefs[ i ];
		const auto val = ttisupval( r ) ? upvalue( r )->v : r;

		setobj( state, state->top, val );
		api_incr_top( state );
		
		lua_rawseti( state, -2, i + 1 );
	}

	return 1;
}

std::int32_t roblox::environment::debug::set_upvalue( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isLfunction( state, 1 ) || lua_isnumber( state, 1 ), 1, "Expected lclosure or number" );

	int level;
	if ( lua_isnumber( state, 1 ) )
	{
		level = lua_tointeger( state, 1 );
		luaL_argcheck( state, level >= 0, 1, "level can't be negative" );
	}
	else if ( lua_isfunction( state, 1 ) )
	{
		// convert absolute index to relative index
		level = -lua_gettop( state );
	}
	else
		luaL_argerror( state, 1, "function or level expected" );

	lua_Debug ar;
	if ( lua_getinfo( state, level, "f", &ar ) == 0 )
		luaL_argerror( state, 1, "invalid level" );

	const auto closure = *reinterpret_cast< Closure** >( index2addr( state, -1 ) );

	const auto idx = lua_tointeger( state, 2 );

	if ( idx < 1 || idx > closure->nupvalues )
		luaL_error( state, "index out of range" );

	setobj( state, &closure->l.uprefs[ idx - 1 ], index2addr( state, 3 ) );

	return 0;
}

std::int32_t roblox::environment::debug::get_constant( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isLfunction( state, 1 ) || lua_isnumber( state, 1 ), 1, "Expected lclosure or number" );

	int level;
	if ( lua_isnumber( state, 1 ) )
	{
		level = lua_tointeger( state, 1 );
		luaL_argcheck( state, level >= 0, 1, "level can't be negative" );
	}
	else if ( lua_isfunction( state, 1 ) )
	{
		// convert absolute index to relative index
		level = -lua_gettop( state );
	}
	else
		luaL_argerror( state, 1, "function or level expected" );

	lua_Debug ar;
	if ( lua_getinfo( state, level, "f", &ar ) == 0 )
		luaL_argerror( state, 1, "invalid level" );

	const auto closure = *reinterpret_cast< Closure** >( index2addr( state, -1 ) );

	const int idx = lua_tonumber( state, 2 );

	if ( idx > closure->l.p->sizek || idx <= 0 )
		luaL_error( state, "index out of range" );

	const auto constant = &closure->l.p->k[ idx - 1 ];

	if ( constant->tt == LUA_TTABLE || constant->tt == LUA_TFUNCTION )
		lua_pushnil( state );
	else
	{
		setobj( state, state->top, constant );
		api_incr_top( state );
	}



	return 1;
}

std::int32_t roblox::environment::debug::get_constants( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isLfunction( state, 1 ) || lua_isnumber( state, 1 ), 1, "Expected lclosure or number" );

	int level;
	if ( lua_isnumber( state, 1 ) )
	{
		level = lua_tointeger( state, 1 );
		luaL_argcheck( state, level >= 0, 1, "level can't be negative" );
	}
	else if ( lua_isfunction( state, 1 ) )
	{
		// convert absolute index to relative index
		level = -lua_gettop( state );
	}
	else
		luaL_argerror( state, 1, "function or level expected" );

	lua_Debug ar;
	if ( lua_getinfo( state, level, "f", &ar ) == 0 )
		luaL_argerror( state, 1, "invalid level" );

	const auto closure = *reinterpret_cast< Closure** >( index2addr( state, -1 ) );

	lua_createtable( state, 0, 0 );

	for ( auto i = 0; i < closure->l.p->sizek; ++i )
	{
		const auto constant = &closure->l.p->k[ i ];

		if ( constant->tt == LUA_TTABLE || constant->tt == LUA_TFUNCTION )
			lua_pushnil( state );
		else
		{
			setobj( state, state->top, constant );
			api_incr_top( state );
		}

		lua_rawseti( state, -2, i + 1 );
	}

	return 1;
}

std::int32_t roblox::environment::debug::set_constant( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isLfunction( state, 1 ) || lua_isnumber( state, 1 ), 1, "Expected lclosure or number" );

	int level;
	if ( lua_isnumber( state, 1 ) )
	{
		level = lua_tointeger( state, 1 );
		luaL_argcheck( state, level >= 0, 1, "level can't be negative" );
	}
	else if ( lua_isfunction( state, 1 ) )
	{
		// convert absolute index to relative index
		level = -lua_gettop( state );
	}
	else
		luaL_argerror( state, 1, "function or level expected" );

	lua_Debug ar;
	if ( lua_getinfo( state, level, "f", &ar ) == 0 )
		luaL_argerror( state, 1, "invalid level" );

	const auto closure = *reinterpret_cast< Closure** >( index2addr( state, -1 ) );

	const int idx = lua_tonumber( state, 2 );

	if ( idx > closure->l.p->sizek || idx <= 0 )
		luaL_error( state, "index out of range" );

	const auto constant = &closure->l.p->k[ idx - 1 ];

	if (constant->tt == LUA_TTABLE || constant->tt == LUA_TFUNCTION)
		return 0;

	setobj( state, constant, index2addr( state, 3 ) );

	return 0;
}

std::int32_t roblox::environment::debug::get_proto( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isLfunction( state, 1 ) || lua_isnumber( state, 1 ), 1, "Expected lclosure or number" );

	int level;
	if ( lua_isnumber( state, 1 ) )
	{
		level = lua_tointeger( state, 1 );
		luaL_argcheck( state, level >= 0, 1, "level can't be negative" );
	}
	else if ( lua_isfunction( state, 1 ) )
	{
		// convert absolute index to relative index
		level = -lua_gettop( state );
	}
	else
		luaL_argerror( state, 1, "function or level expected" );

	lua_Debug ar;
	if ( lua_getinfo( state, level, "f", &ar ) == 0 )
		luaL_argerror( state, 1, "invalid level" );

	const auto closure = *reinterpret_cast< Closure** >( index2addr( state, -1 ) );

	const int idx = lua_tonumber( state, 2 );

	if ( idx > closure->l.p->sizep || idx <= 0 )
		luaL_error( state, "index out of range" );

	const auto proto = closure->l.p->p[ idx - 1 ];

	setclvalue( state, state->top, luaF_newLclosure( state, 0, closure->env, proto ) );

	api_incr_top( state );

	if ( lua_gettop( state ) > 2 && lua_isboolean( state, 3 ) && lua_toboolean( state, 3 ) )
	{
		lua_newtable( state );
		lua_pushvalue( state, -2 );
		lua_rawseti( state, -2, 1 );
	}

	return 1;
}

std::int32_t roblox::environment::debug::get_protos( lua_State* state )
{
	l( );

	luaL_argcheck( state, lua_isLfunction( state, 1 ) || lua_isnumber( state, 1 ), 1, "Expected lclosure or number" );

	int level;
	if ( lua_isnumber( state, 1 ) )
	{
		level = lua_tointeger( state, 1 );
		luaL_argcheck( state, level >= 0, 1, "level can't be negative" );
	}
	else if ( lua_isfunction( state, 1 ) )
	{
		// convert absolute index to relative index
		level = -lua_gettop( state );
	}
	else
		luaL_argerror( state, 1, "function or level expected" );

	lua_Debug ar;
	if ( lua_getinfo( state, level, "f", &ar ) == 0 )
		luaL_argerror( state, 1, "invalid level" );

	const auto closure = *reinterpret_cast< Closure** >( index2addr( state, -1 ) );

	lua_createtable( state, 0, 0 );

	for ( auto i = 0; i < closure->l.p->sizep; ++i )
	{
		const auto proto = closure->l.p->p[ i ];

		setclvalue( state, state->top, luaF_newLclosure( state, 0, closure->env, proto ) );

		api_incr_top( state );

		lua_rawseti( state, -2, i + 1 );
	}

	return 1;
}

std::int32_t roblox::environment::debug::get_info( lua_State* state )
{
	lua_Debug dbg;

	int level;
	if (lua_isnumber(state, 1))
	{
		level = lua_tointeger(state, 1);
		luaL_argcheck(state, level >= 0, 1, "level can't be negative");
	}
	else if (lua_isfunction(state, 1))
		level = -lua_gettop(state);
	else
		luaL_argerror(state, 1, "function or level expected");

	if (!lua_getinfo(state, level, "flnas", &dbg))
		return 0;

	lua_newtable(state);

	lua_pushnumber(state, dbg.isvararg);
	lua_setfield(state, -2, "is_vararg");

	lua_pushinteger(state, dbg.nparams);
	lua_setfield(state, -2, "numparams");

	lua_pushnumber(state, dbg.nupvals);
	lua_setfield(state, -2, "nups");

	lua_pushvalue(state, -2);
	lua_setfield(state, -2, "func");

	lua_pushnumber(state, dbg.currentline);
	lua_setfield(state, -2, "currentline");

	lua_pushstring(state, dbg.name);
	lua_setfield(state, -2, "name");

	lua_pushstring(state, dbg.source);
	lua_setfield(state, -2, "source");

	lua_pushstring(state, dbg.short_src);
	lua_setfield(state, -2, "short_src");

	lua_pushstring(state, dbg.what);
	lua_setfield(state, -2, "what");

	return 1;
}

std::int32_t roblox::environment::debug::get_stack( lua_State* state )
{
	l( );

	int level;
	if (lua_isnumber(state, 1))
	{
		level = lua_tointeger(state, 1);
		luaL_argcheck(state, level >= 0, 1, "level can't be negative");
	}
	else if (lua_isfunction(state, 1))
	{
		// convert absolute index to relative index
		level = -lua_gettop(state);
	}
	else
		luaL_argerror(state, 1, "function or level expected");


	lua_Debug ar;
	if (lua_getinfo(state, level, "f", &ar) == 0)
		luaL_argerror(state, 1, "invalid level");

	if (lua_gettop(state) > 1 && lua_isnumber(state, 2))
	{
		const auto index = luaL_checkinteger(state, 2) - 1;

		const auto ci = (state->ci - level);

		if (index > cast_int(ci->top - ci->base) || index < 0)
			luaL_error(state, "Invalid index");

		luaA_pushobject(state, ci->base + index);
	}
	else
	{
		lua_newtable(state);

		const auto ci = (state->ci - level);

		int iterator = 1;
		for ( auto stack_value = ci->base; stack_value < ci->top; ++stack_value )
		{
			luaA_pushobject(state, stack_value);

			lua_rawseti(state, -2, iterator++);
		}
	}

	return 1;
}

std::int32_t roblox::environment::debug::set_stack(lua_State* state)
{
	l( );

	int level;
	if (lua_isnumber(state, 1))
	{
		level = lua_tointeger(state, 1);
		luaL_argcheck(state, level >= 0, 1, "level can't be negative");
	}
	else if (lua_isfunction(state, 1))
	{
		// convert absolute index to relative index
		level = -lua_gettop(state);
	}
	else
		luaL_argerror(state, 1, "function or level expected");

	lua_Debug ar;
	if (lua_getinfo(state, level, "f", &ar) == 0)
		luaL_argerror(state, 1, "invalid level");

	const auto index = luaL_checkinteger(state, 2) - 1;

	const auto ci = state->ci - level;

	if (index > cast_int(ci->top - ci->base) || index < 0)
		luaL_error(state, "Invalid index");

	setobj(state, (state->ci - level)->base + index, index2addr(state, 3));

	return 0;
}
