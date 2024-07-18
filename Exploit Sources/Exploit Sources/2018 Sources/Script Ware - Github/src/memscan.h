#include <Windows.h>

namespace memory {
	DWORD find_pattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask);
	int scan(DWORD mode, char* content, char* mask);
	int scan_bytes(PCHAR Content, PCHAR Mask, bool dofunc);
}