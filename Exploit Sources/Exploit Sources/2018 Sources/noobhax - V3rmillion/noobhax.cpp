#define _CRT_SECURE_NO_WARNINGS

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdafx.h"
#include <Windows.h>
#include <ShellAPI.h>
#include <iostream>
#include <string>
#include <Tlhelp32.h> 
#include <stdio.h>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <VMProtectSDK.h>
#include "sha256.h"
#include <WinInet.h>
#include <thread>
#include <chrono>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "lua5.1.lib")
#include <Richedit.h>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lua.hpp>
#include <luaconf.h>
#include <lualib.h>
}

/*
Hello, greetings from Bob and Exploit Busters.
Snivy has been such a dickhead lately, and um here you go.
This source is fucking fully skidded and all messy.

Snivy himself is a skid and put everything into one cppfile LOLLLL

bye
*/

lua_State* MyL;


bool hostsok = true;

bool clickdelbool = false;

bool clicktpbool = true;

bool clickexplodebool = false;

bool clickpartbool = false;

bool clickunanchorbool = false;

bool clickanchorbool = false;

bool clicklockbool = false;

bool clickunlockbool = false;

bool clickcancollidefalsebool = false;

bool clickcancollidetruebool = false;

#define setglobal(L, s) rlua_setfield(L, -10002, s)

#define getaddy2(address) (address - 0x400000 + (DWORD)GetModuleHandle(0))

using namespace std;

using namespace std::chrono_literals;

bool whitelisted = true; //ok

bool scanned = false;

boost::thread_group my_threads;

std::vector<int> GetChildren(int Instance);

void Execute(string command, int rlua_State);

bool FileExists(const char *fileName);

BOOL DirectoryExists(const char* dirName);

vector<string> split2(string str, char delimiter);



int vasprintf(char **str, const char *fmt, va_list args)
{
	int size = 0;
	va_list tmpa;
	va_copy(tmpa, args);
	size = vsnprintf(NULL, size, fmt, tmpa);
	va_end(tmpa);

	if (size < 0)
		return -1;
	*str = (char *)malloc(size + 1);
	if (NULL == *str)
		return -1;

	size = vsprintf(*str, fmt, args);
	return size;
}

int removeddoors = 0;

HWND txtbox = NULL;

void Execute2(string command, int rlua_State, boost::thread_group & thg, boost::thread * thisTh) {
	Execute(command, rlua_State);
	thg.remove_thread(thisTh);
	delete thisTh;
}

int rlua_State = 0;

std::string fuckingosha() {
	VMProtectBeginMutation("noob1");
	TCHAR value[255];
	DWORD BufferSize = 255;
	HKEY key = NULL;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, ("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &key);
	int a = RegGetValue(key, "", ("ProductId"), RRF_RT_ANY, NULL, (PVOID)&value, &BufferSize);
	return sha256(value);
	VMProtectEnd();
}

bool boolclicktodelete = false;

bool boolprintmousepos = false;

bool boolrclicktotp = false;

int usednoobiesky = 0;

string prefix = ";";

vector<string> adminplayers;

bool IsAdmin(string fullplayername) {
	bool returnbool = false;
	for (int i = 0; i < adminplayers.size(); i++) {
		if (adminplayers[i] == fullplayername) {
			returnbool = true;
		}
	}
	return returnbool;
}


std::vector<std::string> split(std::string s) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	return vstrings;
}

vector<string> split2(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str);
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}


namespace Memory {
	bool Compare(const char* pData, const char* bMask, const char* szMask)
	{
		while (*szMask) {
			if (*szMask != '?') {
				if (*pData != *bMask) return 0;
			}
			++szMask, ++pData, ++bMask;
		}
		return 1;
	}

	DWORD Scan(const char* vftable)
	{
		MEMORY_BASIC_INFORMATION MBI;
		SYSTEM_INFO SI;
		GetSystemInfo(&SI);
		DWORD Start = (DWORD)SI.lpMinimumApplicationAddress;
		DWORD End = (DWORD)SI.lpMaximumApplicationAddress;
		while (Start < End && VirtualQuery((void*)Start, &MBI, sizeof(MBI))) {
			if ((MBI.Protect & PAGE_READWRITE) && !(MBI.Protect & PAGE_GUARD))
			{
				for (DWORD i = (DWORD)(MBI.BaseAddress); i - (DWORD)(MBI.BaseAddress) < MBI.RegionSize; ++i)
				{
					if (Compare((const char*)i, vftable, "xxxx"))
						return i;
				}
			}
			Start += MBI.RegionSize;
		}
		return 0;
	}


}

int DataModel = 0;
int Workspace = 0;
int Players = 0;
int ScriptContext = 0;

std::string* GetName(int Instance) {
	return (std::string*)(*(int*)(Instance + 0x28) - 0x4);
}

const char* GetClass(int self)
{

	return (const char*)(*(int(**)(void))(*(DWORD *)self + 16))();
}

int FindFirstChild_PartOf(int Instance, std::string Name) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	int fuckingnoob = 0;
	try {
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (strstr(GetName(*(int*)i)->c_str(), Name.c_str()))
			{
				if (*(int*)i == 0)
				{

				}
				else {
					fuckingnoob = *(int*)i;
				}
			}
		}
	}
	catch (exception) {
		Sleep(1);
	}
	if (fuckingnoob != 0) {
		return fuckingnoob;
	}
	else {
		return 0;
	}
}

int FindFirstClass(int Instance, const char* Name) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	int a = 0;
	try {
		for (int i = *(int*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			if (memcmp(GetClass(*(int*)i), Name, strlen(Name)) == 0)
			{
				return *(int*)i;
			}
		}
	}
	catch (exception) {
		Sleep(1);
	}
	return 0;
}

std::vector<int> GetChildren(int Instance) {
	DWORD StartOfChildren = *(DWORD*)(Instance + 0x2C);
	DWORD EndOfChildren = *(DWORD*)(StartOfChildren + 4);
	std::vector<int> Children = {};
	try {
		for (int i = *(int*)StartOfChildren; i < EndOfChildren; i += 8)
		{
			try {
				Children.push_back(*(int*)i);
			}
			catch (exception) {
				Sleep(1);
			}
		}
	}
	catch (exception) {
		Sleep(1);
	}
	return Children;
}

int GetLocalPlayer(int Plr) {
	return *(int*)(Plr + 196);
}


WNDPROC RBLXWindowProc;

typedef int*(__cdecl *Context)();
Context ContextLevel = (Context)getaddy2(0x58FDF0);

void ChangeContextLevel(int newlevel) {
	int *Level = ContextLevel();
	*Level = newlevel;
}


DWORD unprotect(DWORD addr)
{
	BYTE* tAddr = (BYTE*)addr;

	/* Calcualte the size of the function
	In theory this will run until it hits the next
	functions prolog. It assumes all calls are aligned to
	16 bytes. (grazie katie)
	*/
	do
	{
		tAddr += 16;
	} while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

	DWORD funcSz = tAddr - (BYTE*)addr;

	/* Allocate memory for the new function */
	PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (nFunc == NULL)
		return addr;

	/* Copy the function to the newly allocated memory */
	memcpy(nFunc, (void*)addr, funcSz);

	BYTE* pos = (BYTE*)nFunc;
	BOOL valid = false;
	do
	{
		/* Check for the return check with the sig:
		72 ?? A1 ?? ?? ?? ?? 8B
		If the sig matches replace the the jb with a jmp.
		*/
		if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
			*(BYTE*)pos = 0xEB;

			DWORD cByte = (DWORD)nFunc;
			do
			{
				/* Check if the current byte is a call if it is,
				calculate the new relative call(s).
				*(->E8 + 1) = originalFunction - nextInstruction

				oFuncPos - Position of call in original function
				= originalFunction + (->E8 - newFunction)

				oFuncAddr - Original call location
				= oFuncPos + rel32Offset + sizeof(call)

				relativeAddr - New relative address
				= oFuncAddr - ->E8 - sizeof(call)

				Since we are not using a disassembler we assume
				that if we hit a E8 byte which is properly aligned
				it is a relative call.
				For a small amount of compensation I skip location
				of the call, since it is possible to have the byte
				E8 inside of it.
				*/
				if (*(BYTE*)cByte == 0xE8)
				{
					DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
					DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

					if (oFuncAddr % 16 == 0)
					{
						DWORD relativeAddr = oFuncAddr - cByte - 5;
						*(DWORD*)(cByte + 1) = relativeAddr;

						/* Don't check rel32 */
						cByte += 4;
					}
				}

				cByte += 1;
			} while (cByte - (DWORD)nFunc < funcSz);

			valid = true;
		}
		pos += 1;
	} while ((DWORD)pos < (DWORD)nFunc + funcSz);

	/* This function has no return check, let's not waste memory */
	if (!valid)
	{
		VirtualFree(nFunc, funcSz, MEM_RELEASE);
		return addr;
	}

	return (DWORD)nFunc;
}

#define gettopaddr 0x736380 //7387A0


typedef int(__cdecl *xdLua_gettop)(int rlua_State);
xdLua_gettop rlua_gettop = (xdLua_gettop)getaddy2(gettopaddr);

typedef void(__stdcall *xdLua_getfield)(int rlua_State, int index, const char *k);
xdLua_getfield getfield = (xdLua_getfield)unprotect(getaddy2(gettopaddr - 0x4f0));

typedef int(__cdecl *testingk)(unsigned __int16 a1, char a2);
testingk hitest = (testingk)getaddy2(0x4A7B10);

typedef int(__stdcall *xdLua_pushstring)(int rlua_State, const char *s);
xdLua_pushstring pushstring = (xdLua_pushstring)getaddy2(0x737210); //gettop +0xe90

typedef DWORD*(__fastcall *idx)(int a1, signed int a2);
idx Idx = (idx)getaddy2(0x72D6A0); //in pushvalue which is 7372D0 (gettop + 0xf50)

#define LSN 32 //lua struct number

void pushvalue(int a1, int a2) {
	DWORD* v2;
	int v3;

	v2 = Idx(a1, a2);
	v3 = *(DWORD *)(a1 + LSN);
	*(DWORD *)v3 = *v2;
	*(DWORD *)(v3 + 4) = v2[1];
	*(DWORD *)(v3 + 8) = v2[2];
	*(DWORD *)(a1 + LSN) += 16;
}

void pushboolean(int a1, int a2) { //736D10 (gettop + 0x990)
	int v2;

	v2 = *(DWORD *)(a1 + LSN);
	*(DWORD *)v2 = a2 != 0;
	*(DWORD *)(v2 + 8) = 3;
	*(DWORD *)(a1 + LSN) += 16;
}

#define thing 8

void settop(int a1, int a2) { //738030 (gettop + 0x1cb0)
	int i;

	if (a2 < 0)
	{
		*(DWORD *)(a1 + LSN) += 16 * a2 + 16;
	}
	else
	{
		for (i = 16 * a2; *(DWORD *)(a1 + LSN) < (unsigned int)(i + *(DWORD *)(a1 + thing)); *(DWORD *)(a1 + LSN) += 16)
			*(DWORD *)(*(DWORD *)(a1 + LSN) + thing) = 0;
		*(DWORD *)(a1 + LSN) = i + *(DWORD *)(a1 + thing);
	}
}

typedef int(__cdecl *xdLua_newthread)(int rlua_State);
xdLua_newthread NewThread = (xdLua_newthread)unprotect(getaddy2(0x736940));

typedef int(__cdecl *xdLua_pcalls)(int rlua_State, int nargs, int nresults, int err);
xdLua_pcalls rlua_pcall = (xdLua_pcalls)unprotect(getaddy2(0x736C50)); //gettop + 0x8d0

int tempnoob;

int pcall(int a1, int a2, int a3, int a4) {
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getaddy2(0x72EE37)), "\x75", 1, 0); //pcall - 0x7e19  set to jump if NOT 0
	tempnoob = rlua_pcall(a1, a2, a3, a4);
	WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getaddy2(0x72EE37)), "\x74", 1, 0); //set instruction back to jump if zero
	return tempnoob;
}

typedef int(__stdcall *xdLua_setfield)(int rlua_State, signed int index, const char *k);
xdLua_setfield rlua_setfield = (xdLua_setfield)unprotect(getaddy2(0x737C40)); //gettop + 0x18c0

DWORD coloroknoob = 0;

void Output(HWND shit, std::string text, DWORD color = coloroknoob, bool bold = false)
{
	va_list va;
	const char* format = text.c_str();
	va_start(va, format);
	char *buffer;
	if (vasprintf(&buffer, format, va) == -1)
		return;
	va_end(va);

	GETTEXTLENGTHEX gte;
	memset(&gte, 0, sizeof(SETTEXTEX));
	gte.flags = GTL_DEFAULT;
	gte.codepage = CP_ACP;
	int len = SendMessage(shit, EM_GETTEXTLENGTHEX, (WPARAM)&gte, 0);
	int buflen = strlen(buffer);

	SendMessage(shit, EM_SETSEL, -1, -1);

	SETTEXTEX ste;
	memset(&ste, 0, sizeof(SETTEXTEX));
	ste.flags = ST_NEWCHARS | ST_SELECTION;
	ste.codepage = CP_ACP;
	SendMessage(shit, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)buffer);

	SendMessage(shit, EM_SETSEL, len, len + buflen);

	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	if (bold)
	{
		cf.dwMask = CFM_COLOR | CFM_BOLD;
		cf.crTextColor = color;
		cf.dwEffects = CFE_BOLD;
	}
	else
	{
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = color;
	}
	SendMessage(shit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	SendMessage(shit, EM_SETSEL, -1, -1);
	SendMessage(shit, WM_VSCROLL, SB_BOTTOM, 0);
	free(buffer);
}
/*
DWORD EIP_BP = getaddy2(0x40F552);


DWORD EIPOK;

int noobhook(int rlua_State);

LONG __stdcall vehHandler(PEXCEPTION_POINTERS e)
{
if (e->ContextRecord->Eip == EIP_BP)
{
EIPOK = e->ContextRecord->Eip;
e->ContextRecord->Eip = (DWORD)(noobhook);
return EXCEPTION_CONTINUE_EXECUTION;
}

return EXCEPTION_CONTINUE_SEARCH;
}

LONG __stdcall vehHandler2(PEXCEPTION_POINTERS e)
{
if (e->ContextRecord->Eip == (DWORD)(noobhook))
{
e->ContextRecord->Eip = EIPOK;
return EXCEPTION_CONTINUE_EXECUTION;
}

return EXCEPTION_CONTINUE_SEARCH;
}

void callcheck() {
AddVectoredExceptionHandler(TRUE, &vehHandler);
}

void callcheck2() {
AddVectoredExceptionHandler(TRUE, &vehHandler2);
}*/


void getglobal(int rlua_State, const char *k) {
	getfield(rlua_State, -10002, k);
}

typedef double(__stdcall *xdLua_tonumber)(int rlua_State, int index);
xdLua_tonumber rlua_tonumber = (xdLua_tonumber)getaddy2(0x738550); //gettop +0x21d0

typedef signed int(__fastcall *gmt)(int a1, int a2);
gmt getmetatable = (gmt)unprotect(getaddy2(0x736160)); //gettop - 0x220

void setfield(int a1, int a2, const char *a3) {
	pushvalue(a1, a2);
	getmetatable(a1, -1);
	getfield(a1, -1, "__newindex");
	pushvalue(a1, -3);
	pushstring(a1, a3);
	pushvalue(a1, -6);
	pcall(a1, 3, 0, 0);
	settop(a1, -4);
}

typedef int(__thiscall *xdLua_pushnumber)(int rlua_State, double n);
xdLua_pushnumber pushnumber = (xdLua_pushnumber)unprotect(getaddy2(0x737180)); //gettop + 0xe00

typedef const char*(__fastcall *xdLua_tolstring)(int rlua_State, int index, DWORD* len);
xdLua_tolstring rlua_tolstring = (xdLua_tolstring)getaddy2(0x738270); //gettop + 0x1ef0

																	  /*typedef int(__cdecl *xdLua_PushCClosure)(int rlua_State, int function, int n);
																	  xdLua_PushCClosure zPushCClosure = (xdLua_PushCClosure)getaddy2(0x722150); //gettop + 0x9f0

																	  int PushCClosure(int rlua_State, int function, int n) {
																	  WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getaddy2(0x7221BB)), "\xEB", 1, 0);
																	  WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getaddy2(0x722236)), "\xEB", 1, 0);
																	  int samepqpqpq = zPushCClosure(rlua_State, function, n);
																	  WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getaddy2(0x7221BB)), "\x72", 1, 0);
																	  WriteProcessMemory(GetCurrentProcess(), reinterpret_cast<void*>(getaddy2(0x722236)), "\x72", 1, 0);
																	  return samepqpqpq;
																	  }*/

typedef BOOL(__cdecl *xdLua_toboolean)(int rlua_State, int index);
xdLua_toboolean rlua_toboolean = (xdLua_toboolean)getaddy2(0x7381B0); //tonumber - 0x3a0

																	  /*void rlua_pushcfunction(int L, int f) {
																	  PushCClosure(L, (f), 0);
																	  }*/

vector<string> getnames(string allorothers) {
	vector<int> noobs;
	vector<string> thefuckthatwillreturn;
	try {
		noobs = GetChildren(Players);
	}
	catch (exception) {
		Sleep(1);
	}
	if (noobs.size() > 0) {
		if (allorothers == "all") {
			for (int i = 0; i < noobs.size(); i++)
			{
				string noobname = *GetName(noobs[i]);
				thefuckthatwillreturn.push_back(noobname);
			}
		}
		if (allorothers == "others") {
			for (int i = 0; i < noobs.size(); i++)
			{
				string noobname = *GetName(noobs[i]);
				string fuckname = *GetName(GetLocalPlayer(Players));
				if (noobname != fuckname) {
					thefuckthatwillreturn.push_back(noobname);
				}
			}
		}
	}
	return thefuckthatwillreturn;
}

void init();
void ShowWindowForm();
void ShowLuaCForm();


#define NOOBXPLOIT_CONSOLE_WINDOW      (WM_APP + 500)
#define NOOBXPLOIT_INPUT_FIELD         (WM_APP + 501)
#define NOOBXPLOIT_WAYPOINT_BOX        (WM_APP + 502)

#define MNOOBXPLOIT_EXIT               (WM_APP + 600)
#define MNOOBXPLOIT_MINIMIZE           (WM_APP + 601)
#define MNOOBXPLOIT_LOCALFFBTOOLS      (WM_APP + 602)
#define MNOOBXPLOIT_LOCALFIRE		   (WM_APP + 603)
#define MNOOBXPLOIT_LOCALSPARKLES	   (WM_APP + 604)
#define MNOOBXPLOIT_LOCALPARTICLES	   (WM_APP + 605)
#define MNOOBXPLOIT_LOCALSMOKE		   (WM_APP + 606)
#define MNOOBXPLOIT_LOCALGOD		   (WM_APP + 607)
#define MNOOBXPLOIT_LOCALMOREWS		   (WM_APP + 608)
#define MNOOBXPLOIT_LOCALLESSWS		   (WM_APP + 609)
#define MNOOBXPLOIT_LOCALMOREJP	       (WM_APP + 610)
#define MNOOBXPLOIT_LOCALLESSJP	       (WM_APP + 611)
#define MNOOBXPLOIT_LOCALNOOBHAXHAT	   (WM_APP + 612)
#define MNOOBXPLOIT_LOCALSSJGOD	       (WM_APP + 613)
#define MNOOBXPLOIT_LOCALRAPTP		   (WM_APP + 614)
#define MNOOBXPLOIT_LOCALRAP2TP        (WM_APP + 615)
#define MNOOBXPLOIT_TPOUTOFPRISON	   (WM_APP + 616)
#define MNOOBXPLOIT_MURDERGAMES        (WM_APP + 617)
#define MNOOBXPLOIT_CUSTOMNAME         (WM_APP + 618)
#define MNOOBXPLOIT_CASH			   (WM_APP + 619)
#define MNOOBXPLOIT_PARTICLE		   (WM_APP + 620)
#define MNOOBXPLOIT_PARTICLE1		   (WM_APP + 621)
#define MNOOBXPLOIT_PARTICLE2		   (WM_APP + 622)
#define MNOOBXPLOIT_PARTICLE3		   (WM_APP + 623)
#define MNOOBXPLOIT_PARTICLE4		   (WM_APP + 624)
#define MNOOBXPLOIT_PARTICLE6		   (WM_APP + 625)
#define MNOOBXPLOIT_PARTICLE7		   (WM_APP + 626)
#define MNOOBXPLOIT_PARTICLE8		   (WM_APP + 627)
#define MNOOBXPLOIT_UWRFLAG            (WM_APP + 628)
#define MNOOBXPLOIT_UWBFLAG            (WM_APP + 629)
/*#define MNOOBXPLOIT_JBNODOORS          (WM_APP + 630)
#define MNOOBXPLOIT_JBTP1              (WM_APP + 632)
#define MNOOBXPLOIT_JBTP2              (WM_APP + 633)
#define MNOOBXPLOIT_JBTP3              (WM_APP + 634)
#define MNOOBXPLOIT_JBTP4              (WM_APP + 635)
#define MNOOBXPLOIT_JBTP5              (WM_APP + 636)
#define MNOOBXPLOIT_JBTP6              (WM_APP + 637)
#define MNOOBXPLOIT_JBTP7              (WM_APP + 638)
#define MNOOBXPLOIT_JBTP8              (WM_APP + 639)
#define MNOOBXPLOIT_JBTP9              (WM_APP + 640)
#define MNOOBXPLOIT_JBTP10             (WM_APP + 641)
#define MNOOBXPLOIT_JBTP11             (WM_APP + 642)
#define MNOOBXPLOIT_JBTP12             (WM_APP + 643)
#define MNOOBXPLOIT_JBTP13             (WM_APP + 644)
#define MNOOBXPLOIT_JBTP14             (WM_APP + 645)
#define MNOOBXPLOIT_JBTP15             (WM_APP + 646)
#define MNOOBXPLOIT_JBTP16             (WM_APP + 647)
#define MNOOBXPLOIT_JBTP17             (WM_APP + 648)*/
#define MNOOBXPLOIT_CLICKDEL           (WM_APP + 649)
#define MNOOBXPLOIT_CLICKTP            (WM_APP + 650)
#define MNOOBXPLOIT_CLICKEXPLODE       (WM_APP + 651)
#define MNOOBXPLOIT_CLICKPART          (WM_APP + 652)
#define MNOOBXPLOIT_CLICKUNANCHOR      (WM_APP + 653)
#define MNOOBXPLOIT_CLICKANCHOR        (WM_APP + 654)
#define MNOOBXPLOIT_CLICKLOCK          (WM_APP + 655)
#define MNOOBXPLOIT_CLICKUNLOCK        (WM_APP + 656)
#define MNOOBXPLOIT_CLICKNOTHING       (WM_APP + 657)
#define MNOOBXPLOIT_DONATE             (WM_APP + 658)
#define MNOOBXPLOIT_LUACSAME           (WM_APP + 659)
#define MNOOBXPLOIT_LOCALDOMINUS	   (WM_APP + 660)
#define MNOOBXPLOIT_LOCALMLG     	   (WM_APP + 661)
//#define MNOOBXPLOIT_JBTP18             (WM_APP + 662)
#define MNOOBXPLOIT_LOCALBACON     	   (WM_APP + 663)
#define MNOOBXPLOIT_LOCALSSJHAIR       (WM_APP + 664)
#define MNOOBXPLOIT_LOCALSSJGODHAIR    (WM_APP + 665)
#define MNOOBXPLOIT_NOOBHAXSIGN        (WM_APP + 666)
#define MNOOBXPLOIT_NOOBHAXHP          (WM_APP + 667)
#define MNOOBXPLOIT_SPARKLETIME        (WM_APP + 668)
#define MNOOBXPLOIT_NOOBHAXBC          (WM_APP + 669)
#define MNOOBXPLOIT_NOOBHAXTBC         (WM_APP + 670)
#define MNOOBXPLOIT_NOOBHAXOBC         (WM_APP + 671)
#define MNOOBXPLOIT_CLICKCANCOLLIDEFALSE         (WM_APP + 672)
#define MNOOBXPLOIT_CLICKCANCOLLIDETRUE          (WM_APP + 673)
#define MNOOBXPLOIT_LOCALSSJGODSSJ	   (WM_APP + 674)
#define MNOOBXPLOIT_LOCALSSJGODSSJHAIR (WM_APP + 675)
#define MNOOBXPLOIT_LOCALSSJROSEHAIR   (WM_APP + 676)
#define MNOOBXPLOIT_LOCALSSJROSE	   (WM_APP + 677)
//#define MNOOBXPLOIT_JBTP19			   (WM_APP + 678)
//#define MNOOBXPLOIT_JBTP20			   (WM_APP + 679)
//#define MNOOBXPLOIT_JBTP21			   (WM_APP + 680)
#define MNOOBXPLOIT_PLPOLICEAREA       (WM_APP + 681)
#define MNOOBXPLOIT_PoFTPOBBY       (WM_APP + 682)
#define MNOOBXPLOIT_SPAS12		(WM_APP + 683)
#define MNOOBXPLOIT_M16			(WM_APP + 684)
#define MNOOBXPLOIT_M60			(WM_APP + 685)
#define MNOOBXPLOIT_ACR			(WM_APP + 686)
#define MNOOBXPLOIT_AK47		(WM_APP + 687)
#define MNOOBXPLOIT_FAKEIDCARD	(WM_APP + 688)
#define MNOOBXPLOIT_BERETTAM9	(WM_APP + 689)
#define MNOOBXPLOIT_TASER		(WM_APP + 690)
#define MNOOBXPLOIT_HANDCUFFS	(WM_APP + 691)



#define NOOBXPLOIT_LUAC_WINDOW         (WM_APP + 700)
#define NOOBXPLOIT_LUAC_EXE			   (WM_APP + 701)

#define MNOOBXPLOIT_MIN2               (WM_APP + 800)

#define MNOOBXPLOIT_BMP				   (WM_APP + 900)

bool isrgb(double n) {
	if (n >= 0 && n <= 255) {
		return true;
	}
	else {
		return false;
	}
}



HWND ParentWindow = NULL;
HWND MainWindow = NULL;
HWND LuaCWindow = NULL;
HMENU WindowMenu = NULL;
HMENU WindowMenu2 = NULL;
HMODULE HInstance = NULL;
HMODULE HInstance2 = NULL;

HWND InputField = NULL;

HWND WaypointBox = NULL;

bool ok = false;

HDC hEdit = NULL;

HWND luacbox;

