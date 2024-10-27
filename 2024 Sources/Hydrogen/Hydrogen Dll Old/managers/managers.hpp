#pragma once
#include <source_location>

#include <dependencies/common_includes.hpp>

#include <managers/roblox/roblox.hpp>
#include <managers/interface/interface.hpp>
#include <managers/offset/offset.hpp>
#include <managers/hook/hook.hpp>
#include <managers/menu/menu.hpp>
#include <managers/singleton.hpp>

#define l() /*std::printf( std::string( std::source_location::current( ).function_name( ) ).append( "()\n" ).c_str( ) )*/

namespace managers
{
	class managers_t
	{
	public:
		roblox_manager_t& roblox_mgr = singleton_t<roblox_manager_t>::get( );
		interface_manager_t& interface_mgr = singleton_t<interface_manager_t>::get( );
		offset_manager_t& offset_mgr = singleton_t<offset_manager_t>::get( );
		hook_manager_t& hook_mgr = singleton_t<hook_manager_t>::get( );
		menu_manager_t& menu_mgr = singleton_t<menu_manager_t>::get( );
	};

	const managers_t mgrs;
}