#include "trustcheck.hpp"

#include <utilities/io.hpp>
#include <managers/managers.hpp>

void roblox::trustcheck::initiate( )
{
	constexpr std::uint8_t shellcode[ ] = { 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 };

	const auto address = reinterpret_cast< void* >( managers::mgrs.offset_mgr.roblox.trustcheck );

	DWORD old_protection;
	VirtualProtect( address, sizeof( shellcode ), PAGE_EXECUTE_READWRITE, &old_protection );

	std::memcpy( address, shellcode, sizeof( shellcode ) );

	VirtualProtect( address, sizeof( shellcode ), old_protection, &old_protection );
}