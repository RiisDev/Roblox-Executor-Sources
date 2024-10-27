#include "debug_lib.hpp"

#include <rbx/environment/utilities/utilities.hpp>
#include <dependencies/luau/lfunc.h>
#include <dependencies/luau/lmem.h>

sol::table rbx::environment::c_debug::getconstants( sol::object fi, sol::this_state L )
{
	c_utilities::e_is_types( L, { { 1, { "number", "function" } } } );

	int level;
	if ( fi.is< int >( ) )
	{
		level = fi.as< int >( );
		luaL_argcheck( L, level > 0, 1, "invalid level passed to getconstants" );
	}
	else if ( fi.is< sol::function >( ) )
	{
		if( reinterpret_cast< const Closure* >( fi.pointer( ) )->isC )
			luaL_argerror( L, 1, "expected lclosure" );

		level = -lua_gettop( L );
	}

	lua_Debug ar;
	if ( !lua_getinfo( L, level, "f", &ar ) )
		luaL_error( L, "invalid level passed to getconstants" );

	const auto closure = clvalue( luaA_toobject( L, -1 ) );

	std::vector < sol::object > constants;

	for ( auto i = 0; i < closure->l.p->sizek; ++i )
	{
		const auto constant = &closure->l.p->k[ i ];

		if ( constant->tt == LUA_TNIL || constant->tt == LUA_TTABLE || constant->tt == LUA_TFUNCTION )
		{
			constants.push_back( sol::nil );

			continue;
		}

		luaA_pushobject( L, constant );

		constants.push_back( sol::stack::get< sol::object >( L, -1 ) );

		lua_pop( L, 1 );
	}

	return sol::make_object( L, sol::as_table( constants ) );
}

sol::object rbx::environment::c_debug::getconstant( sol::object fi, std::int32_t idx, sol::this_state L )
{
	c_utilities::e_is_types( L, { { 1, { "number", "function" } } } );

	int level;
	if ( fi.is< int >( ) )
	{
		level = fi.as< int >( );
		luaL_argcheck( L, level > 0, 1, "invalid level passed to getconstant" );
	}
	else if ( fi.is< sol::function >( ) )
	{
		if ( reinterpret_cast< const Closure* >( fi.pointer( ) )->isC )
			luaL_argerror( L, 1, "expected lclosure" );

		level = -lua_gettop( L );
	}

	lua_Debug ar;
	if ( !lua_getinfo( L, level, "f", &ar ) )
		luaL_error( L, "invalid level passed to getconstant" );

	const auto closure = clvalue( luaA_toobject( L, -1 ) );

	if ( idx > closure->l.p->sizek || idx <= 0 )
		luaL_error( L, "index out of range" );

	const auto constant = &closure->l.p->k[ idx - 1 ];

	if ( constant->tt == LUA_TNIL || constant->tt == LUA_TTABLE || constant->tt == LUA_TFUNCTION )
		return sol::nil;
	else
	{
		luaA_pushobject( L, constant );

		return sol::stack::get< sol::object >( L, -1 );
	}
}

void rbx::environment::c_debug::setconstant( sol::object fi, std::int32_t idx, sol::object value, sol::this_state L )
{
	c_utilities::e_is_types( L, { { 1, { "number", "function" } } } );

	int level;
	if ( fi.is< int >( ) )
	{
		level = fi.as< int >( );
		luaL_argcheck( L, level > 0, 1, "invalid level passed to setconstant" );
	}
	else if ( fi.is< sol::function >( ) )
	{
		if ( reinterpret_cast< const Closure* >( fi.pointer( ) )->isC )
			luaL_argerror( L, 1, "expected lclosure" );

		level = -lua_gettop( L );
	}

	lua_Debug ar;
	if ( !lua_getinfo( L, level, "f", &ar ) )
		luaL_error( L, "invalid level passed to setconstant" );

	const auto closure = clvalue( luaA_toobject( L, -1 ) );

	if ( idx > closure->l.p->sizek || idx <= 0 )
		luaL_error( L, "index out of range" );

	const auto constant = &closure->l.p->k[ idx - 1 ];

	if ( constant->tt == LUA_TNIL || constant->tt == LUA_TTABLE || constant->tt == LUA_TFUNCTION )
		return;

	value.push( );

	const auto val = luaA_toobject( L, -1 );

	if ( val->tt != constant->tt )
		luaL_argerror( L, 3, "type does not match constant at specified index" );

	constant->tt = val->tt;
	constant->value = val->value;
}

