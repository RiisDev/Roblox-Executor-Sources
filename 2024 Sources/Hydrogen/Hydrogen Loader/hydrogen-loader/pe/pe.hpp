#pragma once
#include <filesystem>
#include <fstream>
#include <span>
#include <string_view>

#include <pe_bliss/pe_bliss.h>

namespace pe
{
	struct pe_t
	{
		void import_dll( std::string_view dll, std::span< const std::string > imports );
		
		pe_t( const std::filesystem::path& file );
		~pe_t( );
	private:

		std::fstream stream;

		pe_bliss::pe_base image;
	};
}