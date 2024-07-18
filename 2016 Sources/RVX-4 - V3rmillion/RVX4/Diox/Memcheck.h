#include <Windows.h>
#include "Segment.h"

#pragma comment(lib, "MSVCRT.LIB")
#pragma comment(lib, "wininet")
#pragma comment(lib, "Dbghelp.lib")

DWORD baser = (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");

DWORD getaddy3(int address) {
	return (address - 0x400000 + baser);
}

DWORD ReturnAddress = 0;
DWORD JmpAddress = getaddy3(0x7B9A80);
DWORD Base = 0x00401000;
DWORD RBPStart;
DWORD TextSegmentDifference;
void* rbpcopy;

__declspec(naked) void MemcheckThing() {
	__asm {
		pop JmpAddress
		burger :
		push eax
			push ecx

			mov eax, esi
			mov ecx, Base
			cmp eax, ecx
			jb lol
			add ecx, TextSegmentDifference
			add eax, 10h
			cmp eax, ecx
			jg lol
			pop ecx
			pop eax

			push RBPStart

			mov eax, [esp]
			mov eax, [eax + esi]
			sub eax, esi
			imul eax, eax, 7A143589h
			add edi, eax
			mov eax, [esp]
			mov eax, [eax + esi + 04]
			add esi, 04h
			rol edi, 0Dh
			sub eax, esi
			imul edi, edi, 9E3779B1h
			imul eax, eax, 7A143589h
			add ebx, eax
			mov eax, [esp]
			mov eax, [eax + esi + 04h]
			add esi, 04h
			rol ebx, 0Dh
			add eax, esi
			imul ebx, ebx, 9E3779B1h
			imul eax, eax, 7A143589h
			sub ecx, eax
			mov eax, [esp]
			mov eax, [eax + esi + 04h]
			add esi, 04h
			rol ecx, 0Dh
			xor eax, esi
			imul ecx, ecx, 9E3779B1h
			imul eax, eax, 7A143589h
			sub edx, eax
			rol edx, 0Dh
			add esi, 04h
			imul edx, edx, 9E3779B1h

			pop RBPStart

			cmp esi, [ebp - 20h]
			jbe burger

			push JmpAddress
			add dword ptr[esp], 5Eh
			ret
		lol :
		pop ecx
			pop eax
			mov eax, [esi]
			sub eax, esi
			imul eax, eax, 7A143589h
			push JmpAddress
			ret

	}
}

void Memcheck_Disable() {
	ImageSectionInfo* Text = GetSegmentInformation(GetModuleHandle("RobloxPlayerBeta.exe"), ".text");
	DWORD Address = (DWORD)Text->SectionAddress;
	DWORD Size = Text->SectionSize;
	rbpcopy = VirtualAlloc(NULL, Size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(rbpcopy, (void*)Address, Size);
	TextSegmentDifference = Text->SectionSize;
	RBPStart = (DWORD)rbpcopy - (DWORD)Text->SectionAddress;

	BYTE* pAddress = (PBYTE)&JmpAddress;
	DWORD dwJumpTo = (DWORD)&MemcheckThing;
	DWORD dwOldProtect, dwBkup, dwRelAddr;
	VirtualProtect(pAddress, 0x10, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;
	*pAddress = 0xE8;
	*(DWORD *)(pAddress + 1) = dwRelAddr;
	*(BYTE*)(pAddress + 0x5) = 0x90;
	*(BYTE*)(pAddress + 0x6) = 0x90;
	*(BYTE*)(pAddress + 0x7) = 0x90;
	*(BYTE*)(pAddress + 0x8) = 0x90;
	*(BYTE*)(pAddress + 0x9) = 0x90;
	VirtualProtect(pAddress, 0x10, dwOldProtect, &dwBkup);
}
