#include "UI.h"
#include "MainLua.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <fstream>
#include <sstream>
#include <Richedit.h>
#include <io.h>
#include <WinInet.h>

#define UI_SIZE_WIDTH			395 //460
#define UI_SIZE_HEIGHT			297 //330

/*
Gui
*/

bool Gui::created = false;
HWND script_wnd, txt_script, txt_script2, btn_exec_script, btn_clear, hConsole, hScriptWnd;
WNDPROC oldbtn, oldtxt;
#define IDM_SCWND 900
#define IDM_EXIT 901

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) //main wndproc
{
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:
			if (MessageBox(0, "If you close Seven, roblox must exit as well. Continue?", "Confirmation", MB_YESNO) == IDYES)
				exit(0);
			break;
		case IDM_SCWND:
			ShowWindow(script_wnd, true);
			return 0;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		ShowWindow(hWnd, SW_FORCEMINIMIZE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ScriptWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		ShowWindow(script_wnd, 0);
		return 1;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZING:
		RECT *rect = (RECT*)lParam;
		SetWindowPos(txt_script, NULL, 0, 0, rect->right - rect->left - 16, rect->bottom - rect->top - 66, NULL);
		SetWindowPos(btn_exec_script, NULL, 1, rect->bottom - rect->top - 65, 110, 25, NULL);
		SetWindowPos(btn_clear, NULL, rect->right - rect->left - 127, rect->bottom - rect->top - 65, 110, 25, NULL);
		return TRUE;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK btn_exec_script_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN)
	{
		int len = SendMessage(txt_script, WM_GETTEXTLENGTH, 0, 0);
		char *hdr = "\n"; //set the 'script' global
		int hdr_len = strlen(hdr);
		char *buf = new char[len + hdr_len + 1]();
		memcpy(buf, hdr, hdr_len);
		SendMessage(txt_script, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)(buf + hdr_len));
//		LuaExecuteString2(buf);
		delete[] buf;
	}
	return CallWindowProc(oldbtn, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK btn_clear_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN)
		SendMessage(txt_script, WM_SETTEXT, NULL, (LPARAM)"");
	return CallWindowProc(oldbtn, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK txt_script_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
		if (GetKeyState(VK_CONTROL) & 0x8000 && wParam == 'A')
			SendMessage(hWnd, EM_SETSEL, 0, -1);
	return CallWindowProc(oldtxt, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK btn_exec_script_proc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN)
	{
		int len = SendMessage(txt_script2, WM_GETTEXTLENGTH, 0, 0);
		char *hdr = "\n"; //set the 'script' global
		int hdr_len = strlen(hdr);
		char *buf = new char[len + hdr_len + 1]();
		memcpy(buf, hdr, hdr_len);
		SendMessage(txt_script2, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)(buf + hdr_len));
//		LuaExecuteString2(buf);
		delete[] buf;
	}
	return CallWindowProc(oldbtn, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK btn_clear_proc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN)
		SendMessage(txt_script2, WM_SETTEXT, NULL, (LPARAM)"");
	return CallWindowProc(oldbtn, hWnd, message, wParam, lParam);
}

LRESULT CALLBACK txt_script_proc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
		if (GetKeyState(VK_CONTROL) & 0x8000 && wParam == 'A')
			SendMessage(hWnd, EM_SETSEL, 0, -1);
	return CallWindowProc(oldtxt, hWnd, message, wParam, lParam);
}

void Gui::WriteEx(bool bold, DWORD color, char *format, ...)
{
	va_list va;
	va_start(va, format);
	char *buffer;
	if (vasprintf(&buffer, format, va) == -1)
		return;
	va_end(va);

	GETTEXTLENGTHEX gte;
	memset(&gte, 0, sizeof(SETTEXTEX));
	gte.flags = GTL_DEFAULT;
	gte.codepage = CP_ACP;
	int len = SendMessage(hConsole, EM_GETTEXTLENGTHEX, (WPARAM)&gte, 0);
	int buflen = strlen(buffer);

	SendMessage(hConsole, EM_SETSEL, -1, -1);

	SETTEXTEX ste;
	memset(&ste, 0, sizeof(SETTEXTEX));
	ste.flags = ST_NEWCHARS | ST_SELECTION;
	ste.codepage = CP_ACP;
	SendMessage(hConsole, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)buffer);

	SendMessage(hConsole, EM_SETSEL, len, len + buflen);

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
	SendMessage(hConsole, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

	SendMessage(hConsole, EM_SETSEL, -1, -1);
	SendMessage(hConsole, WM_VSCROLL, SB_BOTTOM, 0);
	free(buffer);
}

