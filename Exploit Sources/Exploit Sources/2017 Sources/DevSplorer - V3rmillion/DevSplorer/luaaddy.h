#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <tchar.h>
#include <chrono>
#include <urlmon.h>
#include <Lmcons.h>
#include <direct.h>
#include <stdio.h>
#include <sys/types.h>
#include <shlobj.h>
#include <map>

const char* aob_tolstring = "\x55\x8B\xEC\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\xAF\x66\xFF";
char* orgmaskw = "xxxxxxxxxxxxxxxx";

using namespace std;

std::vector<std::string> IndexStack;




namespace explorerLib{

	int RState; // ScriptContext(SCV)
	DWORD SCV; //aslr(0x0111B4B4)
	DWORD DataModel;
	DWORD Workspace;

	int aslr(DWORD addr){
		return addr - 0x400000 + (DWORD)GetModuleHandle("RobloxPlayerBeta.exe");
	}

	DWORD XorConstant = aslr(0x13EE060);
	#define SCVTable aslr(0x011AD290);


	DWORD fetchRState(DWORD* SCVP) {
		return SCVP[41] - (DWORD)&SCVP[41];
	}

	int GetParent(int Instance) {
		return *(int*)(Instance + 0x34);
	}

	const char* GetClass(int self)
	{
		return (const char*)(*(int(**)(void))(*(int*)self + 16))() + 4;
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

	std::string* GetName(int Instance) {
		return (std::string*)(*(int*)(Instance + 0x28));
	}


	int FindFirstChildes(int Instance, const char* Name){
		DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
		DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8){
			if (memcmp(GetName(*(int*)i), Name, strlen(Name)) == 0){
				return *(int*)i;
			}
		}
		return 0;
	}

	std::vector<DWORD> GetChildren(int Instance) {
		if (Instance != NULL)
		{
			DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
			if (StartOfChildren != NULL && StartOfChildren > 0)
			{
				DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
				std::vector<DWORD> Children;
				if (EndOfChildren != NULL && EndOfChildren > 0)
				{
					for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
					{
						Children.push_back(*(DWORD*)i);
					}
					return Children;
				}
				else
				{

				}
			}
		}
	}


	/*double convert_number(double real)
	{
		double new_num;
		void *p_old_num = &(real);
		__asm
		{
			mov edx, p_old_num
				mov eax, XorConstant
				movsd xmm1, qword ptr[edx]
				xorpd xmm1, [eax]
				movsd new_num, xmm1
		}
		return new_num;
	}*/

}

std::vector< DWORD > Memory_WatchArray;
void MemoryWatch_Start(){
	for (int x = 0; x != Memory_WatchArray.size(); x++){
		DWORD s = Memory_WatchArray[x];
		if (*(char*)s == 0x72){
			DWORD none;
			VirtualProtect((LPVOID)s, 0x05, PAGE_EXECUTE_READWRITE, &none);
			*(char*)s = 0xEB;
			VirtualProtect((LPVOID)s, 0x05, none, &none);
		}
	}
}


void MemoryWatch_End(){
	for (int x = 0; x != Memory_WatchArray.size(); x++){
		DWORD s = Memory_WatchArray[x];
		DWORD none;
		VirtualProtect((LPVOID)s, 0x05, PAGE_EXECUTE_READWRITE, &none);
		*(char*)s = 0x72;
		VirtualProtect((LPVOID)s, 0x05, none, &none);
	}
}

void Memory_Watch(){
	DWORD none;
	DWORD s_offset;
	int toreturn = 0;
	bool found = false;
	DWORD region = explorerLib::aslr(0x500000);
	for (int x = 0; x != 100000; x++){
		s_offset = region + x;
		if (*(char*)(s_offset) == 0x72 && *(char*)(s_offset + 18) == 0x72){
			found = true;
			Memory_WatchArray.push_back(s_offset);
		}
	}
}

void EmptyStack(int state){
	*(DWORD *)(state + 16) -= 16;
}


#define adr_gettable 0x510540 //   updated
#define adr_replace 0x511990 //    updated
#define adr_pushlstring 0x5111F0 // updated
#define adr_insert 0x510670 //   updated
#define adr_settable 0x50F5D0 // updated
#define adr_remove 0x511900 // updated
#define adr_gettop 0x5105B0 //updated

