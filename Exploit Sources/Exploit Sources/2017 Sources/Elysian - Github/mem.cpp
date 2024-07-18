#include "mem.h"
#include "memutil.h"
#include "VMProtectSDK.h"
#include <Windows.h>
#include <vector>

// Memory Checker Bypass 

const int gNopCount = 5;
int gHookLocation = 0x1469D6;
int gHookReturn = 0x146A41;

int gSegmentBase;
int gSegmentEnd;
int gSegmentCopy;

int GetCopyDifference(int base) {
	VMProtectBeginMutation("mem");
	if (base > gSegmentBase && base + 16 < gSegmentEnd)
		return gSegmentCopy - gSegmentBase;

	VMProtectEnd();
	return 0;
}

int gSCopy; /* esp-4 copy */
int gSPCopy; /* stack pointer copy */

/*
void __declspec(naked) Hook() { __asm {
	mov		eax, [ebp - 0Ch]
	cmp		eax, [ebp - 0A4h];
	jnb		hook_hash_finish
	sub		esp, 4
	pop		gSCopy

	push	ecx
	push	edx

	push	[ebp - 0Ch]
	call	GetCopyDifference
	add		esp, 4

	pop		edx
	pop		ecx

	mov		gSPCopy, esp
	mov		esp, eax

	mov     ecx, [ebp - 0Ch]
	mov     edx, [esp+ecx]
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
	mov     ecx, [esp+eax]
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
	mov     eax, [esp+edx]
	xor     eax, [ebp - 0Ch]
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
	sub     edx, [esp+ecx]
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

	mov		esp, gSPCopy

	push	gSCopy
	add		esp, 4

	jmp     Hook
hook_hash_finish:
	jmp gHookReturn
} }*/

void __declspec(naked) Hook() { __asm {
	sub		esp, 4
	pop		gSCopy

	push	ecx 
	push	edx

	push	esi
	call	GetCopyDifference
	add		esp, 4

	pop		edx
	pop		ecx

	mov		gSPCopy, esp
	mov		esp, eax

	mov     eax, [esp+esi]
	add     eax, esi
	imul    eax, 1594FE2Dh
	add     edi, eax
	lea     eax, [esi + 4]
	sub     eax, [esp+esi + 4]
	rol     edi, 13h
	imul    eax, 344B5409h
	imul    edi, 0CBB4ABF7h
	add     eax, [ebp - 14h]
	add     esi, 8
	rol     eax, 11h
	imul    eax, 1594FE2Dh
	mov[ebp - 14h], eax
	mov     eax, [esp+esi]
	xor     eax, esi
	imul    eax, 1594FE2Dh
	add     eax, [ebp - 10h]
	add     esi, 4
	rol     eax, 0Dh
	imul    eax, 0CBB4ABF7h
	mov		[ebp - 10h], eax
	mov     eax, [esp+esi]
	sub     eax, esi
	imul    eax, 344B5409h
	add     ebx, eax
	rol     ebx, 0Fh
	add     esi, 4
	imul    ebx, 1594FE2Dh

	mov		esp, gSPCopy
	
	push	gSCopy
	add		esp, 4

	cmp     esi, ecx
	jb      Hook

	jmp		gHookReturn
} }

void BypassMemoryChecker() {
	gHookReturn = memutil->offset(gHookReturn);
	gHookLocation = memutil->offset(gHookLocation);

	ImageSectionInfo segment;
	if (!memutil->GetSegmentInformation(GetModuleHandle(NULL), ".text", &segment))
		throw std::exception("failed to fetch segment information");

	gSegmentCopy = (int)VirtualAlloc(0, segment.Size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!gSegmentCopy)
		throw std::exception("failed to allocate memory");

	gSegmentBase = (int)segment.Address;
	gSegmentEnd = gSegmentBase + segment.Size;

	memcpy((void*)gSegmentCopy, (void*)gSegmentBase, segment.Size);

	memutil->CodeInjection(gHookLocation, 0, Hook, 0, false, JUMPHOOK);

	// hash code is around 100 bytes
	char junk[80];

	for (int i = 0; i < sizeof(junk); i++)
		junk[i] = rand();

	memutil->WriteMemory(gHookLocation + 5, junk, sizeof(junk), 0, true);
}


// Console Bypass

typedef BOOL(WINAPI *FreeConsole_Def)(void);
FreeConsole_Def FreeConsoleT;
ImageSectionInfo gBoundaryData;

BOOL WINAPI FreeConsoleHook(void) {
	int ret;
	__asm mov eax, [ebp + 4];
	__asm mov ret, eax;

	if (ret > (int)gBoundaryData.Address && ret < (int)gBoundaryData.Address + gBoundaryData.Size)
		return FreeConsoleT();

	return TRUE;
}

int EnableConsole(const char* local_module_name) {
	HMODULE local_module = GetModuleHandle(local_module_name);

	if (!memutil->GetSegmentInformation(local_module, ".text", &gBoundaryData))
		return 0;

	HMODULE kernel_module = GetModuleHandle("KERNELBASE.dll");
	if (!kernel_module) kernel_module = GetModuleHandle("kernel32.dll");

	void* func = GetProcAddress(kernel_module, "FreeConsole");
	FreeConsoleT = (FreeConsole_Def)memutil->CreateDetour(func, FreeConsoleHook);

	return 1;
}