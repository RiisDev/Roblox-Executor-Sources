#include "env.hpp"
#include <cstdint>
#include <source_location>

#include <Luau/lgc.h>
#include <Luau/lmem.h>
#include <Luau/lapi.h>
#include <Luau/lcorolib.h>
#include "lualib.h"

#include "../roblox.hpp"

#define l() std::printf( std::string( std::source_location::current( ).function_name( ) ).append( "()\n" ).c_str( ) )

namespace roblox
{
	namespace fn
	{
		void set_thread_identity( lua_State* state, std::size_t identity )
		{
			*reinterpret_cast< std::size_t* >(
				static_cast< std::uint8_t* >( state->userdata ) + off::extraspace::identity ) = identity;
		}

		void set_system_identity( lua_State* state, std::size_t identity )
		{
			set_thread_identity( state, identity );

			/* set camera movement mode for touch devices */
			*reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( __readfsdword( 0x2C ) ) + off::tls::identity ) = identity;
		}

		std::size_t get_thread_identity( lua_State* state )
		{
			return *reinterpret_cast< std::size_t* >(
				static_cast< std::uint8_t* >( state->userdata ) + 0x18 );
		}
	}

	namespace env
	{
		std::int32_t getrenv( lua_State* state )
		{
			l( );

			const auto r_state = rbx_manager_t::get( ).get_rbx_thread( );

			lua_pushvalue( r_state, LUA_GLOBALSINDEX );
			lua_xmove( r_state, state, 1 );

			return 1;
		}

		std::int32_t get_thread_identity( lua_State* state )
		{
			l( );

			lua_pushinteger( state, fn::get_thread_identity( state ) );

			return 1;
		}

		std::int32_t set_thread_identity( lua_State* state )
		{
			l( );

			fn::set_thread_identity( state, luaL_checkinteger( state, 1 ) );

			return 0;
		}

		std::int32_t loadstring( lua_State* state )
		{
			l( );

			size_t l = 0;
			const char* s = luaL_checklstring( state, 1, &l );
			const char* chunkname = luaL_optstring( state, 2, s );

			//lua_setsafeenv( state, LUA_ENVIRONINDEX, false );

			if ( rbx_manager_t::get().push_script(state, std::string(s, l ), chunkname ) )
				return 1;

			lua_pushnil( state );
			lua_insert( state, -2 ); // put before error message
			return 2;          // return nil plus error message
		}

		std::int32_t islclosure( lua_State* state )
		{
			l( );

			luaL_checktype( state, 1, LUA_TFUNCTION );

			lua_pushboolean( state, lua_iscfunction( state, 1 ) == 0 );

			return 1;
		}

		std::int32_t iscclosure( lua_State* state )
		{
			l( );

			luaL_checktype( state, 1, LUA_TFUNCTION );

			lua_pushboolean( state, lua_iscfunction( state, 1 ) );

			return 1;
		}

		std::int32_t identifyexecutor( lua_State* state )
		{
			l( );

			lua_pushstring( state, "Hydrogen" );

			return 1;
		}


		std::int32_t setreadonly( lua_State* state )
		{
			l( );

			luaL_checktype( state, 1, LUA_TTABLE );

			const auto val = lua_isboolean( state, 2 ) ? lua_toboolean( state, 2 ) : lua_isnumber( state, 2 ) == 1;

			lua_setreadonly( state, 1, val );

			return 0;
		}

		std::int32_t isreadonly( lua_State* state )
		{
			l( );

			luaL_checktype( state, 1, LUA_TTABLE );

			lua_pushboolean( state, hvalue( index2addr( state, 1 ) )->readonly );

			return 1;
		}

		std::int32_t getrawmetatable( lua_State* state )
		{
			l( );

			luaL_argcheck( state, lua_isuserdata( state, 1 ) || lua_istable( state, 1 ), 1, "Userdata or Table expected" );

			lua_getmetatable( state, 1 );

			return 1;
		}

		std::int32_t setrawmetatable( lua_State* state )
		{
			l( );

			luaL_argcheck( state, lua_isuserdata( state, 1 ) || lua_istable( state, 1 ), 1, "Userdata or Table expected" );
			luaL_checktype( state, 2, LUA_TTABLE );

			lua_pushvalue( state, 1 );
			lua_pushvalue( state, 2 );
			lua_setmetatable( state, -2 );

			return 0;
		}

		std::int32_t getreg( lua_State* state )
		{
			l( );

			lua_pushvalue( state, LUA_REGISTRYINDEX );

			return 1;
		}

		std::int32_t getgc( lua_State* state )
		{
			l( );

			const auto everything = lua_gettop( state ) && lua_isboolean( state, 1 ) ? lua_toboolean( state, 1 ) : false;

			lua_newtable( state );

			auto count = 1u;
			for ( auto current_page = state->global->allgcopages; current_page; current_page = current_page->gcolistnext )
			{
				char* start, * end;
				int busy_blocks, block_size;
				luaM_getpagewalkinfo( current_page, &start, &end, &busy_blocks, &block_size );

				for ( auto i = start; i != end; i += block_size )
				{
					if ( const auto gco = reinterpret_cast< GCObject* >( i ) )
					{
						if ( gco && ( gco->gch.tt == LUA_TFUNCTION || ( ( gco->gch.tt == LUA_TTABLE || gco->gch.tt == LUA_TUSERDATA ) && everything ) ) )
						{
							state->top->value.gc = gco;
							state->top->tt = gco->gch.tt;
							state->top++;

							lua_rawseti( state, -2, count++ );
						}
					}
				}
			}

			return 1;
		}

		void init( lua_State* const state )
		{
			const auto register_fn = [ state ] ( const lua_CFunction fn,
				const std::initializer_list< const char* > names )
			{
				for ( const auto name : names )
				{
					lua_pushcfunction( state, fn, name );
					lua_setglobal( state, name );
				}
			};

			register_fn( loadstring, { "loadstring" } );
			register_fn( getrenv, { "getrenv" } );
			register_fn( getgc, { "getgc" } );
			register_fn( iscclosure, { "iscclosure" } );
			register_fn( islclosure, { "islclosure" } );
			register_fn( identifyexecutor, { "identifyexecutor" } );
			register_fn( isreadonly, { "isreadonly" } );
			register_fn( setreadonly, { "setreadonly" } );
			register_fn( getrawmetatable, { "getrawmetatable" } );
			register_fn( setrawmetatable, { "setrawmetatable" } );
			register_fn( getreg, { "getreg" } );
			register_fn( hyd_cowrap, { "newcclosure" } );
			register_fn( get_thread_identity, { "getthreadidentity", "getidentity" } );
			register_fn( set_thread_identity, { "setthreadidentity", "setidentity" } );

			rbx_manager_t::get( ).push_script
			(
				state,
				#include "init.lua"
			);

			managers::off_manager_t::get( ).resume(
				rbx_manager_t::get( ).get_script_context( ), state, 0 );
		}
	}
}