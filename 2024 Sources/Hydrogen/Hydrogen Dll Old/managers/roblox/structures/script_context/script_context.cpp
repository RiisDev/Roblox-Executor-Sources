#include "script_context.hpp"

roblox::structures::script_context_t* roblox::structures::script_context_t::get( )
{
	return  datamodel_t::get( )
		->get_instance_by_name< script_context_t* >( "Script Context" );
}
