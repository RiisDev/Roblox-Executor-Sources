#include "managers.hpp"

#include <atomic>

managers::renderer_manager_t managers::renderer_mgr;
managers::menu_manager_t managers::menu_mgr;

std::atomic<bool> b_should_exit = false;

void managers::initiate( )
{
	renderer_mgr.initiate( );
}

void managers::request_exit( )
{
	b_should_exit = true;
}

bool managers::should_exit( )
{
	return b_should_exit;
}
