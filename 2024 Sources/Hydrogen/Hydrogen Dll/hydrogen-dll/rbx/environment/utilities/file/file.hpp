#pragma once

#include <dependencies/luau/lua.h>
#include <sol/sol.hpp>

namespace rbx::environment::utilities
{
	const auto bad_extensions = std::to_array< std::string >( {
		".exe", ".com", ".bat", ".cmd", ".vbs", ".vbe", ".js", ".jse", ".wsf", ".wsh", ".ps1", ".ps1_sys", ".ps2", ".ps2_sys", ".ps3", ".ps3_sys", ".ps4", ".ps4_sys", ".ps5", ".ps5_sys", ".ps6", ".ps6_sys", ".ps7", ".ps7_sys", ".ps8", ".ps8_sys", ".psm1", ".psm1_sys", ".psd1", ".psd1_sys", ".psh1", ".psh1_sys", ".msc", ".msc_sys", ".msh", ".msh_sys", ".msh1", ".msh1_sys", ".msh2", ".msh2_sys", ".mshxml", ".mshxml_sys", ".vshost", ".vshost_sys", ".vbscript", ".vbscript_sys", ".wsf", ".wsf_sys", ".wsh", ".wsh_sys", ".wsh1", ".wsh1_sys", ".wsh2", ".wsh2_sys", ".wshxml", ".wshxml_sys", ".ps1", ".ps1_sys", ".ps2", ".ps2_sys", ".ps3", ".ps3_sys", ".ps4", ".ps4_sys", ".ps5", ".ps5_sys", ".ps6", ".ps6_sys", ".ps7", ".ps7_sys", ".ps8", ".ps8_sys", ".psm1", ".psm1_sys", ".psd1", ".psd1_sys", ".psh1", ".psh1_sys", ".msc", ".msc_sys", ".msh", ".msh_sys", ".msh1", ".msh1_sys", ".msh2", ".msh2_sys", ".mshxml", ".mshxml_sys", ".vshost", ".vshost_sys", ".vbscript", ".vbscript_sys", ".wsf", ".wsf_sys", ".wsh", ".wsh_sys",
	} );

	class c_file
	{
	public:
		static bool is_malicious( const std::string_view str )
		{
			return str.find( "../" ) != std::string::npos || str.find( "..\\" ) != std::string::npos || std::any_of( bad_extensions.begin( ), bad_extensions.end( ), [ &str ]( const auto& ext )
			{
				return str.find( ext ) != std::string::npos;
			} );
		}
	};
}