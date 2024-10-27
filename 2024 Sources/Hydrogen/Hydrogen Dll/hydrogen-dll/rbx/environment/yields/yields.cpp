#include "yields.hpp"

void rbx::environment::c_yields::process( )
{
	std::unique_lock lock( m_lock );

	while ( !m_deque.empty( ) )
	{
		const auto& function = m_deque.front( );

		function( );

		m_deque.pop_back( );
	}
}

void rbx::environment::c_yields::insert( std::function<void( )> callback )
{
	std::unique_lock lock( m_lock );

	m_deque.push_back( callback );
}
