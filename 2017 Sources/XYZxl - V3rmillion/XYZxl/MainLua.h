#pragma once
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>
#include <time.h>
#include <map>
#include <vector>
#include "Addresses_1.h"
#include "VMProtectSDK.h"
#include "Vector3.h"
#include "Color3.h"
#include "UDim2.h"
#include "NumberRange.h"
#include "CFrame.h"
#include "Vector2.h"
#include "XYZ_DB.h"
#include "Events.h"
#include "dllmain.h"
#include "UI.h"

#define DbgPrint ConsoleOutput

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "ldo.h"
#include "lfunc.h"
#include "lmem.h"
#include "lobject.h"
#include "lopcodes.h"
#include "lstring.h"
#include "lundump.h"
#include "lvm.h"
}

static Engine::Events::Event *Evt = new Engine::Events::Event("MyEvent");
static DWORD luaState = 0;
static DWORD ScriptStateHolder = 0;
static DWORD CluaState = 0;
static DWORD LocalState = 0;
static DWORD TeleportState = 0;
static DWORD DeleteState = 0;

class LuaInst {
public:
	int LuaInstanceRef;
};

class Inst {
public:
	int InstanceRef;
};

static int rbxlua_type(DWORD luaState, int idx, BOOL IsNil);

static DWORD rbxlua_gettop(DWORD luaState)
{
	return 0;
}

static bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;
	return (*szMask) == NULL;
}

static DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i<dwLen; i++)
		if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);
	return 0;
}

static DWORD SigScan(const char *AoB, char *Mask) {
	return FindPattern((DWORD)GetModuleHandleA("RobloxPlayerBeta.exe"), 0xF00000, (PBYTE)AoB, Mask);
}

#define aslr(x) (x - 0x400000 + (int)GetModuleHandle(0))

#define Retcheck_ aslr(0x59AD80)//SigScan("\x55\x8B\xEC\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83\xEC\x0C\x57\x6A\x00", "xxxxx????xxx????xxxx????xxxxxx")
#define RetcheckFlag1 0x154BAA0
#define RetcheckFlag2 0x154DB0C
#define CallCheckFlag 0x702910

static void WriteMemory(DWORD Address, DWORD Instruction)
{
	DWORD OldProtection;
	VirtualProtect((LPVOID)Address, 1, PAGE_EXECUTE_READWRITE, &OldProtection);
	*(char*)Address = Instruction;
	VirtualProtect((LPVOID)Address, 1, OldProtection, &OldProtection);
}

static DWORD RetcheckF1, RetcheckF2;
static DWORD ReturnCheck = Retcheck_;

static DWORD Flag = aslr(CallCheckFlag);

static void CallCheck()
{
	DWORD FlagValue = Flag;
	*((BYTE*)FlagValue + *(LONG_PTR*)FlagValue) = NULL;
}

static void Retcheck_Disable()
{
	RetcheckF1 = *(DWORD*)aslr(RetcheckFlag1);
	RetcheckF2 = *(DWORD*)aslr(RetcheckFlag2);
	WriteMemory(ReturnCheck, 0xC3);
}

static void Retcheck_Enable()
{
	*(DWORD*)aslr(RetcheckFlag1) = RetcheckF1;
	*(DWORD*)aslr(RetcheckFlag2) = RetcheckF2;
	WriteMemory(ReturnCheck, 0x55);
}

struct ThreadParams {
	lua_State* lThread;
	int fRef;
	int Time;
};

#pragma warning(disable:4800)

/*--------------------*/


//static int rbxlua_newuserdata(DWORD luaState, size_t size);

static void rbxlua_pushlstring(DWORD luaState, const char* a2, int a3);

static int rbxWrapToLua(DWORD luaState, int InstRef);

///////////////////////

//extern void SetupVariables();
static std::string HttpGet(std::string url);
static void LuaExecuteString(std::string Source);

