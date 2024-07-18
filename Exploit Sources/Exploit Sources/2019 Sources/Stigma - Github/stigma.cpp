#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <WinInet.h>
#include <fstream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <string>
#include <Psapi.h>
#include <tlhelp32.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
	/* lua internals */
#include "lualib.h"
#include "lopcodes.h"
#include "lstate.h"
#include "lobject.h"
#include "lfunc.h"
#include "ldo.h"
}


DWORD RL;

#define QTX_Offset(x)(x - 0x400000 + ((DWORD)GetModuleHandleA(0)))

#define RLUA_TNIL 0
#define RLUA_TLIGHTUSERDATA 1
#define RLUA_TNUMBER 2
#define RLUA_TBOOLEAN 3
#define RLUA_TSTRING 4
#define RLUA_TTHREAD 5
#define RLUA_TFUNCTION 6
#define RLUA_TTABLE 7
#define RLUA_TUSERDATA 8

#define r_lua_setmetatable_addr QTX_Offset(0x510010)
#define r_lua_getfield_addr QTX_Offset(0x50e310)
#define r_luaD_precall_addr QTX_Offset(0x507c80)
#define r_lua_pcall_addr QTX_Offset(0x50f010)
#define r_lua_close_addr QTX_Offset(0x50eea0)
#define r_lua_newthread_addr QTX_Offset(0x50ed10)
#define r_lua_tolstring_addr QTX_Offset(0x510440)
#define r_lua_createtable_addr QTX_Offset(0x50dfb0)
#define r_lua_pushcclosure_addr QTX_Offset(0x50f140)
#define r_lua_B_settop_addr QTX_Offset(0x510210)
#define r_lua_newuserdata_addr QTX_Offset(0x50edf0)
#define r_lua_gettable_addr QTX_Offset(0x50D580)
#define r_lua_settable_addr QTX_Offset(0x50D7D0)
#define r_luaV_execute_addr QTX_Offset(0x50B440) 

#define ScriptContextAddress QTX_Offset(0x1188B80)

#define GlobalStates_ADDR QTX_Offset(0x6A8370) 

#define r_setobj setobj
#define r_etobjs2s  setobjs2s
#define r_setbvalue setbvalue

typedef int(__cdecl *r_lua_setmetatable_Def)(DWORD luaState, int object);
r_lua_setmetatable_Def r_lua_setmetatable = (r_lua_setmetatable_Def)r_lua_setmetatable_addr;
typedef int(__cdecl *r_lua_getfield_Def)(DWORD luaState, int object, const char* k);
r_lua_getfield_Def r_lua_getfield = (r_lua_getfield_Def)r_lua_getfield_addr;
typedef int(__cdecl *r_luaD_precall_Def)(DWORD luaState, int object1, int obj2);
r_luaD_precall_Def r_luaD_precall = (r_luaD_precall_Def)r_luaD_precall_addr;
typedef int(__cdecl *r_luaF_close_Def)(DWORD luaState);
r_luaF_close_Def r_luaF_close = (r_luaF_close_Def)r_lua_close_addr;
typedef int(__cdecl *r_luaC_createtable_Def)(DWORD luaState, int table_object, int table_value);
r_luaC_createtable_Def r_luaC_createtable = (r_luaC_createtable_Def)r_lua_createtable_addr;
typedef int(__cdecl *r_luaF_pushcclosure_Def)(DWORD luaState, int fn, int nothing_func);
r_luaF_pushcclosure_Def r_lua_pushcclosure = (r_luaF_pushcclosure_Def)r_lua_pushcclosure_addr;
typedef int(__cdecl *r_luaF_newthread)(DWORD luaState);
r_luaF_newthread r_lua_newthread = (r_luaF_newthread)r_lua_newthread_addr;
typedef int(__cdecl *r_luaB_settop_Def)(DWORD luaState, int settop_idx);
r_luaB_settop_Def r_luaB_settop = (r_luaB_settop_Def)r_lua_B_settop_addr;
typedef int(__cdecl *r_lua_NewUserData_Def)(DWORD luaState, int idx);
r_lua_NewUserData_Def r_lua_newuserdata = (r_lua_NewUserData_Def)r_lua_newthread_addr;
typedef int(__cdecl *r_luaV_gettable_Def)(DWORD a, DWORD b, DWORD c, DWORD d);
r_luaV_gettable_Def r_lua_gettable = (r_luaV_gettable_Def)r_lua_gettable_addr;
typedef int(__cdecl *r_luaV_gettable_Def)(DWORD a, DWORD b, DWORD c, DWORD d);
r_luaV_gettable_Def r_luaV_settable = (r_luaV_gettable_Def)r_lua_settable_addr;
typedef int(__cdecl *r_luaV_execute_Def)(DWORD a1, DWORD a2);
r_luaV_execute_Def r_luaV_execute = (r_luaV_execute_Def)r_luaV_execute_addr;
typedef int(__thiscall *r_luaF_openstate)(DWORD self);
r_luaF_openstate r_luaopen = (r_luaF_openstate)GlobalStates_ADDR;



