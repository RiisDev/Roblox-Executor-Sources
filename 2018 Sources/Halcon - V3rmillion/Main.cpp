#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <TlHelp32.h>

#define rbxaddy(x)(x - 0x400000 + ((DWORD)GetModuleHandleA(0)))

#define ScriptContextAddress rbxaddy(0x119C870)

static int rbxlua_getService(DWORD luaState, const char* serviceName);


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
#define rbxlua_getservice(L,s)		rbxlua_getService(L, (s))

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
typedef void* (__cdecl *rbxlua_PushCClosure)(DWORD luaState, int fn, int n);
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

typedef int(__thiscall *dolotsmotscots)(void*, const char* message);




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

static rbxlua_pushboolf rbxlua_pushbool = (rbxlua_pushboolf)rbxaddy(0x54A010); // 55 8B EC 8B 55 08 33 C0 39 45 0C

																			   //55 8B EC 6A FF 68 ?? ?? ?? ?? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 51 56 8B 75 08 57 8B 4E 08 8B 44 31 60 3B 44 31 54 72
static rbxlua_Newthread rbxlua_newthread = (rbxlua_Newthread)unprotect(SigScan("\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x51\x56\x8B\x75\x08\x57\x8B\x4E\x08", "xxxxxx????xx????xxxx????xxxxxxxxx")); // "Executing lua from signal, execution de" should be 0x5300000 sub by the middle


																																																															  //55 8B EC 8B 55 08 ?? ?? ?? ?? ?? ?? ?? ?? F2 0F 10 45 0C
static rbxlua_ToNumber rbxlua_tonumber = (rbxlua_ToNumber)unprotect(SigScan("\x55\x8B\xEC\x8B\x55\x08\x33\xC0\x39\x45\x0C", "xxxxxxxxxxx"));/*0x54A260);/*0x549010);/*0x546DE0);/*0x546DE0);/*0x546B50);/*0x5459C0);/*0x5465E0);/*0x5431A0);/*0x5411B0);/*0x540470);/*0x53D760);/*0x536900);/*0x534200);/*0x5348B0);/*0x534110;/*0x50EE90;/*0x517690;/*0x5180B0;
																																			/* Find "elapsedTime" & "ElapsedTime"
																																			follow the address inbetween them that looks like 'offset sub_NUMBERS'
																																			there should be 2 subs.. it should be the second one :)
																																			*/

static rbxlua_PushString rbxlua_pushstring = (rbxlua_PushString)SigScan("\x55\x8B\xEC\x8B\x55\x0C\x85\xD2\x75\x0D", "xxxxxxxxxx");//0x54A2E0);//0x549090);//0x546E60);//0x546E60);//0x546BC0);//0x545A30);//0x546650);//0x543210);//0x541220);//0x5404E0);//0x53D7D0);//0x53E3C0);//0x53C2E0);//0x536C90);//0x536DC0);//0x536E10);//0x534270);//0x534AA0);//0x534920;//0x534180;//0x516F40;//0x517700;//0x518120;//0x518D60;//0x51C280;//0x51BA60;//0x53FE10; // "__tostring"
																																  //updated