static DWORD unprotect(DWORD addr) {
	BYTE * tAddr = (BYTE *)addr;

	do {
		tAddr += 0x10;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	PVOID nFunc = VirtualAlloc(NULL, 1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	memcpy(nFunc, (BYTE *)addr, 1000);

	DWORD pos = (DWORD)nFunc;
	BOOL valid = false;

	do {
		if (*(BYTE *)pos == 0x72 && * (BYTE *)(pos + 0x2) == 0xA1 && * (BYTE *)(pos + 0x7) == 0x8B) {
			memcpy((void *)pos, "\xEB", 1);

			DWORD cNFunc = (DWORD)nFunc;
			do {
				if (*(BYTE *)cNFunc == 0xE8) {
					DWORD tFunc = addr + (cNFunc - (DWORD)nFunc);
					DWORD oFunc = (tFunc + *(DWORD *)(tFunc + 1)) + 5;

					if (oFunc % 16 == 0) {
						DWORD rebFAddr = oFunc - cNFunc - 5;
						memcpy((PVOID)(cNFunc + 1), &rebFAddr, 4);
					}
				}
				cNFunc += 1;
			} while (cNFunc - (DWORD)nFunc < 1000);

			valid = true;
		}
		pos += 1;
	} while (pos < (DWORD)nFunc + 1000);

	if (!valid) {
		VirtualFree(nFunc, 1000, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}


//////////////////////

//DWORD luaState = 0;
//#define rbxlua_USE_APICHECK // 1337hax - still testing
#define rbxlua_REGISTRYINDEX       (-10000)
#define rbxlua_ENVIRONINDEX        (-10001)
#define rbxlua_GLOBALSINDEX        (-10002)
#define rbxlua_upvalueindex(i)     (rbxlua_GLOBALSINDEX-(i))
//#define l_pushcfunction(luaState,f) l_pushcclosure(luaState,f,0)

#define rbxlua_TNONE               (-1)
#define rbxlua_TNIL					 0
#define rbxlua_TNUMBER				 2
#define rbxlua_TBOOLEAN				 3
#define rbxlua_TSTRING				 4
#define rbxlua_TLIGHTUSERDATA		 1
#define rbxlua_TTABLE				 7
#define rbxlua_TUSERDATA			 8
#define rbxlua_TFUNCTION			 6
#define rbxlua_TPROTO				 9
#define rbxlua_TTHREAD				 5
#define rbxlua_TUPVALUE				 10



#define rbxlua_pop(L,n)            rbxlua_settop(L, -(n)-1)
//#define rbxlua_emptystack(L)       rbxlua_settop(L, 0)
#define rbxlua_isfunction(L,n)     (rbxlua_type(L, (n), FALSE) == rbxlua_TFUNCTION)
#define rbxlua_istable(L,n)        (rbxlua_type(L, (n), FALSE) == rbxlua_TTABLE)
#define rbxlua_islightuserdata(L,n)        (rbxlua_type(L, (n), FALSE) == rbxlua_TLIGHTUSERDATA)
#define rbxlua_isnil(L,n)			(rbxlua_type(L, (n), FALSE) != rbxlua_TUSERDATA)//== rbxlua_TNIL)
#define rbxlua_isboolean(L,n)      (rbxlua_type(L, (n), FALSE) == rbxlua_TBOOLEAN)
#define rbxlua_isstring(L,n)      (rbxlua_type(L, (n), FALSE) == rbxlua_TSTRING)
#define rbxlua_isnumber(L,n)      (rbxlua_type(L, (n), FALSE) == rbxlua_TNUMBER)
#define rbxlua_isthread(L,n)       (rbxlua_type(L, (n), FALSE) == rbxlua_TTHREAD)
#define rbxlua_isnone(L,n)         (rbxlua_type(L, (n), TRUE) == rbxlua_TNONE)
#define rbxlua_isnoneornil(L, n)   (rbxlua_type(L, (n), TRUE) <= 0)

#define rbxlua_setglobal(L,s)      rbxlua_setfield(L, rbxlua_GLOBALSINDEX, (s))
#define rbxlua_getglobal(L,s)      rbxlua_getfield(L, rbxlua_GLOBALSINDEX, (s))
#define rbxlua_tostring(L,i)       rbxlua_tolstring(L, (i), NULL)
#define rbxlua_getservice(L,s)		_rbxlua_getservice(L, (s))

////
#define rbxlua_pushcfunction(L,f)	rbxlua_pushcclosure(L, (f), 0)
#define rbxlua_register(L,n,f)		(rbxlua_pushcfunction(L, (f)), rbxlua_setglobal(L, (n)))

#define rbxlua_pushliteral(L, s)	\
	rbxlua_pushlstring(L, "" s, (sizeof(s)/sizeof(char))-1)

/*
** compatibility macros and functions
*/
#define rbxlua_getregistry(L)	rbxlua_pushvalue(L, rbxlua_REGISTRYINDEX)

static void setJb(DWORD addr) {
	DWORD oldProtection;
	VirtualProtect((LPVOID)addr, 0x05, PAGE_EXECUTE_READWRITE, &oldProtection);
	*(char*)addr = 0xEB;
	VirtualProtect((LPVOID)addr, 0x05, oldProtection, &oldProtection);
}
static void restoreJb(DWORD addr) {
	DWORD oldProtection;
	VirtualProtect((void*)addr, 5, PAGE_EXECUTE_READWRITE, &oldProtection);
	*(char*)addr = 0x72;
	VirtualProtect((void*)addr, 5, oldProtection, &oldProtection);
}


/*
** {======================================================================
** Debug API
** =======================================================================
*/

/*
** Event codes
*/
#define rbxlua_HOOKCALL	0
#define rbxlua_HOOKRET	1
#define rbxlua_HOOKLINE	2
#define rbxlua_HOOKCOUNT	3
#define rbxlua_HOOKTAILRET 4


/*
** Event masks
*/
#define rbxlua_MASKCALL		(1 << rbxlua_HOOKCALL)
#define rbxlua_MASKRET		(1 << rbxlua_HOOKRET)
#define rbxlua_MASKLINE		(1 << rbxlua_HOOKLINE)
#define rbxlua_MASKCOUNT	(1 << rbxlua_HOOKCOUNT)

////

typedef void* (__cdecl *rbxlua_GetField)(DWORD luaState, int idx, const char *k);
typedef int(__cdecl *rbxlua_PCall)(DWORD luaState, int nargs, int nresults, int errfunc);
typedef void(__cdecl *rbxlua_PushString)(DWORD luaState, const char *s);
typedef void(__cdecl *rbxlua_PushNumber)(DWORD luaState, double Number);
typedef const char *(__cdecl *rbxlua_ToLString)(DWORD luaState, int idx, size_t *len);
typedef void* (__cdecl *rbxlua_PushValue)(DWORD luaState, int idx);
typedef int(__cdecl *rbxlua_GetTop)(DWORD luaState);
typedef void* (__cdecl *rbxlua_SetTop)(DWORD luaState, int n);
typedef int(__cdecl *rbxlua_ToUserData)(DWORD luaState, int idx);
typedef void* (__cdecl *rbxlua_SetField)(DWORD luaState, signed int idx, const char *k);
//typedef void(*rbxlua_SetField)(int luaState, int index, const char *k);
typedef void* (__cdecl *rbxlua_PushCClosure)(DWORD luaState, int fn(), int n);
typedef BOOL(__cdecl *rbxlua_ToBoolean)(DWORD luaState, int idx);
typedef int(__cdecl *rbxlua_Next)(DWORD luaState, int idx);
typedef int(__cdecl *rbxlua_Type)(DWORD luaState, int idx);

//typedef int(__cdecl *rbxlua_pushcfunction)(DWORD luaState, int idx); // not correct args!!1
typedef int(__cdecl *rbxlua_Insert)(DWORD luaState, int idx);
typedef int(__cdecl *rbxlua_Remove)(DWORD luaState, int idx);

typedef int(__cdecl *rbxlua_IsString)(DWORD luaState, int idx); // new

typedef int(__cdecl *rbxlua_Newthread)(DWORD luaState);
typedef int(*rbxlua_error)(DWORD luaState);
typedef void(*rbxlua_createtable)(DWORD luaState, int narr, int nrec);
typedef void(*rbxlua_pushlightuserdata)(DWORD luaState, void *p);
typedef signed int(__cdecl *rbxlua_Setmetatable)(DWORD luaState, signed int idx);
typedef int(__cdecl *rbxlua_Newuserdata)(DWORD luaState, size_t size);
typedef void(*rbxlua_pushinteger)(DWORD luaState, ptrdiff_t n);
typedef double(__cdecl *rbxlua_ToNumber)(DWORD luaState, int idx);
typedef int(__cdecl *rbxlua_pushboolf)(DWORD luaState, int idx);

typedef int(__thiscall *rbxlua_chatthing)(void* players, std::string* message);

typedef int(__cdecl *rbxlua_Print)(int state, int* msginfo);

typedef int(__thiscall *dolotsmotscots)(void* , const char* message);




typedef LONG(NTAPI *NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI *NtResumeProcess)(IN HANDLE ProcessHandle);
static void SuspendProc()
{
	HANDLE processHandle = GetCurrentProcess();

	NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
		GetModuleHandle(TEXT("ntdll")), "NtSuspendProcess");

	pfnNtSuspendProcess(processHandle);
}
static void ResumeProc()
{
	HANDLE processHandle = GetCurrentProcess();

	NtResumeProcess pfnNtResumeProcess = (NtResumeProcess)GetProcAddress(
		GetModuleHandle(TEXT("ntdll")), "NtResumeProcess");

	pfnNtResumeProcess(processHandle);
}

typedef int(__thiscall *rbxlua_chatthing)(void* players, std::string* message);

typedef int(__cdecl *rbxlua_Print)(int state, int* msginfo);

typedef int(__thiscall *dolotsmotscots)(void*, const char* message);

static rbxlua_pushboolf _rbxlua_pushbool = (rbxlua_pushboolf)rbxaddy(0x709EF0); // 55 8B EC 8B 55 08 33 C0 39 45 0C

																				//55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 51 56 8B 75 08 57 8B 4E 08 8B 44 31 60 3B 44 31 54 72
static rbxlua_Newthread rbxlua_newthread = (rbxlua_Newthread)rbxaddy(0x709B20);  // "Executing lua from signal, execution de" should be 0x5300000 sub by the middle


																																																													//55 8B EC 8B 55 08 ?? ?? ?? ?? ?? ?? ?? ?? F2 0F 10 45 0C
static rbxlua_ToNumber rbxlua_tonumber = (rbxlua_ToNumber)rbxaddy(); // notupdated /*0x54A260);/*0x549010);/*0x546DE0);/*0x546DE0);/*0x546B50);/*0x5459C0);/*0x5465E0);/*0x5431A0);/*0x5411B0);/*0x540470);/*0x53D760);/*0x536900);/*0x534200);/*0x5348B0);/*0x534110;/*0x50EE90;/*0x517690;/*0x5180B0;
																																 /* Find "elapsedTime" & "ElapsedTime"
																																 follow the address inbetween them that looks like 'offset sub_NUMBERS'
																																 there should be 2 subs.. it should be the second one :)
																																 */

static rbxlua_PushString rbxlua_pushstring = (rbxlua_PushString)rbxaddy(0x70A390);//0x54A2E0);//0x549090);//0x546E60);//0x546E60);//0x546BC0);//0x545A30);//0x546650);//0x543210);//0x541220);//0x5404E0);//0x53D7D0);//0x53E3C0);//0x53C2E0);//0x536C90);//0x536DC0);//0x536E10);//0x534270);//0x534AA0);//0x534920;//0x534180;//0x516F40;//0x517700;//0x518120;//0x518D60;//0x51C280;//0x51BA60;//0x53FE10; // "__tostring"
																																   //updated
static rbxlua_SetField rbxlua_setfield = (rbxlua_SetField)rbxaddy(0x70AC90);/*0x54AA30);/*0x5497E0);/*0x5475B0);/*0x5475B0);/*0x547290);/*0x546100);/*0x546D20);/*0x5438E0);/*0x5418F0);/*0x540BB0);/*0x53DEA0);/*0x53EA90);/*0x53C9B0);/*0x537360);/*0x537490);/*0x5374E0);/*0x534940);/*0x535170);/*0x533EC0;/*0x534FF0;/*0x534850;/*0x517610;/*0x517DD0;/*0x5187F0;/*0x517D90;//0x519430;/*0x51C950;//

																																																																																										//go below to repeated subs and it should look like:
																																																																																										/* // sub_51C950 is the setfield
																																																																																										sub_51C950(v5, -10002, "delay");
																																																																																										sub_51BEF0(v5, sub_508FF0, 0);
																																																																																										sub_51C950(v5, -10002, "Delay");
																																																																																										sub_51BEF0(v5, sub_51A6B0, 0);
																																																																																										sub_51C950(v5, -10002, "ypcall");
																																																																																										sub_51BEF0(v5, sub_51A6B0, 0);
																																																																																										*/
																																																																																										/*0x51C950;/*0x51C130;/*0x5404E0;		/*0x53BDA0;
																																																																																										The 'call sub_NUMBERS' right under "wait" AKA aWait; // OLD METHOD


																																																																																										//updated												*/
static rbxlua_PushNumber rbxlua_pushnumber = (rbxlua_PushNumber)rbxaddy(0x70A310);/*0x54A260);/*0x549010);/*0x546DE0);/*0x546DE0);/*0x546B50);/*0x5459C0);/*0x5465E0);/*0x5431A0);/*0x5411B0);/*0x540470);/*0x53D760);/*0x53E350);/*0x53C270);/*0x536C20);/*0x536D50);/*0x536DA0);/*0x534200);/*0x534A30);/*0x5348B0;/*0x534110;/*0x516ED0;/*0x517690;/*0x5180B0;/*0x518CF0;/*0x51C210;/*0x51C210;/*0x53FDA0;/*0x53FE50
																																   At the very bottom of the string "math" AKA aMath and ontop of "pi" or aPi;
																																   //updated												 */
static rbxlua_PCall rbxlua_pcall = (rbxlua_PCall)rbxaddy(0x709E30);/*0x549D70);/*0x548B30);/*0x546900);/*0x546900);/*0x546720);/*0x545590);/*0x5461B0);/*0x542D60);/*0x540D80);/*0x540040);/*0x53D330);/*0x53DF20);/*0x53BE40);/*0x5367F0);/*0x536920);/*0x536970);/*0x533DB0);/*0x534600);/*0x534480;/*0x533CE0;/*0x516AA0;/*0x517260;/*0x517C80;/*0x5188C0;/*0x51BDE0;/*0x51B5C0;/*0x53F970;/*0x5388B0;
																															  Find "Breakpoint %d condition runtime error: " AKA aBreakpointDC_0;
																															  The first sub_NUMBERS in that function will be pcall;
																															  */
static rbxlua_ToLString rbxlua_tolstring = (rbxlua_ToLString)rbxaddy(0x70B270);/*0x54AF00);/*0x549CB0);/*0x547A80);/*0x547A80);/*0x547700);/*0x546570);/*0x5471A0);/*0x543D50);/*0x541D60);/*0x541020);/*0x53E310);/*0x53EF00);/*0x53CE20);/*0x5377D0);/*0x537900);/*0x537950);/*0x534DB0);/*0x5355E0);/*0x535460;/*0x534CC0;/*0x517A80;/*0x518240;/*0x518C60;/*0x5198A0;/*0x51CDC0;/*0x51C5A0;/*0x532450;		/*0x53C150;
																																											 Find "!handler.empty()" AKA aHandler_empty;
																																											 And follow the 'jnz short loc_NUMBERS' right under the emptyhandler;
																																											 And find the first sub_NUMBERS and your done!1;
																																											 */
																																											 //updated// newproxy only supports the arguments ni
static rbxlua_SetTop _rbxlua_settop = (rbxlua_SetTop)rbxaddy(0x70B040);/*0x549E30);/*0x548BF0);/*0x5469C0);/*0x5469C0);/*0x547520);/*0x546390);/*0x546FC0);/*0x543B70);/*0x541B80);/*0x540E40);/*0x53E130);/*0x53ED20);/*0x53CC40);/*0x5375F0);/*0x537720);/*0x537770);/*0x533810);/*0x535280;/*0x533EE0;/*0x533740;/*0x516500;/*0x516CC0;/*0x5176E0;/*0x518320;/*0x51/B840;/*0x51B020;/*0x540770;/*0x53F3D0;				/*0x53BFB0;
																	   Find "top+1 == lua_gettop(functionThread)" AKA aTop1rbxlua_gettop;
																	   And the first sub_NUMBERS on the function is the address;
																	   */
																	   //updated
static rbxlua_ToBoolean _rbxlua_toboolean = (rbxlua_ToBoolean)rbxaddy(); //NOTUPDATED /*0x54AE80);/*0x549C30);/*0x547A00);/*0x547A00);/*0x547680);/*0x5464F0);/*0x547120);/*0x543CD0);/*0x541CE0);/*0x540FA0);/*0x53E290);/*0x53EE80);/*0x53CDA0);/*0x537750);/*0x537880);/*0x5378D0);/*0x534D30);/*0x5353E0);/*0x534C40;/*0x517A00;/*0x5181C0;/*0x518BE0;/*0x519820;/*0x51CD40;/*0x51C520;/*0x5408D0;		/*0x53C0D0;
																				/*Find "isdst" AKA aIsdst;
																				And follow the 'jnz short loc_NUMBERS' in the function;
																				And it will be the sub_NUMBERS in the function;
																				[normally right under the aIsdst function];
																				//updated													*/
static rbxlua_Type _rbxlua_type = (rbxlua_Type)rbxaddy(0x70B690);/*0x547E40);/*0x547A70);/*0x5468E0);/*0x547510);/*0x5440C0);/*0x5420D0);/*0x541390);/*0x53E680);/*0x53F270);/*0x53D190);/*0x537B40);/*0x537C70);/*0x537CC0);/*0x535120);/*0x535950);/*0x5357D0;/*0x535030;/*0x517DF0;/*0x5185B0;/*0x518FD0;/*0x519C10;/*0x51D130;/*0x51C910;/*0x540CC0;						/*0x53C440;
																																										Find "rbxlua_isthread(functionThread, -1)" AKA arbxlua_isthreadFu;
																																										It will be the first sub_NUMBERS in the function;
																																										//updated													*/
static rbxlua_GetField rbxlua_getfield = (rbxlua_GetField)rbxaddy(0x709130);/*0x549550);/*0x548310);/*0x5460E0);/*0x5460E0);/*0x545FE0);/*0x544E50);/*0x545A70);/*0x542620);/*0x540640);/*0x53F900);/*0x53CBF0);/*0x53D7E0);/*0x53B700);/*0x5360B0);/*0x5361E0);/*0x536230);/*0x533670);/*0x533EC0);/*0x533D40;/*0x5335A0;/*0x516360;/*0x516B20;/*0x517540;/*0x518180;/*0x518180;/*0x51B6A0;/*0x51D130;/*0x51B6A0;/*0x51B6A0;/*0x51AE80;/*0x53F230;		/*0x53AE70;
																																																																																						 Find "__unm" AKA a__unm [it should say offset to the left of a_unm];
																																																																																						 Follow the 'offset sub_NUMBERS' right underneath it;
																																																																																						 Follow the last loc_NUMBERS in the function;
																																																																																						 It should be the first sub_NUMBERS;
																																																																																						 //updated													*/
static rbxlua_PushValue rbxlua_pushvalue = (rbxlua_PushValue)rbxaddy(0x70A450);/*0x54A3A0);/*0x549150);/*0x546F20);/*0x546F20);/*0x546CB0);/*0x545B20);/*0x546740);/*0x543300);/*0x541310);/*0x5405D0);/*0x53D8C0);/*0x53E4B0);/*0x53C3D0);/*0x536D80);/*0x536EB0);/*0x536F00);/*0x534360);/*0x534B90);/*0x534A10;/*0x534270;/*0x517030;/*0x5177F0;/*0x518210;/*0x518E50;/*0x51C370;/*0x51CB70;/*0x572700;/*0x51BB50;/*0x53FF00;		/*0x53B8D0;
																																															Find "%s.%s" AKA aS_S;
																																															In the function look for 'jz short loc_NUMBERS' and follow it;
																																															And it should be the second to last 'call sub_NUMBERS' in the function;
																																															//updated													*/
static rbxlua_GetTop _rbxlua_gettop = (rbxlua_GetTop)rbxaddy();/*0x549720);/*0x5484E0);/*0x5462B0);/*0x5462B0);/*0x546180);/*0x544FF0);/*0x545C10);/*0x5427C0);/*0x5407E0);/*0x53FAA0);/*0x53CD90);/*0x53D980);/*0x53B8A0);/*0x536250);/*0x536380);/*0x5363D0);/*0x533810);/*0x534060);/*0x533EE0;/*0x533740;/*0x516500;/*0x516CC0;/*0x5176E0;/*0x518320;/*0x51B840;/*0x51B020;/*0x53F3D0;				/*0x53AFB0;
																	   Find "tostring" AKA aTostring;
																	   It will be right on top of the "offset aTostring ; "tostring"";
																	   */
static rbxlua_ToUserData rbxlua_touserdata = (rbxlua_ToUserData)rbxaddy();/*0x54B290);/*0x54A040);/*0x547E10);/*0x547E10);/*0x547A40);/*0x5468B0);/*0x5474E0);/*0x544090);/*0x5420A0);/*0x541360);/*0x53E650);/*0x53F240);/*0x53D160);/*0x537B10);/*0x537C40);/*0x5357A0);/*0x517DC0;/*0x518580;/*0x518FA0;/*0x519BE0;/*0x51D100;/*0x51C8E0;/*0x540C90;		/*0x53C410;
																				   Find "Bad argument materials[%d][%d][%d] to '" AKA aBadArgumentM_0;
																				   Should be the 3rd sub_NUMBERS in the function;
																				   [and the 1st and 2nd should possibly be the same]
																				   */
static rbxlua_Next rbxlua_next;

static rbxlua_chatthing _rbxluachat = (rbxlua_chatthing)rbxaddy();

static rbxlua_PushCClosure rbxlua_pushcclosure;
static rbxlua_Insert rbxlua_insert;
static rbxlua_Remove rbxlua_remove;

static rbxlua_Setmetatable rbxlua_setmetatable = (rbxlua_Setmetatable)rbxaddy();

static rbxlua_Newuserdata rbxlua_newuserdata = (rbxlua_Newuserdata)rbxaddy();
/*
Search "Width" and it should be the top one, follow it. And sub_533B10 is it!! That is newuserdata!!
.text:00751117 loc_751117:                             ; CODE XREF: .text:00751110j
.text:00751117                 test    eax, eax
.text:00751119                 jnz     short loc_751157
.text:0075111B                 mov     eax, [ebp+8]
.text:0075111E                 mov     esi, [ebp+10h]
.text:00751121                 movss   xmm0, dword ptr [eax+8]
.text:00751126                 movss   dword ptr [ebp-8], xmm0
.text:0075112B                 movss   xmm0, dword ptr [eax+0Ch]
.text:00751130                 push    8
.text:00751132                 push    esi
.text:00751133                 movss   dword ptr [ebp-4], xmm0
.text:00751138                 call    sub_533B10 // IT'S HERE!! IT'S HERE!!!!!!!!!!!!
.text:0075113D                 add     esp, 8
.text:00751140                 test    eax, eax
.text:00751142                 jz      short loc_7510C5
.text:00751144                 movss   xmm0, dword ptr [ebp-8]
.text:00751149                 movss   dword ptr [eax], xmm0
.text:0075114D                 movss   xmm0, dword ptr [ebp-4]
.text:00751152                 jmp     loc_7510C0
.text:00751157 ; ---------------------------------------------------------------------------
.text:00751157
.text:00751157 loc_751157:                             ; CODE XREF: .text:00751119j
.text:00751157                 mov     ecx, offset aWidth ; "Width"
.text:0075115C                 mov     eax, esi
.text:0075115E                 mov     edi, edi
*/
/*
static int l_cppfunction(lua_State *L) {
double arg = luaL_checknumber(L, 1);
lua_pushnumber(L, arg * 0.5);
return 1;
}
*/
static int stackSize(int State) { return rbxlua_gettop(State); }
static void checkStack(int State) { assert(rbxlua_gettop(State) <= 20); }
static bool hasIndex(int State, int index) {
	return (stackSize(State) >= (index < 0 ? -index : index) && index != 0);
}
static void clearStack(int State) {
	
}

static void _rbxlua_pushnil(DWORD luaState) { // rbxlua_pushnil
	__try {
		*(DWORD*)(*(DWORD *)(luaState + 16) + 8) = 0;
		*(DWORD*)(luaState + 16) += 16;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		MessageBoxA(0, "A unexpected error has occured! ERROR: 0x41", "", 0); Sleep(1);
	}
}

#define r_lua_setmetatable_addr rbxaddy(0x510010)
#define r_lua_getfield_addr rbxaddy(0x50e310)
#define r_luaD_precall_addr rbxaddy(0x507c80)
#define r_lua_pcall_addr rbxaddy(0x50f010)
#define r_lua_close_addr rbxaddy(0x50eea0)
#define r_lua_newthread_addr rbxaddy(0x50ed10)
#define r_lua_tolstring_addr rbxaddy(0x510440)
#define r_lua_createtable_addr rbxaddy(0x50dfb0)
#define r_lua_pushcclosure_addr rbxaddy(0x50f140)
#define r_lua_B_settop_addr rbxaddy(0x510210)
#define r_lua_newuserdata_addr rbxaddy(0x50edf0)

#define r_setobj setobj


typedef int(__cdecl *r_lua_setmetatable_Def)(DWORD luaState, int object);
static r_lua_setmetatable_Def r_lua_setmetatable = (r_lua_setmetatable_Def)r_lua_setmetatable_addr;
typedef int(__cdecl *r_lua_getfield_Def)(DWORD luaState, int object, const char* k);
static r_lua_getfield_Def r_lua_getfield = (r_lua_getfield_Def)r_lua_getfield_addr;
typedef int(__cdecl *r_luaD_precall_Def)(DWORD luaState, int object1, int obj2);
static r_luaD_precall_Def r_luaD_precall = (r_luaD_precall_Def)r_luaD_precall_addr;
typedef int(__cdecl *r_luaF_close_Def)(DWORD luaState);
static r_luaF_close_Def r_luaF_close = (r_luaF_close_Def)r_lua_close_addr;
typedef int(__cdecl *r_luaC_createtable_Def)(DWORD luaState, int table_object, int table_value);
static r_luaC_createtable_Def r_luaC_createtable = (r_luaC_createtable_Def)r_lua_createtable_addr;
typedef int(__cdecl *r_luaF_pushcclosure_Def)(DWORD luaState, int fn, int nothing_func);
static r_luaF_pushcclosure_Def r_lua_pushcclosure = (r_luaF_pushcclosure_Def)r_lua_pushcclosure_addr;
typedef int(__cdecl *r_luaF_newthread)(DWORD luaState);
static r_luaF_newthread r_lua_newthread = (r_luaF_newthread)r_lua_newthread_addr;
typedef int(__cdecl *r_luaB_settop_Def)(DWORD luaState, int settop_idx);
static r_luaB_settop_Def r_luaB_settop = (r_luaB_settop_Def)r_lua_B_settop_addr;
typedef int(__cdecl *r_lua_NewUserData_Def)(DWORD luaState, int idx);
static r_lua_NewUserData_Def r_lua_newuserdata = (r_lua_NewUserData_Def)r_lua_newthread_addr;

static void ConsoleShittyBypass()
{

}

static int wrap_obj(DWORD luaState, int object, int exe)
{
	if (luaState && object)
	{
		int *func = (int*)r_lua_newuserdata(luaState, 8);
		*func = object;
		*(func + 1) = exe;
		r_lua_getfield(luaState, LUA_REGISTRYINDEX, "Object");
		r_lua_setmetatable(luaState, -2);
	}
	return 0;
}


static void rbxlua_pushnil(DWORD luaState) {
	try {
		_rbxlua_pushnil(luaState);
	}
	catch (std::exception& e) {
		MessageBoxA(0, e.what(), "", 0);
	}
	catch (...) {
		MessageBoxA(0, "A unexpected error has occured! ERROR: 0x74", "", 0);
	}
}

static int _rbxlua_getservice(DWORD luaState, const char* ServiceName) { // custom function // -5 or -6
	if (ServiceName != "") {
		rbxlua_getglobal(luaState, ServiceName);
		if (rbxlua_isnil(luaState, -1)) { // no '!' at the start since we are checking to make sure it's nil
			rbxlua_getglobal(luaState, "game");
			if (!rbxlua_isnil(luaState, -1)) {
				rbxlua_getfield(luaState, -1, "GetService");
				if (rbxlua_isfunction(luaState, -1)) {
					rbxlua_pushvalue(luaState, -2);
					rbxlua_pushstring(luaState, ServiceName);
					rbxlua_pcall(luaState, 2, 1, 0);
					if (!rbxlua_isnil(luaState, -1)) {
						rbxlua_setglobal(luaState, ServiceName);
						rbxlua_getglobal(luaState, ServiceName);
						return 1;
					}
					else {
						return 0;
						clearStack(luaState);
					}
				}
				else {
					clearStack(luaState);
					return 0;
				}
			}
		}
		else {
			return 1;
		}
	}
	ConsoleOutput("Executed!\n");
	return 0;
}

static int rbxlua_getobject(DWORD luaState, int idx) {
	return (int)rbxlua_touserdata(luaState, idx);
}

// pushbool: use pushnumber 1 to push a true boolean, use pushnil to push false

/*void rbxlua_pushbool(DWORD luaState, int a2) {
	__try {
		int v2 = *(DWORD*)(luaState + 16);
		*(DWORD*)(v2 + 8) = 3;
		*(DWORD*)v2 = a2 != 0;
		*(DWORD*)(luaState + 16) += 16;
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		Sleep(1);
	}
}*/

static void rbxlua_pushbool(DWORD luaState, BOOL val) { // custom function
    //Retcheck_Disable();
	try {
		_rbxlua_pushbool(luaState, val);
	}
	catch (std::exception) {}
	catch (...) {}
	/*if (val == TRUE) {
	rbxlua_pushnumber(luaState, 1);
	}
	else if (val == FALSE) {
	rbxlua_pushnil(luaState);
	}*/
	if (LocalState != luaState) {
		Sleep(5);
	}
	//Retcheck_Enable();
}

int rbxlua_type(DWORD luaState, int idx, BOOL IsNil) { // custom function
	int ret;
	//luaL_checktype(L, 2, LUA_TTABLE);
	try {
		ret = _rbxlua_type(luaState, idx);
	}
	catch (std::exception& e) {
		//clearStack();
		if (IsNil == TRUE) { // luaL_checktype(luaState, idx, _rbxlua_TNIL);
			ret = luaState;
		}
		MessageBoxA(0, e.what(), "", 0);
	}
	catch (...) {
		//clearStack();
		if (IsNil == TRUE) {
			ret = luaState;
		}
		MessageBoxA(0, "A unexpected error has occured! ERROR#: 5", "", 0);
	}
	return ret;
}




static std::string rbxlua_getvalue(DWORD luaState, int idx, const char* k) { // custom function
	std::string val;
	rbxlua_getfield(luaState, idx, k);
	val = rbxlua_tostring(luaState, idx);
	return val;
}





static std::string HttpGet(std::string URL) {
	rbxlua_getglobal(luaState, "game");
	if (!rbxlua_isnil(luaState, -1)) {
		rbxlua_getfield(luaState, -1, "HttpGet");
		rbxlua_pushvalue(luaState, -2);
		rbxlua_pushstring(luaState, URL.c_str());
		rbxlua_pushbool(luaState, 1);
		rbxlua_pcall(luaState, 3, 1, 0); Sleep(50);

		if (rbxlua_isstring(luaState, -1)) {
			const std::string returnval = rbxlua_tostring(luaState, -1);
			return returnval;
		}
	}
	return std::string("");
}

static std::string HttpGetAsync(std::string URL) {
	rbxlua_getglobal(luaState, "game");
	if (!rbxlua_isnil(luaState, -1)) {
		rbxlua_getfield(luaState, -1, "HttpGetAsync");
		rbxlua_pushvalue(luaState, -2);
		rbxlua_pushstring(luaState, URL.c_str());
		//rbxlua_pushbool(luaState, 1);
		rbxlua_pcall(luaState, 2, 1, 0); Sleep(50);

		if (rbxlua_isstring(luaState, -1)) {
			const std::string returnval = rbxlua_tostring(luaState, -1);
			return returnval;
		}
	}
	return std::string("");
}

static void LoadGame(std::string URL) {
	rbxlua_getglobal(luaState, "game");
	if (!rbxlua_isnil(luaState, -1)) {
		std::string FixedURL = std::string("http://assetgame.roblox.com/Asset/?id=") + std::string(URL);
		rbxlua_getfield(luaState, -1, "Load");
		rbxlua_pushvalue(luaState, -2);
		rbxlua_pushstring(luaState, FixedURL.c_str());
		rbxlua_pcall(luaState, 2, 0, 0); Sleep(50);
	}
}

static int GetObjects(lua_State* L)
{
    std::string URL = lua_tostring(L, -1);
	rbxlua_getservice(luaState, "ContentProvider");
	if (!rbxlua_isnil(luaState, -1)) {
		std::string FixedURL = std::string("http://www.roblox.com/asset/?id=") + std::string(URL);
		rbxlua_getfield(luaState, -1, "Preload");
		rbxlua_pushvalue(luaState, -2);
		rbxlua_pushstring(luaState, FixedURL.c_str());
		rbxlua_pcall(luaState, 2, 0, 0); Sleep(50);
	}
	return 0;
}

static void LoadInAsset(std::string URL) {
	rbxlua_getservice(luaState, "ContentProvider");
	if (!rbxlua_isnil(luaState, -1)) {
		std::string FixedURL = std::string("http://www.roblox.com/asset/?id=") + std::string(URL);
		rbxlua_getfield(luaState, -1, "Preload");
		rbxlua_pushvalue(luaState, -2);
		rbxlua_pushstring(luaState, FixedURL.c_str());
		rbxlua_pcall(luaState, 2, 0, 0); Sleep(50);
	}
	/*rbxlua_getservice(luaState, "InsertService");
	if (!rbxlua_isnil(luaState, -1)) {
		std::string FixedURL = std::string("http://assetgame.roblox.com/Asset/?id=");
		rbxlua_getfield(luaState, -1, "SetAssetUrl");
		rbxlua_pushvalue(luaState, -2);
		rbxlua_pushstring(luaState, FixedURL.c_str());
		rbxlua_pcall(luaState, 2, 0, 0); Sleep(50);
	}

	rbxlua_getservice(luaState, "InsertService");
	if (!rbxlua_isnil(luaState, -1)) {
		//std::string FixedURL = std::string("http://assetgame.roblox.com/Asset/?id=") + std::string(URL);
		rbxlua_getfield(luaState, -1, "SetTrustLevel");
		rbxlua_pushvalue(luaState, -2);
		rbxlua_pushnumber(luaState, 255);
		//rbxlua_pushstring(luaState, URL.c_str());//FixedURL.c_str());
		rbxlua_pcall(luaState, 2, 0, 0); Sleep(50);
	}

	rbxlua_getservice(luaState, "InsertService");
	if (!rbxlua_isnil(luaState, -1)) {
		//std::string FixedURL = std::string("http://assetgame.roblox.com/Asset/?id=") + std::string(URL);
		rbxlua_getfield(luaState, -1, "ApproveAssetId");
		rbxlua_pushvalue(luaState, -2);
		rbxlua_pushnumber(luaState, std::stoi(URL));
		//rbxlua_pushstring(luaState, URL.c_str());//FixedURL.c_str());
		rbxlua_pcall(luaState, 2, 0, 0); Sleep(50);
	}

	rbxlua_getservice(luaState, "InsertService");
	if (!rbxlua_isnil(luaState, -1)) {
		//std::string FixedURL = std::string("http://assetgame.roblox.com/Asset/?id=") + std::string(URL);
		rbxlua_getfield(luaState, -1, "LoadAsset");
		rbxlua_pushvalue(luaState, -2);
		rbxlua_pushnumber(luaState, std::stoi(URL));
		//rbxlua_pushstring(luaState, URL.c_str());//FixedURL.c_str());
		rbxlua_pcall(luaState, 2, 0, 0); Sleep(50);
	}*/
}

static std::map<int, Inst*> InstanceMap = {};
static std::map<int, LuaInst*> LuaInstanceMap = {};
static int WrapToLua(lua_State* L, int InstRef);
template <typename T> static inline T *CheckInstanceUnsafe(lua_State *L, int idx, bool selfCheck = true) {
	static_assert(std::is_base_of<Inst, T>::value, "T must inherit from Instance");
	if (lua_isnil(L, idx)) {
		const char *msg = lua_pushfstring(L, "Instance expected, got nil");
		lua_pop(L, 1);
		luaL_argerror(L, idx, msg);
		return NULL;
	}
	void *udata = lua_touserdata(L, idx);
	if (udata == NULL) {
		if (selfCheck) {
			const char *msg = lua_pushfstring(L, "Instance expected, got %s", lua_typename(L, lua_type(L, idx)));
			lua_pop(L, 1);
			luaL_argerror(L, idx, msg);
		}
		return NULL;
	}
	Inst *inst = *((Inst**)udata);
	T *requested_instance = dynamic_cast<T*>(inst);
	if (requested_instance == NULL) {
		if (selfCheck) {
			const char *msg = lua_pushfstring(L, "Instance expected, got userdata");
			lua_pop(L, 1);
			luaL_argerror(L, idx, msg);
		}
	}
	return requested_instance;
}

template <typename T> static inline T *CheckInstance(lua_State *L, int idx, bool selfCheck = true) {
	__try {
		return CheckInstanceUnsafe<T>(L, idx, selfCheck);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		Sleep(0);
	}
	luaL_error(L, "A unexpected error has occured! ERROR CODE: 0x14");
	return nullptr;
}

static void LuaThreadHandler(ThreadParams* tParams) {
	if (tParams->Time > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(tParams->Time * 1000));
	lua_settop(tParams->lThread, 0);
	lua_rawgeti(tParams->lThread, LUA_REGISTRYINDEX, tParams->fRef);
	int Results = lua_pcall(tParams->lThread, 0, 0, 0);
	if (Results != 0) {
		//MessageBox(NULL, lua_tostring(tParams->lThread, -1), TEXT("Lua Error"), NULL);
		ConsoleOutput(std::string("Lua Error: ") + lua_tostring(tParams->lThread, -1), RGB(255, 0, 0));
		lua_settop(tParams->lThread, 0);
	}
}

template <typename T>
static T* rbxlua_grabobject(DWORD luaState, int index) {
	T* obj = NULL;
	if (rbxlua_type<T>(luaState, index)) {
		obj = *(T**)rbxlua_touserdata(luaState, index);
	}
	return obj;
}

static int LuaCurrentEpoch(lua_State *L) {
	std::chrono::milliseconds a = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	lua_pushnumber(L, a.count() / 1000);
	return 1;
}

static int luaToString(lua_State* L) {
	Inst* ind = CheckInstance<Inst>(L, 1);
	if (ind) {
		rbxlua_pushstring((DWORD)L, GetName(ind->InstanceRef)->c_str());
		return 1;
	}
	return 0;
}

static int luaWait(lua_State* L) {
	double WaitTime = 0.005;
	if (lua_gettop(L) > 0) {
		WaitTime = luaL_checknumber(L, 1);
	}
	std::this_thread::sleep_for(std::chrono::milliseconds((int)WaitTime * 1000));
	return 1;
}

static int luaSpawn(lua_State* L) {
	if (lua_type(L, 1) != LUA_TFUNCTION) {
		return luaL_error(L, "Function expected, got %s", lua_typename(L, lua_type(L, 1)));
	}
	int fRef = lua_ref(L, LUA_REGISTRYINDEX);
	ThreadParams tParams{
		lua_newthread(L),
		fRef,
		0
	};
	std::thread LuaThread(&LuaThreadHandler, &tParams);
	LuaThread.detach();
	lua_settop(L, 0);
}

static int luaDelay(lua_State* L) {
	int Time = luaL_checkinteger(L, 1);
	if (lua_type(L, 2) != LUA_TFUNCTION) {
		return luaL_error(L, "Function expected, got %s", lua_typename(L, lua_type(L, 1)));
	}
	int fRef = lua_ref(L, LUA_REGISTRYINDEX);
	ThreadParams tParams{
		lua_newthread(L),
		fRef,
		Time
	};
	std::thread LuaThread(&LuaThreadHandler, &tParams);
	LuaThread.detach();
	lua_settop(L, 0);
}


static int LuaWarn(lua_State* L) {
	int n = lua_gettop(L);  // number of arguments
	int i;
	std::string out;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  // function to be called
		lua_pushvalue(L, i);   // value to print
							   //rbxlua_call(L, 1, 1);
		if (!lua_isnil(L, i)) {
			if (lua_isstring(L, -1)) {
				s = lua_tostring(L, -1);  // get result
				if (s == NULL) {
					s = "nil";
				}
			}
		}
		else {
			s = "nil";
		}
		if (i > 1) out += "\t";
		out += s;
		lua_pop(L, 1);  // pop result
	}
	rbxlua_getglobal(luaState, "warn");
	rbxlua_pushstring(luaState, out.c_str());
	rbxlua_pcall(luaState, 1, 0, 0);
	return 0;
}

static int LuaPrint(lua_State* L) {
	rbxlua_getfield(luaState, -10002, "print");
	int ArgCount = 0;
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		switch (lua_type(L, x * -1)) {
		case LUA_TNIL: {
			//rbxlua_pushnil(rbxlua_State);
			break;
		}
		case LUA_TNUMBER: {
			double no = lua_tonumber(L, x * -1);
			//rbxlua_pushnumber(rbxlua_State, no);
			break;
		}
		case LUA_TSTRING: {
			const char* str = lua_tostring(L, x * -1);
			rbxlua_pushstring(luaState, str);
			break;
		}
		case LUA_TTABLE: {
			lua_getfield(L, -1, "__address");
			if (!lua_isnoneornil(L, -1)) {
				//int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
				//*userdata = lua_tonumber(L, x * -1);
				//*(userdata + 1) = 0;
				rbxlua_getfield(luaState, -10000, "Object");
				//rbxlua_setmetatable(rbxlua_State, -2);
			}
			else {
				lua_pop(L, 1);
				rbxlua_pushstring(luaState, "table: #NULL");
			}
			break;
		}
		default:
		{
			const char* str = lua_tostring(L, x * -1);
			rbxlua_pushstring(luaState, str);
			break;
		}
		}
		ArgCount += 1;
	}
	rbxlua_pcall(luaState, ArgCount, 0, 0);
	return 0;
}

static int LuaError(lua_State* L) {
	int n = lua_gettop(L);  // number of arguments
	int i;
	std::string out;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  //
		lua_pushvalue(L, i);
		lua_call(L, 1, 1);
		s = lua_tostring(L, -1);
		if (s == NULL)
			return luaL_error(L, LUA_QL("tostring") " must return a string to "
				LUA_QL("print"));
		if (i > 1) out += "\t";
		out += s;
		lua_pop(L, 1);
	}
	rbxlua_getglobal(luaState, "error");
	rbxlua_pushstring(luaState, out.c_str());
	//rbxlua_pushnumber(luaState, 1);
	rbxlua_pcall(luaState, 1, 0, 0);
	return 0;
}

