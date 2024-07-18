#include <Windows.h>
#include "Serializer.h"
#include "RBX.hpp"
#include "WinMem.h"
#include <string>
#include <iostream>

#include <windows.h>
#include <commctrl.h>
#include <Richedit.h>
#include <stdio.h>
#include <vector>
#include <map>

// Yay! Visual Styles :)
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#pragma comment(lib, "Comctl32.lib")
#define SetFont(HWND, FONT) SendMessage(HWND, WM_SETFONT, (WPARAM)FONT, TRUE)
#define EXECUTE_BUTTON 101
#define CLEAR_BUTTON   102

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
HFONT hDefaultFont = CreateFont(14, 0, 0, 0, 400, FALSE, FALSE, FALSE, 1, 400, 0, 0, 0, ("Ms Shell Dlg"));
static HWND TabControl;
static HWND Output;
static HWND ExecuteBox;
static std::map<int, std::vector<HWND>> TabItems;

HWND LOMainWnd;
WNDPROC RobloxWndProcOld;

LRESULT CALLBACK RobloxWndProcHook(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_INPUT)
	{
		if (GetForegroundWindow() == LOMainWnd)
		{
			UINT sz = sizeof(RAWINPUTHEADER);
			RAWINPUTHEADER hdr;
			memset(&hdr, 0, sz);

			GetRawInputData((HRAWINPUT)lParam, RID_HEADER, (LPVOID)(&hdr), &sz, sizeof(RAWINPUTHEADER));

			if (hdr.dwType == RIM_TYPEKEYBOARD)
				return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
	return CallWindowProc(RobloxWndProcOld, hwnd, message, wParam, lParam);
}

void HookRobloxWndProc()
{
	HWND RobloxWindow = FindWindowW(NULL, L"ROBLOX");
	if (RobloxWindow) {
		RobloxWndProcOld = (WNDPROC)GetWindowLongPtr(RobloxWindow, GWLP_WNDPROC);
		SetWindowLongPtr(RobloxWindow, GWLP_WNDPROC, (LONG)RobloxWndProcHook);
	}
}

void AppendText(char* text) {
	int length = GetWindowTextLength(Output);
	SendMessageA(Output, EM_SETSEL, (WPARAM)length, (LPARAM)length);
	SendMessageA(Output, EM_REPLACESEL, 0, (LPARAM)text);
}

void SetColor(HWND hwnd, COLORREF Color) {
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = Color;
	SendMessage(hwnd, EM_SETCHARFORMAT, (LPARAM)SCF_SELECTION, (LPARAM)& cf);
}

void OutputText(char* format, ...) {
	va_list args;
	va_start(args, format);
	char buf[1024];
	vsnprintf_s(buf, sizeof(buf), format, args);
	SetColor(Output, RGB(0, 0, 0));
	AppendText(buf);
	va_end(args);
}

void OutputText(COLORREF Color, const char* format, ...) {
	va_list args;
	va_start(args, format);
	char buf[1024];
	vsnprintf(buf, sizeof(buf), format, args);
	SetColor(Output, Color);
	AppendText(buf);
	va_end(args);
}

void CreateTab(HWND TbCtrl, const char* Text, int Index) {
	TCITEM tci;
	ZeroMemory(&tci, sizeof(TCITEM));
	tci.mask = TCIF_TEXT;
	tci.pszText = (LPSTR)Text;
	TabCtrl_InsertItem(TbCtrl, Index, &tci);
	TabItems[Index] = {};
}

void AddToTab(HWND Item, int Index) {
	if (TabItems.find(Index) != TabItems.end()) {
		TabItems[Index].push_back(Item);
		if (Index != 0) {
			ShowWindow(Item, SW_HIDE);
		}
	}
}

uintptr_t rL;
lua_State* L;
uint32_t encodeKey;
uint32_t decodeKey;
uintptr_t* deserializerFlag;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case EXECUTE_BUTTON: {
			int Length = GetWindowTextLength(ExecuteBox) + 1;
			char* buf = new char[Length + 1]();
			GetWindowTextA(ExecuteBox, buf, Length);
			if (luaL_loadstring(L, buf))
			{
				OutputText(RGB(255, 0, 0), lua_tostring(L, -1));
				lua_pop(L, 1);
				break;
			}
			RbxOpEncoder Encoder;
			std::string Bytecode = LuaSerializer::serialize(L, buf, Encoder, encodeKey, decodeKey);
			OutputText(RGB(0, 0, 0), "Converting lua proto....\n");
			const int Result = RBX::Deserialize(rL, Bytecode, "@LunarV4", 1);
			int hash = LuaSerializer::returnHash(L, encodeKey);
			OutputText(RGB(0, 0, 0), "L: 0x%x\n", L);
			OutputText(RGB(0, 0, 0), "rL: 0x%x\n", rL);
			OutputText(RGB(0, 0, 0), "encode key: 0x%x\n", encodeKey);
			OutputText(RGB(0, 0, 0), "decode key: 0x%x\n", decodeKey);
			OutputText(RGB(0, 0, 0), "rp->hash: 0x%x\n", hash);
			OutputText(RGB(0, 0, 0), "p: 0x%x\n", ok);
			OutputText(RGB(0, 0, 0), "p->nups: 0x%x\n", ok->nups);
			OutputText(RGB(0, 0, 0), "p->numparams: 0x%x\n", ok->numparams);
			OutputText(RGB(0, 0, 0), "p->source: %s\n", ok->source);
			OutputText(RGB(0, 0, 0), "p->sizelineinfo: 0x%x\n", ok->sizelineinfo);
			OutputText(RGB(0, 0, 0), "p->sizep: 0x%x\n", ok->sizep);
			OutputText(RGB(0, 0, 0), "p->sizek: 0x%x\n", ok->sizek);
			OutputText(RGB(0, 0, 0), "p->maxstacksize: 0x%x\n", ok->maxstacksize);
			OutputText(RGB(0, 0, 0), "Done pushing function.\n");
			*deserializerFlag &= ~64;

			if (*(BYTE*)(rL + *(DWORD*)(rL + 16) + 16))
			{
				OutputText(RGB(255, 0, 0), "Game is luaU.\n");

				/*
				
				SNIP
				
			
				*/

				//RBX::execute_script(rL, Bytecode);
				/*
				
				R_lua_pushLClosure(rL, lua_topointer(L, -1);
				*/
				RBX::Spawn(rL);
				break;
			}
			RBX::Spawn(rL);
	
			OutputText(RGB(0, 0, 0), "Calling spawn\n");
			Bytecode.clear();
			break;
		}
		case CLEAR_BUTTON: {
			SetWindowText(ExecuteBox, "");
			break;
		}
		}
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case TCN_SELCHANGING:	return FALSE;
		case TCN_SELCHANGE: {
			int Index = TabCtrl_GetCurSel(TabControl);
			for (auto I : TabItems) {
				bool IsIndex = (I.first == Index);
				for (size_t iitem = 0; iitem < I.second.size(); iitem++) {
					if (IsIndex) {
						ShowWindow(I.second[iitem], SW_SHOW);
					}
					else {
						ShowWindow(I.second[iitem], SW_HIDE);
					}
				}
			}
			break;
		}

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

