#include "memutil.h"
#include <limits.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#define WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY |PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

// Memory Utility

int MemoryUtility::WriteNOPs(int dAddress, byte dSize) {
	if (dSize > 0) {
		byte patch[UCHAR_MAX];
		memset(patch, 0x90, dSize);
		return write_memory(dAddress, patch, dSize);
	}
	return 1;
}

int MemoryUtility::CodeInjection(int destAddress, byte nopCount, void(*func)(void), int funcLength, byte saveInjection, CODEINJ_t type, PatchInformation** pPatchInfo) {
	int offset;
	if (saveInjection) {
		void* allocf = VirtualAlloc(NULL, 1024, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		if (!funcLength) {
			byte* iter = (byte*)func;

			while (*iter != 0xC3)
				++iter;

			++iter;
			memcpy(allocf, func, iter - (byte*)func);
		}
		else {
			memcpy(allocf, func, funcLength);
		}

		offset = (PtrToUlong(allocf) - destAddress) - 5;
	}
	else {
		offset = (PtrToUlong(func) - destAddress) - 5;
	}

	byte patch[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 }; // E8 = call, E9 = jmp 64 bit relative
	patch[0] = type == JUMPHOOK ? 0xE9 : 0xE8;
	memcpy(patch + 1, &offset, sizeof(int));
	return WriteMemory(destAddress, patch, 5, nopCount, !saveInjection, pPatchInfo);
}

int MemoryUtility::WriteMemory(int dAddress, const void* patch, int dSize, byte nopCount, byte doDiscard, PatchInformation** pPatchInfo) {
	int totalSize = dSize + nopCount;

	if (doDiscard) {
		PatchInformation patchInfo;
		byte* originalMemory = new byte[totalSize];

		memcpy(originalMemory, (void*)dAddress, totalSize);
		patchInfo.Address = (void*)dAddress;
		patchInfo.Memory = originalMemory;
		patchInfo.Size = totalSize;

		patch_list->push_back(patchInfo);

		if (pPatchInfo)
			*pPatchInfo = &patch_list->back();
	}

	if (write_memory(dAddress, patch, dSize)) {
		return WriteNOPs(dAddress + dSize, nopCount);
	}

	return 0;
}

int MemoryUtility::write_memory(int dAddress, const void* patch, int dSize) {
	int oldProtect;
	if (VirtualProtect((LPVOID)dAddress, dSize, PAGE_EXECUTE_READWRITE, (PDWORD)&oldProtect)) {
		for (int i = 0; i < dSize; i++)
			((unsigned char*)dAddress)[i] = ((unsigned char*)patch)[i];
		return VirtualProtect((LPVOID)dAddress, dSize, oldProtect, (PDWORD)&oldProtect);
	}
	return 0;
}

/* creates a detour and returns the trampoline function */
void* MemoryUtility::CreateDetour(void* targetf, void* detourf, int nopCount) {
	if (!targetf || !detourf)
		return nullptr;

	/* store old bytes */
	int bytecount = 5 + nopCount;
	byte* oldbytes = new byte[bytecount];
	memcpy(oldbytes, targetf, bytecount);

	/* create jmp to detour patch */
	int offset = PtrToUlong(detourf) - PtrToUlong(targetf) - 5;
	byte patch[5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	memcpy(patch + 1, &offset, sizeof(int));

	/* create trampoline function */
	void* trampolinef = VirtualAlloc(0, bytecount + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!trampolinef)
		return nullptr;

	/* write trampoline function */
	memcpy(trampolinef, oldbytes, bytecount);
	int trampoffset = PtrToUlong(targetf) - (PtrToUlong(trampolinef) + 5) - 5 + 5 - nopCount;
	byte tramppatch[5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	memcpy(tramppatch + 1, &trampoffset, sizeof(int));
	memcpy((void*)(PtrToUlong(trampolinef) + 5 + nopCount), tramppatch, 5);

	/* hook target function */
	write_memory(PtrToUlong(targetf), patch, 5);

	/* write nops (if necessary) */
	WriteNOPs(PtrToUlong(targetf) + 5, nopCount);

	/* create detour def and add to list */
	DetourDef def;
	def.targetf = targetf;
	def.detourf = detourf;
	def.trampolinef = trampolinef;
	def.oldbytes = oldbytes;
	def.bytecount = bytecount;

	detour_list->push_back(def);

	/* return trampoline */
	return trampolinef;
}

void MemoryUtility::destroy_detour(DetourDef* def) {
	/* free allocated trampoline function */
	VirtualFree(def->trampolinef, def->bytecount + 5, MEM_RELEASE);

	/* restore target function */
	write_memory((int)def->targetf, def->oldbytes, def->bytecount);

	/* free allocated old bytes */
	delete[] def->oldbytes;
}

void MemoryUtility::DestroyDetour(void* targetf) {
	for (std::vector<DetourDef>::iterator it = detour_list->begin(); it != detour_list->end(); it++) {
		if (it->targetf == targetf) {
			/* destroy */
			destroy_detour(&*it);

			/* erase detourdef */
			detour_list->erase(it);
			break;
		}
	}
}

int MemoryUtility::GetSegmentInformation(HMODULE hModule, const char* s_name, ImageSectionInfo* pSectionInfo) {
	memset(pSectionInfo, 0, sizeof(ImageSectionInfo));

	//store the base address the loaded Module
	int dllImageBase = (int)hModule; //suppose hModule is the handle to the loaded Module (.exe or .dll)

	//get the address of NT Header
	IMAGE_NT_HEADERS *pNtHdr = ImageNtHeader(hModule);

	//after Nt headers comes the table of section, so get the addess of section table
	IMAGE_SECTION_HEADER *pSectionHdr = (IMAGE_SECTION_HEADER *)(pNtHdr + 1);

	//iterate through the list of all sections, and check the section name in the if conditon. etc
	for (int i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++)
	{
		char *name = (char*)pSectionHdr->Name;
		if (strcmp(name, s_name) == 0)
		{
			pSectionInfo->Address = (void*)(dllImageBase + pSectionHdr->VirtualAddress);
			pSectionInfo->Size = pSectionHdr->Misc.VirtualSize;
			strcpy(pSectionInfo->Name, name);
			return 1;
		}
		pSectionHdr++;
	}
	return 0;
}

void* MemoryUtility::CopySegment(ImageSectionInfo& seg_info, int page_protection) {
	void* seg_cpy = VirtualAlloc(0, seg_info.Size, MEM_COMMIT | MEM_RESERVE, page_protection);

	if (seg_cpy) {
		memcpy(seg_cpy, seg_info.Address, seg_info.Size);

		return seg_cpy;
	}

	return 0;
}

MemoryUtility::MemoryUtility() {
	base_module = GetModuleHandle(NULL);
	patch_list = new std::vector < PatchInformation >;
	detour_list = new std::vector < DetourDef >;

	//AddVectoredExceptionHandler(true, VectoredHandler);
}

MemoryUtility::~MemoryUtility() {
	/* clean patch_list */
	for (int i = 0; i < patch_list->size(); i++) {
		PatchInformation patch_info = patch_list->at(i);
		WriteMemory((int)patch_info.Address, patch_info.Memory, patch_info.Size, 0, false);
		delete[] patch_info.Memory;
	}

	/* clean detour_list */
	for (std::vector<DetourDef>::iterator it = detour_list->begin(); it != detour_list->end(); it++) {
		destroy_detour(&*it);
	}

	delete patch_list;
	delete detour_list;
}

namespace sigscan {
	int compare(const char* location, const char* aob, const char* mask) {
		try {
			for (; *mask; ++aob, ++mask, ++location) {
				if (*mask == 'x' && *location != *aob)
					return 0;
			}
		}
		catch (...) {
			return 0;
		}
		return 1;
	};

	int scan(const char* aob, const char* mask, int start, int end) {
		for (; start <= end; ++start) {
			if (compare((char*)start, (char*)aob, mask))
				return start;
		}
		return 0;
	};

	int scan(const char* aob, const char* mask) {
		return scan(aob, mask, (int)GetModuleHandle(0), 0xF00000);
	}

	int scan(const char* module, const char* aob, const char* mask) {
		MODULEINFO info;
		if (GetModuleInformation(GetCurrentProcess(), GetModuleHandle(module), &info, sizeof(info)))
			return scan(aob, mask, (int)info.lpBaseOfDll, (int)info.lpBaseOfDll + info.SizeOfImage);

		return 0;
	}

	int scan_writable(const char* aob, const char* mask, int start) {
		if (!start)
			start = (int)GetModuleHandle(0);

		HANDLE proc = GetCurrentProcess();
		MEMORY_BASIC_INFORMATION memoryInfo;
		DWORD regionEnd = 0;
		DWORD maskLen = strlen(mask);
		for (int i = start; i <= 0x4FFFFFFF; ++i) {
			if (i + maskLen > regionEnd)
				i += maskLen;

			if (i > regionEnd || !regionEnd) {
				ZeroMemory(&memoryInfo, sizeof(memoryInfo));
				VirtualQuery((DWORD*)i, &memoryInfo, sizeof(memoryInfo));
				regionEnd = i + memoryInfo.RegionSize;
				if (!((memoryInfo.State & MEM_COMMIT) && (memoryInfo.Protect & WRITABLE))) {
					i = regionEnd + 1;
					continue;
				}
			}

			if (compare((char*)i, (char*)aob, mask)) {
				return i;
			}
		}
		return 0;
	}

	int scan_writable(const char* aob, const char* mask) {
		return scan_writable(aob, mask, 0);
	}


}

SignatureScanner::SignatureScanner() : error_count(0) {};

void SignatureScanner::Queue(const char* tag, int* out, const char* aob, const char* mask, signed int offset, SSCallback callback) {
	SSEntry entry;
	entry.out = out;
	entry.aob = aob;
	entry.mask = mask;
	entry.callback = callback;
	entry.offset = offset;
	entry.state = SSRESULT_UNINITIATED;

	queue[tag] = entry;
}

void SignatureScanner::Queue(const char* tag, int* out, const char* aob, const char* mask, signed int offset) {
	Queue(tag, out, aob, mask, offset, 0);
}

void SignatureScanner::Run() {
	map_queue::iterator it = queue.begin();
	for (; it != queue.end(); it++) {
		SSEntry* entry = &it->second;
		if (entry->state == SSRESULT_UNINITIATED) {
			int* out = entry->out;

			*out = sigscan::scan(NULL, entry->aob, entry->mask);
			if (*out) {
				entry->state = SSRESULT_SUCCESS;
				*out += entry->offset;

				if (*(unsigned char*)(*out) != 0x55) {
					printf("sigscan: warning for '%s', *%x is 0x%x instead of 0x55!", it->first, *out, *(unsigned char*)(*out));
				}
			}
			else {
				error_count++;
				entry->state = SSRESULT_FAILURE;
			}

			if (entry->callback)
				entry->callback(entry);
		}
	}
}

int SignatureScanner::ErrorCount() {
	return error_count;
}

map_queue SignatureScanner::GetResults() {
	return queue;
}

void SignatureScanner::Reset() {
	queue.clear();
	error_count = 0;
}

MemoryUtility lmemutil;
MemoryUtility* memutil = &lmemutil;