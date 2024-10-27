#pragma once

#include <luau/lua.h>
#include <util/deque/deque.hpp>

namespace rbx::environment
{
	class c_scripts : util::c_deque< std::string >
	{
	public:
		void process( );

		void insert( const std::string& script );
	};
}