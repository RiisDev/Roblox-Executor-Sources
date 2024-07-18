#include "RbxDefs.h"
#include "SigScanner.h"
//#include "VMProtectSDK.h"
#include "Update.h"

int ggs;
int deserialize;
int resume;
int newthread;
int setfield;
int spawn;
int precallflag;

int unksub1;
int unksub2;
int rets[4];

void LocateRbxDefs() {
rets[0] = (int)GetModuleHandle(NULL) + 0x1AF0; // C3 -- C3 55 8B EC 64 A1 ?? ?? ?? ?? 6A FF 68 2E BB E8
	rets[1] = (int)GetModuleHandle(NULL) + 0x44F67;// 83 C4 04 C3 
	rets[2] = (int)GetModuleHandle(NULL) + 0x48288;  // 83 C4 08 C3
	rets[3] = (int)GetModuleHandle(NULL) + 0x2BF7E; // 83 C4 0C C3
	rets[4] = (int)GetModuleHandle(NULL) + 0x08FB0; // 83 C4 10 C3
	SigScanner s; //wrong addy
	spawn = (int)GetModuleHandle(0); +0x25D0D0; //55 8B EC 6A FF 68 D6 E1 E8 ?? 64 A1 ?? ?? ?? ??
	resume = s.ScanNoAlign("\x55\x8B\xEC\x56\x8B\x75\x08\x8A\x46\x06\x3C\x01\x74", 13); /*55 8B EC 56 8B 75 08 8A 46 06 3C 01 74*/
	ggs = s.ScanNoAlign("\x55\x8B\xEC\x56\x57\x6A\x05", 7); //55 8B EC 56 57 6A 05
	deserialize = (s.ScanNoAlign("\x56\x57\x6A\xFF\x6A\x00\x51\x8D\x4D\xCC\xC7\x45\xE0\x0F\x00\x00", 16) - 0xFC); //8B 4D FC 8B 45 0C FF 49 08 83 C4 04 43 39 07 75 9D
	newthread = (s.ScanNoAlign("\x51\x56\x8B\x75\x08\x57\x8B\x4E\x08\x8B\x44\x31\x60\x3B\x44\x31\x54", 17) - 24); //51 56 8B 75 08 57 8B 4E 08 8B 44 31 60 3B 44 31 54
	unksub1 = s.ScanNoAlign("\x55\x8B\xEC\x83\xEC\x08\x8B\x55\x08\x53\x8D\x4A\x08", 13); //55 8B EC 83 EC 08 8B 55 08 53 8D 4A 08
	unksub2 = s.ScanNoAlign("\x55\x8B\xEC\x53\x8B\x5D\x0C\x56\x57\x8B\x7D\x08\xC1\xE3\x04", 15); //55 8B EC 53 8B 5D 0C 56 57 8B 7D 08 C1 E3 04
	setfield = (s.ScanNoAlign("\x55\x8B\xEC\x8B\x45\x0C\x85\xC0\x74\x07", 10) - 0xB0); //55 8B EC 8B 45 0C 85 C0 74 07
	precallflag = CallCheckAddress;
}


// genuine monstrosity
__declspec(naked) int rbx_pushcfunction(int one, int two, int three) {
	__asm {
		push ebp
		mov ebp, esp
		push esi
		mov esi, [ebp + 08h]
		mov ecx, [esi + 08h]
		mov eax, [ecx + esi + 60h]
		cmp eax, [ecx + esi + 54h]
		jb loc3
		push esi
		call unksub1
		add esp, 04h
		loc3 :
		mov eax, [esi + 0Ch]
			cmp eax, [esi + 28h]
			jne loc1
			mov eax, [esi + 68h]
			jmp loc2
			loc1 :
		mov eax, [eax + 10h]
			mov eax, [eax]
			mov eax, [eax + 0Ch]
			loc2 :
			push ebx
			push edi
			mov edi, [ebp + 10h]
			push eax
			push edi
			push esi
			call unksub2
			mov ebx, [ebp + 0Ch]
			lea edx, [eax + 10h]
			mov ecx, ebx
			sub ecx, edx
			mov[edx], ecx
			mov edx, edi
			shl edx, 04h
			sub[esi + 10h], edx
			mov[ebp + 08h], eax
			mov eax, 0
			mov ecx, ebx
			sub ecx, eax
			add esp, 0Ch
			mov[ebp + 10h], edx
			test edi, edi
			je endlupe
			add edx, 18
			add edx, [ebp + 08h]
			mov ebx, 0FFFFFFE8h
			sub ebx, [ebp + 08h]
			lupe:
		mov ecx, [esi + 10h]
			lea edx, [edx - 10h]
			add ecx, ebx
			mov eax, [ecx + edx]
			mov[edx], eax
			mov eax, [ecx + edx + 04h]
			mov[edx + 04h], eax
			mov eax, [ecx + edx + 08h]
			mov[edx + 08h], eax
			dec edi
			jne lupe
			endlupe :
		mov eax, [esi + 10h]
			mov ecx, [ebp + 08h]
			mov[eax], ecx
			mov[eax + 08h], 00000006h
			add dword ptr[esi + 10h], 10h
			mov eax, 0
			mov ecx, [ebp + 04h]
			sub ecx, eax
			pop edi
			pop ebx
			pop esi
			pop ebp
			ret
	}
}