void Gui::Write(char *format, ...)
{
	va_list va;
	va_start(va, format);
	char *buffer;
	if (vasprintf(&buffer, format, va) == -1)
		return;
	va_end(va);

	SendMessage(hConsole, EM_SETSEL, -1, -1);

	SETTEXTEX ste;
	memset(&ste, 0, sizeof(SETTEXTEX));
	ste.flags = ST_NEWCHARS | ST_SELECTION;
	ste.codepage = CP_ACP;
	SendMessage(hConsole, EM_SETTEXTEX, (WPARAM)&ste, (LPARAM)buffer);

	SendMessage(hConsole, EM_SETSEL, -1, -1);
	SendMessage(hConsole, WM_VSCROLL, SB_BOTTOM, 0);
	free(buffer);
}

Gui::Gui()
{
	LoadLibrary("msftedit.dll");
	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Make the Large Script Window
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ScriptWndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "kLBi2xP0o4czr7ckuzyQ";
	RegisterClassEx(&wc);

	HFONT font = CreateFont(14.73, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Helvetica");

	script_wnd = CreateWindowEx(WS_EX_TOPMOST, "kLBi2xP0o4czr7ckuzyQ", "Beta Script Exeuction", WS_OVERLAPPEDWINDOW, 0, 0, 400, 386, NULL, NULL, hInstance, NULL);
	txt_script = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 0, 384, 320, script_wnd, NULL, hInstance, NULL);
	btn_exec_script = CreateWindowEx(NULL, "BUTTON", "Execute Script", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 1, 321, 110, 25, script_wnd, NULL, hInstance, NULL);
	btn_clear = CreateWindowEx(NULL, "BUTTON", "Clear", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 273, 321, 110, 25, script_wnd, NULL, hInstance, NULL);

	SendMessage(btn_exec_script, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(btn_clear, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(txt_script, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(txt_script, EM_LIMITTEXT, 0x7FFFFFFE, 0);
	oldbtn = (WNDPROC)SetWindowLongPtr(btn_exec_script, GWLP_WNDPROC, (LONG)btn_exec_script_proc);
	oldtxt = (WNDPROC)SetWindowLongPtr(txt_script, GWLP_WNDPROC, (LONG)txt_script_proc);
	SetWindowLongPtr(btn_clear, GWLP_WNDPROC, (LONG)btn_clear_proc);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Make the main window

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "kLBi2xP0o4czr7ckuzyF";
	RegisterClassEx(&wc);

	HMENU main_menu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hViewMenu = CreateMenu();

	AppendMenu(main_menu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	AppendMenu(main_menu, MF_POPUP, (UINT_PTR)hViewMenu, "View");
	AppendMenu(hViewMenu, MF_STRING, IDM_SCWND, "Large Script Window");
	AppendMenu(hFileMenu, MF_STRING, IDM_EXIT, "Exit");

	HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, "kLBi2xP0o4czr7ckuzyF", "Halcyon v6.0.7", WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX, 0, 0, 395, 297, NULL, main_menu, hInstance, NULL);
	txt_script2 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VISIBLE, 10, 216, UI_SIZE_WIDTH - 26, 25, hWnd, NULL, hInstance, NULL);
	hConsole = CreateWindowExW(0, MSFTEDIT_CLASS, L"", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL, 0, 0, UI_SIZE_WIDTH - 5, 211, hWnd, NULL, hInstance, NULL);
	//HWND btn_exec_script2 = CreateWindowEx(NULL, "BUTTON", "Execute Script", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 5, 365, 110, 25, hWnd, NULL, hInstance, NULL);
	//HWND btn_clear2 = CreateWindowEx(NULL, "BUTTON", "Clear", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 318, 365, 110, 25, hWnd, NULL, hInstance, NULL);

	SendMessage(hConsole, WM_SETFONT, (WPARAM)font, TRUE);
	SendMessage(hConsole, EM_SETBKGNDCOLOR, 0, RGB(212, 212, 212));
	SendMessage(txt_script2, WM_SETFONT, (WPARAM)font, TRUE);
	//SendMessage(btn_exec_script2, WM_SETFONT, (WPARAM)font, TRUE);
	//SendMessage(btn_clear2, WM_SETFONT, (WPARAM)font, TRUE);
	//SetWindowLongPtr(btn_exec_script2, GWLP_WNDPROC, (LONG)btn_exec_script_proc2);
	//SetWindowLongPtr(btn_clear2, GWLP_WNDPROC, (LONG)btn_clear_proc2);
	SetWindowLongPtr(txt_script2, GWLP_WNDPROC, (LONG)txt_script_proc2);
	SendMessage(txt_script2, EM_LIMITTEXT, 0x7FFFFFFE, 0);

	ShowWindow(hWnd, true);
	created = true;
}