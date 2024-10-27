#pragma once

#include <dependencies/common_includes.hpp>

namespace roblox::structures
{
	struct base_t
	{
		std::uintptr_t address( ) const
		{
			return reinterpret_cast< std::uintptr_t >( this );
		}
	};

	struct class_descriptor_t : base_t
	{
		char pad_0[ 0x4 ];
		std::string class_name;
	};

	struct instance_t : public base_t
	{
		struct child_container_t
		{
			instance_t* instance;
			void* reference;
		};

		[[nodiscard]] std::vector< child_container_t > get_children( ) const
		{
			const auto container = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( this ) + 0x2C );

			std::vector< child_container_t > children { *reinterpret_cast< child_container_t** >( container + 0x0 ), *reinterpret_cast< child_container_t** >( container + 0x4 ) };

			return children;
		}

		instance_t* get_instance_by_name( std::string_view name ) const
		{
			for ( const auto children = get_children( ); const auto child : children )
			{
				if ( child.instance->name( ) == name )
					return child.instance;
			}

			return nullptr;
		}

		template< typename t >
		t get_instance_by_name( std::string_view name ) const
		{
			for ( const auto children = get_children( ); const auto child : children )
			{
				if ( child.instance->name( ) == name )
					return reinterpret_cast< t >( child.instance );
			}

			return nullptr;
		}

		instance_t* get_parent( ) const
		{
			return *reinterpret_cast< instance_t** >( address( ) + 0x30 );
		}

		std::string name( ) const
		{
			return **reinterpret_cast< std::string** >( reinterpret_cast< std::uintptr_t >( this ) + 0x28 );
		}

		std::string get_class_name( ) const
		{
			return *reinterpret_cast< std::string* ( * )( ) >( *reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( address( ) ) + 0x14 ) )( );
		}

		class_descriptor_t* get_class_descriptor( ) const
		{
			return *reinterpret_cast< class_descriptor_t** >( address( ) + 0xC ); 
		}
	};
}