static int LuaConsolePrint(lua_State* L) {
	int n = lua_gettop(L);  // number of arguments
	int i;
	std::string out;
	lua_getglobal(L, "tostring");
	for (i = 1; i <= n; i++) {
		const char *s;
		lua_pushvalue(L, -1);  // function to be called
		lua_pushvalue(L, i);   // value to print
							   //rbxlua_call(L, 1, 1);
		if (!lua_isnil(L, i)) {
			if (lua_isstring(L, -1)) {
				s = lua_tostring(L, -1);  // get result
				if (s == NULL) {
					s = "nil";
					//return luaL_error(L, LUA_QL("tostring") " must return a string to "
					//	LUA_QL("print"));
				}
			}
			/*else {
			void *udata = lua_touserdata(L, -1);
			if (udata != NULL) {
			s = GetClass((int)udata);
			}
			}*/
		}
		else {
			s = "nil";
		}
		if (i > 1) out += "\t";
		out += s;
		lua_pop(L, 1);  // pop result
	}
	out = out + "\r\n";
	ConsoleOutput(out);
	return 0;
}

static std::map < std::string, std::map<std::string, lua_CFunction> > ClassFuncs = {
	{ "RemoteEvent",{
		{ "FireServer", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "FireServer");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				int numberofargs = lua_gettop(L) - 1;
				if (numberofargs >= 0) {
					for (int i = 1; i <= numberofargs; i++) {
						int currentIndex = i + 1;
						if (lua_isstring(L, currentIndex)) {
							const char* val = luaL_checkstring(L, currentIndex);
							rbxlua_pushstring(luaState, val);
						}
						else if (lua_isboolean(L, currentIndex)) {
							bool val = lua_toboolean(L, currentIndex);
							rbxlua_pushbool(luaState, val ? 1 : 0);
						}
						else if (lua_isnumber(L, currentIndex)) {
							double val = luaL_checknumber(L, currentIndex);
							rbxlua_pushnumber(luaState, val);
						}
					}
					rbxlua_pcall(luaState, (numberofargs + 1), 0, 0);
					return 1;
				}
				else {
					clearStack(luaState);
				}
			}
			else {
				clearStack(luaState);
			}
		}
		else {
			clearStack(luaState);
		}
	}
	return 0;
} }
	} },
	/*{ "Debris",{
	{ "AddItem", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
	if (!lua_isnumber(L, 3)) luaL_argerror(L, 3, "number expected");
	rbxWrapToLua(luaState, inst->InstanceRef);
	rbxlua_getfield(luaState, -1, "AddItem");
	rbxlua_pushvalue(luaState, -2);

	rbxWrapToLua(luaState, inst->InstanceRef);
	rbxlua_pushvalue(luaState, -2);

	rbxlua_pcall(luaState, 2, 0, 0);
	}
	return 0;
	} }
	} },*/
	{ "StarterGui",{
		{ "SetCoreGuiEnabled", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		const char* TypeGui = luaL_checkstring(L, 2);
		if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
		bool boolval = lua_toboolean(L, 3);
		BOOL REAL = FALSE;
		if (boolval == true) {
			REAL = TRUE;
		}
		rbxlua_getservice(luaState, "StarterGui");
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "SetCoreGuiEnabled");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pushstring(luaState, TypeGui);
				rbxlua_pushbool(luaState, REAL);
				rbxlua_pcall(luaState, 3, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} }
	} },
	{ "Sound",{
		{ "Play", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Play");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0); Sleep(150);
				return 1;
			}
		}
	}
	return 0;
} },
{ "play", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Play");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0); Sleep(150);
				return 1;
			}
		}
	}
	return 0;
} },
{ "Stop", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Stop");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "stop", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Stop");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "Resume", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Resume");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "Pause", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Pause");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} }
	} },
	{ "Part",{
		{ "BreakJoints", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		//BreakJoints(inst->InstanceRef);
		return 1;
	}
	return 0;
} },
{ "breakJoints", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		//BreakJoints(inst->InstanceRef);
		return 1;
	}
	return 0;
} },
	} },
	{ "Model",{
		{ "BreakJoints", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		//BreakJoints(inst->InstanceRef);
		return 1;
	}
	return 0;
} },
{ "breakJoints", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		//BreakJoints(inst->InstanceRef);
		return 1;
	}
	return 0;
} },
	} },
	{ "Workspace",{
		{ "BreakJoints", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		//BreakJoints(inst->InstanceRef);
		return 1;
	}
	return 0;
} },
{ "breakJoints", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		//BreakJoints(inst->InstanceRef);
		return 1;
	}
	return 0;
} },
	} },
	{ "Terrain",{
		{ "Clear", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Clear");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "clear", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		//ClearTerrain(inst->InstanceRef);
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "clear");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
	} },
	{ "Teams",{
		{ "GetTeams", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		std::vector<DWORD> retTeams = GetChildren(inst->InstanceRef);
		lua_newtable(L);
		for (int c = 0; c < retTeams.size(); ++c) {
			if (strcmp(GetClass(retTeams[c]), "Team") == 0) {
				lua_pushinteger(L, c + 1);
				WrapToLua(L, retTeams[c]);
				lua_settable(L, -3);
			}
		}
		lua_pushvalue(L, -1);
		return 1;
	}
	return 0;
} },
{ "RebalanceTeams", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		//ClearTerrain(inst->InstanceRef);
		rbxlua_getservice(luaState, "Teams");
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "RebalanceTeams");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
	} },
	{ "Players",{
		{ "GetPlayers", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		std::vector<DWORD> Children = GetPlayers();
		lua_newtable(L);
		for (int c = 0; c < Children.size(); ++c) {
			lua_pushinteger(L, c + 1);
			WrapToLua(L, Children[c]);
			lua_settable(L, -3);
		}
		lua_pushvalue(L, -1);
		return 1;
	}
	return 0;
} },
{ "getPlayers", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		std::vector<DWORD> Children = GetPlayers();
		lua_newtable(L);
		for (int c = 0; c < Children.size(); ++c) {
			lua_pushinteger(L, c + 1);
			WrapToLua(L, Children[c]);
			lua_settable(L, -3);
		}
		lua_pushvalue(L, -1);
		return 1;
	}
	return 0;
} },
{ "players", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		std::vector<DWORD> Children = GetPlayers();
		lua_newtable(L);
		for (int c = 0; c < Children.size(); ++c) {
			lua_pushinteger(L, c + 1);
			WrapToLua(L, Children[c]);
			lua_settable(L, -3);
		}
		lua_pushvalue(L, -1);
		return 1;
	}
	return 0;
} },
	} },
	{ "Player",{
		{ "ClearCharacterAppearance", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "ClearCharacterAppearance");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "GetMouse", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(TeleportState, -1, "GetMouse");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(TeleportState, -2);
				rbxlua_pcall(TeleportState, 1, 1, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "JumpCharacter", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "JumpCharacter");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "LoadCharacter", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "LoadCharacter");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "RemoveCharacter", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "RemoveCharacter");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} }
	} },
	{ "DataModel",{
		{ "Load", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		const char* URL = luaL_checkstring(L, 2);
		rbxlua_getglobal(luaState, "game");
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Load");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pushstring(luaState, URL);
				rbxlua_pcall(luaState, 2, 0, 0); Sleep(50);
				return 1;
			}
		}
		luaL_error(L, "Unknown error occured!", URL);
	}
	return 0;
} },
{ "HttpGet", [](lua_State* L)->int {
    Inst* instance = CheckInstance<Inst>(L, 1);
	const char* URL = luaL_checkstring(L, 2);
	if (std::strstr(URL, "edufeedback") == NULL) {
		if (URL != nullptr) {
			rbxlua_getglobal(luaState, "game");
			if (!rbxlua_isnil(luaState, -1)) {
				rbxlua_getfield(luaState, -1, "HttpGet");
				if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
					rbxlua_pushvalue(luaState, -2);
					rbxlua_pushstring(luaState, URL);
					if (lua_isboolean(L, 3)) {
						//bool val = rbxlua_toboolean(luaState, 2);
				//		rbxlua_pushbool(luaState, val ? 1 : 0);
					}
					else {
					//	rbxlua_pushbool(luaState, 1);
					}
					rbxlua_pcall(luaState, 3, 1, 0); Sleep(50);
					const std::string returnval = rbxlua_tostring(luaState, -1);
					lua_pushstring(L, returnval.c_str());
					return 1;
				}
			}
			luaL_error(L, "Unknown error occured!", URL);
		}
	}
	return 0;
} },
{ "HttpPost", [](lua_State* L)->int {
	const char* URL = luaL_checkstring(L, 2);
	const char* data = luaL_checkstring(L, 3);
	const char* contentType;
	bool synchorized = 1;
	int mon = 4;
	if (!lua_isboolean(L, 4)) {
		contentType = luaL_checkstring(L, 4);
	}
	else if (!lua_isboolean(L, 4)) {
		synchorized = lua_toboolean(L, 4);
		if (lua_isstring(L, 5)) {
			contentType = luaL_checkstring(L, 5);
		}
	}
	if (std::strstr(URL, "edufeedback") == NULL) {
		if (URL != nullptr) {
			rbxlua_getglobal(luaState, "game");
			if (!rbxlua_isnil(luaState, -1)) {
				rbxlua_getfield(luaState, -1, "HttpPost");
				if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
					rbxlua_pushvalue(luaState, -2);
					rbxlua_pushstring(luaState, URL);
					rbxlua_pushstring(luaState, data);
					if (lua_isboolean(L, 4)) {
						rbxlua_pushbool(luaState, synchorized ? 1 : 0);
					}
					else {
						rbxlua_pushbool(luaState, 1);
					}
					if (lua_isstring(L, 5)) {
						rbxlua_pushstring(luaState, contentType);
						mon = mon + 1;
					}
					rbxlua_pcall(luaState, mon, 1, 0); Sleep(50);
					//const std::string returnval = rbxlua_tostring(luaState, -1);
					//lua_pushstring(L, returnval.c_str());
					return 1;
				}
			}
			luaL_error(L, "Unknown error occured!", URL);
		}
	}
	return 0;
} },{ "GetService", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		const char* Service = luaL_checkstring(L, 2);
		std::vector<DWORD> Services = GetChildren(inst->InstanceRef);
		for (int c = 0; c < Services.size(); c++) {
			if (strcmp(GetName(Services[c])->c_str(), Service) == 0) {
				WrapToLua(L, Services[c]);
				lua_pushvalue(L, -1);
				return 1;
			}
		}
		luaL_error(L, "'%s' is not a valid service name.", Service);
	}
	return 0;
} },{ "service", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		const char* Service = luaL_checkstring(L, 2);
		std::vector<DWORD> Services = GetChildren(inst->InstanceRef);
		for (int c = 0; c < Services.size(); c++) {
			if (strcmp(GetName(Services[c])->c_str(), Service) == 0) {
				WrapToLua(L, Services[c]);
				lua_pushvalue(L, -1);
				return 1;
			}
		}
		luaL_error(L, "'%s' is not a valid service name.", Service);
	}
	return 0;
} },
	} },
	/*{ "LocalScript",{
	{ "BreakScript", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
	((void(__thiscall*)(int, int))0x503DF0)(ScriptContext, inst->InstanceRef);
	}
	return 0;
	} },
	} },*/
	{ "1337IsBack",{
		{ "remove", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "remove");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "Remove", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Remove");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "destroy", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "destroy");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "Destroy", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		rbxWrapToLua(luaState, inst->InstanceRef);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_getfield(luaState, -1, "Destroy");
			if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "GetChildren", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		std::vector<DWORD> Children = GetChildren(inst->InstanceRef);
		lua_newtable(L);
		for (std::size_t c = 0; c < Children.size(); ++c) {
			if (Children[c] != 0) {
				lua_pushinteger(L, c + 1);
				WrapToLua(L, Children[c]);
				lua_settable(L, -3);
			}
		}
		lua_pushvalue(L, -1);
		return 1;
	}
	return 0;
} },
{ "getChildren", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		std::vector<DWORD> Children = GetChildren(inst->InstanceRef);
		lua_newtable(L);
		for (std::size_t c = 0; c < Children.size(); ++c) {
			if (Children[c] != 0) {
				lua_pushinteger(L, c + 1);
				WrapToLua(L, Children[c]);
				lua_settable(L, -3);
			}
		}
		lua_pushvalue(L, -1);
		return 1;
	}
	return 0;
} },
{ "children", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		std::vector<DWORD> Children = GetChildren(inst->InstanceRef);
		lua_newtable(L);
		for (std::size_t c = 0; c < Children.size(); ++c) {
			if (Children[c] != 0) {
				lua_pushinteger(L, c + 1);
				WrapToLua(L, Children[c]);
				lua_settable(L, -3);
			}
		}
		lua_pushvalue(L, -1);
		return 1;
	}
	return 0;
} },
{ "Clone", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		if (inst->InstanceRef != 0) {
			rbxWrapToLua(luaState, inst->InstanceRef); Sleep(50);
			if (!rbxlua_isnil(luaState, -1)) {
				rbxlua_getfield(luaState, -1, "Clone");
				if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
					rbxlua_pushvalue(luaState, -2);
					rbxlua_pcall(luaState, 1, 1, 0); Sleep(50);
					if (!rbxlua_isnil(luaState, -1)) {
						int newIns = rbxlua_touserdata(luaState, -1);
						if (newIns != 0) {
							WrapToLua(L, newIns);
							lua_pushvalue(L, -1); Sleep(5);
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
} },
{ "clone", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		if (inst->InstanceRef != 0) {
			rbxWrapToLua(luaState, inst->InstanceRef); Sleep(50);
			if (!rbxlua_isnil(luaState, -1)) {
				rbxlua_getfield(luaState, -1, "Clone");
				if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
					rbxlua_pushvalue(luaState, -2);
					rbxlua_pcall(luaState, 1, 1, 0); Sleep(50);
					if (!rbxlua_isnil(luaState, -1)) {
						int newIns = rbxlua_touserdata(luaState, -1);
						if (newIns != 0) {
							WrapToLua(L, newIns);
							lua_pushvalue(L, -1); Sleep(5);
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
} },
{ "IsA", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		const char* MoreCool = luaL_checkstring(L, 2);
		const char* Ins = GetClass(inst->InstanceRef);
		if (Ins != "0" && Ins != 0) {
			if (strcmp(MoreCool, Ins) == 0) {
				lua_pushboolean(L, 1);
				return 1;
			}
			else {
				lua_pushboolean(L, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "isA", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		const char* MoreCool = luaL_checkstring(L, 2);
		const char* Ins = GetClass(inst->InstanceRef);
		if (Ins != "0" && Ins != 0) {
			if (strcmp(MoreCool, Ins) == 0) {
				lua_pushboolean(L, 1);
				return 1;
			}
			else {
				lua_pushboolean(L, 0);
				return 1;
			}
		}
	}
	return 0;
} },
{ "FindService", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		const char* MoreCool = luaL_checkstring(L, 2);
		int Ins = FindFirstClass(inst->InstanceRef, MoreCool);
		if (Ins != 0) {
			WrapToLua(L, Ins);
			lua_pushvalue(L, -1);
			return 1;
		}
	}
	return 0;
} },
{ "FindFirstChild", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		const char* MoreCool = luaL_checkstring(L, 2);
		int Ins = FindFirstChild(inst->InstanceRef, std::string(MoreCool));
		if (Ins != 0) {
			WrapToLua(L, Ins);
			lua_pushvalue(L, -1);
			return 1;
		}
	}
	return 0;
} },
{ "findFirstChild", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		const char* MoreCool = luaL_checkstring(L, 2);
		int Ins = FindFirstChild(inst->InstanceRef, std::string(MoreCool));
		if (Ins != 0) {
			WrapToLua(L, Ins);
			lua_pushvalue(L, -1);
			return 1;
		}
	}
	return 0;
} },
{ "GetFullName", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		int indexIns = inst->InstanceRef;
		if (indexIns != 0) {
			std::string strIns = GetFullName(indexIns);
			if (strIns != "0" || strIns != "." || strIns != "") {
				lua_pushstring(L, strIns.c_str());
				return 1;
			}
		}
	}
	return 0;
} },
{ "ClearAllChildren", [](lua_State* L)->int {
	Inst* inst = CheckInstance<Inst>(L, 1);
	if (inst != nullptr) {
		int indexIns = inst->InstanceRef;
		if (indexIns != 0) {
			rbxWrapToLua(luaState, indexIns);
			if (!rbxlua_isnil(luaState, -1)) {
				rbxlua_getfield(luaState, -1, "ClearAllChildren");
				if (rbxlua_isfunction(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
					rbxlua_pushvalue(luaState, -2);
					rbxlua_pcall(luaState, 1, 0, 0);
					return 1;
				}
			}
		}
	}
	return 0;
} },
	} },
};