int multi_line(char *sentence)
{
	std::stringstream ss(sentence);
	std::string to;

	if (sentence != NULL)
	{
		if (split2(sentence, '\n').size() != 0) {
			vector<string> nubh4x = split2(sentence, '\n');
			for (int i = 0; i < nubh4x.size(); i++) {
				string msgk = nubh4x[i];
				Execute(msgk, rlua_State);
			}
		}
	}

	return 0;
}

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	VMProtectBeginMutation("noob with a n");
	switch (message)
	{
	case WM_CTLCOLORSTATIC:
	{
		if (hwnd != FindWindowW(NULL, L"Roblox")) {
			hEdit = (HDC)wParam;
			SetTextColor(hEdit, RGB(0, 0, 0));
			SetBkColor((HDC)txtbox, RGB(255, 255, 255));
			return (LRESULT)GetStockObject(WHITE_BRUSH);
		}
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case NOOBXPLOIT_LUAC_EXE:
			if (whitelisted && scanned) {
				TCHAR * buffer = 0;
				int nLen = 0;
				nLen = GetWindowTextLength(luacbox);
				if (nLen > 0)
				{
					buffer = new TCHAR[nLen + 1];
					GetWindowText(luacbox, buffer, nLen + 1);
				}
				char* text = buffer;
				multi_line(text);
			}
			break;
		case MNOOBXPLOIT_EXIT:
			if (MessageBox(hwnd, "Are you sure you want to close Roblox?", "Close Roblox", MB_YESNOCANCEL) == IDYES)
				exit(4);
			break;
		case MNOOBXPLOIT_MINIMIZE:
			ShowWindow(MainWindow, SW_MINIMIZE);
			break;
		case MNOOBXPLOIT_MIN2:
			ShowWindow(LuaCWindow, SW_MINIMIZE);
			break;
		case MNOOBXPLOIT_DONATE:
			if (MessageBox(hwnd, "Thanks for believing in this exploit! Are you sure you want to support it by donating? This won't give you extra whitelists or benefits.", "Any donation is appreciated!", MB_YESNOCANCEL) == IDYES)
				ShellExecute(NULL, "open", "http://paypal.me/supportwiki/", NULL, NULL, SW_SHOWNORMAL);
			break;
		case MNOOBXPLOIT_LUACSAME:
			if (whitelisted && scanned) {
				if (!FindWindowW(NULL, L"noobhax Lua C execution")) {
					ShowLuaCForm();
				}
				else {
					ShowWindow(LuaCWindow, SW_SHOWNORMAL);
				}
			}
			break;
			/*case MNOOBXPLOIT_JBTP17:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 165.2);
			pushnumber(rlua_State, 15.866);
			pushnumber(rlua_State, 1366.599);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP18:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 1310.53955);
			pushnumber(rlua_State, 15.015626);
			pushnumber(rlua_State, 376.857422);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP19:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, -1580.02173);
			pushnumber(rlua_State, 17.8980503);
			pushnumber(rlua_State, 706.622864);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP20:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 265.322113);
			pushnumber(rlua_State, 17.818119);
			pushnumber(rlua_State, -1756.97229);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP21:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 32.5797005);
			pushnumber(rlua_State, 0.605509818);
			pushnumber(rlua_State, 800.982971);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP16:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 722.239);
			pushnumber(rlua_State, 68.692);
			pushnumber(rlua_State, 1128.122);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP15:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, -1171.404);
			pushnumber(rlua_State, 56.1);
			pushnumber(rlua_State, -1571.698);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP14:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 1583.4);
			pushnumber(rlua_State, 48.068);
			pushnumber(rlua_State, -1756.803);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP13:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 154.2);
			pushnumber(rlua_State, 15.868);
			pushnumber(rlua_State, 774.2);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP12:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, -1545.899);
			pushnumber(rlua_State, 16.398);
			pushnumber(rlua_State, 691.701);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP11:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 752.824);
			pushnumber(rlua_State, 16.068);
			pushnumber(rlua_State, -1243.077);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP10:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, -1354.001);
			pushnumber(rlua_State, 17);
			pushnumber(rlua_State, -1534.399);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP9:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 129.654);
			pushnumber(rlua_State, 17.616);
			pushnumber(rlua_State, 1331.293);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP8:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 1741.512);
			pushnumber(rlua_State, 48.014);
			pushnumber(rlua_State, -1730.414);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP7:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 744.981506);
			pushnumber(rlua_State, 38.1523933);
			pushnumber(rlua_State, 1132.73462);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP6:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, -1130.11426);
			pushnumber(rlua_State, 17.950058);
			pushnumber(rlua_State, -1593.09766);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP5:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, -1100.4469);
			pushnumber(rlua_State, 17.7500038);
			pushnumber(rlua_State, -1493.18286);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP4:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, 1.1110189);
			pushnumber(rlua_State, 17.9213142);
			pushnumber(rlua_State, 782.103455);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP3:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, -1176.4231);
			pushnumber(rlua_State, 59.7911148);
			pushnumber(rlua_State, -1572.6908);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP2:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, -342.130798);
			pushnumber(rlua_State, 19.3135223);
			pushnumber(rlua_State, 1182.73669);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBTP1:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getglobal(rlua_State, "CFrame");
			getfield(rlua_State, -1, "new");
			pushnumber(rlua_State, -222.339157);
			pushnumber(rlua_State, 17.9125443);
			pushnumber(rlua_State, 1575.82336);
			pcall(rlua_State, 3, 1, 0);
			setfield(rlua_State, -3, "CFrame");
			settop(rlua_State, 0);
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;
			case MNOOBXPLOIT_JBNODOORS:
			if (whitelisted && scanned) {
			try {
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			int placeid = rlua_tonumber(rlua_State, -1);
			settop(rlua_State, 0);
			if (placeid == 606849621) {
			try {
			if (removeddoors) {
			Output(txtbox, "You've already removed the doors\n");
			}
			else {
			getglobal(rlua_State, "workspace");
			getfield(rlua_State, -1, "Doors");
			getfield(rlua_State, -1, "remove");
			pushvalue(rlua_State, -2);
			pcall(rlua_State, 1, 0, 0);
			settop(rlua_State, 0);
			removeddoors = 1;
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			else {
			Output(txtbox, "You aren't on Jailbreak (Beta) made by Badimo\n");
			}
			}
			catch (exception) {
			Sleep(1);
			}
			}
			break;*/
		case MNOOBXPLOIT_CASH:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "Buy");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pushnumber(rlua_State, -1000000);
							pcall(rlua_State, 2, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_CUSTOMNAME:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Name");
							string okhi = rlua_tolstring(rlua_State, -1, NULL);
							settop(rlua_State, 0);
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "GuiEvent");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, (okhi + "\nnoobhax buyer\n(created by supersupersnivy97 @V3rmillion)").c_str());
							pcall(rlua_State, 2, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PARTICLE:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "GiveParticle");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PARTICLE1:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "GiveParticle1");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PARTICLE2:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "GiveParticle2");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PARTICLE3:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "GiveParticle3");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PARTICLE4:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "GiveParticle4");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PARTICLE6:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "GiveParticle6");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PARTICLE7:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "GiveParticle7");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PARTICLE8:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 268066633) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "GiveParticle8");
							getfield(rlua_State, -1, "FireServer");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Boys And Girls Hangout made by DemSkittlesDoee\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_MURDERGAMES:
			if (whitelisted && scanned) {
				try {
					vector<string> murderers;
					vector<string> sheriffs;
					vector<int> Playersok = GetChildren(Players);
					for (int i = 0; i < Playersok.size(); i++) {
						string name = *GetName(Playersok[i]);
						std::this_thread::sleep_for(100ms);
						getglobal(rlua_State, "game");
						getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
						getfield(rlua_State, -1, name.c_str());
						getfield(rlua_State, -1, "Backpack");
						getfield(rlua_State, -1, "FindFirstChild");
						pushvalue(rlua_State, -2);
						pushstring(rlua_State, "Knife");
						pcall(rlua_State, 2, 1, 0);
						bool Knife = rlua_toboolean(rlua_State, -1);
						settop(rlua_State, 0);
						getglobal(rlua_State, "game");
						getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
						getfield(rlua_State, -1, name.c_str());
						getfield(rlua_State, -1, "Backpack");
						getfield(rlua_State, -1, "FindFirstChild");
						pushvalue(rlua_State, -2);
						pushstring(rlua_State, "Revolver");
						pcall(rlua_State, 2, 1, 0);
						bool Revolver = rlua_toboolean(rlua_State, -1);
						settop(rlua_State, 0);
						if (Knife) {
							murderers.push_back(name);
						}
						if (Revolver) {
							sheriffs.push_back(name);
						}

					}
					if (murderers.size() == 0) {
						Output(txtbox, "Murderer not found\n");
					}
					else {
						for (int i = 0; i < murderers.size(); i++) {
							Output(txtbox, murderers[i] + " is a murderer\n");
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, murderers[i].c_str());
							getfield(rlua_State, -1, "Character");
							getglobal(rlua_State, "Instance");
							getfield(rlua_State, -1, "new");
							pushstring(rlua_State, "SelectionBox");
							pushvalue(rlua_State, -4);
							pcall(rlua_State, 2, 1, 0);
							pushstring(rlua_State, "murderer_noobhax_ok");
							setfield(rlua_State, -2, "Name");
							settop(rlua_State, 0);
							std::this_thread::sleep_for(100ms);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, murderers[i].c_str());
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "murderer_noobhax_ok");
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, murderers[i].c_str());
							getfield(rlua_State, -1, "Character");
							setfield(rlua_State, -5, "Adornee");
							settop(rlua_State, 0);
							std::this_thread::sleep_for(100ms);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, murderers[i].c_str());
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "murderer_noobhax_ok");
							getglobal(rlua_State, "BrickColor");
							getfield(rlua_State, -1, "new");
							pushstring(rlua_State, "Bright red");
							pcall(rlua_State, 1, 1, 0);
							setfield(rlua_State, -3, "Color");
							settop(rlua_State, 0);
							Output(txtbox, "Gave " + murderers[i] + " a red SelectionBox\n");
						}
					}
					if (sheriffs.size() == 0) {
						Output(txtbox, "Sheriff not found\n");
					}
					else {
						for (int i = 0; i < sheriffs.size(); i++) {
							Output(txtbox, sheriffs[i] + " is a sheriff\n");
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, sheriffs[i].c_str());
							getfield(rlua_State, -1, "Character");
							getglobal(rlua_State, "Instance");
							getfield(rlua_State, -1, "new");
							pushstring(rlua_State, "SelectionBox");
							pushvalue(rlua_State, -4);
							pcall(rlua_State, 2, 1, 0);
							pushstring(rlua_State, "sheriffs_noobhax_ok");
							setfield(rlua_State, -2, "Name");
							settop(rlua_State, 0);
							std::this_thread::sleep_for(100ms);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, sheriffs[i].c_str());
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "sheriffs_noobhax_ok");
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, sheriffs[i].c_str());
							getfield(rlua_State, -1, "Character");
							setfield(rlua_State, -5, "Adornee");
							settop(rlua_State, 0);
							std::this_thread::sleep_for(100ms);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, sheriffs[i].c_str());
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "sheriffs_noobhax_ok");
							getglobal(rlua_State, "BrickColor");
							getfield(rlua_State, -1, "new");
							pushstring(rlua_State, "Bright green");
							pcall(rlua_State, 1, 1, 0);
							setfield(rlua_State, -3, "Color");
							settop(rlua_State, 0);
							Output(txtbox, "Gave " + sheriffs[i] + " a green SelectionBox\n");
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_TPOUTOFPRISON:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 155615604) {
						try {
							getglobal(rlua_State, "Vector3");
							getfield(rlua_State, -1, "new");
							pushnumber(rlua_State, 456);
							pushnumber(rlua_State, 98);
							pushnumber(rlua_State, 2216);
							pcall(rlua_State, 3, 1, 0);
							getglobal(rlua_State, "CFrame");
							getfield(rlua_State, -1, "new");
							pushvalue(rlua_State, -3);
							pcall(rlua_State, 1, 1, 0);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "HumanoidRootPart");
							pushvalue(rlua_State, -4);
							setfield(rlua_State, -2, "CFrame");
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Prison Life v2.0\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PLPOLICEAREA:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 155615604) {
						try {
							getglobal(rlua_State, "Vector3");
							getfield(rlua_State, -1, "new");
							pushnumber(rlua_State, 834.258789);
							pushnumber(rlua_State, 99.9908752);
							pushnumber(rlua_State, 2337.10327);
							pcall(rlua_State, 3, 1, 0);
							getglobal(rlua_State, "CFrame");
							getfield(rlua_State, -1, "new");
							pushvalue(rlua_State, -3);
							pcall(rlua_State, 1, 1, 0);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "HumanoidRootPart");
							pushvalue(rlua_State, -4);
							setfield(rlua_State, -2, "CFrame");
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Prison Life v2.0\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_PoFTPOBBY:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 564086481) {
						try {
							getglobal(rlua_State, "Vector3");
							getfield(rlua_State, -1, "new");
							pushnumber(rlua_State, -275);
							pushnumber(rlua_State, 11.5);
							pushnumber(rlua_State, -40);
							pcall(rlua_State, 3, 1, 0);
							getglobal(rlua_State, "CFrame");
							getfield(rlua_State, -1, "new");
							pushvalue(rlua_State, -3);
							pcall(rlua_State, 1, 1, 0);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "HumanoidRootPart");
							pushvalue(rlua_State, -4);
							setfield(rlua_State, -2, "CFrame");
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Plates of Fate: Mayhem (by DylanBuildar)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_SPAS12:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 402122991) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "resources");
							getfield(rlua_State, -1, "RemoteFunction");
							getfield(rlua_State, -1, "InvokeServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "giveItem");
							pushstring(rlua_State, "SPAS-12");
							pcall(rlua_State, 3, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Redwood Prison (by RoyStanford)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_M16:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 402122991) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "resources");
							getfield(rlua_State, -1, "RemoteFunction");
							getfield(rlua_State, -1, "InvokeServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "giveItem");
							pushstring(rlua_State, "M16");
							pcall(rlua_State, 3, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Redwood Prison (by RoyStanford)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_M60:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 402122991) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "resources");
							getfield(rlua_State, -1, "RemoteFunction");
							getfield(rlua_State, -1, "InvokeServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "giveItem");
							pushstring(rlua_State, "M60");
							pcall(rlua_State, 3, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Redwood Prison (by RoyStanford)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_ACR:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 402122991) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "resources");
							getfield(rlua_State, -1, "RemoteFunction");
							getfield(rlua_State, -1, "InvokeServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "giveItem");
							pushstring(rlua_State, "ACR");
							pcall(rlua_State, 3, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Redwood Prison (by RoyStanford)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_AK47:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 402122991) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "resources");
							getfield(rlua_State, -1, "RemoteFunction");
							getfield(rlua_State, -1, "InvokeServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "giveItem");
							pushstring(rlua_State, "AK47");
							pcall(rlua_State, 3, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Redwood Prison (by RoyStanford)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_FAKEIDCARD:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 402122991) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "resources");
							getfield(rlua_State, -1, "RemoteFunction");
							getfield(rlua_State, -1, "InvokeServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "giveItem");
							pushstring(rlua_State, "Fake ID Card");
							pcall(rlua_State, 3, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Redwood Prison (by RoyStanford)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_BERETTAM9:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 402122991) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "resources");
							getfield(rlua_State, -1, "RemoteFunction");
							getfield(rlua_State, -1, "InvokeServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "giveItem");
							pushstring(rlua_State, "Beretta M9");
							pcall(rlua_State, 3, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Redwood Prison (by RoyStanford)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_TASER:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 402122991) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "resources");
							getfield(rlua_State, -1, "RemoteFunction");
							getfield(rlua_State, -1, "InvokeServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "giveItem");
							pushstring(rlua_State, "Taser");
							pcall(rlua_State, 3, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Redwood Prison (by RoyStanford)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_HANDCUFFS:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 402122991) {
						try {
							getglobal(rlua_State, "workspace");
							getfield(rlua_State, -1, "resources");
							getfield(rlua_State, -1, "RemoteFunction");
							getfield(rlua_State, -1, "InvokeServer");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "giveItem");
							pushstring(rlua_State, "Handcuffs");
							pcall(rlua_State, 3, 0, 0);
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Redwood Prison (by RoyStanford)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_UWBFLAG:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 189632) {
						try {
							getglobal(rlua_State, "Vector3");
							getfield(rlua_State, -1, "new");
							pushnumber(rlua_State, 6.87806749);
							pushnumber(rlua_State, 18);
							pushnumber(rlua_State, -74.1744308);
							pcall(rlua_State, 3, 1, 0);
							getglobal(rlua_State, "CFrame");
							getfield(rlua_State, -1, "new");
							pushvalue(rlua_State, -3);
							pcall(rlua_State, 1, 1, 0);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "HumanoidRootPart");
							pushvalue(rlua_State, -4);
							setfield(rlua_State, -2, "CFrame");
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on The Underground War\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_UWRFLAG:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 189632) {
						try {
							getglobal(rlua_State, "Vector3");
							getfield(rlua_State, -1, "new");
							pushnumber(rlua_State, -7.32489729);
							pushnumber(rlua_State, 18);
							pushnumber(rlua_State, 76.006897);
							pcall(rlua_State, 3, 1, 0);
							getglobal(rlua_State, "CFrame");
							getfield(rlua_State, -1, "new");
							pushvalue(rlua_State, -3);
							pcall(rlua_State, 1, 1, 0);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "HumanoidRootPart");
							pushvalue(rlua_State, -4);
							setfield(rlua_State, -2, "CFrame");
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on The Underground War\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALRAP2TP:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 1125790688) {
						try {
							getglobal(rlua_State, "Vector3");
							getfield(rlua_State, -1, "new");
							pushnumber(rlua_State, -3.78023744);
							pushnumber(rlua_State, 6.59998131);
							pushnumber(rlua_State, 55.9015732);
							pcall(rlua_State, 3, 1, 0);
							getglobal(rlua_State, "CFrame");
							getfield(rlua_State, -1, "new");
							pushvalue(rlua_State, -3);
							pcall(rlua_State, 1, 1, 0);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "HumanoidRootPart");
							pushvalue(rlua_State, -4);
							setfield(rlua_State, -2, "CFrame");
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Auto Rap Battles (PlaceId 1125790688)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALRAPTP:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 1125790688) {
						try {
							getglobal(rlua_State, "Vector3");
							getfield(rlua_State, -1, "new");
							pushnumber(rlua_State, -15.1639919);
							pushnumber(rlua_State, 7.46997643);
							pushnumber(rlua_State, 55.9096069);
							pcall(rlua_State, 3, 1, 0);
							getglobal(rlua_State, "CFrame");
							getfield(rlua_State, -1, "new");
							pushvalue(rlua_State, -3);
							pcall(rlua_State, 1, 1, 0);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "HumanoidRootPart");
							pushvalue(rlua_State, -4);
							setfield(rlua_State, -2, "CFrame");
							settop(rlua_State, 0);
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Auto Rap Battles (PlaceId 1125790688)\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALSSJGOD:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Torso");
					getfield(rlua_State, -1, "FindFirstChild");
					pushvalue(rlua_State, -2);
					pushstring(rlua_State, "ssjgod_noobhax");
					pcall(rlua_State, 2, 1, 0);
					bool isssjgod = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (isssjgod == false) {
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getglobal(rlua_State, "Instance");
						getfield(rlua_State, -1, "new");
						pushstring(rlua_State, "ParticleEmitter");
						pushvalue(rlua_State, -4);
						pcall(rlua_State, 2, 1, 0);
						pushstring(rlua_State, "ssjgod_noobhax");
						setfield(rlua_State, -2, "Name");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjgod_noobhax");
						getglobal(rlua_State, "NumberSequence");
						getfield(rlua_State, -1, "new");
						pushnumber(rlua_State, 15);
						pcall(rlua_State, 1, 1, 0);
						setfield(rlua_State, -3, "Size");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjgod_noobhax");
						pushstring(rlua_State, "rbxassetid://288406230");
						setfield(rlua_State, -2, "Texture");
						pushnumber(rlua_State, 2);
						setfield(rlua_State, -2, "Rate");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjgod_noobhax");
						getglobal(rlua_State, "NumberRange");
						getfield(rlua_State, -1, "new");
						pushnumber(rlua_State, 5);
						pcall(rlua_State, 1, 1, 0);
						setfield(rlua_State, -3, "Speed");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjgod_noobhax");
						pushnumber(rlua_State, 20);
						setfield(rlua_State, -2, "VelocitySpread");
						settop(rlua_State, 0);
					}
					else {
						try {
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "Name");
							string charnamelolkk = rlua_tolstring(rlua_State, -1, NULL);
							settop(rlua_State, 0);
							int character = FindFirstChild_PartOf(Workspace, charnamelolkk);
							int torso = FindFirstChild_PartOf(character, "Torso");
							vector<int> fuckingtorsochildren = GetChildren(torso);
							for (int i = 0; i < fuckingtorsochildren.size(); i++) {
								try {
									if (*GetName(fuckingtorsochildren[i]) == "ssjgod_noobhax") {
										getglobal(rlua_State, "noobhaxmyplr");
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Torso");
										getfield(rlua_State, -1, "ssjgod_noobhax");
										getfield(rlua_State, -1, "remove");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
									}
									else {
										Sleep(1);
									}
								}
								catch (exception) {
									Sleep(1);
								}
							}
							try {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ParticleEmitter");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "ssjgod_noobhax");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjgod_noobhax");
								getglobal(rlua_State, "NumberSequence");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 1, 1, 0);
								setfield(rlua_State, -3, "Size");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjgod_noobhax");
								pushstring(rlua_State, "rbxassetid://288406230");
								setfield(rlua_State, -2, "Texture");
								pushnumber(rlua_State, 2);
								setfield(rlua_State, -2, "Rate");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjgod_noobhax");
								getglobal(rlua_State, "NumberRange");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 5);
								pcall(rlua_State, 1, 1, 0);
								setfield(rlua_State, -3, "Speed");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjgod_noobhax");
								pushnumber(rlua_State, 20);
								setfield(rlua_State, -2, "VelocitySpread");
								settop(rlua_State, 0);
							}
							catch (exception) {
								Sleep(1);
							}
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALSSJGODSSJ:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Torso");
					getfield(rlua_State, -1, "FindFirstChild");
					pushvalue(rlua_State, -2);
					pushstring(rlua_State, "ssjgodssj_noobhax");
					pcall(rlua_State, 2, 1, 0);
					bool isssjgodssj = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (isssjgodssj == false) {
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getglobal(rlua_State, "Instance");
						getfield(rlua_State, -1, "new");
						pushstring(rlua_State, "ParticleEmitter");
						pushvalue(rlua_State, -4);
						pcall(rlua_State, 2, 1, 0);
						pushstring(rlua_State, "ssjgodssj_noobhax");
						setfield(rlua_State, -2, "Name");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjgodssj_noobhax");
						getglobal(rlua_State, "NumberSequence");
						getfield(rlua_State, -1, "new");
						pushnumber(rlua_State, 15);
						pcall(rlua_State, 1, 1, 0);
						setfield(rlua_State, -3, "Size");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjgodssj_noobhax");
						pushstring(rlua_State, "rbxassetid://1016161318");
						setfield(rlua_State, -2, "Texture");
						pushnumber(rlua_State, 2);
						setfield(rlua_State, -2, "Rate");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjgodssj_noobhax");
						getglobal(rlua_State, "NumberRange");
						getfield(rlua_State, -1, "new");
						pushnumber(rlua_State, 5);
						pcall(rlua_State, 1, 1, 0);
						setfield(rlua_State, -3, "Speed");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjgodssj_noobhax");
						pushnumber(rlua_State, 20);
						setfield(rlua_State, -2, "VelocitySpread");
						settop(rlua_State, 0);
					}
					else {
						try {
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "Name");
							string charnamelolkk = rlua_tolstring(rlua_State, -1, NULL);
							settop(rlua_State, 0);
							int character = FindFirstChild_PartOf(Workspace, charnamelolkk);
							int torso = FindFirstChild_PartOf(character, "Torso");
							vector<int> fuckingtorsochildren = GetChildren(torso);
							for (int i = 0; i < fuckingtorsochildren.size(); i++) {
								try {
									if (*GetName(fuckingtorsochildren[i]) == "ssjgodssj_noobhax") {
										getglobal(rlua_State, "noobhaxmyplr");
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Torso");
										getfield(rlua_State, -1, "ssjgodssj_noobhax");
										getfield(rlua_State, -1, "remove");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
									}
									else {
										Sleep(1);
									}
								}
								catch (exception) {
									Sleep(1);
								}
							}
							try {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ParticleEmitter");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "ssjgodssj_noobhax");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjgodssj_noobhax");
								getglobal(rlua_State, "NumberSequence");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 1, 1, 0);
								setfield(rlua_State, -3, "Size");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjgodssj_noobhax");
								pushstring(rlua_State, "rbxassetid://1016161318");
								setfield(rlua_State, -2, "Texture");
								pushnumber(rlua_State, 2);
								setfield(rlua_State, -2, "Rate");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjgodssj_noobhax");
								getglobal(rlua_State, "NumberRange");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 5);
								pcall(rlua_State, 1, 1, 0);
								setfield(rlua_State, -3, "Speed");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjgodssj_noobhax");
								pushnumber(rlua_State, 20);
								setfield(rlua_State, -2, "VelocitySpread");
								settop(rlua_State, 0);
							}
							catch (exception) {
								Sleep(1);
							}
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALSSJROSE:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Torso");
					getfield(rlua_State, -1, "FindFirstChild");
					pushvalue(rlua_State, -2);
					pushstring(rlua_State, "ssjrose_noobhax");
					pcall(rlua_State, 2, 1, 0);
					bool isssjrose = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (isssjrose == false) {
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getglobal(rlua_State, "Instance");
						getfield(rlua_State, -1, "new");
						pushstring(rlua_State, "ParticleEmitter");
						pushvalue(rlua_State, -4);
						pcall(rlua_State, 2, 1, 0);
						pushstring(rlua_State, "ssjrose_noobhax");
						setfield(rlua_State, -2, "Name");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjrose_noobhax");
						getglobal(rlua_State, "NumberSequence");
						getfield(rlua_State, -1, "new");
						pushnumber(rlua_State, 15);
						pcall(rlua_State, 1, 1, 0);
						setfield(rlua_State, -3, "Size");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjrose_noobhax");
						pushstring(rlua_State, "rbxassetid://1016284374");
						setfield(rlua_State, -2, "Texture");
						pushnumber(rlua_State, 2);
						setfield(rlua_State, -2, "Rate");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjrose_noobhax");
						getglobal(rlua_State, "NumberRange");
						getfield(rlua_State, -1, "new");
						pushnumber(rlua_State, 5);
						pcall(rlua_State, 1, 1, 0);
						setfield(rlua_State, -3, "Speed");
						settop(rlua_State, 0);
						getglobal(rlua_State, "noobhaxmyplr");
						getfield(rlua_State, -1, "Character");
						getfield(rlua_State, -1, "Torso");
						getfield(rlua_State, -1, "ssjrose_noobhax");
						pushnumber(rlua_State, 20);
						setfield(rlua_State, -2, "VelocitySpread");
						settop(rlua_State, 0);
					}
					else {
						try {
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "Name");
							string charnamelolkk = rlua_tolstring(rlua_State, -1, NULL);
							settop(rlua_State, 0);
							int character = FindFirstChild_PartOf(Workspace, charnamelolkk);
							int torso = FindFirstChild_PartOf(character, "Torso");
							vector<int> fuckingtorsochildren = GetChildren(torso);
							for (int i = 0; i < fuckingtorsochildren.size(); i++) {
								try {
									if (*GetName(fuckingtorsochildren[i]) == "ssjrose_noobhax") {
										getglobal(rlua_State, "noobhaxmyplr");
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Torso");
										getfield(rlua_State, -1, "ssjrose_noobhax");
										getfield(rlua_State, -1, "remove");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
									}
									else {
										Sleep(1);
									}
								}
								catch (exception) {
									Sleep(1);
								}
							}
							try {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ParticleEmitter");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "ssjrose_noobhax");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjrose_noobhax");
								getglobal(rlua_State, "NumberSequence");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 1, 1, 0);
								setfield(rlua_State, -3, "Size");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjrose_noobhax");
								pushstring(rlua_State, "rbxassetid://1016284374");
								setfield(rlua_State, -2, "Texture");
								pushnumber(rlua_State, 2);
								setfield(rlua_State, -2, "Rate");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjrose_noobhax");
								getglobal(rlua_State, "NumberRange");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 5);
								pcall(rlua_State, 1, 1, 0);
								setfield(rlua_State, -3, "Speed");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "ssjrose_noobhax");
								pushnumber(rlua_State, 20);
								setfield(rlua_State, -2, "VelocitySpread");
								settop(rlua_State, 0);
							}
							catch (exception) {
								Sleep(1);
							}
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALNOOBHAXHAT:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "noobhax_cap");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "noobhax_cap");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0.2);
					pushnumber(rlua_State, 0.2);
					pushnumber(rlua_State, 0.2);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "noobhax_cap");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://16190466");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://580675160");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "noobhax_cap");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALDOMINUS:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_dominus");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_dominus");
					pushstring(rlua_State, "noobhax_dominus");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_dominus");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, 0.4);
					pushnumber(rlua_State, 0);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_dominus");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_dominus");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://21057410");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://48544900");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_dominus");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALMLG:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_mlg");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_mlg");
					pushstring(rlua_State, "noobhax_mlg");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_mlg");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, 0.4);
					pushnumber(rlua_State, 0.08);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_mlg");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0.2);
					pushnumber(rlua_State, 0.2);
					pushnumber(rlua_State, 0.2);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_mlg");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://121910245");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://182196075");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_mlg");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALBACON:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_bacon");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_bacon");
					pushstring(rlua_State, "noobhax_bacon");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_bacon");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, 0.25);
					pushnumber(rlua_State, 0.25);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_bacon");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 1);
					pushnumber(rlua_State, 1);
					pushnumber(rlua_State, 2);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_bacon");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://83293901");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://83284747");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_bacon");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALSSJGODHAIR:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_ssjgod");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssjgod");
					pushstring(rlua_State, "noobhax_ssjgod");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssjgod");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.1);
					pushnumber(rlua_State, 0);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_ssjgod");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_handlek");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek");
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek");
					pushstring(rlua_State, "Neon");
					setfield(rlua_State, -2, "Material");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek");
					getglobal(rlua_State, "BrickColor");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Really red");
					pcall(rlua_State, 1, 1, 0);
					setfield(rlua_State, -3, "BrickColor");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					getglobal(rlua_State, "noob_handlek");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_meshk");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk");
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk");
					pushstring(rlua_State, "rbxassetid://582320218");
					setfield(rlua_State, -2, "MeshId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 5.5);
					pushnumber(rlua_State, 5.5);
					pushnumber(rlua_State, 5.5);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Scale");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.067);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Offset");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssjgod");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALSSJROSEHAIR:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_ssjrose");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssjrose");
					pushstring(rlua_State, "noobhax_ssjrose");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssjrose");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.1);
					pushnumber(rlua_State, 0);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_ssjrose");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_handlek01");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek01");
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek01");
					pushstring(rlua_State, "Neon");
					setfield(rlua_State, -2, "Material");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek01");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek01");
					getglobal(rlua_State, "BrickColor");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Carnation pink");
					pcall(rlua_State, 1, 1, 0);
					setfield(rlua_State, -3, "BrickColor");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek01");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					getglobal(rlua_State, "noob_handlek01");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_meshk01");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk01");
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk01");
					pushstring(rlua_State, "rbxassetid://582320218");
					setfield(rlua_State, -2, "MeshId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk01");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk01");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 5.5);
					pushnumber(rlua_State, 5.5);
					pushnumber(rlua_State, 5.5);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Scale");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk01");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.067);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Offset");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssjrose");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALSSJGODSSJHAIR:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_ssjgodssj");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssjgodssj");
					pushstring(rlua_State, "noobhax_ssjgodssj");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssjgodssj");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.1);
					pushnumber(rlua_State, 0);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_ssjgodssj");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_handlek97");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek97");
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek97");
					pushstring(rlua_State, "Neon");
					setfield(rlua_State, -2, "Material");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek97");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek97");
					getglobal(rlua_State, "BrickColor");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Toothpaste");
					pcall(rlua_State, 1, 1, 0);
					setfield(rlua_State, -3, "BrickColor");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek97");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					getglobal(rlua_State, "noob_handlek97");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_meshk97");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk97");
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk97");
					pushstring(rlua_State, "rbxassetid://582320218");
					setfield(rlua_State, -2, "MeshId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk97");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk97");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 5.5);
					pushnumber(rlua_State, 5.5);
					pushnumber(rlua_State, 5.5);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Scale");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk97");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.067);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Offset");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssjgodssj");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_NOOBHAXSIGN:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_sign");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_sign");
					pushstring(rlua_State, "noobhax_sign");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_sign");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.75);
					pushnumber(rlua_State, 0);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_sign");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 1.6);
					pushnumber(rlua_State, 1);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_sign");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://1037158");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://961626085");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_sign");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_NOOBHAXTBC:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_tbc");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_tbc");
					pushstring(rlua_State, "noobhax_tbc");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_tbc");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.25);
					pushnumber(rlua_State, 0.15);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_tbc");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 1);
					pushnumber(rlua_State, 0.8);
					pushnumber(rlua_State, 1);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_tbc");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://1073659");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://11844713");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_tbc");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_NOOBHAXOBC:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_obc");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_obc");
					pushstring(rlua_State, "noobhax_obc");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_obc");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.25);
					pushnumber(rlua_State, 0.15);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_obc");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 1);
					pushnumber(rlua_State, 0.8);
					pushnumber(rlua_State, 1);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_obc");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://1073659");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://17169223");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_obc");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_NOOBHAXBC:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_bc");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_bc");
					pushstring(rlua_State, "noobhax_bc");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_bc");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.25);
					pushnumber(rlua_State, 0.15);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_bc");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 1);
					pushnumber(rlua_State, 0.8);
					pushnumber(rlua_State, 1);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_bc");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://1073659");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://112346870");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_bc");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_NOOBHAXHP:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_headphones");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_headphones");
					pushstring(rlua_State, "noobhax_headphones");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_headphones");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, 0.3);
					pushnumber(rlua_State, 0);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_headphones");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 1);
					pushnumber(rlua_State, 1);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_headphones");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://1051545");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://1051546");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_headphones");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_SPARKLETIME:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_red_sparkle_time_fedora");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_red_sparkle_time_fedora");
					pushstring(rlua_State, "noobhax_red_sparkle_time_fedora");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_red_sparkle_time_fedora");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.1);
					pushnumber(rlua_State, 0.05);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_red_sparkle_time_fedora");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 1);
					pushnumber(rlua_State, 0.4);
					pushnumber(rlua_State, 1);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_red_sparkle_time_fedora");
					getfield(rlua_State, -1, "Handle");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, "rbxassetid://1285237");
					setfield(rlua_State, -2, "MeshId");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					pushstring(rlua_State, "rbxassetid://67996500");
					setfield(rlua_State, -2, "TextureId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_red_sparkle_time_fedora");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALSSJHAIR:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Hat");
					getglobal(rlua_State, "noobhaxmyplr");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_ssj");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssj");
					pushstring(rlua_State, "noobhax_ssj");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssj");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -0.1);
					pushnumber(rlua_State, 0);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "AttachmentPos");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "noob_ssj");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_handlek2");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek2");
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek2");
					pushstring(rlua_State, "Neon");
					setfield(rlua_State, -2, "Material");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek2");
					pushstring(rlua_State, "Block");
					setfield(rlua_State, -2, "Shape");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek2");
					getglobal(rlua_State, "BrickColor");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "New Yeller");
					pcall(rlua_State, 1, 1, 0);
					setfield(rlua_State, -3, "BrickColor");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_handlek2");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pushnumber(rlua_State, 2);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "SpecialMesh");
					getglobal(rlua_State, "noob_handlek2");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noob_meshk2");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk2");
					pushstring(rlua_State, "Mesh");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk2");
					pushstring(rlua_State, "rbxassetid://430344159");
					setfield(rlua_State, -2, "MeshId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk2");
					pushstring(rlua_State, "FileMesh");
					setfield(rlua_State, -2, "MeshType");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk2");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 5.5);
					pushnumber(rlua_State, 5.5);
					pushnumber(rlua_State, 5.5);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Scale");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_meshk2");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, -0.05);
					pushnumber(rlua_State, 0.45);
					pushnumber(rlua_State, 0.5);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Offset");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noob_ssj");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALLESSJP:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Humanoid");
					getfield(rlua_State, -1, "JumpPower");
					double currentjp = rlua_tonumber(rlua_State, -1);
					double newjp = currentjp - 50;
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Humanoid");
					pushnumber(rlua_State, newjp);
					setfield(rlua_State, -2, "JumpPower");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALMOREJP:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Humanoid");
					getfield(rlua_State, -1, "JumpPower");
					double currentjp = rlua_tonumber(rlua_State, -1);
					double newjp = currentjp + 50;
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Humanoid");
					pushnumber(rlua_State, newjp);
					setfield(rlua_State, -2, "JumpPower");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALLESSWS:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Humanoid");
					getfield(rlua_State, -1, "WalkSpeed");
					double currentws = rlua_tonumber(rlua_State, -1);
					double newws = currentws - 16;
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Humanoid");
					pushnumber(rlua_State, newws);
					setfield(rlua_State, -2, "WalkSpeed");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALMOREWS:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Humanoid");
					getfield(rlua_State, -1, "WalkSpeed");
					double currentws = rlua_tonumber(rlua_State, -1);
					double newws = currentws + 16;
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Humanoid");
					pushnumber(rlua_State, newws);
					setfield(rlua_State, -2, "WalkSpeed");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_CLICKEXPLODE:
			if (whitelisted && scanned) {
				if (clickexplodebool) {
					Output(txtbox, "CTRL + click to explode already enabled\n");
				}
				else {
					clickexplodebool = true;
					clickdelbool = false;
					clicktpbool = false;
					clickpartbool = false;
					clickunanchorbool = false;
					clickanchorbool = false;
					clicklockbool = false;
					clickunlockbool = false;
					clickcancollidefalsebool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Enabled CTRL + click to explode\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKANCHOR:
			if (whitelisted && scanned) {
				if (clickanchorbool) {
					Output(txtbox, "CTRL + click to anchor already enabled\n");
				}
				else {
					clickanchorbool = true;
					clickdelbool = false;
					clicktpbool = false;
					clickpartbool = false;
					clickexplodebool = false;
					clickunanchorbool = false;
					clicklockbool = false;
					clickunlockbool = false;
					clickcancollidefalsebool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Enabled CTRL + click to anchor\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKNOTHING:
			if (whitelisted && scanned) {
				if (!clickanchorbool && !clickdelbool && !clicktpbool && !clickpartbool && !clickexplodebool && !clickunanchorbool && !clickanchorbool && !clicklockbool && !clickunlockbool && !clickcancollidefalsebool && !clickcancollidetruebool) {
					Output(txtbox, "CTRL + click commands already disabled\n");
				}
				else {
					clickanchorbool = false;
					clickdelbool = false;
					clicktpbool = false;
					clickpartbool = false;
					clickexplodebool = false;
					clickunanchorbool = false;
					clicklockbool = false;
					clickunlockbool = false;
					clickcancollidefalsebool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Disabled CTRL + click commands\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKUNANCHOR:
			if (whitelisted && scanned) {
				if (clickunanchorbool) {
					Output(txtbox, "CTRL + click to unanchor already enabled\n");
				}
				else {
					clickunanchorbool = true;
					clickdelbool = false;
					clicktpbool = false;
					clickpartbool = false;
					clickexplodebool = false;
					clickanchorbool = false;
					clicklockbool = false;
					clickunlockbool = false;
					clickcancollidefalsebool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Enabled CTRL + click to unanchor\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKPART:
			if (whitelisted && scanned) {
				if (clickpartbool) {
					Output(txtbox, "CTRL + click to make part already enabled\n");
				}
				else {
					clickpartbool = true;
					clicktpbool = false;
					clickexplodebool = false;
					clickdelbool = false;
					clickunanchorbool = false;
					clickanchorbool = false;
					clicklockbool = false;
					clickunlockbool = false;
					clickcancollidefalsebool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Enabled CTRL + click to make part\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKDEL:
			if (whitelisted && scanned) {
				if (clickdelbool) {
					Output(txtbox, "CTRL + click to delete already enabled\n");
				}
				else {
					clickdelbool = true;
					clicktpbool = false;
					clickexplodebool = false;
					clickpartbool = false;
					clickunanchorbool = false;
					clickanchorbool = false;
					clicklockbool = false;
					clickunlockbool = false;
					clickcancollidefalsebool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Enabled CTRL + click to delete\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKTP:
			if (whitelisted && scanned) {
				if (clicktpbool) {
					Output(txtbox, "CTRL + click to tp already enabled\n");
				}
				else {
					clicktpbool = true;
					clickdelbool = false;
					clickexplodebool = false;
					clickpartbool = false;
					clickunanchorbool = false;
					clickanchorbool = false;
					clicklockbool = false;
					clickunlockbool = false;
					clickcancollidefalsebool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Enabled CTRL + click to tp\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKUNLOCK:
			if (whitelisted && scanned) {
				if (clickunlockbool) {
					Output(txtbox, "CTRL + click to unlock already enabled\n");
				}
				else {
					clickunlockbool = true;
					clickdelbool = false;
					clickexplodebool = false;
					clickpartbool = false;
					clickunanchorbool = false;
					clickanchorbool = false;
					clicktpbool = false;
					clicklockbool = false;
					clickcancollidefalsebool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Enabled CTRL + click to unlock\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKLOCK:
			if (whitelisted && scanned) {
				if (clicklockbool) {
					Output(txtbox, "CTRL + click to lock already enabled\n");
				}
				else {
					clicklockbool = true;
					clickdelbool = false;
					clickexplodebool = false;
					clickpartbool = false;
					clickunanchorbool = false;
					clickanchorbool = false;
					clicktpbool = false;
					clickunlockbool = false;
					clickcancollidefalsebool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Enabled CTRL + click to lock\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKCANCOLLIDEFALSE:
			if (whitelisted && scanned) {
				if (clickcancollidefalsebool) {
					Output(txtbox, "CTRL + click to CanCollide false already enabled\n");
				}
				else {
					clickcancollidefalsebool = true;
					clickdelbool = false;
					clickexplodebool = false;
					clickpartbool = false;
					clickunanchorbool = false;
					clickanchorbool = false;
					clicktpbool = false;
					clickunlockbool = false;
					clicklockbool = false;
					clickcancollidetruebool = false;
					Output(txtbox, "Enabled CTRL + click to CanCollide false\n");
				}
			}
			break;
		case MNOOBXPLOIT_CLICKCANCOLLIDETRUE:
			if (whitelisted && scanned) {
				if (clickcancollidetruebool) {
					Output(txtbox, "CTRL + click to CanCollide true already enabled\n");
				}
				else {
					clickcancollidetruebool = true;
					clickdelbool = false;
					clickexplodebool = false;
					clickpartbool = false;
					clickunanchorbool = false;
					clickanchorbool = false;
					clicktpbool = false;
					clickunlockbool = false;
					clicklockbool = false;
					clickcancollidefalsebool = false;
					Output(txtbox, "Enabled CTRL + click to CanCollide true\n");
				}
			}
			break;
		case MNOOBXPLOIT_LOCALGOD:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Humanoid");
					pushnumber(rlua_State, HUGE_VAL);
					setfield(rlua_State, -2, "MaxHealth");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALSMOKE:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Torso");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Smoke");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 0, 0);
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALPARTICLES:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Torso");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "ParticleEmitter");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 0, 0);
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALSPARKLES:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Torso");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Sparkles");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 0, 0);
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALFIRE:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Torso");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Fire");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 0, 0);
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case MNOOBXPLOIT_LOCALFFBTOOLS:
			if (whitelisted && scanned) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "ForceField");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 0, 0);
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "HopperBin");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushnumber(rlua_State, 1);
					setfield(rlua_State, -2, "BinType");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "HopperBin");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushnumber(rlua_State, 3);
					setfield(rlua_State, -2, "BinType");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "HopperBin");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushnumber(rlua_State, 4);
					setfield(rlua_State, -2, "BinType");
					settop(rlua_State, 0);
				}
				catch (exception) {
					Sleep(1);
				}
			}
			break;
		case NOOBXPLOIT_INPUT_FIELD:
			if (HIWORD(wParam) == EN_MAXTEXT) {
				char cText[100000];
				SendMessage((HWND)lParam, WM_GETTEXT, 100000, (LPARAM)cText);

				if (strcmp(cText, "") == 0)
					break;

				SendMessage((HWND)lParam, WM_SETTEXT, NULL, (LPARAM)"");
			}

			break;
		}
		break;
	case WM_DESTROY:
		exit(4);
		break;

	case WM_QUIT:
		exit(4);
		break;
	default:
		if (hwnd == FindWindowW(NULL, L"Roblox")) {
			return CallWindowProcA(RBLXWindowProc, hwnd, message, wParam, lParam);
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	VMProtectEnd();
	return 0;
}

string replaceAll(string subject, const string& search,
	const string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

string DownloadURL(string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}

BOOL RegisterWindowClass(const char* wClassName) {
	WNDCLASSEX nClass;

	nClass.cbSize = sizeof(WNDCLASSEX);
	nClass.style = CS_DBLCLKS;
	nClass.lpfnWndProc = DLLWindowProc;
	nClass.cbClsExtra = 0;
	nClass.cbWndExtra = 0;
	nClass.hInstance = GetModuleHandle(NULL);
	nClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	nClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	nClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	string pathsame = "C:\\noobhax\\rgb.txt";
	if (!FileExists(pathsame.c_str()) || !DirectoryExists("C:\\noobhax")) {
		if (!FileExists(pathsame.c_str()) && DirectoryExists("C:\\noobhax")) {
			if (MessageBox(NULL, "rgb.txt not found, you're going to use a light blue background (edit C:\\noobhax\\rgb.txt to change it)", "Noobhax background color warning", MB_OK) == IDOK) {
				string commandsame = "echo 76, 133, 224 >\"C:\\noobhax\\rgb.txt\"";
				system(commandsame.c_str());
				nClass.hbrBackground = CreateSolidBrush(RGB(76, 133, 224));
			}
		}
		if (!DirectoryExists("C:\\noobhax")) {
			if (MessageBox(NULL, "noobhax folder not found, you're going to use a light blue background (edit C:\\noobhax\\rgb.txt to change it)", "Noobhax background color warning", MB_OK) == IDOK) {
				system("md C:\\noobhax");
				string commandsame = "echo 76, 133, 224 >\"C:\\noobhax\\rgb.txt\"";
				system(commandsame.c_str());
				nClass.hbrBackground = CreateSolidBrush(RGB(76, 133, 224));
			}
		}
	}
	else {
		double array[3];
		int i = 0;
		char cNum[256];
		ifstream infile;
		infile.open(pathsame, ifstream::in);
		if (infile.is_open())
		{
			while (infile.good())
			{
				infile.getline(cNum, 256, ',');
				array[i] = atof(cNum);
				i++;
			}
			infile.close();
		}
		else
		{
			Output(txtbox, "Error opening file\n");
		}
		double r = array[0];
		double g = array[1];
		double b = array[2];
		nClass.hbrBackground = CreateSolidBrush(RGB(r, g, b));
	}
	string pathsame4 = "C:\\noobhax\\fontrgb.txt";
	if (!FileExists(pathsame4.c_str())) {
		if (MessageBox(NULL, "fontrgb.txt not found, you're going to use the default black fontcolor for the output (edit C:\\noobhax\\fontrgb.txt to change it)", "Noobhax output fontcolor warning", MB_OK) == IDOK) {
			string commandsame = "echo 0, 0, 0 >\"C:\\noobhax\\fontrgb.txt\"";
			system(commandsame.c_str());
			coloroknoob = RGB(0, 0, 0);
		}
	}
	else {
		double arrayz[3];
		int iz = 0;
		char cNumz[256];
		ifstream infilez;
		infilez.open(pathsame4, ifstream::in);
		if (infilez.is_open())
		{
			while (infilez.good())
			{
				infilez.getline(cNumz, 256, ',');
				arrayz[iz] = atof(cNumz);
				iz++;
			}
			infilez.close();
		}
		else
		{
			Output(txtbox, "Error opening file\n");
		}
		double rz = arrayz[0];
		double gz = arrayz[1];
		double bz = arrayz[2];
		coloroknoob = RGB(rz, gz, bz);
	}
	nClass.lpszMenuName = "what";
	nClass.lpszClassName = wClassName;

	if (!RegisterClassEx(&nClass))
		return 0;

	return 1;
}

BOOL StartMessageLoop() {
	MSG msg;
	BOOL bRet;

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == 0) {
			return 0;
		}
		else if (bRet == -1)
		{
			return 0;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


using namespace boost::algorithm;

#include <random>

int GetPlayer(std::string noobchar) {
	noobchar = to_lower_copy(noobchar);
	if (noobchar == "me") {
		int Player = GetLocalPlayer(Players);
		return Player;
	}
	if (noobchar == "random") {
		vector<int> players = GetChildren(Players);
		int max = players.size() - 1;
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, max);
		int chosennumber = distr(eng);
		return players[chosennumber];
	}
	else if (noobchar != "me" && noobchar != "all" && noobchar != "others" && noobchar != "random") {
		int Player = 0;
		try {
			vector<int> playersokkk = GetChildren(Players);
			for (int i = 0; i < playersokkk.size() && !Player; i++) {
				std::string testing1 = (*GetName(playersokkk[i])).substr(0, strlen(noobchar.c_str()));
				std::string testing2 = noobchar;
				testing1 = to_upper_copy(testing1);
				testing2 = to_upper_copy(testing2);
				if (testing1 == testing2) {
					Player = playersokkk[i];
				}
			}
		}
		catch (exception) {
			Sleep(1);
		}
		if (Player) {
			return Player;
		}
		else {
			return 0;
		}
	}
}

int GetCharacter(std::string noobchar) {
	if (noobchar == "me") {
		return *(int*)(GetLocalPlayer(Players) + 88);
	}
	else if (noobchar != "me" && noobchar != "all" && noobchar != "others") {
		return *(int*)(GetPlayer(noobchar) + 88);
	}
}

/*

boost::thread_group my_threads;

void setstate(boost::thread_group & thg, boost::thread * thisTh) {
VMProtectBeginMutation("nubcaikssss");
getglobal(rlua_State, "myhum");
getfield(rlua_State, -1, "ChangeState");
pushvalue(rlua_State, -2);
getglobal(rlua_State, statestr.c_str());
pcall(rlua_State, 2, 0, 0);
settop(rlua_State, 0);
thg.remove_thread(thisTh);
delete thisTh;
VMProtectEnd();
}

void humanoidhacking() {
for (;;) {
std::this_thread::sleep_for(100ms);
if (humanoidhax) {
boost::thread *t = new boost::thread();
*t = boost::thread(
boost::bind(&setstate, boost::ref(my_threads), t)
);
my_threads.add_thread(t);
}
}
}

*/

bool WhyAreYouDecompilingNoob(string a) {
	const char* nubcaikssssyum = a.c_str();
	if (a == "0") { return true; }
	return atoi(nubcaikssssyum);
}

BOOL DirectoryExists(const char* dirName) {
	DWORD attribs = ::GetFileAttributesA(dirName);
	if (attribs == INVALID_FILE_ATTRIBUTES) {
		return false;
	}
	return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}

bool FileExists(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}


DWORD GetLuaState(DWORD* v41) {
	return (int)&v41[41] + v41[41];
}

int checkforkey(lua_State* MyL) {
	int key = lua_tonumber(MyL, -1);
	bool what = false;
	if (GetAsyncKeyState(key)) {
		what = true;
	}
	return what;
}

int wait(lua_State* MyL) {
	int time = lua_tonumber(MyL, -1);
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
	return 0;
}

int clicktp(lua_State* MyL) {
	VMProtectBeginMutation("noobs eat cakes");
	if (whitelisted && scanned && (GetForegroundWindow() == FindWindowW(NULL, L"Roblox"))) {
		int rlua_State2 = (int)((DWORD*)NewThread(rlua_State));
		if (clicktpbool && !clickdelbool && !clickexplodebool && !clickpartbool && !clickunanchorbool && !clickanchorbool && !clicklockbool && !clickunlockbool && !clickcancollidefalsebool && !clickcancollidetruebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool yes = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (yes) {
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Hit");
				getfield(rlua_State2, -1, "p");
				setglobal(rlua_State2, "noobhaxtpposxd");
				settop(rlua_State2, 0);
				getglobal(rlua_State2, "tostring");
				getglobal(rlua_State2, "noobhaxtpposxd");
				pcall(rlua_State2, 1, 1, 0);
				string possame = rlua_tolstring(rlua_State2, -1, NULL);
				settop(rlua_State2, 0);
				getglobal(rlua_State2, "noobhaxmyplr");
				getfield(rlua_State2, -1, "Character");
				getfield(rlua_State2, -1, "HumanoidRootPart");
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Hit");
				setfield(rlua_State2, -3, "CFrame");
				settop(rlua_State2, 0);
				Output(txtbox, "Teleported to " + possame + "\n");
			}
		}
		else if (clickdelbool && !clicktpbool && !clickexplodebool && !clickpartbool && !clickunanchorbool && !clickanchorbool && !clicklockbool && !clickunlockbool && !clickcancollidefalsebool && !clickcancollidetruebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool exists = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (exists) {
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "Name");
				string targetname = rlua_tolstring(rlua_State2, -1, NULL);
				settop(rlua_State2, 0);
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "IsA");
				pushvalue(rlua_State2, -2);
				pushstring(rlua_State2, "BasePart");
				pcall(rlua_State2, 2, 1, 0);
				bool isabasepart = rlua_toboolean(rlua_State2, -1);
				settop(rlua_State2, 0);
				if (isabasepart) {
					getglobal(rlua_State2, "noobhaxmymouse");
					getfield(rlua_State2, -1, "Target");
					getfield(rlua_State2, -1, "remove");
					pushvalue(rlua_State2, -2);
					pcall(rlua_State2, 1, 0, 0);
					settop(rlua_State2, 0);
					Output(txtbox, "Removed " + targetname + "\n");
				}
				else {
					Output(txtbox, "Error: target isn't a BasePart\n");
				}
			}
		}
		else if (clickexplodebool && !clickdelbool && !clicktpbool && !clickpartbool && !clickunanchorbool && !clickanchorbool && !clicklockbool && !clickunlockbool && !clickcancollidefalsebool && !clickcancollidetruebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool yes = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (yes) {
				getglobal(rlua_State2, "Instance");
				getfield(rlua_State2, -1, "new");
				pushstring(rlua_State2, "Explosion");
				getglobal(rlua_State2, "workspace");
				pcall(rlua_State2, 2, 1, 0);
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Hit");
				getfield(rlua_State2, -1, "p");
				setfield(rlua_State2, -4, "Position");
				settop(rlua_State2, 0);
				Output(txtbox, "BOOM\n");
			}
		}
		else if (clickpartbool && !clickexplodebool && !clickdelbool && !clicktpbool && !clickunanchorbool && !clickanchorbool && !clicklockbool && !clickunlockbool && !clickcancollidefalsebool && !clickcancollidetruebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool yes = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (yes) {
				getglobal(rlua_State2, "Instance");
				getfield(rlua_State2, -1, "new");
				pushstring(rlua_State2, "Part");
				getglobal(rlua_State2, "workspace");
				pcall(rlua_State2, 2, 1, 0);
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Hit");
				getfield(rlua_State2, -1, "p");
				setfield(rlua_State2, -4, "Position");
				settop(rlua_State2, 0);
				Output(txtbox, "Made a part\n");
			}
		}
		else if (clickunanchorbool && !clickexplodebool && !clickdelbool && !clicktpbool && !clickpartbool && !clickanchorbool && !clicklockbool && !clickunlockbool && !clickcancollidefalsebool && !clickcancollidetruebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool exists = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (exists) {
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "Name");
				string targetname = rlua_tolstring(rlua_State2, -1, NULL);
				settop(rlua_State2, 0);
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "IsA");
				pushvalue(rlua_State2, -2);
				pushstring(rlua_State2, "BasePart");
				pcall(rlua_State2, 2, 1, 0);
				bool isabasepart = rlua_toboolean(rlua_State2, -1);
				settop(rlua_State2, 0);
				if (isabasepart) {
					getglobal(rlua_State2, "noobhaxmymouse");
					getfield(rlua_State2, -1, "Target");
					pushboolean(rlua_State2, 0);
					setfield(rlua_State2, -2, "Anchored");
					settop(rlua_State2, 0);
					Output(txtbox, "Unanchored " + targetname + "\n");
				}
				else {
					Output(txtbox, "Error: target isn't a BasePart\n");
				}
			}
		}
		else if (clickanchorbool && !clickexplodebool && !clickdelbool && !clicktpbool && !clickpartbool && !clickunanchorbool && !clicklockbool && !clickunlockbool && !clickcancollidefalsebool && !clickcancollidetruebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool exists = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (exists) {
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "Name");
				string targetname = rlua_tolstring(rlua_State2, -1, NULL);
				settop(rlua_State2, 0);
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "IsA");
				pushvalue(rlua_State2, -2);
				pushstring(rlua_State2, "BasePart");
				pcall(rlua_State2, 2, 1, 0);
				bool isabasepart = rlua_toboolean(rlua_State2, -1);
				settop(rlua_State2, 0);
				if (isabasepart) {
					getglobal(rlua_State2, "noobhaxmymouse");
					getfield(rlua_State2, -1, "Target");
					pushboolean(rlua_State2, 1);
					setfield(rlua_State2, -2, "Anchored");
					settop(rlua_State2, 0);
					Output(txtbox, "Anchored " + targetname + "\n");
				}
				else {
					Output(txtbox, "Error: target isn't a BasePart\n");
				}
			}
		}
		else if (clicklockbool && !clickexplodebool && !clickdelbool && !clicktpbool && !clickpartbool && !clickunanchorbool && !clickanchorbool && !clickunlockbool && !clickcancollidefalsebool && !clickcancollidetruebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool exists = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (exists) {
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "Name");
				string targetname = rlua_tolstring(rlua_State2, -1, NULL);
				settop(rlua_State2, 0);
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "IsA");
				pushvalue(rlua_State2, -2);
				pushstring(rlua_State2, "BasePart");
				pcall(rlua_State2, 2, 1, 0);
				bool isabasepart = rlua_toboolean(rlua_State2, -1);
				settop(rlua_State2, 0);
				if (isabasepart) {
					getglobal(rlua_State2, "noobhaxmymouse");
					getfield(rlua_State2, -1, "Target");
					pushboolean(rlua_State2, 1);
					setfield(rlua_State2, -2, "Locked");
					settop(rlua_State2, 0);
					Output(txtbox, "Locked " + targetname + "\n");
				}
				else {
					Output(txtbox, "Error: target isn't a BasePart\n");
				}
			}
		}
		else if (clickunlockbool && !clickexplodebool && !clickdelbool && !clicktpbool && !clickpartbool && !clickunanchorbool && !clickanchorbool && !clicklockbool && !clickcancollidefalsebool && !clickcancollidetruebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool exists = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (exists) {
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "Name");
				string targetname = rlua_tolstring(rlua_State2, -1, NULL);
				settop(rlua_State2, 0);
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "IsA");
				pushvalue(rlua_State2, -2);
				pushstring(rlua_State2, "BasePart");
				pcall(rlua_State2, 2, 1, 0);
				bool isabasepart = rlua_toboolean(rlua_State2, -1);
				settop(rlua_State2, 0);
				if (isabasepart) {
					getglobal(rlua_State2, "noobhaxmymouse");
					getfield(rlua_State2, -1, "Target");
					pushboolean(rlua_State2, 0);
					setfield(rlua_State2, -2, "Locked");
					settop(rlua_State2, 0);
					Output(txtbox, "Unlocked " + targetname + "\n");
				}
				else {
					Output(txtbox, "Error: target isn't a BasePart\n");
				}
			}
		}
		else if (clickcancollidefalsebool && !clickexplodebool && !clickdelbool && !clicktpbool && !clickpartbool && !clickunanchorbool && !clickanchorbool && !clicklockbool && !clickunlockbool && !clickcancollidetruebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool exists = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (exists) {
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "Name");
				string targetname = rlua_tolstring(rlua_State2, -1, NULL);
				settop(rlua_State2, 0);
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "IsA");
				pushvalue(rlua_State2, -2);
				pushstring(rlua_State2, "BasePart");
				pcall(rlua_State2, 2, 1, 0);
				bool isabasepart = rlua_toboolean(rlua_State2, -1);
				settop(rlua_State2, 0);
				if (isabasepart) {
					getglobal(rlua_State2, "noobhaxmymouse");
					getfield(rlua_State2, -1, "Target");
					pushboolean(rlua_State2, 0);
					setfield(rlua_State2, -2, "CanCollide");
					settop(rlua_State2, 0);
					Output(txtbox, "Set " + targetname + " CanCollide to false\n");
				}
				else {
					Output(txtbox, "Error: target isn't a BasePart\n");
				}
			}
		}
		else if (clickcancollidetruebool && !clickexplodebool && !clickdelbool && !clicktpbool && !clickpartbool && !clickunanchorbool && !clickanchorbool && !clicklockbool && !clickunlockbool && !clickcancollidefalsebool) {
			getglobal(rlua_State2, "noobhaxmymouse");
			getfield(rlua_State2, -1, "Target");
			bool exists = rlua_toboolean(rlua_State2, -1);
			settop(rlua_State2, 0);
			if (exists) {
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "Name");
				string targetname = rlua_tolstring(rlua_State2, -1, NULL);
				settop(rlua_State2, 0);
				getglobal(rlua_State2, "noobhaxmymouse");
				getfield(rlua_State2, -1, "Target");
				getfield(rlua_State2, -1, "IsA");
				pushvalue(rlua_State2, -2);
				pushstring(rlua_State2, "BasePart");
				pcall(rlua_State2, 2, 1, 0);
				bool isabasepart = rlua_toboolean(rlua_State2, -1);
				settop(rlua_State2, 0);
				if (isabasepart) {
					getglobal(rlua_State2, "noobhaxmymouse");
					getfield(rlua_State2, -1, "Target");
					pushboolean(rlua_State2, 1);
					setfield(rlua_State2, -2, "CanCollide");
					settop(rlua_State2, 0);
					Output(txtbox, "Set " + targetname + " CanCollide to true\n");
				}
				else {
					Output(txtbox, "Error: target isn't a BasePart\n");
				}
			}
		}
		rlua_State2 = 0;
		delete (int*)rlua_State2;
	}
	VMProtectEnd();
	return 0;
}

