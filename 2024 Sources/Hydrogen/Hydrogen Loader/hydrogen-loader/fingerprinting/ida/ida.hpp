#pragma once
#include "../shared.hpp"

#include <filesystem>
#include <map>

#include <WinReg.hpp>

namespace fingerprinting
{
	namespace wr = winreg;

	struct ida_history_t
	{
		std::vector< std::string > history;

		std::map<
			std::string,
			std::vector< std::string >
		> commandlets;
	};

	struct ida_t : public element_manager_t
	{
		static ida_history_t get_history( );

	public:
		ida_t( );
	};
}