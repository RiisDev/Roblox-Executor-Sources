#ifndef _UTIL_
#define _UTIL_

#include <memory>
#include <spdlog/spdlog.h>

namespace util
{
	void open_console( );

	std::shared_ptr< spdlog::logger > logger( );
}

#endif