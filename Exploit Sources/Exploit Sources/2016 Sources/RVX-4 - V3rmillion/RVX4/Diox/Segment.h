#pragma once
#include <Windows.h>
#include <Dbghelp.h>
// fuck windows and their shit api and everything that comes with it i hope they burn

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

ImageSectionInfo* GetSegmentInformation(HMODULE Module, char* SegmentName) {
	char* dllImageBase = (char*)Module;
	IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(dllImageBase);
	IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);
	ImageSectionInfo *pSectionInfo = NULL;
	for (int i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++)
	{
		char *name = (char*)pSectionHdr->Name;
		if (memcmp(name, SegmentName, sizeof(SegmentName)) == 0)
		{
			pSectionInfo = new ImageSectionInfo(SegmentName);
			pSectionInfo->SectionAddress = dllImageBase + pSectionHdr->VirtualAddress;
			pSectionInfo->SectionSize = pSectionHdr->Misc.VirtualSize;
			return pSectionInfo;
		}
		pSectionHdr++;
	}
}
