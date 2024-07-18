#pragma once
#include <Windows.h>
#include "main.h"

LPCSTR g_szClassName = "MainWindow_Class";
HINSTANCE hInstance2 = NULL;
HWND hwnd;
HWND CommandBar;
HWND ConsoleField;

//#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CTLCOLORSTATIC:
	{
		HDC hEdit = (HDC)wParam;
		SetTextColor(hEdit, RGB(255, 75, 0)); // text color of console
		SetBkColor(hEdit, RGB(255, 255, 255)); // dont fuck with this, looks ugly on anything else but white
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case (WM_APP + 502):
			if (HIWORD(wParam) == EN_MAXTEXT) {
				char cText[500];
				SendMessage((HWND)lParam, WM_GETTEXT, 500, (LPARAM)cText);

				if (strcmp(cText, "") == 0)
					break;

				SendMessage((HWND)lParam, WM_SETTEXT, NULL, (LPARAM)"");

				RunCommand(cText);
			}
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
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
			// handle the error and possibly exit
			//return msg.wParam;
			return 0;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}

void ConsoleOutput(const char * format, ...) {
	char message[500];
	va_list vl;
	va_start(vl, format);
	vsnprintf_s(message, 500, format, vl);
	va_end(vl);

	SendMessage(ConsoleField, EM_SETSEL, -1, 0);
	SendMessage(ConsoleField, EM_REPLACESEL, FALSE, (LPARAM)message);
}

BOOL CreateThisWindow()
{
	WNDCLASSEX wc;
	MSG msg;
	BOOL bRet;

	hInstance2 = GetModuleHandle(NULL);

	ZeroMemory(&wc, sizeof(&wc));

	wc.style = 0;
	wc.lpszClassName = g_szClassName;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIconSm = LoadIcon(NULL, IDC_ICON);
	wc.hIcon = LoadIcon(NULL, IDC_ICON);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;



	if (!RegisterClassEx(&wc))
	{
		LPCSTR Error01 = "Could not register class!";
		LPCSTR Error01_Caption = "Error";
		MessageBox(NULL, Error01, Error01_Caption, MB_OK | MB_ICONERROR);
	}

	LPCSTR WindowTitle = "Aex Alpha V1";

	hwnd = CreateWindowEx(WS_EX_TOPMOST, g_szClassName, WindowTitle, NULL, CW_USEDEFAULT, CW_USEDEFAULT, 512, 309, 0, 0, hInstance2, 0);
	CommandBar = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VISIBLE, 12, 210, 472, 20, hwnd, (HMENU)(WM_APP + 502), hInstance2, 0);
	
	ConsoleField = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL, 12, 12, 472, 157, hwnd, (HMENU)0, hInstance2, 0);


	RECT rc; GetWindowRect(hwnd, &rc); int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2; int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2; SetWindowPos(hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	SendMessage(CommandBar, EM_SETLIMITTEXT, 500, NULL); //500 char limit
	HFONT textFont = CreateFont(18, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
	SendMessage(CommandBar, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(ConsoleField, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));

	if (hwnd == NULL)
	{
		LPCSTR Error02 = "Could not create Window!";
		LPCSTR Error02_Caption = "Error";
		MessageBox(NULL, Error02, Error02_Caption, MB_OK | MB_ICONERROR);
	}

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);


	/*
	while (GetMessage(&msg, 0, 0, 0))
	{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}
	*/

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == 0) {
			return FALSE;
		}
		else if (bRet == -1)
		{
			// handle the error and possibly exit
			//return msg.wParam;
			return FALSE;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//return msg.wParam;
	//StartMessageLoop();

	return TRUE;
	//	return StartMessageLoop();
}