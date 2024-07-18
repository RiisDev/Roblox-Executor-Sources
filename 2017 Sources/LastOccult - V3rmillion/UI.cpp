#include "stdafx.h"
#include "OccultLua/OccultVM.h"
#include <iostream>
#include <windows.h>
#include <commctrl.h>
#include <Richedit.h>
#include <stdio.h>
#include <vector>
#include <map>

// Yay! Visual Styles :)
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")



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
	SendMessage(hwnd, EM_SETCHARFORMAT, (LPARAM)SCF_SELECTION, (LPARAM)&cf);
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

void CreateTab(HWND TbCtrl, char* Text, int Index) {
	TCITEM tci;
	ZeroMemory(&tci, sizeof(TCITEM));
	tci.mask = TCIF_TEXT;
	tci.pszText = Text;
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

void CreateUI(FDef OnLoad) {

	HINSTANCE hInst = GetModuleHandle(NULL);

	if (!LoadLibrary(TEXT("Riched32.dll"))) {
		MessageBox(NULL, "Failed to load in Msftedit.dll", "Error", MB_ICONERROR | MB_OK);
		std::abort();
		return;
	}

	MSG messages;
	WNDCLASSEX wincl;


	wincl.hInstance = hInst;
	wincl.lpszClassName = "LastOccult";
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
		"LastOccult",
		"Last-Occult | Script Executor",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
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

	HWND ExecuteButton = CreateWindowEx(NULL, "BUTTON", "Execute Script", WS_TABSTOP | WS_CHILD, 520, 293, 90, 25, LOMainWnd, (HMENU)EXECUTE_BUTTON, hInst, NULL);
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

	HookRobloxWndProc();

	if (OnLoad) {
		(*OnLoad)();
	}

	while (GetMessage(&messages, NULL, 0, 0)) {
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}

}


struct args {
	DWORD rL;
	char* buf;
	std::function<void()> fat;
};

void threadstuf(void* asd) {
	__try {
		args* fat = (args*)asd;
		Occult_luaA_run(fat->rL, fat->buf);
		fat->fat();
	}
	__except (EXCEPTION_EXECUTE_FAULT) {
		OutputText(RGB(255, 0, 0), "An error occured during execution\n");
	}
}

void execscript() {
	extern DWORD RL;
	int Length = GetWindowTextLength(ExecuteBox) + 1;
	char* buf = new char[Length + 1]();
	GetWindowTextA(ExecuteBox, buf, Length);
	args* asd = new args{ RL, buf };
	asd->fat = [&]() {
		delete[] buf;
	};
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)threadstuf, (LPVOID)asd, NULL, NULL);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case EXECUTE_BUTTON: {
			execscript();
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