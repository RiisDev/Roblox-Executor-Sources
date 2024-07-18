#pragma once
#include <Windows.h>
#include <string.h>
#include <iostream>

/* Defines for ScriptContext and GetDataModel*/

DWORD rL;
DWORD GetDataModel_OnState;


#define aslr(x) (x - 0x400000 + (DWORD)GetModuleHandle(0))

DWORD unprotect(DWORD addr)
{
	BYTE* tAddr = (BYTE*)addr;

	do {
		tAddr += 0x10;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	memcpy(nFunc, (BYTE*)addr, funcSz);

	DWORD pos = (DWORD)nFunc;
	BOOL valid = false;
	do {
		if (*(BYTE*)pos == 0x72 && *(BYTE*)(pos + 0x2) == 0xA1 && *(BYTE*)(pos + 0x7) == 0x8B) {
			memcpy((void*)pos, "\xEB", 1);

			DWORD cNFunc = (DWORD)nFunc;
			do {
				if (*(BYTE*)cNFunc == 0xE8)
				{
					DWORD tFunc = addr + (cNFunc - (DWORD)nFunc);
					DWORD oFunc = (tFunc + *(DWORD*)(tFunc + 1)) + 5;

					if (oFunc % 16 == 0)
					{
						DWORD rebFAddr = oFunc - cNFunc - 5;
						*(DWORD*)(cNFunc + 1) = rebFAddr;
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

typedef int(__fastcall* lua_getfield)(DWORD rL, int index, const char* k);
typedef DWORD(__cdecl* getdatamodel_bypass)();
typedef DWORD(__thiscall* getdatamodel)(DWORD, DWORD);
typedef DWORD(__thiscall* findLuaState)(DWORD, int* ptr);
typedef void(__cdecl* lua_pushnumber)(DWORD rL, double i);
typedef void(__cdecl* lua_setfield)(DWORD rL, int i, const char* k);
typedef void(__stdcall* lua_settop)(DWORD rL, int i);
typedef int(__stdcall* lua_pushvalue)(DWORD ls, int i);
typedef int(__cdecl* a_getmetatable)(DWORD ls, int i);
typedef void(__cdecl* a_pushstring)(DWORD ls, const char* k);
typedef void(__cdecl* lua_pcall)(DWORD rL, DWORD arg1, DWORD arg2, DWORD arg3);
typedef void(__cdecl* a_pushboolean)(DWORD ls, bool k);

#define r_lua_pop(a, b) (RBLX::r_lua_settop(a, -(b)-1))

namespace RBLX
{
	findLuaState r_findls;
	lua_getfield r_lua_getfield;
	getdatamodel_bypass r_getdatamodel_bypass;
	getdatamodel r_getdatamodel;
	lua_pushnumber r_lua_pushnumber;
	lua_setfield r_lua_setfield;
	lua_settop r_lua_settop;
	lua_pushvalue r_lua_pushvalue;
	a_getmetatable r_lua_getmetatable;
	a_pushstring r_lua_pushstring;
	lua_pcall r_lua_pcall;
	a_pushboolean r_lua_pushboolean;
	/* Define the Address using the calling conventions */
	void r_lua_getglobal(DWORD rL, const char* epic)
	{
		r_lua_getfield(rL, -10002, epic);
		printf("Got: (%s) \n", epic);
	}

	void Init()
	{
		/* Init the offsets for the Exploit to work.*/
		r_findls = (findLuaState)(aslr(0x782660));
		r_getdatamodel_bypass = (getdatamodel_bypass)(aslr(0xF50930));
		r_getdatamodel = (getdatamodel)(aslr(0xF50A80));
		r_lua_getfield = (lua_getfield)(unprotect(aslr(0x147A160)));
		r_lua_pushnumber = (lua_pushnumber)(unprotect(aslr(0x147ACC0)));
		r_lua_setfield = (lua_setfield)(unprotect(aslr(0x147B640)));
		r_lua_settop = (lua_settop)(unprotect(aslr(0x147B960)));
		r_lua_getmetatable = (a_getmetatable)(unprotect(aslr(0x147A200)));
		r_lua_pushvalue = (lua_pushvalue)(unprotect(aslr(0x147AED0)));
		r_lua_pushstring = (a_pushstring)(unprotect(aslr(0x147AD20)));
		r_lua_pcall = (lua_pcall)(unprotect(aslr(0x147A8E0)));
		r_lua_pushboolean = (a_pushboolean)(unprotect(aslr(0x147A9A0)));
		/* Big brain rlua! */
	}
	
}

namespace RobloxDataModel
{
	uint32_t getDataModel()
	{
		uint32_t Array[2]{};
		RBLX::r_getdatamodel(RBLX::r_getdatamodel_bypass(), reinterpret_cast<DWORD>(Array));
		return Array[0] + 0xC;
	}

	const char* GetClass(int self)
	{
		return (const char*)(*(int(**)(void))(*(int*)self + 16))();
	}

	int FindFirstClass(int Instance, const char* Name)
	{
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);

		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
			{
				return *(int*)i;
			}
		}
	}
}

