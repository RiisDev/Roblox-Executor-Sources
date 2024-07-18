#include "mem.h"

void hook(void* func, void* newFunc, int len) {
	DWORD nOldProtect;
	VirtualProtect(func, len, PAGE_EXECUTE_READWRITE, &nOldProtect);
	memset(func, 0x90, len);
	int relativeAddress = (int)newFunc - (int)func - 5;
	*(byte*)func = 0xE9;
	*(int*)((int)func + 1) = relativeAddress;
	VirtualProtect(func, len, nOldProtect, &nOldProtect);
}

void writebytes(void* address, byte* bytes, int len) {
	DWORD nOldProtect;
	VirtualProtect(address, len, PAGE_READWRITE, &nOldProtect);
	for (int i = 0; i < len; i++) {
		*(byte*)((int)address + i) = *(bytes + i);
	}
	VirtualProtect(address, len, nOldProtect, &nOldProtect);
}

void writebyte(void* address, byte byte) {
	writebytes(address, &byte, 1);
}