sol::table rbx::environment::c_debug::getupvalues( sol::object fi, sol::this_state L )
{
	c_utilities::e_is_types( L, { { 1, { "number", "function" } } } );

	int level;
	if ( fi.is< int >( ) )
	{
		level = fi.as< int >( );
		luaL_argcheck( L, level > 0, 1, "invalid level passed to getupvalues" );
	}
	else if ( fi.is< sol::function >( ) )
	{
		if ( reinterpret_cast< const Closure* >( fi.pointer( ) )->isC )
			luaL_argerror( L, 1, "expected lclosure" );

		level = -lua_gettop( L );
	}

	lua_Debug ar;
	if ( !lua_getinfo( L, level, "f", &ar ) )
		luaL_error( L, "invalid level passed to getupvalues" );

	const auto closure = clvalue( luaA_toobject( L, -1 ) );

	std::vector < sol::object > upvalues;

	for ( auto i = 0; i < closure->nupvalues; ++i )
	{
		const auto upval = &closure->l.uprefs[ i ];

		if ( upval->tt == LUA_TNIL )
		{
			upvalues.push_back( sol::nil );

			continue;
		}

		luaA_pushobject( L, upval );

		upvalues.push_back( sol::stack::get< sol::object >( L, -1 ) );

		lua_pop( L, 1 );
	}

	return sol::make_object( L, sol::as_table( upvalues ) );
}

sol::object rbx::environment::c_debug::getupvalue( sol::object fi, std::int32_t idx, sol::this_state L )
{
	c_utilities::e_is_types( L, { { 1, { "number", "function" } } } );

	int level;
	if ( fi.is< int >( ) )
	{
		level = fi.as< int >( );
		luaL_argcheck( L, level > 0, 1, "invalid level passed to getupvalue" );
	}
	else if ( fi.is< sol::function >( ) )
	{
		if ( reinterpret_cast< const Closure* >( fi.pointer( ) )->isC )
			luaL_argerror( L, 1, "expected lclosure" );

		level = -lua_gettop( L );
	}

	lua_Debug ar;
	if ( !lua_getinfo( L, level, "f", &ar ) )
		luaL_error( L, "invalid level passed to getupvalue" );

	const auto closure = clvalue( luaA_toobject( L, -1 ) );

	if ( idx > closure->nupvalues || idx <= 0 )
		luaL_error( L, "index out of range" );

	const auto upval = &closure->l.uprefs[ idx - 1 ];

	if ( upval->tt == LUA_TNIL )
		return sol::nil;
	else
	{
		luaA_pushobject( L, upval );

		return sol::stack::get< sol::object >( L, -1 );
	}
}

void rbx::environment::c_debug::m_setupvalue( sol::object fi, std::int32_t idx, sol::object value, sol::this_state L )
{
	c_utilities::e_is_types( L, { { 1, { "number", "function" } } } );

	int level;
	if ( fi.is< int >( ) )
	{
		level = fi.as< int >( );
		luaL_argcheck( L, level > 0, 1, "invalid level passed to setupvalue" );
	}
	else if ( fi.is< sol::function >( ) )
	{
		if ( reinterpret_cast< const Closure* >( fi.pointer( ) )->isC )
			luaL_argerror( L, 1, "expected lclosure" );

		level = -lua_gettop( L );
	}

	lua_Debug ar;
	if ( !lua_getinfo( L, level, "f", &ar ) )
		luaL_error( L, "invalid level passed to setupvalue" );

	const auto closure = clvalue( luaA_toobject( L, -1 ) );

	if ( idx > closure->nupvalues || idx <= 0 )
		luaL_error( L, "index out of range" );

	const auto upvalue = &closure->l.uprefs[ idx - 1 ];

	value.push( );

	const auto val = luaA_toobject( L, -1 );

	upvalue->tt = val->tt;
	upvalue->value = val->value;
}

