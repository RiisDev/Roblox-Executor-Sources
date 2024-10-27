#pragma once

#include <dependencies/common_includes.hpp>

namespace managers
{
	template<typename wrapped_class_t>
	class singleton_t
	{
		singleton_t( ) {}
	public:
		singleton_t( singleton_t& other ) = delete;
		void operator=( const singleton_t& ) = delete;

		[[nodiscard]] static wrapped_class_t& get( )
		{
			static wrapped_class_t instance;
			return instance;
		}
	};
}