void CreateUI() {

	HINSTANCE hInst = GetModuleHandle(NULL);

	if (!LoadLibrary(TEXT("Riched32.dll"))) {
		MessageBox(NULL, "Failed to load in Msftedit.dll", "Error", MB_ICONERROR | MB_OK);
		std::abort();
		return;
	}

	MSG messages;
	WNDCLASSEX wincl;


	wincl.hInstance = hInst;
	wincl.lpszClassName = "LunarV4";
	wincl.lpfnWndProc = WindowProcedure;
	wincl.style = CS_HREDRAW | CS_VREDRAW;
	wincl.cbSize = sizeof(WNDCLASSEX);


	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;

	wincl.hbrBackground = (HBRUSH)(COLOR_WINDOW);


	if (!RegisterClassEx(&wincl))
		return;


	LOMainWnd = CreateWindowEx(
		NULL,
		"LunarV4",
		"Lunar V4 | By KingEzz/LordEzz7",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | MB_TOPMOST,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		628,
		 360,
		HWND_DESKTOP,
		NULL,
		hInst,
		NULL
	);

	INITCOMMONCONTROLSEX icce;

	icce.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icce.dwICC = ICC_TAB_CLASSES;
	InitCommonControlsEx(&icce);

	TabControl = CreateWindowEx(NULL, WC_TABCONTROL, "", WS_CHILD | WS_VISIBLE, 5, 5, 614, 322, LOMainWnd, NULL, hInst, NULL);
	if (!TabControl) {
		MessageBox(LOMainWnd, "Failed to create Tab Control", "Error", MB_ICONERROR | MB_OK);
		std::abort();
		return;
	}

	SetFont(TabControl, hDefaultFont);
	CreateTab(TabControl, "Output", 0);
	CreateTab(TabControl, "Execute", 1);
	CreateTab(TabControl, "From File", 2);
	Output = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | ES_DISABLENOSCROLL, 10, 30, 600, 290, LOMainWnd, NULL, hInst, NULL);
	ExecuteBox = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, "", WS_CHILD | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_DISABLENOSCROLL, 10, 30, 600, 260, LOMainWnd, NULL, hInst, NULL);

	HWND ExecuteButton = CreateWindowEx(NULL, "BUTTON", "Execute", WS_TABSTOP | WS_CHILD, 520, 293, 90, 25, LOMainWnd, (HMENU)EXECUTE_BUTTON, hInst, NULL);
	HWND ClearButton = CreateWindowEx(NULL, "BUTTON", "Clear", WS_TABSTOP | WS_CHILD, 10, 293, 90, 25, LOMainWnd, (HMENU)CLEAR_BUTTON, hInst, NULL);

	SetFont(Output, hDefaultFont);
	SetFont(ExecuteBox, hDefaultFont);
	SetFont(ExecuteButton, hDefaultFont);
	SetFont(ClearButton, hDefaultFont);

	AddToTab(Output, 0);
	AddToTab(ExecuteBox, 1);
	AddToTab(ExecuteButton, 1);
	AddToTab(ClearButton, 1);

	ShowWindow(LOMainWnd, SW_SHOW);
	UpdateWindow(LOMainWnd);
	SetWindowPos(LOMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	HookRobloxWndProc();

	
	OutputText(RGB(0, 0, 255), "ready!\n By KingEzz\n");

	while (GetMessage(&messages, NULL, 0, 0)) {
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

}

void* placeHook(DWORD address, void* hook, bool revert = false) {
	DWORD oldprot;
	if (!revert) {
		void* oldmem = new void*;
		void* result = new void*;
		memcpy(oldmem, (void*)address, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		*(char*)address = 0xE9; *(DWORD*)(address + 1) = (DWORD)hook - address - 5;
		memcpy(result, oldmem, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
		return result;
	}
	else {
		VirtualProtect((LPVOID)address, 1, PAGE_EXECUTE_READWRITE, &oldprot);
		memcpy((void*)address, hook, sizeof(void*) * 4);
		VirtualProtect((LPVOID)address, 1, oldprot, &oldprot);
		return NULL;
	}
}

int GetTopHook(uintptr_t Thread)
{
	rL = Thread;
	return (*(DWORD*)(Thread + 12) - *(DWORD*)(Thread + 28)) >> 4;
}


VOID Main()
{
	Sleep(1000);
	L = luaL_newstate();
	void* Ret = placeHook(ASLR(0x82AA00), GetTopHook);
	do { Sleep(1); } while (rL == 0);
	placeHook(ASLR(0x82AA00), Ret, true);

	decodeKey = (rL + 52 + *(DWORD*)(rL + 16)) ^ *(DWORD*)(rL + 52 + *(DWORD*)(rL + 16)); /*

																								   /* (a1 + 52 + *(_DWORD *)(a1 + 12)) ^ *(_DWORD *)(a1 + 52 + *(_DWORD *)(a1 + 12))
																								   Look up "local" string press F5

																								   v10 = sub_82BD20(v7, v14, v6, (a1 + 44 + *(_DWORD *)(a1 + 12)) ^ *(_DWORD *)(a1 + 44 + *(_DWORD *)(a1 + 12)), 0);

																								   *(_DWORD *)(a1 + 12)) ^ *(_DWORD *)(a1 + 44 + *(_DWORD *)(a1 + 12)) is the decodeKey

																								   */
	encodeKey = modInverse(decodeKey, 4294967296);
	deserializerFlag = reinterpret_cast<uint32_t*>(*reinterpret_cast<uintptr_t*>(WinMem::scan(GetModuleHandleA(nullptr), "83 0D ? ? ? ? 08 8B 4D F4", 0, false) + 2));
	int script[] = { 0, 0 };
	RBX::SandboxThread(rL, 6, (int)script);
	CreateUI();
}

BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, LPVOID Reserved)
{
	DisableThreadLibraryCalls(Module);
	if (Reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)Main, nullptr, NULL, nullptr);
	}
	return TRUE;
}