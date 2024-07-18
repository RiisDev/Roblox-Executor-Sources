#pragma once
#include <windows.h>
#include <DbgHelp.h>
#include <string>
#include <time.h>
#include <conio.h>
#include "Globals.h"
#pragma comment(lib, "dbghelp.lib")

#define log() MessageBoxA(0, std::to_string(Step).c_str(), "", 0); Step = Step + 1;
#define format(x) (x - 0x400000 + rBase)
DWORD Base, TextSegmentDifference, TextEnd, _edi, _esi, _ecx, JmpAddress, LegitJmp, Step;

struct ImageSectionInfo
{
	char SectionName[IMAGE_SIZEOF_SHORT_NAME];
	char *SectionAddress;
	int SectionSize;
	ImageSectionInfo(const char* name)
	{
		strcpy(SectionName, name);
	}
};

ImageSectionInfo* GetSegmentInfo(HMODULE hModule, PCHAR SegmentName)
{
	char* dllImageBase = (char*)hModule;
	IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(hModule);
	IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);
	ImageSectionInfo *pSectionInfo = NULL;
	for (int i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++)
	{
		char *name = (char*)pSectionHdr->Name;
		if (strcmp(name, SegmentName) == 0)
		{
			pSectionInfo = new ImageSectionInfo(".data");
			pSectionInfo->SectionAddress = dllImageBase + pSectionHdr->VirtualAddress;
			pSectionInfo->SectionSize = pSectionHdr->Misc.VirtualSize;
			return pSectionInfo;
		}
		pSectionHdr++;
	}
	return NULL;
}

BOOL NopRange(DWORD Address, size_t Size, DWORD OptionalJump = 0)
{
	DWORD OriginalProtection;
	VirtualProtect((LPVOID)Address, Size, PAGE_EXECUTE_READWRITE, &OriginalProtection);
	memset((LPVOID)Address, 0x90, Size);
	if (OptionalJump)
	{
		*(char*)(Address) = 0xE9;
		*(unsigned long*)(Address + 1) = (OptionalJump - Address) - 5;
	}
	VirtualProtect((LPVOID)Address, Size, OriginalProtection, &OriginalProtection);
	return TRUE;
}

DWORD orig_013ED5F0;
DWORD orig_013ED65F;
__declspec(naked) void Memcheck_Hook()
{
	__asm
	{
		mov edx, esp
		pop edx
		mov esp, [esp + 08h]

		orig_013ED5F0:
		mov eax, [esi]
			add eax, esi
			imul eax, eax, 1594FE2Dh
			add eax, [ebp - 0Ch]
			rol eax, 13h
			imul eax, eax, 0CBB4ABF7h
			mov[ebp - 0Ch], eax
			lea eax, [esi + 04h]
			sub eax, [esi + 04h]
			add esi, 08h
			imul eax, eax, 344B5409h
			add eax, [ebp - 14]
			rol eax, 11h
			imul eax, eax, 1594FE2Dh
			mov[ebp - 14], eax
			mov eax, [esi]
			xor eax, esi
			add esi, 04h
			imul eax, eax, 1594FE2Dh
			add eax, [ebp - 10]
			rol eax, 0Dh
			imul eax, eax, 0CBB4ABF7h
			mov[ebp - 10], eax
			mov eax, [esi]
			sub eax, esi
			add esi, 04h
			imul eax, eax, 344B5409h
			add eax, ebx
			rol eax, 0Fh
			imul ebx, eax, 1594FE2Dh
			cmp esi, edi

			orig_013ED65F :
		lea esp, [esp - 0Ch]
	}
}

BOOL MessageBox_()
{
	MessageBoxA(0, "This exploit uses Louka's Memcheck.\n\nFor more information, go fuck yourself.", "", 0);
	return TRUE;
}

BOOL KB()
{
	while (true)
	{
		SHORT State = GetAsyncKeyState(VK_PAUSE);
		if ((1 << 16) & State)
		{
			MessageBox_();
		}
		Sleep(50);
	}
}

bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)   return 0;
	return (*szMask) == NULL;
}

DWORD Pattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i<dwLen; i++)
		if (Compare((BYTE*)(dwAddress + i), bMask, szMask))  return (DWORD)(dwAddress + i);
	return 0;
}

BOOL Memcheck_Disable()
{

	void* RBPCopy;
	ImageSectionInfo* Text;
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)KB, 0, 0, 0);
	Text = GetSegmentInfo(GetModuleHandleA(0), ".text");
	TextEnd = Text->SectionSize;
	RBPCopy = VirtualAlloc(NULL, Text->SectionSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	Base = (DWORD)Text->SectionAddress;
	TextSegmentDifference = ((DWORD)RBPCopy - Base);
	memcpy(RBPCopy, (LPVOID)Text->SectionAddress, Text->SectionSize);

	DWORD Membase = ceaslr(0x158A5F0);
	NopRange(Membase, 9, (DWORD)&Memcheck_Hook);

	return TRUE;
}