// TODO: Dont allow calling the return
sol::table rbx::environment::c_debug::getprotos( sol::object fi, sol::this_state L )
{
	c_utilities::e_is_types( L, { { 1, { "number", "function" } } } );

	int level;
	if ( fi.is< int >( ) )
	{
		level = fi.as< int >( );
		luaL_argcheck( L, level > 0, 1, "invalid level passed to getprotos" );
	}
	else if ( fi.is< sol::function >( ) )
	{
		if ( reinterpret_cast< const Closure* >( fi.pointer( ) )->isC )
			luaL_argerror( L, 1, "expected lclosure" );

		level = -lua_gettop( L );
	}

	lua_Debug ar;
	if ( !lua_getinfo( L, level, "f", &ar ) )
		luaL_error( L, "invalid level passed to getprotos" );

	const auto closure = clvalue( luaA_toobject( L, -1 ) );

	std::vector < sol::object > functions;

	for ( auto i = 0; i < closure->l.p->sizep; ++i )
	{
		const auto proto = closure->l.p->p[ i ];

		lua_checkstack( L, 1 );

		setclvalue( L, L->top, luaF_newLclosure( L, 0, closure->env, proto ) );
		incr_top( L );

		functions.push_back( sol::stack::get< sol::object >( L, -1 ) );

		lua_pop( L, 1 );
	}

	return sol::make_object( L, sol::as_table( functions ) );
}

// TODO: Dont allow calling the return
sol::object rbx::environment::c_debug::getproto( sol::object fi, std::int32_t idx, std::optional< bool > activated, sol::this_state L )
{
	c_utilities::e_is_types( L, { { 1, { "number", "function" } } } );

	int level;
	if ( fi.is< int >( ) )
	{
		level = fi.as< int >( );
		luaL_argcheck( L, level > 0, 1, "invalid level passed to getproto" );
	}
	else if ( fi.is< sol::function >( ) )
	{
		if ( reinterpret_cast< const Closure* >( fi.pointer( ) )->isC )
			luaL_argerror( L, 1, "expected lclosure" );

		level = -lua_gettop( L );
	}

	lua_Debug ar;
	if ( !lua_getinfo( L, level, "f", &ar ) )
		luaL_error( L, "invalid level passed to getproto" );

	const auto closure = clvalue( luaA_toobject( L, -1 ) );

	if ( idx > closure->l.p->sizep || idx <= 0 )
		luaL_error( L, "index out of range" );

	const auto proto = closure->l.p->p[ idx - 1 ];

	if ( activated.value_or( false ) ) 
	{
		struct
		{
			lua_State* L;
			Closure* cl;
			std::vector< Closure* > functions;
		} c_ctx{ L, luaF_newLclosure( L, 0, closure->env, proto ) };

		luaM_visitgco( L, &c_ctx, [ ]( void* context, lua_Page* page, GCObject* gco )
		{
			const auto ctx = static_cast< decltype( &c_ctx ) >( context );

			if ( !gco )
				return false;

			if ( isdead( ctx->L->global, gco ) )
				return false;

			const auto tt = gco->gch.tt;

			if ( tt == LUA_TFUNCTION )
			{
				const auto func = gco2cl( gco );

				if ( func && !func->isC )
				{
					if ( ctx->cl->l.p->maxstacksize != func->l.p->maxstacksize )
						return false;

					if ( ctx->cl->l.p->sizeupvalues != func->l.p->sizeupvalues )
						return false;

					if ( ctx->cl->l.p->is_vararg != func->l.p->is_vararg )
						return false;

					if ( ctx->cl->l.p->numparams != func->l.p->numparams )
						return false;

					if ( ctx->cl->l.p->nups != func->l.p->nups )
						return false;

					if ( ctx->cl->l.p->sizep != func->l.p->sizep )
						return false;

					if ( ctx->cl->l.p->sizek != func->l.p->sizek )
						return false;

					TString* debug_name = func->l.p->debugname;
					TString* orig_debug_name = ctx->cl->l.p->debugname;

					if ( ( debug_name && !orig_debug_name ) || ( !debug_name && !orig_debug_name ) )
						return false;

					if ( debug_name && orig_debug_name )
					{
						if ( strcmp( getstr( debug_name ), getstr( orig_debug_name ) ) )
							return false;
					}

					TString* source = func->l.p->source;
					TString* orig_source = ctx->cl->l.p->source;

					if ( ( source && !orig_source ) || ( !source && !orig_source ) )
						return false;

					if ( source && orig_source )
					{
						if ( strcmp( getstr( source ), getstr( orig_source ) ) )
							return false;
					}

					if ( ctx->cl->l.p->sizecode == func->l.p->sizecode )
					{
						Instruction* code = func->l.p->code;
						Instruction* orig_code = ctx->cl->l.p->code;

						for ( auto i = 0u; i < ctx->cl->l.p->sizecode; ++i )
						{
							const auto instr = code[ i ];
							const auto orig_instr = orig_code[ i ];

							if ( std::uint8_t( ( instr & 0xFF ) * 203u ) != std::uint8_t( ( orig_instr & 0xFF ) * 203u ) )
								return false;
						}

						for ( auto i = 0; i < ctx->cl->l.p->sizek; ++i )
						{
							const auto k = &func->l.p->k[ i ];
							const auto orig_k = &ctx->cl->l.p->k[ i ];

							if ( k->tt != orig_k->tt )
								return false;

							switch ( k->tt )
							{
								case LUA_TSTRING:
								{
									const auto s = tsvalue( k );
									const auto orig_ts = tsvalue( orig_k );
										
									if ( s->atom == ATOM_UNDEF )
										s->atom = ctx->L->global->cb.useratom ? ctx->L->global->cb.useratom( s->data, s->len ) : -1;

									if ( orig_ts->atom == ATOM_UNDEF )
										orig_ts->atom = ctx->L->global->cb.useratom ? ctx->L->global->cb.useratom( orig_ts->data, orig_ts->len ) : -1;
										
									if ( s->atom != orig_ts->atom )
										return false;

									break;
								}

								case LUA_TNUMBER:
								{
									if ( nvalue( k ) != nvalue( orig_k ) )
										return false;

									break;
								}

								case LUA_TBOOLEAN:
								{
									if ( bvalue( k ) != bvalue( orig_k ) )
										return false;

									break;
								}

								default:
									break;
							}
						}
						ctx->functions.push_back( func );
					}
				}
			}

			return false;
		} );

		std::vector< sol::function > functions;

		for ( const auto& func : c_ctx.functions )
		{
			lua_checkstack( L, 1 );

			setclvalue( L, L->top, func );
			incr_top( L );

			const auto function = sol::stack::get< sol::function >(  L, -1 );

			functions.push_back( function );

			lua_pop( L, 1 );
		}

		return sol::make_object( L, sol::as_table( functions ) );
	}

	lua_checkstack( L, 1 );

	setclvalue( L, L->top, luaF_newLclosure( L, 0, closure->env, proto ) );
	incr_top( L );

	return sol::stack::get< sol::object >( L, -1 );
}

