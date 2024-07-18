#pragma once

#include "MemoryLocations.h"

#ifndef bigjuicybooty
#define bigjuicybooty

DWORD ChangeContext() {
	typedef PDWORD (*context) ();
	context Context = (context)location_scriptcontextvftable;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Context, NULL, NULL, NULL);
	DWORD *NewContext = Context();
	return *NewContext = 7;
}

#endif