#pragma once

#include <deque>
#include <mutex>

namespace util
{
	template < class T >
	class c_deque
	{
	protected:
		std::deque< T > m_deque;
		std::mutex m_lock;
	};
}