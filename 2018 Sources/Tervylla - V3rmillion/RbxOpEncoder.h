#pragma once

extern "C" {
#include "Lua\lopcodes.h"
}

class RbxOpEncoder {
public:
	RbxOpEncoder();
	int encode(Instruction i, int idx, int ckey);
};