void ConsoleShittyBypass()
{

}

int wrap_obj(DWORD luaState, int object, int exe)
{
	if (luaState && object)
	{
	    int *func = (int*)r_lua_newuserdata(luaState, object);
		*func = object;
		*(func + 1) = exe;
		r_lua_getfield(luaState, LUA_REGISTRYINDEX, "Object");
		r_lua_setmetatable(luaState, -2);
	}
	return 0;
}

#define runtime_check(L, c)	{ if (!(c)) break; }

#define RA(i)	(base+GETARG_A(i))
/* to be used after possible stack reallocation */
#define RB(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgR, base+GETARG_B(i))
#define RC(i)	check_exp(getCMode(GET_OPCODE(i)) == OpArgR, base+GETARG_C(i))
#define RKB(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgK, \
	ISK(GETARG_B(i)) ? k+INDEXK(GETARG_B(i)) : base+GETARG_B(i))
#define RKC(i)	check_exp(getCMode(GET_OPCODE(i)) == OpArgK, \
	ISK(GETARG_C(i)) ? k+INDEXK(GETARG_C(i)) : base+GETARG_C(i))
#define KBx(i)	check_exp(getBMode(GET_OPCODE(i)) == OpArgK, k+GETARG_Bx(i))


#define dojump(L,pc,i)	{(pc) += (i); luai_threadyield(L);}


#define Protect(x)	{ L->savedpc = pc; {x;}; base = L->base; }

BOOL r_luaV_Execute(DWORD rL, lua_State* L, int nexeccalls)
{
	LClosure *cl;
	StkId base;
	TValue *k;
	const Instruction *pc;
reentry:  /* entry point */
	pc = L->savedpc;
	cl = &clvalue(L->ci->func)->l;
	base = L->base;
	k = cl->p->k;
	/* main loop of interpreter */
//	for (;;) {
		const Instruction i = *pc++;
		StkId ra;
		ra = RA(i);
		switch (GET_OPCODE(i))
		{
		    case OP_MOVE:
			{
			    int object1 = (DWORD)KBx(i);
			    r_etobjs2s(rL, ra, (const TValue*)object1);
	//			continue;
			}
			case OP_LOADK:
			{
			    int object_ = (DWORD)KBx(i);
				r_setobj(rL, ra, (const TValue*)object_);
				return object_;
		//		continue;
			}
			case OP_LOADBOOL:
			{
				TValue* ARG_B = (TValue*)GETARG_B(i);
				r_setbvalue(ra, GETARG_B(i));
				return (int)ARG_B;
			//    continue;
			}
			case OP_LOADNIL: {
				TValue *rb = RB(i);
				do {
					setnilvalue(rb--);
				} while (rb >= ra);
			//	continue;
			}
			case OP_GETUPVAL: {
				int b = GETARG_B(i);
				setobj2s(L, ra, cl->upvals[b]->v);
		//		continue;
			}
			case OP_GETGLOBAL: {
				TValue g;
				TValue *rb = KBx(i);
				sethvalue(L, &g, cl->env);
				Protect((DWORD)r_lua_gettable(rL, (DWORD)&g, (DWORD)rb, (DWORD)ra));
		//		continue;
			}
			case OP_SETTABLE:
			{
				Protect(r_luaV_settable(rL, (DWORD)ra, (DWORD)RKB(i), (DWORD)RKC(i)));
			}
			case OP_JMP: {
				dojump(L, pc, GETARG_sBx(i));
		//		continue;
			}
			case OP_CALL:
			{
				int b = GETARG_B(i);
				int nresults = GETARG_C(i) - 1;
				if (b != 0) L->top = ra + b;  /* else previous instruction set top */
				L->savedpc = pc;
				switch (r_luaD_precall(rL, b, nresults))
				{
					case 0:
					{
					    printf("Was Lua a Call?\n");//edit of Occult
						r_luaV_execute(rL, 1);
					}
					case 1:
					{
					    printf("Was C a call?\n");
						if (nresults >= 0) {
							*(DWORD*)(rL + 16) = *(DWORD*)(*(DWORD*)(rL + 12) + 8);
						}
					} default:
					{
						return 1;
			//			continue;
					}
				}
			}
			case OP_TESTSET:
			{
				TValue *rb = RB(i);
				if (l_isfalse(rb) != GETARG_C(i)) {
					setobjs2s(L, ra, rb);
					dojump(L, pc, GETARG_sBx(*pc));
				}
				pc++;
				//continue;
			}
			case OP_CLOSURE:
			{
				Proto *p;
				Closure *ncl;
				int nup, j;
				p = cl->p->p[GETARG_Bx(i)];
				nup = p->nups;

			}
		}
	}

