#pragma once

#include <Windows.h>
#include <cstdint>
typedef unsigned char byte;

namespace Retcheck
{
	DWORD Unprotect(DWORD addr, bool mode);
}

namespace sigscan
{
	bool compare(const char* location, const char* aob, const char* mask);
	bool compare_reverse(const char* location, const char* aob, const char* mask);
	byte* scan(const char* aob, const char* mask, uintptr_t start, uintptr_t end);
	byte* scan(const char* module, const char* aob, const char* mask);
}