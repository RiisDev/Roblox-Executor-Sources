#ifndef MEMUTIL_H
#define MEMUTIL_H
#pragma once

#include <windows.h>
#include <vector>
#include <map>

typedef struct ImageSectionInfo
{
	char Name[8];//the macro is defined WinNT.h
	void* Address;
	int Size;
};

typedef struct PatchInformation {
	void* Address;
	byte* Memory;
	int Size;
};

typedef struct DetourDef {
	void* targetf;
	void* detourf;
	void* trampolinef;
	int bytecount;
	byte* oldbytes;
};

typedef enum CODEINJ_t {
	JUMPHOOK,
	CALLHOOK,
};

typedef enum SSResult {
	SSRESULT_UNINITIATED,
	SSRESULT_FAILURE,
	SSRESULT_SUCCESS,
};

typedef void(*SSCallback)(void*);

typedef struct SSEntry  {
	int* out;
	const char* aob;
	const char* mask;
	signed int offset;
	SSCallback callback;
	SSResult state;
};

typedef std::map<const char*, SSEntry> map_queue;

#define reg_offset(o) (memutil->register_offset(&o))

class MemoryUtility {
private:
	std::vector<PatchInformation>*	patch_list;
	std::vector<DetourDef>*			detour_list;
	HMODULE							base_module;
	void							destroy_detour(DetourDef* def);

	/* base function used for all writing in the class */
	virtual int						write_memory(int dAddress, const void* patch, int dSize);
public:
	HMODULE							GetBase() { return base_module; }
	int								WriteNOPs(int dAddress, byte dSize);

	/* type:
	0 = jmp
	1 = call
	*/
	int								CodeInjection(int destAddress, byte nopCount, void(*func)(void), int funcLength, byte saveInjection, CODEINJ_t type, PatchInformation** pPatchInfo = 0);
	void*							CreateDetour(void* targetf, void* detourf, int nopCount = 0);
	void							DestroyDetour(void* targetf);

	/* writes patch information to patch_info if doDiscard is true */
	int								WriteMemory(int dAddress, const void* patch, int dSize, byte nopCount, byte doDiscard, PatchInformation** pPatchInfo = 0);
	int								GetSegmentInformation(HMODULE hModule, const char* s_name, ImageSectionInfo* pSectionInfo);
	void*							CopySegment(ImageSectionInfo& seg_info, int page_protection);
	MemoryUtility();
	~MemoryUtility();

	int								offset(int off) { return (int)base_module + off; }
	void							register_offset(void* val) { *(int*)val += (int)base_module; }
};

namespace sigscan {
	int compare(const char* location, const char* aob, const char* mask);

	int scan(const char* aob, const char* mask);

	int scan(const char* aob, const char* mask, int start, int end);

	int scan(const char* module, const char* aob, const char* mask);

	int scan_writable(const char* aob, const char* mask);

	int scan_writable(const char* aob, const char* mask, int start);
};

class SignatureScanner {
private:
	map_queue queue;
	int error_count;
public:
	SignatureScanner();
	void Queue(const char* tag, int* out, const char* aob, const char* mask, signed int offset, SSCallback callback);
	void Queue(const char* tag, int* out, const char* aob, const char* mask, signed int offset);
	void Run();
	int ErrorCount();
	map_queue GetResults();
	void Reset();
};

extern MemoryUtility* memutil;

#endif