sol::object rbx::environment::c_debug::getstack( std::int32_t indice, std::optional< std::int32_t > index, sol::this_state L )
{
	luaL_argcheck( L, indice > 0, 1, "invalid level passed to getstack" );

	int level = indice;

	Closure* f = NULL;
	CallInfo* ci = NULL;
	if ( unsigned( level ) < unsigned( L->ci - L->base_ci ) )
	{
		ci = L->ci - level;
		
		const auto args = cast_int( ci->top - ci->base );

		if ( index )
		{
			if ( *index <= 0 )
				luaL_error( L, "invalid index passed to getstack" );

			if ( args > ( *index - 1 ) )
			{
				luaA_pushobject( L, ci->base + ( *index - 1 ) );

				return sol::make_object( L, sol::stack::get< sol::object >( L, -1 ) );
			}
			else
				luaL_error( L, "invalid index passed to getstack" );
		}
		else
		{
			std::vector< sol::object > tbl;

			int iterator = 1;
			for ( auto stack_value = ci->base; stack_value < ci->top; ++stack_value )
			{
				luaA_pushobject( L, stack_value );

				tbl.push_back( sol::stack::get< sol::object >( L, -1 ) );

				lua_pop( L, 1 );
			}

			return sol::make_object( L, sol::as_table( tbl ) );
		}
	}
	else
		luaL_error( L, "invalid level passed to getstack" );
}

