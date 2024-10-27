#include "boundary.hpp"

#include <utilities/io.hpp>
#include <managers/managers.hpp>

void roblox::boundary::initiate( )
{
	const auto patch_boundary_compare = [ ]( std::uint8_t* const boundary_compare )
	{
		DWORD old_protection;
		VirtualProtect( boundary_compare, sizeof( void* ), PAGE_EXECUTE_READWRITE, &old_protection );

		static const auto spoofed_boundary = std::numeric_limits< std::uintptr_t >::max( );
		*reinterpret_cast< const std::uintptr_t** >( boundary_compare ) = &spoofed_boundary;
		
		VirtualProtect( boundary_compare, sizeof( void* ), old_protection, &old_protection );
	};

	auto count = 0u;

	for ( auto position = text.start; position < text.end; ++position )
		if ( *reinterpret_cast< std::uintptr_t* >( position ) == managers::mgrs.offset_mgr.roblox.boundary )
			++count, patch_boundary_compare( reinterpret_cast< std::uint8_t* >( position ) );

	utilities::io::log( utilities::io::log_options_t::debug, "Patched %i Boundaries", count );
}