typedef void(__cdecl *GETFIELD)(int luaState, int value, const char *field);
static GETFIELD r_getfield = (GETFIELD)(explorerLib::aslr(0x510C40)); // 0x510110

typedef void(__cdecl *SETFIELD)(int luaState, int value, const char *field);
static SETFIELD r_setfield = (SETFIELD)explorerLib::aslr(0x5127B0);

typedef int(__cdecl *PCALL)(int luaState, int naArgs, int nresults, int errorFunction);
static PCALL r_pcall = (PCALL)(explorerLib::aslr(0x511940));

typedef void(__cdecl *PUSHSTRING)(int luaState, const char *string);
static PUSHSTRING r_pushstring = (PUSHSTRING)explorerLib::aslr(0x511EA0);

typedef const char*(__cdecl *TOSTRING)(int luaState, int idx, int* len);
static TOSTRING r_tolstring = (TOSTRING)explorerLib::aslr(0x512D70);

typedef bool(__cdecl *PUSHBOOLEAN)(int lua_State, int value);
PUSHBOOLEAN r_pushboolean = (PUSHBOOLEAN)explorerLib::aslr(0x513020);

//0x5132B0

typedef void(__cdecl *PUSHVALUE)(int luaState, int value);
static PUSHVALUE r_pushvalue = (PUSHVALUE)(explorerLib::aslr(0x512550));

typedef double(__cdecl *TONUMBER)(int lua_State, int idx);
static TONUMBER r_tonumber = (TONUMBER)explorerLib::aslr(0x513020);

typedef void(__cdecl *PUSHNUMBER)(int luaState, double value);
static PUSHNUMBER r_pushnumber = (PUSHNUMBER)(explorerLib::aslr(0x511E20));

typedef BOOL(__cdecl *TBOOLEAN)(int lua_State, int idx);
static TBOOLEAN r_toboolean = (TBOOLEAN)explorerLib::aslr(0x512CB0);

typedef int(__cdecl *ty)(int a, int b);
ty r_type = (ty)explorerLib::aslr(0x513180); // 0x5136a0 : 0x513780

typedef int(__cdecl *SETTOP)(int luaState, int value);
static SETTOP r_settop = (SETTOP)explorerLib::aslr(0x512B40);

#define rlua_isnil(L,n)  (r_type(L, (n)) == 0)

void r_getglobal(int RState, const char* word)
{
	r_getfield(RState, -10002, word);
}

void r_emptystack(int RState)
{
	r_settop(RState, 0);
}


void r_getfield_explorer(int luaState, int idx, const char* field)
{
	if (field == "game")
	{
		r_getfield(luaState, -10002, "game");
		IndexStack.push_back("game");
	}

	else if (field != "" && field != "game")
	{	
		r_getfield(luaState, idx, field);
		IndexStack.push_back(field);
	}
}

void r_getfield_property(int luaState, int idx, string field)
{
	//MemoryWatch_Start();
	if (field == "game")
	{
		r_getfield(luaState, -10002, "game");
	}

	else if (field != "game" && field != "")
	{
		r_getfield(luaState, idx, field.c_str());
	}
	//MemoryWatch_End();
}

const char* r_tostring(int luaState, int idx) {
	return r_tolstring(luaState, idx, NULL);
};


namespace Memory {


	bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
			if (*szMask == 'x' && *pData != *bMask) return 0;
		return (*szMask) == NULL;
	}


	DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask){
		for (DWORD i = 0; i<dwLen; i++)
			if (Compare((BYTE*)(dwAddress + i), bMask, szMask))  return (DWORD)(dwAddress + i);
		return 0;
	}

	int mScan(DWORD mode, char* content, char* mask)
	{
		//VMProtectBeginUltra("snsdkfbwkebsdkfbniwe");
		DWORD PageSize;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		PageSize = si.dwPageSize;
		MEMORY_BASIC_INFORMATION mi;
		for (DWORD lpAddr = 0; lpAddr<0x7FFFFFFF; lpAddr += PageSize)
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
		//VMProtectEnd();
	}
}