bool CheckHTTP(string urlhi) {
	bool rtn = DownloadURL(urlhi) != "";
	return rtn;
}

void ss_prop(int state, const char* prop, const char* str) { //function by froggysfriend, he forced me to add this
	pushstring(state, str);
	setfield(state, -2, prop);
}

void sn_prop(int state, const char* prop, double value) { //function by froggysfriend, he forced me to add this
	pushnumber(state, value);
	setfield(state, -2, prop);
}

void PushVector3(int state, const char* field, int x, int y, int z) { //function by froggysfriend, he forced me to add this
	getglobal(state, "Vector3");
	getfield(state, -1, "new");
	pushnumber(state, x);
	pushnumber(state, y);
	pushnumber(state, z);
	pcall(state, 3, 1, 0);
	setfield(state, -3, field);
}

void PushUDim2(int state, const char* field, int x1, int y1, int x2, int y2) { //function by froggysfriend, he forced me to add this
	getglobal(state, "UDim2");
	getfield(state, -1, "new");
	pushnumber(state, x1);
	pushnumber(state, y1);
	pushnumber(state, x2);
	pushnumber(state, y2);
	pcall(state, 4, 1, 0);
	setfield(state, -3, field);
}

void InstanceNew(int state, const char* instance) { //function by froggysfriend, he forced me to add this
	getglobal(state, "Instance");
	getfield(state, -1, "new");
	pushstring(state, instance);
	pushvalue(state, -4);
	pcall(state, 2, 1, 0);
}


void ChangeFace(int state, const char* plr, const char* face) { //function by froggysfriend, he forced me to add this
	getglobal(state, "game");
	getfield(state, -1, "GetService"); pushvalue(state, -2); pushstring(state, "Players"); pcall(state, 2, 1, 0);
	getfield(state, -1, plr);
	getfield(state, -1, "Character");
	getfield(state, -1, "Head");
	setglobal(state, "noobhax_head_okk");
	settop(state, 0);
	getglobal(state, "noobhax_head_okk");
	InstanceNew(state, "BillboardGui");
	setglobal(state, "noobhax_billboardgui_okk");
	settop(state, 0);
	getglobal(state, "noobhax_billboardgui_okk");
	getglobal(state, "noobhax_head_okk");
	setfield(state, -2, "Adornee");
	settop(state, 0);
	getglobal(state, "noobhax_billboardgui_okk");
	PushUDim2(state, "Size", 2.5, 0, 2.5, 0);
	settop(state, 0);
	getglobal(state, "noobhax_billboardgui_okk");
	PushVector3(state, "StudsOffset", 0, 0.2, 0);
	settop(state, 0);
	getglobal(state, "noobhax_billboardgui_okk");
	pushnumber(state, 1);
	setfield(state, -2, "AlwaysOnTop");
	settop(state, 0);
	getglobal(state, "noobhax_billboardgui_okk");
	InstanceNew(state, "ImageLabel");
	setglobal(state, "noobhax_imagelabel_okk");
	settop(state, 0);
	getglobal(state, "noobhax_imagelabel_okk");
	pushstring(state, face);
	setfield(state, -2, "Image");
	settop(state, 0);
	getglobal(state, "noobhax_imagelabel_okk");
	PushUDim2(state, "Size", 1, 0, 1, 0);
	settop(state, 0);
	getglobal(state, "noobhax_imagelabel_okk");
	PushUDim2(state, "Position", 0, 0, 0, 0);
	settop(state, 0);
	getglobal(state, "noobhax_imagelabel_okk");
	sn_prop(state, "Transparency", 1);
	settop(state, 0);
	getglobal(state, "noobhax_head_okk");
	sn_prop(state, "Transparency", 1);
	settop(rlua_State, 0);
}

