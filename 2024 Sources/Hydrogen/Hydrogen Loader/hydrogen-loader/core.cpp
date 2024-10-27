#include <iostream>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include "fingerprinting/fingerprinter.hpp"
#include "pe/pe.hpp"

int main( int argc, char* argv[ ] )
{
	{
		pe::pe_t pe( argv[ 1 ] );
		pe.import_dll( "injector.dll", std::to_array< std::string >( { "_DllMain@12" } ) );
	}

	return std::getchar( );
}