#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <format>
#include <set>

#include "instance/instance.hpp"

namespace rbx::environment
{
	class c_utilities
	{
	public:
		template< typename ...t >
		static bool is_type( const sol::object& obj, t... types )
		{
			if ( !obj.valid( ) || !obj.lua_state( ) )
				return false;

			sol::state_view sv{ obj.lua_state( ) };

			const auto object_type = sv[ "type" ]( obj ).get< std::string >( );

			for ( const auto type : std::initializer_list< std::string_view >{ types... } )
			{
				if ( object_type == type )
					return true;
			}

			return false;
		}

		template< typename ...t >
		static bool is_typeof( const sol::object& obj, t... types )
		{
			if ( !obj.valid( ) || !obj.lua_state( ) )
				return false;

			sol::state_view sv{ obj.lua_state( ) };

			const auto object_type = sv[ "typeof" ]( obj ).get< std::string >( );

			for ( const auto type : std::initializer_list< std::string_view >{ types... } )
			{
				if ( object_type == type )
					return true;
			}

			return false;
		}

		template< typename ...t >
		static void e_is_typeof( std::int32_t idx, const sol::object& obj, t... types )
		{
			const auto L = obj.lua_state( );

			sol::state_view sv{ L };

			if ( !is_typeof( obj, types... ) )
			{
				std::string fmt_string{};
				for ( auto i = 0u; i < sizeof...( t ); ++i )
				{
					fmt_string += "{}";

					if ( i < sizeof...( t ) - 1 )
						fmt_string += "|";
				}

				const auto expected = std::vformat( fmt_string, std::make_format_args( types... ) ).c_str( );

				const auto recieved = sv[ "typeof" ]( obj ).get< std::string >( );

				lua_pushfstring( L, "stack index %d, expected %s, received %s:",
					idx, expected, recieved );

				lua_error( L );
			}
		}

		static void e_is_types( lua_State* L, std::initializer_list< std::pair< std::uint32_t, std::initializer_list< const char* > > > type_router )
		{
			for ( const auto& [ index, types ] : type_router )
			{
				auto obj = sol::stack::get< sol::object >( L, index );
				auto found_matching_type = false;

				for ( const auto router : types )
				{
					if( is_type( obj, router ) )
					{
						found_matching_type = true;
						break;
					}
				}

				if ( !found_matching_type )
				{
					std::string fmt_string{};
					for ( auto i = types.begin(); i != types.end(); ++i )
					{
						fmt_string += std::string{ *i };

						if ( i < types.end( ) - 1 )
							fmt_string += "|";
					}

					lua_pushfstring( L, "stack index %d, expected %s, received %s:", index, fmt_string.c_str(), sol::type_name( L, obj.get_type( ) ).c_str( ) );
					lua_error( L );
				}
			}
		}

		template < typename t = sol::object >
		static t get_service( lua_State* L, std::string_view service )
		{
			sol::state_view sv{ L };

			const auto game = sv[ "game" ];

			return game[ "GetService" ]
				( game, service ).get< t >( );
		}
	};
}