void Execute(string command, int rlua_State) {
	vector<string> In = split(command);
	if (In.size() == 0) {
		Output(txtbox, "uh?!\n");
	}
	else {
		if (In.at(0) == "ssjrosé" && In.size() == 2) {
			Output(txtbox, ">" + (command + "\n"));
			Output(txtbox, "Use ssjrose instead\n");
		}
		else {
			In.at(0) = to_lower_copy(In.at(0));
			if (In.at(0) != "output") {
				Output(txtbox, ">" + (command + "\n"));
			}
		}
		if (In.at(0) == "cmds") {
			if (In.size() == 1) {
				Output(txtbox, "ff [p] - Gives [p] a ForceField\n");
				Output(txtbox, "unff [p] or uninvisff [p] or uninvisibleff [p] - Removes [p]'s ForceField\n");
				Output(txtbox, "fire [p] [optional red value] [optional green value] [optional blue value] - Gives [p] Fire\n");
				Output(txtbox, "unfire [p] - Removes [p]'s Fire\n");
				Output(txtbox, "sparkles [p] [optional red value] [optional green value] [optional blue value] or sp [p] [optional red value] [optional green value] [optional blue value] - Gives [p] Sparkles\n");
				Output(txtbox, "unsparkles [p] or unsp [p] - Removes [p]'s Sparkles\n");
				Output(txtbox, "smoke [p] [optional red value] [optional green value] [optional blue value] - Gives [p] Smoke\n");
				Output(txtbox, "unsmoke [p] - Removes [p]'s Smoke\n");
				Output(txtbox, "particles [p] [optional image id] - Gives [p] particles\n");
				Output(txtbox, "unparticles [p] - Removes [p]'s particles\n");
				Output(txtbox, "ws [p] [n] - Sets [p]'s WalkSpeed to [n]\n");
				Output(txtbox, "god [p] - Sets [p]'s MaxHealth to HUGE_VAL\n");
				Output(txtbox, "ungod [p] - Sets [p]'s MaxHealth to 100\n");
				Output(txtbox, "jp [p] [n] - Sets [p]'s JumpPower to [n]\n");
				Output(txtbox, "time [n] - Sets the time to [n]\n");
				Output(txtbox, "btools [p] - Gives [p] building tools\n");
				Output(txtbox, "makepart - Makes a Part in the Workspace\n");
				Output(txtbox, "fogend [n] - Sets FogEnd to [n]\n");
				Output(txtbox, "kill [p] - Kills [p]\n");
				Output(txtbox, "statchange1 [p] [stat] [new int value] or changestat1 [p] [stat] [new int value] or changestat [p] [stat] [new int value] - Sets [p]'s [stat] to [new int value]\n");
				Output(txtbox, "statchange2 [p] [stat] [new string value] or changestat2 [p] [stat] [new string value] - Sets [p]'s [stat] to [new string value]\n");
				Output(txtbox, "getglobal [name] - Use only if you know what it is\n");
				Output(txtbox, "getfield [index] [name] - Use only if you know what it is\n");
				Output(txtbox, "setfield [index] [name] - Use only if you know what it is\n");
				Output(txtbox, "setglobal [name] - Use only if you know what it is\n");
				Output(txtbox, "pushvalue [index] - Use only if you know what it is\n");
				Output(txtbox, "pushstring [string] - Use only if you know what it is\n");
				Output(txtbox, "pushnumber [number] - Use only if you know what it is\n");
				Output(txtbox, "pushboolean [boolean] - Use only if you know what it is\n");
				Output(txtbox, "pcall [nargs] [nresults] [err func = 0] - Use only if you know what it is\n");
				Output(txtbox, "emptystack - Use only if you know what it is\n");
				Output(txtbox, "gettop - Use only if you know what it is\n");
				Output(txtbox, "pop [n] - Use only if you know what it is\n");
				Output(txtbox, "settop [n] - Use only if you know what it is\n");
				Output(txtbox, "invisible [p] or invis [p] - Makes [p] invisible\n");
				Output(txtbox, "visible [p] or unghost [p] or vis [p] - Makes [p] visible\n");
				Output(txtbox, "gravity [n] - Sets the gravity to [n] (default is 196.2)\n");
				Output(txtbox, "print [string] - Prints [string] in the dev console\n");
				Output(txtbox, "tp [p1] [p2] - Teleports [p1] to [p2]\n");
				Output(txtbox, "music [id] [optional pitch] - Plays the music with the id [id]\n");
				Output(txtbox, "explode [p] - Blows up [p]\n");
				Output(txtbox, "punish [p] - Punishes [p]\n");
				Output(txtbox, "unpunish [p] - Unpunishes [p]\n");
				Output(txtbox, "sky [image id] - Makes a skybox with the id [id]\n");
				Output(txtbox, "rsky - Removes noobhax's skybox\n");
				Output(txtbox, "stun [p] - Stuns [p]\n");
				Output(txtbox, "unstun [p] - Unstuns [p]\n");
				Output(txtbox, "light [p] - Lights [p]\n");
				Output(txtbox, "unlight [p] - Unlights [p]\n");
				Output(txtbox, "credits - Outputs the credits\n");
				Output(txtbox, "cls - Cleans the output\n");
				Output(txtbox, "chat [p] [string] - Makes [p] chat [string]\n");
				Output(txtbox, "maxhealth [p] [value] - Sets [p]'s MaxHealth to [value]\n");
				Output(txtbox, "name [p] [new name] - Sets [p]'s Name to [new name]\n");
				Output(txtbox, "clone [p] - Clones [p]\n");
				Output(txtbox, "unname [p] - Restores [p]'s name\n");
				Output(txtbox, "duck [p] - Makes [p] a duck\n");
				Output(txtbox, "unduck [p] - Makes [p] stop being a duck\n");
				Output(txtbox, "freeze [p] or frz [p] - Freezes [p]\n");
				Output(txtbox, "unfreeze [p] or unfrz [p] or thaw [p] - Unfreezes [p]\n");
				Output(txtbox, "ambient [r] [g] [b] - Sets Ambient to [r] [g] [b]\n");
				Output(txtbox, "fogcolor [r] [g] [b] - Sets FogColor to [r] [g] [b]\n");
				Output(txtbox, "fogstart [n] - Sets FogStart to [n]\n");
				Output(txtbox, "bighead [p] - Makes [p]'s Head big\n");
				Output(txtbox, "normalhead [p] or unbighead [p] - Makes [p]'s Head normal\n");
				Output(txtbox, "fling [p] - Flings [p]\n");
				Output(txtbox, "shiny [p] - Makes [p] shiny\n");
				Output(txtbox, "unshiny [p] - Makes [p] not shiny\n");
				Output(txtbox, "ghost [p] - Makes [p] a ghost\n");
				Output(txtbox, "printidentity - Prints current identity (if you think this is fake, do getglobal printidentity and pcall 0 0)\n");
				Output(txtbox, "repeat [n] [command] - Executes [n] times the command [command]\n");
				Output(txtbox, "baseplate - Makes a baseplate\n");
				Output(txtbox, "evilduck [p] - Makes [p] an evil duck\n");
				Output(txtbox, "unevilduck [p] - Makes [p] stop being an evil duck\n");
				Output(txtbox, "giant [p] - Makes [p] a giant\n");
				Output(txtbox, "ungiant [p] - Makes [p] not a giant\n");
				Output(txtbox, "mesh [p] [x] [y] [z] [meshid] [textureid] - Gives [p] a mesh with the given properties (x y z are for the Scale property)\n");
				Output(txtbox, "unmesh [p] or unilluminati [p] or unoshawott [p] or unsnivy [p] or untepig [p] or unpatrick [p] or unugandanknuckles [p] - Removes [p]'s meshes made with the mesh command\n");
				Output(txtbox, "neon [p] - Makes [p] neon\n");
				Output(txtbox, "unneon [p] - Makes [p] stop being neon\n");
				Output(txtbox, "team [p] [new team name] - Makes [p]'s team [new team name]\n");
				Output(txtbox, "damage [p] [n] - Damages [p]\n");
				Output(txtbox, "bring [p] - Brings [p]\n");
				Output(txtbox, "goto [p] or to [p] - Teleports LocalPlayer to [p]\n");
				Output(txtbox, "noob [p] - Makes [p] a noob\n");
				Output(txtbox, "unnoob [p] - Makes [p] stop being a noob\n");
				Output(txtbox, "lock [p] - Locks [p]\n");
				Output(txtbox, "unlock [p] - Unlocks [p]\n");
				Output(txtbox, "fencingreach - Fencing only\n");
				Output(txtbox, "brightness [n] - Sets Brightness to [n]\n");
				Output(txtbox, "stealtools [p] - Steals [p]'s tools\n");
				Output(txtbox, "swordreach - Works on games with a Sword which has Sword in the tool name\n");
				Output(txtbox, "heal [p] - Heals [p]\n");
				Output(txtbox, "clrworkspace - Clears Workspace children if they aren't a Player's Character or Terrain or Camera\n");
				Output(txtbox, "ssjgod [p] - Makes [p] a Super Saiyan God\n");
				Output(txtbox, "mouselock [p] [bool] - Sets [p]'s MouseLock to [bool]\n");
				Output(txtbox, "firstperson [p] - Sets [p]'s Camera to first person\n");
				Output(txtbox, "thirdperson [p] - Sets [p]'s Camera to third person\n");
				Output(txtbox, "age [p] - Outputs [p]'s AccountAge\n");
				Output(txtbox, "rmusic - Removes every Sound in the Workspace\n");
				Output(txtbox, "fegod - Enables FilteringEnabled god mode\n");
				Output(txtbox, "drophats - Drops LocalPlayer's hats\n");
				Output(txtbox, "tptopos [p] [x] [y] [z] - Teleports [p] to the Position [x] [y] [z]\n");
				Output(txtbox, "removetools [p] - Removes [p]'s tools\n");
				Output(txtbox, "printpos [p] - Prints [p]'s Position\n");
				Output(txtbox, "blur - Makes a BlurEffect in the Lighting\n");
				Output(txtbox, "unblur - Removes Blur from the Lighting\n");
				Output(txtbox, "fixl or fixlighting or fix - Removes Lighting children\n");
				Output(txtbox, "box [p] - Gives [p] a SelectionBox\n");
				Output(txtbox, "unbox [p] - Removes [p]'s SelectionBox\n");
				Output(txtbox, "removehats - Removes LocalPlayer's hats\n");
				Output(txtbox, "printtools - Prints LocalPlayer's tools (if you have any)\n");
				Output(txtbox, "savetool [case sensitive full tool name] - Puts the chosen tool in your StarterGear\n");
				Output(txtbox, "stealtools2 [p1] [p2] - Steals [p1]'s tools and gives them to [p2]\n");
				Output(txtbox, "watch [p] - Makes you watch [p]\n");
				Output(txtbox, "unwatch [p] - Makes you stop watching [p]\n");
				Output(txtbox, "waypoint-add [name] - Creates a waypoint with the name [name] for the current game\n");
				Output(txtbox, "waypoint-tp [name] - Teleports to the waypoint [name] for the current game\n");
				Output(txtbox, "waypoint-remove [name] or waypoint-r [name] - Removes the waypoint [name] for the current game\n");
				Output(txtbox, "clonetool [case sensitive full tool name] - Clones the chosen tool for LocalPlayer\n");
				Output(txtbox, "hipheight [p] [n] - Makes [p]'s HipHeight [n]\n");
				Output(txtbox, "warn [string] - Warns [string] in the dev console\n");
				Output(txtbox, "smoketool - Command by FroggysFriend, credits to him\n");
				Output(txtbox, "snivydoll [p] - Gives [p] a Snivy doll!\n");
				Output(txtbox, "tepigdoll [p] - Gives [p] a Tepig doll!\n");
				Output(txtbox, "chickendoll [p] - Gives [p] a Chicken doll!\n");
				Output(txtbox, "signtool [p] - Gives [p] a noobhax sign tool\n");
				Output(txtbox, "oshawottdoll [p] - Gives [p] an Oshawott doll!\n");
				Output(txtbox, "noobdoll [p] - Gives [p] a Noob doll!\n");
				Output(txtbox, "info [string] - Makes an informational message in the dev console with the text [string]\n");
				Output(txtbox, "error [string] - Errors [string] in the dev console\n");
				Output(txtbox, "output [r] [g] [b] [bold boolean] [text] - Self explanatory\n");
				Output(txtbox, "respawn [p] - Respawns [p]\n");
				Output(txtbox, "fedog - Makes you weird :O (replicates on FE)\n");
				Output(txtbox, "invisff [p] or invisibleff [p] - Gives [p] an invisible ForceField\n");
				Output(txtbox, "nuke [p] - Nukes [p]\n");
				Output(txtbox, "harambe [p] - Makes [p] harambe\n");
				Output(txtbox, "unharambe [p] - Makes [p] stop being harambe\n");
				Output(txtbox, "ssjgodssj [p] - Makes [p] a Super Saiyan God Super Saiyan\n");
				Output(txtbox, "ssjrose [p] - Makes [p] a Super Saiyan Rosé\n");
				Output(txtbox, "localchat [string] - Chats [string]\n");
				Output(txtbox, "execluac [url] - Executes Lua C commands from [url] (example: https://pastebin.com/raw/3Bz8h3LU)\n");
				Output(txtbox, "illuminati [p] - ???\n");
				Output(txtbox, "snivy [p] - ???\n");
				Output(txtbox, "tepig [p] - ???\n");
				Output(txtbox, "oshawott [p] - ???\n");
				Output(txtbox, "patrick [p] - ???\n");
				Output(txtbox, "bigfire [p] - Gives [p] big fire\n");
				Output(txtbox, "ugandanknuckles [p] - ???\n");
				//Output(txtbox, "prefix [new chathook prefix] - Sets the chathook prefix to [new chathook prefix]\n");
				//Output(txtbox, "admin [p] - Gives [p] chathook admin\n");
				Output(txtbox, "jolteondoll [p] - Gives [p] a Jolteon doll\n");
				Output(txtbox, "hat [p] [accessory id] - Gives [p] the chosen hat\n");
				Output(txtbox, "insert [model/basepart id] - ***USE IT ONLY ON LUA C EXECUTION BOX FOR BETTER RESULTS*** Inserts the chosen Model or BasePart\n");
				Output(txtbox, "rhum [p] - Removes [p]'s Humanoid\n");
				Output(txtbox, "friendspam [ON/OFF] - Enables or disables friend requests spam when you inject\n");
				Output(txtbox, "trump [p] - Gives [p] a Trump face (by FroggysFriend)\n");
				Output(txtbox, "untrump [p] - Removes [p]'s Trump face\n");
			}
		}
		if (In.at(0) == "untrump") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Head");
								pcall(rlua_State, 2, 1, 0);
								bool found = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (!found)
									Output(txtbox, charname + " doesn't have a Head...\n");
								else {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "FindFirstChild");
									pushvalue(rlua_State, -2);
									pushstring(rlua_State, "BillboardGui");
									pcall(rlua_State, 2, 1, 0);
									bool foundok = rlua_toboolean(rlua_State, -1);
									settop(rlua_State, 0);
									if (!foundok)
										Output(txtbox, charname + " doesn't have a Trump head\n");
									else {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Head");
										getfield(rlua_State, -1, "BillboardGui");
										getfield(rlua_State, -1, "remove");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Head");
										pushnumber(rlua_State, 0);
										setfield(rlua_State, -2, "Transparency");
										settop(rlua_State, 0);
										Output(txtbox, "Removed " + charname + "'s Trump head\n");
									}
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Head");
								pcall(rlua_State, 2, 1, 0);
								bool found = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (!found)
									Output(txtbox, charname + " doesn't have a Head...\n");
								else {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "FindFirstChild");
									pushvalue(rlua_State, -2);
									pushstring(rlua_State, "BillboardGui");
									pcall(rlua_State, 2, 1, 0);
									bool foundok = rlua_toboolean(rlua_State, -1);
									settop(rlua_State, 0);
									if (!foundok)
										Output(txtbox, charname + " doesn't have a Trump head\n");
									else {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Head");
										getfield(rlua_State, -1, "BillboardGui");
										getfield(rlua_State, -1, "remove");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Head");
										pushnumber(rlua_State, 0);
										setfield(rlua_State, -2, "Transparency");
										settop(rlua_State, 0);
										Output(txtbox, "Removed " + charname + "'s Trump head\n");
									}
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "trump") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Head");
								pcall(rlua_State, 2, 1, 0);
								bool found = rlua_toboolean(rlua_State, -1);
								if (!found)
									Output(txtbox, charname + " doesn't have a Head...\n");
								else {
									ChangeFace(rlua_State, charname.c_str(), "rbxassetid://343377229");
									Output(txtbox, charname + " now has a Trump head\n");
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Head");
								pcall(rlua_State, 2, 1, 0);
								bool found = rlua_toboolean(rlua_State, -1);
								if (!found)
									Output(txtbox, charname + " doesn't have a Head...\n");
								else {
									ChangeFace(rlua_State, charname.c_str(), "rbxassetid://343377229");
									Output(txtbox, charname + " now has a Trump head\n");
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "friendspam") {
			if (In.size() == 2) {
				In.at(1) = to_upper_copy(In.at(1));
				if (In.at(1) == "ON" || In.at(1) == "YES") {
					system("echo YES >\"C:\\noobhax\\rich.txt\"");
					Output(txtbox, "Enabled friend requests spam when you inject\n");
				}
				else if (In.at(1) == "OFF" || In.at(1) == "NO") {
					system("echo NO >\"C:\\noobhax\\rich.txt\"");
					Output(txtbox, "Disabled friend requests spam when you inject\n");
				}
				else {
					Output(txtbox, In.at(1) + " isn't a valid option\n");
				}
			}
			else {
				Output(txtbox, "Usage: friendspam [ON/OFF]\n");
			}
		}
		if (In.at(0) == "rhum") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								setglobal(rlua_State, "noobscharacter");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobscharacter");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Humanoid");
								pcall(rlua_State, 2, 1, 0);
								bool found = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (!found) {
									Output(txtbox, charname + " doesn't have a Humanoid\n");
								}
								else {
									getglobal(rlua_State, "noobscharacter");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s Humanoid\n");
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								setglobal(rlua_State, "noobscharacter");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobscharacter");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Humanoid");
								pcall(rlua_State, 2, 1, 0);
								bool found = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (!found) {
									Output(txtbox, charname + " doesn't have a Humanoid\n");
								}
								else {
									getglobal(rlua_State, "noobscharacter");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s Humanoid\n");
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "insert") {
			if (In.size() == 2) {
				try {
					string id = "rbxassetid://" + In.at(1);
					if (CheckHTTP("http://www.roblox.com/asset/?id=" + In.at(1))) {
						getglobal(rlua_State, "game");
						getfield(rlua_State, -1, "GetService");
						pushvalue(rlua_State, -2);
						pushstring(rlua_State, "InsertService");
						pcall(rlua_State, 2, 1, 0);
						getfield(rlua_State, -1, "LoadLocalAsset");
						pushvalue(rlua_State, -2);
						pushstring(rlua_State, id.c_str());
						pcall(rlua_State, 2, 1, 0);
						setglobal(rlua_State, "temp");
						settop(rlua_State, 0);
						Sleep(5);
						getglobal(rlua_State, "temp");
						getfield(rlua_State, -1, "IsA");
						pushvalue(rlua_State, -2);
						pushstring(rlua_State, "BasePart");
						pcall(rlua_State, 2, 1, 0);
						bool isabasepart = rlua_toboolean(rlua_State, -1);
						settop(rlua_State, 0);
						Sleep(5);
						getglobal(rlua_State, "temp");
						getfield(rlua_State, -1, "ClassName");
						string classname = rlua_tolstring(rlua_State, -1, NULL);
						settop(rlua_State, 0);
						Sleep(5);
						if (isabasepart && classname != "Model") {
							getglobal(rlua_State, "temp");
							getglobal(rlua_State, "workspace");
							setfield(rlua_State, -2, "Parent");
							settop(rlua_State, 0);
							Sleep(5);
							getglobal(rlua_State, "temp");
							getfield(rlua_State, -1, "MakeJoints");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
							Sleep(5);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "Head");
							getfield(rlua_State, -1, "Position");
							setglobal(rlua_State, "pos");
							settop(rlua_State, 0);
							Sleep(5);
							getglobal(rlua_State, "temp");
							getglobal(rlua_State, "pos");
							setfield(rlua_State, -2, "Position");
							settop(rlua_State, 0);
							Output(txtbox, "OK!\n");
						}
						else if (classname == "Model") {
							getglobal(rlua_State, "temp");
							getglobal(rlua_State, "workspace");
							setfield(rlua_State, -2, "Parent");
							settop(rlua_State, 0);
							Sleep(5);
							getglobal(rlua_State, "temp");
							getfield(rlua_State, -1, "MakeJoints");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
							Sleep(5);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "Head");
							getfield(rlua_State, -1, "Position");
							setglobal(rlua_State, "pos");
							settop(rlua_State, 0);
							Sleep(5);
							getglobal(rlua_State, "temp");
							getfield(rlua_State, -1, "MoveTo");
							pushvalue(rlua_State, -2);
							getglobal(rlua_State, "pos");
							pcall(rlua_State, 2, 0, 0);
							settop(rlua_State, 0);
							Output(txtbox, "OK!\n");
						}
						else {
							Output(txtbox, "The selected ID isn't a BasePart or a Model\n");
						}
					}
					else {
						Output(txtbox, "ID isn't valid\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "hat") {
			if (In.size() == 3) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								string hatid = In.at(2);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								setglobal(rlua_State, "noobhax_hat_parent_ok_hi");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "InsertService"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, "LoadLocalAsset");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, ("rbxassetid://" + hatid).c_str());
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "noobhax_hat_parent_ok_hi");
								setfield(rlua_State, -2, "Parent");
								Output(txtbox, "Gave the chosen hat to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								string hatid = In.at(2);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								setglobal(rlua_State, "noobhax_hat_parent_ok_hi");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "InsertService"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, "LoadLocalAsset");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, ("rbxassetid://" + hatid).c_str());
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "noobhax_hat_parent_ok_hi");
								setfield(rlua_State, -2, "Parent");
								Output(txtbox, "Gave the chosen hat to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "jolteondoll") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "jolteondoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteondoll");
								pushstring(rlua_State, "noobhax Jolteon doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "jolteondoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "jolteonhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "jolteonhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "jolteonmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								pushstring(rlua_State, "noobhax_jolteon_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								pushstring(rlua_State, "rbxassetid://895004074");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								pushstring(rlua_State, "rbxassetid://895004079");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.025);
								pushnumber(rlua_State, 0.025);
								pushnumber(rlua_State, 0.025);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a jolteon doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "jolteondoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteondoll");
								pushstring(rlua_State, "noobhax Jolteon doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "jolteondoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "jolteonhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "jolteonhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "jolteonmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								pushstring(rlua_State, "noobhax_jolteon_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								pushstring(rlua_State, "rbxassetid://895004074");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								pushstring(rlua_State, "rbxassetid://895004079");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "jolteonmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.025);
								pushnumber(rlua_State, 0.025);
								pushnumber(rlua_State, 0.025);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a jolteon doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		/*
		if (In.at(0) == "admin") {
		if (In.size() == 2) {
		try {
		if (In.at(1) == "all" || In.at(1) == "others") {
		vector<string> noobfaces = getnames(In.at(1));
		for (int i = 0; i < noobfaces.size(); i++) {

		try {
		string noob1 = noobfaces[i];
		bool isadmin = IsAdmin(noob1);
		if (isadmin) {
		Output(txtbox, noob1 + " is already an admin\n");
		}
		else {
		string hithere = "You're now a noobhax admin! Remember: don't use 'me'. Try " + prefix + "ff [your name]";
		callcheck();
		rlua_pushcfunction(rlua_State, (int)EIP_BP);
		setglobal(rlua_State, "mynoobhookfunction");
		settop(rlua_State, 0);
		getglobal(rlua_State, "mynoobhookfunction");
		setglobal(rlua_State, "mynoobhookfunc");
		settop(rlua_State, 0);
		getglobal(rlua_State, "game");
		getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
		getfield(rlua_State, -1, noob1.c_str());
		getfield(rlua_State, -1, "Chatted");
		getfield(rlua_State, -1, "connect");
		pushvalue(rlua_State, -2);
		getglobal(rlua_State, "mynoobhookfunc");
		pcall(rlua_State, 2, 0, 0);
		settop(rlua_State, 0);
		callcheck2();
		Output(txtbox, (noob1 + " is now an admin\n"));
		getglobal(rlua_State, "game");
		getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
		getfield(rlua_State, -1, noob1.c_str());
		getfield(rlua_State, -1, "Character");
		getfield(rlua_State, -1, "Head");
		getglobal(rlua_State, "game");
		getfield(rlua_State, -1, "Chat");
		getfield(rlua_State, -1, "Chat");
		pushvalue(rlua_State, -2);
		pushvalue(rlua_State, -5);
		pushstring(rlua_State, hithere.c_str());
		pushstring(rlua_State, "Red");
		pcall(rlua_State, 4, 0, 0);
		settop(rlua_State, 0);
		adminplayers.push_back(noob1);
		}
		}
		catch (exception) {
		Sleep(1);
		}
		}
		}
		else {
		int noobokokok;
		try {
		noobokokok = GetPlayer(In.at(1));
		}
		catch (exception) {
		Sleep(1);
		}
		if (noobokokok == 0) {
		Output(txtbox, In.at(1) + " not found\n");
		}
		else {
		try {
		string noob1 = *GetName(noobokokok);
		bool isadmin = IsAdmin(noob1);
		if (isadmin) {
		Output(txtbox, noob1 + " is already an admin\n");
		}
		else {
		string hithere = "You're now a noobhax admin! Remember: don't use 'me'. Try " + prefix + "ff [your name]";
		callcheck();
		rlua_pushcfunction(rlua_State, (int)EIP_BP);
		setglobal(rlua_State, "mynoobhookfunction");
		settop(rlua_State, 0);
		getglobal(rlua_State, "mynoobhookfunction");
		setglobal(rlua_State, "mynoobhookfunc");
		settop(rlua_State, 0);
		getglobal(rlua_State, "game");
		getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
		getfield(rlua_State, -1, noob1.c_str());
		getfield(rlua_State, -1, "Chatted");
		getfield(rlua_State, -1, "connect");
		pushvalue(rlua_State, -2);
		getglobal(rlua_State, "mynoobhookfunc");
		pcall(rlua_State, 2, 0, 0);
		settop(rlua_State, 0);
		callcheck2();
		Output(txtbox, (noob1 + " is now an admin\n"));
		getglobal(rlua_State, "game");
		getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
		getfield(rlua_State, -1, noob1.c_str());
		getfield(rlua_State, -1, "Character");
		getfield(rlua_State, -1, "Head");
		getglobal(rlua_State, "game");
		getfield(rlua_State, -1, "Chat");
		getfield(rlua_State, -1, "Chat");
		pushvalue(rlua_State, -2);
		pushvalue(rlua_State, -5);
		pushstring(rlua_State, hithere.c_str());
		pushstring(rlua_State, "Red");
		pcall(rlua_State, 4, 0, 0);
		settop(rlua_State, 0);
		adminplayers.push_back(noob1);
		}
		}
		catch (exception) {
		Sleep(1);
		}
		}
		}
		}
		catch (exception) {
		Sleep(1);
		}
		}
		}
		if (In.at(0) == "prefix") {
		if (In.size() == 2) {
		string a = prefix;
		prefix = In.at(1);
		Output(txtbox, "Set the prefix from " + a + " to " + prefix + "\n");
		}
		else {
		Output(txtbox, "Usage: prefix [new chathook prefix]\n");
		}
		}
		*/
		if (In.at(0) == "bigfire") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Fire");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 40);
								setfield(rlua_State, -2, "Size");
								settop(rlua_State, 0);
								Output(txtbox, "Gave big fire to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Fire");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 40);
								setfield(rlua_State, -2, "Size");
								settop(rlua_State, 0);
								Output(txtbox, "Gave big fire to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "illuminati" || In.at(0) == "snivy" || In.at(0) == "tepig" || In.at(0) == "oshawott" || In.at(0) == "patrick" || In.at(0) == "ugandanknuckles") {
			if (In.size() == 2) {
				string meshid, textureid;
				if (In.at(0) == "illuminati") {
					meshid = "rbxassetid://438530093";
					textureid = "rbxassetid://438530120";
				}
				else if (In.at(0) == "snivy") {
					meshid = "rbxassetid://528336272";
					textureid = "rbxassetid://528336299";
				}
				else if (In.at(0) == "tepig") {
					meshid = "rbxassetid://894825018";
					textureid = "rbxassetid://894825026";
				}
				else if (In.at(0) == "oshawott") {
					meshid = "rbxassetid://969521738";
					textureid = "rbxassetid://969523476";
				}
				else if (In.at(0) == "ugandanknuckles") {
					meshid = "rbxassetid://1300136843";
					textureid = "rbxassetid://1300137893";
				}
				else {
					meshid = "rbxassetid://432003142";
					textureid = "rbxassetid://432003146";
				}
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_mesh_ok");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, meshid.c_str());
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, textureid.c_str());
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								if (In.at(0) == "patrick") {
									pushnumber(rlua_State, 0.7);
									pushnumber(rlua_State, 0.7);
									pushnumber(rlua_State, 0.7);
								}
								else if (In.at(0) == "ugandanknuckles") {
									pushnumber(rlua_State, 0.07);
									pushnumber(rlua_State, 0.062);
									pushnumber(rlua_State, 0.078);
								}
								else {
									pushnumber(rlua_State, 0.45);
									pushnumber(rlua_State, 0.45);
									pushnumber(rlua_State, 0.45);
								}
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a mesh to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_mesh_ok");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, meshid.c_str());
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, textureid.c_str());
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								if (In.at(0) == "patrick") {
									pushnumber(rlua_State, 0.7);
									pushnumber(rlua_State, 0.7);
									pushnumber(rlua_State, 0.7);
								}
								else if (In.at(0) == "ugandanknuckles") {
									pushnumber(rlua_State, 0.07);
									pushnumber(rlua_State, 0.062);
									pushnumber(rlua_State, 0.078);
								}
								else {
									pushnumber(rlua_State, 0.45);
									pushnumber(rlua_State, 0.45);
									pushnumber(rlua_State, 0.45);
								}
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a mesh to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "execluac") {
			if (In.size() == 2) {
				string txt = DownloadURL(In.at(1));
				if (txt == "") {
					Output(txtbox, "Couldn't get text from URL\n");
				}
				else {
					vector<string> nubh4xxor = split2(txt, '\n');
					for (int i = 0; i < nubh4xxor.size(); i++) {
						Execute(nubh4xxor[i], rlua_State);
					}
					Output(txtbox, "OK!\n");
				}
			}
			else {
				Output(txtbox, "Usage: execluac [url]\n");
			}
		}
		if (In.at(0) == "localchat") {
			string string;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					string.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					string.append(In.at(i));
				}
			}
			try {
				getglobal(rlua_State, "game");
				getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "ReplicatedStorage"); pcall(rlua_State, 2, 1, 0);
				getfield(rlua_State, -1, "DefaultChatSystemChatEvents");
				getfield(rlua_State, -1, "SayMessageRequest");
				getfield(rlua_State, -1, "FireServer");
				pushvalue(rlua_State, -2);
				pushstring(rlua_State, string.c_str());
				pushstring(rlua_State, "All");
				pcall(rlua_State, 3, 0, 0);
				settop(rlua_State, 0);
				Output(txtbox, "Chatted " + string + "\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "ssjrose") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "ssjrose_noobhax");
								pcall(rlua_State, 2, 1, 0);
								bool isssjrose = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isssjrose == false) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getglobal(rlua_State, "Instance");
									getfield(rlua_State, -1, "new");
									pushstring(rlua_State, "ParticleEmitter");
									pushvalue(rlua_State, -4);
									pcall(rlua_State, 2, 1, 0);
									pushstring(rlua_State, "ssjrose_noobhax");
									setfield(rlua_State, -2, "Name");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjrose_noobhax");
									getglobal(rlua_State, "NumberSequence");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 15);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Size");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjrose_noobhax");
									pushstring(rlua_State, "rbxassetid://1016284374");
									setfield(rlua_State, -2, "Texture");
									pushnumber(rlua_State, 2);
									setfield(rlua_State, -2, "Rate");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjrose_noobhax");
									getglobal(rlua_State, "NumberRange");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 5);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Speed");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjrose_noobhax");
									pushnumber(rlua_State, 20);
									setfield(rlua_State, -2, "VelocitySpread");
									settop(rlua_State, 0);
									Output(txtbox, charname + " IS NOW A SUPER SAIYAN ROSÉ\n");
								}
								else {
									try {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Name");
										string charnamelolkk = rlua_tolstring(rlua_State, -1, NULL);
										settop(rlua_State, 0);
										int character = GetCharacter(charnamelolkk);
										int torso = FindFirstChild_PartOf(character, "Torso");
										vector<int> fuckingtorsochildren = GetChildren(torso);
										for (int i = 0; i < fuckingtorsochildren.size(); i++) {
											try {
												if (*GetName(fuckingtorsochildren[i]) == "ssjrose_noobhax") {
													getglobal(rlua_State, "game");
													getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
													getfield(rlua_State, -1, charname.c_str());
													getfield(rlua_State, -1, "Character");
													getfield(rlua_State, -1, "Torso");
													getfield(rlua_State, -1, "ssjrose_noobhax");
													getfield(rlua_State, -1, "remove");
													pushvalue(rlua_State, -2);
													pcall(rlua_State, 1, 0, 0);
													settop(rlua_State, 0);
												}
												else {
													Sleep(1);
												}
											}
											catch (exception) {
												Sleep(1);
											}
										}
										try {
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getglobal(rlua_State, "Instance");
											getfield(rlua_State, -1, "new");
											pushstring(rlua_State, "ParticleEmitter");
											pushvalue(rlua_State, -4);
											pcall(rlua_State, 2, 1, 0);
											pushstring(rlua_State, "ssjrose_noobhax");
											setfield(rlua_State, -2, "Name");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjrose_noobhax");
											getglobal(rlua_State, "NumberSequence");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 15);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Size");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjrose_noobhax");
											pushstring(rlua_State, "rbxassetid://1016284374");
											setfield(rlua_State, -2, "Texture");
											pushnumber(rlua_State, 2);
											setfield(rlua_State, -2, "Rate");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjrose_noobhax");
											getglobal(rlua_State, "NumberRange");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 5);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Speed");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjrose_noobhax");
											pushnumber(rlua_State, 20);
											setfield(rlua_State, -2, "VelocitySpread");
											settop(rlua_State, 0);
											Output(txtbox, charname + " IS NOW A SUPER SAIYAN ROSÉ\n");
										}
										catch (exception) {
											Sleep(1);
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "ssjrose_noobhax");
								pcall(rlua_State, 2, 1, 0);
								bool isssjrose = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isssjrose == false) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getglobal(rlua_State, "Instance");
									getfield(rlua_State, -1, "new");
									pushstring(rlua_State, "ParticleEmitter");
									pushvalue(rlua_State, -4);
									pcall(rlua_State, 2, 1, 0);
									pushstring(rlua_State, "ssjrose_noobhax");
									setfield(rlua_State, -2, "Name");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjrose_noobhax");
									getglobal(rlua_State, "NumberSequence");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 15);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Size");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjrose_noobhax");
									pushstring(rlua_State, "rbxassetid://1016284374");
									setfield(rlua_State, -2, "Texture");
									pushnumber(rlua_State, 2);
									setfield(rlua_State, -2, "Rate");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjrose_noobhax");
									getglobal(rlua_State, "NumberRange");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 5);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Speed");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjrose_noobhax");
									pushnumber(rlua_State, 20);
									setfield(rlua_State, -2, "VelocitySpread");
									settop(rlua_State, 0);
									Output(txtbox, charname + " IS NOW A SUPER SAIYAN ROSÉ\n");
								}
								else {
									try {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Name");
										string charnamelolkk = rlua_tolstring(rlua_State, -1, NULL);
										settop(rlua_State, 0);
										int character = GetCharacter(charnamelolkk);
										int torso = FindFirstChild_PartOf(character, "Torso");
										vector<int> fuckingtorsochildren = GetChildren(torso);
										for (int i = 0; i < fuckingtorsochildren.size(); i++) {
											try {
												if (*GetName(fuckingtorsochildren[i]) == "ssjrose_noobhax") {
													getglobal(rlua_State, "game");
													getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
													getfield(rlua_State, -1, charname.c_str());
													getfield(rlua_State, -1, "Character");
													getfield(rlua_State, -1, "Torso");
													getfield(rlua_State, -1, "ssjrose_noobhax");
													getfield(rlua_State, -1, "remove");
													pushvalue(rlua_State, -2);
													pcall(rlua_State, 1, 0, 0);
													settop(rlua_State, 0);
												}
												else {
													Sleep(1);
												}
											}
											catch (exception) {
												Sleep(1);
											}
										}
										try {
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getglobal(rlua_State, "Instance");
											getfield(rlua_State, -1, "new");
											pushstring(rlua_State, "ParticleEmitter");
											pushvalue(rlua_State, -4);
											pcall(rlua_State, 2, 1, 0);
											pushstring(rlua_State, "ssjrose_noobhax");
											setfield(rlua_State, -2, "Name");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjrose_noobhax");
											getglobal(rlua_State, "NumberSequence");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 15);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Size");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjrose_noobhax");
											pushstring(rlua_State, "rbxassetid://1016284374");
											setfield(rlua_State, -2, "Texture");
											pushnumber(rlua_State, 2);
											setfield(rlua_State, -2, "Rate");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjrose_noobhax");
											getglobal(rlua_State, "NumberRange");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 5);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Speed");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjrose_noobhax");
											pushnumber(rlua_State, 20);
											setfield(rlua_State, -2, "VelocitySpread");
											settop(rlua_State, 0);
											Output(txtbox, charname + " IS NOW A SUPER SAIYAN ROSÉ\n");
										}
										catch (exception) {
											Sleep(1);
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "ssjgodssj") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "ssjgodssj_noobhax");
								pcall(rlua_State, 2, 1, 0);
								bool isssjgodssj = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isssjgodssj == false) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getglobal(rlua_State, "Instance");
									getfield(rlua_State, -1, "new");
									pushstring(rlua_State, "ParticleEmitter");
									pushvalue(rlua_State, -4);
									pcall(rlua_State, 2, 1, 0);
									pushstring(rlua_State, "ssjgodssj_noobhax");
									setfield(rlua_State, -2, "Name");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgodssj_noobhax");
									getglobal(rlua_State, "NumberSequence");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 15);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Size");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgodssj_noobhax");
									pushstring(rlua_State, "rbxassetid://1016161318");
									setfield(rlua_State, -2, "Texture");
									pushnumber(rlua_State, 2);
									setfield(rlua_State, -2, "Rate");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgodssj_noobhax");
									getglobal(rlua_State, "NumberRange");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 5);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Speed");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgodssj_noobhax");
									pushnumber(rlua_State, 20);
									setfield(rlua_State, -2, "VelocitySpread");
									settop(rlua_State, 0);
									Output(txtbox, charname + " IS NOW A SUPER SAIYAN GOD SUPER SAIYAN\n");
								}
								else {
									try {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Name");
										string charnamelolkk = rlua_tolstring(rlua_State, -1, NULL);
										settop(rlua_State, 0);
										int character = GetCharacter(charnamelolkk);
										int torso = FindFirstChild_PartOf(character, "Torso");
										vector<int> fuckingtorsochildren = GetChildren(torso);
										for (int i = 0; i < fuckingtorsochildren.size(); i++) {
											try {
												if (*GetName(fuckingtorsochildren[i]) == "ssjgodssj_noobhax") {
													getglobal(rlua_State, "game");
													getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
													getfield(rlua_State, -1, charname.c_str());
													getfield(rlua_State, -1, "Character");
													getfield(rlua_State, -1, "Torso");
													getfield(rlua_State, -1, "ssjgodssj_noobhax");
													getfield(rlua_State, -1, "remove");
													pushvalue(rlua_State, -2);
													pcall(rlua_State, 1, 0, 0);
													settop(rlua_State, 0);
												}
												else {
													Sleep(1);
												}
											}
											catch (exception) {
												Sleep(1);
											}
										}
										try {
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getglobal(rlua_State, "Instance");
											getfield(rlua_State, -1, "new");
											pushstring(rlua_State, "ParticleEmitter");
											pushvalue(rlua_State, -4);
											pcall(rlua_State, 2, 1, 0);
											pushstring(rlua_State, "ssjgodssj_noobhax");
											setfield(rlua_State, -2, "Name");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgodssj_noobhax");
											getglobal(rlua_State, "NumberSequence");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 15);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Size");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgodssj_noobhax");
											pushstring(rlua_State, "rbxassetid://1016161318");
											setfield(rlua_State, -2, "Texture");
											pushnumber(rlua_State, 2);
											setfield(rlua_State, -2, "Rate");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgodssj_noobhax");
											getglobal(rlua_State, "NumberRange");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 5);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Speed");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgodssj_noobhax");
											pushnumber(rlua_State, 20);
											setfield(rlua_State, -2, "VelocitySpread");
											settop(rlua_State, 0);
											Output(txtbox, charname + " IS NOW A SUPER SAIYAN GOD SUPER SAIYAN\n");
										}
										catch (exception) {
											Sleep(1);
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "ssjgodssj_noobhax");
								pcall(rlua_State, 2, 1, 0);
								bool isssjgodssj = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isssjgodssj == false) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getglobal(rlua_State, "Instance");
									getfield(rlua_State, -1, "new");
									pushstring(rlua_State, "ParticleEmitter");
									pushvalue(rlua_State, -4);
									pcall(rlua_State, 2, 1, 0);
									pushstring(rlua_State, "ssjgodssj_noobhax");
									setfield(rlua_State, -2, "Name");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgodssj_noobhax");
									getglobal(rlua_State, "NumberSequence");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 15);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Size");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgodssj_noobhax");
									pushstring(rlua_State, "rbxassetid://1016161318");
									setfield(rlua_State, -2, "Texture");
									pushnumber(rlua_State, 2);
									setfield(rlua_State, -2, "Rate");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgodssj_noobhax");
									getglobal(rlua_State, "NumberRange");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 5);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Speed");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgodssj_noobhax");
									pushnumber(rlua_State, 20);
									setfield(rlua_State, -2, "VelocitySpread");
									settop(rlua_State, 0);
									Output(txtbox, charname + " IS NOW A SUPER SAIYAN GOD SUPER SAIYAN\n");
								}
								else {
									try {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Name");
										string charnamelolkk = rlua_tolstring(rlua_State, -1, NULL);
										settop(rlua_State, 0);
										int character = GetCharacter(charnamelolkk);
										int torso = FindFirstChild_PartOf(character, "Torso");
										vector<int> fuckingtorsochildren = GetChildren(torso);
										for (int i = 0; i < fuckingtorsochildren.size(); i++) {
											try {
												if (*GetName(fuckingtorsochildren[i]) == "ssjgodssj_noobhax") {
													getglobal(rlua_State, "game");
													getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
													getfield(rlua_State, -1, charname.c_str());
													getfield(rlua_State, -1, "Character");
													getfield(rlua_State, -1, "Torso");
													getfield(rlua_State, -1, "ssjgodssj_noobhax");
													getfield(rlua_State, -1, "remove");
													pushvalue(rlua_State, -2);
													pcall(rlua_State, 1, 0, 0);
													settop(rlua_State, 0);
												}
												else {
													Sleep(1);
												}
											}
											catch (exception) {
												Sleep(1);
											}
										}
										try {
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getglobal(rlua_State, "Instance");
											getfield(rlua_State, -1, "new");
											pushstring(rlua_State, "ParticleEmitter");
											pushvalue(rlua_State, -4);
											pcall(rlua_State, 2, 1, 0);
											pushstring(rlua_State, "ssjgodssj_noobhax");
											setfield(rlua_State, -2, "Name");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgodssj_noobhax");
											getglobal(rlua_State, "NumberSequence");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 15);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Size");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgodssj_noobhax");
											pushstring(rlua_State, "rbxassetid://1016161318");
											setfield(rlua_State, -2, "Texture");
											pushnumber(rlua_State, 2);
											setfield(rlua_State, -2, "Rate");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgodssj_noobhax");
											getglobal(rlua_State, "NumberRange");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 5);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Speed");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgodssj_noobhax");
											pushnumber(rlua_State, 20);
											setfield(rlua_State, -2, "VelocitySpread");
											settop(rlua_State, 0);
											Output(txtbox, charname + " IS NOW A SUPER SAIYAN GOD SUPER SAIYAN\n");
										}
										catch (exception) {
											Sleep(1);
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unharambe") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_harambe");
								pcall(rlua_State, 2, 1, 0);
								bool isharambe = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isharambe) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "face");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "noobhax_harambe");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, charname + " isn't harambe anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_harambe");
								pcall(rlua_State, 2, 1, 0);
								bool isharambe = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isharambe) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "face");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "noobhax_harambe");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, charname + " isn't harambe anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "harambe") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_harambe");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, "rbxassetid://430330296");
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, "rbxassetid://430330316");
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.01);
								pushnumber(rlua_State, 0.01);
								pushnumber(rlua_State, 0.01);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now harambe\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_harambe");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, "rbxassetid://430330296");
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, "rbxassetid://430330316");
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.01);
								pushnumber(rlua_State, 0.01);
								pushnumber(rlua_State, 0.01);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now harambe\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "nuke") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string fuckingnoob = noobfaces[i];
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "workspace");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxnuke");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Anchored");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "CanCollide");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushstring(rlua_State, "Symmetric");
								setfield(rlua_State, -2, "FormFactor");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushstring(rlua_State, "Ball");
								setfield(rlua_State, -2, "Shape");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 70);
								pushnumber(rlua_State, 70);
								pushnumber(rlua_State, 70);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Size");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								getglobal(rlua_State, "BrickColor");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "New Yeller");
								pcall(rlua_State, 1, 1, 0);
								setfield(rlua_State, -3, "BrickColor");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushnumber(rlua_State, 0.2);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "TopSurface");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "BottomSurface");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, fuckingnoob.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "HumanoidRootPart");
								getfield(rlua_State, -1, "Position");
								setglobal(rlua_State, "noobhaxnukepos");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Explosion");
								getglobal(rlua_State, "workspace");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxactualnuke");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxactualnuke");
								getglobal(rlua_State, "noobhaxnukepos");
								setfield(rlua_State, -2, "Position");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxactualnuke");
								pushnumber(rlua_State, 11);
								setfield(rlua_State, -2, "BlastRadius");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxactualnuke");
								pushnumber(rlua_State, HUGE_VAL);
								setfield(rlua_State, -2, "BlastPressure");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								getglobal(rlua_State, "noobhaxnukepos");
								setfield(rlua_State, -2, "Position");
								settop(rlua_State, 0);
								std::this_thread::sleep_for(1500ms);
								getglobal(rlua_State, "noobhaxnuke");
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Blew up " + fuckingnoob + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try
							{
								string fuckingnoob = *GetName(noobokokok);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "workspace");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxnuke");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Anchored");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "CanCollide");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushstring(rlua_State, "Symmetric");
								setfield(rlua_State, -2, "FormFactor");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushstring(rlua_State, "Ball");
								setfield(rlua_State, -2, "Shape");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 70);
								pushnumber(rlua_State, 70);
								pushnumber(rlua_State, 70);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Size");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								getglobal(rlua_State, "BrickColor");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "New Yeller");
								pcall(rlua_State, 1, 1, 0);
								setfield(rlua_State, -3, "BrickColor");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushnumber(rlua_State, 0.2);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "TopSurface");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "BottomSurface");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, fuckingnoob.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "HumanoidRootPart");
								getfield(rlua_State, -1, "Position");
								setglobal(rlua_State, "noobhaxnukepos");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Explosion");
								getglobal(rlua_State, "workspace");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxactualnuke");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxactualnuke");
								getglobal(rlua_State, "noobhaxnukepos");
								setfield(rlua_State, -2, "Position");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxactualnuke");
								pushnumber(rlua_State, 11);
								setfield(rlua_State, -2, "BlastRadius");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxactualnuke");
								pushnumber(rlua_State, HUGE_VAL);
								setfield(rlua_State, -2, "BlastPressure");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxnuke");
								getglobal(rlua_State, "noobhaxnukepos");
								setfield(rlua_State, -2, "Position");
								settop(rlua_State, 0);
								std::this_thread::sleep_for(1500ms);
								getglobal(rlua_State, "noobhaxnuke");
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Blew up " + fuckingnoob + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "invisff" || In.at(0) == "invisibleff") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ForceField");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Visible");
								settop(rlua_State, 0);
								Output(txtbox, "Gave an invisible ForceField to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ForceField");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Visible");
								settop(rlua_State, 0);
								Output(txtbox, "Gave an invisible ForceField to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "google") {
			string stringq;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					stringq.append(In.at(i) + '+');
				}
				if (i == (In.size() - 1)) {
					stringq.append(In.at(i));
				}
			}
			string stringw;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					stringw.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					stringw.append(In.at(i));
				}
			}
			ShellExecute(NULL, "open", ("http://google.com/search?&q=" + stringq).c_str(), NULL, NULL, SW_SHOWNORMAL);
			Output(txtbox, "Googled " + stringw + " for you :-)\n");
		}
		if (In.at(0) == "pornhub") {
			if (In.size() == 1) {
				if (MessageBox(0, "Continue only if you are 18+", "Warning", MB_YESNOCANCEL) == IDYES) {
					ShellExecute(NULL, "open", "http://pornhub.com", NULL, NULL, SW_SHOWNORMAL);
					Output(txtbox, "How did you find this?\n");
				}
				else {
					Output(txtbox, "Come back when you are older!\n");
				}
			}
		}
		if (In.at(0) == "fedog") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					bool charokornot = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (charokornot) {
						int Character = *(int*)(GetLocalPlayer(Players) + 88);
						vector<int> childrenok = GetChildren(Character);
						for (int i = 0; i < childrenok.size(); i++) {
							string noobk = *GetName(childrenok[i]);
							if (noobk == "CharacterMesh") {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, noobk.c_str());
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
							}
							string classnamek = GetClass(childrenok[i]);
							if (classnamek == "Shirt" || classnamek == "Pants") {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, noobk.c_str());
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
							}
							if (classnamek == "Accessory" || classnamek == "Hat") {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, noobk.c_str());
								getfield(rlua_State, -1, "Handle");
								getfield(rlua_State, -1, "Mesh");
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
							}
							if (noobk == "Head") {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, noobk.c_str());
								getfield(rlua_State, -1, "Mesh");
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
							}
							if (classnamek == "Part" && noobk != "Torso" && noobk != "Head" && noobk != "HumanoidRootPart") {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, noobk.c_str());
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
							}
						}
						Output(txtbox, "Woof woof!\n");
					}
					else {
						Sleep(1);
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "respawn") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								setglobal(rlua_State, "noobhaxplrOKHI");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxplrOKHI");
								getfield(rlua_State, -1, "Character");
								setglobal(rlua_State, "noobhaxcharOKHI");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Model");
								getglobal(rlua_State, "noobhaxplrOKHI");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxmycharOKHI");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxplrOKHI");
								getfield(rlua_State, -1, "Name");
								getglobal(rlua_State, "noobhaxmycharOKHI");
								pushvalue(rlua_State, -2);
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Humanoid");
								getglobal(rlua_State, "noobhaxmycharOKHI");
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxcharOKHI");
								bool CHAROKXD = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (CHAROKXD) {
									getglobal(rlua_State, "noobhaxcharOKHI");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
								}
								getglobal(rlua_State, "noobhaxplrOKHI");
								getglobal(rlua_State, "noobhaxmycharOKHI");
								setfield(rlua_State, -2, "Character");
								settop(rlua_State, 0);
								Output(txtbox, "Respawned " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								setglobal(rlua_State, "noobhaxplrOKHI");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxplrOKHI");
								getfield(rlua_State, -1, "Character");
								setglobal(rlua_State, "noobhaxcharOKHI");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Model");
								getglobal(rlua_State, "noobhaxplrOKHI");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxmycharOKHI");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxplrOKHI");
								getfield(rlua_State, -1, "Name");
								getglobal(rlua_State, "noobhaxmycharOKHI");
								pushvalue(rlua_State, -2);
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Humanoid");
								getglobal(rlua_State, "noobhaxmycharOKHI");
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxcharOKHI");
								bool CHAROKXD = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (CHAROKXD) {
									getglobal(rlua_State, "noobhaxcharOKHI");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
								}
								getglobal(rlua_State, "noobhaxplrOKHI");
								getglobal(rlua_State, "noobhaxmycharOKHI");
								setfield(rlua_State, -2, "Character");
								settop(rlua_State, 0);
								Output(txtbox, "Respawned " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "output") {
			bool rISOK = false;
			bool gISOK = false;
			bool bISOK = false;
			int strlenr = strlen(In.at(1).c_str());
			if (In.at(1)[strlenr - 1] == ',') { In.at(1) = In.at(1).substr(0, strlenr - 1); }
			int strleng = strlen(In.at(2).c_str());
			if (In.at(2)[strleng - 1] == ',') { In.at(2) = In.at(2).substr(0, strleng - 1); }
			int strlenb = strlen(In.at(3).c_str());
			if (In.at(3)[strlenb - 1] == ',') { In.at(3) = In.at(3).substr(0, strlenb - 1); }
			if (WhyAreYouDecompilingNoob(In.at(1))) { rISOK = true; }
			if (WhyAreYouDecompilingNoob(In.at(2))) { gISOK = true; }
			if (WhyAreYouDecompilingNoob(In.at(3))) { bISOK = true; }
			if (!rISOK || !gISOK || !bISOK) {
				Output(txtbox, "Check your output rgb arguments...\n");
			}
			else {
				double roknub = atof(In.at(1).c_str());
				double goknub = atof(In.at(2).c_str());
				double boknub = atof(In.at(3).c_str());
				bool boldoknubcaik = false;
				if (isrgb(roknub) && isrgb(goknub) && isrgb(boknub)) {
					if (In.at(4) == "true" || In.at(4) == "1" || In.at(4) == "true," || In.at(4) == "1,") {
						boldoknubcaik = true;
					}
					string newvalue;
					for (int i = 5; i < In.size(); i++) {
						if (i < (In.size() - 1)) {
							newvalue.append(In.at(i) + ' ');
						}
						if (i == (In.size() - 1)) {
							newvalue.append(In.at(i));
						}
					}
					Output(txtbox, newvalue + "\n", RGB(roknub, goknub, boknub), boldoknubcaik);
				}
				else {
					Output(txtbox, "[r] [g] and [b] must be within the range 0-255\n");
				}
			}
		}
		if (In.at(0) == "error") {
			string string;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					string.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					string.append(In.at(i));
				}
			}
			try {
				hitest(1u, (unsigned int)string.c_str());
				Output(txtbox, "Made an error message in the dev console with the text " + string + "\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "info") {
			string string;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					string.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					string.append(In.at(i));
				}
			}
			try {
				hitest(4u, (unsigned int)string.c_str());
				Output(txtbox, "Made an informational message in the dev console with the text " + string + "\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "noobdoll") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobdoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobdoll");
								pushstring(rlua_State, "noobhax Noob doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "noobdoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "noobhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								pushstring(rlua_State, "noobhax_noob_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								pushstring(rlua_State, "rbxassetid://433396052");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								pushstring(rlua_State, "rbxassetid://433396064");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a noob doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobdoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobdoll");
								pushstring(rlua_State, "noobhax Noob doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "noobdoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "noobhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								pushstring(rlua_State, "noobhax_noob_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								pushstring(rlua_State, "rbxassetid://433396052");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								pushstring(rlua_State, "rbxassetid://433396064");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a noob doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "oshawottdoll") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "oshawottdoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottdoll");
								pushstring(rlua_State, "noobhax Oshawott doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "oshawottdoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "oshawotthandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawotthandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "oshawotthandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "oshawottmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								pushstring(rlua_State, "noobhax_oshawott_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								pushstring(rlua_State, "rbxassetid://969521738");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								pushstring(rlua_State, "rbxassetid://969523476");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave an oshawott doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "oshawottdoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottdoll");
								pushstring(rlua_State, "noobhax Oshawott doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "oshawottdoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "oshawotthandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawotthandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "oshawotthandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "oshawottmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								pushstring(rlua_State, "noobhax_oshawott_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								pushstring(rlua_State, "rbxassetid://969521738");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								pushstring(rlua_State, "rbxassetid://969523476");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "oshawottmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave an oshawott doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "signtool") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxsignlol");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlol");
								pushstring(rlua_State, "noobhax sign");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "noobhaxsignlol");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxsignlolhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "noobhaxsignlolhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxsignlolmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								pushstring(rlua_State, "noobhax_sign_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								pushstring(rlua_State, "rbxassetid://1037158");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								pushstring(rlua_State, "rbxassetid://967145205");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 1.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a noobhax sign to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxsignlol");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlol");
								pushstring(rlua_State, "noobhax sign");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "noobhaxsignlol");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxsignlolhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "noobhaxsignlolhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "noobhaxsignlolmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								pushstring(rlua_State, "noobhax_sign_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								pushstring(rlua_State, "rbxassetid://1037158");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								pushstring(rlua_State, "rbxassetid://967145205");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 1.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "noobhaxsignlolmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a noobhax sign to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "chickendoll") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "chickendoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickendoll");
								pushstring(rlua_State, "noobhax Chicken doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "chickendoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "chickenhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "chickenhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "chickenmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								pushstring(rlua_State, "noobhax_chicken_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								pushstring(rlua_State, "rbxassetid://438720618");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								pushstring(rlua_State, "rbxassetid://438720622");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a chicken doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "chickendoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickendoll");
								pushstring(rlua_State, "noobhax Chicken doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "chickendoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "chickenhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "chickenhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "chickenmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								pushstring(rlua_State, "noobhax_chicken_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								pushstring(rlua_State, "rbxassetid://438720618");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								pushstring(rlua_State, "rbxassetid://438720622");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "chickenmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a chicken doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "tepigdoll") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "tepigdoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigdoll");
								pushstring(rlua_State, "noobhax Tepig doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "tepigdoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "tepighandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepighandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Fire");
								getglobal(rlua_State, "tepighandle");
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 12);
								setfield(rlua_State, -2, "Size");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "tepighandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "tepigmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								pushstring(rlua_State, "noobhax_tepig_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								pushstring(rlua_State, "rbxassetid://894825018");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								pushstring(rlua_State, "rbxassetid://894825026");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a tepig doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "tepigdoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigdoll");
								pushstring(rlua_State, "noobhax Tepig doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "tepigdoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "tepighandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepighandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Fire");
								getglobal(rlua_State, "tepighandle");
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 12);
								setfield(rlua_State, -2, "Size");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "tepighandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "tepigmesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								pushstring(rlua_State, "noobhax_tepig_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								pushstring(rlua_State, "rbxassetid://894825018");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								pushstring(rlua_State, "rbxassetid://894825026");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "tepigmesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a tepig doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "snivydoll") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "snivydoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivydoll");
								pushstring(rlua_State, "noobhax Snivy doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "snivydoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "snivyhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivyhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "snivyhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "snivymesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								pushstring(rlua_State, "noobhax_snivy_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								pushstring(rlua_State, "rbxassetid://528336272");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								pushstring(rlua_State, "rbxassetid://528336299");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a snivy doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Tool");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "snivydoll");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivydoll");
								pushstring(rlua_State, "noobhax Snivy doll");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Part");
								getglobal(rlua_State, "snivydoll");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "snivyhandle");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivyhandle");
								pushstring(rlua_State, "Handle");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								getglobal(rlua_State, "snivyhandle");
								pcall(rlua_State, 2, 1, 0);
								setglobal(rlua_State, "snivymesh");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								pushstring(rlua_State, "noobhax_snivy_mesh_ok");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								pushstring(rlua_State, "rbxassetid://528336272");
								setfield(rlua_State, -2, "MeshId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								pushstring(rlua_State, "rbxassetid://528336299");
								setfield(rlua_State, -2, "TextureId");
								settop(rlua_State, 0);
								getglobal(rlua_State, "snivymesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pushnumber(rlua_State, 0.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a snivy doll to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "smoketool") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Tool");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "froggystool");
					settop(rlua_State, 0);
					getglobal(rlua_State, "froggystool");
					pushstring(rlua_State, "noobhax FroggysFriend's SmokeTool");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "froggystool");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "froggyshandle");
					settop(rlua_State, 0);
					getglobal(rlua_State, "froggyshandle");
					pushstring(rlua_State, "Handle");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Smoke");
					getglobal(rlua_State, "froggyshandle");
					pcall(rlua_State, 2, 0, 0);
					settop(rlua_State, 0);
					Output(txtbox, "OK!\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "wait") {
			if (In.size() == 2) {
				int time = atoi(In.at(1).c_str());
				std::this_thread::sleep_for(std::chrono::milliseconds(time * 1000));
			}
		}
		if (In.at(0) == "warn") {
			string string;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					string.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					string.append(In.at(i));
				}
			}
			try {
				getglobal(rlua_State, "warn");
				pushstring(rlua_State, string.c_str());
				pcall(rlua_State, 1, 0, 0);
				settop(rlua_State, 0);
				Output(txtbox, "Warned " + string + "\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "hipheight") {
			if (In.size() == 3) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								float hax = atof(In.at(2).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, hax);
								setfield(rlua_State, -2, "HipHeight");
								settop(rlua_State, 0);
								Output(txtbox, (charname + "'s HipHeight is now " + In.at(2) + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								float hax = atof(In.at(2).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, hax);
								setfield(rlua_State, -2, "HipHeight");
								settop(rlua_State, 0);
								Output(txtbox, (charname + "'s HipHeight is now " + In.at(2) + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "clonetool") {
			string newvalue;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			try {
				getglobal(rlua_State, "noobhaxmyplr");
				getfield(rlua_State, -1, "Backpack");
				getfield(rlua_State, -1, "FindFirstChild");
				pushvalue(rlua_State, -2);
				pushstring(rlua_State, newvalue.c_str());
				pcall(rlua_State, 2, 1, 0);
				bool toolokxd = rlua_toboolean(rlua_State, -1);
				settop(rlua_State, 0);
				if (toolokxd) {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getfield(rlua_State, -1, newvalue.c_str());
					getfield(rlua_State, -1, "Clone");
					pushvalue(rlua_State, -2);
					pcall(rlua_State, 1, 1, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
					Output(txtbox, "Done!\n");
				}
				else {
					Output(txtbox, newvalue + " not found in your Backpack\n");
				}
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "waypoint-remove" || In.at(0) == "waypoint-r") {
			string newvalue;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			getglobal(rlua_State, "tostring");
			pushvalue(rlua_State, -2);
			pcall(rlua_State, 1, 1, 0);
			string idok = rlua_tolstring(rlua_State, -1, NULL);
			settop(rlua_State, 0);
			string newpath = "C:\\noobhax\\" + idok + "\\" + newvalue + ".txt";
			if (!FileExists(newpath.c_str())) {
				Output(txtbox, "Waypoint " + newvalue + " not found\n");
			}
			else {
				string commandxdkk123 = "del C:\\noobhax\\" + idok + "\\" + "\"" + newvalue + ".txt\"";
				system(commandxdkk123.c_str());
				Output(txtbox, "OK!\n");
			}
		}
		if (In.at(0) == "waypoint-tp") {
			string newvalue;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			getglobal(rlua_State, "tostring");
			pushvalue(rlua_State, -2);
			pcall(rlua_State, 1, 1, 0);
			string idok = rlua_tolstring(rlua_State, -1, NULL);
			settop(rlua_State, 0);
			string newpath = "C:\\noobhax\\" + idok + "\\" + newvalue + ".txt";
			if (!FileExists(newpath.c_str())) {
				Output(txtbox, "Waypoint " + newvalue + " not found\n");
			}
			else {
				double array[3];
				int i = 0;
				char cNum[256];
				ifstream infile;
				infile.open(newpath, ifstream::in);
				if (infile.is_open())
				{
					while (infile.good())
					{
						infile.getline(cNum, 256, ',');
						array[i] = atof(cNum);
						i++;
					}
					infile.close();
				}
				else
				{
					Output(txtbox, "Error opening file\n");
				}
				double x = array[0];
				double y = array[1];
				double z = array[2];
				getglobal(rlua_State, "noobhaxmyplr");
				getfield(rlua_State, -1, "Character");
				getfield(rlua_State, -1, "HumanoidRootPart");
				getglobal(rlua_State, "CFrame");
				getfield(rlua_State, -1, "new");
				pushnumber(rlua_State, x);
				pushnumber(rlua_State, y);
				pushnumber(rlua_State, z);
				pcall(rlua_State, 3, 1, 0);
				setfield(rlua_State, -3, "CFrame");
				settop(rlua_State, 0);
				Output(txtbox, "OK!\n");
			}
		}
		if (In.at(0) == "waypoint-add") {
			string newvalue;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			if (!DirectoryExists("C:\\noobhax")) {
				system("md C:\\noobhax");
			}
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "PlaceId");
			getglobal(rlua_State, "tostring");
			pushvalue(rlua_State, -2);
			pcall(rlua_State, 1, 1, 0);
			string idok = rlua_tolstring(rlua_State, -1, NULL);
			settop(rlua_State, 0);
			string newpath = "C:\\noobhax\\" + idok;
			if (!DirectoryExists(newpath.c_str())) {
				string commandxdkk = "md " + newpath;
				system(commandxdkk.c_str());
			}
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Character");
			getfield(rlua_State, -1, "HumanoidRootPart");
			getfield(rlua_State, -1, "Position");
			getglobal(rlua_State, "tostring");
			pushvalue(rlua_State, -2);
			pcall(rlua_State, 1, 1, 0);
			string posok = rlua_tolstring(rlua_State, -1, NULL);
			settop(rlua_State, 0);
			newpath = "C:\\noobhax\\" + idok + "\\" + newvalue + ".txt";
			if (!FileExists(newpath.c_str())) {
				string commandxdkk3 = "echo " + posok + " >" + "\"" + "C:\\noobhax\\" + idok + "\\" + newvalue + ".txt\"";
				system(commandxdkk3.c_str());
				Output(txtbox, "Created the waypoint " + newvalue + "\n");
			}
			else {
				string commandxdkk1 = "del C:\\noobhax\\" + idok + "\\" + "\"" + newvalue + ".txt\"";
				system(commandxdkk1.c_str());
				string commandxdkk3 = "echo " + posok + " >" + "\"" + "C:\\noobhax\\" + idok + "\\" + newvalue + ".txt\"";
				system(commandxdkk3.c_str());
				Output(txtbox, "Created the waypoint " + newvalue + "\n");
			}
		}
		if (In.at(0) == "identity") {
			if (In.size() == 2) {
				ChangeContextLevel(atoi(In.at(1).c_str()));
			}
		}
		if (In.at(0) == "unwatch") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getglobal(rlua_State, "workspace");
								getfield(rlua_State, -1, "CurrentCamera");
								pushvalue(rlua_State, -3);
								setfield(rlua_State, -2, "CameraSubject");
								settop(rlua_State, 0);
								Output(txtbox, "Not watching " + charname + " anymore\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getglobal(rlua_State, "workspace");
								getfield(rlua_State, -1, "CurrentCamera");
								pushvalue(rlua_State, -3);
								setfield(rlua_State, -2, "CameraSubject");
								settop(rlua_State, 0);
								Output(txtbox, "Not watching " + charname + " anymore\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "watch") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getglobal(rlua_State, "workspace");
								getfield(rlua_State, -1, "CurrentCamera");
								pushvalue(rlua_State, -3);
								setfield(rlua_State, -2, "CameraSubject");
								settop(rlua_State, 0);
								Output(txtbox, "Now watching " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getglobal(rlua_State, "workspace");
								getfield(rlua_State, -1, "CurrentCamera");
								pushvalue(rlua_State, -3);
								setfield(rlua_State, -2, "CameraSubject");
								settop(rlua_State, 0);
								Output(txtbox, "Now watching " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "stealtools2") {
			if (In.size() == 3) {
				if (In.at(2) == "all" || In.at(2) == "others") {
					Output(txtbox, "wtf are you trying to do\n");
				}
				else {
					try {
						if (In.at(1) == "all" || In.at(1) == "others") {
							vector<string> noobfaces = getnames(In.at(1));
							int noobokokok2;
							try {
								noobokokok2 = GetPlayer(In.at(2));
							}
							catch (exception) {
								Sleep(1);
							}
							if (noobokokok2 == 0)
							{
								Output(txtbox, In.at(2) + " not found\n");
							}
							else {
								for (int i = 0; i < noobfaces.size(); i++) {
									std::this_thread::sleep_for(100ms);
									try {
										string noob1 = noobfaces[i];
										string noob2 = *GetName(noobokokok2);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noob1.c_str());
										getfield(rlua_State, -1, "Backpack");
										getfield(rlua_State, -1, "FindFirstChildOfClass");
										pushvalue(rlua_State, -2);
										pushstring(rlua_State, "Tool");
										pcall(rlua_State, 2, 1, 0);
										bool oktool = rlua_toboolean(rlua_State, -1);
										settop(rlua_State, 0);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noob1.c_str());
										getfield(rlua_State, -1, "Backpack");
										getfield(rlua_State, -1, "FindFirstChildOfClass");
										pushvalue(rlua_State, -2);
										pushstring(rlua_State, "HopperBin");
										pcall(rlua_State, 2, 1, 0);
										bool okhb = rlua_toboolean(rlua_State, -1);
										settop(rlua_State, 0);
										if (oktool || okhb) {
											try {
												int player = GetPlayer(noob1);
												int backpack = FindFirstClass(player, "Backpack");
												vector<int> tools = GetChildren(backpack);
												for (int i = 0; i < tools.size(); i++) {
													try {
														string okkkk = GetClass(tools[i]);
														bool toolok = okkkk == "Tool";
														bool hbok = okkkk == "HopperBin";
														if (toolok || hbok) {
															try {
																string toolname = *GetName(tools[i]);
																getglobal(rlua_State, "game");
																getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
																getfield(rlua_State, -1, noob1.c_str());
																getfield(rlua_State, -1, "Backpack");
																getfield(rlua_State, -1, toolname.c_str());
																getglobal(rlua_State, "game");
																getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
																getfield(rlua_State, -1, noob2.c_str());
																getfield(rlua_State, -1, "Backpack");
																setfield(rlua_State, -5, "Parent");
																settop(rlua_State, 0);
																Output(txtbox, "Stole " + noob1 + "'s " + toolname + " and gave it to " + noob2 + "\n");
															}
															catch (exception) {
																Sleep(1);
															}
														}
														else {
															Sleep(1);
														}
													}
													catch (exception) {
														Sleep(1);
													}
												}
											}
											catch (exception) {
												Sleep(1);
											}
										}
										else {
											Output(txtbox, noob1 + " doesn't have any tool\n");
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
							}
						}
						else {
							int noobokokok;
							int noobokokok2;
							try {
								noobokokok = GetPlayer(In.at(1));
								noobokokok2 = GetPlayer(In.at(2));
							}
							catch (exception) {
								Sleep(1);
							}
							if (noobokokok == 0) {
								Output(txtbox, In.at(1) + " not found\n");
							}
							if (noobokokok2 == 0)
							{
								Output(txtbox, In.at(2) + " not found\n");
							}
							if (noobokokok != 0 && noobokokok2 != 0) {
								try {
									string noob1 = *GetName(noobokokok);
									string noob2 = *GetName(noobokokok2);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noob1.c_str());
									getfield(rlua_State, -1, "Backpack");
									getfield(rlua_State, -1, "FindFirstChildOfClass");
									pushvalue(rlua_State, -2);
									pushstring(rlua_State, "Tool");
									pcall(rlua_State, 2, 1, 0);
									bool oktool = rlua_toboolean(rlua_State, -1);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noob1.c_str());
									getfield(rlua_State, -1, "Backpack");
									getfield(rlua_State, -1, "FindFirstChildOfClass");
									pushvalue(rlua_State, -2);
									pushstring(rlua_State, "HopperBin");
									pcall(rlua_State, 2, 1, 0);
									bool okhb = rlua_toboolean(rlua_State, -1);
									settop(rlua_State, 0);
									if (oktool || okhb) {
										try {
											int player = GetPlayer(noob1);
											int backpack = FindFirstClass(player, "Backpack");
											vector<int> tools = GetChildren(backpack);
											for (int i = 0; i < tools.size(); i++) {
												try {
													string okkkk = GetClass(tools[i]);
													bool toolok = okkkk == "Tool";
													bool hbok = okkkk == "HopperBin";
													if (toolok || hbok) {
														try {
															string toolname = *GetName(tools[i]);
															getglobal(rlua_State, "game");
															getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
															getfield(rlua_State, -1, noob1.c_str());
															getfield(rlua_State, -1, "Backpack");
															getfield(rlua_State, -1, toolname.c_str());
															getglobal(rlua_State, "game");
															getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
															getfield(rlua_State, -1, noob2.c_str());
															getfield(rlua_State, -1, "Backpack");
															setfield(rlua_State, -5, "Parent");
															settop(rlua_State, 0);
															Output(txtbox, "Stole " + noob1 + "'s " + toolname + " and gave it to " + noob2 + "\n");
														}
														catch (exception) {
															Sleep(1);
														}
													}
													else {
														Sleep(1);
													}
												}
												catch (exception) {
													Sleep(1);
												}
											}
										}
										catch (exception) {
											Sleep(1);
										}
									}
									else {
										Output(txtbox, noob1 + " doesn't have any tool\n");
									}
								}
								catch (exception) {
									Sleep(1);
								}
							}
						}
					}
					catch (exception) {
						Sleep(1);
					}
				}
			}
		}
		if (In.at(0) == "savetool") {
			string newvalue;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			try {
				getglobal(rlua_State, "noobhaxmyplr");
				getfield(rlua_State, -1, "Backpack");
				getfield(rlua_State, -1, "FindFirstChild");
				pushvalue(rlua_State, -2);
				pushstring(rlua_State, newvalue.c_str());
				pcall(rlua_State, 2, 1, 0);
				bool toolokxd = rlua_toboolean(rlua_State, -1);
				settop(rlua_State, 0);
				if (toolokxd) {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getfield(rlua_State, -1, newvalue.c_str());
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "StarterGear");
					setfield(rlua_State, -3, "Parent");
					settop(rlua_State, 0);
					Output(txtbox, "Done!\n");
				}
				else {
					Output(txtbox, newvalue + " not found in your Backpack\n");
				}
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "printtools") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getfield(rlua_State, -1, "FindFirstChildOfClass");
					pushvalue(rlua_State, -2);
					pushstring(rlua_State, "Tool");
					pcall(rlua_State, 2, 1, 0);
					bool oktool = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getfield(rlua_State, -1, "FindFirstChildOfClass");
					pushvalue(rlua_State, -2);
					pushstring(rlua_State, "HopperBin");
					pcall(rlua_State, 2, 1, 0);
					bool okhb = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (oktool || okhb) {
						try {
							Output(txtbox, "LocalPlayer's tools:\n");
							getglobal(rlua_State, "print");
							pushstring(rlua_State, "LocalPlayer's tools:\n");
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
							int player = GetLocalPlayer(Players);
							int backpack = FindFirstClass(player, "Backpack");
							vector<int> tools = GetChildren(backpack);
							for (int i = 0; i < tools.size(); i++) {
								try {
									string okkkk = GetClass(tools[i]);
									bool toolok = okkkk == "Tool";
									bool hbok = okkkk == "HopperBin";
									if (toolok || hbok) {
										string toolname = *GetName(tools[i]);
										Output(txtbox, toolname + "\n");
										getglobal(rlua_State, "print");
										pushstring(rlua_State, toolname.c_str());
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
									}
									else {
										Sleep(1);
									}
								}
								catch (exception) {
									Sleep(1);
								}
							}
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You don't have any tool\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unbox") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "SelectionBox");
								pcall(rlua_State, 2, 1, 0);
								bool sb = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (sb) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "SelectionBox");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s SelectionBox\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							string charname = *GetName(noobokokok);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, charname.c_str());
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "FindFirstChild");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "SelectionBox");
							pcall(rlua_State, 2, 1, 0);
							bool sb = rlua_toboolean(rlua_State, -1);
							settop(rlua_State, 0);
							if (sb) {
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "SelectionBox");
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Removed " + charname + "'s SelectionBox\n");
							}
							else {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "box") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SelectionBox");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushvalue(rlua_State, -3);
								setfield(rlua_State, -2, "Adornee");
								settop(rlua_State, 0);
								Output(txtbox, "Gave " + charname + " a SelectionBox\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SelectionBox");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushvalue(rlua_State, -3);
								setfield(rlua_State, -2, "Adornee");
								settop(rlua_State, 0);
								Output(txtbox, "Gave " + charname + " a SelectionBox\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unblur") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					getfield(rlua_State, -1, "FindFirstChild");
					pushvalue(rlua_State, -2);
					pushstring(rlua_State, "Blur");
					pcall(rlua_State, 2, 1, 0);
					bool blurfound = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (blurfound) {
						getglobal(rlua_State, "game");
						getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
						getfield(rlua_State, -1, "Blur");
						getfield(rlua_State, -1, "remove");
						pushvalue(rlua_State, -2);
						pcall(rlua_State, 1, 0, 0);
						settop(rlua_State, 0);
						Output(txtbox, "Removed Blur from the Lighting\n");
					}
					else {
						Sleep(1);
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "fixl" || In.at(0) == "fixlighting" || In.at(0) == "fix") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					getfield(rlua_State, -1, "ClearAllChildren");
					pushvalue(rlua_State, -2);
					pcall(rlua_State, 1, 0, 0);
					settop(rlua_State, 0);
					Output(txtbox, "Removed Lighting children\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "blur") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "BlurEffect");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					settop(rlua_State, 0);
					Output(txtbox, "Made a BlurEffect in the Lighting\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "printpos") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								bool charOK = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (charOK) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "Health");
									int healthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "MaxHealth");
									int maxhealthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									if (healthOK > 0 || to_string(maxhealthOK) == "-2147483648") {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "HumanoidRootPart");
										getfield(rlua_State, -1, "Position");
										getglobal(rlua_State, "tostring");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 1, 0);
										string posok = rlua_tolstring(rlua_State, -1, NULL);
										settop(rlua_State, 0);
										getglobal(rlua_State, "print");
										pushstring(rlua_State, (charname + "'s Position: " + posok).c_str());
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
										Output(txtbox, charname + "'s Position: " + posok + "\n");
									}
									else {
										Sleep(1);
									}
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								bool charOK = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (charOK) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "Health");
									int healthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "MaxHealth");
									int maxhealthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									if (healthOK > 0 || to_string(maxhealthOK) == "-2147483648") {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "HumanoidRootPart");
										getfield(rlua_State, -1, "Position");
										getglobal(rlua_State, "tostring");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 1, 0);
										string posok = rlua_tolstring(rlua_State, -1, NULL);
										settop(rlua_State, 0);
										getglobal(rlua_State, "print");
										pushstring(rlua_State, (charname + "'s Position: " + posok).c_str());
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
										Output(txtbox, charname + "'s Position: " + posok + "\n");
									}
									else {
										Sleep(1);
									}
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "removetools") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getfield(rlua_State, -1, "ClearAllChildren");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Removed " + charname + "'s tools (if he/she had any)\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getfield(rlua_State, -1, "ClearAllChildren");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Removed " + charname + "'s tools (if he/she had any)\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "tptopos") {
			if (In.size() == 5) {
				float xpos = atof(In.at(2).c_str());
				float ypos = atof(In.at(3).c_str());
				float zpos = atof(In.at(4).c_str());
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								bool charOK = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (charOK) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "Health");
									int healthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "MaxHealth");
									int maxhealthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									if (healthOK > 0 || to_string(maxhealthOK) == "-2147483648") {
										getglobal(rlua_State, "Vector3");
										getfield(rlua_State, -1, "new");
										pushnumber(rlua_State, xpos);
										pushnumber(rlua_State, ypos);
										pushnumber(rlua_State, zpos);
										pcall(rlua_State, 3, 1, 0);
										getglobal(rlua_State, "CFrame");
										getfield(rlua_State, -1, "new");
										pushvalue(rlua_State, -3);
										pcall(rlua_State, 1, 1, 0);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "HumanoidRootPart");
										pushvalue(rlua_State, -6);
										setfield(rlua_State, -2, "CFrame");
										settop(rlua_State, 0);
										Output(txtbox, "Teleported " + charname + " to the position " + In.at(2) + ", " + In.at(3) + ", " + In.at(4) + "\n");
									}
									else {
										Sleep(1);
									}
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								bool charOK = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (charOK) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "Health");
									int healthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "MaxHealth");
									int maxhealthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									if (healthOK > 0 || to_string(maxhealthOK) == "-2147483648") {
										getglobal(rlua_State, "Vector3");
										getfield(rlua_State, -1, "new");
										pushnumber(rlua_State, xpos);
										pushnumber(rlua_State, ypos);
										pushnumber(rlua_State, zpos);
										pcall(rlua_State, 3, 1, 0);
										getglobal(rlua_State, "CFrame");
										getfield(rlua_State, -1, "new");
										pushvalue(rlua_State, -3);
										pcall(rlua_State, 1, 1, 0);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "HumanoidRootPart");
										pushvalue(rlua_State, -6);
										setfield(rlua_State, -2, "CFrame");
										settop(rlua_State, 0);
										Output(txtbox, "Teleported " + charname + " to the position " + to_string(xpos) + ", " + to_string(ypos) + ", " + to_string(zpos) + "\n");
									}
									else {
										Sleep(1);
									}
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "drophats") {
			if (In.size() == 2) {
				Output(txtbox, "Do drophats instead\n");
			}
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					bool charokornot = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (charokornot) {
						int Character = *(int*)(GetLocalPlayer(Players) + 88);
						vector<int> childrenok = GetChildren(Character);
						for (int i = 0; i < childrenok.size(); i++) {
							string noobk = *GetName(childrenok[i]);
							string classnameknoob = GetClass(childrenok[i]);
							if (classnameknoob == "Hat" || classnameknoob == "Accessory") {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, noobk.c_str());
								getglobal(rlua_State, "workspace");
								setfield(rlua_State, -2, "Parent");
								settop(rlua_State, 0);
								Output(txtbox, "Dropped " + noobk + "\n");
							}
							else {
								Sleep(1);
							}
						}
					}
					else {
						Sleep(1);
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "removehats") {
			if (In.size() == 2) {
				Output(txtbox, "Do removehats instead\n");
			}
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					bool charokornot = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (charokornot) {
						int Character = *(int*)(GetLocalPlayer(Players) + 88);
						vector<int> childrenok = GetChildren(Character);
						for (int i = 0; i < childrenok.size(); i++) {
							string noobk = *GetName(childrenok[i]);
							string classnameknoob = GetClass(childrenok[i]);
							if (classnameknoob == "Hat" || classnameknoob == "Accessory") {
								getglobal(rlua_State, "noobhaxmyplr");
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, noobk.c_str());
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Removed " + noobk + "\n");
							}
							else {
								Sleep(1);
							}
						}
					}
					else {
						Sleep(1);
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "fegod") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "FindFirstChild");
					pushvalue(rlua_State, -2);
					pushstring(rlua_State, "Humanoid");
					pcall(rlua_State, 2, 1, 0);
					bool humanoidfound = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (humanoidfound) {
						try {
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "Humanoid");
							getfield(rlua_State, -1, "remove");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							getglobal(rlua_State, "Instance");
							getfield(rlua_State, -1, "new");
							pushstring(rlua_State, "Humanoid");
							pushvalue(rlua_State, -4);
							pcall(rlua_State, 2, 0, 0);
							settop(rlua_State, 0);
							Output(txtbox, "OK! To respawn use the respawn command\n");
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Sleep(1);
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "rmusic") {
			if (In.size() == 1) {
				try {
					vector<int> wschildrenk = GetChildren(Workspace);
					for (int i = 0; i < wschildrenk.size(); i++) {
						try {
							string wschildname = *GetName(wschildrenk[i]);
							string okkkk = GetClass(wschildrenk[i]);
							if (okkkk == "Sound") {
								try {
									getglobal(rlua_State, "workspace");
									getfield(rlua_State, -1, wschildname.c_str());
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + wschildname + " from the Workspace\n");
								}
								catch (exception) {
									Sleep(1);
								}

							}
							else {
								Sleep(1);
							}
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "age") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "AccountAge");
								int ageok = rlua_tonumber(rlua_State, -1);
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "userId");
								int useridok = rlua_tonumber(rlua_State, -1);
								settop(rlua_State, 0);
								Output(txtbox, charname + "'s Age (userId " + to_string(useridok) + "): " + to_string(ageok) + " days (" + to_string(ageok / 365) + " years)\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "AccountAge");
								int ageok = rlua_tonumber(rlua_State, -1);
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "userId");
								int useridok = rlua_tonumber(rlua_State, -1);
								settop(rlua_State, 0);
								Output(txtbox, charname + "'s Age (userId " + to_string(useridok) + "): " + to_string(ageok) + " days (" + to_string(ageok / 365) + " years)\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "thirdperson") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "CameraMode");
								settop(rlua_State, 0);
								Output(txtbox, "Set " + charname + "'s Camera to third person\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "CameraMode");
								settop(rlua_State, 0);
								Output(txtbox, "Set " + charname + "'s Camera to third person\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "firstperson") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "CameraMode");
								settop(rlua_State, 0);
								Output(txtbox, "Set " + charname + "'s Camera to first person\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "CameraMode");
								settop(rlua_State, 0);
								Output(txtbox, "Set " + charname + "'s Camera to first person\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "mouselock") {
			if (In.size() == 3) {
				if (In.at(2) == "0" || In.at(2) == "false" || In.at(2) == "1" || In.at(2) == "true") {
					try {
						if (In.at(1) == "all" || In.at(1) == "others") {
							vector<string> noobfaces = getnames(In.at(1));
							for (int i = 0; i < noobfaces.size(); i++) {

								try {
									string charname = noobfaces[i];
									int boolok = 0;
									if (In.at(2) == "0" || In.at(2) == "false") {
										boolok = 0;
									}
									if (In.at(2) == "1" || In.at(2) == "true") {
										boolok = 1;
									}
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									pushboolean(rlua_State, boolok);
									setfield(rlua_State, -2, "DevEnableMouseLock");
									settop(rlua_State, 0);
									string lolnewbool = "";
									if (boolok == 1) {
										lolnewbool = "true";
									}
									if (boolok == 0) {
										lolnewbool = "false";
									}
									Output(txtbox, "Set " + charname + "'s MouseLock to " + lolnewbool + "\n");
								}
								catch (exception) {
									Sleep(1);
								}
							}
						}
						else {
							int noobokokok;
							try {
								noobokokok = GetPlayer(In.at(1));
							}
							catch (exception) {
								Sleep(1);
							}
							if (noobokokok == 0) {
								Output(txtbox, In.at(1) + " not found\n");
							}
							else {
								try {
									string charname = *GetName(noobokokok);
									int boolok = 0;
									if (In.at(2) == "0" || In.at(2) == "false") {
										boolok = 0;
									}
									if (In.at(2) == "1" || In.at(2) == "true") {
										boolok = 1;
									}
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									pushboolean(rlua_State, boolok);
									setfield(rlua_State, -2, "DevEnableMouseLock");
									settop(rlua_State, 0);
									string lolnewbool = "";
									if (boolok == 1) {
										lolnewbool = "true";
									}
									if (boolok == 0) {
										lolnewbool = "false";
									}
									Output(txtbox, "Set " + charname + "'s MouseLock to " + lolnewbool + "\n");
								}
								catch (exception) {
									Sleep(1);
								}
							}
						}
					}
					catch (exception) {
						Sleep(1);
					}
				}
			}
			else {
				Sleep(1);
			}
		}
		if (In.at(0) == "ssjgod") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "ssjgod_noobhax");
								pcall(rlua_State, 2, 1, 0);
								bool isssjgod = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isssjgod == false) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getglobal(rlua_State, "Instance");
									getfield(rlua_State, -1, "new");
									pushstring(rlua_State, "ParticleEmitter");
									pushvalue(rlua_State, -4);
									pcall(rlua_State, 2, 1, 0);
									pushstring(rlua_State, "ssjgod_noobhax");
									setfield(rlua_State, -2, "Name");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgod_noobhax");
									getglobal(rlua_State, "NumberSequence");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 15);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Size");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgod_noobhax");
									pushstring(rlua_State, "rbxassetid://288406230");
									setfield(rlua_State, -2, "Texture");
									pushnumber(rlua_State, 2);
									setfield(rlua_State, -2, "Rate");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgod_noobhax");
									getglobal(rlua_State, "NumberRange");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 5);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Speed");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgod_noobhax");
									pushnumber(rlua_State, 20);
									setfield(rlua_State, -2, "VelocitySpread");
									settop(rlua_State, 0);
									Output(txtbox, charname + " IS NOW A SUPER SAIYAN GOD\n");
								}
								else {
									try {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Name");
										string charnamelolkk = rlua_tolstring(rlua_State, -1, NULL);
										settop(rlua_State, 0);
										int character = GetCharacter(charnamelolkk);
										int torso = FindFirstChild_PartOf(character, "Torso");
										vector<int> fuckingtorsochildren = GetChildren(torso);
										for (int i = 0; i < fuckingtorsochildren.size(); i++) {
											try {
												if (*GetName(fuckingtorsochildren[i]) == "ssjgod_noobhax") {
													getglobal(rlua_State, "game");
													getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
													getfield(rlua_State, -1, charname.c_str());
													getfield(rlua_State, -1, "Character");
													getfield(rlua_State, -1, "Torso");
													getfield(rlua_State, -1, "ssjgod_noobhax");
													getfield(rlua_State, -1, "remove");
													pushvalue(rlua_State, -2);
													pcall(rlua_State, 1, 0, 0);
													settop(rlua_State, 0);
												}
												else {
													Sleep(1);
												}
											}
											catch (exception) {
												Sleep(1);
											}
										}
										try {
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getglobal(rlua_State, "Instance");
											getfield(rlua_State, -1, "new");
											pushstring(rlua_State, "ParticleEmitter");
											pushvalue(rlua_State, -4);
											pcall(rlua_State, 2, 1, 0);
											pushstring(rlua_State, "ssjgod_noobhax");
											setfield(rlua_State, -2, "Name");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgod_noobhax");
											getglobal(rlua_State, "NumberSequence");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 15);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Size");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgod_noobhax");
											pushstring(rlua_State, "rbxassetid://288406230");
											setfield(rlua_State, -2, "Texture");
											pushnumber(rlua_State, 2);
											setfield(rlua_State, -2, "Rate");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgod_noobhax");
											getglobal(rlua_State, "NumberRange");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 5);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Speed");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgod_noobhax");
											pushnumber(rlua_State, 20);
											setfield(rlua_State, -2, "VelocitySpread");
											settop(rlua_State, 0);
											Output(txtbox, charname + " IS NOW A SUPER SAIYAN GOD\n");
										}
										catch (exception) {
											Sleep(1);
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "ssjgod_noobhax");
								pcall(rlua_State, 2, 1, 0);
								bool isssjgod = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isssjgod == false) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getglobal(rlua_State, "Instance");
									getfield(rlua_State, -1, "new");
									pushstring(rlua_State, "ParticleEmitter");
									pushvalue(rlua_State, -4);
									pcall(rlua_State, 2, 1, 0);
									pushstring(rlua_State, "ssjgod_noobhax");
									setfield(rlua_State, -2, "Name");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgod_noobhax");
									getglobal(rlua_State, "NumberSequence");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 15);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Size");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgod_noobhax");
									pushstring(rlua_State, "rbxassetid://288406230");
									setfield(rlua_State, -2, "Texture");
									pushnumber(rlua_State, 2);
									setfield(rlua_State, -2, "Rate");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgod_noobhax");
									getglobal(rlua_State, "NumberRange");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 5);
									pcall(rlua_State, 1, 1, 0);
									setfield(rlua_State, -3, "Speed");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ssjgod_noobhax");
									pushnumber(rlua_State, 20);
									setfield(rlua_State, -2, "VelocitySpread");
									settop(rlua_State, 0);
									Output(txtbox, charname + " IS NOW A SUPER SAIYAN GOD\n");
								}
								else {
									try {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Name");
										string charnamelolkk = rlua_tolstring(rlua_State, -1, NULL);
										settop(rlua_State, 0);
										int character = GetCharacter(charnamelolkk);
										int torso = FindFirstChild_PartOf(character, "Torso");
										vector<int> fuckingtorsochildren = GetChildren(torso);
										for (int i = 0; i < fuckingtorsochildren.size(); i++) {
											try {
												if (*GetName(fuckingtorsochildren[i]) == "ssjgod_noobhax") {
													getglobal(rlua_State, "game");
													getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
													getfield(rlua_State, -1, charname.c_str());
													getfield(rlua_State, -1, "Character");
													getfield(rlua_State, -1, "Torso");
													getfield(rlua_State, -1, "ssjgod_noobhax");
													getfield(rlua_State, -1, "remove");
													pushvalue(rlua_State, -2);
													pcall(rlua_State, 1, 0, 0);
													settop(rlua_State, 0);
												}
												else {
													Sleep(1);
												}
											}
											catch (exception) {
												Sleep(1);
											}
										}
										try {
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getglobal(rlua_State, "Instance");
											getfield(rlua_State, -1, "new");
											pushstring(rlua_State, "ParticleEmitter");
											pushvalue(rlua_State, -4);
											pcall(rlua_State, 2, 1, 0);
											pushstring(rlua_State, "ssjgod_noobhax");
											setfield(rlua_State, -2, "Name");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgod_noobhax");
											getglobal(rlua_State, "NumberSequence");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 15);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Size");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgod_noobhax");
											pushstring(rlua_State, "rbxassetid://288406230");
											setfield(rlua_State, -2, "Texture");
											pushnumber(rlua_State, 2);
											setfield(rlua_State, -2, "Rate");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgod_noobhax");
											getglobal(rlua_State, "NumberRange");
											getfield(rlua_State, -1, "new");
											pushnumber(rlua_State, 5);
											pcall(rlua_State, 1, 1, 0);
											setfield(rlua_State, -3, "Speed");
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, charname.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Torso");
											getfield(rlua_State, -1, "ssjgod_noobhax");
											pushnumber(rlua_State, 20);
											setfield(rlua_State, -2, "VelocitySpread");
											settop(rlua_State, 0);
											Output(txtbox, charname + " IS NOW A SUPER SAIYAN GOD\n");
										}
										catch (exception) {
											Sleep(1);
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "clrworkspace") {
			if (In.size() == 1) {
				try {
					vector<int> wschildren = GetChildren(Workspace);
					for (int i = 0; i < wschildren.size(); i++) {
						try {
							string wschildname = *GetName(wschildren[i]);
							if (wschildname != "Terrain" && wschildname != "Camera")
							{
								try {
									vector<string> playernames = getnames("all");
									vector<string> charnames;
									for (int i = 0; i < playernames.size(); i++) {
										try {
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, playernames[i].c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Name");
											string charnamek = rlua_tolstring(rlua_State, -1, NULL);
											settop(rlua_State, 0);
											charnames.push_back(charnamek);
										}
										catch (exception) {
											Sleep(1);
										}
									}
									bool isacharplayer = false;
									for (int i = 0; i < charnames.size(); i++) {
										if (charnames[i] == wschildname) {
											isacharplayer = true;
										}
									}
									if (isacharplayer == false) {
										getglobal(rlua_State, "workspace");
										getfield(rlua_State, -1, wschildname.c_str());
										getfield(rlua_State, -1, "ClassName");
										string classnameok = rlua_tolstring(rlua_State, -1, NULL);
										settop(rlua_State, 0);
										getglobal(rlua_State, "workspace");
										getfield(rlua_State, -1, wschildname.c_str());
										getfield(rlua_State, -1, "remove");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
										Output(txtbox, "Removed " + wschildname + " (" + classnameok + ") from the Workspace\n");
									}
									else {
										Sleep(1);
									}
								}
								catch (exception) {
									Sleep(1);
								}
							}
							else {
								Sleep(1);
							}
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "heal") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getfield(rlua_State, -1, "MaxHealth");
								setfield(rlua_State, -7, "Health");
								settop(rlua_State, 0);
								Output(txtbox, "Healed " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getfield(rlua_State, -1, "MaxHealth");
								setfield(rlua_State, -7, "Health");
								settop(rlua_State, 0);
								Output(txtbox, "Healed " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "swordreach") {
			if (In.size() == 1) {
				try {
					int player = GetLocalPlayer(Players);
					int backpack = FindFirstClass(player, "Backpack");
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getfield(rlua_State, -1, "FindFirstChildOfClass");
					pushvalue(rlua_State, -2);
					pushstring(rlua_State, "Tool");
					pcall(rlua_State, 2, 1, 0);
					bool oktool = rlua_toboolean(rlua_State, -1);
					settop(rlua_State, 0);
					if (oktool) {
						string swordsame = "";
						bool eeqqqqqqq = false;
						vector<int> awedinwiowenf = GetChildren(backpack);
						for (int i = 0; i < awedinwiowenf.size(); i++) {
							if ((*GetName(awedinwiowenf[i])).find("Sword") != std::string::npos) {
								swordsame = *GetName(awedinwiowenf[i]);
								eeqqqqqqq = true;
							}
						}
						if (eeqqqqqqq) {
							string fullswordname = swordsame;
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Backpack");
							getfield(rlua_State, -1, "FindFirstChild");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, fullswordname.c_str());
							pcall(rlua_State, 2, 1, 0);
							bool found = rlua_toboolean(rlua_State, -1);
							settop(rlua_State, 0);
							if (found) {
								try {
									getglobal(rlua_State, "noobhaxmyplr");
									getfield(rlua_State, -1, "Backpack");
									getfield(rlua_State, -1, fullswordname.c_str());
									getfield(rlua_State, -1, "Handle");
									getglobal(rlua_State, "Vector3");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 0.01);
									pushnumber(rlua_State, 0.01);
									pushnumber(rlua_State, 80);
									pcall(rlua_State, 3, 1, 0);
									setfield(rlua_State, -3, "Size");
									settop(rlua_State, 0);
									getglobal(rlua_State, "noobhaxmyplr");
									getfield(rlua_State, -1, "Backpack");
									getfield(rlua_State, -1, fullswordname.c_str());
									getfield(rlua_State, -1, "Handle");
									getglobal(rlua_State, "Instance");
									getfield(rlua_State, -1, "new");
									pushstring(rlua_State, "SelectionBox");
									pushvalue(rlua_State, -4);
									pcall(rlua_State, 2, 1, 0);
									getglobal(rlua_State, "noobhaxmyplr");
									getfield(rlua_State, -1, "Backpack");
									getfield(rlua_State, -1, fullswordname.c_str());
									getfield(rlua_State, -1, "Handle");
									setfield(rlua_State, -5, "Adornee");
									settop(rlua_State, 0);
									Output(txtbox, "OK!\n");
								}
								catch (exception) {
									Sleep(1);
								}
							}
							else {
								Output(txtbox, "Sword not found\n");
							}
						}
						else {
							Output(txtbox, "Sword not found\n");
						}
					}
					else {
						Output(txtbox, "Sword not found\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "stealtools") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {
							std::this_thread::sleep_for(100ms);
							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getfield(rlua_State, -1, "FindFirstChildOfClass");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Tool");
								pcall(rlua_State, 2, 1, 0);
								bool oktool = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getfield(rlua_State, -1, "FindFirstChildOfClass");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "HopperBin");
								pcall(rlua_State, 2, 1, 0);
								bool okhb = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (oktool || okhb) {
									try {
										int player = GetPlayer(charname);
										int backpack = FindFirstClass(player, "Backpack");
										vector<int> tools = GetChildren(backpack);
										for (int i = 0; i < tools.size(); i++) {
											try {
												string okkkk = GetClass(tools[i]);
												bool toolok = okkkk == "Tool";
												bool hbok = okkkk == "HopperBin";
												if (toolok || hbok) {
													try {
														string toolname = *GetName(tools[i]);
														getglobal(rlua_State, "game");
														getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
														getfield(rlua_State, -1, charname.c_str());
														getfield(rlua_State, -1, "Backpack");
														getfield(rlua_State, -1, toolname.c_str());
														getglobal(rlua_State, "noobhaxmyplr");
														getfield(rlua_State, -1, "Backpack");
														setfield(rlua_State, -3, "Parent");
														settop(rlua_State, 0);
														Output(txtbox, "Stole " + charname + "'s " + toolname + "\n");
													}
													catch (exception) {
														Sleep(1);
													}
												}
												else {
													Sleep(1);
												}
											}
											catch (exception) {
												Sleep(1);
											}
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
								else {
									Output(txtbox, charname + " doesn't have any tool\n");
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getfield(rlua_State, -1, "FindFirstChildOfClass");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Tool");
								pcall(rlua_State, 2, 1, 0);
								bool oktool = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getfield(rlua_State, -1, "FindFirstChildOfClass");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "HopperBin");
								pcall(rlua_State, 2, 1, 0);
								bool okhb = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (oktool || okhb) {
									try {
										int player = GetPlayer(charname);
										int backpack = FindFirstClass(player, "Backpack");
										vector<int> tools = GetChildren(backpack);
										for (int i = 0; i < tools.size(); i++) {
											try {
												string okkkk = GetClass(tools[i]);
												bool toolok = okkkk == "Tool";
												bool hbok = okkkk == "HopperBin";
												if (toolok || hbok) {
													try {
														string toolname = *GetName(tools[i]);
														getglobal(rlua_State, "game");
														getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
														getfield(rlua_State, -1, charname.c_str());
														getfield(rlua_State, -1, "Backpack");
														getfield(rlua_State, -1, toolname.c_str());
														getglobal(rlua_State, "noobhaxmyplr");
														getfield(rlua_State, -1, "Backpack");
														setfield(rlua_State, -3, "Parent");
														settop(rlua_State, 0);
														Output(txtbox, "Stole " + charname + "'s " + toolname + "\n");
													}
													catch (exception) {
														Sleep(1);
													}
												}
												else {
													Sleep(1);
												}
											}
											catch (exception) {
												Sleep(1);
											}
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
								else {
									Output(txtbox, charname + " doesn't have any tool\n");
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "brightness") {
			if (In.size() == 2) {
				try {
					float hax = atof(In.at(1).c_str());
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					pushnumber(rlua_State, hax);
					setfield(rlua_State, -2, "Brightness");
					settop(rlua_State, 0);
					Output(txtbox, "Set Brightness to " + In.at(1) + "\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "fencingreach") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "PlaceId");
					int placeid = rlua_tonumber(rlua_State, -1);
					settop(rlua_State, 0);
					if (placeid == 12109643) {
						try {
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Backpack");
							getfield(rlua_State, -1, "FindFirstChild");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "Foil");
							pcall(rlua_State, 2, 1, 0);
							bool found = rlua_toboolean(rlua_State, -1);
							settop(rlua_State, 0);
							if (found) {
								try {
									getglobal(rlua_State, "noobhaxmyplr");
									getfield(rlua_State, -1, "Backpack");
									getfield(rlua_State, -1, "Foil");
									getfield(rlua_State, -1, "Handle");
									getglobal(rlua_State, "Vector3");
									getfield(rlua_State, -1, "new");
									pushnumber(rlua_State, 0.01);
									pushnumber(rlua_State, 0.01);
									pushnumber(rlua_State, 80);
									pcall(rlua_State, 3, 1, 0);
									setfield(rlua_State, -3, "Size");
									settop(rlua_State, 0);
									getglobal(rlua_State, "noobhaxmyplr");
									getfield(rlua_State, -1, "Backpack");
									getfield(rlua_State, -1, "Foil");
									getfield(rlua_State, -1, "Handle");
									getglobal(rlua_State, "Instance");
									getfield(rlua_State, -1, "new");
									pushstring(rlua_State, "SelectionBox");
									pushvalue(rlua_State, -4);
									pcall(rlua_State, 2, 1, 0);
									getglobal(rlua_State, "noobhaxmyplr");
									getfield(rlua_State, -1, "Backpack");
									getfield(rlua_State, -1, "Foil");
									getfield(rlua_State, -1, "Handle");
									setfield(rlua_State, -5, "Adornee");
									settop(rlua_State, 0);
									Output(txtbox, "OK!\n");
								}
								catch (exception) {
									Sleep(1);
								}
							}
							else {
								Output(txtbox, "Foil not found\n");
							}
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Output(txtbox, "You aren't on Fencing\n");
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "lock") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								Output(txtbox, "Locked " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								Output(txtbox, "Locked " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unlock") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								Output(txtbox, "Unlocked " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Locked");
								settop(rlua_State, 0);
								Output(txtbox, "Unlocked " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unnoob") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "CanLoadCharacterAppearance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Health");
								settop(rlua_State, 0);
								Output(txtbox, charname + " isn't a noob anymore :(\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "CanLoadCharacterAppearance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Health");
								settop(rlua_State, 0);
								Output(txtbox, charname + " isn't a noob anymore :(\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "noob") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "CanLoadCharacterAppearance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Health");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now a noob, LOL POOR NOOB\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "CanLoadCharacterAppearance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Health");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now a noob, LOL POOR NOOB\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "goto" || In.at(0) == "to") {
			if (In.size() == 2) {
				if (In.at(1) == "others" || In.at(1) == "all") {
					Output(txtbox, "are you drunk\n");
				}
				else {
					int noobokokok;
					try {
						noobokokok = GetPlayer(In.at(1));
					}
					catch (exception) {
						Sleep(1);
					}
					if (noobokokok == 0) {
						Output(txtbox, In.at(1) + " not found\n");
					}
					else {
						try {
							string charname = *GetName(noobokokok);
							getglobal(rlua_State, "noobhaxmyplr");
							getfield(rlua_State, -1, "Character");
							bool charmeOK = rlua_toboolean(rlua_State, -1);
							settop(rlua_State, 0);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, charname.c_str());
							getfield(rlua_State, -1, "Character");
							bool charOK = rlua_toboolean(rlua_State, -1);
							settop(rlua_State, 0);
							if (charmeOK && charOK) {
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getfield(rlua_State, -1, "Health");
								int healthOK = rlua_tonumber(rlua_State, -1);
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getfield(rlua_State, -1, "MaxHealth");
								int maxhealthOK = rlua_tonumber(rlua_State, -1);
								settop(rlua_State, 0);
								if (healthOK > 0 || to_string(maxhealthOK) == "-2147483648") {
									getglobal(rlua_State, "noobhaxmyplr");
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "HumanoidRootPart");
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "HumanoidRootPart");
									getfield(rlua_State, -1, "CFrame");
									setfield(rlua_State, -7, "CFrame");
									settop(rlua_State, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Teleported LocalPlayer to " + charname + "\n");
								}
								else {
									Sleep(1);
								}
							}
							else {
								Sleep(1);
							}
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
			}
		}
		if (In.at(0) == "bring") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								bool charOK = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (charOK) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "Health");
									int healthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "MaxHealth");
									int maxhealthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									if (healthOK > 0 || to_string(maxhealthOK) == "-2147483648") {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "HumanoidRootPart");
										getglobal(rlua_State, "noobhaxmyplr");
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "HumanoidRootPart");
										getfield(rlua_State, -1, "CFrame");
										setfield(rlua_State, -5, "CFrame");
										settop(rlua_State, 0);
										Output(txtbox, "Brought " + charname + "\n");
									}
									else {
										Sleep(1);
									}
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								bool charOK = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (charOK) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "Health");
									int healthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									getfield(rlua_State, -1, "MaxHealth");
									int maxhealthOK = rlua_tonumber(rlua_State, -1);
									settop(rlua_State, 0);
									if (healthOK > 0 || to_string(maxhealthOK) == "-2147483648") {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, charname.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "HumanoidRootPart");
										getglobal(rlua_State, "noobhaxmyplr");
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "HumanoidRootPart");
										getfield(rlua_State, -1, "CFrame");
										setfield(rlua_State, -5, "CFrame");
										settop(rlua_State, 0);
										Output(txtbox, "Brought " + charname + "\n");
									}
									else {
										Sleep(1);
									}
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "damage") {
			if (In.size() == 3) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getfield(rlua_State, -1, "Health");
								float healthok = rlua_tonumber(rlua_State, -1);
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, (healthok - atof(In.at(2).c_str())));
								setfield(rlua_State, -2, "Health");
								settop(rlua_State, 0);
								Output(txtbox, "Damaged " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getfield(rlua_State, -1, "Health");
								float healthok = rlua_tonumber(rlua_State, -1);
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, (healthok - atof(In.at(2).c_str())));
								setfield(rlua_State, -2, "Health");
								settop(rlua_State, 0);
								Output(txtbox, "Damaged " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "team") {
			string newvalue;
			for (int i = 2; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "Teams");
			getfield(rlua_State, -1, "FindFirstChildOfClass");
			pushvalue(rlua_State, -2);
			pushstring(rlua_State, "Team");
			pcall(rlua_State, 2, 1, 0);
			bool okteam = rlua_toboolean(rlua_State, -1);
			settop(rlua_State, 0);
			if (okteam) {
				string realteamname = "";
				bool eeqqqqqqq5 = false;
				int teamsk = FindFirstClass(DataModel, "Team");
				vector<int> awedinwiowenf4 = GetChildren(teamsk);
				for (int i = 0; i < awedinwiowenf4.size(); i++) {
					if ((*GetName(awedinwiowenf4[i])).find(newvalue) != std::string::npos) {
						realteamname = *GetName(awedinwiowenf4[i]);
						eeqqqqqqq5 = true;
					}
				}
				if (eeqqqqqqq5) {
					try {
						if (In.at(1) == "all" || In.at(1) == "others") {
							vector<string> noobfaces = getnames(In.at(1));
							for (int i = 0; i < noobfaces.size(); i++) {

								try {
									string charname = noobfaces[i];
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "Teams");
									getfield(rlua_State, -1, realteamname.c_str());
									getfield(rlua_State, -1, "TeamColor");
									setfield(rlua_State, -5, "TeamColor");
									settop(rlua_State, 0);
									Output(txtbox, charname + "'s team is now " + realteamname + "\n");
								}
								catch (exception) {
									Sleep(1);
								}
							}
						}
						else {
							int noobokokok;
							try {
								noobokokok = GetPlayer(In.at(1));
							}
							catch (exception) {
								Sleep(1);
							}
							if (noobokokok == 0) {
								Output(txtbox, In.at(1) + " not found\n");
							}
							else {
								try {
									string charname = *GetName(noobokokok);
									int teams = FindFirstClass(DataModel, "Teams");
									int teamlolok = FindFirstChild_PartOf(teams, newvalue);
									string realteamname = *GetName(teamlolok);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "Teams");
									getfield(rlua_State, -1, realteamname.c_str());
									getfield(rlua_State, -1, "TeamColor");
									setfield(rlua_State, -5, "TeamColor");
									settop(rlua_State, 0);
									Output(txtbox, charname + "'s team is now " + realteamname + "\n");
								}
								catch (exception) {
									Sleep(1);
								}
							}
						}
					}
					catch (exception) {
						Sleep(1);
					}
				}
				else {
					Output(txtbox, newvalue + " not found\n");
				}
			}
			else {
				Output(txtbox, "No teams found...\n");
			}
		}
		if (In.at(0) == "unneon") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noob1 = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								Output(txtbox, (noob1 + " isn't neon anymore (if he/she was)\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noob1 = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushstring(rlua_State, "Plastic");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								Output(txtbox, (noob1 + " isn't neon anymore (if he/she was)\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "neon") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noob1 = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								Output(txtbox, ("Made " + noob1 + " neon\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noob1 = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushstring(rlua_State, "Neon");
								setfield(rlua_State, -2, "Material");
								settop(rlua_State, 0);
								Output(txtbox, ("Made " + noob1 + " neon\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unmesh" || In.at(0) == "unilluminati" || In.at(0) == "unoshawott" || In.at(0) == "untepig" || In.at(0) == "unsnivy" || In.at(0) == "unpatrick" || In.at(0) == "unugandanknuckles") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_mesh_ok");
								pcall(rlua_State, 2, 1, 0);
								bool hasmeshok = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (hasmeshok) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "face");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Name");
									string charnameok = rlua_tolstring(rlua_State, -1, NULL);
									settop(rlua_State, 0);
									int noobiecharok = GetCharacter(charnameok);
									int noobietorsook = FindFirstChild_PartOf(noobiecharok, "Torso");
									vector<int> noobskk = GetChildren(noobietorsook);
									for (int i = 0; i < noobskk.size(); i++) {
										try {
											if (*GetName(noobskk[i]) == "noobhax_mesh_ok") {
												getglobal(rlua_State, "game");
												getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
												getfield(rlua_State, -1, charname.c_str());
												getfield(rlua_State, -1, "Character");
												getfield(rlua_State, -1, "Torso");
												getfield(rlua_State, -1, "noobhax_mesh_ok");
												getfield(rlua_State, -1, "remove");
												pushvalue(rlua_State, -2);
												pcall(rlua_State, 1, 0, 0);
												settop(rlua_State, 0);
											}
										}
										catch (exception) {
											Sleep(1);
										}
									}
									Output(txtbox, charname + " doesn't have a mesh anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_mesh_ok");
								pcall(rlua_State, 2, 1, 0);
								bool hasmeshok = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (hasmeshok) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "face");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Name");
									string charnameok = rlua_tolstring(rlua_State, -1, NULL);
									settop(rlua_State, 0);
									int noobiecharok = GetCharacter(charnameok);
									int noobietorsook = FindFirstChild_PartOf(noobiecharok, "Torso");
									vector<int> noobskk = GetChildren(noobietorsook);
									for (int i = 0; i < noobskk.size(); i++) {
										try {
											if (*GetName(noobskk[i]) == "noobhax_mesh_ok") {
												getglobal(rlua_State, "game");
												getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
												getfield(rlua_State, -1, charname.c_str());
												getfield(rlua_State, -1, "Character");
												getfield(rlua_State, -1, "Torso");
												getfield(rlua_State, -1, "noobhax_mesh_ok");
												getfield(rlua_State, -1, "remove");
												pushvalue(rlua_State, -2);
												pcall(rlua_State, 1, 0, 0);
												settop(rlua_State, 0);
											}
										}
										catch (exception) {
											Sleep(1);
										}
									}
									Output(txtbox, charname + " doesn't have a mesh anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "mesh") {
			if (In.size() < 7) {
				Output(txtbox, "Not enough arguments (usage: mesh [p] [x] [y] [z] [meshid] [textureid])\n");
			}
			if (In.size() == 7) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								int meshidxd = atoi(In.at(5).c_str());
								string meshid = "rbxassetid://" + to_string(meshidxd);
								int textureidxd = atoi(In.at(6).c_str());
								string textureid = "rbxassetid://" + to_string(textureidxd);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_mesh_ok");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, meshid.c_str());
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, textureid.c_str());
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, atof(In.at(2).c_str()));
								pushnumber(rlua_State, atof(In.at(3).c_str()));
								pushnumber(rlua_State, atof(In.at(4).c_str()));
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a mesh to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								int meshidxd = atoi(In.at(5).c_str());
								string meshid = "rbxassetid://" + to_string(meshidxd);
								int textureidxd = atoi(In.at(6).c_str());
								string textureid = "rbxassetid://" + to_string(textureidxd);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_mesh_ok");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, meshid.c_str());
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, textureid.c_str());
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, atof(In.at(2).c_str()));
								pushnumber(rlua_State, atof(In.at(3).c_str()));
								pushnumber(rlua_State, atof(In.at(4).c_str()));
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a mesh to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "ungiant") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_head_mesh");
								pcall(rlua_State, 2, 1, 0);
								bool isgiant = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isgiant) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "HipHeight");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "noobhax_head_mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, charname + " isn't a giant anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_head_mesh");
								pcall(rlua_State, 2, 1, 0);
								bool isgiant = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isgiant) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Humanoid");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "HipHeight");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "noobhax_head_mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									getfield(rlua_State, -1, "Mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, charname + " isn't a giant anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "giant") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 25);
								setfield(rlua_State, -2, "HipHeight");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_head_mesh");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "noobhax_head_mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "noobhax_head_mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 32);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//head above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 12);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//torso above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 24);
								pushnumber(rlua_State, 12);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//left arm above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, -24);
								pushnumber(rlua_State, 12);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//right arm above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 8);
								pushnumber(rlua_State, -15);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//left leg above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, -8);
								pushnumber(rlua_State, -15);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now a giant\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 25);
								setfield(rlua_State, -2, "HipHeight");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_head_mesh");
								setfield(rlua_State, -2, "Name");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "noobhax_head_mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "noobhax_head_mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 32);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//head above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 12);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//torso above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 24);
								pushnumber(rlua_State, 12);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//left arm above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, -24);
								pushnumber(rlua_State, 12);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//right arm above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 8);
								pushnumber(rlua_State, -15);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								//left leg above
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "Brick");
								setfield(rlua_State, -2, "MeshType");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pushnumber(rlua_State, 15);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, -8);
								pushnumber(rlua_State, -15);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Offset");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now a giant\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unevilduck") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_evilduck_mesh");
								pcall(rlua_State, 2, 1, 0);
								bool isevilduck = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isevilduck) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "face");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "noobhax_evilduck_mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, charname + " isn't an evil duck anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_evilduck_mesh");
								pcall(rlua_State, 2, 1, 0);
								bool isevilduck = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isevilduck) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "face");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "noobhax_evilduck_mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, charname + " isn't an evil duck anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "evilduck") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_evilduck_mesh");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, "rbxassetid://14810664");
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, "rbxassetid://14810649");
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 5);
								pushnumber(rlua_State, 5);
								pushnumber(rlua_State, 5);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now an evil duck\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_evilduck_mesh");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, "rbxassetid://14810664");
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, "rbxassetid://14810649");
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 5);
								pushnumber(rlua_State, 5);
								pushnumber(rlua_State, 5);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now an evil duck\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "baseplate") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					getglobal(rlua_State, "workspace");
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "noobhax_bs");
					setfield(rlua_State, -2, "Name");
					pushboolean(rlua_State, 1);
					setfield(rlua_State, -2, "Anchored");
					pushboolean(rlua_State, 1);
					setfield(rlua_State, -2, "Locked");
					getglobal(rlua_State, "BrickColor");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Bright red");
					pcall(rlua_State, 1, 1, 0);
					setfield(rlua_State, -3, "BrickColor");
					settop(rlua_State, 0);
					getglobal(rlua_State, "workspace");
					getfield(rlua_State, -1, "noobhax_bs");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 0);
					pushnumber(rlua_State, -100);
					pushnumber(rlua_State, 0);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Position");
					settop(rlua_State, 0);
					getglobal(rlua_State, "workspace");
					getfield(rlua_State, -1, "noobhax_bs");
					getglobal(rlua_State, "Vector3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, 2048);
					pushnumber(rlua_State, 1);
					pushnumber(rlua_State, 2048);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Size");
					settop(rlua_State, 0);
					Output(txtbox, "Made a baseplate in the Workspace\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "printidentity") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "printidentity");
					pcall(rlua_State, 0, 0, 0);
					settop(rlua_State, 0);
					Output(txtbox, "OK!\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "repeat" && In.size() != 1) {
			int n = atoi(In.at(1).c_str());
			string newvalue;
			for (int i = 2; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			for (int i = 0; i < n; i++) {
				std::this_thread::sleep_for(100ms);
				int rlua_State123 = (int)((DWORD*)NewThread(rlua_State));
				boost::thread *t = new boost::thread();
				*t = boost::thread(
					boost::bind(&Execute2, newvalue, rlua_State123, boost::ref(my_threads), t)
				);
				my_threads.add_thread(t);
				rlua_State123 = 0;
				delete (int*)rlua_State123;
			}
		}
		if (In.at(0) == "ghost") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noob1 = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								Output(txtbox, ("Made " + noob1 + " a ghost\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noob1 = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 0.5);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								Output(txtbox, ("Made " + noob1 + " a ghost\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unshiny") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								Output(txtbox, charname + " isn't shiny anymore\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								Output(txtbox, charname + " isn't shiny anymore\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "shiny") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now shiny\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Reflectance");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now shiny\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "fling") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Velocity");
								settop(rlua_State, 0);
								std::this_thread::sleep_for(100ms);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "BodyForce");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 5555 * 4);
								pushnumber(rlua_State, 9999 * 5);
								pushnumber(rlua_State, 5555 * 4);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Force");
								Output(txtbox, "Flang " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 0);
								pushnumber(rlua_State, 0);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Velocity");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "BodyForce");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 3000);
								pushnumber(rlua_State, 3000);
								pushnumber(rlua_State, 3000);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Force");
								Output(txtbox, "Flang " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "bighead") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 2.5);
								pushnumber(rlua_State, 2.5);
								pushnumber(rlua_State, 2.5);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + "'s Head is now big\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 2.5);
								pushnumber(rlua_State, 2.5);
								pushnumber(rlua_State, 2.5);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + "'s Head is now big\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "normalhead" || In.at(0) == "unbighead") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 1.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + "'s Head is now normal\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "Mesh");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 1.25);
								pushnumber(rlua_State, 1.25);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + "'s Head is now normal\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "fogstart") {
			if (In.size() == 2) {
				try {
					float hax = atof(In.at(1).c_str());
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					pushnumber(rlua_State, hax);
					setfield(rlua_State, -2, "FogStart");
					settop(rlua_State, 0);
					Output(txtbox, "Set FogStart to " + In.at(1) + "\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "fogcolor") {
			if (In.size() == 4) {
				try {
					float R = atof(In.at(1).c_str());
					float G = atof(In.at(2).c_str());
					float B = atof(In.at(3).c_str());
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					getglobal(rlua_State, "Color3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, R);
					pushnumber(rlua_State, G);
					pushnumber(rlua_State, B);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "FogColor");
					settop(rlua_State, 0);
					Output(txtbox, "Set FogColor to " + In.at(1) + ", " + In.at(2) + ", " + In.at(3) + "\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "ambient") {
			if (In.size() == 4) {
				try {
					float R = atof(In.at(1).c_str());
					float G = atof(In.at(2).c_str());
					float B = atof(In.at(3).c_str());
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					getglobal(rlua_State, "Color3");
					getfield(rlua_State, -1, "new");
					pushnumber(rlua_State, R);
					pushnumber(rlua_State, G);
					pushnumber(rlua_State, B);
					pcall(rlua_State, 3, 1, 0);
					setfield(rlua_State, -3, "Ambient");
					settop(rlua_State, 0);
					Output(txtbox, "Set Ambient to " + In.at(1) + ", " + In.at(2) + ", " + In.at(3) + "\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "pushboolean") {
			if (In.size() == 2) {
				try {
					int boolean;
					if (In.at(1) == "true" || In.at(1) == "false") {
						if (In.at(1) == "true") {
							boolean = 1;
						}
						if (In.at(1) == "false") {
							boolean = 0;
						}
					}
					else {
						boolean = atoi(In.at(1).c_str());
					}
					pushboolean(rlua_State, boolean);
					Output(txtbox, "OK!\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unfreeze" || In.at(0) == "unfrz" || In.at(0) == "thaw") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Anchored");
								settop(rlua_State, 0);
								Output(txtbox, "Unfroze " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Anchored");
								settop(rlua_State, 0);
								Output(txtbox, "Unfroze " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "freeze" || In.at(0) == "frz") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Anchored");
								settop(rlua_State, 0);
								Output(txtbox, "Froze " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Anchored");
								settop(rlua_State, 0);
								Output(txtbox, "Froze " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unduck") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_duck_mesh");
								pcall(rlua_State, 2, 1, 0);
								bool isanoobieduckie = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isanoobieduckie) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "face");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "noobhax_duck_mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, charname + " isn't a duck anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "noobhax_duck_mesh");
								pcall(rlua_State, 2, 1, 0);
								bool isanoobieduckie = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (isanoobieduckie) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Left Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Arm");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Right Leg");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Head");
									getfield(rlua_State, -1, "face");
									pushnumber(rlua_State, 0);
									setfield(rlua_State, -2, "Transparency");
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "noobhax_duck_mesh");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, charname + " isn't a duck anymore\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "duck") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_duck_mesh");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, "rbxassetid://9419831");
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, "rbxassetid://9419827");
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 5);
								pushnumber(rlua_State, 5);
								pushnumber(rlua_State, 5);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now a duck\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "SpecialMesh");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobhax_duck_mesh");
								setfield(rlua_State, -2, "Name");
								pushstring(rlua_State, "FileMesh");
								setfield(rlua_State, -2, "MeshType");
								pushstring(rlua_State, "rbxassetid://9419831");
								setfield(rlua_State, -2, "MeshId");
								pushstring(rlua_State, "rbxassetid://9419827");
								setfield(rlua_State, -2, "TextureId");
								getglobal(rlua_State, "Vector3");
								getfield(rlua_State, -1, "new");
								pushnumber(rlua_State, 5);
								pushnumber(rlua_State, 5);
								pushnumber(rlua_State, 5);
								pcall(rlua_State, 3, 1, 0);
								setfield(rlua_State, -3, "Scale");
								settop(rlua_State, 0);
								Output(txtbox, charname + " is now a duck\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unname") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Name");
								setfield(rlua_State, -5, "Name");
								settop(rlua_State, 0);
								Output(txtbox, "Restored " + charname + "'s name\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Name");
								setfield(rlua_State, -5, "Name");
								settop(rlua_State, 0);
								Output(txtbox, "Restored " + charname + "'s name\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "name") {
			string newvalue;
			for (int i = 2; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			try {
				if (In.at(1) == "all" || In.at(1) == "others") {
					vector<string> noobfaces = getnames(In.at(1));
					for (int i = 0; i < noobfaces.size(); i++) {

						try {
							string charname = noobfaces[i];
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, charname.c_str());
							getfield(rlua_State, -1, "Character");
							pushstring(rlua_State, newvalue.c_str());
							setfield(rlua_State, -2, "Name");
							settop(rlua_State, 0);
							Output(txtbox, charname + "'s Name is now " + newvalue + "\n");
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
				else {
					int noobokokok;
					try {
						noobokokok = GetPlayer(In.at(1));
					}
					catch (exception) {
						Sleep(1);
					}
					if (noobokokok == 0) {
						Output(txtbox, In.at(1) + " not found\n");
					}
					else {
						try {
							string charname = *GetName(noobokokok);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, charname.c_str());
							getfield(rlua_State, -1, "Character");
							pushstring(rlua_State, newvalue.c_str());
							setfield(rlua_State, -2, "Name");
							settop(rlua_State, 0);
							Output(txtbox, charname + "'s Name is now " + newvalue + "\n");
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "clone") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Archivable");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Clone");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 1, 0);
								getglobal(rlua_State, "workspace");
								setfield(rlua_State, -2, "Parent");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Archivable");
								settop(rlua_State, 0);
								Output(txtbox, "Cloned " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								pushboolean(rlua_State, 1);
								setfield(rlua_State, -2, "Archivable");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Clone");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 1, 0);
								getglobal(rlua_State, "workspace");
								setfield(rlua_State, -2, "Parent");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								pushboolean(rlua_State, 0);
								setfield(rlua_State, -2, "Archivable");
								settop(rlua_State, 0);
								Output(txtbox, "Cloned " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "maxhealth") {
			if (In.size() == 3) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								double newvalue = atof(In.at(2).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, newvalue);
								setfield(rlua_State, -2, "MaxHealth");
								settop(rlua_State, 0);
								Output(txtbox, ("Set " + charname + "'s MaxHealth to " + In.at(2) + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								double newvalue = atof(In.at(2).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, newvalue);
								setfield(rlua_State, -2, "MaxHealth");
								settop(rlua_State, 0);
								Output(txtbox, "Set " + charname + "'s MaxHealth to " + In.at(2) + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "chat") {
			string newvalue;
			for (int i = 2; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			try {
				if (In.at(1) == "all" || In.at(1) == "others") {
					vector<string> noobfaces = getnames(In.at(1));
					for (int i = 0; i < noobfaces.size(); i++) {

						try {
							string charname = noobfaces[i];
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, charname.c_str());
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "Head");
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "Chat");
							getfield(rlua_State, -1, "Chat");
							pushvalue(rlua_State, -2);
							pushvalue(rlua_State, -5);
							pushstring(rlua_State, newvalue.c_str());
							pushstring(rlua_State, "Red");
							pcall(rlua_State, 4, 0, 0);
							settop(rlua_State, 0);
							Output(txtbox, "Made " + charname + " chat " + newvalue + "\n");
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
				else {
					int noobokokok;
					try {
						noobokokok = GetPlayer(In.at(1));
					}
					catch (exception) {
						Sleep(1);
					}
					if (noobokokok == 0) {
						Output(txtbox, In.at(1) + " not found\n");
					}
					else {
						try {
							string charname = *GetName(noobokokok);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, charname.c_str());
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "Head");
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "Chat");
							getfield(rlua_State, -1, "Chat");
							pushvalue(rlua_State, -2);
							pushvalue(rlua_State, -5);
							pushstring(rlua_State, newvalue.c_str());
							pushstring(rlua_State, "Red");
							pcall(rlua_State, 4, 0, 0);
							settop(rlua_State, 0);
							Output(txtbox, "Made " + charname + " chat " + newvalue + "\n");
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "cls") {
			if (In.size() == 1) {
				SetWindowText(txtbox, "");
			}
		}
		if (In.at(0) == "credits") {
			if (In.size() == 1)
			{
				Output(txtbox, "Noobhax credits:\n\n");
				Output(txtbox, "you - for buying :)))))\n");
				Output(txtbox, "KingSea - donating, helping\n");
				Output(txtbox, "Nowadays (KingSea's friend) - making autobuy and autowhitelister\n");
				Output(txtbox, "oshawott - hwid function, helping and making new whitelister!!!\n");
				Output(txtbox, "Quad Core - helping a lot in the past!\n");
				Output(txtbox, "Louka, Tepig - their functions (and Tepig's scan) and credits to Tepig for helping in general in the past\n");
				Output(txtbox, "Eternal - his retcheck\n");
				Output(txtbox, "Aero, Africanus - helping\n");
			}
		}
		if (In.at(0) == "unlight") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noobface = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobface.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Torso");
								pcall(rlua_State, 2, 1, 0);
								bool torso = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (torso) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noobface.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "FindFirstChild");
									pushvalue(rlua_State, -2);
									pushstring(rlua_State, "noobxploit_light");
									pcall(rlua_State, 2, 1, 0);
									bool noobietest = rlua_toboolean(rlua_State, -1);
									settop(rlua_State, 0);
									if (noobietest) {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noobface.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Torso");
										getfield(rlua_State, -1, "noobxploit_light");
										getfield(rlua_State, -1, "remove");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
										Output(txtbox, "Removed " + noobface + "'s PointLight\n");
									}
									else {
										Sleep(1);
									}
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noobface = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobface.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Torso");
								pcall(rlua_State, 2, 1, 0);
								bool torso = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (torso) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noobface.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "FindFirstChild");
									pushvalue(rlua_State, -2);
									pushstring(rlua_State, "noobxploit_light");
									pcall(rlua_State, 2, 1, 0);
									bool noobietest = rlua_toboolean(rlua_State, -1);
									settop(rlua_State, 0);
									if (noobietest) {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noobface.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Torso");
										getfield(rlua_State, -1, "noobxploit_light");
										getfield(rlua_State, -1, "remove");
										pushvalue(rlua_State, -2);
										pcall(rlua_State, 1, 0, 0);
										settop(rlua_State, 0);
										Output(txtbox, "Removed " + noobface + "'s PointLight\n");
									}
									else {
										Sleep(1);
									}
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "light") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noobface = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobface.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "PointLight");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobxploit_light");
								setfield(rlua_State, -2, "Name");
								pushnumber(rlua_State, 40);
								setfield(rlua_State, -2, "Range");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a PointLight to " + noobface + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noobface = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobface.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "PointLight");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, "noobxploit_light");
								setfield(rlua_State, -2, "Name");
								pushnumber(rlua_State, 40);
								setfield(rlua_State, -2, "Range");
								settop(rlua_State, 0);
								Output(txtbox, "Gave a PointLight to " + noobface + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "stun") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noobface = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobface.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
								setfield(rlua_State, -3, "Parent");
								settop(rlua_State, 0);
								Output(txtbox, "Removed " + noobface + "'s Humanoid\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noobface = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobface.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
								setfield(rlua_State, -3, "Parent");
								settop(rlua_State, 0);
								Output(txtbox, "Removed " + noobface + "'s Humanoid\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unstun") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noobface = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Humanoid");
								pcall(rlua_State, 2, 1, 0);
								bool truefalsestunnoob = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobface.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Humanoid");
								pcall(rlua_State, 2, 1, 0);
								bool thishastobefalse = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (truefalsestunnoob && !thishastobefalse) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, "Humanoid");
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noobface.c_str());
									getfield(rlua_State, -1, "Character");
									setfield(rlua_State, -5, "Parent");
									settop(rlua_State, 0);
									Output(txtbox, "Restored " + noobface + "'s Humanoid\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noobface = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Humanoid");
								pcall(rlua_State, 2, 1, 0);
								bool truefalsestunnoob = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobface.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Humanoid");
								pcall(rlua_State, 2, 1, 0);
								bool thishastobefalse = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (truefalsestunnoob && !thishastobefalse) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, "Humanoid");
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noobface.c_str());
									getfield(rlua_State, -1, "Character");
									setfield(rlua_State, -5, "Parent");
									settop(rlua_State, 0);
									Output(txtbox, "Restored " + noobface + "'s Humanoid\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "rsky") {
			if (In.size() == 1) {
				try {
					if (usednoobiesky > 0)
					{
						try {
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, "noobhax_sky");
							getfield(rlua_State, -1, "remove");
							pushvalue(rlua_State, -2);
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
							Output(txtbox, "Removed skybox\n");
							usednoobiesky = usednoobiesky - 1;
						}
						catch (exception) {
							Sleep(1);
						}
					}
					else {
						Sleep(1);
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "sky") {
			if (In.size() == 2) {
				try {
					string skybox = "rbxassetid://" + In.at(1);
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Sky");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, "noobhax_sky");
					setfield(rlua_State, -2, "Name");
					pushstring(rlua_State, skybox.c_str());
					setfield(rlua_State, -2, "SkyboxBk");
					pushstring(rlua_State, skybox.c_str());
					setfield(rlua_State, -2, "SkyboxDn");
					pushstring(rlua_State, skybox.c_str());
					setfield(rlua_State, -2, "SkyboxFt");
					pushstring(rlua_State, skybox.c_str());
					setfield(rlua_State, -2, "SkyboxLf");
					pushstring(rlua_State, skybox.c_str());
					setfield(rlua_State, -2, "SkyboxRt");
					pushstring(rlua_State, skybox.c_str());
					setfield(rlua_State, -2, "SkyboxUp");
					settop(rlua_State, 0);
					Output(txtbox, "Inserted the skybox with the ID " + In.at(1) + "\n");
					usednoobiesky++;
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "setfield") {
			try {
				string newvalue;
				for (int i = 2; i < In.size(); i++) {
					if (i < (In.size() - 1)) {
						newvalue.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 1)) {
						newvalue.append(In.at(i));
					}
				}
				setfield(rlua_State, atof(In.at(1).c_str()), newvalue.c_str());
				Output(txtbox, "OK!\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "setglobal") {
			try {
				string newvalue;
				for (int i = 1; i < In.size(); i++) {
					if (i < (In.size() - 1)) {
						newvalue.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 1)) {
						newvalue.append(In.at(i));
					}
				}
				rlua_setfield(rlua_State, -10002, newvalue.c_str());
				Output(txtbox, "OK!\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "punish") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noob1 = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
								setfield(rlua_State, -3, "Parent");
								settop(rlua_State, 0);
								Output(txtbox, "Punished " + noob1 + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noob1 = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
								setfield(rlua_State, -3, "Parent");
								settop(rlua_State, 0);
								Output(txtbox, "Punished " + noob1 + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unpunish") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noob1 = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, noob1.c_str());
								pcall(rlua_State, 2, 1, 0);
								bool truefalsepunishnoob = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (truefalsepunishnoob) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noob1.c_str());
									getglobal(rlua_State, "workspace");
									setfield(rlua_State, -2, "Parent");
									settop(rlua_State, 0);
									Output(txtbox, "Unpunished " + noob1 + "\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noob1 = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, noob1.c_str());
								pcall(rlua_State, 2, 1, 0);
								bool truefalsepunishnoob = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (truefalsepunishnoob) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noob1.c_str());
									getglobal(rlua_State, "workspace");
									setfield(rlua_State, -2, "Parent");
									settop(rlua_State, 0);
									Output(txtbox, "Unpunished " + noob1 + "\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}

						}
					}
				}
				catch (exception)
				{
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "music") {
			if (In.size() == 2) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Sound");
					getglobal(rlua_State, "workspace");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noobhax_music");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					pushboolean(rlua_State, 1);
					setfield(rlua_State, -2, "Looped");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					pushstring(rlua_State, ("rbxassetid://" + In.at(1)).c_str());
					setfield(rlua_State, -2, "SoundId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					pushstring(rlua_State, "noobhax_music");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					pushnumber(rlua_State, 1);
					setfield(rlua_State, -2, "Volume");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					getfield(rlua_State, -1, "Play");
					pushvalue(rlua_State, -2);
					pcall(rlua_State, 1, 0, 0);
					settop(rlua_State, 0);
					Output(txtbox, "Inserted the music with the ID " + In.at(1) + "\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
			else if (In.size() == 3) {
				try {
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Sound");
					getglobal(rlua_State, "workspace");
					pcall(rlua_State, 2, 1, 0);
					setglobal(rlua_State, "noobhax_music");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					pushboolean(rlua_State, 1);
					setfield(rlua_State, -2, "Looped");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					pushstring(rlua_State, ("rbxassetid://" + In.at(1)).c_str());
					setfield(rlua_State, -2, "SoundId");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					pushstring(rlua_State, "noobhax_music");
					setfield(rlua_State, -2, "Name");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					pushnumber(rlua_State, 1);
					setfield(rlua_State, -2, "Volume");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					pushnumber(rlua_State, atof(In.at(2).c_str()));
					setfield(rlua_State, -2, "Pitch");
					settop(rlua_State, 0);
					getglobal(rlua_State, "noobhax_music");
					getfield(rlua_State, -1, "Play");
					pushvalue(rlua_State, -2);
					pcall(rlua_State, 1, 0, 0);
					settop(rlua_State, 0);
					Output(txtbox, "Inserted the music with the ID " + In.at(1) + " and the pitch " + In.at(2) + "\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "explode") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string fuckingnoob = noobfaces[i];
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Explosion");
								getglobal(rlua_State, "workspace");
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, fuckingnoob.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "HumanoidRootPart");
								getfield(rlua_State, -1, "Position");
								setfield(rlua_State, -7, "Position");
								settop(rlua_State, 0);
								Output(txtbox, "Blew up " + fuckingnoob + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try
							{
								string fuckingnoob = *GetName(noobokokok);
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Explosion");
								getglobal(rlua_State, "workspace");
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, fuckingnoob.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "HumanoidRootPart");
								getfield(rlua_State, -1, "Position");
								setfield(rlua_State, -7, "Position");
								settop(rlua_State, 0);
								Output(txtbox, "Blew up " + fuckingnoob + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "tp") {
			if (In.size() == 3) {
				if (In.at(2) == "all" || In.at(2) == "others") {
					Output(txtbox, "wtf are you trying to do\n");
				}
				else {
					try {
						if (In.at(1) == "all" || In.at(1) == "others") {
							vector<string> noobfaces = getnames(In.at(1));
							int noobokokok2;
							try {
								noobokokok2 = GetPlayer(In.at(2));
							}
							catch (exception) {
								Sleep(1);
							}
							if (noobokokok2 == 0)
							{
								Output(txtbox, In.at(2) + " not found\n");
							}
							else {
								for (int i = 0; i < noobfaces.size(); i++) {

									try {
										string noob1 = noobfaces[i];
										string noob2 = *GetName(noobokokok2);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noob1.c_str());
										getfield(rlua_State, -1, "Character");
										bool charOK = rlua_toboolean(rlua_State, -1);
										settop(rlua_State, 0);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noob2.c_str());
										getfield(rlua_State, -1, "Character");
										bool char2OK = rlua_toboolean(rlua_State, -1);
										settop(rlua_State, 0);
										if (charOK && char2OK) {
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, noob1.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Humanoid");
											getfield(rlua_State, -1, "Health");
											int health1 = rlua_tonumber(rlua_State, -1);
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, noob1.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Humanoid");
											getfield(rlua_State, -1, "MaxHealth");
											int maxhealth1OK = rlua_tonumber(rlua_State, -1);
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, noob2.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Humanoid");
											getfield(rlua_State, -1, "Health");
											int health2 = rlua_tonumber(rlua_State, -1);
											settop(rlua_State, 0);
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, noob2.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "Humanoid");
											getfield(rlua_State, -1, "MaxHealth");
											int maxhealth2OK = rlua_tonumber(rlua_State, -1);
											settop(rlua_State, 0);
											if ((health1 > 0 || to_string(maxhealth1OK) == "-2147483648") && (health2 > 0 || to_string(maxhealth2OK) == "-2147483648")) {
												getglobal(rlua_State, "game");
												getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
												getfield(rlua_State, -1, noob1.c_str());
												getfield(rlua_State, -1, "Character");
												getfield(rlua_State, -1, "HumanoidRootPart");
												getglobal(rlua_State, "game");
												getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
												getfield(rlua_State, -1, noob2.c_str());
												getfield(rlua_State, -1, "Character");
												getfield(rlua_State, -1, "HumanoidRootPart");
												getfield(rlua_State, -1, "CFrame");
												setfield(rlua_State, -7, "CFrame");
												settop(rlua_State, 0);
												Output(txtbox, ("Teleported " + noob1 + " to " + noob2 + "\n"));
											}
											else {
												Sleep(1);
											}
										}
										else {
											Sleep(1);
										}
									}
									catch (exception) {
										Sleep(1);
									}
								}
							}
						}
						else {
							int noobokokok;
							int noobokokok2;
							try {
								noobokokok = GetPlayer(In.at(1));
								noobokokok2 = GetPlayer(In.at(2));
							}
							catch (exception) {
								Sleep(1);
							}
							if (noobokokok == 0) {
								Output(txtbox, In.at(1) + " not found\n");
							}
							if (noobokokok2 == 0)
							{
								Output(txtbox, In.at(2) + " not found\n");
							}
							if (noobokokok != 0 && noobokokok2 != 0) {
								try {
									string noob1 = *GetName(noobokokok);
									string noob2 = *GetName(noobokokok2);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noob1.c_str());
									getfield(rlua_State, -1, "Character");
									bool charOK = rlua_toboolean(rlua_State, -1);
									settop(rlua_State, 0);
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, noob2.c_str());
									getfield(rlua_State, -1, "Character");
									bool char2OK = rlua_toboolean(rlua_State, -1);
									settop(rlua_State, 0);
									if (charOK && char2OK) {
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noob1.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Humanoid");
										getfield(rlua_State, -1, "Health");
										int health1 = rlua_tonumber(rlua_State, -1);
										settop(rlua_State, 0);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noob1.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Humanoid");
										getfield(rlua_State, -1, "MaxHealth");
										int maxhealth1OK = rlua_tonumber(rlua_State, -1);
										settop(rlua_State, 0);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noob2.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Humanoid");
										getfield(rlua_State, -1, "Health");
										int health2 = rlua_tonumber(rlua_State, -1);
										settop(rlua_State, 0);
										getglobal(rlua_State, "game");
										getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
										getfield(rlua_State, -1, noob2.c_str());
										getfield(rlua_State, -1, "Character");
										getfield(rlua_State, -1, "Humanoid");
										getfield(rlua_State, -1, "MaxHealth");
										int maxhealth2OK = rlua_tonumber(rlua_State, -1);
										settop(rlua_State, 0);
										if ((health1 > 0 || to_string(maxhealth1OK) == "-2147483648") && (health2 > 0 || to_string(maxhealth2OK) == "-2147483648")) {
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, noob1.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "HumanoidRootPart");
											getglobal(rlua_State, "game");
											getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
											getfield(rlua_State, -1, noob2.c_str());
											getfield(rlua_State, -1, "Character");
											getfield(rlua_State, -1, "HumanoidRootPart");
											getfield(rlua_State, -1, "CFrame");
											setfield(rlua_State, -7, "CFrame");
											settop(rlua_State, 0);
											Output(txtbox, ("Teleported " + noob1 + " to " + noob2 + "\n"));
										}
										else {
											Sleep(1);
										}
									}
									else {
										Sleep(1);
									}
								}
								catch (exception) {
									Sleep(1);
								}
							}
						}
					}
					catch (exception) {
						Sleep(1);
					}
				}
			}
		}
		if (In.at(0) == "print") {
			string string;
			for (int i = 1; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					string.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					string.append(In.at(i));
				}
			}
			try {
				getglobal(rlua_State, "print");
				pushstring(rlua_State, string.c_str());
				pcall(rlua_State, 1, 0, 0);
				settop(rlua_State, 0);
				Output(txtbox, "Printed " + string + "\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "gravity") {
			if (In.size() == 2) {
				try {
					int newgravity = atof(In.at(1).c_str());
					getglobal(rlua_State, "workspace");
					pushnumber(rlua_State, newgravity);
					setfield(rlua_State, -2, "Gravity");
					settop(rlua_State, 0);
					Output(txtbox, "Set the gravity to " + In.at(1) + "\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "gettop") {
			if (In.size() == 1) {
				try {
					Output(txtbox, ("Elements on the stack: " + to_string(rlua_gettop(rlua_State)) + "\n"));
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "emptystack") {
			if (In.size() == 1) {
				try {
					settop(rlua_State, 0);
					Output(txtbox, "OK!\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "pcall") {
			if (In.size() == 3 || (In.size() == 4 && In.at(3) == "0")) {
				try {
					pcall(rlua_State, atof(In.at(1).c_str()), atof(In.at(2).c_str()), 0);
					Output(txtbox, "OK!\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "settop") {
			if (In.size() == 2) {
				try {
					int n = atoi(In.at(1).c_str());
					settop(rlua_State, n);
					Output(txtbox, "OK!\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "pop") {
			if (In.size() == 2) {
				try {
					int n = atoi(In.at(1).c_str());
					settop(rlua_State, -(n)-1);
					Output(txtbox, "OK!\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "pushnumber") {
			if (In.size() == 2) {
				try {
					pushnumber(rlua_State, atof(In.at(1).c_str()));
					Output(txtbox, "OK!\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "pushstring") {
			try {
				string newvalue;
				for (int i = 1; i < In.size(); i++) {
					if (i < (In.size() - 1)) {
						newvalue.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 1)) {
						newvalue.append(In.at(i));
					}
				}
				pushstring(rlua_State, newvalue.c_str());
				Output(txtbox, "OK!\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "pushvalue") {
			if (In.size() == 2) {
				try {
					pushvalue(rlua_State, atof(In.at(1).c_str()));
					Output(txtbox, "OK!\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "getfield") {
			try {
				string newvalue;
				for (int i = 2; i < In.size(); i++) {
					if (i < (In.size() - 1)) {
						newvalue.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 1)) {
						newvalue.append(In.at(i));
					}
				}
				getfield(rlua_State, atof(In.at(1).c_str()), newvalue.c_str());
				Output(txtbox, "OK!\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "getglobal") {
			try {
				string newvalue;
				for (int i = 1; i < In.size(); i++) {
					if (i < (In.size() - 1)) {
						newvalue.append(In.at(i) + ' ');
					}
					if (i == (In.size() - 1)) {
						newvalue.append(In.at(i));
					}
				}
				getglobal(rlua_State, newvalue.c_str());
				Output(txtbox, "OK!\n");
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "invisible" || In.at(0) == "invis") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noob1 = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								Output(txtbox, ("Made " + noob1 + " invisible\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noob1 = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								Output(txtbox, ("Made " + noob1 + " invisible\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "visible" || In.at(0) == "unghost" || In.at(0) == "vis") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noob1 = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								Output(txtbox, ("Made " + noob1 + " visible\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string noob1 = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Arm");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Left Leg");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Arm");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Right Leg");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noob1.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Head");
								getfield(rlua_State, -1, "face");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Transparency");
								settop(rlua_State, 0);
								Output(txtbox, ("Made " + noob1 + " visible\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "statchange1" || In.at(0) == "changestat1" || In.at(0) == "changestat") {
			if (In.size() == 4) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string playername = noobfaces[i];
								int leaderstats = FindFirstChild_PartOf(GetPlayer(playername), "leaderstats");
								int stat = FindFirstChild_PartOf(leaderstats, In.at(2));
								string statname = *GetName(stat);
								int newvalue = atof(In.at(3).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, playername.c_str());
								getfield(rlua_State, -1, "leaderstats");
								getfield(rlua_State, -1, statname.c_str());
								pushnumber(rlua_State, newvalue);
								setfield(rlua_State, -2, "Value");
								settop(rlua_State, 0);
								Output(txtbox, (playername + "'s " + statname + " value is now " + In.at(3) + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								int Player = GetPlayer(In.at(1));
								string playername = *GetName(Player);
								int leaderstats = FindFirstChild_PartOf(Player, "leaderstats");
								int stat = FindFirstChild_PartOf(leaderstats, In.at(2));
								string statname = *GetName(stat);
								int newvalue = atof(In.at(3).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, playername.c_str());
								getfield(rlua_State, -1, "leaderstats");
								getfield(rlua_State, -1, statname.c_str());
								pushnumber(rlua_State, newvalue);
								setfield(rlua_State, -2, "Value");
								settop(rlua_State, 0);
								Output(txtbox, (playername + "'s " + statname + " value is now " + In.at(3) + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "statchange2" || In.at(0) == "changestat2") {
			string newvalue;
			for (int i = 3; i < In.size(); i++) {
				if (i < (In.size() - 1)) {
					newvalue.append(In.at(i) + ' ');
				}
				if (i == (In.size() - 1)) {
					newvalue.append(In.at(i));
				}
			}
			try {
				if (In.at(1) == "all" || In.at(1) == "others") {
					vector<string> noobfaces = getnames(In.at(1));
					for (int i = 0; i < noobfaces.size(); i++) {

						try {
							string playername = noobfaces[i];
							int leaderstats = FindFirstChild_PartOf(GetPlayer(playername), "leaderstats");
							int stat = FindFirstChild_PartOf(leaderstats, In.at(2));
							string statname = *GetName(stat);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, playername.c_str());
							getfield(rlua_State, -1, "leaderstats");
							getfield(rlua_State, -1, statname.c_str());
							pushstring(rlua_State, newvalue.c_str());
							setfield(rlua_State, -2, "Value");
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
							Output(txtbox, (playername + "'s " + statname + " value is now " + newvalue + "\n"));
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
				else {
					int noobokokok;
					try {
						noobokokok = GetPlayer(In.at(1));
					}
					catch (exception) {
						Sleep(1);
					}
					if (noobokokok == 0) {
						Output(txtbox, In.at(1) + " not found\n");
					}
					else {
						int Player = GetPlayer(In.at(1));
						string playername = *GetName(Player);
						int leaderstats = FindFirstChild_PartOf(Player, "leaderstats");
						int stat = FindFirstChild_PartOf(leaderstats, In.at(2));
						string statname = *GetName(stat);
						try {
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, playername.c_str());
							getfield(rlua_State, -1, "leaderstats");
							getfield(rlua_State, -1, statname.c_str());
							pushstring(rlua_State, newvalue.c_str());
							setfield(rlua_State, -2, "Value");
							pcall(rlua_State, 1, 0, 0);
							settop(rlua_State, 0);
							Output(txtbox, (playername + "'s " + statname + " value is now " + newvalue + "\n"));
						}
						catch (exception) {
							Sleep(1);
						}
					}
				}
			}
			catch (exception) {
				Sleep(1);
			}
		}
		if (In.at(0) == "ff") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ForceField");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Gave a ForceField to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ForceField");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Gave a ForceField to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "fogend") {
			if (In.size() == 2) {
				try {
					float hax = atof(In.at(1).c_str());
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					pushnumber(rlua_State, hax);
					setfield(rlua_State, -2, "FogEnd");
					settop(rlua_State, 0);
					Output(txtbox, "Set FogEnd to " + In.at(1) + "\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unff" || In.at(0) == "uninvisff" || In.at(0) == "uninvisibleff") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "ForceField");
								pcall(rlua_State, 2, 1, 0);
								bool ff = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (ff) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "ForceField");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s ForceField\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							string charname = *GetName(noobokokok);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, charname.c_str());
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "FindFirstChild");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "ForceField");
							pcall(rlua_State, 2, 1, 0);
							bool ff = rlua_toboolean(rlua_State, -1);
							settop(rlua_State, 0);
							if (ff) {
								try {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "ForceField");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s ForceField\n");
								}
								catch (exception) {
									Sleep(1);
								}
							}
							else {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "fire") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Fire");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Gave Fire to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Fire");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Gave Fire to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			else if (In.size() == 5) {
				float R = atof(In.at(2).c_str());
				float G = atof(In.at(3).c_str());
				float B = atof(In.at(4).c_str());
				getglobal(rlua_State, "Color3");
				getfield(rlua_State, -1, "new");
				pushnumber(rlua_State, R);
				pushnumber(rlua_State, G);
				pushnumber(rlua_State, B);
				pcall(rlua_State, 3, 1, 0);
				setglobal(rlua_State, "noobhax_fire_color_ok");
				settop(rlua_State, 0);
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Fire");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "noobhax_fire_color_ok");
								setfield(rlua_State, -2, "Color");
								settop(rlua_State, 0);
								Output(txtbox, "Gave Fire to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Fire");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "noobhax_fire_color_ok");
								setfield(rlua_State, -2, "Color");
								settop(rlua_State, 0);
								Output(txtbox, "Gave Fire to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			else {
				Output(txtbox, "Usage: fire [p]  [optional red value] [optional green value] [optional blue value]\n");
			}
		}
		if (In.at(0) == "unfire") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Fire");
								pcall(rlua_State, 2, 1, 0);
								bool fire = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (fire) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "Fire");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s Fire\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							string charname = *GetName(noobokokok);
							getglobal(rlua_State, "game");
							getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
							getfield(rlua_State, -1, charname.c_str());
							getfield(rlua_State, -1, "Character");
							getfield(rlua_State, -1, "Torso");
							getfield(rlua_State, -1, "FindFirstChild");
							pushvalue(rlua_State, -2);
							pushstring(rlua_State, "Fire");
							pcall(rlua_State, 2, 1, 0);
							bool fire = rlua_toboolean(rlua_State, -1);
							settop(rlua_State, 0);
							if (fire) {
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "Fire");
								getfield(rlua_State, -1, "remove");
								pushvalue(rlua_State, -2);
								pcall(rlua_State, 1, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Removed " + charname + "'s Fire\n");
							}
							else {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "sparkle") {
			if (In.size() == 2 || In.size() == 5) {
				Output(txtbox, "sparkles not sparkle\n");
			}
		}
		if (In.at(0) == "sparkles" || In.at(0) == "sp") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Sparkles");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Gave Sparkles to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Sparkles");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Gave Sparkles to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			else if (In.size() == 5) {
				float R = atof(In.at(2).c_str());
				float G = atof(In.at(3).c_str());
				float B = atof(In.at(4).c_str());
				getglobal(rlua_State, "Color3");
				getfield(rlua_State, -1, "new");
				pushnumber(rlua_State, R);
				pushnumber(rlua_State, G);
				pushnumber(rlua_State, B);
				pcall(rlua_State, 3, 1, 0);
				setglobal(rlua_State, "noobhax_sp_color_ok");
				settop(rlua_State, 0);
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Sparkles");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "noobhax_sp_color_ok");
								setfield(rlua_State, -2, "SparkleColor");
								settop(rlua_State, 0);
								Output(txtbox, "Gave Sparkles to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Sparkles");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "noobhax_sp_color_ok");
								setfield(rlua_State, -2, "SparkleColor");
								settop(rlua_State, 0);
								Output(txtbox, "Gave Sparkles to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			else {
				Output(txtbox, "Usage: sparkles/sp [p]  [optional red value] [optional green value] [optional blue value]\n");
			}
		}
		if (In.at(0) == "unsparkles" || In.at(0) == "unsp") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Sparkles");
								pcall(rlua_State, 2, 1, 0);
								bool hassparkles = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (hassparkles) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "Sparkles");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s Sparkles\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Sparkles");
								pcall(rlua_State, 2, 1, 0);
								bool hassparkles = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (hassparkles) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "Sparkles");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s Sparkles\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "smoke") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Smoke");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Gave Smoke to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Smoke");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 0, 0);
								settop(rlua_State, 0);
								Output(txtbox, "Gave Smoke to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			else if (In.size() == 5) {
				float R = atof(In.at(2).c_str());
				float G = atof(In.at(3).c_str());
				float B = atof(In.at(4).c_str());
				getglobal(rlua_State, "Color3");
				getfield(rlua_State, -1, "new");
				pushnumber(rlua_State, R);
				pushnumber(rlua_State, G);
				pushnumber(rlua_State, B);
				pcall(rlua_State, 3, 1, 0);
				setglobal(rlua_State, "noobhax_smoke_color_ok");
				settop(rlua_State, 0);
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Smoke");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "noobhax_smoke_color_ok");
								setfield(rlua_State, -2, "Color");
								settop(rlua_State, 0);
								Output(txtbox, "Gave Smoke to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "Smoke");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								getglobal(rlua_State, "noobhax_smoke_color_ok");
								setfield(rlua_State, -2, "Color");
								settop(rlua_State, 0);
								Output(txtbox, "Gave Smoke to " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			else {
				Output(txtbox, "Usage: smoke [p]  [optional red value] [optional green value] [optional blue value]\n");
			}
		}
		if (In.at(0) == "unsmoke") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Smoke");
								pcall(rlua_State, 2, 1, 0);
								bool hassmoke = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (hassmoke) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "Smoke");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s Smoke\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "Smoke");
								pcall(rlua_State, 2, 1, 0);
								bool hassmoke = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (hassmoke) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "Smoke");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, "Removed " + charname + "'s Smoke\n");
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "particle") {
			Output(txtbox, "particles not particle\n");
		}
		if (In.at(0) == "particles") {
			if (In.size() == 3) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								string texture = "rbxassetid://" + In.at(2);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ParticleEmitter");
								pushvalue(rlua_State, -4);
								std::this_thread::sleep_for(100ms);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, texture.c_str());
								setfield(rlua_State, -2, "Texture");
								std::this_thread::sleep_for(100ms);
								pushnumber(rlua_State, 120);
								setfield(rlua_State, -2, "VelocitySpread");
								settop(rlua_State, 0);
								Output(txtbox, ("Gave particles with the ID " + In.at(2) + " to " + charname + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								string texture = "rbxassetid://" + In.at(2);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ParticleEmitter");
								pushvalue(rlua_State, -4);
								std::this_thread::sleep_for(100ms);
								pcall(rlua_State, 2, 1, 0);
								pushstring(rlua_State, texture.c_str());
								setfield(rlua_State, -2, "Texture");
								std::this_thread::sleep_for(100ms);
								pushnumber(rlua_State, 120);
								setfield(rlua_State, -2, "VelocitySpread");
								settop(rlua_State, 0);
								Output(txtbox, ("Gave particles with the ID " + In.at(2) + " to " + charname + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
			else if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ParticleEmitter");
								pushvalue(rlua_State, -4);
								std::this_thread::sleep_for(100ms);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 120);
								setfield(rlua_State, -2, "VelocitySpread");
								settop(rlua_State, 0);
								Output(txtbox, ("Gave particles to " + charname + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "ParticleEmitter");
								pushvalue(rlua_State, -4);
								std::this_thread::sleep_for(100ms);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 120);
								setfield(rlua_State, -2, "VelocitySpread");
								settop(rlua_State, 0);
								Output(txtbox, ("Gave particles to " + charname + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "unparticles") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "ParticleEmitter");
								pcall(rlua_State, 2, 1, 0);
								bool hasparticles = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (hasparticles) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ParticleEmitter");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, ("Removed " + charname + "'s particles\n"));
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Torso");
								getfield(rlua_State, -1, "FindFirstChild");
								pushvalue(rlua_State, -2);
								pushstring(rlua_State, "ParticleEmitter");
								pcall(rlua_State, 2, 1, 0);
								bool hasparticles = rlua_toboolean(rlua_State, -1);
								settop(rlua_State, 0);
								if (hasparticles) {
									getglobal(rlua_State, "game");
									getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
									getfield(rlua_State, -1, charname.c_str());
									getfield(rlua_State, -1, "Character");
									getfield(rlua_State, -1, "Torso");
									getfield(rlua_State, -1, "ParticleEmitter");
									getfield(rlua_State, -1, "remove");
									pushvalue(rlua_State, -2);
									pcall(rlua_State, 1, 0, 0);
									settop(rlua_State, 0);
									Output(txtbox, ("Removed " + charname + "'s particles\n"));
								}
								else {
									Sleep(1);
								}
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "ws") {
			if (In.size() == 3) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								float hax = atof(In.at(2).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, hax);
								setfield(rlua_State, -2, "WalkSpeed");
								settop(rlua_State, 0);
								Output(txtbox, (charname + "'s WalkSpeed is now " + In.at(2) + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								float hax = atof(In.at(2).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, hax);
								setfield(rlua_State, -2, "WalkSpeed");
								settop(rlua_State, 0);
								Output(txtbox, (charname + "'s WalkSpeed is now " + In.at(2) + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "kill") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Health");
								settop(rlua_State, 0);
								Output(txtbox, "Killed " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 0);
								setfield(rlua_State, -2, "Health");
								settop(rlua_State, 0);
								Output(txtbox, "Killed " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "god") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, HUGE_VAL);
								setfield(rlua_State, -2, "MaxHealth");
								settop(rlua_State, 0);
								Output(txtbox, "Godded " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, HUGE_VAL);
								setfield(rlua_State, -2, "MaxHealth");
								settop(rlua_State, 0);
								Output(txtbox, "Godded " + charname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "ungod") {
			if (In.size() == 2) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 100);
								setfield(rlua_State, -2, "MaxHealth");
								settop(rlua_State, 0);
								Output(txtbox, ("Ungodded " + charname + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, 100);
								setfield(rlua_State, -2, "MaxHealth");
								settop(rlua_State, 0);
								Output(txtbox, ("Ungodded " + charname + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "jp") {
			if (In.size() == 3) {
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string charname = noobfaces[i];
								float hax = atof(In.at(2).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, hax);
								setfield(rlua_State, -2, "JumpPower");
								settop(rlua_State, 0);
								Output(txtbox, (charname + "'s JumpPower is now " + In.at(2) + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								string charname = *GetName(noobokokok);
								float hax = atof(In.at(2).c_str());
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, charname.c_str());
								getfield(rlua_State, -1, "Character");
								getfield(rlua_State, -1, "Humanoid");
								pushnumber(rlua_State, hax);
								setfield(rlua_State, -2, "JumpPower");
								settop(rlua_State, 0);
								Output(txtbox, (charname + "'s JumpPower is now " + In.at(2) + "\n"));
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "time") {
			if (In.size() == 2) {
				try {
					string newTimeOfDay = In.at(1);
					getglobal(rlua_State, "game");
					getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Lighting"); pcall(rlua_State, 2, 1, 0);
					pushstring(rlua_State, newTimeOfDay.c_str());
					setfield(rlua_State, -2, "TimeOfDay");
					//settop(rlua_State, 0);
					Output(txtbox, "Set TimeOfDay to " + In.at(1) + "\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "btools") {
			if (In.size() == 2) {
				getglobal(rlua_State, "game");
				getfield(rlua_State, -1, "PlaceId");
				int placeid = rlua_tonumber(rlua_State, -1);
				settop(rlua_State, 0);
				if (placeid == 16613936) {
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Backpack");
					getfield(rlua_State, -1, "Script");
					pushboolean(rlua_State, 1);
					setfield(rlua_State, -2, "Disabled");
					settop(rlua_State, 0);
					Output(txtbox, "bypassed greenwood town btools check\n");
				}
				try {
					if (In.at(1) == "all" || In.at(1) == "others") {
						vector<string> noobfaces = getnames(In.at(1));
						for (int i = 0; i < noobfaces.size(); i++) {

							try {
								string noobname = noobfaces[i];
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "HopperBin");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "BinType");
								settop(rlua_State, 0);

								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "HopperBin");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 3);
								setfield(rlua_State, -2, "BinType");
								settop(rlua_State, 0);

								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "HopperBin");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 4);
								setfield(rlua_State, -2, "BinType");
								settop(rlua_State, 0);
								Output(txtbox, "Gave btools to " + noobname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
					else {
						int noobokokok;
						try {
							noobokokok = GetPlayer(In.at(1));
						}
						catch (exception) {
							Sleep(1);
						}
						if (noobokokok == 0) {
							Output(txtbox, In.at(1) + " not found\n");
						}
						else {
							try {
								int noob = GetPlayer(In.at(1));
								string noobname = *GetName(noob);
								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "HopperBin");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 1);
								setfield(rlua_State, -2, "BinType");
								settop(rlua_State, 0);

								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "HopperBin");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 3);
								setfield(rlua_State, -2, "BinType");
								settop(rlua_State, 0);

								getglobal(rlua_State, "game");
								getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
								getfield(rlua_State, -1, noobname.c_str());
								getfield(rlua_State, -1, "Backpack");
								getglobal(rlua_State, "Instance");
								getfield(rlua_State, -1, "new");
								pushstring(rlua_State, "HopperBin");
								pushvalue(rlua_State, -4);
								pcall(rlua_State, 2, 1, 0);
								pushnumber(rlua_State, 4);
								setfield(rlua_State, -2, "BinType");
								settop(rlua_State, 0);
								Output(txtbox, "Gave btools to " + noobname + "\n");
							}
							catch (exception) {
								Sleep(1);
							}
						}
					}
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
		if (In.at(0) == "makepart") {
			if (In.size() == 1) {
				try {
					getglobal(rlua_State, "workspace");
					getglobal(rlua_State, "Instance");
					getfield(rlua_State, -1, "new");
					pushstring(rlua_State, "Part");
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 1, 0);
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "Character");
					getfield(rlua_State, -1, "Head");
					getfield(rlua_State, -1, "Position");
					setfield(rlua_State, -5, "Position");
					settop(rlua_State, 0);
					Output(txtbox, "Made a part in the Workspace\n");
				}
				catch (exception) {
					Sleep(1);
				}
			}
		}
	}
}


/*
int noobhook(int rlua_State) {
try {
string chatMsg = rlua_tolstring(rlua_State, -2, NULL);
if (chatMsg.substr(0, prefix.size()) == prefix || chatMsg.substr(0, (3 + prefix.size())) == "/e " + prefix) {
if (chatMsg.substr(0, prefix.size()) == prefix) {
chatMsg = chatMsg.substr(prefix.size(), std::string::npos);
ChangeContextLevel(7);
vector<string> In = split(chatMsg);
In.at(0) = to_lower_copy(In.at(0));
if (In.at(0) == "cmds") {
Output(txtbox, "Please type cmds on the noobhax GUI and not with chathook\n");
}
else {
if (In.at(0) == "admin") {
if (In.size() == 2) {
try {
if (In.at(1) == "all" || In.at(1) == "others") {
vector<string> noobfaces = getnames(In.at(1));
for (int i = 0; i < noobfaces.size(); i++) {

try {
string noob1 = noobfaces[i];
bool isadmin = IsAdmin(noob1);
if (isadmin) {
Output(txtbox, noob1 + " is already an admin\n");
}
else {
string hithere = "You're now a noobhax admin! Remember: don't use 'me'. Try " + prefix + "ff [your name]";
callcheck();
rlua_pushcfunction(rlua_State, (int)EIP_BP);
setglobal(rlua_State, "mynoobhookfunction");
settop(rlua_State, 0);
getglobal(rlua_State, "mynoobhookfunction");
setglobal(rlua_State, "mynoobhookfunc");
settop(rlua_State, 0);
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
getfield(rlua_State, -1, noob1.c_str());
getfield(rlua_State, -1, "Chatted");
getfield(rlua_State, -1, "connect");
pushvalue(rlua_State, -2);
getglobal(rlua_State, "mynoobhookfunc");
pcall(rlua_State, 2, 0, 0);
settop(rlua_State, 0);
callcheck2();
Output(txtbox, (noob1 + " is now an admin\n"));
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
getfield(rlua_State, -1, noob1.c_str());
getfield(rlua_State, -1, "Character");
getfield(rlua_State, -1, "Head");
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "Chat");
getfield(rlua_State, -1, "Chat");
pushvalue(rlua_State, -2);
pushvalue(rlua_State, -5);
pushstring(rlua_State, hithere.c_str());
pushstring(rlua_State, "Red");
pcall(rlua_State, 4, 0, 0);
settop(rlua_State, 0);
adminplayers.push_back(noob1);
}
}
catch (exception) {
Sleep(1);
}
}
}
else {
int noobokokok;
try {
noobokokok = GetPlayer(In.at(1));
}
catch (exception) {
Sleep(1);
}
if (noobokokok == 0) {
Output(txtbox, In.at(1) + " not found\n");
}
else {
try {
string noob1 = *GetName(noobokokok);
bool isadmin = IsAdmin(noob1);
if (isadmin) {
Output(txtbox, noob1 + " is already an admin\n");
}
else {
string hithere = "You're now a noobhax admin! Remember: don't use 'me'. Try " + prefix + "ff [your name]";
callcheck();
rlua_pushcfunction(rlua_State, (int)EIP_BP);
setglobal(rlua_State, "mynoobhookfunction");
settop(rlua_State, 0);
getglobal(rlua_State, "mynoobhookfunction");
setglobal(rlua_State, "mynoobhookfunc");
settop(rlua_State, 0);
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
getfield(rlua_State, -1, noob1.c_str());
getfield(rlua_State, -1, "Chatted");
getfield(rlua_State, -1, "connect");
pushvalue(rlua_State, -2);
getglobal(rlua_State, "mynoobhookfunc");
pcall(rlua_State, 2, 0, 0);
settop(rlua_State, 0);
callcheck2();
Output(txtbox, (noob1 + " is now an admin\n"));
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
getfield(rlua_State, -1, noob1.c_str());
getfield(rlua_State, -1, "Character");
getfield(rlua_State, -1, "Head");
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "Chat");
getfield(rlua_State, -1, "Chat");
pushvalue(rlua_State, -2);
pushvalue(rlua_State, -5);
pushstring(rlua_State, hithere.c_str());
pushstring(rlua_State, "Red");
pcall(rlua_State, 4, 0, 0);
settop(rlua_State, 0);
adminplayers.push_back(noob1);
}
}
catch (exception) {
Sleep(1);
}
}
}
}
catch (exception) {
Sleep(1);
}
}
}
else {
Execute(chatMsg, rlua_State);
}
}
}
if (chatMsg.substr(0, 3 + prefix.size()) == "/e " + prefix) {
chatMsg = chatMsg.substr(3 + prefix.size(), std::string::npos);
ChangeContextLevel(7);
vector<string> In = split(chatMsg);
In.at(0) = to_lower_copy(In.at(0));
if (In.at(0) == "cmds") {
Output(txtbox, "Please type cmds on the noobhax GUI and not with chathook\n");
}
else {
if (In.at(0) == "admin") {
if (In.size() == 2) {
try {
if (In.at(1) == "all" || In.at(1) == "others") {
vector<string> noobfaces = getnames(In.at(1));
for (int i = 0; i < noobfaces.size(); i++) {

try {
string noob1 = noobfaces[i];
bool isadmin = IsAdmin(noob1);
if (isadmin) {
Output(txtbox, noob1 + " is already an admin\n");
}
else {
string hithere = "You're now a noobhax admin! Remember: don't use 'me'. Try " + prefix + "ff [your name]";
callcheck();
rlua_pushcfunction(rlua_State, (int)EIP_BP);
setglobal(rlua_State, "mynoobhookfunction");
settop(rlua_State, 0);
getglobal(rlua_State, "mynoobhookfunction");
setglobal(rlua_State, "mynoobhookfunc");
settop(rlua_State, 0);
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
getfield(rlua_State, -1, noob1.c_str());
getfield(rlua_State, -1, "Chatted");
getfield(rlua_State, -1, "connect");
pushvalue(rlua_State, -2);
getglobal(rlua_State, "mynoobhookfunc");
pcall(rlua_State, 2, 0, 0);
settop(rlua_State, 0);
callcheck2();
Output(txtbox, (noob1 + " is now an admin\n"));
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
getfield(rlua_State, -1, noob1.c_str());
getfield(rlua_State, -1, "Character");
getfield(rlua_State, -1, "Head");
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "Chat");
getfield(rlua_State, -1, "Chat");
pushvalue(rlua_State, -2);
pushvalue(rlua_State, -5);
pushstring(rlua_State, hithere.c_str());
pushstring(rlua_State, "Red");
pcall(rlua_State, 4, 0, 0);
settop(rlua_State, 0);
adminplayers.push_back(noob1);
}
}
catch (exception) {
Sleep(1);
}
}
}
else {
int noobokokok;
try {
noobokokok = GetPlayer(In.at(1));
}
catch (exception) {
Sleep(1);
}
if (noobokokok == 0) {
Output(txtbox, In.at(1) + " not found\n");
}
else {
try {
string noob1 = *GetName(noobokokok);
bool isadmin = IsAdmin(noob1);
if (isadmin) {
Output(txtbox, noob1 + " is already an admin\n");
}
else {
string hithere = "You're now a noobhax admin! Remember: don't use 'me'. Try " + prefix + "ff [your name]";
callcheck();
rlua_pushcfunction(rlua_State, (int)EIP_BP);
setglobal(rlua_State, "mynoobhookfunction");
settop(rlua_State, 0);
getglobal(rlua_State, "mynoobhookfunction");
setglobal(rlua_State, "mynoobhookfunc");
settop(rlua_State, 0);
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
getfield(rlua_State, -1, noob1.c_str());
getfield(rlua_State, -1, "Chatted");
getfield(rlua_State, -1, "connect");
pushvalue(rlua_State, -2);
getglobal(rlua_State, "mynoobhookfunc");
pcall(rlua_State, 2, 0, 0);
settop(rlua_State, 0);
callcheck2();
Output(txtbox, (noob1 + " is now an admin\n"));
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
getfield(rlua_State, -1, noob1.c_str());
getfield(rlua_State, -1, "Character");
getfield(rlua_State, -1, "Head");
getglobal(rlua_State, "game");
getfield(rlua_State, -1, "Chat");
getfield(rlua_State, -1, "Chat");
pushvalue(rlua_State, -2);
pushvalue(rlua_State, -5);
pushstring(rlua_State, hithere.c_str());
pushstring(rlua_State, "Red");
pcall(rlua_State, 4, 0, 0);
settop(rlua_State, 0);
adminplayers.push_back(noob1);
}
}
catch (exception) {
Sleep(1);
}
}
}
}
catch (exception) {
Sleep(1);
}
}
}
else {
Execute(chatMsg, rlua_State);

}
}
}
}
else {
Sleep(1);
}

}
catch (exception) {
Sleep(1);
}
return 1;
}
*/

WNDPROC oldEditProc;

WNDPROC oldEditProc2;

bool Win7bool = false;


LRESULT CALLBACK subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (GetKeyState(VK_CONTROL) & 0x8000 && wParam == 'A') {
			SendMessage(wnd, EM_SETSEL, 0, -1);
		}
		switch (wParam)
		{
		case VK_RETURN:
			TCHAR * buffer = 0;
			int nLen = 0;
			if (whitelisted && scanned) {
				nLen = GetWindowTextLength(InputField);
				if (nLen > 0)
				{
					buffer = new TCHAR[nLen + 1];
					GetWindowText(InputField, buffer, nLen + 1);
				}
				if (buffer && sizeof(buffer) != 0) {
					string same = buffer;
					boost::thread *t = new boost::thread();
					*t = boost::thread(
						boost::bind(&Execute2, same, rlua_State, boost::ref(my_threads), t)
					);
					my_threads.add_thread(t);
				}
			}
			else if (whitelisted && !scanned) {
				Output(txtbox, "Please wait ok, if you crashed please press ALT+F4 on noobhax and rejoin\nIf you are on windows 7 and you get kicked by server, you won't get a refund.");
			}
			SetWindowText(InputField, "");
			break;
		}
	default:
		return CallWindowProc(oldEditProc, wnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK subEditProc2(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (GetKeyState(VK_CONTROL) & 0x8000 && wParam == 'A') {
			SendMessage(wnd, EM_SETSEL, 0, -1);
		}
	default:
		return CallWindowProc(oldEditProc2, wnd, msg, wParam, lParam);
	}
	return 0;
}

BOOL CreateSubwindows() {
	LoadLibrary("msftedit.dll");
	txtbox = CreateWindowExW(0, MSFTEDIT_CLASS, L"", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL, 5, 5, 408, 240, MainWindow, (HMENU)NOOBXPLOIT_CONSOLE_WINDOW, HInstance, 0);
	InputField = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | WS_VISIBLE, 3, 249, 413.5, 25, MainWindow, (HMENU)NOOBXPLOIT_INPUT_FIELD, HInstance, 0);
	oldEditProc = (WNDPROC)SetWindowLongPtr(InputField, GWLP_WNDPROC, (LONG_PTR)subEditProc);
	SendMessage(InputField, EM_SETLIMITTEXT, HUGE_VAL, NULL);
	HFONT textFont = CreateFont(18, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
	SendMessage(txtbox, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(InputField, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	UpdateWindow(MainWindow);
	return 1;
}

BOOL CreateSubwindows2() {
	luacbox = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOHSCROLL | ES_MULTILINE | WS_VISIBLE | ES_AUTOVSCROLL, 5, 5, 408, 240, LuaCWindow, (HMENU)NOOBXPLOIT_LUAC_WINDOW, HInstance2, 0);
	HWND execute = CreateWindowEx(NULL, "button", "Execute", WS_CHILD | WS_VISIBLE | WS_BORDER, 3, 249, 413.5, 35, LuaCWindow, (HMENU)NOOBXPLOIT_LUAC_EXE, HInstance2, NULL);
	oldEditProc2 = (WNDPROC)SetWindowLongPtr(luacbox, GWLP_WNDPROC, (LONG_PTR)subEditProc2);
	SendMessage(luacbox, EM_SETLIMITTEXT, HUGE_VAL, NULL);
	HFONT textFont = CreateFont(22, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
	SendMessage(luacbox, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(execute, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	UpdateWindow(LuaCWindow);
	return 1;
}

bool richbool = false;

BOOL CreateWindowMenu() {
	WindowMenu = CreateMenu();
	if (!WindowMenu)
		return 0;

	RBLXWindowProc = (WNDPROC)GetWindowLongA(FindWindowW(NULL, L"Roblox"), -4);

	HMENU mainDropdown = CreatePopupMenu();
	AppendMenu(mainDropdown, MF_STRING, MNOOBXPLOIT_EXIT, "Exit");
	AppendMenu(mainDropdown, MF_STRING, MNOOBXPLOIT_MINIMIZE, "Minimize");
	AppendMenu(mainDropdown, MF_STRING, MNOOBXPLOIT_DONATE, "Donate");
	AppendMenu(mainDropdown, MF_STRING, MNOOBXPLOIT_LUACSAME, "Lua C execution");
	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)mainDropdown, "noobhax");


	HMENU secondDropdown = CreatePopupMenu();
	AppendMenu(secondDropdown, MF_STRING, MNOOBXPLOIT_LOCALFFBTOOLS, "Forcefield + btools");
	AppendMenu(secondDropdown, MF_STRING, MNOOBXPLOIT_LOCALFIRE, "Fire");
	AppendMenu(secondDropdown, MF_STRING, MNOOBXPLOIT_LOCALSPARKLES, "Sparkles");
	AppendMenu(secondDropdown, MF_STRING, MNOOBXPLOIT_LOCALPARTICLES, "Particles");
	AppendMenu(secondDropdown, MF_STRING, MNOOBXPLOIT_LOCALSMOKE, "Smoke");
	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)secondDropdown, "LocalPlayer");


	HMENU HumanoidDropdown = CreatePopupMenu();
	AppendMenu(secondDropdown, MF_POPUP, (UINT_PTR)HumanoidDropdown, "Humanoid");
	AppendMenu(HumanoidDropdown, MF_STRING, MNOOBXPLOIT_LOCALGOD, "God");
	AppendMenu(HumanoidDropdown, MF_STRING, MNOOBXPLOIT_LOCALMOREWS, "WalkSpeed +16");
	AppendMenu(HumanoidDropdown, MF_STRING, MNOOBXPLOIT_LOCALLESSWS, "WalkSpeed -16");
	AppendMenu(HumanoidDropdown, MF_STRING, MNOOBXPLOIT_LOCALMOREJP, "JumpPower +50");
	AppendMenu(HumanoidDropdown, MF_STRING, MNOOBXPLOIT_LOCALLESSJP, "JumpPower -50");


	AppendMenu(secondDropdown, MF_STRING, MNOOBXPLOIT_LOCALSSJGOD, "SUPER SAIYAN GOD");
	AppendMenu(secondDropdown, MF_STRING, MNOOBXPLOIT_LOCALSSJGODSSJ, "SUPER SAIYAN GOD SUPER SAIYAN");
	AppendMenu(secondDropdown, MF_STRING, MNOOBXPLOIT_LOCALSSJROSE, "SUPER SAIYAN ROSÉ");
	HMENU HatsDropdown = CreatePopupMenu();
	AppendMenu(secondDropdown, MF_POPUP, (UINT_PTR)HatsDropdown, "Hats");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_LOCALNOOBHAXHAT, "noobhax cap");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_LOCALDOMINUS, "Dominus hat");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_LOCALMLG, "MLG sunglasses");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_LOCALBACON, "Bacon hair");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_LOCALSSJHAIR, "Super Saiyan hair");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_LOCALSSJGODHAIR, "Super Saiyan God hair");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_LOCALSSJGODSSJHAIR, "Super Saiyan God Super Saiyan hair");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_LOCALSSJROSEHAIR, "Super Saiyan Rosé hair");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_NOOBHAXSIGN, "noobhax sign");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_NOOBHAXHP, "Headphones");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_SPARKLETIME, "Red Sparkle Time Fedora");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_NOOBHAXBC, "BC hat");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_NOOBHAXTBC, "TBC hat");
	AppendMenu(HatsDropdown, MF_STRING, MNOOBXPLOIT_NOOBHAXOBC, "OBC hat");

	HMENU thirdDropdown = CreatePopupMenu();
	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)thirdDropdown, "Extra");

	HMENU ARBDropdown = CreatePopupMenu();
	AppendMenu(thirdDropdown, MF_POPUP, (UINT_PTR)ARBDropdown, "Auto Rap Battles (PlaceId 1125790688)");
	AppendMenu(ARBDropdown, MF_STRING, MNOOBXPLOIT_LOCALRAPTP, "Tp to the dj area");
	AppendMenu(ARBDropdown, MF_STRING, MNOOBXPLOIT_LOCALRAP2TP, "Tp to the rappers' area");
	HMENU PLDropdown = CreatePopupMenu();
	AppendMenu(thirdDropdown, MF_POPUP, (UINT_PTR)PLDropdown, "Prison Life v2.0");
	AppendMenu(PLDropdown, MF_STRING, MNOOBXPLOIT_TPOUTOFPRISON, "Tp out of prison");
	AppendMenu(PLDropdown, MF_STRING, MNOOBXPLOIT_PLPOLICEAREA, "Tp to the police area");
	HMENU UWDropdown = CreatePopupMenu();
	AppendMenu(thirdDropdown, MF_POPUP, (UINT_PTR)UWDropdown, "The Underground War");
	AppendMenu(UWDropdown, MF_STRING, MNOOBXPLOIT_UWRFLAG, "Tp to the red flag");
	AppendMenu(UWDropdown, MF_STRING, MNOOBXPLOIT_UWBFLAG, "Tp to the blue flag");
	HMENU MGDropdown = CreatePopupMenu();
	AppendMenu(thirdDropdown, MF_POPUP, (UINT_PTR)MGDropdown, "Murder games");
	AppendMenu(MGDropdown, MF_STRING, MNOOBXPLOIT_MURDERGAMES, "Detect Murderer and Sheriff");
	HMENU BGDropdown = CreatePopupMenu();
	AppendMenu(thirdDropdown, MF_POPUP, (UINT_PTR)BGDropdown, "Boys And Girls Hangout (by DemSkittlesDoee)");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_CASH, "Get 1000000 Cash");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_CUSTOMNAME, "Get a cool custom name");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_PARTICLE, "Get Particle");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_PARTICLE1, "Get Particle1");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_PARTICLE2, "Get Particle2");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_PARTICLE3, "Get Particle3");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_PARTICLE4, "Get Particle4");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_PARTICLE6, "Get Particle6");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_PARTICLE7, "Get Particle7");
	AppendMenu(BGDropdown, MF_STRING, MNOOBXPLOIT_PARTICLE8, "Get Particle8");
	HMENU PoFDropdown = CreatePopupMenu();
	AppendMenu(thirdDropdown, MF_POPUP, (UINT_PTR)PoFDropdown, "Plates of Fate: Mayhem (by DylanBuildar)");
	AppendMenu(PoFDropdown, MF_STRING, MNOOBXPLOIT_PoFTPOBBY, "Tp to the end of the obby");
	HMENU RPDropdown = CreatePopupMenu();
	AppendMenu(thirdDropdown, MF_POPUP, (UINT_PTR)RPDropdown, "Redwood Prison (by RoyStanford)");
	AppendMenu(RPDropdown, MF_STRING, MNOOBXPLOIT_SPAS12, "Get SPAS-12");
	AppendMenu(RPDropdown, MF_STRING, MNOOBXPLOIT_M16, "Get M16");
	AppendMenu(RPDropdown, MF_STRING, MNOOBXPLOIT_M60, "Get M60");
	AppendMenu(RPDropdown, MF_STRING, MNOOBXPLOIT_ACR, "Get ACR");
	AppendMenu(RPDropdown, MF_STRING, MNOOBXPLOIT_AK47, "Get AK47");
	AppendMenu(RPDropdown, MF_STRING, MNOOBXPLOIT_FAKEIDCARD, "Get Fake ID Card");
	AppendMenu(RPDropdown, MF_STRING, MNOOBXPLOIT_BERETTAM9, "Get Beretta M9");
	AppendMenu(RPDropdown, MF_STRING, MNOOBXPLOIT_TASER, "Get Taser");
	AppendMenu(RPDropdown, MF_STRING, MNOOBXPLOIT_HANDCUFFS, "Get Handcuffs");
	/*HMENU JBDropdown = CreatePopupMenu();
	AppendMenu(thirdDropdown, MF_POPUP, (UINT_PTR)JBDropdown, "Jailbreak (Beta) (by Badimo)");
	AppendMenu(JBDropdown, MF_STRING, MNOOBXPLOIT_JBNODOORS, "Remove doors");
	AppendMenu(JBDropdown, MF_STRING, MNOOBXPLOIT_JBTP1, "Tp to the Criminal Base 1");
	AppendMenu(JBDropdown, MF_STRING, MNOOBXPLOIT_JBTP2, "Tp to the Garage");
	AppendMenu(JBDropdown, MF_STRING, MNOOBXPLOIT_JBTP3, "Tp to the Helicopter Spawn 1");
	AppendMenu(JBDropdown, MF_STRING, MNOOBXPLOIT_JBTP4, "Tp to the Outside Bank");
	AppendMenu(JBDropdown, MF_STRING, MNOOBXPLOIT_JBTP5, "Tp to the Outside Prison");
	AppendMenu(JBDropdown, MF_STRING, MNOOBXPLOIT_JBTP6, "Tp to the Police Base 1");
	AppendMenu(JBDropdown, MF_STRING, MNOOBXPLOIT_JBTP7, "Tp to the Police Base 2");
	HMENU JBMoreDropdown = CreatePopupMenu();
	AppendMenu(JBDropdown, MF_POPUP, (UINT_PTR)JBMoreDropdown, "More");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP8, "Tp to the Criminal Base 2");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP9, "Tp to the Jewelry Store");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP10, "Tp to the Swat Van");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP11, "Tp to the Mini Cooper Car");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP12, "Tp to the Pickup Truck");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP13, "Tp to the Lamborghini (Bank)");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP14, "Tp to the Lamborghini (Criminal Base 2)");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP15, "Tp to the Heli (Jail)");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP16, "Tp to the Heli (Police Base 2)");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP17, "Tp to the Bugatti");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP18, "Tp to the Dirtbike");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP19, "Tp to the Gas Station");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP20, "Tp to the Donut Shop");
	AppendMenu(JBMoreDropdown, MF_STRING, MNOOBXPLOIT_JBTP21, "Tp to the Vault");
	*/
	HMENU clickDropdown = CreatePopupMenu();
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKDEL, "CTRL + click to delete");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKTP, "CTRL + click to tp");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKEXPLODE, "CTRL + click to explode");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKPART, "CTRL + click to make part");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKUNANCHOR, "CTRL + click to unanchor");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKANCHOR, "CTRL + click to anchor");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKLOCK, "CTRL + click to lock");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKUNLOCK, "CTRL + click to unlock");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKCANCOLLIDEFALSE, "CTRL + click to CanCollide false");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKCANCOLLIDETRUE, "CTRL + click to CanCollide true");
	AppendMenu(clickDropdown, MF_STRING, MNOOBXPLOIT_CLICKNOTHING, "Disable CTRL + click commands");
	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)clickDropdown, "CTRL + click");
	SetWindowLongA(FindWindowW(NULL, L"Roblox"), -4, (LONG)DLLWindowProc);
	bool valid = true;
	bool valid2 = true;
	if (!FileExists("C:\\noobhax\\oldhwid.txt")) {
		string haxxing = "echo " + fuckingosha() + " >\"C:\\noobhax\\oldhwid.txt\"";
		system(haxxing.c_str());
	}
	if (FileExists("C:\\noobhax\\win7.txt")) {
		ifstream infile;
		infile.open("C:\\noobhax\\win7.txt", ifstream::in);
		bool done = false;
		if (infile.is_open())
		{
			while (infile.good() && !done)
			{
				string temp = "";
				getline(infile, temp);
				if (to_upper_copy(temp).substr(0, 3) == "YES" && !done) {
					Win7bool = true;
					done = true;
				}
				else if (to_upper_copy(temp).substr(0, 2) == "NO" && !done) {
					done = true;
				}
				else {
					valid = false;
					done = true;
				}
			}
			infile.close();
		}
		else
		{
			Output(txtbox, "Error opening file win7.txt\n");
		}
	}
	if (FileExists("C:\\noobhax\\rich.txt")) {
		ifstream infile2;
		infile2.open("C:\\noobhax\\rich.txt", ifstream::in);
		bool done2 = false;
		if (infile2.is_open())
		{
			while (infile2.good() && !done2)
			{
				string temp2 = "";
				getline(infile2, temp2);
				if (to_upper_copy(temp2).substr(0, 3) == "YES" && !done2) {
					richbool = true;
					done2 = true;
				}
				else if (to_upper_copy(temp2).substr(0, 2) == "NO" && !done2) {
					done2 = true;
				}
				else {
					valid2 = false;
					done2 = true;
				}
			}
			infile2.close();
		}
		else
		{
			Output(txtbox, "Error opening file rich.txt\n");
		}
	}
	if (!FileExists("C:\\noobhax\\win7.txt") || !valid) {
		if (MessageBox(NULL, "Are you on Windows 7?", "noobhax question", MB_YESNO) == IDYES) {
			Win7bool = true;
			system("echo YES >\"C:\\noobhax\\win7.txt\"");
			MessageBox(NULL, "No refund for you.", "why didn't you read before buying????", MB_OK);
		}
		else {
			system("echo NO >\"C:\\noobhax\\win7.txt\"");
		}
	}
	if (!FileExists("C:\\noobhax\\rich.txt") || !valid2) {
		if (MessageBox(NULL, "Do you want to send friend requests automatically after you join a place and noobhax loads?", "noobhax question", MB_YESNO) == IDYES) {
			richbool = true;
			system("echo YES >\"C:\\noobhax\\rich.txt\"");
		}
		else {
			system("echo NO >\"C:\\noobhax\\rich.txt\"");
		}
	}
	if (!Win7bool) {
		SetMenu(FindWindowW(NULL, L"Roblox"), WindowMenu);
		DrawMenuBar(FindWindowW(NULL, L"Roblox"));
	}

	return 1;
}

