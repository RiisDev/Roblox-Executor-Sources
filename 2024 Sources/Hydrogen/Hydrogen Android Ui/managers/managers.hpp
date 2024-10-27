#pragma once

#include "renderer/renderer.hpp"
#include "menu/menu.hpp"

namespace managers 
{
	void initiate( );
	
	/* below functions are for Windows ONLY. Used by renderer manager to tell the program to exit. */
	void request_exit( );
	bool should_exit( );
}