void rbx::environment::c_debug::setstack( std::int32_t indice, std::int32_t index, sol::object value, sol::this_state L )
{
	luaL_argcheck( L, indice > 0, 1, "invalid level passed to setstack" );

	int level = indice;

	Closure* f = NULL;
	CallInfo* ci = NULL;
	if ( unsigned( level ) < unsigned( L->ci - L->base_ci ) )
	{
		ci = L->ci - level;

		const auto args = cast_int( ci->top - ci->base );

		if ( args > ( index - 1 ) )
		{
			auto reg = ci->base + ( index - 1 );

			value.push( );

			const auto val = luaA_toobject( L, -1 );

			if ( val->tt != reg->tt )
				luaL_argerror( L, 3, "type does not match register at specified index" );

			reg->tt = val->tt;
			reg->value = val->value;
		}
		else
			luaL_error( L, "invalid index passed to setstack" );

	}
	else
		luaL_error( L, "invalid level passed to setstack" );
}

sol::table rbx::environment::c_debug::getinfo( sol::object fi, sol::this_state L )
{
	c_utilities::e_is_types( L, { { 1, { "number", "function" } } } );

	int level;
	if ( fi.is< int >( ) )
	{
		level = fi.as< int >( );
		luaL_argcheck( L, level > 0, 1, "invalid level passed to getinfo" );
	}
	else if ( fi.is< sol::function >( ) )
	{
		if ( reinterpret_cast< const Closure* >( fi.pointer( ) )->isC )
			luaL_argerror( L, 1, "expected lclosure" );

		level = -lua_gettop( L );
	}

	lua_Debug ar;
	if ( !lua_getinfo( L, level, "flnasu", &ar ) )
		luaL_error( L, "invalid level passed to getinfo" );

	const auto function = sol::stack::get< sol::function >( L, -1 );

	sol::state_view sv{ L };

	auto info = sv.create_table( );

	// TODO: validate, stuff like name may be nullptr

	info[ "source" ] = ar.source;
	info[ "short_src" ] = ar.short_src;
	info[ "func" ] = sol::as_function( function );
	info[ "what" ] = ar.what;
	info[ "currentline" ] = ar.currentline;
	info[ "name" ] = ar.name;
	info[ "nups" ] = ar.nupvals;
	info[ "numparams" ] = ar.nparams;
	info[ "is_vararg" ] = int( ar.isvararg );

	return info;
}

//TODO: debug.traceback

void rbx::environment::c_debug::init( lua_State* L ) const
{
	sol::state_view sv{ L };

	sol::table debug = sv.create_named_table( "debug" );

	const auto register_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			sv.set_function( alias, function );
	};

	const auto register_debug_fn = [ & ]( auto function, std::initializer_list< const char* > aliases )
	{
		for ( const auto alias : aliases )
			debug.set_function( alias, function );
	};

	register_debug_fn( getconstants, { "getconstants" } );
	register_debug_fn( getconstant, { "getconstant" } );
	register_debug_fn( setconstant, { "setconstant" } );
	register_debug_fn( getupvalues, { "getupvalues" } );
	register_debug_fn( getupvalue, { "getupvalue" } );
	register_debug_fn( m_setupvalue, { "setupvalue" } );
	register_debug_fn( getprotos, { "getprotos" } );
	register_debug_fn( getproto, { "getproto" } );
	register_debug_fn( getstack, { "getstack" } );
	register_debug_fn( setstack, { "setstack" } );
	register_debug_fn( getinfo, { "getinfo" } );

	register_debug_fn( [ ]( lua_State* L ) -> std::int32_t
	{
		c_utilities::e_is_types( L, { { 1, {"table", "userdata" } },
			{ 2, { "table" } } } );

		lua_setmetatable( L, -2 );

		return 0;
	}, { "setmetatable" } );

	register_debug_fn( [ ]( lua_State* L ) -> std::int32_t
	{
		sol::state_view sv{ L };

		return sol::stack::push( L, sv.lua_registry( ) );
	}, { "getregistry" } );

	register_fn( getconstants, { "getconstants" } );
	register_fn( getconstant, { "getconstant" } );
	register_fn( setconstant, { "setconstant" } );
	register_fn( getupvalues, { "getupvalues" } );
	register_fn( getupvalue, { "getupvalue" } );
	register_fn( m_setupvalue, { "setupvalue" } );
	register_fn( getprotos, { "getprotos" } );
	register_fn( getproto, { "getproto" } );
	register_fn( getstack, { "getstack" } );
	register_fn( setstack, { "setstack" } );
	register_fn( getinfo, { "getinfo" } );
}