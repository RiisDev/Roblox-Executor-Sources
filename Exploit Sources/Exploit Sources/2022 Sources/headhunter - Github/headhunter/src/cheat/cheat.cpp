#include "../console/console.h"
#include "cheat.h"


void cheat_t::speed_hack()
{
	output << console::color::pink << "Running speedhack!";
}

void cheat_t::jump_hack()
{
	output << console::color::green << "Running jumphack!";
}