static rbxlua_SetField rbxlua_setfield = (rbxlua_SetField)unprotect(SigScan("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x83\xC4\x08\x8B\xCA\x8B\xF8\x8D\x59\x01\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\x89\x45\xF0", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx????xxx"));/*0x54AA30);/*0x5497E0);/*0x5475B0);/*0x5475B0);/*0x547290);/*0x546100);/*0x546D20);/*0x5438E0);/*0x5418F0);/*0x540BB0);/*0x53DEA0);/*0x53EA90);/*0x53C9B0);/*0x537360);/*0x537490);/*0x5374E0);/*0x534940);/*0x535170);/*0x533EC0;/*0x534FF0;/*0x534850;/*0x517610;/*0x517DD0;/*0x5187F0;/*0x517D90;//0x519430;/*0x51C950;//

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
static rbxlua_PushNumber rbxlua_pushnumber = (rbxlua_PushNumber)unprotect(SigScan("\x55\x8B\xEC\x8B\x55\x08\x56\x8B\x42\x08", "xxxxxxxxxx"));/*0x54A260);/*0x549010);/*0x546DE0);/*0x546DE0);/*0x546B50);/*0x5459C0);/*0x5465E0);/*0x5431A0);/*0x5411B0);/*0x540470);/*0x53D760);/*0x53E350);/*0x53C270);/*0x536C20);/*0x536D50);/*0x536DA0);/*0x534200);/*0x534A30);/*0x5348B0;/*0x534110;/*0x516ED0;/*0x517690;/*0x5180B0;/*0x518CF0;/*0x51C210;/*0x51C210;/*0x53FDA0;/*0x53FE50
																																			 At the very bottom of the string "math" AKA aMath and ontop of "pi" or aPi;
																																			 //updated												 */
static rbxlua_PCall rbxlua_pcall = (rbxlua_PCall)unprotect(SigScan("\x55\x8B\xEC\x8B\x45\x14\x83\xEC\x08\x53\x56", "xxxxxxxxxxx"));/*0x549D70);/*0x548B30);/*0x546900);/*0x546900);/*0x546720);/*0x545590);/*0x5461B0);/*0x542D60);/*0x540D80);/*0x540040);/*0x53D330);/*0x53DF20);/*0x53BE40);/*0x5367F0);/*0x536920);/*0x536970);/*0x533DB0);/*0x534600);/*0x534480;/*0x533CE0;/*0x516AA0;/*0x517260;/*0x517C80;/*0x5188C0;/*0x51BDE0;/*0x51B5C0;/*0x53F970;/*0x5388B0;
																																   Find "Breakpoint %d condition runtime error: " AKA aBreakpointDC_0;
																																   The first sub_NUMBERS in that function will be pcall;
																																   */
static rbxlua_ToLString rbxlua_tolstring = (rbxlua_ToLString)unprotect(SigScan("\x55\x8B\xEC\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\xF8", "xxxxxxxxxxxxx????xx"));/*0x54AF00);/*0x549CB0);/*0x547A80);/*0x547A80);/*0x547700);/*0x546570);/*0x5471A0);/*0x543D50);/*0x541D60);/*0x541020);/*0x53E310);/*0x53EF00);/*0x53CE20);/*0x5377D0);/*0x537900);/*0x537950);/*0x534DB0);/*0x5355E0);/*0x535460;/*0x534CC0;/*0x517A80;/*0x518240;/*0x518C60;/*0x5198A0;/*0x51CDC0;/*0x51C5A0;/*0x532450;		/*0x53C150;
																																													   Find "!handler.empty()" AKA aHandler_empty;
																																													   And follow the 'jnz short loc_NUMBERS' right under the emptyhandler;
																																													   And find the first sub_NUMBERS and your done!1;
																																													   */
																																													   //updated// newproxy only supports the arguments ni
static rbxlua_SetTop _rbxlua_settop = (rbxlua_SetTop)rbxaddy(0x54A010);/*0x549E30);/*0x548BF0);/*0x5469C0);/*0x5469C0);/*0x547520);/*0x546390);/*0x546FC0);/*0x543B70);/*0x541B80);/*0x540E40);/*0x53E130);/*0x53ED20);/*0x53CC40);/*0x5375F0);/*0x537720);/*0x537770);/*0x533810);/*0x535280;/*0x533EE0;/*0x533740;/*0x516500;/*0x516CC0;/*0x5176E0;/*0x518320;/*0x51/B840;/*0x51B020;/*0x540770;/*0x53F3D0;				/*0x53BFB0;
																	   Find "top+1 == lua_gettop(functionThread)" AKA aTop1rbxlua_gettop;
																	   And the first sub_NUMBERS on the function is the address;
																	   */
																	   //updated
static rbxlua_ToBoolean _rbxlua_toboolean = (rbxlua_ToBoolean)rbxaddy(0x54B060);/*0x54AE80);/*0x549C30);/*0x547A00);/*0x547A00);/*0x547680);/*0x5464F0);/*0x547120);/*0x543CD0);/*0x541CE0);/*0x540FA0);/*0x53E290);/*0x53EE80);/*0x53CDA0);/*0x537750);/*0x537880);/*0x5378D0);/*0x534D30);/*0x5353E0);/*0x534C40;/*0x517A00;/*0x5181C0;/*0x518BE0;/*0x519820;/*0x51CD40;/*0x51C520;/*0x5408D0;		/*0x53C0D0;
																				Find "isdst" AKA aIsdst;
																				And follow the 'jnz short loc_NUMBERS' in the function;
																				And it will be the sub_NUMBERS in the function;
																				[normally right under the aIsdst function];
																				//updated													*/
static rbxlua_Type _rbxlua_type = (rbxlua_Type)SigScan("\x55\x8B\xEC\xFF\x75\x0C\xFF\x75\x08\xE8\x00\x00\x00\x00\x83\xC4\x08\x3D\x00\x00\x00\x00\x75\x05", "xxxxxxxxxx????xxxx????xx");/*0x54B2C0);/*0x54A070);/*0x547E40);/*0x547E40);/*0x547A70);/*0x5468E0);/*0x547510);/*0x5440C0);/*0x5420D0);/*0x541390);/*0x53E680);/*0x53F270);/*0x53D190);/*0x537B40);/*0x537C70);/*0x537CC0);/*0x535120);/*0x535950);/*0x5357D0;/*0x535030;/*0x517DF0;/*0x5185B0;/*0x518FD0;/*0x519C10;/*0x51D130;/*0x51C910;/*0x540CC0;						/*0x53C440;
																																													   Find "rbxlua_isthread(functionThread, -1)" AKA arbxlua_isthreadFu;
																																													   It will be the first sub_NUMBERS in the function;
																																													   //updated													*/
static rbxlua_GetField rbxlua_getfield = (rbxlua_GetField)unprotect(SigScan("\x55\x8B\xEC\x83\xEC\x10\x53\x56\x8B\x75\x08\x57\xFF\x75\x0C\x56\xE8\x00\x00\x00\x00\x8B\x55\x10\x83\xC4\x08\x8B\xCA\x8B\xF8\x8D\x59\x01\x8A\x01\x41\x84\xC0\x75\xF9\x2B\xCB\x51\x52\x56\xE8\x00\x00\x00\x00\xFF\x76\x10", "xxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxx????xxx"));/*0x549550);/*0x548310);/*0x5460E0);/*0x5460E0);/*0x545FE0);/*0x544E50);/*0x545A70);/*0x542620);/*0x540640);/*0x53F900);/*0x53CBF0);/*0x53D7E0);/*0x53B700);/*0x5360B0);/*0x5361E0);/*0x536230);/*0x533670);/*0x533EC0);/*0x533D40;/*0x5335A0;/*0x516360;/*0x516B20;/*0x517540;/*0x518180;/*0x518180;/*0x51B6A0;/*0x51D130;/*0x51B6A0;/*0x51B6A0;/*0x51AE80;/*0x53F230;		/*0x53AE70;
																																																																																								   Find "__unm" AKA a__unm [it should say offset to the left of a_unm];
																																																																																								   Follow the 'offset sub_NUMBERS' right underneath it;
																																																																																								   Follow the last loc_NUMBERS in the function;
																																																																																								   It should be the first sub_NUMBERS;
																																																																																								   //updated													*/
static rbxlua_PushValue rbxlua_pushvalue = (rbxlua_PushValue)unprotect(SigScan("\x55\x8B\xEC\x56\xFF\x75\x0C\x8B\x75\x08\x56\xE8\x00\x00\x00\x00\x8B\x56\x10\x83\xC4\x08", "xxxxxxxxxxxx????xxxxxx"));/*0x54A3A0);/*0x549150);/*0x546F20);/*0x546F20);/*0x546CB0);/*0x545B20);/*0x546740);/*0x543300);/*0x541310);/*0x5405D0);/*0x53D8C0);/*0x53E4B0);/*0x53C3D0);/*0x536D80);/*0x536EB0);/*0x536F00);/*0x534360);/*0x534B90);/*0x534A10;/*0x534270;/*0x517030;/*0x5177F0;/*0x518210;/*0x518E50;/*0x51C370;/*0x51CB70;/*0x572700;/*0x51BB50;/*0x53FF00;		/*0x53B8D0;
																																																	  Find "%s.%s" AKA aS_S;
																																																	  In the function look for 'jz short loc_NUMBERS' and follow it;
																																																	  And it should be the second to last 'call sub_NUMBERS' in the function;
																																																	  //updated													*/
static rbxlua_GetTop _rbxlua_gettop = (rbxlua_GetTop)rbxaddy(0x549900);/*0x549720);/*0x5484E0);/*0x5462B0);/*0x5462B0);/*0x546180);/*0x544FF0);/*0x545C10);/*0x5427C0);/*0x5407E0);/*0x53FAA0);/*0x53CD90);/*0x53D980);/*0x53B8A0);/*0x536250);/*0x536380);/*0x5363D0);/*0x533810);/*0x534060);/*0x533EE0;/*0x533740;/*0x516500;/*0x516CC0;/*0x5176E0;/*0x518320;/*0x51B840;/*0x51B020;/*0x53F3D0;				/*0x53AFB0;
																	   Find "tostring" AKA aTostring;
																	   It will be right on top of the "offset aTostring ; "tostring"";
																	   */
static rbxlua_ToUserData rbxlua_touserdata = (rbxlua_ToUserData)SigScan("\x55\x8B\xEC\x56\x8B\x75\x08\x6A\x01\x56\xE8\x00\x00\x00\x00\x8D\x45\x10", "xxxxxxxxxxx????xxx");/*0x54B290);/*0x54A040);/*0x547E10);/*0x547E10);/*0x547A40);/*0x5468B0);/*0x5474E0);/*0x544090);/*0x5420A0);/*0x541360);/*0x53E650);/*0x53F240);/*0x53D160);/*0x537B10);/*0x537C40);/*0x5357A0);/*0x517DC0;/*0x518580;/*0x518FA0;/*0x519BE0;/*0x51D100;/*0x51C8E0;/*0x540C90;		/*0x53C410;
																																										  Find "Bad argument materials[%d][%d][%d] to '" AKA aBadArgumentM_0;
																																										  Should be the 3rd sub_NUMBERS in the function;
																																										  [and the 1st and 2nd should possibly be the same]
																																										  */
static rbxlua_Next rbxlua_next;

static rbxlua_chatthing _rbxluachat = (rbxlua_chatthing)rbxaddy(0xB1C2E0);

static rbxlua_PushCClosure rbxlua_pushcclosure = (rbxlua_PushCClosure)unprotect(SigScan("\x55\x8B\xEC\x56\x8B\x75\x08\x8B\x4E\x08\x8B\x44\x0E\x60", "xxxxxxxxxxxxxx"));
static rbxlua_Insert rbxlua_insert;
static rbxlua_Remove rbxlua_remove;

static rbxlua_Setmetatable rbxlua_setmetatable = (rbxlua_Setmetatable)unprotect(SigScan("\x55\x8B\xEC\x56\x8B\x75\x08\x57\x8B\x4E\x08\x8B\x44\x31\x60\x3B\x44\x31\x54\x72\x09\x56\xE8\x00\x00\x00\x00\x83\xC4\x04\x8B\x46\x0C", "xxxxxxxxxxxxxxxxxxxxxxx"));

static rbxlua_Newuserdata rbxlua_newuserdata = (rbxlua_Newuserdata)unprotect(SigScan("\x55\x8B\xEC\x53\x56\x57\xFF\x75\x0C\x8B\x7D\x08\x57\xE8", "xxxxxxxxxxxxxx"));

static int rbxlua_type(DWORD luaState, size_t Size, BOOL luaIsNil)
{
	try
	{
		_rbxlua_type(luaState, Size);
	}
	catch (std::exception e)
	{

	}
	return 0;
}

// RunCommand.h
#include <Windows.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include "CoreMain.h"
#include "LuaFuncs.h"

/*
extern "C"
{
#include "CoreLua\lua.h"
#include "CoreLua\lauxlib.h"
#include "CoreLua\lualib.h"
#include "CoreLua\lopcodes.h"
#include "CoreLua\lstring.h"
#include "CoreLua\lfunc.h"
#include "CoreLua\ldo.h"
#include "CoreLua\lvm.h"
}
*/

std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

std::string Input() {
	std::string ass;
	getline(std::cin, ass);
	return ass;
}

void ConsoleHacks()
{
	DWORD nothing;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nothing); //roblox has a "freeconsole" gay thingy
	*(BYTE*)(&FreeConsole) = 0xC3;
}

void Console(char* title) { //void console haxs
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow(); //gets the console window
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW); //make program topmost (above roblox)
																													   //topmost function: Look at http://www.cplusplus.com/forum/windows/125528/
	::ShowWindow(ConsoleHandle, SW_NORMAL); //show window
}

