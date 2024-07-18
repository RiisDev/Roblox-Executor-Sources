#pragma once

#ifndef succ
#define succ

int LocalScriptState(unsigned long* This) {
	int v49, v50;
	v49 = 0;
	v50 = (int)&This[14 * v49 + 41] + This[14 * v49 + 41];
	return v50;
}

#endif