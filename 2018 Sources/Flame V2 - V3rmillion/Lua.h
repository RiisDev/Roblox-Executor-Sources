#pragma once

/* Includes */

#include "Data.h"

inline void SetTop(int L, int idx) {
	try {
		typedef void(__cdecl *lua_settop)(int, int);
		((lua_settop)(SETTOP_ADDRESS))(L, idx);
	}
	catch (...) {};
}

inline void PushNil(int L)
{
	try {
		*(int*)(*(int*)(L + 16) + 8) = 0;
		*(int*)(L + 16) += 16;
	}
	catch (...) {};
}

inline void GetField(int L, int index, const char* k) {
	try {
		typedef void(__cdecl *lua_getfield)(int, int, const char*);
		((lua_getfield)(GETFIELD_ADDRESS))(L, index, k);
	}
	catch (...) {};
}

inline void GetGlobal(int L, const char* k) {
	try {
		typedef void(__cdecl *lua_getglobal)(int, int, const char*);
		((lua_getglobal)(GETFIELD_ADDRESS))(L, -10002, k);
	}
	catch (...) {};
}

inline double XOR_Constant(double* num)
{
	double stackNumb;
	void *oldnum = num;

	__asm
	{
		mov edx, oldnum
		mov eax, XORCONST_Flag
		movsd xmm1, qword ptr[edx]
		xorpd xmm1, [eax]
		movsd stackNumb, xmm1
	}

	return stackNumb;
}

inline void PushNumber(int L, double n) {
	try {
		int top = *(DWORD*)(L + 16);
		*(DWORD*)(top + 8) = 2;
		*(double*)top = XOR_Constant(&n);
		*(DWORD*)(L + 16) += 16;
	}
	catch (...) {};
}

inline void PushBoolean(int L, double b) {
	try {
		int top = *(DWORD*)(L + 16);
		*(DWORD*)(top + 8) = 3;
		*(DWORD*)top = b != 0;
		*(DWORD*)(L + 16) += 16;
	}
	catch (...) {};
}

inline void PushValue(int L, int index) {
	try {
		typedef void(__cdecl *lua_pushvalue)(int, int);
		((lua_pushvalue)PUSHVALUE_ADDRESS)(L, index);
	}
	catch (...) {};
}

inline const char* PushString(int L, const char* s) {
	try {
		typedef const char*(__cdecl *lua_pushstring)(int, const char*);
		((lua_pushstring)(PUSHSTRING_ADDRESS))(L, s);
	}
	catch (...) {};

	return 0;
}


inline void SetField(int L, int idx, const char* k) {
	try {
		typedef void(__cdecl *lua_setfield)(int, int, const char*);
		((lua_setfield)(SETFIELD_ADDRESS))(L, idx, k);
	}
	catch (...) {};
}

inline int Index2Adr(int L, int idx)
{
	int result = 0;
	if (idx > 0)
	{
		if ((unsigned int)(16 * idx + *(int*)(L + 28) - 16) < *(int*)(L + 16))
			result = 16 * idx + *(int*)(L + 28) - 16;
	}
	else if (idx > -10000)
		result = *(int*)(L + 16) + 16 * idx;

	return result;
}

inline char* ToString(int L, int idx)
{
	int stkid = Index2Adr(L, idx);
	if (stkid && *(int*)(stkid + 8) == LUA_TSTRING)
		return *(char**)stkid + 24;
	return 0;
}

inline double ToNumber(DWORD RL, int idx)
{
	int stkid = Index2Adr(RL, idx);
	int n = *(int*)(stkid + 8);
	if (n != LUA_TNUMBER)
		return 0;
	return XOR_Constant((double*)stkid);
}


inline void Pcall(int L, int nargs, int nresults, int errfunc) {
	try {
		typedef void(__cdecl *lua_pcall)(int, int, int, int);
		((lua_pcall)(PCALL_ADDRESS))(L, nargs, nresults, errfunc);
	}
	catch (...) {};
}

inline int Type(int L, int idx)
{
	try {
		int stkid = Index2Adr(L, idx);
		if (stkid) {
			return *(int*)(stkid + 8);
		}
	}
	catch (...) {};
}


inline void GetService(int L, const char* s)
{
	GetGlobal(L, "game");
	GetField(L, -1, "GetService");
	PushValue(L, -2);
	PushString(L, s);
	Pcall(L, 2, 1, 0);
}

inline void PrintIdentity(int L)
{
	GetGlobal(L, "printidentity");
	Pcall(L, 0, 0, 0);
}

namespace CFrame
{
	void New(int L, int x, int y, int z, int R00, int R01, int R02, int R10, int R11, int R12, int R20, int R21, int R22)
	{
		GetGlobal(L, "CFrame");
		GetField(L, -1, "new");
		PushNumber(L, x);
		PushNumber(L, y);
		PushNumber(L, z);
		PushNumber(L, R00);
		PushNumber(L, R01);
		PushNumber(L, R02);
		PushNumber(L, R10);
		PushNumber(L, R11);
		PushNumber(L, R12);
		PushNumber(L, R20);
		PushNumber(L, R21);
		PushNumber(L, R22);
		Pcall(L, 12, 1, 0);
	}
}

namespace Vector3
{
	void New(int L, int X, int Y, int Z)
	{
		GetGlobal(L, "Vector3");
		GetField(L, -1, "new");
		PushNumber(L, X);
		PushNumber(L, Y);
		PushNumber(L, Z);
		Pcall(L, 3, 1, 0);
	}
}

static int GetDistance(int v1, int v2)
{
	return v1 - v2;
}

#define Pop(L, idx) SetTop(L, -(idx)-1)
#define EmptyStack(L)       SetTop(L, 0)