#include "scripts.hpp"

#include <luau/bytecode/bytecode.hpp>
#include <rbx/environment/environment.hpp>

using c_scripts = rbx::environment::c_scripts;

void c_scripts::process( )
{
	std::unique_lock lock( m_lock );

	while ( !m_deque.empty( ) )
	{
		const auto& script = m_deque.front( );

		g_environment.execute( script );

		m_deque.pop_back( );
	}
}

void c_scripts::insert( const std::string& script )
{
	std::unique_lock lock( m_lock );

	m_deque.push_back( script );
}