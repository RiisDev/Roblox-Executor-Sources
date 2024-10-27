#include "datamodel.hpp"

#include <managers/managers.hpp>

roblox::structures::datamodel_t* roblox::structures::datamodel_t::get( )
{
	const auto place_launcher = *reinterpret_cast< std::uintptr_t* >( managers::mgrs.offset_mgr.roblox.place_launcher );

	const auto place_launcher_inst = *reinterpret_cast< std::uintptr_t* >( place_launcher + 0x4 );
	const auto secure_player_game = *reinterpret_cast< std::uintptr_t* >( place_launcher_inst + 0x8 );

	return reinterpret_cast< datamodel_t* >( *reinterpret_cast< std::uintptr_t* >( secure_player_game + 0x14 ) + 0xC );
}
