#include <cstdint>
#include <array>
#include <iostream>

#include <Windows.h>
#include <intrin.h>
#include <mmintrin.h>

#include <PersesSDK.h>

#pragma section( ".nemi" )

__declspec( dllexport ) auto hydrogen( )
{
	PERSES_MUTATION_START( );

	__declspec( allocate( ".nemi" ) ) static const std::array< const char*, 50 > fake_data {
		"real men fly", "good luck gogo", "hello nemi"
	};

	const char stack_string[ ] = "skid check detected";

	std::printf( stack_string );

	PERSES_MUTATION_END( );

	return &fake_data;
}

bool DllMain( void*, const std::uint32_t, void* )
{
	return true;
}