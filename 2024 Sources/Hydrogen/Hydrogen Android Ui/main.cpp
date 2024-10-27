/*
	This project has been written to develop & debug the Android ImGui UI, on Windows.
	It has been designed so you can copy paste the 'menu' folder to Android without running into any issues.

	author(s): amoy, aspect, volio
	project name: Hydrogen Android
*/

#include <iostream>
#include <Windows.h>
#include <thread>

#include "managers/managers.hpp"

int main( ) {
	using namespace std::chrono_literals;

	managers::initiate( );

	while ( !managers::should_exit( ) ) {
		std::this_thread::sleep_for( 100ms );
	}

	return EXIT_SUCCESS;
}
