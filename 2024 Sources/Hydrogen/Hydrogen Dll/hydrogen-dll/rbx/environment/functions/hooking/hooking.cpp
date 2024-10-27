#include "hooking.hpp"

#include <rbx/environment/utilities/cclosure/cclosure.hpp>
#include <rbx/environment/utilities/utilities.hpp>

using closure_type_t 
	= rbx::environment::utilities::closure_type_t;

int rbx::environment::c_hooking::hookfunction( lua_State* L )
{
	c_utilities::e_is_types( L, { { 1, { "function" } }, { 2, { "function" } } } );

	lua_ref( L, 1 );
	lua_ref( L, 2 );

	const auto fn1 = clvalue( luaA_toobject( L, 1 ) );
	const auto fn2 = clvalue( luaA_toobject( L, 2 ) );

	const auto fn1_nups = fn1->nupvalues;
	const auto fn2_nups = fn2->nupvalues;

	const auto fn1_t 
		= utilities::g_cclosure.get_closure_type
			( fn1 );
	const auto fn2_t
		= utilities::g_cclosure.get_closure_type
			( fn2 );

	std::printf( "%i %i\n", fn1_t, fn2_t );

	if ( fn1_t != closure_type_t::lclosure && fn2_t == closure_type_t::cclosure )
	{
		if ( fn1_nups >= fn2_nups )
		{
			utilities::g_cclosure.clone_function( L, 1 );

			lua_ref( L, -1 );

			fn1->c.f = fn2->c.f;
			fn1->c.cont = fn2->c.cont;
			fn1->stacksize = fn2->stacksize;

			for ( int i = 0; i < fn1_nups; i++ )
				setobj2n( L, &fn1->c.upvals[ i ], luaO_nilobject );

			for ( int i = 0; i < fn2_nups; i++ )
				setobj2n( L, &fn1->c.upvals[ i ], &fn2->c.upvals[ i ] );

			return 1;
		}

		luaL_error( L, "Destination has an upvalue count of %i source has an upvalue count of %i", fn1_t, fn2_t );
	}

	if ( fn1_t == closure_type_t::newcclosure && fn2_t == closure_type_t::newcclosure )
	{
		utilities::g_cclosure.clone_function( L, 1 );

		lua_ref( L, -1 );

		const auto lclosure = utilities::g_cclosure.get_closure( fn2 );

		if ( !lclosure )
			luaL_error( L, "Hookfunction: newcclosure has no binded lclosure" );

		utilities::g_cclosure.add( fn1, *lclosure );

		fn1->c.cont = fn2->c.cont;
		fn1->stacksize = fn2->stacksize;
		fn1->nupvalues = fn2_nups;

		return 1;
	}

	if ( fn1_t == closure_type_t::cclosure && fn2_t == closure_type_t::newcclosure )
	{
		utilities::g_cclosure.clone_function( L, 1 );

		lua_ref( L, -1 );

		const auto lclosure = utilities::g_cclosure.get_closure( fn2 );

		if ( !lclosure )
			luaL_error( L, "Hookfunction: newcclosure has no binded lclosure" );

		utilities::g_cclosure.convert( fn1, *lclosure );

		return 1;
	}
	
	if ( fn1_t != closure_type_t::lclosure && fn2_t == closure_type_t::lclosure )
	{
		utilities::g_cclosure.clone_function( L, 1 );

		lua_ref( L, -1 );

		utilities::g_cclosure.convert( fn1, fn2 );

		return 1;
	}

	if ( fn1_t == closure_type_t::lclosure && fn2_t == closure_type_t::lclosure )
	{
		if ( fn1_nups >= fn2_nups )
		{
			utilities::g_cclosure.clone_function( L, 1 );

			lua_ref( L, -1 );

			fn1->l.p = fn2->l.p;

			for ( int i = 0; i < fn1_nups; i++ )
				setobj2n( L, &fn1->l.uprefs[ i ], luaO_nilobject );

			for ( int i = 0; i < fn2_nups; i++ )
				setobj2n( L , &fn1->l.uprefs[ i ], &fn2->l.uprefs[ i ] );

			return 1;
		}

		luaL_error( L, "Destination has an upvalue count of %i source has an upvalue count of %i", fn1, fn2 );
	}

	luaL_error( L, "Invalid hookfunction arguments: destination %i source %i", fn1_t, fn2_t );
}

std::int32_t rbx::environment::c_hooking::hookmetamethod( lua_State* L )
{
	c_utilities::e_is_types( L, { { 1, { "table", "userdata" } }, { 2, { "string" } }, { 3, { "function" } } } );

	const auto method = luaL_checkstring( L, 2 );

	if ( !luaL_getmetafield( L, 1, method ) )
		luaL_error( L, "table/object has no metamethod %s", method );

	const auto dest = luaA_toobject( L, -1 );
	const auto src = luaA_toobject( L, 3 );

	lua_settop( L, 0 );

	luaA_pushobject( L, dest );
	luaA_pushobject( L, src );

	return hookfunction( L );
}

int rbx::environment::c_hooking::newcclosure( lua_State* L )
{
	c_utilities::e_is_types( L, { { 1, { "function" } } } );

	if ( !lua_iscfunction( L, 1 ) )
		return utilities::g_cclosure.wrap( L );

	lua_pushvalue( L, 1 );

	return 1;
}

int rbx::environment::c_hooking::clonefunction( lua_State* L )
{
	utilities::g_cclosure.clone_function( L, 1 );

	return 1;
}

void rbx::environment::c_hooking::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	register_fn( hookfunction, { "hookfunction" } );
	register_fn( hookmetamethod, { "hookmetamethod" } );
	register_fn( newcclosure, { "newcclosure" } );
	register_fn( clonefunction, { "clonefunction" } );
}
