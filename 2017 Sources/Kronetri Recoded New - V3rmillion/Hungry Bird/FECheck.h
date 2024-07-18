#include "aobscan.h"

int FE(int Workspace) {
	return *(BYTE*)(Workspace + 603);
}