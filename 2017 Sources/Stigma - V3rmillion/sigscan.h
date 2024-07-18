#define WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY |PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
#include <Windows.h>

namespace aobscan {
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask);

	DWORD scan(const char* aob, const char* mask);

	DWORD scanWritable(const char* aob, const char* mask);
};