void Console()
{
	AllocConsole();
	SetConsoleTitle("QTX Virtual Machine");
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUTS$", "r", stdout);
}

int r_luaV_equals(lua_State *L, const TValue *t1, const TValue *t2)
{
	
}

BOOL luaA_execute(DWORD rL_, const char* source)
{
   
    lua_State* L = lua_open();
	DWORD rL = r_lua_newthread(rL_);
	if (luaL_loadbuffer(L, source, strlen(source), source) == 0)
	{
	    r_luaV_Execute(rL, L, 0);
		 return FALSE;
	}
	else
	{
		printf("Error Decompiling: %s\n", lua_tostring(L, -1));
	}
}

bool Compare(const BYTE *pData, const BYTE *bMask, const char *szMask) {
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask) return 0;
	return (*szMask) == NULL;
}

DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, const char *szMask) {
	for (int i = 0; i<(int)dwLen; i++)
		if (Compare((BYTE*)(dwAddress + (int)i), bMask, szMask))  return (int)(dwAddress + i);
	return 0;
}

int Scan(DWORD mode, const char* content, const char* mask) {
	//VMProtectBeginMutation("khsfaoifhryh24h8"); // Hot scan function
	DWORD PageSize;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	PageSize = si.dwPageSize;
	MEMORY_BASIC_INFORMATION mi;
	for (DWORD lpAddr = 0; lpAddr < 0x7FFFFFFE; lpAddr += PageSize) {
		DWORD vq = VirtualQuery((void*)lpAddr, &mi, PageSize);
		if (vq == ERROR_INVALID_PARAMETER || vq == 0) break;
		if (mi.Type == MEM_MAPPED) continue;
		if (mi.Protect == mode) {
			int addr = FindPattern(lpAddr, PageSize, (PBYTE)content, mask);
			if (addr != 0) {
		//		VMProtectEnd();
				return addr;
			}
		}
	}
	return 0;
}

