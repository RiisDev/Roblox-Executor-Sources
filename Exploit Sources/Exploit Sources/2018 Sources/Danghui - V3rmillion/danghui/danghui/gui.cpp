
#include "gui.hpp"

HWND MainScriptWindow;
HWND MainScriptTextBox, MainScriptExecute;
WNDPROC OldBtn, OldTxt;

LRESULT CALLBACK ScriptWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
			ShowWindow(MainScriptWindow, 0);
			return 1;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK ExecuteWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONUP)
	{
		int len = SendMessage(MainScriptTextBox, WM_GETTEXTLENGTH, 0, 0);
		char *hdr = "script = Instance.new(\"LocalScript\")\n";
		int hdr_len = strlen(hdr);
		char *buf = new char[len + hdr_len + 1]();
		memcpy(buf, hdr, hdr_len);
		SendMessage(MainScriptTextBox, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)(buf + hdr_len));
		danghui_Aexecute(buf);
		delete[] buf;
	}
	return CallWindowProc(OldBtn, hWnd, message, wParam, lParam);
}

VOID OpenGui()
{
	LoadLibrary("msftedit.dll");
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = ScriptWndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "DanghuiTemporaryWindow";
	RegisterClassEx(&wc);
	MainScriptWindow = CreateWindowEx(WS_EX_TOPMOST, "DanghuiTemporaryWindow", "Danghui Temporary Window (better one will come soon)", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, 395, 405, NULL, NULL, hInstance, NULL);
	MainScriptTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, 0, 0, 384, 350, MainScriptWindow, NULL, hInstance, NULL);
	MainScriptExecute = CreateWindowEx(NULL, "BUTTON", "Execute", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 1, 350, 382, 25, MainScriptWindow, NULL, hInstance, NULL);
	SendMessage(MainScriptTextBox, EM_LIMITTEXT, 0x7FFFFFFE, 0);
	OldBtn = (WNDPROC)SetWindowLongPtr(MainScriptExecute, GWLP_WNDPROC, (LONG)ExecuteWndProc);
	ShowWindow(MainScriptWindow, SW_SHOW);
}