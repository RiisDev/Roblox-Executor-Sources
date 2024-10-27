#pragma once

#include <luau/lua.h>
#include <sol/sol.hpp>

#include <format>

#include <magic_enum/magic_enum.hpp>

#include <bitset>


namespace rbx::environment::utilities
{
	enum class instance_types_t
	{
		Instance,

		ProximityPrompt,
		BasePart,
		ScriptSignal,
		ClickDetector,
		LocalScript,
		ModuleScript
	};

	class c_instance
	{
	public:
		static std::uint8_t* get_parent( std::uint8_t* instance )
		{
			return *reinterpret_cast< std::uint8_t** >( instance + instance::offs::parent );
		}
	};

	template< instance_types_t type >
	class c_sol_instance : public sol::userdata
	{
		struct class_descriptor_t
		{
			char pad_0[ 0x4 ];
			std::string* class_name;
		};

	public:
		std::uint8_t* get_internal( ) const
		{
			return *reinterpret_cast< std::uint8_t** >( const_cast< void* >( this->pointer( ) ) );
		}

		template< typename ...T >
		bool is_type( T... types ) const
		{
			if ( !this->valid( ) || !this->lua_state( ) )
				return false;

			sol::state_view sv{ this->lua_state( ) };

			const auto object_type = sv[ "type" ]( *this ).get< std::string >( );

			for ( const auto t : std::initializer_list< std::string_view >{ types... } )
			{
				if ( object_type == t )
					return true;
			}

			return false;
		}

		template< typename ...T >
		bool is_typeof( T... types ) const
		{
			if ( !this->valid( ) || !this->lua_state( ) )
				return false;

			sol::state_view sv{ this->lua_state( ) };

			const auto object_type = sv[ "typeof" ]( *this ).get< std::string >( );

			for ( const auto t : std::initializer_list< std::string_view >{ types... } )
			{
				if ( object_type == t )
					return true;
			}

			return false;
		}

		template< typename ...T >
		bool is_a( T... types ) const
		{
			if ( !this->is< sol::userdata >( ) )
				return false;

			if ( !is_typeof( "Instance" ) )
				return false;

			sol::state_view sv{ this->lua_state( ) };

			for ( const auto t : std::initializer_list< std::string_view >{ types... } )
			{
				if ( this->as< sol::userdata >( )[ "IsA" ].call< bool >( *this, t ) )
					return true;
			}

			return false;
		}

		template< typename ...T >
		void e_is_a( T... types ) const
		{
			const auto L = this->lua_state( );

			std::string fmt_string{ };
			for ( auto i = 0u; i < sizeof...( T ); ++i )
			{
				fmt_string += "{}";

				if ( i < sizeof...( T ) - 1 )
					fmt_string += "|";
			}

			if ( !is_typeof( "Instance" ) )
			{
				lua_pushfstring( L, "stack index % d, expected %s, received %s:", 
					this->stack_index( ), std::vformat( fmt_string, std::make_format_args( types... ) ).c_str( ), 
						sol::type_name( this->lua_state( ), this->get_type( ) ).c_str( ) );

				lua_error( L );
			}

			if ( !is_a( types... ) )
			{
				const auto classname = this->get< const char* >( "ClassName" );

				lua_pushfstring( L, "stack index %d, expected %s, received %s:", 
					this->stack_index( ), std::vformat( fmt_string, std::make_format_args( types... ) ).c_str( ), classname );

				lua_error( L );
			}
		}

		template< typename ...t >
		bool is_descendant_of( t... ancestors ) const
		{
			if ( !this->is< sol::userdata >( ) )
				return false;

			if ( !is_typeof( "Instance" ) )
				return false;

			sol::state_view sv{ this->lua_state( ) };

			for ( const auto ancestor : std::initializer_list< sol::object >{ ancestors... } )
			{
				if ( !ancestor.valid( ) || ancestor == sol::nil )
					continue;

				if ( this->as< sol::userdata >( )[ "IsDescendantOf" ]( *this, ancestor ).get< bool >( ) )
					return true;
			}

			return false;
		}