static BOOL rbxlua_toboolean(DWORD luaState, int idx)
{
	return FALSE;
}


#define BOOL_VALUE				1
#define NUMBER_VALUE			2
#define INSTANCE_VALUE			3
#define STRING_VALUE			4
#define VECTOR3_VALUE			5
#define CFRAME_VALUE			6
#define COLOR3_VALUE			7

static int GetType(lua_State* L, const char* Property, int Type, Inst* Instance) {
	if (Instance != nullptr) {
		int Ins = Instance->InstanceRef;
		if (Ins != 0) {
			switch (Type) {
			case BOOL_VALUE: {
				rbxWrapToLua(luaState, Ins);
				if (!rbxlua_isnil(luaState, -1)) {
					rbxlua_getfield(luaState, -1, Property); Sleep(25);
					bool asd = rbxlua_toboolean(luaState, -1);
					lua_pushboolean(L, asd);
				}
			}
			case NUMBER_VALUE: {
				rbxWrapToLua(luaState, Ins);
				if (!rbxlua_isnil(luaState, -1)) {
					rbxlua_getfield(luaState, -1, Property); Sleep(25);
					int asd = rbxlua_tonumber(luaState, -1);
					lua_pushnumber(L, asd);
				}
			}
			case INSTANCE_VALUE: {
				rbxWrapToLua(luaState, Ins);
				if (!rbxlua_isnil(luaState, -1)) {
					rbxlua_getfield(luaState, -1, Property); Sleep(25);
					const char* asd = rbxlua_tostring(luaState, -1);
					lua_pushstring(L, asd);
				}
			}
			case STRING_VALUE: {
				rbxWrapToLua(luaState, Ins);
				if (!rbxlua_isnil(luaState, -1)) {
					rbxlua_getfield(luaState, -1, Property); Sleep(25);
					const char* asd = rbxlua_tostring(luaState, -1);
					lua_pushstring(L, asd);
				}
			}
			}
		}
	}
	return 1;
}

typedef double(__thiscall *doublertn)(int self);

static int ReturnValue(lua_State* L, std::string ClassName, std::string Property, Inst* Instance) {
	if (Instance != nullptr) {
		int Ins = Instance->InstanceRef;
		if (Ins != 0) {
			if (ClassName == "Humanoid") {
				if (Property == "Health" || Property == "MaxHealth") {
					GetType(L, Property.c_str(), NUMBER_VALUE, Instance);
					return 1;
				}
			}
			else if (ClassName == "TextLabel" || ClassName == "TextBox" || ClassName == "TextButton") {
				if (Property == "Text") {
					GetType(L, Property.c_str(), STRING_VALUE, Instance);
					return 1;
				}
			}
			else if (ClassName == "Sound") {
				if (Property == "SoundId") {
					GetType(L, Property.c_str(), STRING_VALUE, Instance);
					return 1;
				}
			}
			else if (ClassName == "Decal" || ClassName == "Sky") {
				if (Property == "Texture" || Property == "SkyboxBk" || Property == "SkyboxDn" || Property == "SkyboxFt" || Property == "SkyboxLf" || Property == "SkyboxRt" || Property == "SkyboxUp") {
					GetType(L, Property.c_str(), STRING_VALUE, Instance);
					return 1;
				}
			}
			else if (ClassName == "Pants" || ClassName == "Shirt") {
				if (Property == "PantsTemplate" || Property == "ShirtTemplate") {
					GetType(L, Property.c_str(), STRING_VALUE, Instance);
					return 1;
				}
			}
		}
	}
	return 0;
}

