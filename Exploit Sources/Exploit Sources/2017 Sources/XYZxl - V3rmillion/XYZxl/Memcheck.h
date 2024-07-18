/*#pragma once
//#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <Dbghelp.h>
#include <iostream>
#include "detours.h"
//#include "VMProtectSDK.h"
#pragma comment(lib, "Dbghelp.lib")
DWORD TextBase = (DWORD)GetModuleHandle(0);
#define format(x) (x - 0x400000 + TextBase)

int MA;

struct ImageSectionInfo
{
	char SectionName[IMAGE_SIZEOF_SHORT_NAME];
	char *SectionAddress;
	int SectionSize;
	ImageSectionInfo(const char* name)
	{
		strcpy_s(SectionName, name);
	}
};

ImageSectionInfo* GetSegmentInformation(HMODULE Module, char* SegmentName) {
	char* dllImageBase = (char*)Module;
	IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(dllImageBase);
	IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);
	ImageSectionInfo *pSectionInfo = NULL;
	for (int i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++)
	{
		char *name = (char*)pSectionHdr->Name;
		if (strcmp(name, SegmentName) == 0)
		{
			pSectionInfo = new ImageSectionInfo(SegmentName);
			pSectionInfo->SectionAddress = dllImageBase + pSectionHdr->VirtualAddress;
			pSectionInfo->SectionSize = pSectionHdr->Misc.VirtualSize;
			return pSectionInfo;
		}
		pSectionHdr++;
	}
}

void __nop(int address, int bytes, int jmpto = 0) {
	DWORD A, B;
	VirtualProtect((LPVOID)address, bytes, PAGE_EXECUTE_READWRITE, &A);
	for (int x = 0; x != bytes; x++) {
		*(BYTE*)(address + x) = 0x90;
	}
	if (jmpto != 0 && bytes >= 5) {
		*(BYTE*)address = 0xE9;
		*(DWORD*)(address + 1) = (DWORD)(jmpto - (DWORD)address) - 5;
	}
	VirtualProtect((LPVOID)address, bytes, A, &B);
}

DWORD JmpAddress = format(0x53BEC7);
DWORD LegitJmp = format(0x53BE67);
DWORD TextSegmentDifference, Base, TextEnd, _edx, _esi;

void* rbpcopy;

__declspec(naked) void Hook() {
	__asm ret
}

__declspec(naked) void SHook() {
	__asm {

	TextLoop:

		mov		_edx, edx
			mov		_esi, esi
			mov		edx, Base
			cmp		esi, edx
			jb		Outside
			add		edx, TextEnd
			add		esi, 10h
			cmp		esi, edx
			jg		Outside
			mov		esi, _esi
			mov		edx, _edx

			mov     eax, esi
			mov		_esi, ebx
			mov		ebx, TextSegmentDifference
			sub     eax, [ebx + esi]
			mov		ebx, _esi
			add     esi, 4
			imul    eax, 7A143589h
			add     edi, eax
			mov		eax, TextSegmentDifference
			mov     eax, [eax + esi]
			add     eax, esi
			rol     edi, 0Dh
			imul    eax, 7A143589h
			sub     ecx, eax
			imul    edi, 9E3779B1h
			rol     ecx, 0Dh
			mov		eax, TextSegmentDifference
			mov     eax, [eax + esi + 4]
			imul    ecx, 9E3779B1h
			add     esi, 4
			sub     eax, esi
			imul    eax, 7A143589h
			add     ebx, eax
			mov		eax, TextSegmentDifference
			mov     eax, [eax + esi + 4]
			add     esi, 4
			rol     ebx, 0Dh
			add     eax, esi
			imul    ebx, 9E3779B1h
			imul    eax, 7A143589h
			sub     edx, eax
			rol     edx, 0Dh
			add     esi, 4
			imul    edx, 9E3779B1h

			cmp     esi, [ebp - 20h]
			jbe		short TextLoop
			jmp		JmpAddress

			Outside :

		mov		esi, _esi
			mov		edx, _edx
			mov     eax, esi
			sub     eax, [esi]
			add     esi, 4
			jmp		LegitJmp
	}
}

void Memcheck_Disable() {
	VMProtectBeginUltra("Memcheck disabler");
	ImageSectionInfo* Text = GetSegmentInformation(GetModuleHandle("RobloxPlayerBeta.exe"), ".text");
	DWORD Address = (DWORD)Text->SectionAddress;
	DWORD Size = Text->SectionSize;
	rbpcopy = VirtualAlloc(NULL, Size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(rbpcopy, (void*)Address, Size);
	TextEnd = Text->SectionSize;
	Base = Address;
	TextSegmentDifference = (DWORD)rbpcopy - Base;

	__nop(format(0x53BE60), 7, (DWORD)&SHook);
	__nop(format(0x53C260), 6, (DWORD)&Hook);

	VMProtectEnd();
}*/