#include "Hook.h"

Hook::Hook(char *name, DWORD address) {
	hookName = name;
	hookAddress = address;
}

void Hook::deployHook(void(*func)(void), BYTE nopCount, bool isJmp) {
	DWORD offset = (PtrToUlong(func) - hookAddress) - 5;
	BYTE nopPatch[0xFF] = { 0 };
	BYTE patch[5] = { 0xE8, 0, 0, 0, 0 };
	if (isJmp)
		patch[0] = 0xE9;
	memcpy(patch + 1, &offset, sizeof(DWORD));
	writeBytesASM(hookAddress, patch, 5);
	if (nopCount == 0) {
		return;
	}
	memset(nopPatch, 0x90, nopCount);
	writeBytesASM(hookAddress + 5, nopPatch, nopCount);
}

void Hook::writeBytesASM(DWORD destAddress, LPVOID patch, DWORD numBytes) {
	DWORD oldProtect = 0;
	DWORD srcAddress = PtrToUlong(patch);
	VirtualProtect((void*)(destAddress), numBytes, PAGE_EXECUTE_READWRITE, &oldProtect);
	__asm {
		nop
		nop
		nop

		mov esi, srcAddress
		mov edi, destAddress
		mov ecx, numBytes
		Start :
		cmp ecx, 0
			jz Exit

			mov al, [esi]
			mov[edi], al
			dec ecx
			inc esi
			inc edi
			jmp Start
			Exit :
		nop
			nop
			nop
	}
	VirtualProtect((void*)(destAddress), numBytes, oldProtect, &oldProtect);
}
