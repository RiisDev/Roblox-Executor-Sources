#include <stdexcept>
#include <tuple>
#include <typeinfo>
#include <windows.h>
#include <strsafe.h>
#include <string>
#include <iostream>
#include <thread>

extern "C" {
#include "lua\lua.h"
#include "lua\lualib.h"
#include "lua\lfunc.h"
#include "lua\lobject.h"
#include "lua\lmem.h"
#include "lua\lauxlib.h"
#include "lua\lstate.h"
}

lua_State* lua_state;
int rState, ScriptContext, int3bpt;

BOOL compare(const BYTE* location, const BYTE* aob, const char* mask) {
	for (; *mask; ++aob, ++mask, ++location) {
		__try {
			if (*mask == 'x' && *location != *aob)
				return 0;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return 0;
		}
	}
	return 1;
}

DWORD scan(const char* aob, const char* mask) {
	for (DWORD i = (DWORD)GetModuleHandle(NULL); i <= 0xF000000; ++i) {
		if (compare((BYTE*)i, (BYTE*)aob, mask))
			return i;
	}
	return 0;
}

DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
{
	for (int i = 0; i<(int)dwLen; i++)
		if (compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
	return 0;
}

bool CompareData(const char* Data, const char* Pattern, const char* Mask) {
	while (*Mask) {
		if (*Mask != '?') {
			if (*Data != *Pattern) {
				return false;
			};
		};
		++Mask;
		++Data;
		++Pattern;
	};
	return true;
};

DWORD ScanForScriptContext(const char* SCVFT_Offsetted) {
	MEMORY_BASIC_INFORMATION BasicMemoryInformation = {};
	SYSTEM_INFO SystemInformation = {};
	GetSystemInfo(&SystemInformation);
	DWORD StartingMemorySearchPosition = (DWORD)SystemInformation.lpMinimumApplicationAddress;
	DWORD MaximumSearchBoundary = (DWORD)SystemInformation.lpMaximumApplicationAddress;
	do {
		while (VirtualQuery((void*)StartingMemorySearchPosition, &BasicMemoryInformation, sizeof(BasicMemoryInformation))) {
			if ((BasicMemoryInformation.Protect & PAGE_READWRITE) && !(BasicMemoryInformation.Protect & PAGE_GUARD)) {
				for (DWORD Key = (DWORD)(BasicMemoryInformation.BaseAddress); ((Key - (DWORD)(BasicMemoryInformation.BaseAddress) < BasicMemoryInformation.RegionSize)); ++Key) {
					if (CompareData((const char*)Key, SCVFT_Offsetted, "xxxx")) {
						return Key;
					};
				};
			};
			StartingMemorySearchPosition += BasicMemoryInformation.RegionSize;
		};
	} while (StartingMemorySearchPosition < MaximumSearchBoundary);
	return 0x0;
};


int Scan(DWORD mode, char* content, char* mask)
{
	DWORD PageSize;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	PageSize = si.dwPageSize;
	MEMORY_BASIC_INFORMATION mi;
	for (DWORD lpAddr = (DWORD)GetModuleHandle(NULL); lpAddr<0xF000000; lpAddr += PageSize)
	{
		DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
		if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
		if (mi.Type == MEM_MAPPED) continue;
		if (mi.Protect == mode)
		{
			int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
			if (addr != 0)
			{
				return addr;
			}
		}
	}

}

DWORD unprotect(DWORD addr)
{
	BYTE* tAddr = (BYTE *)addr;

	do {
		tAddr += 0x10;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	memcpy(nFunc, (void*)addr, funcSz);
	DWORD pos = (DWORD)nFunc;
	BOOL valid = false;
	do {
		if ((*(BYTE*)pos == 0x72 && (*(BYTE*)(pos + 0x2)) == 0xA1 && (*(BYTE*)(pos + 0x7)) == 0x8B) || (*(BYTE*)pos == 0x72 && (*(BYTE*)(pos + 0x1)) == 0x44)) {
			memcpy((void*)pos, "\xEB", 1);

			DWORD cNFunc = (DWORD)nFunc;
			do {
				if (*(BYTE*)cNFunc == 0xE8)
				{
					DWORD tFunc = addr + (cNFunc - (DWORD)nFunc);
					DWORD oFunc = (tFunc + *(DWORD*)(tFunc + 1)) + 5;

					if (oFunc % 16 == 0)
					{
						DWORD realCAddr = oFunc - cNFunc - 5;
						*(DWORD*)(cNFunc + 1) = realCAddr;
					}
					cNFunc += 5;
				}
				else
					cNFunc += 1;
			} while (cNFunc - (DWORD)nFunc < funcSz);

			valid = true;
		}
		pos += 1;
	} while (pos < (DWORD)nFunc + funcSz);

	if (!valid) {
		VirtualFree(nFunc, funcSz, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}

#define ScanAddress(Address)		 (Address - 0x400000 + (DWORD)GetModuleHandle("RobloxPlayerBeta.exe"))
#define ScanAddress1(Address)		 (Address + 0x400000 - (DWORD)GetModuleHandle("RobloxPlayerBeta.exe"))
#define rlua_getglobal(l,g)			 rlua_getfield(l, LUA_GLOBALSINDEX, g)
#define rlua_newtable(L)			 rlua_createtable(L,0,0)
#define rluaS_newstr(RL,str)		 rluaS_newlstr(RL, str, strlen(str))
#define rlua_isnil(L,n)			     (rlua_type(L, (n)) == 0)
#define rlua_clearstack(L)           rlua_settop(L,0) 
#define RLUA_TNONE					 (-1)
#define RLUA_TNIL                     0
#define RLUA_TLIGHTUSERDATA		      1
#define RLUA_TNUMBER                  2
#define RLUA_TBOOLEAN                 3
#define RLUA_TSTRING                  4
#define RLUA_TTHREAD                  5
#define RLUA_TFUNCTION				  6
#define RLUA_TTABLE                   7
#define RLUA_TUSERDATA			      8
#define RLUA_TPROTO                   9
#define RLUA_TUPVALUE                 10


/*typdefs are needed for signature checking, would use auto*/
typedef TValue*(__cdecl *Llua_index2adr)(int lst, int idx); // updated
Llua_index2adr rlua_index2adr;

//typedef void(__stdcall *Lua_getfield)(int lst, int index, const char *k, char x); // updated
//Lua_getfield rlua_getfield_real;

typedef void(__stdcall *Lua_getfield)(int lst, int index, const char *k); // updated
Lua_getfield rlua_getfield_real;

typedef int(__stdcall *Lua_setfield)(int lst, int index, const char *x); // updated
Lua_setfield rlua_setfield_r;

typedef int(__cdecl *Lua_getmetatable)(int lst, int idx); // updated
Lua_getmetatable rlua_getmetatable;

typedef const char*(__cdecl *Llua_tolstring)(int lst, int idx, int size); // updated
Llua_tolstring rlua_tolstring;

typedef DWORD(__cdecl* Llua_rawgeti)(int lst, int idx, int n); // updated
Llua_rawgeti rlua_rawgeti_real;

typedef DWORD(__cdecl* Llua_rawseti)(int lst, int idx, int n); // updated
Llua_rawseti rlua_rawseti;

typedef void*(__cdecl* Llua_createtable)(int L, int narray, int nrec); // updated
Llua_createtable rlua_createtable;

typedef int *(__cdecl *Identity)();
Identity r_identity;///////////////////////////////////////////////////////////////////////////

typedef int(__cdecl* Rlua_pcall)(int, int, int, int); // updated
Rlua_pcall rlua_pcall_r;

typedef void*(__cdecl* pcc)(int, int, int); // updated
pcc rlua_pushcclosure;

typedef int(__cdecl* nxt)(int, int); // updated
nxt rlua_next;

typedef int*(__cdecl* stt)(int, int); // updated
stt rlua_settable;

typedef int*(__cdecl *ona)(int, int, const char*); // updated
ona rlua_getmetafield_real;

//typedef int(__cdecl *err)(int, const char*);
//err rlua_error;

typedef const char*(__stdcall *Llua_pushnumber)(int lst, double num); // updated
Llua_pushnumber rlua_pushnumber;

typedef int(__fastcall *LuaS_newlstr)(int RL, const char *str, size_t l); // updated
LuaS_newlstr rluaS_newlstr;

 int mabs(int num) {
	if (num > 0)
		return num;
	else if (num < 0)
		return -num;
	else
		return 0;
}

void toClipboard(const std::string &s) {
	OpenClipboard(GetDesktopWindow());
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

 inline bool rlua_getmetafield(int lst, int idx, const char * k) {
	bool ret;
	__try {
		rlua_getmetafield_real(lst, idx, k);
		ret = true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		ret = false;
	}
	return ret;
}

 inline bool rlua_rawgeti(int lst, int idx, int a) {
	bool ret;
	__try {
		rlua_rawgeti_real(lst, idx, a);
		ret = true;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		ret = false;
	}
	return ret;
}

/*more credits to eternal*/
 void fakeChain(DWORD* chain)
 {
	 chain[1] = 0x1555555;
	 if ((((DWORD*)chain[0])[1]) != NULL) {
		 ((DWORD*)chain[0])[1] = 0x1555555;
	 }
 }
 void restoreChain(DWORD* chain, DWORD unk, DWORD nextUnk)
 {
	 chain[1] = unk;
	 if ((((DWORD*)chain[0])[1]) != NULL) {
		 ((DWORD*)chain[0])[1] = nextUnk;
	 }
 }

int rlua_pcall(DWORD rL, int nargs, int nresults, int errfunc)
{
	DWORD* exceptionChain = (DWORD*)__readfsdword(0);
	DWORD unk = exceptionChain[1];
	((((DWORD*)exceptionChain[0])[1]) != NULL );
	{
		DWORD nextUnk = ((DWORD*)exceptionChain[0])[1];
		fakeChain(exceptionChain);
		int ret = rlua_pcall_r(rL, nargs, nresults, errfunc);
		restoreChain(exceptionChain, unk, nextUnk);
		return ret;
	}

	fakeChain(exceptionChain);
	int ret = rlua_pcall_r(rL, nargs, nresults, errfunc);
	restoreChain(exceptionChain, unk, 0);
	return ret;
}

void rlua_setfield(int rL, int idx, const char *val) {
	DWORD* exceptionChain = (DWORD*)__readfsdword(0);
	DWORD unk = exceptionChain[1];
	((((DWORD*)exceptionChain[0])[1]) != NULL);
	{
		DWORD nextUnk = ((DWORD*)exceptionChain[0])[1];
		fakeChain(exceptionChain);
		rlua_setfield_r(rL, idx, val);
		restoreChain(exceptionChain, unk, nextUnk);
	}

	fakeChain(exceptionChain);
	int ret = rlua_setfield_r(rL, idx, val);
	restoreChain(exceptionChain, unk, 0);
}
/*more credits to eternal*/

struct calldata {
	lua_State *L;
	LClosure *f;
};

int int3fnhandler(DWORD rL)
{
	typedef int(*_handle)(DWORD rL);
	_handle handle = (_handle)(rlua_index2adr(rL, lua_upvalueindex(1))->value.p);
	return handle(rL);
}

LONG WINAPI handlerfunctions(PEXCEPTION_POINTERS ex)
{
	if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		if (ex->ContextRecord->Eip == int3bpt) //set at EIP
		{
			ex->ContextRecord->Eip = (DWORD)(int3fnhandler);//(DWORD)(int3fnhandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

/*TValue *rlua_StateHook(int rL, int idx) {
	DWORD oldp;
	rState = rL;
	//std::cout << cast_int(r->top - r->base) << std::endl;
	unsigned char unhookopcodes[11] = {
		0x55,              //push ebp
		0x8B, 0xEC,          //mov     ebp, esp
		0x56,              //push    esi 
		0x8B, 0x75, 0x0C, //mov     esi, [ebp+arg_4]
		0x85, 0xF6,          //test    esi, esi
		0x7E, 0x1C          //jle     short addr 
	};
	VirtualProtect((LPVOID)rlua_index2adr_adr, 5, PAGE_EXECUTE_READWRITE, &oldp);
	memcpy((void*)rlua_index2adr_adr, unhookopcodes, sizeof(unhookopcodes));
	VirtualProtect((LPVOID)rlua_index2adr_adr, 5, oldp, &oldp);
	auto rlua_iindex2adr = reinterpret_cast<TValue*(__fastcall*)(int, int)>(rlua_index2adr_adr);
	return rlua_iindex2adr(rL, idx);
}*/

DWORD findbreakpoint(int offset = 0) {
	for (int i = reinterpret_cast<DWORD>(GetModuleHandle(NULL) + offset); i < 0x7FFFFFFF; ++i) {
		PBYTE pI = reinterpret_cast<PBYTE>(i);
		if (pI[0] == 0xCC && pI[1] == 0xCC && pI[2] == 0xCC) {
			return i;
		}
	}
}

void BypassCallCheck()
{
	int3bpt = findbreakpoint();
	AddVectoredExceptionHandler(TRUE, handlerfunctions);
}