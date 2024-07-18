#include "SigScanner.h"

/* thank you, booing! */
int SigScanner::ScanNoAlign(char *toFind, int bcount, int reachsz, int start, int endd, int align) {
	MEMORY_BASIC_INFORMATION mem;
	int smem = sizeof(mem);
	char* reach = (char*)new char[reachsz];

	for (char* i = (char*)start;;) {
		VirtualQueryEx((HANDLE)-1, i, &mem, smem);
		if ((mem.AllocationProtect & 238) && !(mem.Protect & 257) && (mem.State & 4096)) {
			char* end = (char*)mem.BaseAddress + mem.RegionSize;
			for (char* x = (char*)mem.BaseAddress; x < end;) {
				SIZE_T sz;
				ReadProcessMemory((HANDLE)-1, x, reach, reachsz, &sz);
				for (int ii = 0; ii < sz; ii += align) {
					if (*(int*)(reach + ii) == *(int*)toFind) {
						if (bcount == 4) return (int)x + ii;
						bool isMatch = true;
						for (int b = 4; b < bcount; b++)
							if (*(unsigned char*)(reach + ii + b) != *(unsigned char*)(toFind + b))
								isMatch = false;
						if (isMatch) return (int)x + ii;
					}
				}
				x += sz;
			}
		}
		int oldi = (int)i;
		i += mem.RegionSize;
		if (((int)i > endd - mem.RegionSize) || (oldi>(int)i))
			break;
	}
	return -1;
}

#define WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY |PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
/* cheers to booing for this func */
int SigScanner::ScanNoAlignW(char *toFind, int bcount, int reachsz, int start, int endd, int align) {
	MEMORY_BASIC_INFORMATION mem;
	int smem = sizeof(mem);
	char* reach = (char*)malloc(reachsz);

	for (char* i = (char*)start;;) {
		VirtualQueryEx((HANDLE)-1, i, &mem, smem);
		if ((mem.AllocationProtect & 238) && !(mem.Protect & 257) && (mem.State & 4096) && (mem.Protect & WRITABLE)) {
			char* end = (char*)mem.BaseAddress + mem.RegionSize;
			for (char* x = (char*)mem.BaseAddress; x < end;) {
				SIZE_T sz;
				ReadProcessMemory((HANDLE)-1, x, reach, reachsz, &sz);
				for (int ii = 0; ii < sz; ii += align) {
					if (*(int*)(reach + ii) == *(int*)toFind) {
						if (bcount == 4) return (int)x + ii;
						int isMatch = 1;
						for (int b = 4; b < bcount; b++)
							if (*(unsigned char*)(reach + ii + b) != *(unsigned char*)(toFind + b))
								isMatch = 0;
						if (isMatch) return (int)x + ii;
					}
				}
				x += sz;
			}
		}
		int oldi = (int)i;
		i += mem.RegionSize;
		if (((int)i > endd - mem.RegionSize) || (oldi>(int)i))
			break;
	}
	return -1;
}