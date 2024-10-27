#pragma once
#include "Globals.hpp"

namespace Fluxus {
	class Main {
	public:
		static void Initiate(); //main initiation (I have a call_once to make sure we only call this once :D ) (some manual map glitch also calls this more than once..)
		//static bool ReInitiate(); //for rescanning on teleports...
	};
}