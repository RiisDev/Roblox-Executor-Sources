#pragma once

extern "C" {
#include "lopcodes.h"
}

class RbxOpEncoder {
public:
	RbxOpEncoder();
	int Encrypt(Instruction i, int idx, int ckey);
};