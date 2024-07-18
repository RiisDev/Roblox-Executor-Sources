#include "form.h"
#include <iostream>

HWND ParentWindow = NULL;
HWND MainWindow = NULL;
HMENU WindowMenu = NULL;
HMODULE HInstance = NULL;

HWND InputField = NULL;
HWND ConsoleField = NULL;
HWND WaypointBox = NULL;

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CTLCOLORSTATIC:
	{
							  HDC hEdit = (HDC)wParam;
							  SetTextColor(hEdit, RGB(0, 0, 0));
							  SetBkColor(hEdit, RGB(255, 255, 255));
							  return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MALX_EXIT:
			if (MessageBox(hwnd, "Are you sure you want to exit?", ":(", MB_YESNOCANCEL) == IDYES)
				ExitThread(0);
			break;
		case MALX_RESTART:
			if (MessageBox(hwnd, "Are you sure you want to restart?", ":(", MB_YESNOCANCEL) == IDYES)
				ExitThread(ALXRESTART);
			break;

		case MALX_CREDITS:
			MessageBox(hwnd, "                                  Special thanks to:\n\nChirality (Asymmetry) - Teaching, sharing knowledge.\n(alx wouldn't be possible without him)\n\nLordAbolition - Helped test Alx.", "Credits", MB_OKCANCEL);
			break;

		case MALX_COMMANDS:
			HandleCommand("cmds");
			break;
		case ALX_INPUT_FIELD:
			if (HIWORD(wParam) == EN_MAXTEXT) {
				char cText[INPUT_CHAR_LIMIT];
				SendMessage((HWND)lParam, WM_GETTEXT, INPUT_CHAR_LIMIT, (LPARAM)cText);

				if (strcmp(cText, "") == 0)
					break;

				SendMessage((HWND)lParam, WM_SETTEXT, NULL, (LPARAM)"");

				//std::string command = cText;
				HandleCommand(cText);
			}

			break;
		}
		break;
	case WM_DESTROY:
		ExitThread(0);
		break;

	case WM_QUIT:
		ExitThread(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL RegisterWindowClass(const char* wClassName) {
	WNDCLASSEX nClass;

	nClass.cbSize = sizeof(WNDCLASSEX);
	nClass.style = CS_DBLCLKS;
	nClass.lpfnWndProc = DLLWindowProc;
	nClass.cbClsExtra = 0;
	nClass.cbWndExtra = 0;
	nClass.hInstance = GetModuleHandle(NULL);
	nClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // TODO: make an icon for alx?
	nClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	nClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	nClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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

BOOL CreateSubwindows() {
	//HINSTANCE hInstance = GetModuleHandle(NULL);
	ConsoleField = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL, 10, 20, 520, 200, MainWindow, (HMENU)ALX_CONSOLE_WINDOW, HInstance, 0);
	HWND consoleFieldLabel = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE, 10, 0, 100, 20, MainWindow, NULL, HInstance, NULL);
	InputField = CreateWindowEx(NULL, "EDIT", "", WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VISIBLE, 10, 250, 500, 20, MainWindow, (HMENU)ALX_INPUT_FIELD, HInstance, 0);
	HWND inputFieldLabel = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE, 10, 230, 100, 20, MainWindow, NULL, HInstance, NULL);
	SendMessage(inputFieldLabel, WM_SETTEXT, NULL, (LPARAM)"Input");
	SendMessage(consoleFieldLabel, WM_SETTEXT, NULL, (LPARAM)"Console");
	SendMessage(InputField, EM_SETLIMITTEXT, INPUT_CHAR_LIMIT, NULL);

	//WaypointBox = CreateWindowEx(NULL, "LISTBOX", "", WS_CHILD | WS_BORDER | WS_VSCROLL | WS_VISIBLE, 10, 280, 520, 100, MainWindow, (HMENU)ALX_WAYPOINT_BOX, HInstance, 0);
	//SendMessage(WaypointBox, LVM_SETITEMTEXT, )

	HFONT textFont = CreateFont(18, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));

	SendMessage(inputFieldLabel, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(consoleFieldLabel, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(ConsoleField, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(InputField, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));

	UpdateWindow(MainWindow);

	return 1;
}

BOOL CreateWindowMenu() {
	WindowMenu = CreateMenu();
	if (!WindowMenu)
		return 0;

	HMENU mainDropdown = CreatePopupMenu();
	AppendMenu(mainDropdown, MF_STRING, MALX_EXIT, "Exit");
	AppendMenu(mainDropdown, MF_STRING, MALX_RESTART, "Restart");

	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)mainDropdown, "Alx");



	HMENU aboutDropdown = CreatePopupMenu();
	AppendMenu(aboutDropdown, MF_STRING, MALX_CREDITS, "Credits");
	AppendMenu(aboutDropdown, MF_STRING, MALX_ABOUT, "About");
	AppendMenu(aboutDropdown, MF_STRING, MALX_COMMANDS, "Commands");

	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)aboutDropdown, "Help");

	return 1;
}

BOOL InitiateWindow() {
	HInstance = GetModuleHandle(NULL);

	UnregisterClass("ALX_WINDOW", HInstance);
	RegisterWindowClass("ALX_WINDOW");

	char alxName[50];

	//ParentWindow = FindWindow(NULL, "ROBLOX");
	if (!CreateWindowMenu())
		return 0;

	if (!(MainWindow = CreateWindowEx(
		NULL,
		"ALX_WINDOW",
		alxName,
		WS_SYSMENU | WS_MINIMIZEBOX,
		100,
		100,
		560,
		335,
		NULL, //ParentWindow,
		WindowMenu,
		HInstance,
		NULL))) return 0;

	//ScrollWindowEx(MainWindow, 0, 560, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_SMOOTHSCROLL);
	//EnableScrollBar(MainWindow, SB_VERT, ESB_ENABLE_BOTH);

	CreateSubwindows();
	UpdateWindow(MainWindow);

	ShowWindow(MainWindow, SW_SHOWNORMAL);
	main();

	return StartMessageLoop();
}

void ConsoleOutput(const char * format, ...) {
	char message[INPUT_CHAR_LIMIT];
	va_list vl;
	va_start(vl, format);
	va_end(vl);

	SendMessage(ConsoleField, EM_SETSEL, -1, 0);
	SendMessage(ConsoleField, EM_REPLACESEL, FALSE, (LPARAM)message);
}

