#pragma once
#include "../shared.hpp"

#include <vector>

#include <tlhelp32.h>
#include <tchar.h>

namespace fingerprinting
{
	class processes_t : public element_manager_t
	{
		static std::vector< PROCESSENTRY32 > get_processes( );
		static json_t serialize_process( const PROCESSENTRY32& proc );

	public:
		processes_t( );
	};
}