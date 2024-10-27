#include "storage.hpp"

#include <rbx/environment/utilities/drawing_types/drawing_types.hpp>

void rbx::environment::utilities::c_drawing_cache::add( base_t* obj )
{
	safe_request( [ & ]( )
	{
		m_container.push_back( obj );

		std::sort( m_container.begin( ), m_container.end( ), [ ]( base_t* a, base_t* b )
		{
			return a->zindex > b->zindex;
		} );
	} );
}
