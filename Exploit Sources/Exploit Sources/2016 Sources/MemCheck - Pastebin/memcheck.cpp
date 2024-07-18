// complie this as a DLL.
#include <windows.h>
#include <DbgHelp.h>
#include <string>
#include <time.h>
#include <conio.h>
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

__declspec(naked) void Memcheck_Hook()
{
	__asm
	{
	TextLoop:
		mov		_ecx, ecx
			mov		_esi, esi
			mov		ecx, [ebp - 0Ch]
			mov		esi, Base
			cmp		ecx, esi
			jb		Outside
			add		esi, TextEnd
			add		ecx, 10h
			cmp		ecx, esi
			jg		Outside
			mov		ecx, _ecx
			mov		esi, _esi
			mov     eax, [ebp - 0Ch]
			cmp     eax, [ebp - 0A4h]
			jb		Proceed
			jmp		JmpAddress
			Proceed :
		mov		_esi, esi
			mov		esi, TextSegmentDifference
			mov     ecx, [ebp - 0Ch]
			mov     edx, [esi + ecx]
			add     edx, [ebp - 0Ch]
			imul    edx, 1594FE2Dh
			add     edx, [ebp - 20h]
			mov[ebp - 20h], edx
			mov     eax, [ebp - 20h]
			rol     eax, 13h
			mov[ebp - 20h], eax
			mov     ecx, [ebp - 20h]
			imul    ecx, 0CBB4ABF7h
			mov[ebp - 20h], ecx
			mov     edx, [ebp - 0Ch]
			add     edx, 4
			mov[ebp - 0Ch], edx
			mov     eax, [ebp - 0Ch]
			mov     ecx, [esi + eax]
			sub     ecx, [ebp - 0Ch]
			imul    ecx, 0CBB4ABF7h
			add     ecx, [ebp - 28h]
			mov[ebp - 28h], ecx
			mov     edx, [ebp - 28h]
			rol     edx, 11h
			mov[ebp - 28h], edx
			mov     eax, [ebp - 28h]
			imul    eax, 1594FE2Dh
			mov[ebp - 28h], eax
			mov     ecx, [ebp - 0Ch]
			add     ecx, 4
			mov[ebp - 0Ch], ecx
			mov     edx, [ebp - 0Ch]
			mov     eax, [esi + edx]
			xor eax, [ebp - 0Ch]
			imul    eax, 1594FE2Dh
			add     eax, [ebp - 2Ch]
			mov[ebp - 2Ch], eax
			mov     ecx, [ebp - 2Ch]
			rol     ecx, 0Dh
			mov[ebp - 2Ch], ecx
			mov     edx, [ebp - 2Ch]
			imul    edx, 0CBB4ABF7h
			mov[ebp - 2Ch], edx
			mov     eax, [ebp - 0Ch]
			add     eax, 4
			mov[ebp - 0Ch], eax
			mov     ecx, [ebp - 0Ch]
			mov     edx, [ebp - 0Ch]
			sub     edx, [esi + ecx]
			imul    edx, 0CBB4ABF7h
			add     edx, [ebp - 24h]
			mov[ebp - 24h], edx
			mov     eax, [ebp - 24h]
			rol     eax, 0Fh
			mov[ebp - 24h], eax
			mov     ecx, [ebp - 24h]
			imul    ecx, 1594FE2Dh
			mov[ebp - 24h], ecx
			mov     edx, [ebp - 0Ch]
			add     edx, 4
			mov[ebp - 0Ch], edx
			mov		esi, _esi
			jmp     TextLoop
			Outside :
		mov		ecx, _ecx
			mov		esi, _esi
			mov     eax, [ebp - 0Ch]
			cmp     eax, [ebp - 0A4h]
			jmp		LegitJmp
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


DWORD Bypass(DWORD bypass, DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask) {
	bypass - +dwAddress, dwLen, (BYTE*)bMask, (char*)szMask;
	return 0;
}

// \x69\xC0\x89\x35\x14\x7A\x2B\xC8\x8D

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

	DWORD Membase = Pattern((DWORD)GetModuleHandleA("RobloxPlayerBeta.exe"), Text->SectionSize, (PBYTE)"\x69\xC0\x89\x35\x14\x7A\x2B\xC8\x8D", "xxxxxxxxx");
	JmpAddress = Membase + 0xDC;
	LegitJmp = Membase + 9;
	NopRange(Membase, 9, (DWORD)&Memcheck_Hook);

	return TRUE;
}



void Main() {
	Memcheck_Disable();
	MessageBoxA(0, "Executed Memcheck", "", 0);
}

int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
	}
	return TRUE;
}