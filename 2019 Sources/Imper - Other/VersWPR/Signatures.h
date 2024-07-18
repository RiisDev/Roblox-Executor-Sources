#pragma once
#include <Windows.h>
#include "Vitals.h"
#include "Utility.h"
#include "Retcheck.h"

Ret *returnCheck;

unsigned int CLog(int MessageType, const char *msg, ...) {
	typedef unsigned int(*err)(int a1, const char* a2, ...);
	err arg = (err)sigscan::scanaob("\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83\xEC\x30\x8D\x45\x10", "xxxxxx????xx????xxxx????xxxxxx");
	return arg(MessageType, msg);
}

int mabs(int num) {
	if (num > 0)
		return num;
	else if (num < 0)
		return -num;
	else
		return 0;
}

inline void getmetafield(int lst, int idx, const char * k) {
	__try {
		rlua_getmetafield_real(lst, idx, k);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		pushnil(lst);
	}
}

void fakeChain(DWORD* chain)
{
	chain[1] = 0x1555555;
	if ((((DWORD*)chain[0])[1]) != NULL) {
		((DWORD*)chain[0])[1] = 0x1555555;
	}
	else {
		((DWORD*)chain[0])[1] = NULL;
	}
}

void restoreChain(DWORD* chain, DWORD unk, DWORD nextUnk)
{
	chain[1] = unk;
	if ((((DWORD*)chain[0])[1]) != NULL) {
		((DWORD*)chain[0])[1] = nextUnk;
	}
	else {
		((DWORD*)chain[0])[1] = NULL;
	}
}

int pcall(DWORD rL, int nargs, int nresults, int errfunc)
{
	
}