HWND MainWindowHWND;
HMENU hMenu;
HMENU hMenuPopupFile;
HMENU hMenuPopupAbout;
HMODULE hInstance;
HWND ScriptTextBoxHWND;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#define MYMENU_EXIT                     (WM_APP + 101)
#define MYMENU_ABOUTMB                  (WM_APP + 102)
#define MYMENU_EXECUTECODE              (WM_APP + 103)
#define MYMENU_SCRIPTTEXTBOX            (WM_APP + 104)
#define MYMENU_CLEARSCRIPT              (WM_APP + 105)
#define MYMENU_MINIMIZE                 (WM_APP + 109)

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case MYMENU_EXIT:
			if (MessageBox(0, "Are you sure you want to close stigma?", "wtf r u doin", MB_YESNO) == IDYES)
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case MYMENU_MINIMIZE:
			ShowWindow(hwnd, SW_MINIMIZE);
			break;
		case MYMENU_ABOUTMB:
			MessageBox(hwnd, "Original script by 9/11\nProgram by [FaZe] GabeN (aka Asymmetry)", "About", MB_OK);
			break;
		case MYMENU_CLEARSCRIPT:
			SetDlgItemText(hwnd, MYMENU_SCRIPTTEXTBOX, "");
			break;
		case MYMENU_EXECUTECODE:
			int length;
			length = SendMessage(ScriptTextBoxHWND, WM_GETTEXTLENGTH, 0, 0);
			if (length == -1)
				break;
			char buff[80896]; // = 1024 * 79
			char len[255];
			_itoa_s(length, len, 10);
			GetDlgItemText(hwnd, MYMENU_SCRIPTTEXTBOX, buff, length + 1);

			luaA_execute(RL, buff);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL RegisterDLLWindowClass(char *szClassName)
{
	WNDCLASSEX wc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = szClassName;
	wc.lpfnWndProc = DLLWindowProc;
	wc.style = CS_DBLCLKS;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = "Test";
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	if (!RegisterClassEx(&wc))
		return 0;
	return 1;
}

void RefreshContextMenu(HMENU hhMenu)
{
	hMenuPopupFile = CreatePopupMenu();
	AppendMenu(hMenuPopupFile, MF_STRING, MYMENU_MINIMIZE, TEXT("Minimize"));
	AppendMenu(hMenuPopupFile, MF_STRING, MYMENU_EXIT, TEXT("Exit"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupFile, TEXT("File"));

	hMenuPopupAbout = CreatePopupMenu();
	AppendMenu(hMenuPopupAbout, MF_STRING, MYMENU_ABOUTMB, TEXT("About"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupAbout, TEXT("Help"));
}

HMENU CreateDLLWindowMenu()
{
	HMENU heyMenu;
	heyMenu = CreateMenu();

	if (heyMenu == NULL)
		return FALSE;

	RefreshContextMenu(heyMenu);
	return heyMenu;
}

void CreateFWindows()
{
	CreateWindow("BUTTON", "EXE", WS_CHILD | WS_VISIBLE, 350, 0, 45, 150, MainWindowHWND, (HMENU)MYMENU_EXECUTECODE, hInstance, NULL);
	CreateWindow("BUTTON", "CLEAR", WS_CHILD | WS_VISIBLE, 350, 100, 45, 150, MainWindowHWND, (HMENU)MYMENU_CLEARSCRIPT, hInstance, NULL);
	ScriptTextBoxHWND = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_BORDER | WS_VSCROLL, 0, 0, 350, 250, MainWindowHWND, (HMENU)MYMENU_SCRIPTTEXTBOX, hInstance, 0);
	SendMessage(ScriptTextBoxHWND, EM_SETLIMITTEXT, 0x7FFFFFFE, 0);
}

void ShowForm()
{
	hInstance = GetModuleHandle(NULL);
	hMenu = CreateDLLWindowMenu();
	RegisterDLLWindowClass("DLLWindowClass");
	MainWindowHWND = CreateWindowEx(WS_EX_TOPMOST, "DLLWindowClass", "QTX", WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, hMenu, hInstance, NULL);
	CreateFWindows();
	ShowWindow(MainWindowHWND, SW_SHOWNORMAL);

	MSG messages;
	while (GetMessage(&messages, NULL, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
}


void QTXVMEntry()
{
	// code here.
	try {
	MessageBoxA(0, "Scanning... ", "", 0);
	int SCVFTABLE = ScriptContextAddress;
    int SC = Scan(PAGE_READWRITE, (char*)&SCVFTABLE, "xxxx");
	RL = r_luaopen(SC);
	MessageBoxA(0, "OK", "", 0);
	}
	catch (std::exception_ptr e)
	{

	}
	ShowForm();
}

int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)QTXVMEntry, 0, 0, 0);
	}
	return TRUE;
}
