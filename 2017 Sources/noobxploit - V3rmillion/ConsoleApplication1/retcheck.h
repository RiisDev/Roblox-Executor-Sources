#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include "Segment.h"

#pragma comment(lib, "MSVCRT.LIB")
#pragma comment(lib, "wininet")
#pragma comment(lib, "Dbghelp.lib")

DWORD ReturnAddress = 0;
DWORD JmpAddress = 0x551F56;
DWORD Base = 0x00401000;
DWORD RBPStart;
DWORD TextSegmentDifference;
DWORD bytes[] = { 0x46 + 0x61 + 0x69 + 0x6C + 0x65 + 0x64 + 0x20 + 0x74 + 0x6F + 0x20 + 0x63 + 0x72 + 0x65 + 0x61 + 0x74 + 0x65 + 0x20 + 0x4C + 0x75 + 0x61 + 0x20 + 0x73 + 0x74 + 0x61 + 0x74 + 0x65 + 0x00 };
void* rbpcopy;
std::string aFailedToCreate;
__declspec(naked) void RetcheckThing() {

	__asm {
	sub_CA94A4:
		push    Base
			pop     edx
			lea     edx, bytes
			push    dword ptr[ebx + 29Ch]
			mov    eax, dword ptr[ebp + 8]
			mov ebx, offset aFailedToCreate; "Failed to create Lua state"
			add     esp, 8
			test    edi, edi
			off_FDE8FC :
		jnz     loc_533BE9
			lea     eax, [ebp + 8]
			push    eax
			lea     ecx, [ebp - 1Ch]
			add    eax, dword ptr[ebp + 8]
			mov ebx, offset aFailedToCreate; "Failed to create Lua state"
			loc_533BE9:
		call    ds : off_F176DC
			push    ecx
			mov ecx, [edx + 0x00533BD9]
			lea     eax, [ebp - 1Ch]
			push    eax
			off_F176DC :
		mov     dword ptr[ebp - 1Ch], offset off_FDE8FC
			call    sub_CA94A4



	}
}

void RetCheck_Disable() {
	ImageSectionInfo* Text = GetSegmentInformation(GetModuleHandle((LPCSTR)"RobloxPlayerBeta.exe"), ".text");
	DWORD Address = (DWORD)Text->SectionAddress;
	DWORD Size = Text->SectionSize;
	rbpcopy = VirtualAlloc(NULL, Size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(rbpcopy, (void*)Address, Size);
	TextSegmentDifference = Text->SectionSize;
	RBPStart = (DWORD)rbpcopy - (DWORD)Text->SectionAddress;

	BYTE* pAddress = (PBYTE)&JmpAddress;
	DWORD dwJumpTo = (DWORD)&RetcheckThing;
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