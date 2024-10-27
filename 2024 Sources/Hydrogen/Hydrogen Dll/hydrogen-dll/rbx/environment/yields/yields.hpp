#pragma once

#include <luau/lua.h>
#include <util/deque/deque.hpp>
#include <functional>

namespace rbx::environment
{
	class c_yields : util::c_deque< std::function< void( ) > >
	{
	public:
		void process( );

		void insert( std::function< void( ) > callback );
	};
}