constexpr unsigned int _INT(const char* str, int h = 0) {
	return !str[h] ? 5381 : (_INT(str, h + 1) * 33) ^ str[h];
}

std::vector<std::string> GetArguments(std::string input) {
	std::vector<std::string> rtn;

	if (input[0] == ' ') {
		input = input.substr(1);
	}
	//BYTE
	std::size_t size = input.size();
	DWORD pos1 = 0;

	for (std::size_t i = 0; i < size; ++i) {
		if (input[i] == ' ') {
			rtn.push_back(input.substr(pos1, i - pos1));
			pos1 = i + 1;
		}
		else if (i == size - 1) {
			rtn.push_back(input.substr(pos1, i - pos1 + 1));
			pos1 = i + 1;
		}
	}
	return rtn;
}

void DoCommand(std::string Cmd) {
	try {
		std::vector<std::string> args = GetArguments(Cmd);
		//std::string cmdarg = command.at(1).c_str();
		std::string command = args.front(); args.erase(args.begin());
		std::vector<DWORD> players = GetPlayersFromMessage(args);
		if (args.size() != NULL) {
			switch (_INT(command.c_str()))
			{
			case _INT("ff"):
			{
				try {
					for (std::size_t i = 0; players.size(); i++) {
						std::string* Player = GetName(players[i]);
						rbxlua_getglobal(luaState, "game");
						rbxlua_getfield(luaState, -1, "Players");
						rbxlua_getfield(luaState, -1, Player->c_str());
						rbxlua_getfield(luaState, -1, "Character");
						rbxlua_getglobal(luaState, "Instance");
						rbxlua_getfield(luaState, -1, "new");
						rbxlua_pushstring(luaState, "ForceField");
						rbxlua_pushvalue(luaState, -4);
						rbxlua_pcall(luaState, 2, 1, 0);
					}
				}
				catch (std::exception e)
				{

				}
			}
			case _INT("kill"):
			{
				try {
					for (std::size_t i = 0; players.size(); i++) {
						std::string* Player = GetName(players[i]);
						rbxlua_getglobal(luaState, "game");
						rbxlua_getfield(luaState, -1, "Players");
						rbxlua_getfield(luaState, -1, Player->c_str());
						rbxlua_getfield(luaState, -1, "Character");
						rbxlua_getfield(luaState, -1, "BreakJoints");
						rbxlua_pushvalue(luaState, -2);
						rbxlua_pcall(luaState, 1, 0, 0);
					}
				}
				catch (std::exception e)
				{

				}
			}
			case _INT("time"): {
				std::string timefloat = args.at(0).c_str();
				rbxlua_getglobal(luaState, "game");
				rbxlua_getfield(luaState, -1, "Lighting");
				rbxlua_pushstring(luaState, timefloat.c_str());
				rbxlua_setfield(luaState, -2, "TimeOfDay");
			}
			case _INT("print"):
			{
				std::string msg = args.at(0).c_str();
				rbxlua_getglobal(luaState, "print");
				rbxlua_pushstring(luaState, msg.c_str());
				rbxlua_pcall(luaState, 1, 0, 0);
			}
			case _INT("sparkles"):
			{
				for (std::size_t i = 0; players.size(); i++) {
					std::string* Player = GetName(players[i]);
					rbxlua_getglobal(luaState, "game");
					rbxlua_getfield(luaState, -1, "Players");
					rbxlua_getfield(luaState, -1, Player->c_str());
					rbxlua_getfield(luaState, -1, "Character");
					rbxlua_getfield(luaState, -1, "Torso");
					rbxlua_getglobal(luaState, "Instance");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushstring(luaState, "Sparkles");
					rbxlua_pushvalue(luaState, -4);
					rbxlua_pcall(luaState, 2, 1, 0);
				}
			}
			case _INT("fire"):
			{
				for (std::size_t i = 0; players.size(); i++) {
					std::string* Player = GetName(players[i]);
					rbxlua_getglobal(luaState, "game");
					rbxlua_getfield(luaState, -1, "Players");
					rbxlua_getfield(luaState, -1, Player->c_str());
					rbxlua_getfield(luaState, -1, "Character");
					rbxlua_getfield(luaState, -1, "Torso");
					rbxlua_getglobal(luaState, "Instance");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushstring(luaState, "Fire");
					rbxlua_pushvalue(luaState, -4);
					rbxlua_pcall(luaState, 2, 1, 0);
				}
			}
			case _INT("smoke"):
			{
				for (std::size_t i = 0; players.size(); i++) {
					std::string* Player = GetName(players[i]);
					rbxlua_getglobal(luaState, "game");
					rbxlua_getfield(luaState, -1, "Players");
					rbxlua_getfield(luaState, -1, Player->c_str());
					rbxlua_getfield(luaState, -1, "Character");
					rbxlua_getfield(luaState, -1, "Torso");
					rbxlua_getglobal(luaState, "Instance");
					rbxlua_getfield(luaState, -1, "new");
					rbxlua_pushstring(luaState, "Smoke");
					rbxlua_pushvalue(luaState, -4);
					rbxlua_pcall(luaState, 2, 1, 0);
				}
			}
			case _INT("fog"):
			{
				std::string pusharg = args.at(0).c_str();
				rbxlua_getglobal(luaState, "game");
				rbxlua_getfield(luaState, -1, "Lighting");
				rbxlua_pushstring(luaState, pusharg.c_str());
				rbxlua_setfield(luaState, -2, "FogEnd");
			}
			case _INT("ragdoll"):
			{
				for (std::size_t i = 0; players.size(); i++) {
					std::string Player = *GetName(players[i]);
					rbxlua_getglobal(luaState, "game");
					rbxlua_getfield(luaState, -1, "Workspace");
					rbxlua_getfield(luaState, -1, Player.c_str());
					rbxlua_getfield(luaState, -1, "Humanoid");
					rbxlua_pushstring(luaState, "1");
					rbxlua_setfield(luaState, -2, "PlatformStand");
				}
			}
			case _INT("noob"):
			{
				try {
					for (std::size_t i = 0; players.size(); i++) {
						std::string Player = *GetName(players[i]);
						rbxlua_getglobal(luaState, "game");
						rbxlua_getfield(luaState, -1, "Players");
						rbxlua_getfield(luaState, -1, Player.c_str());
						rbxlua_pushstring(luaState, "false");
						rbxlua_setfield(luaState, -2, "CanLoadCharacterAppearance");
					}
				}
				catch (std::exception e)
				{

				}
			}
			case _INT("regular"):
			{
				try {
					for (std::size_t i = 0; players.size(); i++) {
						std::string Player = *GetName(players[i]);
						rbxlua_getglobal(luaState, "game");
						rbxlua_getfield(luaState, -1, "Players");
						rbxlua_getfield(luaState, -1, Player.c_str());
						rbxlua_pushstring(luaState, "1");
						rbxlua_setfield(luaState, -2, "CanLoadCharacterAppearance");
					}
				}
				catch (std::exception e)
				{

				}
			}
			case _INT("animate"):
			{
				try {
					for (std::size_t i = 0; players.size(); i++)
					{
						std::string Player = *GetName(players[i]);
						rbxlua_getglobal(luaState, "game");
						rbxlua_getfield(luaState, -1, "Workspace");
						rbxlua_getfield(luaState, -1, Player.c_str());
						rbxlua_getfield(luaState, -1, "Animate");
						rbxlua_pushstring(luaState, "1");
						rbxlua_setfield(luaState, -2, "Disabled");
					}
				}
				catch (std::exception e)
				{

				}
			}
			case _INT("fegod"):
			{
				rbxlua_getglobal(luaState, "game");
				rbxlua_getfield(luaState, -1, "Workspace");
				rbxlua_getfield(luaState, -1, "RipAxstin");
				rbxlua_getfield(luaState, -1, "Humanoid");
				rbxlua_getfield(luaState, -1, "Health");
				rbxlua_getfield(luaState, -1, "Destroy");
				rbxlua_pcall(luaState, 1, 1, 0);
				printf("fegod ran.\n");
			}
			case _INT("terrianclr"):
			{
				rbxlua_getglobal(luaState, "game");
				rbxlua_getfield(luaState, -1, "Workspace");
				rbxlua_getfield(luaState, -1, "Terrian");
				rbxlua_getfield(luaState, -1, "Clear");
				rbxlua_pushvalue(luaState, -2);
				rbxlua_pcall(luaState, 1, 0, 0);
			}
			}
		}
	}
	catch (std::exception e)
	{
		printf("\r\n");
	}
}

