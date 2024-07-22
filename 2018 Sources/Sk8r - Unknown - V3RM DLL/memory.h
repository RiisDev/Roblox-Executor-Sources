#pragma once

#include <Windows.h>
#include <cstdint>
typedef unsigned char byte;

namespace Retcheck
{
	DWORD Unprotect(DWORD addr, bool mode);
}