		std::optional< std::uintptr_t > get_property_descriptor( lua_State* L, const std::string& prop ) const
		{
			lua_pushlstring( L, prop.c_str( ), prop.length( ) );

			std::int32_t atom;
			lua_tostringatom( L, -1, &atom );

			lua_pop( L, 1 );

			const auto prop_descriptor
				= instance::offs::property_table[ atom ];

			if ( !prop_descriptor )
				return std::nullopt;

			return prop_descriptor;
		}

		class_descriptor_t* get_class_descriptor( ) const
		{
			return *reinterpret_cast< 
				class_descriptor_t** >( get_internal( ) + instance::offs::class_descriptor );
		}

		std::optional< bool > get_scriptable( lua_State* L, const std::string& prop ) const
		{
			const auto property_descriptor = get_property_descriptor( L, prop );

			if ( !property_descriptor )
				return std::nullopt;

			const auto prop_i = instance::offs
				::get_property( get_class_descriptor( ), *property_descriptor );

			if ( !prop_i )
				return std::nullopt;

			return *reinterpret_cast< std::uint32_t* >(
					*prop_i + instance::offs::scriptable ) & 0x20;
		}

		void set_scriptable( lua_State* L, const std::string& prop, bool state ) const
		{
			auto property_descriptor = get_property_descriptor( L, prop );

			if ( !property_descriptor )
				return;

			if ( const auto prop_i = instance::offs::get_property( get_class_descriptor( ), *property_descriptor ) )
			{
				const auto scriptable = *reinterpret_cast< 
					std::uint32_t* >( *prop_i + instance::offs::scriptable );

				*reinterpret_cast< std::uint32_t* >( *prop_i + instance::offs::scriptable ) 
					= std::bitset<32>{ scriptable }.set( 5, state ).to_ulong( );
			}
		}
	};

	template < typename Handler, instance_types_t T >
	bool sol_lua_check( sol::types< c_sol_instance< T > >, lua_State* L, int index, Handler&& handler, sol::stack::record& tracking )
	{
		sol::state_view sv{ L };

		const auto absolute_index = lua_absindex( L, index );

		if ( sol::stack::check< sol::userdata >( L, absolute_index ) )
		{
			const auto obj = sol::stack::get< sol::userdata >( L, absolute_index );

			const auto object_type = sv[ "typeof" ]( obj ).get< std::string >( );

			if ( object_type == "Instance" )
			{
				tracking.use( 1 );

				if ( T == instance_types_t::Instance )
					return true;

				if ( obj[ "IsA" ].call< bool >( obj, magic_enum::enum_name< T >( ) ) )
					return true;
				else
				{
					const auto classname = obj.get< const char* >( "ClassName" );

					lua_pushfstring( L, std::format( "stack index {}, expected {}, received {}", index, magic_enum::enum_name< T >( ), classname ).c_str( ) );
					lua_error( L );
				}
			}
		}
		else
		{
			lua_pushfstring( L, std::format( "stack index {}, expected {}, received {}", index, magic_enum::enum_name< T >( ), lua_typename( L, absolute_index ) ).c_str( ) );
			lua_error( L );
		}

		return false;
	}

	template < instance_types_t T >
	c_sol_instance< T > sol_lua_get( sol::types< c_sol_instance< T > >, lua_State* L, int index, sol::stack::record& tracking )
	{
		int absolute_index = lua_absindex( L, index );

		const auto obj = sol::stack::get< sol::userdata >( L, absolute_index );
		
		tracking.use( 1 );

		return static_cast< c_sol_instance< T > >( obj );
	}
}

namespace rbx::environment
{
	using instance_t = utilities::c_sol_instance< utilities::instance_types_t::Instance >;
	using proximity_prompt_t = utilities::c_sol_instance< utilities::instance_types_t::ProximityPrompt >;
	using base_part_t = utilities::c_sol_instance< utilities::instance_types_t::BasePart >;
	using script_signal_t = utilities::c_sol_instance< utilities::instance_types_t::ScriptSignal >;
	using click_detector_t = utilities::c_sol_instance< utilities::instance_types_t::ClickDetector >;
	using local_script_t = utilities::c_sol_instance< utilities::instance_types_t::LocalScript >;
	using module_script_t = utilities::c_sol_instance< utilities::instance_types_t::ModuleScript >;
}