// Main.cpp

#include "RbxOffsets.h"
#include "Memory.h"
#include "LuaFuncs.h"
#include "CoreMain.h"
#include "RunCommand.h"
#include "CoreChecks.h"



using namespace std;

typedef int(__thiscall *nsopenstate)(DWORD self);
nsopenstate openstate = (nsopenstate)SigScan("\x55\x8B\xEC\x56\x57\x6A\x05", "xxxxxxx");

static BYTE GetFE(BYTE Workspace)
{
	return *(byte*)(Workspace + 0x25B);
}

void Main()
{
	try {
		ConsoleHacks();
		Console("");
		printf("Scanning... ");
		DWORD ScriptContextVftable = ScriptContextAddress;
		int ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContextVftable, "xxxx");
		DataModel = GetParent(ScriptContext);
		Players = FindFirstClass(DataModel, "Players");
		Workspace = FindFirstClass(DataModel, "Workspace");
		luaState = *(DWORD*)(ScriptContext + 220) - (ScriptContext + 220);
		printf("OK!\n");
		std::cout << "Current LuaState: " << std::hex << luaState << "\n";
		HANDLE hPipe;
		char buffer[1024];
		DWORD dwRead;
		do
		{
			std::string x = Input();
			DoCommand(x);
		} while (true);
	}
	catch (std::exception e)
	{
		
	}
}

int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
	}
	return TRUE;
}