BOOL CreateWindowMenu2() {
	WindowMenu2 = CreateMenu();
	if (!WindowMenu2)
		return 0;

	HMENU mainDropdown20 = CreatePopupMenu();
	AppendMenu(mainDropdown20, MF_STRING, MNOOBXPLOIT_MIN2, "Minimize");
	AppendMenu(WindowMenu2, MF_POPUP, (UINT_PTR)mainDropdown20, "Lua C window");

	return 1;
}


LRESULT CALLBACK RobloxWndProcHook(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_INPUT)
	{
		if (GetForegroundWindow() == MainWindow || GetForegroundWindow() == LuaCWindow)
		{
			UINT sz = sizeof(RAWINPUTHEADER);
			RAWINPUTHEADER hdr;
			memset(&hdr, 0, sz);

			GetRawInputData((HRAWINPUT)lParam, RID_HEADER, (LPVOID)(&hdr), &sz, sizeof(RAWINPUTHEADER));

			if (hdr.dwType == RIM_TYPEKEYBOARD)
				return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	return CallWindowProc(DLLWindowProc, hwnd, message, wParam, lParam);
}

void HookRobloxWndProc()
{
	HWND RobloxWindow = FindWindowW(NULL, L"Roblox");
	if (RobloxWindow) {
		SetWindowLongPtr(RobloxWindow, GWLP_WNDPROC, (LONG)RobloxWndProcHook);
	}
}

BOOL InitiateWindow() {
	HInstance = GetModuleHandle(NULL);

	UnregisterClass("NOOBXPLOIT_WINDOW", HInstance);
	RegisterWindowClass("NOOBXPLOIT_WINDOW");

	char NOOBXPLOITName[80];

	_snprintf_s(NOOBXPLOITName, 80, "noobhax re-reborn beta - by supersupersnivy97 @V3rmillion");

	if (!CreateWindowMenu())
		return 0;

	if (Win7bool) {
		MessageBox(NULL, "No refund for you.", "why didn't you read before buying????", MB_OK);
		if (!(MainWindow = CreateWindowEx(
			NULL,
			"NOOBXPLOIT_WINDOW",
			NOOBXPLOITName,
			WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			425,
			326,
			NULL,
			WindowMenu,
			HInstance,
			NULL))) return 0;
	}
	else {
		if (!(MainWindow = CreateWindowEx(
			NULL,
			"NOOBXPLOIT_WINDOW",
			NOOBXPLOITName,
			WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			425,
			307.5,
			NULL,
			0,
			HInstance,
			NULL))) return 0;
	}
	CreateSubwindows();
	ShowWindow(MainWindow, SW_SHOWNORMAL);
	SetWindowPos(MainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	UpdateWindow(MainWindow);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, 0, 0, 0);
	HookRobloxWndProc();


	return StartMessageLoop();
}

BOOL InitiateWindow2() {
	HInstance2 = GetModuleHandle(NULL);

	UnregisterClass("NOOBXPLOIT_LUAC", HInstance2);
	RegisterWindowClass("NOOBXPLOIT_LUAC");

	char NOOBXPLOITLuaCName[80];

	_snprintf_s(NOOBXPLOITLuaCName, 80, "noobhax Lua C execution");

	if (!CreateWindowMenu2())
		return 0;

	if (!(LuaCWindow = CreateWindowEx(
		NULL,
		"NOOBXPLOIT_LUAC",
		NOOBXPLOITLuaCName,
		WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		425,
		338,
		NULL,
		WindowMenu2,
		HInstance2,
		NULL))) return 0;

	CreateSubwindows2();
	ShowWindow(LuaCWindow, SW_SHOWNORMAL);
	SetWindowPos(LuaCWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	UpdateWindow(LuaCWindow);


	return StartMessageLoop();
}

//hi ok

void ShowWindowForm() {
	InitiateWindow();
}

void ShowLuaCForm() {
	InitiateWindow2();
}

/*

int rclick(int rlua_State) {
if (boolrclicktotp) {
try {
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "Character");
bool charOK = rlua_toboolean(rlua_State, -1);
settop(rlua_State, 0);
if (charOK) {
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "Character");
getfield(rlua_State, -1, "Humanoid");
getfield(rlua_State, -1, "Health");
int healthOK = rlua_tonumber(rlua_State, -1);
settop(rlua_State, 0);
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "Character");
getfield(rlua_State, -1, "Humanoid");
getfield(rlua_State, -1, "MaxHealth");
int maxhealthOK = rlua_tonumber(rlua_State, -1);
settop(rlua_State, 0);
if (healthOK > 0 || to_string(maxhealthOK) == "-2147483648") {
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "GetMouse");
pushvalue(rlua_State, -2);
pcall(rlua_State, 1, 1, 0);
getfield(rlua_State, -1, "Hit");
getfield(rlua_State, -1, "p");
getglobal(rlua_State, "CFrame");
getfield(rlua_State, -1, "new");
pushvalue(rlua_State, -3);
pcall(rlua_State, 1, 1, 0);
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "Character");
getfield(rlua_State, -1, "HumanoidRootPart");
pushvalue(rlua_State, -6);
setfield(rlua_State, -2, "CFrame");
settop(rlua_State, 0);
}
else {
Sleep(1);
}
}
else {
Sleep(1);
}
}
catch (exception) {
Sleep(1);
}
}
else {
Sleep(1);
}
return 0;
}

int clicktodelete(int rlua_State) {
bool isabasepart;
bool exists;
string targetname;
if (boolclicktodelete) {
try {
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "GetMouse");
pushvalue(rlua_State, -2);
pcall(rlua_State, 1, 1, 0);
getfield(rlua_State, -1, "Target");
exists = rlua_toboolean(rlua_State, -1);
settop(rlua_State, 0);
if (exists) {
try {
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "GetMouse");
pushvalue(rlua_State, -2);
pcall(rlua_State, 1, 1, 0);
getfield(rlua_State, -1, "Target");
getfield(rlua_State, -1, "Name");
targetname = rlua_tolstring(rlua_State, -1, NULL);
settop(rlua_State, 0);
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "GetMouse");
pushvalue(rlua_State, -2);
pcall(rlua_State, 1, 1, 0);
getfield(rlua_State, -1, "Target");
getfield(rlua_State, -1, "IsA");
pushvalue(rlua_State, -2);
pushstring(rlua_State, "BasePart");
pcall(rlua_State, 2, 1, 0);
isabasepart = rlua_toboolean(rlua_State, -1);
settop(rlua_State, 0);
}
catch (exception) {
Sleep(1);
}
if (isabasepart) {
try {
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "GetMouse");
pushvalue(rlua_State, -2);
pcall(rlua_State, 1, 1, 0);
getfield(rlua_State, -1, "Target");
getfield(rlua_State, -1, "remove");
pushvalue(rlua_State, -2);
pcall(rlua_State, 1, 0, 0);
settop(rlua_State, 0);
Output(txtbox, "Removed " + targetname + "\n");
}
catch (exception) {
Sleep(1);
}
}
else {
Output(txtbox, "Error: target isn't a BasePart\n");
Sleep(1);
}
}
else {
Sleep(1);
}
}
catch (exception) {
Sleep(1);
}
}
if (boolprintmousepos) {
try {
getglobal(rlua_State, "noobhaxmyplr");
getfield(rlua_State, -1, "GetMouse");
pushvalue(rlua_State, -2);
pcall(rlua_State, 1, 1, 0);
getfield(rlua_State, -1, "Hit");
getfield(rlua_State, -1, "p");
getglobal(rlua_State, "tostring");
pushvalue(rlua_State, -2);
pcall(rlua_State, 1, 1, 0);
string posok = rlua_tolstring(rlua_State, -1, NULL);
settop(rlua_State, 0);
getglobal(rlua_State, "print");
pushstring(rlua_State, posok.c_str());
pcall(rlua_State, 1, 0, 0);
settop(rlua_State, 0);
Output(txtbox, "Mouse position: " + posok + "\n");
}
catch (exception) {
Sleep(1);
}
}
else {
Sleep(1);
}
return 0;
}

*/

int refreshadminlist(int rlua_State) {
	pushvalue(rlua_State, -1);
	getfield(rlua_State, -1, "Name");
	string playername = rlua_tolstring(rlua_State, -1, NULL);
	settop(rlua_State, 0);
	if (playername != *GetName(GetLocalPlayer(Players))) {
		bool isadmink = IsAdmin(playername);
		if (isadmink) {
			for (int i = 0; i < adminplayers.size(); i++) {
				if (adminplayers[i] == playername) {
					adminplayers[i] = "";
				}
			}
			Output(txtbox, "Noobhax admin " + playername + " has left the game.\n");
		}
		else {
			Sleep(1);
		}
	}
	else {
		Sleep(1);
	}
	return 0;
}

int noobhaxexec(int rlua_State) {
	string asd = rlua_tolstring(rlua_State, -1, NULL);
	Execute(asd, rlua_State);
	return 0;
}

void clickhaxsame(boost::thread_group & thg, boost::thread * thisTh) {
	VMProtectBeginMutation("noobs eat noobs");
	int a = luaL_dostring(MyL, "while true do wait(100) if checkforkey(162) and checkforkey(1) then wait(1) if checkforkey(1) then wait(1) if checkforkey(162) then clicktp() end end end end");
	if (a) {
		string err = lua_tostring(MyL, -1);
		Output(txtbox, err + "\n", RGB(255, 0, 0));
	}
	VMProtectEnd();
	thg.remove_thread(thisTh);
	delete thisTh;
}

bool checkHostsFile()
{
	VMProtectBeginUltra("hi im noob123");
	ifstream infile;
	infile.open("C:\\Windows\\System32\\drivers\\etc\\hosts", ifstream::in);
	if (infile.is_open())
	{
		while (infile.good())
		{
			string line = "";
			getline(infile, line);
			if (line.find("gaynibber.xyz") != string::npos)
			{
				return true;
			}
		}
		infile.close();
		return false;
	}
	else
	{
		//error();
		hostsok = false;
		return true;
	}
	VMProtectEnd();
}


void auth() {
	VMProtectBeginUltra("noobo");
	string host = "http://gaynibber.xyz/nbhx/hwidisok.php?key=";
	string hwidPro = fuckingosha();
	string result = DownloadURL(host + hwidPro);
	if (result == "ok") {
		Output(txtbox, "Welcome back noobhax user! (authorized)\n");
		whitelisted = true;
	}
	else {
		Output(txtbox, "you're not whitelisted...\n");
		Sleep(5000);
		exit(5);
	}
	VMProtectEnd();
}

bool isRunning(LPCSTR pName)
{
	HWND hwnd;
	hwnd = FindWindow(NULL, pName);
	if (hwnd != 0) {
		return true;
	}
	else {
		return false;
	}
}

bool FindRunningProcess(std::string process) {
	/*
	Function takes in a string value for the process it is looking for like ST3Monitor.exe
	then loops through all of the processes that are currently running on windows.
	If the process is found it is running, therefore the function returns true.
	*/
	string compare;
	bool procRunning = false;

	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		procRunning = false;
	}
	else {
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hProcessSnap, &pe32)) { // Gets first running process
			if (pe32.szExeFile == process) {
				procRunning = true;
			}
			else {
				// loop through all running processes looking for process
				while (Process32Next(hProcessSnap, &pe32)) {
					// Set to an AnsiString instead of Char[] to make compare easier
					compare = pe32.szExeFile;
					if (compare == process) {
						// if found process is running, set to true and break from loop
						procRunning = true;
						break;
					}
				}
			}
			// clean the snapshot object
			CloseHandle(hProcessSnap);
		}
	}

	return procRunning;
}