/*int SetValue(lua_State* L, const char* Property, int Type, Inst* Instance) {
	if (Instance != nullptr) {
		int Ins = Instance->InstanceRef;
		if (Ins != 0) {
			switch (Type) {
			case BOOL_VALUE: {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				if (!rbxlua_isnil(luaState, -1)) {
					rbxlua_pushbool(luaState, val ? 1 : 0);
					rbxlua_setfield(luaState, -2, Property);
				}
			}
			case NUMBER_VALUE: {
				double val = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				if (!rbxlua_isnil(luaState, -1)) {
					rbxlua_pushnumber(luaState, val);
					rbxlua_setfield(luaState, -2, Property);
				}
			}
			case INSTANCE_VALUE: {
				rbxWrapToLua(luaState, Ins);
				if (!rbxlua_isnil(luaState, -1)) {
					rbxlua_getfield(luaState, -1, Property); Sleep(25);
					const char* asd = rbxlua_tostring(luaState, -1);
					lua_pushstring(L, asd);
				}
			}
			case STRING_VALUE: {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				if (!rbxlua_isnil(luaState, -1)) {
					rbxlua_pushstring(luaState, val);
					rbxlua_setfield(luaState, -2, Property);
				}
			}
			case VECTOR3_VALUE: {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3 != nullptr) {
					rbxWrapToLua(luaState, Ins);
					if (!rbxlua_isnil(luaState, -1)) {
						rbxlua_getglobal(luaState, "Vector3");
						rbxlua_getfield(luaState, -1, "new");
						rbxlua_pushnumber(luaState, LuaVec3->X);
						rbxlua_pushnumber(luaState, LuaVec3->Y);
						rbxlua_pushnumber(luaState, LuaVec3->Z);
						rbxlua_pcall(luaState, 3, 1, 0);
						rbxlua_setfield(luaState, -3, Property);
					}
				}
			}
			case CFRAME_VALUE: {
				Types::CFrame* LuaCFrame3 = Types::CheckCFrame(L, 3); Sleep(10);
				if (LuaCFrame3 != nullptr) {
					rbxWrapToLua(luaState, Ins);
					if (!rbxlua_isnil(luaState, -1)) {
						rbxlua_getglobal(luaState, "CFrame");
						rbxlua_getfield(luaState, -1, "new");
						rbxlua_pushnumber(luaState, LuaCFrame3->X);
						rbxlua_pushnumber(luaState, LuaCFrame3->Y);
						rbxlua_pushnumber(luaState, LuaCFrame3->Z);
						rbxlua_pcall(luaState, 3, 1, 0);
						rbxlua_setfield(luaState, -3, Property);
					}
				}
			}
			case COLOR3_VALUE: {
				Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
				if (LuaColor3 != nullptr) {
					rbxWrapToLua(luaState, Ins);
					if (!rbxlua_isnil(luaState, -1)) {
						rbxlua_getglobal(luaState, "Color3");
						rbxlua_getfield(luaState, -1, "new");
						rbxlua_pushnumber(luaState, LuaColor3->X);
						rbxlua_pushnumber(luaState, LuaColor3->Y);
						rbxlua_pushnumber(luaState, LuaColor3->Z);
						rbxlua_pcall(luaState, 3, 1, 0);
						rbxlua_setfield(luaState, -3, Property);
					}

				}
			}
			case BRICKCOLOR_VALUE: {
				rbxWrapToLua(luaState, Ins);
				if (!rbxlua_isnil(luaState, -1)) {
					rbxlua_pushvalue(luaState, -3);
					rbxlua_setfield(luaState, -2, Property);
				}
			}
			}
		}
	}
	return 1;
}*/


static int luaIndex(lua_State* L) {
	Inst* inst = CheckInstance<Inst>(L, 1);
	const char* Index = luaL_checkstring(L, 2);
	if (inst) {
		int Ins = inst->InstanceRef;
		std::string InsName(*GetName(Ins));
		std::string Class(GetClass(Ins));
		std::string Prop(Index);

		if (Class == "Workspace") {
			if (Prop == "FilteringEnabled") {
				lua_pushboolean(L, (int)GetFilteringEnabled(Workspace));;
				return 1;
			}
			else if (Prop == "CurrentCamera") {
				WrapToLua(L, GetCurrentCamera(Workspace));
				lua_pushvalue(L, -1);
				return 1;
			}
		}
		else if (Class == "Part") {
			rbxWrapToLua(luaState, Ins);
			rbxlua_getfield(luaState, -1, Prop.c_str());
			const char* ret = rbxlua_tostring(luaState, -1);//*(IntVal_ZeroVal) ^ *(DWORD*)(Ins + 72);
			lua_pushstring(L, ret);
			return 1;
		}
		else if (Class == "Player") {
			if (Prop == "Character") {
				WrapToLua(L, GetCharacterFromPlayer(Ins));
				lua_pushvalue(L, -1);
				return 1;
			}
			else if (Prop == "userId" || Prop == "UserId") {
			//	lua_pushinteger(L, GetPlayerUserId(Ins));
				return 1;
			}
			else if (Prop == "AccountAge") {
				lua_pushinteger(L, GetAccountAge(Ins));
				return 1;
			}
		}
		else if (Class == "Players") {
			if (Prop == "LocalPlayer" || Prop == "localPlayer") {
				WrapToLua(L, GetLocalPlayer(Ins));
				lua_pushvalue(L, -1);
				return 1;
			}
		}
		else if (Class == "IntValue") {
			if (Prop == "Value") {
				rbxWrapToLua(luaState, Ins);
				rbxlua_getfield(luaState, -1, Prop.c_str());
				double ret = rbxlua_tonumber(luaState, -1);//*(IntVal_ZeroVal) ^ *(DWORD*)(Ins + 72);
				lua_pushnumber(L, ret);
				return 1;
			}
		}
		else if (Class == "NumberValue") {
			if (Prop == "Value") {
				rbxWrapToLua(luaState, Ins);
				rbxlua_getfield(luaState, -1, Prop.c_str());
				if (rbxlua_isstring(luaState, -1)) {
					double ret = rbxlua_tonumber(luaState, -1);//*(IntVal_ZeroVal) ^ *(DWORD*)(Ins + 72);
					lua_pushnumber(L, ret);
					return 1;
				}
			}
		}
		else if (Class == "StringValue") {
			if (Prop == "Value") {
				rbxWrapToLua(luaState, Ins);
				rbxlua_getfield(luaState, -1, Prop.c_str());
				const char* asd = rbxlua_tostring(luaState, -1);//(std::string*)(Ins + 72);
				lua_pushstring(L, asd);
				return 1;
			}
		}
		else if (Class == "BoolValue") {
			if (Prop == "Value") {
				rbxWrapToLua(luaState, Ins);
				rbxlua_getfield(luaState, -1, Prop.c_str());
				bool asd = rbxlua_toboolean(luaState, -1);//*(bool*)(Ins + 72);
				lua_pushboolean(L, asd);
				return 1;
			}
		}
		/*else if (Class == "ObjectValue") {
		if (Prop == "Value") {
		int asd = GetObjectValue(Ins);
		if (asd != 0) {
		WrapToLua(L, asd);
		lua_pushvalue(L, -1);
		return 1;
		}
		lua_pushnil(L);
		return 1;
		}
		}*/
		if (Prop == "Name") {
			lua_pushstring(L, InsName.c_str());
			return 1;
		}
		else if (Prop == "ClassName" || Prop == "className") {
			lua_pushstring(L, Class.c_str());
			return 1;
		}
		else if (Prop == "Parent") {
			WrapToLua(L, GetParent(Ins));
			lua_pushvalue(L, -1);
			return 1;
		}
		else if (Prop == "RobloxLocked") {
			lua_pushboolean(L, GetRobloxLocked(Ins));
			return 1;
		}

		if (ClassFuncs.find(Class) != ClassFuncs.end()) {
			if (ClassFuncs[Class].find(Prop) != ClassFuncs[Class].end()) {
				lua_pushcfunction(L, ClassFuncs[Class][Prop]);
				return 1;
			}
		}

		if (ClassFuncs["1337IsBack"].find(Prop) != ClassFuncs["1337IsBack"].end()) {
			lua_pushcfunction(L, ClassFuncs["1337IsBack"][Prop]);
			return 1;
		}

		int Child = FindFirstChild_PartOf(Ins, Prop);
		if (Child) {
			WrapToLua(L, Child);
			lua_pushvalue(L, -1);
			return 1;
		}

		/*rbxWrapToLua(luaState, Ins);
		if (!rbxlua_isnil(luaState, -1)) {
		rbxlua_getfield(luaState, -1, Prop.c_str());
		if (!rbxlua_isnoneornil(luaState, -1)) {
		const char* asd = rbxlua_tostring(luaState, -1);
		if (strcmp(asd, "nil") != 0) {
		lua_pushstring(L, asd);
		}
		else {
		bool asdf = rbxlua_toboolean(luaState, -1);
		lua_pushboolean(L, asdf);
		//else {
		//	double ret = rbxlua_tonumber(luaState, -1);
		//	lua_pushnumber(L, ret);
		//}
		}
		}
		}
		return 1;*/

		//luaL_error(L, "%s is not a valid member of %s", Prop.c_str(), GetName(Ins).c_str());
	}
	return 0;
}

