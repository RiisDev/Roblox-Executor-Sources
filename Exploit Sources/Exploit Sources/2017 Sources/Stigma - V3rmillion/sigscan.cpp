#include "sigscan.h"

namespace aobscan {
	BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
		for (; *mask; ++aob, ++mask, ++location) {
			__try {
				if (*mask == 'x' && *location != *aob)
					return 0;
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				return 0;
			}
		}
		return 1;
	};

	DWORD aobscan::scan(const char* aob, const char* mask) {
		for (DWORD i = (DWORD)GetModuleHandle(NULL); i <= 0xF000000; ++i) {
			if (compare((BYTE*)i, (BYTE*)aob, mask))
				return i;
		}
		return 0;
	};

	DWORD aobscan::scanWritable(const char* aob, const char* mask) {
		MEMORY_BASIC_INFORMATION memoryInfo;
		DWORD regionEnd = 0;
		DWORD maskLen = strlen(mask);
		for (DWORD i = (DWORD)GetModuleHandle(NULL); i <= 0x4FFFFFFF; ++i) {
			if (i + maskLen > regionEnd) {
				i += maskLen;
			}

			if (i > regionEnd || !regionEnd) {
				VirtualQuery((DWORD*)i, &memoryInfo, sizeof(memoryInfo));
				regionEnd = i + memoryInfo.RegionSize;
				if (!((memoryInfo.State & MEM_COMMIT) && (memoryInfo.Protect & WRITABLE))) {
					i = regionEnd + 1;
					continue;
				}
			}

			if (compare((BYTE*)i, (BYTE*)aob, mask))
				return i;
		}
		return 0;
	}
}