void init() {
	VMProtectBeginUltra("hi");
	if (!hostsok) {
		Output(txtbox, "Couldn't open C:\\Windows\\System32\\drivers\\etc\\hosts\nplease restart your computer\n");
		Sleep(5000);
		exit(4);
	}
	if (checkHostsFile() && hostsok) {
		string ahithere = "dont change the hosts file";
		if (MessageBox(NULL, ahithere.c_str(), "noobhax error", MB_OK) == IDOK) {
			exit(4);
		}
	}
	HWND handle = GetForegroundWindow();
	int bufsize = GetWindowTextLength(handle) + 1;
	LPWSTR title = new WCHAR[bufsize];
	GetWindowText(handle, (LPSTR)title, bufsize);
	string hititle = (LPSTR)title;
	if (isRunning("HTTP Debugger Pro v8.10") || hititle.find("HTTP Debugger") != std::string::npos || hititle.find("HTTP") != std::string::npos || hititle.find("Debugger") != std::string::npos || hititle.find("HTTP Debugger Pro v8.10") != std::string::npos || isRunning("Fiddler Web Debugger") || hititle.find("Wireshark") != std::string::npos || hititle.find("Fiddler") != std::string::npos || FindRunningProcess("fiddler.exe") || FindRunningProcess("wireshark.exe") || FindRunningProcess("HTTPDebuggerUI.exe")) {
		string chithere = "close your web debugging program";
		if (MessageBox(NULL, chithere.c_str(), "noobhax error", MB_OK) == IDOK) {
			exit(4);
		}
	}
	auth();
	if (whitelisted) {
		Output(txtbox, "Scanning please wait...\n");
		MyL = lua_open();
		luaL_openlibs(MyL);
		DWORD VFTable = getaddy2(0x1031114);
		do {
			ScriptContext = Memory::Scan((char*)&VFTable);
		} while (!ScriptContext);
		DWORD VFTable2 = getaddy2(0x1032C98);
		do {
			Workspace = Memory::Scan((char*)&VFTable2);
		} while (!Workspace);
		DWORD VFTable3 = getaddy2(0x1029008);
		do {
			Players = Memory::Scan((char*)&VFTable3);
		} while (!Players);
		Output(txtbox, "1/4\n");
		DataModel = *(DWORD *)(ScriptContext + 52);
		Output(txtbox, "2/4\n");
		rlua_State = GetLuaState((DWORD*)ScriptContext);
		Output(txtbox, "3/4\n");
		getglobal(rlua_State, "workspace");
		Output(txtbox, "you're noob\n");
		getfield(rlua_State, -1, "FilteringEnabled");
		Output(txtbox, "you're not noob\n");
		bool FilteringEnabled = rlua_toboolean(rlua_State, -1);
		settop(rlua_State, 0);
		Output(txtbox, "4/4\n");
		string hahanoob = *GetName(GetLocalPlayer(Players));
		if (whitelisted) {
			//adminplayers.push_back(hahanoob);
			SetWindowText(txtbox, "");
			Output(txtbox, ("Welcome, " + hahanoob + "\n"));
			Output(txtbox, "Filtering is ");
			if (FilteringEnabled) {
				Output(txtbox, "enabled\nThis means changes won't replicate over the server\n", RGB(255, 0, 0), true);
			}
			else {
				Output(txtbox, "disabled\n", RGB(0, 255, 0), true);
			}
			Output(txtbox, "Type cmds for a list of commands\n");
			getglobal(rlua_State, "game");
			getfield(rlua_State, -1, "GetService"); pushvalue(rlua_State, -2); pushstring(rlua_State, "Players"); pcall(rlua_State, 2, 1, 0);
			getfield(rlua_State, -1, "LocalPlayer");
			setglobal(rlua_State, "noobhaxmyplr");
			settop(rlua_State, 0);
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "GetMouse");
			pushvalue(rlua_State, -2);
			pcall(rlua_State, 1, 1, 0);
			setglobal(rlua_State, "noobhaxmymouse");
			settop(rlua_State, 0);
			lua_pushcfunction(MyL, checkforkey);
			lua_setglobal(MyL, "checkforkey");
			lua_settop(MyL, 0);
			lua_pushcfunction(MyL, wait);
			lua_setglobal(MyL, "wait");
			lua_settop(MyL, 0);
			lua_pushcfunction(MyL, clicktp);
			lua_setglobal(MyL, "clicktp");
			lua_settop(MyL, 0);
			Output(txtbox, "CTRL + click to tp is enabled\n");
			Output(txtbox, "Type credits for the credits\n");
			boost::thread *t = new boost::thread();
			*t = boost::thread(
				boost::bind(&clickhaxsame, boost::ref(my_threads), t)
			);
			/*
			callcheck();
			rlua_pushcfunction(rlua_State, (int)EIP_BP);
			setglobal(rlua_State, "mynoobhookfunction");
			settop(rlua_State, 0);
			getglobal(rlua_State, "mynoobhookfunction");
			setglobal(rlua_State, "mynoobhookfunc");
			settop(rlua_State, 0);
			getglobal(rlua_State, "noobhaxmyplr");
			getfield(rlua_State, -1, "Chatted");
			getfield(rlua_State, -1, "connect");
			pushvalue(rlua_State, -2);
			getglobal(rlua_State, "mynoobhookfunc");
			pcall(rlua_State, 2, 0, 0);
			settop(rlua_State, 0);
			callcheck2();
			Output(txtbox, "Chathook is enabled try chatting ;ff me or /e ;ff me\n");
			*/
			if (richbool) {
				getglobal(rlua_State, "noobhaxmyplr");
				getfield(rlua_State, -1, "Parent");
				setglobal(rlua_State, "playersok");
				settop(rlua_State, 0);
				vector<string> names = getnames("others");
				for (int i = 0; i < names.size(); i++) {
					string person = names[i];
					getglobal(rlua_State, "playersok");
					getfield(rlua_State, -1, person.c_str());
					getglobal(rlua_State, "noobhaxmyplr");
					getfield(rlua_State, -1, "RequestFriendship");
					pushvalue(rlua_State, -2);
					pushvalue(rlua_State, -4);
					pcall(rlua_State, 2, 0, 0);
					settop(rlua_State, 0);
					std::this_thread::sleep_for(100ms);
				}
			}
			scanned = true;
		}
		else {
			Output(txtbox, "you're not whitelisted...\n");
			Sleep(5000);
			exit(5);
		}
	}
	else {
		Output(txtbox, "you're not whitelisted...\n");
		Sleep(5000);
		exit(5);
	}
	VMProtectEnd();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hinstDLL);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ShowWindowForm, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ChangeContextLevel, (LPVOID)7, 0, 0);
	}
	return 1;
}