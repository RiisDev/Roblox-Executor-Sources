#include "Memory.h"


int Memory::ScanNoAlign(char *toFind, int bcount, int reachsz, int start, int endd, int align) {
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
int Memory::ScanNoAlignW(char *toFind, int bcount, int reachsz, int start, int endd, int align) {
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

void Memory::WriteBytes(DWORD addr, BYTE* bytes, int len)
{
	BYTE *backup = new BYTE[len + 32];
	memcpy(backup, (void*)(addr - 16), len + 32);

	DWORD oldProt;
	VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &oldProt);
	memcpy((void*)addr, bytes, len);
	VirtualProtect((void*)addr, len, oldProt, &oldProt);
}

DWORD Memory::Hook(BYTE* addr, DWORD dst, int len)
{
	BYTE *backup = new BYTE[len + 32];
	memcpy(backup, (void*)(addr - 16), len + 32);

	DWORD dwOldProtect, dwBkup, dwRelAddr;
	VirtualProtect((void*)addr, len, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	dwRelAddr = (DWORD)(dst - (DWORD)addr) - 5;
	*addr = 0xE9;
	*((DWORD*)(addr + 0x1)) = dwRelAddr;
	for (int x = 0x5; x < len; x++)
		*(addr + x) = 0x90;
	VirtualProtect(addr, len, dwOldProtect, &dwBkup);

	return (DWORD)addr + len;
}
bool Memory::Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask) return 0;
	return (*szMask) == NULL;
}

DWORD Memory::FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
{
	for (int i = 0; i<(int)dwLen; i++)
		if (Memory::Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
	return 0;
}

int Memory::Scan(char * content, char* mask)
{
	DWORD PageSize;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	PageSize = si.dwPageSize;
	MEMORY_BASIC_INFORMATION mi;
	for (DWORD lpAddr = 0; lpAddr < 0x7FFFFFFF; lpAddr += PageSize)
	{
		DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
		if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
		if (mi.Type == MEM_MAPPED) continue;
		if (mi.Protect == PAGE_READWRITE)
		{
			int addr = Memory::FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
			if (addr != 0)
			{
				return addr;
			}
		}
	}
	return 0;
}

DWORD Memory::scan2(const char *AoB, char *Mask) {
	return Memory::FindPattern((DWORD)GetModuleHandleA("RobloxPlayerBeta.exe"), 0xF00000, (PBYTE)AoB, Mask);
}