static int luaNewIndex(lua_State* L) {
	Inst* inst = CheckInstance<Inst>(L, 1);
	const char* Index = luaL_checkstring(L, 2);
	if (inst) {
		int Ins = inst->InstanceRef;
		std::string Prop(Index);
		std::string Class = GetClass(Ins);
		bool setProp = false;
		if (Class == "Part" || Class == "VehicleSeat" || Class == "TrussPart" || Class == "RightAngleRampPart" || Class == "PyramidPart" || Class == "PrismPart" || Class == "ParallelRampPart" || Class == "MeshPart" || Class == "CornerWedgePart") {
			if (Prop == "Anchored") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				if (val == true) {
					SetAnchored(Ins, 1); //? 1 : 0);
				}
				else {
					SetAnchored(Ins, 0);
				}
				setProp = true;
			}
			else if (Prop == "Transparency") {
				double Trans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Trans);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Reflectance") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Locked") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "CanCollide") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BackParamA") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BackParamB") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BottomParamA") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BottomParamB") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "FrontParamA") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "FrontParamB") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "LeftParamA") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "LeftParamB") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "RightParamA") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "RightParamB") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Elasticity") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Friction") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "LocalTransparencyModifier") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "ReceiveAge") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "ResizeIncrement") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "SpecificGravity") {
				double Reflec = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Reflec);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Material") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BackSurface") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BackSurfaceInput") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BottomSurface") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BottomSurfaceInput") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "FrontSurface") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "FrontSurfaceInput") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "LeftSurface") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "LeftSurfaceInput") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "RightSurface") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "RightSurfaceInput") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "TopSurface") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "TopSurfaceInput") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Shape") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Size") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Position") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "RotVelocity") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Rotation") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Velocity") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "BrickColor" || Prop == "brickColor") {
				Types::BrickColor* LuaBrickColor = Types::CheckBrickColor(L, 3);
				if (LuaBrickColor) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "BrickColor");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushstring(luaState, LuaBrickColor->A);
					rbxlua_pcall(luaState, 1, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}

			if (setProp == false) {
				if (Class == "Part") {
					if (Prop == "FormFactor" || Prop == "formFactor") {
						const char* val = luaL_checkstring(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushstring(luaState, val);
						rbxlua_setfield(luaState, -2, Prop.c_str());
						setProp = true;
					}
				}
			}
		}
		else if (Class == "Lighting") {
			if (Prop == "FogEnd") {
				//int fog = luaL_checkinteger(L, 3);
				//SetFogEnd(Ins, fog);
				int val = luaL_checknumber(L, 3);
				rbxlua_getservice(luaState, "Lighting");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "FogStart") {
				//int fog = luaL_checkinteger(L, 3);
				//SetFogStart(Ins, fog);
				int val = luaL_checknumber(L, 3);
				rbxlua_getservice(luaState, "Lighting");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "FogColor") {
				Types::Color3* LuaColor3 = Types::CheckColor3(L, -1);
				if (LuaColor3) {
					rbxlua_getglobal(luaState, "game");
					rbxlua_getfield(luaState, -1, "Lighting");
					rbxlua_getglobal(luaState, "Color3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushstring(luaState, std::to_string(LuaColor3->X).c_str());
					rbxlua_pushstring(luaState, std::to_string(LuaColor3->Y).c_str());
					rbxlua_pushstring(luaState, std::to_string(LuaColor3->Z).c_str());
					rbxlua_pcall(luaState, 2, 1, 0);
					rbxlua_setfield(luaState, -2, Prop.c_str());
					setProp = true;
				}
				else {
					luaL_argerror(L, 3, "table expected");
				}
			}
			else if (Prop == "GlobalShadows") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxlua_getservice(luaState, "Lighting");
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Outlines") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxlua_getservice(luaState, "Lighting");
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Brightness") {
				int val = luaL_checkinteger(L, 3);
				rbxlua_getservice(luaState, "Lighting");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "GeographicLatitude") {
				int val = luaL_checkinteger(L, 3);
				rbxlua_getservice(luaState, "Lighting");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "TimeOfDay") {
				int val = luaL_checkinteger(L, 3);
				rbxlua_getservice(luaState, "Lighting");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "Accoutrement" || Class == "Hat") {
			if (Prop == "AttachmentForward") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, -1);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "AttachmentPoint") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "AttachmentPos") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "AttachmentRight") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "AttachmentUp") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "BlockMesh") {
			if (Prop == "Offset") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Scale") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "VertexColor") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "CylinderkMesh") {
			if (Prop == "Offset") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Scale") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "VertexColor") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "Sky" || Prop == "SkyboxBk" || Prop == "SkyboxDn" || Prop == "SkyboxFt" || Prop == "SkyboxLf" || Prop == "SkyboxRt" || Prop == "SkyboxUp")
		{
			std::string id = lua_tostring(L, -1);
			rbxlua_getservice(CluaState, "Lighting");
			rbxlua_getglobal(CluaState, "Instance");
			rbxlua_getfield(CluaState, -1, "new");
			rbxlua_pushstring(CluaState, "Sky");
			rbxlua_pushvalue(CluaState, -4);
			rbxlua_pcall(CluaState, 2, 1, 0); Sleep(15);
			rbxlua_pushstring(CluaState, id.c_str());
			rbxlua_setfield(CluaState, -2, "SkyboxBk");

			rbxlua_pushstring(CluaState, id.c_str());
			rbxlua_setfield(CluaState, -2, "SkyboxDn");

			rbxlua_pushstring(CluaState, id.c_str());
			rbxlua_setfield(CluaState, -2, "SkyboxFt");

			rbxlua_pushstring(CluaState, id.c_str());
			rbxlua_setfield(CluaState, -2, "SkyboxLf");

			rbxlua_pushstring(CluaState, id.c_str());
			rbxlua_setfield(CluaState, -2, "SkyboxRt");

			rbxlua_pushstring(CluaState, id.c_str());
			rbxlua_setfield(CluaState, -2, "SkyboxUp");
		}
		else if (Class == "SpecialMesh") {
			if (Prop == "MeshType") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Offset") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Scale") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "VertexColor") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "PointLight") {
			if (Prop == "Brightness") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Range") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Enabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Shadows") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "SpotLight") {
			if (Prop == "Brightness") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Range") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Angle") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Face") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Enabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Shadows") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "SurfaceLight") {
			if (Prop == "Brightness") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Range") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Angle") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Face") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Enabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Shadows") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "ParticleEmitter") {
			if (Prop == "Drag") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "LightEmission") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Rate") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "VelocityInheritance") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "VelocitySpread") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "ZOffset") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Texture") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Enabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "LockedToPart") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Acceleration") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Lifetime") {
				Types::NumberRange* LuaNumberRange = Types::CheckNumberRange(L, 3);
				if (LuaNumberRange) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "NumberRange");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaNumberRange->A);
					rbxlua_pushnumber(luaState, LuaNumberRange->B);
					rbxlua_pcall(luaState, 2, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "RotSpeed") {
				Types::NumberRange* LuaNumberRange = Types::CheckNumberRange(L, 3);
				if (LuaNumberRange) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "NumberRange");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaNumberRange->A);
					rbxlua_pushnumber(luaState, LuaNumberRange->B);
					rbxlua_pcall(luaState, 2, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Rotation") {
				Types::NumberRange* LuaNumberRange = Types::CheckNumberRange(L, 3);
				if (LuaNumberRange) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "NumberRange");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaNumberRange->A);
					rbxlua_pushnumber(luaState, LuaNumberRange->B);
					rbxlua_pcall(luaState, 2, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Speed") {
				Types::NumberRange* LuaNumberRange = Types::CheckNumberRange(L, 3);
				if (LuaNumberRange) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "NumberRange");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaNumberRange->A);
					rbxlua_pushnumber(luaState, LuaNumberRange->B);
					rbxlua_pcall(luaState, 2, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "Smoke") {
			if (Prop == "Opacity") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "RiseVelocity") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, "Size");
				setProp = true;
			}
			else if (Prop == "Size") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, "Size");
				setProp = true;
			}
			else if (Prop == "Enabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Color") {
				Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
				if (LuaColor3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Color3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaColor3->X);
					rbxlua_pushnumber(luaState, LuaColor3->Y);
					rbxlua_pushnumber(luaState, LuaColor3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "Fire") {
			if (Prop == "Heat") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Size" || Prop == "size") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, "Size");
				setProp = true;
			}
			else if (Prop == "Enabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Color") {
				Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
				if (LuaColor3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Color3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaColor3->X);
					rbxlua_pushnumber(luaState, LuaColor3->Y);
					rbxlua_pushnumber(luaState, LuaColor3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "SecondaryColor") {
				Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
				if (LuaColor3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Color3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaColor3->X);
					rbxlua_pushnumber(luaState, LuaColor3->Y);
					rbxlua_pushnumber(luaState, LuaColor3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "Sparkles") {
			if (Prop == "Enabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Color") {
				Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
				if (LuaColor3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Color3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaColor3->X);
					rbxlua_pushnumber(luaState, LuaColor3->Y);
					rbxlua_pushnumber(luaState, LuaColor3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "SparkleColor") {
				Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
				if (LuaColor3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Color3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaColor3->X);
					rbxlua_pushnumber(luaState, LuaColor3->Y);
					rbxlua_pushnumber(luaState, LuaColor3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "Tool") {
			if (Prop == "CanBeDropped") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Enabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "ManualActivationOnly") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "RequiresHandle") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "ToolTip") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "HopperBin") {
			if (Prop == "Activate") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BinType") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "Animation") {
			if (Prop == "AnimationId") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "ShirtGraphic") {
			if (Prop == "Graphic") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "Shirt") {
			if (Prop == "ShirtTemplate") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "Pants") {
			if (Prop == "PantsTemplate") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "CFrameValue") {
			if (Prop == "Value") {
				Types::CFrame* LuaCFrame3 = Types::CheckCFrame(L, 3); Sleep(10);
				if (LuaCFrame3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "CFrame");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaCFrame3->X);
					rbxlua_pushnumber(luaState, LuaCFrame3->Y);
					rbxlua_pushnumber(luaState, LuaCFrame3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "IntValue") {
			if (Prop == "Value") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "NumberValue") {
			if (Prop == "Value") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				//rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, "Value");
				setProp = true;
			}
		}
		else if (Class == "StringValue") {
			if (Prop == "Value") {
				const char* val = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, val);
				rbxlua_setfield(luaState, -2, "Value");
				setProp = true;
			}
		}
		else if (Class == "BoolValue") {
			if (Prop == "Value") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val);
				rbxlua_setfield(luaState, -2, "Value");
				setProp = true;
			}
		}
		else if (Class == "Vector3Value") {
			if (Prop == "Value") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "Humanoid") {
			if (Prop == "Health") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "JumpPower") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "MaxHealth" || Prop == "maxHealth") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "HealthDisplayDistance") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "MaxSlopeAngle") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "HipHeight") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "NameDisplayDistance") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "RagdollCriteria") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "WalkSpeed") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "AutoJumpEnabled") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, Class.c_str());
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "AutoRotate") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, Class.c_str());
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Jump") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, Class.c_str());
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "PlatformStand") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, Class.c_str());
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Sit") {
				int val = lua_tonumber(L, -1);
				rbxlua_getservice(luaState, "Players");
				rbxlua_getfield(luaState, -1, "LocalPlayer");
				rbxlua_getfield(luaState, -1, "Character");
				rbxlua_getfield(luaState, -1, Class.c_str());
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "CameraOffset") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "MoveDirection") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "TargetPoint") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "WalkToPoint") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "Player") {
			if (Prop == "CharacterAppearance") {
				const char* CharApp = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, CharApp);
				rbxlua_setfield(luaState, -2, "CharacterAppearance");
				setProp = true;
			}
			else if (Prop == "CanLoadCharacterAppearance") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, "CanLoadCharacterAppearance");
				setProp = true;
			}
			else if (Prop == "Character") {
				Inst* newChar = CheckInstance<Inst>(L, 3);
			//	SetCharacter(Ins, newChar->InstanceRef);
				setProp = true;
			}
		}
		else if (Class == "SpawnLocation") {
			if (Prop == "AllowTeamChangeOnTouch") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Enabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Neutral") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Duration") {
				int starCount = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(starCount).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "BodyAngularVelocity") {
			if (Prop == "P") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "AngularVelocity") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "MaxTorque" || Prop == "maxTorque") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "BodyForce") {
			if (Prop == "Force" || Prop == "force") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "BodyGyro") {
			if (Prop == "P") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "D") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "MaxTorque" || Prop == "maxTorque") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "CFrame" || Prop == "cframe") {
				Types::CFrame* LuaCFrame3 = Types::CheckCFrame(L, 3); Sleep(10);
				if (LuaCFrame3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "CFrame");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaCFrame3->X);
					rbxlua_pushnumber(luaState, LuaCFrame3->Y);
					rbxlua_pushnumber(luaState, LuaCFrame3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "BodyPosition") {
			if (Prop == "D") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "P") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "MaxForce" || Prop == "maxForce") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Position" || Prop == "position") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "BodyThrust") {
			if (Prop == "Force" || Prop == "force") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Location" || Prop == "location") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "BodyVelocity") {
			if (Prop == "P") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "MaxForce" || Prop == "maxForce") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Velocity" || Prop == "velocity") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "RocketPropulsion") {
			if (Prop == "CartoonFactor") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "MaxSpeed") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "MaxThrust") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "TargetRadius") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "ThrustD") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "ThrustP") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "TurnD") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "TurnP") {
				int val = luaL_checkinteger(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				//rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "TargetOffset") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "MaxTorque") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Target") {
				Inst* newInstance = CheckInstance<Inst>(L, 3);
				rbxWrapToLua(luaState, newInstance->InstanceRef);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushvalue(luaState, -3);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}

		else if (Class == "Sound") {
			if (Prop == "SoundId") {
				const char* Id = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, Id);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Volume") {
				int Volume = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(Volume).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
			}
			else if (Prop == "Pitch") {
				int Pitch = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(Pitch).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "MaxDistance") {
				int val = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "MinDistance") {
				int val = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(val).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Looped") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
		}
		else if (Class == "Explosion") {
			if (Prop == "ExplosionType") {
				const char* Id = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, Id);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BlastRadius") {
				int BlastRadius = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(BlastRadius).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BlastPressure") {
				int BlastPressure = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(BlastPressure).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "DestroyJointRadiusPercent") {
				int BlastPressure = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, std::to_string(BlastPressure).c_str());
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Position") {
				Types::Vector3* LuaVec3 = Types::CheckVector3(L, 3);
				if (LuaVec3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Vector3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaVec3->X);
					rbxlua_pushnumber(luaState, LuaVec3->Y);
					rbxlua_pushnumber(luaState, LuaVec3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "TextLabel" || Class == "TextBox" || Class == "TextButton" || Class == "Frame" || Class == "ImageLabel" || Class == "ScrollingFrame" || Class == "ImageButton") {
			if (Prop == "Active") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Visible") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "ClipsDescendants") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Draggable") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "Selectable") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "BackgroundTransparency") {
				double Trans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Trans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "BorderSizePixel") {
				double val = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "Rotation") {
				double val = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "Transparency") {
				double val = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "ZIndex") {
				double val = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "SizeConstraint") {
				const char* newText = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, newText);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "Size") {
				Types::UDim2* LuaUDim2 = Types::CheckUDim2(L, 3);
				if (LuaUDim2) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "UDim2");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaUDim2->A);
					rbxlua_pushnumber(luaState, LuaUDim2->B);
					rbxlua_pushnumber(luaState, LuaUDim2->C);
					rbxlua_pushnumber(luaState, LuaUDim2->D);
					rbxlua_pcall(luaState, 4, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Position") {
				Types::UDim2* LuaUDim2 = Types::CheckUDim2(L, 3);
				if (LuaUDim2) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "UDim2");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaUDim2->A);
					rbxlua_pushnumber(luaState, LuaUDim2->B);
					rbxlua_pushnumber(luaState, LuaUDim2->C);
					rbxlua_pushnumber(luaState, LuaUDim2->D);
					rbxlua_pcall(luaState, 4, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "BackgroundColor3") {
				Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
				if (LuaColor3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Color3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaColor3->X);
					rbxlua_pushnumber(luaState, LuaColor3->Y);
					rbxlua_pushnumber(luaState, LuaColor3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "BorderColor3") {
				Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
				if (LuaColor3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "Color3");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaColor3->X);
					rbxlua_pushnumber(luaState, LuaColor3->Y);
					rbxlua_pushnumber(luaState, LuaColor3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
				else {
					luaL_argerror(L, 3, "table expected");
				}
			}

			if (setProp != true) {
				if (Class == "TextLabel" || Class == "TextBox" || Class == "TextButton") {
					if (Prop == "Text") {
						const char* newText = luaL_checkstring(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushstring(luaState, newText);
						rbxlua_setfield(luaState, -2, Prop.c_str());
						setProp = true;
					}
					else if (Prop == "Font") {
						const char* newText = luaL_checkstring(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushstring(luaState, newText);
						rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
						setProp = true;
					}
					else if (Prop == "FontSize") {
						const char* newText = luaL_checkstring(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushstring(luaState, newText);
						rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
						setProp = true;
					}
					else if (Prop == "TextXAlignment") {
						const char* newText = luaL_checkstring(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushstring(luaState, newText);
						rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
						setProp = true;
					}
					else if (Prop == "TextYAlignment") {
						const char* newText = luaL_checkstring(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushstring(luaState, newText);
						rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
						setProp = true;
					}
					else if (Prop == "FontSize") {
						const char* newText = luaL_checkstring(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushstring(luaState, newText);
						rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
						setProp = true;
					}
					else if (Prop == "TextStrokeTranparency") {
						double val = luaL_checknumber(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushnumber(luaState, val);
						rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
						setProp = true;
					}
					else if (Prop == "TextTransparency") {
						double val = luaL_checknumber(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushnumber(luaState, val);
						rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
						setProp = true;
					}
					else if (Prop == "TextFits") {
						if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
						bool val = lua_toboolean(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushbool(luaState, val ? 1 : 0);
						rbxlua_setfield(luaState, -2, Prop.c_str());
						setProp = true;
					}
					else if (Prop == "TextScaled") {
						if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
						bool val = lua_toboolean(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushbool(luaState, val ? 1 : 0);
						rbxlua_setfield(luaState, -2, Prop.c_str());
						setProp = true;
					}
					else if (Prop == "TextWrapped") {
						if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
						bool val = lua_toboolean(L, 3);
						rbxWrapToLua(luaState, Ins);
						rbxlua_pushbool(luaState, val ? 1 : 0);
						rbxlua_setfield(luaState, -2, Prop.c_str());
						setProp = true;
					}
					else if (Prop == "TextColor3") {
						Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
						if (LuaColor3) {
							rbxWrapToLua(luaState, Ins);
							rbxlua_getglobal(luaState, "Color3");
							rbxlua_getfield(luaState, -1, "new");
							rbxlua_pushnumber(luaState, LuaColor3->X);
							rbxlua_pushnumber(luaState, LuaColor3->Y);
							rbxlua_pushnumber(luaState, LuaColor3->Z);
							rbxlua_pcall(luaState, 3, 1, 0);
							rbxlua_setfield(luaState, -3, Prop.c_str());
							setProp = true;
						}
						else {
							luaL_argerror(L, 3, "table expected");
						}
					}
					else if (Prop == "TextStrokeColor3") {
						Types::Color3* LuaColor3 = Types::CheckColor3(L, 3);
						if (LuaColor3) {
							rbxWrapToLua(luaState, Ins);
							rbxlua_getglobal(luaState, "Color3");
							rbxlua_getfield(luaState, -1, "new");
							rbxlua_pushnumber(luaState, LuaColor3->X);
							rbxlua_pushnumber(luaState, LuaColor3->Y);
							rbxlua_pushnumber(luaState, LuaColor3->Z);
							rbxlua_pcall(luaState, 3, 1, 0);
							rbxlua_setfield(luaState, -3, Prop.c_str());
							setProp = true;
						}
						else {
							luaL_argerror(L, 3, "table expected");
						}
					}
				}
			}
		}
		else if (Class == "HopperBin") {
			if (Prop == "BinType") {
				const char* newText = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, newText);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "Camera") {
			if (Prop == "CameraSubject") {
				Inst* newSubject = CheckInstance<Inst>(L, 3);
				SetCameraSubject(Ins, newSubject->InstanceRef);
				setProp = true;
			}
			else if (Prop == "HeadLocked") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val ? 1 : 0);
				rbxlua_setfield(luaState, -2, Prop.c_str());
				setProp = true;
			}
			else if (Prop == "FieldOfView") {
				double Trans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Trans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "HeadScale") {
				double Trans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Trans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "CFrame") {
				Types::CFrame* LuaCFrame3 = Types::CheckCFrame(L, 3); Sleep(10);
				if (LuaCFrame3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "CFrame");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaCFrame3->X);
					rbxlua_pushnumber(luaState, LuaCFrame3->Y);
					rbxlua_pushnumber(luaState, LuaCFrame3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Focus" || Prop == "focus") {
				Types::CFrame* LuaCFrame3 = Types::CheckCFrame(L, 3); Sleep(10);
				if (LuaCFrame3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "CFrame");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaCFrame3->X);
					rbxlua_pushnumber(luaState, LuaCFrame3->Y);
					rbxlua_pushnumber(luaState, LuaCFrame3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "CoordinateFrame") {
				Types::CFrame* LuaCFrame3 = Types::CheckCFrame(L, 3); Sleep(10);
				if (LuaCFrame3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "CFrame");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaCFrame3->X);
					rbxlua_pushnumber(luaState, LuaCFrame3->Y);
					rbxlua_pushnumber(luaState, LuaCFrame3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
		}
		else if (Class == "Weld") {
			if (Prop == "C0") {
				Types::CFrame* LuaCFrame3 = Types::CheckCFrame(L, 3); Sleep(10);
				if (LuaCFrame3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "CFrame");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaCFrame3->X);
					rbxlua_pushnumber(luaState, LuaCFrame3->Y);
					rbxlua_pushnumber(luaState, LuaCFrame3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "C1") {
				Types::CFrame* LuaCFrame3 = Types::CheckCFrame(L, 3); Sleep(10);
				if (LuaCFrame3) {
					rbxWrapToLua(luaState, Ins);
					rbxlua_getglobal(luaState, "CFrame");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushnumber(luaState, LuaCFrame3->X);
					rbxlua_pushnumber(luaState, LuaCFrame3->Y);
					rbxlua_pushnumber(luaState, LuaCFrame3->Z);
					rbxlua_pcall(luaState, 3, 1, 0);
					rbxlua_setfield(luaState, -3, Prop.c_str());
					setProp = true;
				}
			}
			else if (Prop == "Part0") {
				Inst* newInstance = CheckInstance<Inst>(L, 3);
				if (newInstance) {
					rbxWrapToLua(luaState, newInstance->InstanceRef); Sleep(10);
					rbxWrapToLua(luaState, Ins);
					rbxlua_pushvalue(luaState, -4);
					rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
					setProp = true;
				}
			}
			else if (Prop == "Part1") {
				Inst* newInstance = CheckInstance<Inst>(L, 3);
				if (newInstance) {
					rbxWrapToLua(luaState, newInstance->InstanceRef); Sleep(10);
					rbxWrapToLua(luaState, Ins);
					rbxlua_pushvalue(luaState, -4);
					rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
					setProp = true;
				}
			}
		}
		else if (Class == "SurfaceSelection") {
			if (Prop == "Adornee") {
				Inst* newInstance = CheckInstance<Inst>(L, 3);
				rbxWrapToLua(luaState, newInstance->InstanceRef);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushvalue(luaState, -3);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "SelectionBox") {
			if (Prop == "Adornee") {
				Inst* newInstance = CheckInstance<Inst>(L, 3);
				rbxWrapToLua(luaState, newInstance->InstanceRef);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushvalue(luaState, -3);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "LineThickness") {
				double Trans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Trans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "SurfaceTransparency") {
				double Trans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Trans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "SelectionSphere") {
			if (Prop == "Adornee") {
				Inst* newInstance = CheckInstance<Inst>(L, 3);
				rbxWrapToLua(luaState, newInstance->InstanceRef);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushvalue(luaState, -3);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "SurfaceTransparency") {
				double Trans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Trans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "BillboardGui") {
			if (Prop == "Adornee") {
				Inst* newInstance = CheckInstance<Inst>(L, 3);
				rbxWrapToLua(luaState, newInstance->InstanceRef);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushvalue(luaState, -3);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "SurfaceGui") {
			if (Prop == "Adornee") {
				Inst* newInstance = CheckInstance<Inst>(L, 3);
				rbxWrapToLua(luaState, newInstance->InstanceRef);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushvalue(luaState, -3);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "Hint" || Class == "Message") {
			if (Prop == "Text") {
				const char* newText = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, newText);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "Decal") {
			if (Prop == "Texture") {
				const char* newText = luaL_checkstring(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushstring(luaState, newText);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "Transparency") {
				double Trans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Trans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "LocalTransparencyModifier") {
				double LocalTrans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, LocalTrans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "Texture") {
			if (Prop == "StudsPerTileU") {
				double Trans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, Trans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
			else if (Prop == "StudsPerTileV") {
				double LocalTrans = luaL_checknumber(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushnumber(luaState, LocalTrans);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "LocalScript") {
			if (Prop == "Disabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}
		else if (Class == "ScriptContext") {
			if (Prop == "ScriptsDisabled") {
				if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
				bool val = lua_toboolean(L, 3);
				rbxWrapToLua(luaState, Ins);
				rbxlua_pushbool(luaState, val);
				rbxlua_setfield(luaState, -2, Prop.c_str()); Sleep(10);
				setProp = true;
			}
		}

		//props that work on any class

		if (Prop == "Name") {
			const char* newName = luaL_checkstring(L, 3);
			//int* t = (int*)rbxlua_newuserdata(luaState, 8);
			//*t = Ins;
			rbxWrapToLua(luaState, Ins);
			rbxlua_pushstring(luaState, newName);
			rbxlua_setfield(luaState, -2, "Name"); Sleep(1);
			setProp = true;
		}
		else if (Prop == "Parent") {
			Inst* newParent = CheckInstance<Inst>(L, 3);
			//rbxWrapToLua(luaState, newParent->InstanceRef);
			//rbxlua_setfield(luaState, -3, "Parent"); Sleep(1);
			setParent(Ins, newParent->InstanceRef); Sleep(1);
			setProp = true;
		}
		else if (Prop == "Archivable" || Prop == "archivable") {
			if (!lua_isboolean(L, 3)) luaL_argerror(L, 3, "boolean expected");
			bool val = lua_toboolean(L, 3);
			rbxWrapToLua(luaState, Ins);
			rbxlua_pushbool(luaState, val ? 1 : 0);
			rbxlua_setfield(luaState, -2, "Archivable"); Sleep(1);
			setProp = true;
		}
		Sleep(10);
		if (setProp != true) {
		//	luaL_error(L, "%s is not a valid member of %s", Prop, GetName(Ins).c_str());
		}
	}
	return 0;
}

static int luaEquals(lua_State* L) {
	Inst* Inst1 = CheckInstance<Inst>(L, 1);
	Inst* Inst2 = CheckInstance<Inst>(L, 2);
	if (Inst1 != nullptr && Inst2 != nullptr) {
		lua_pushboolean(L, Inst1->InstanceRef == Inst2->InstanceRef);
		return 1;
	}
	lua_pushboolean(L, 0);
	return 1;
}

static int CreateGlobalMetatable(lua_State* L) {
	luaL_newmetatable(L, "ObjectMetatable");
	lua_pushstring(L, "This metatable is locked.");
	lua_setfield(L, -2, "__metatable");
//	lua_pushcfunction(L, luaToString);
//	lua_setfield(L, -2, "__tostring");
	lua_pushcfunction(L, luaNewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, luaIndex);
	lua_setfield(L, -2, "__index");
	return 0;
}

static int InstanceNew(lua_State* L) {
	int newParent = 0;
	int indexMax = -1;
	int Ins;
	const char* Class = luaL_checkstring(L, 1);
	if (lua_gettop(L) == 2) {
		Inst* newPar = CheckInstance<Inst>(L, 2);
		rbxlua_getglobal(luaState, "Instance");
		rbxlua_getfield(luaState, -1, "new");
		rbxlua_pushstring(luaState, Class);
		//rbxlua_pushvalue(luaState, -4);
		rbxlua_pcall(luaState, 1, 1, 0); Sleep(2);

		if (!rbxlua_isnil(luaState, -1)) {
			Ins = rbxlua_touserdata(luaState, -1);
			if (Ins != 0) {
				setParent(Ins, newPar->InstanceRef);
			}
			/*rbxWrapToLua(luaState, newPar->InstanceRef);
			if (!rbxlua_isnil(luaState, -1)) {
			//rbxlua_pushvalue(luaState, -1);
			rbxlua_setfield(luaState, -3, "Parent"); Sleep(20);
			//rbxlua_pcall(luaState, 0, 0, 0);
			}
			else if (!rbxlua_isnil(luaState, -2)) {
			rbxlua_setfield(luaState, -4, "Parent"); Sleep(20);
			}*/
		}
		indexMax = -2;
	}
	else {
		rbxlua_getglobal(luaState, "Instance");
		rbxlua_getfield(luaState, -1, "new");
		rbxlua_pushstring(luaState, Class);
		//rbxlua_pushvalue(luaState, -4);
		rbxlua_pcall(luaState, 1, 1, 0);
		if (!rbxlua_isnil(luaState, -1)) {
			Ins = rbxlua_touserdata(luaState, -1);
		}
	}
	if (!rbxlua_isnil(luaState, indexMax)) {
		if (Ins != 0) {
			WrapToLua(L, Ins);
			lua_pushvalue(L, -1);
		}
	}
	return 1;
}

static int Vector3New(lua_State* L) {
	double X = luaL_checknumber(L, 1);
	double Y = luaL_checknumber(L, 2);
	double Z = luaL_checknumber(L, 3);

	Types::Vector3* LuaVec3 = new Types::Vector3(X, Y, Z);
	LuaVec3->WrapLua(L);
	return 1;
}

static int Color3New(lua_State* L) {
	double X = luaL_checknumber(L, 1);
	double Y = luaL_checknumber(L, 2);
	double Z = luaL_checknumber(L, 3);

	Types::Color3* LuaColor3 = new Types::Color3(X, Y, Z);
	LuaColor3->WrapLua(L);
	return 1;
}

static int CFrameNew(lua_State* L) {
	double X = luaL_checknumber(L, 1);
	double Y = luaL_checknumber(L, 2);
	double Z = luaL_checknumber(L, 3);

	Types::CFrame* LuaColor3 = new Types::CFrame(X, Y, Z);
	LuaColor3->WrapLua(L);
	return 1;
}

static int UDim2New(lua_State* L) {
	double A = luaL_checknumber(L, 1);
	double B = luaL_checknumber(L, 2);
	double C = luaL_checknumber(L, 3);
	double D = luaL_checknumber(L, 4);

	Types::UDim2* LuaUDim2 = new Types::UDim2(A, B, C, D);
	LuaUDim2->WrapLua(L);
	return 1;
}

static int NumberRangeNew(lua_State* L) {
	double A = luaL_checknumber(L, 1);
	double B = luaL_checknumber(L, 2);

	Types::NumberRange* LuaNumberRange = new Types::NumberRange(A, B);
	LuaNumberRange->WrapLua(L);
	return 1;
}

static int BrickColorNew(lua_State* L) {
	const char* A = luaL_checkstring(L, 1);
	//double A = luaL_checknumber(L, 1);
	//double B = luaL_checknumber(L, 2);

	rbxlua_getglobal(luaState, "BrickColor");
	rbxlua_getfield(luaState, -1, "new");
	rbxlua_pushstring(luaState, A);
	rbxlua_pcall(luaState, 1, 1, 0);
	//Types::BrickColor* LuaBrickColor = new Types::BrickColor(A);
	//LuaBrickColor->WrapLua(L);
	return 1;
}


static int GetItem(int Instance) {
	DWORD StartOfChildren = (DWORD)(Instance + 0x2C);
	DWORD EndOfChildren = (DWORD)(StartOfChildren + 4);
	for (int i = (int)StartOfChildren; i != EndOfChildren; i += 8) {
		return (int)i;
	}
	return 0;
}


static int ReplaceInsertServiceItems(int Instance, const char* Name) {
	DWORD StartOfChildren = (DWORD)(Instance + 0x2C);
	DWORD EndOfChildren = (DWORD)(StartOfChildren + 4);
	for (int i = (int)StartOfChildren; i != EndOfChildren; i += 8) {
		if (memcmp(GetName((int)i), Name, strlen(Name)) == 0) {

			rbxlua_getfield(luaState, -10002, "game");
			rbxlua_getfield(luaState, -1, "InsertService");
			rbxlua_getfield(luaState, -1, "Folder");
			rbxlua_pushstring(luaState, "Xero");
			rbxlua_setfield(luaState, -2, "Name");
			rbxlua_pcall(luaState, 1, 0, 0);
			return (int)i;
		}
	}
	return 0;
}

static void LoadAsset(int AssetId) {
	std::string Lp = *GetName(GetLocalPlayer(Players));

	int InsertService = FindFirstClass(DataModel, "InsertService");
	ReplaceInsertServiceItems(InsertService, "Folder");
	Sleep(500);
	rbxlua_getfield(luaState, -10002, "game");
	rbxlua_getfield(luaState, -1, "InsertService");
	rbxlua_getfield(luaState, -1, "LoadAsset");
	rbxlua_pushvalue(luaState, -2);
	rbxlua_pushnumber(luaState, AssetId);
	rbxlua_pcall(luaState, 2, 0, 0);

	int ParentLocation;

	if (MessageBoxA(0, "Would you like this item to be inside your Backpack\n[Yes] = Backpack(Tool)\n[No] = Character(Hat)", "Parent", MB_YESNO) == IDYES) {
		int Player = GetLocalPlayer(Players);
		ParentLocation = FindFirstChild(Player, "Backpack");
	}
	else {
		std::string LocalPlayer = *GetName(GetLocalPlayer(Players));
		ParentLocation = FindFirstChild(Workspace, LocalPlayer);
	}

	int Folder = FindFirstChild(InsertService, "Folder");
	Sleep(500);
	int Model = FindFirstClass(Folder, "Model");
	Sleep(500);
	int Instance = GetItem(Model);
	Sleep(1000);
	*(int*)(Instance + 0x34) = ParentLocation;
}

static int LoadAssetB(lua_State *L) {

	int ArgCount = 0;
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		switch (lua_type(L, x * -1)) {

		default:
		{
			const char* String = lua_tostring(L, x * -1);
			int AssetId = atoi(String);
			LoadAsset(AssetId);
			break;
		}
		}
		ArgCount += 1;
	}
	return 0;
}

static lua_State* SetupLuaState(lua_State* L) {

	luaL_openlibs(L);

	lua_pushcfunction(L, LuaConsolePrint);
	lua_setglobal(L, "printconsole");

	lua_pushcfunction(L, LuaPrint);
	lua_setglobal(L, "print");

	lua_pushcfunction(L, LuaWarn);
	lua_setglobal(L, "warn");

	lua_pushcfunction(L, LuaError);
	lua_setglobal(L, "error");

	//lua_pushcfunction(L, LuaExecuteCommand);
	//lua_setglobal(L, "execute");

	lua_pushcfunction(L, luaWait);
	lua_setfield(L, -10002, "wait");

	lua_pushcfunction(L, luaSpawn);
	lua_setfield(L, -10002, "spawn");
	lua_getfield(L, -10002, "spawn");
	lua_setfield(L, -10002, "Spawn");

	lua_pushcfunction(L, luaDelay);
	lua_setfield(L, -10002, "delay");
	lua_getfield(L, -10002, "delay");
	lua_setfield(L, -10002, "Delay");

	//lua_pushcfunction(L, LuaRbxTime);
	//lua_setglobal(L, "time");

	lua_pushcfunction(L, LuaCurrentEpoch);
	lua_setglobal(L, "tick");

	//

	//

	//lua_newtable(L);
	//
	//lua_pushcfunction(L, XYZHttpGet);
	//lua_setfield(L, -2, "get");
	//
	//lua_setglobal(L, "Http");

	//

	lua_newtable(L);

	lua_pushcfunction(L, InstanceNew);
	lua_setfield(L, -2, "new");

	lua_setglobal(L, "Instance");

	//

	//lua_pushnil(L);
	//lua_setglobal(L, "debug");

	CreateGlobalMetatable(L);
	WrapToLua(L, DataModel);
	lua_setglobal(L, "Game");
	lua_getfield(L, -10002, "Game");
	lua_setglobal(L, "game");
	WrapToLua(L, Workspace);
	lua_setglobal(L, "Workspace");
	lua_getfield(L, -10002, "Workspace");
	lua_setglobal(L, "workspace");

	lua_pushcfunction(L, LoadAssetB);
	lua_setglobal(L, "LoadAsset");

	Types::Vector3::RegisterLua(L);

	lua_newtable(L);

	lua_pushcfunction(L, Vector3New);
	lua_setfield(L, -2, "new");

	lua_setglobal(L, "Vector3");

	//

	Types::Color3::RegisterLua(L);

	lua_newtable(L);

	lua_pushcfunction(L, Color3New);
	lua_setfield(L, -2, "new");

	lua_setglobal(L, "Color3");

	//

	Types::UDim2::RegisterLua(L);

	lua_newtable(L);

	lua_pushcfunction(L, UDim2New);
	lua_setfield(L, -2, "new");

	lua_setglobal(L, "UDim2");

	//

	Types::NumberRange::RegisterLua(L);

	lua_newtable(L);

	lua_pushcfunction(L, NumberRangeNew);
	lua_setfield(L, -2, "new");

	lua_setglobal(L, "NumberRange");

	//

	Types::CFrame::RegisterLua(L);

	lua_newtable(L);

	lua_pushcfunction(L, CFrameNew);
	lua_setfield(L, -2, "new");

	lua_setglobal(L, "CFrame");

	return L;
}

static int WrapToLua(lua_State* L, int InstRef) {
	Inst* InstC;
	if (InstanceMap.find(InstRef) != InstanceMap.end()) {
		InstC = InstanceMap[InstRef];
	}
	else {
		InstC = new Inst();
		InstC->InstanceRef = InstRef;
		InstanceMap[InstRef] = InstC;
	}

	Inst** userdata = (Inst**)lua_newuserdata(L, sizeof(Inst));
	*userdata = InstC;
	luaL_getmetatable(L, "ObjectMetatable");
	lua_setmetatable(L, -2);
	return 1;
}
//game.StarterGui:SetCoreGuiEnabled("All",false)
static int rbxWrapToLua(DWORD luaState, int InstRef) {
	int* t = (int*)rbxlua_newuserdata(luaState, 8);
	*t = InstRef;
	rbxlua_getfield(luaState, rbxlua_REGISTRYINDEX, "Object");
	if (!rbxlua_isnil(luaState, -1) && !rbxlua_isnil(luaState, -2)) {
		rbxlua_setmetatable(luaState, -2);
		if (!rbxlua_isnil(luaState, -1)) {
			rbxlua_pushvalue(luaState, -1);
		}
		else {
			rbxlua_pushnil(luaState);
		}
	}
	else {
		rbxlua_pushnil(luaState);
	}
	return 1;
}

static void LuaExecuteString(std::string Source) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	SetupLuaState(L);
	if (luaL_dostring(L, Source.c_str()))
	{
		MessageBoxA(0, lua_tostring(L, -1), 0, 0);
	}
}

#define toproto(L,i) (clvalue(L->top+(i))->l.p)

static Proto* combine(lua_State* L, int n)
{
	if (n == 1)
		return toproto(L, -1);
	else
	{
		int i, pc;
		Proto* f = luaF_newproto(L);
		setptvalue2s(L, L->top, f); incr_top(L);
		//f->source = luaS_newliteral(L, "=(" PROGNAME ")");
		f->maxstacksize = 1;
		pc = 2 * n + 1;
		f->code = luaM_newvector(L, pc, Instruction);
		f->sizecode = pc;
		f->p = luaM_newvector(L, n, Proto*);
		f->sizep = n;
		pc = 0;
		for (i = 0; i<n; i++)
		{
			f->p[i] = toproto(L, i - n - 1);
			f->code[pc++] = CREATE_ABx(OP_CLOSURE, 0, i);
			f->code[pc++] = CREATE_ABC(OP_CALL, 0, 1, 1);
		}
		f->code[pc++] = CREATE_ABC(OP_RETURN, 0, 1, 0);
		return f;
	}
}

static lua_Writer kpt_lua_Writer(lua_State* L)
{
	SetupLuaState(L);
	return 0;
}

static int kpt_lua_Writer(lua_State *L, const void *p, size_t sz, void *u)
{
	SetupLuaState(L);
	return (fwrite(p, sz, 1, (FILE*)u) != 1) && (sz != 0);

}


static const char* buildLua(std::string luaScript)
{
	const Proto* f;
	char *byteCode = 0L;
	size_t byteCodeLen = 0;
	FILE* wd = NULL;
	const char* cs;
	lua_State *L = luaL_newstate();
	f = combine(L, 0);
	luaL_loadstring(L, luaScript.c_str());
	luaL_openlibs(L);
	lua_lock(L);
	lua_dump(L, kpt_lua_Writer, 0);
	lua_unlock(L);
	lua_close(L);
	cs = byteCode;
	return cs;
}

#define SETBITS(x,m)	((x) |= (m))

static int ConvertIntrustion(Instruction i)
{
	OpCode op = GET_OPCODE(i);
	int instruction = i;
	switch (getOpMode(i))
	{
	case iABC: {
		SETBITS(instruction, GETARG_A(i), 8, 18);
		SETBITS(instruction, GETARG_B(i), 9, 0);
		SETBITS(instruction, GETARG_C(i), 9, 9);
		break;
	}
	case iABx: {
		SETBITS(instruction, GETARG_A(i), 8, 18);
		SETBITS(instruction, GETARG_Bx(i), 18, 0);
		break;
	}
	case iAsBx: {
		SETBITS(instruction, GETARG_A(i), 8, 18);
		SETBITS(instruction, GETARG_sBx(i) + MAXARG_sBx, 18, 0);
		break;
	}
	}
	return instruction;
}

typedef struct {
	size_t *len;
	char **data;
} BS_DESCRIP;

static int scriptMemoryWriter_(lua_State* ls, const void* p, size_t sz, void* ud)
{
	BS_DESCRIP* bd = (BS_DESCRIP*)ud;
	char* newData = (char*)realloc(*(bd->data), (*(bd->len)) + sz);
	SetupLuaState(ls);
	if (newData)
	{
		memcpy(newData + (*(bd->len)), p, sz);
		*(bd->data) = newData;
		*(bd->len) += sz;

	}
	else {
		free(newData);

		return 1;
	}

	return 0;
}


static std::string DumpToString(void * data_, size_t count);

typedef struct TStreamBuffer
{
	const char * src;
	size_t sze;
	size_t ind;
	char buff[LUAL_BUFFERSIZE];
} TStreamBuffer;

typedef struct TCompiledBuffer
{
	char * ptr;
	size_t sze;
	int result;
} TCompiledBuffer;

static int MyCustomWriter(lua_State * L, const void * src, size_t sze, void * userdata)
{
	TCompiledBuffer * cb = (TCompiledBuffer *)userdata;
	UNUSED(L);
	cb->ptr = (char *)realloc(cb->ptr, cb->sze + sze);
	if (cb->ptr == NULL)
	{
		return 1;
	}
	memcpy(cb->ptr + cb->sze, src, sze);
	cb->sze += sze;
	return 0;
}

static const char * MyBufferStreamer(lua_State * L, void * userdata, size_t * sze)
{
	TStreamBuffer * sb = (TStreamBuffer *)userdata;
	*sze = sizeof(sb->buff);
	UNUSED(L);
	memset(sb->buff, 0, sizeof(sb->buff));
	if (sb->ind >= sb->sze)
	{
		*sze = 0;
		return NULL;
	}
	if (sb->sze - sb->ind < *sze)
	{
		*sze = sb->sze - sb->ind;
	}
	memcpy(sb->buff, sb->src + sb->ind, *sze);
	sb->ind += *sze;
	return sb->buff;
}

static TCompiledBuffer CompileString(lua_State * L, const char * input)
{
	const Proto * proto = 0;
	TCompiledBuffer cb = { 0 };
	TStreamBuffer sb = { 0 };
	sb.sze = strlen(input);
	sb.src = input;
	cb.result = lua_load(L, MyBufferStreamer, &sb, "");
	SetupLuaState(L);
	if (cb.result != 0)
	{
		return cb;
	}
	lua_lock(L);
	proto = clvalue(L->top + (-1))->l.p;
	cb.result = luaU_dump(L, proto, MyCustomWriter, &cb, 1);
	lua_unlock(L);
	if (cb.result != 0)
	{
		cb.sze = 0;
		if (cb.ptr)
		{
			free(cb.ptr);
			cb.ptr = 0;
		}
	}
	return cb;
}


// Formats a stream of data into a hex/ascii dump
static std::string DumpToString(void * data_, size_t count)
{
	char msg[256] = { 0 };
	std::stringstream smsg;
	int sze = count;
	int outputsize = sze;
	if (outputsize % 16 != 0) outputsize += (16 - outputsize % 16);
	if (outputsize == 0) outputsize = 16;
	int ctr = 0;
	char ch[17] = { 0 };
	int x1 = 0;
	unsigned char * data = reinterpret_cast<unsigned char *>(data_);
	for (int x = 0; x < outputsize; ++x)
	{
		if (x < sze)
		{
			unsigned char b = data[x];
			sprintf_s(msg + ctr, sizeof(msg) - ctr, "%.2X ", b);
			ch[x1] = (isprint(b) && !isspace(b) ? b : '.');
		}
		else
		{
			sprintf_s(msg + ctr, sizeof(msg) - ctr, "   ");
			ch[x1] = '.';
		}
		x1++;
		ctr += 3;
		if ((x + 1) % 16 == 0)
		{
			x1 = 0;
			sprintf_s(msg + ctr, sizeof(msg) - ctr, "  %s", ch);
			ctr += 18;
			sprintf_s(msg + ctr, sizeof(msg) - ctr, "\n");
			try
			{
				smsg << msg;
			}
			catch (std::exception & e)
			{
				printf("[%s] %s\n", __FUNCTION__, e.what());
				return "";
			}
			ctr = 0;
			memset(msg, 0, sizeof(msg));
		}
	}
	return smsg.str();
}

static DWORD rbxlua_State;

static void PushInstance(lua_State *L, DWORD addy) {
	lua_newtable(L);
	lua_pushnumber(L, addy);
	lua_setfield(L, -2, "__address");

	lua_getglobal(L, "robject");
	lua_setmetatable(L, -2);
}

static int CreateInstance(lua_State *L) {
	int Args = lua_gettop(L);
	if (Args == 1) {
		const char* Class = lua_tostring(L, -1);
		rbxlua_pushstring(rbxlua_State, Class);
		//InstanceNew(rbxlua_State);
		int u = (int)rbxlua_touserdata(rbxlua_State, -1);
		PushInstance(L, u);
		return 1;
	}
	else if (Args == 2) {
		lua_getfield(L, -1, "__address");
		int AddressLoc = lua_tonumber(L, -1);

		const char* Class = lua_tostring(L, -3);
		rbxlua_pushstring(rbxlua_State, Class);

		int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
		*userdata = AddressLoc;
		*(userdata + 1) = 0;
		rbxlua_getfield(rbxlua_State, -10000, "Object");
		rbxlua_setmetatable(rbxlua_State, -2);

		//InstanceNew(rbxlua_State);

		int u = (int)rbxlua_touserdata(rbxlua_State, -1);
		PushInstance(L, u);
		return 1;
	}
	else {
		return 0;
	}
}



static int Print(lua_State *L) {
	//MessageBoxA(0, "Checking for print...", "", 0);
	rbxlua_getfield(rbxlua_State, -10002, "print");
	int ArgCount = 0;
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		switch (lua_type(L, x * -1)) {
		case LUA_TNIL: {
			//rbxlua_pushnil(rbxlua_State);
			break;
		}
		case LUA_TNUMBER: {
			double no = lua_tonumber(L, x * -1);
			//rbxlua_pushnumber(rbxlua_State, no);
			break;
		}
		case LUA_TSTRING: {
			const char* str = lua_tostring(L, x * -1);
			rbxlua_pushstring(rbxlua_State, str);
			break;
		}
		case LUA_TTABLE: {
			lua_getfield(L, -1, "__address");
			if (!lua_isnoneornil(L, -1)) {
				//int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
				//*userdata = lua_tonumber(L, x * -1);
				//*(userdata + 1) = 0;
				rbxlua_getfield(rbxlua_State, -10000, "Object");
				//rbxlua_setmetatable(rbxlua_State, -2);
			}
			else {
				lua_pop(L, 1);
				rbxlua_pushstring(rbxlua_State, "table: #NULL");
			}
			break;
		}
		default:
		{
			const char* str = lua_tostring(L, x * -1);
			rbxlua_pushstring(rbxlua_State, str);
			break;
		}
		}
		ArgCount += 1;
	}
	rbxlua_pcall(rbxlua_State, ArgCount, 0, 0);
	return 0;
	//MessageBoxA(0, "OK", "", 0);
}

static int Warn(lua_State *L) {
	rbxlua_getfield(rbxlua_State, -10002, "warn");
	int ArgCount = 0;
	for (int x = lua_gettop(L); x != 0; x -= 1) {
		switch (lua_type(L, x * -1)) {
		case LUA_TNIL: {
			//rbxlua_pushnil(rbxlua_State);
			break;
		}
		case LUA_TNUMBER: {
			double no = lua_tonumber(L, x * -1);
			rbxlua_pushnumber(rbxlua_State, no);
			break;
		}
		case LUA_TSTRING: {
			const char* str = lua_tostring(L, x * -1);
			rbxlua_pushstring(rbxlua_State, str);
			break;
		}
		case LUA_TTABLE: {
			lua_getfield(L, -1, "__address");
			if (!lua_isnoneornil(L, -1)) {
				int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
				*userdata = lua_tonumber(L, x * -1);
				*(userdata + 1) = 0;
				rbxlua_getfield(rbxlua_State, -10000, "Object");
				rbxlua_setmetatable(rbxlua_State, -2);
			}
			else {
				lua_pop(L, 1);
				rbxlua_pushstring(rbxlua_State, "table: #NULL");
			}
			break;
		}
		default:
		{
			const char* str = lua_tostring(L, x * -1);
			rbxlua_pushstring(rbxlua_State, str);
			break;
		}
		}
		ArgCount += 1;
	}
	rbxlua_pcall(rbxlua_State, ArgCount, 0, 0);
	return 0;
}

static int PrintIdentity(lua_State *L) {
	rbxlua_getfield(rbxlua_State, -10002, "printidentity");
	int ArgCount = 0;
	if (lua_gettop(L) != 0) {
		for (int x = lua_gettop(L); x != 0; x -= 1) {
			switch (lua_type(L, x * -1)) {
			case LUA_TNIL: {
				//rbxlua_pushnil(rbxlua_State);
				break;
			}
			case LUA_TNUMBER: {
				double no = lua_tonumber(L, x * -1);
				rbxlua_pushnumber(rbxlua_State, no);
				break;
			}
			case LUA_TSTRING: {
				const char* str = lua_tostring(L, x * -1);
				rbxlua_pushstring(rbxlua_State, str);
				break;
			}
			case LUA_TTABLE: {
				lua_getfield(L, -1, "__address");
				if (!lua_isnoneornil(L, -1)) {
					int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
					*userdata = lua_tonumber(L, x * -1);
					*(userdata + 1) = 0;
					rbxlua_getfield(rbxlua_State, -10000, "Object");
					rbxlua_setmetatable(rbxlua_State, -2);
				}
				else {
					lua_pop(L, 1);
					rbxlua_pushstring(rbxlua_State, "table: #NULL");
				}
				break;
			}
			default:
			{
				const char* str = lua_tostring(L, x * -1);
				rbxlua_pushstring(rbxlua_State, str);
				break;
			}
			}
			ArgCount += 1;
		}
	}
	rbxlua_pcall(rbxlua_State, ArgCount, 0, 0);
	return 0;
}

static int GetChildren(lua_State* L) {
	lua_getfield(L, -1, "__address");
	int Instance = lua_tonumber(L, -1);
	lua_pop(L, 2);
	lua_newtable(L);
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		PushInstance(L, *(int*)i);
		lua_setfield(L, -2, std::to_string(i).c_str());
	}
	return 1;
}

static int LuaFindFirstChild(lua_State* L) {
	const char* Name = lua_tostring(L, -1);
	lua_pop(L, 1);
	lua_getfield(L, -1, "__address");
	int rInstance = lua_tonumber(L, -1);
	lua_pop(L, 2);
	int Instance = FindFirstChild(rInstance, Name);
	if (Instance != 0) {
		PushInstance(L, Instance);
		return 1;
	}
	else {
		return 0;
	}
}

static int GetService(lua_State* L) {
	lua_getfield(L, -1, "__address");
	int Instance = lua_tonumber(L, -1);
	lua_pop(L, 2);
	lua_newtable(L);
	DWORD StartOfChildren = *(DWORD*)(DataModel + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
	{
		PushInstance(L, *(int*)i);
		lua_setfield(L, -2, std::to_string(i).c_str());
	}
	return 1;
}

static int CallMethod(lua_State* L) {
	int ArgCount, fArgs = 0;
	try {
		for (int x = lua_gettop(L); x != 0; x -= 1) {
			switch (lua_type(L, x * -1)) {
			case LUA_TNIL: {
				//rbxlua_pushnil(rbxlua_State);
				break;
			}
			case LUA_TNUMBER: {
				double no = lua_tonumber(L, x * -1);
				rbxlua_pushnumber(rbxlua_State, no);
				break;
			}
			case LUA_TSTRING: {
				const char* str = lua_tostring(L, x * -1);
				rbxlua_pushstring(rbxlua_State, str);
				break;
			}
			case LUA_TTABLE: {
				lua_getfield(L, -1, "__address");
				if (!lua_isnoneornil(L, -1)) {
					int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
					*userdata = lua_tonumber(L, x * -1);
					*(userdata + 1) = 0;
					rbxlua_getfield(rbxlua_State, -10000, "Object");
					rbxlua_setmetatable(rbxlua_State, -2);
					lua_pop(L, 1);
				}
				else {
					lua_pop(L, 1);
					rbxlua_pushstring(rbxlua_State, "table: #NULL");
				}
				break;
			}
			case LUA_TFUNCTION: {
				break;
			}
			default:
			{
				const char* str = lua_tostring(L, x * -1);
				rbxlua_pushstring(rbxlua_State, str);
				break;
			}
			}
			ArgCount += 1;
		}
		rbxlua_pcall(rbxlua_State, ArgCount, LUA_MULTRET, 0);
		for (int x = lua_gettop(L); x != 0; x -= 1) {
			switch (lua_type(L, x * -1)) {
			case LUA_TNIL: {
				lua_pushnil(L);
				break;
			}
						   /*
						   case LUA_TNUMBER: {
						   const char* str = rbxlua_tolstring(rbxlua_State, x * -1, NULL);
						   lua_pushnumber(L, ::atof(str));
						   break;
						   }
						   case LUA_TBOOLEAN: {
						   bool _bool = rbxlua_toboolean(rbxlua_State, x * -1);
						   lua_pushboolean(L, _bool);
						   break;
						   }
						   case LUA_TSTRING: {
						   const char* str = rbxlua_tolstring(rbxlua_State, x * -1, NULL);
						   lua_pushstring(L, str);
						   break;
						   }
						   case LUA_TUSERDATA: {
						   int u = (int)rbxlua_touserdata(rbxlua_State, -1);
						   PushInstance(L, u);
						   break;
						   }
						   }
						   fArgs += 1;
						   }
						   */
			}
		}
	}
	catch (std::exception e) {
		std::string err = "C ERROR: ";
		err += e.what();
		std::cout << err.c_str() << std::endl;
	}
	return fArgs;
}

static int Index(lua_State* L) {

	const char* Key = lua_tostring(L, -2);
	lua_pop(L, 2);
	int inst = lua_tonumber(L, 4);
	lua_pop(L, 4);
	int* userdata = (int*)lua_newuserdata(L, 0);
	userdata = (int*)inst;
	*(userdata + 0) = 1;



	//rbxlua_setmetatable(rbxlua_State, -2);

	if (Key == "GetChildren" || Key == "getChildren" || Key == "children") {
		lua_pushcfunction(L, GetChildren);
		return 1;
	}
	else if (Key == "GetService") {
		lua_pushcfunction(L, GetService);
		return 1;
	}
	else if (Key == "FindFirstChild" || Key == "findFirstChild") {
		lua_pushcfunction(L, LuaFindFirstChild);
		return 1;
	}
	else if (Key == "Parent") {

		return 1;
	}
}

static int NewIndex(lua_State* L) {
	MessageBoxA(0, "checking for crashes... ", "", 0);
	int Type = lua_type(L, -1);

	const char* Key = lua_tostring(L, -2);
	lua_pop(L, 2);

	//rbxlua_setmetatable(rbxlua_State, -2);

	switch (Type) {
	case LUA_TNUMBER: {
		double no = lua_tonumber(L, -1);
		rbxlua_pushnumber(rbxlua_State, no);
		rbxlua_setfield(rbxlua_State, -2, Key);
		return 0;
	}
	case LUA_TSTRING: {
		const char* str = lua_tostring(L, -1);
		rbxlua_pushstring(rbxlua_State, str);
		rbxlua_setfield(rbxlua_State, -2, Key);
		return 0;
	}
	case LUA_TBOOLEAN: {
		bool a = lua_toboolean(L, -1);
		if (a) {
			rbxlua_pushnumber(rbxlua_State, 1);
		}
		else {
			//rbxlua_pushnil(rbxlua_State);
		}
		rbxlua_setfield(rbxlua_State, -2, Key);
		return 0;
	}
	case LUA_TTABLE: {
		lua_getfield(L, -1, "__address");
		if (!lua_isnil(L, -1)) {
			int inst = lua_tonumber(L, -1);
			lua_pop(L, 1);
			int* userdata = (int*)rbxlua_newuserdata(rbxlua_State, 8);
			*userdata = inst;
			*(userdata + 1) = 0;
			rbxlua_getfield(rbxlua_State, -10000, "Object");
			//rbxlua_setmetatable(rbxlua_State, -2);
			rbxlua_setfield(rbxlua_State, -2, Key);
		}
		else {
			lua_pop(L, 1);
		}
		return 0;
	}
	}
	MessageBoxA(0, "OK", "", 0);
}

static int SpawnThread(lua_State *L) {
	lua_State* nL = lua_newthread(L);
}

static int Tick(lua_State *L) {
	//lua_pushnumber(L, std::time(0));
	return 1;
}


static int loadsring_lua(lua_State* L) {
	
	return 1;
}

static int LuaEnvExecuter(lua_State* L) {
	
	return 1;
}

static void Init(lua_State* L) {
	lua_newtable(L);
	lua_pushcfunction(L, Index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, NewIndex);
	lua_setfield(L, -2, "__newindex");
	lua_setglobal(L, "robject");

	PushInstance(L, DataModel);
	lua_setglobal(L, "game");

	PushInstance(L, Workspace);
	lua_setglobal(L, "workspace");

	PushInstance(L, Workspace);
	lua_setglobal(L, "Workspace");

	lua_pushcfunction(L, Print);
	lua_setglobal(L, "print");

	lua_pushcfunction(L, loadsring_lua);
	lua_setglobal(L, "loadstring");

	lua_pushcfunction(L, Warn);
	lua_setglobal(L, "warn");

	lua_pushcfunction(L, PrintIdentity);
	lua_setglobal(L, "printidentity");

	lua_pushcfunction(L, LuaEnvExecuter);
	lua_setglobal(L, "execute"); // test lole

	lua_pushcfunction(L, GetService);
	lua_setglobal(L, "GetService");

	lua_newtable(L);
	lua_pushcfunction(L, CreateInstance);
	lua_setfield(L, -2, "new");
	lua_setglobal(L, "Instance");

	lua_pushcfunction(L, Tick);
	lua_setglobal(L, "tick");

}

static void ExecuteScript(std::string inData)
{
	lua_State* L = luaL_newstate();
	Init(L);
	try {
		int err = luaL_dostring(L, inData.c_str());
		if (err) {
			const char* errmsg = lua_tostring(L, -1);
			std::string err = "ERROR: ";
			err += errmsg;
			MessageBoxA(0, err.c_str(), 0, 0);
			lua_pop(L, 1);
		}
	}
	catch (std::exception e) {
		std::string err = "C ERROR: ";
		err += e.what();
		MessageBoxA(0, err.c_str(), 0, 0);
	}
	lua_close(L);
}
