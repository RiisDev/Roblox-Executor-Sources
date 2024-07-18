#pragma once
#include <Windows.h>

struct Addresses {
	// Contains offset of address from base address.

	// VFTables.
	static const DWORD DataModel = 0x1CACFD8;
	static const DWORD VisualEngine = 0x1DE0B90;
};