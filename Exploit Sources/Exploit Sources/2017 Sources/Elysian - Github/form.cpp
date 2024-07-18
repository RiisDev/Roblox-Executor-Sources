#include "form.h"
#include "elysian.h"
#include "util.h"
#include "alua.h"

#include <Richedit.h>
#include <CommCtrl.h>

#pragma comment(lib, "Comctl32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' \
					   name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
					   processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HFONT gMainFont = CreateFontA(15, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Courier New"));

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CTLCOLORSTATIC: {
		SetBkColor((HDC)wParam, RGB(255, 255, 255));
		return (LRESULT)GetStockObject(WHITE_BRUSH);
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case EUI_SHOWCONSOLE:
			form->ToggleConsole();
			break;
		case EUI_INITCMDTHREAD:
			form->StartConsoleThread();
			break;
		case EUI_EXECUTE:
		{
			int length = SendMessage((HWND)form->GetScriptTextbox(), WM_GETTEXTLENGTH, NULL, NULL);
			char* code = new char[length];
			if (!code) {
				form->print(RGB(255, 0, 0), "ERROR: allocation error");
				break;
			}

			SendMessage((HWND)form->GetScriptTextbox(), WM_GETTEXT, length + 1, (LPARAM)code);

			luaA_execute(code);

			delete[] code;
			break;
		}
		case EUI_CREDITS:
			MessageBox(form->GetWindow(), ELYSIAN_CREDITS, "Credits", MB_OK);
			break;
		case EUI_EXIT:
			if (MessageBox(form->GetWindow(), "ROBLOX will now close. Continue?", "Exit", MB_OKCANCEL) == IDOK)
				TerminateProcess(GetCurrentProcess(), 0);
			break;
		}
		break;
	case WM_NOTIFY:
	{
		LPNMHDR info = ((LPNMHDR)lParam);
		switch (info->code) {
		case NM_RCLICK:
			if (info->idFrom == EUI_FILEVIEW) {
				LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
				int iSelected = -1;
				iSelected = SendMessage(((LPNMHDR)lParam)->hwndFrom, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

				if (iSelected != -1) {
					int result = GetMessagePos();
					POINTS coords = MAKEPOINTS(result);
					HMENU hPopupMenu = CreatePopupMenu();
					InsertMenu(hPopupMenu, 0, MF_STRING, EUI_LV_RCM_EXEC, "Execute");
					int ret = TrackPopupMenu(hPopupMenu, TPM_RETURNCMD | TPM_TOPALIGN | TPM_LEFTALIGN, coords.x, coords.y, 0, form->GetWindow(), NULL);

					// Elysian UI ListView RightClickMenu Execute
					if (ret == EUI_LV_RCM_EXEC) {
						LVITEM lvitem;
						ZeroMemory(&lvitem, sizeof(LVITEM));
						char* name[MAX_PATH];

						lvitem.iItem = lpnmitem->iItem;
						lvitem.mask = LVIF_PARAM | LVIF_TEXT;
						lvitem.pszText = (LPSTR)name;
						lvitem.cchTextMax = MAX_PATH;
						ListView_GetItem(info->hwndFrom, &lvitem);

						//printf("iItem: %d, lParam: %x, pszText: %s\n", lpnmitem->iItem, lvitem.lParam, lvitem.pszText);
						std::string* path = (std::string*)lvitem.lParam;
						if (path) {
							std::string data;
							if (util::ReadFile(*path, data, 0)) {
								luaA_execute(data.c_str());
								form->print(RGB(0, 150, 0), "executed file '%s'\r\n", lvitem.pszText);
							}
							else {
								form->print(RGB(255, 0, 0), "ERROR: failed to open '%s'\r\n", lvitem.pszText);
							}
						}
						else {
							form->print(RGB(255, 0, 0), "ERROR: failed to get path for '%s'\r\n", lvitem.pszText);
						}
					}
					return 1;
				}
			}
			break;
		}
		break;
	}
	case WM_CLOSE:
		//ShowWindow(hwnd, SW_MINIMIZE);
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}


// --- winapi ui utilities --- \\

int AddListViewItem(HWND listView, const char* name, void* lParam) {
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.pszText = (LPSTR)name;
	lvi.lParam = (LPARAM)lParam;

	if (ListView_InsertItem(listView, &lvi) == -1)
		return 0;

	return 1;
}

int ClearListView(HWND listView) {
	int count = ListView_GetItemCount(listView);
	if (!count)
		return 0;

	LVITEM lvitem;
	ZeroMemory(&lvitem, sizeof(LVITEM));

	for (int i = 0; i < count; i++) {
		lvitem.iItem = i;
		lvitem.mask = LVIF_PARAM;
		ListView_GetItem(listView, &lvitem);

		std::string* str = (std::string*)lvitem.lParam;
		if (str)
			delete str;
	}

	ListView_DeleteAllItems(listView);
}


void RemoveStyle(HWND hwnd, int style) {
	LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	lExStyle &= ~(style);
	SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
}

HWND CreateToolTip(HWND hwndTool, PTSTR pszText)
{
	if (!hwndTool || !pszText)
	{
		return FALSE;
	}
	// Get the window of the tool.

	// Create the tooltip. g_hInst is the global instance handle.
	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hwndTool, NULL,
		NULL, NULL);

	if (!hwndTool || !hwndTip)
	{
		return (HWND)NULL;
	}

	// Associate the tooltip with the tool.
	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hwndTool;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hwndTool;
	toolInfo.lpszText = pszText;
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

	return hwndTip;
}

void Form::Start(const char* window_name, const char* class_name, const char* module) {
	if (IsRunning()) return;

	_class = class_name;
	_hinstance = GetModuleHandle(module);

	/* set up init event for communication with window thread */
	_init_event = CreateEventA(0, TRUE, FALSE, 0);

	/* spawn window thread */
	_thread = new util::athread(init_stub, this);

	/* wait for init */
	WaitForSingleObject(_init_event, INFINITE);

	/* check for errors */
	if (_init_error)
		throw std::exception(_init_error);

	/* set title*/
	SetTitle(window_name);

	/* clean up */
	_init_error = 0;
	CloseHandle(_init_event);
	_init_event = 0;
}

Form::~Form() {
	delete _thread;
	delete _watch_directory_thread;
	delete _console_thread;
}

int Form::IsRunning() {
	if (!_thread)
		return 0;

	return _thread->running();
}

void Form::Wait() {
	if (_thread)
		_thread->wait();
}

void Form::Close() {
	if (IsRunning())
		PostMessage(_window, WM_DESTROY, 0, 0);
}

void Form::SetTitle(const char* title) {
	SetWindowText(_window, title);
}

void Form::ToggleConsole() {
	_console_toggled = !_console_toggled;

	if (_console_toggled)
		ShowWindow(GetConsoleWindow(), SW_NORMAL);
	else
		ShowWindow(GetConsoleWindow(), SW_HIDE);
}

int Form::StartConsoleThread() {
	if (_console_routine && !IsConsoleThreadRunning()) {
		_console_thread = new util::athread(_console_routine, 0);
		return 1;
	}

	return 0;
}

void Form::AssignConsoleRoutine(CONSOLE_THREAD routine) {
	_console_routine = routine;
}

void Form::print(COLORREF col, const char * format, ...) {
	char message[CONSOLE_MESSAGE_LIMIT];
	memset(message, 0, sizeof(message));
	va_list vl;
	va_start(vl, format);
	vsnprintf_s(message, CONSOLE_MESSAGE_LIMIT, format, vl);
	va_end(vl);

	int len = SendMessage(_console_edit, WM_GETTEXTLENGTH, NULL, NULL);
	SendMessage(_console_edit, EM_SETSEL, len, len);

	CHARFORMAT cfd; // default
	CHARFORMAT cf;
	SendMessage(_console_edit, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cfd);
	memcpy(&cf, &cfd, sizeof(CHARFORMAT));

	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR; // change color
	cf.crTextColor = col;
	cf.dwEffects = 0;

	SendMessage(_console_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	SendMessage(_console_edit, EM_REPLACESEL, FALSE, (LPARAM)message);
	SendMessage(_console_edit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfd);
}

void Form::init_stub(Form* form) {
	if (form)
		form->init();
}

void Form::init() {
	#define init_error(error)	_init_error = error; SetEvent(_init_event); return;
	#define init_success()		_init_error = 0; SetEvent(_init_event);

	/* register window class */
	if (!register_window()) {
		init_error("failed to register window");
	}

	/* create window menu */
	HMENU window_menu = create_window_menu();

	/* create window */
	_window = CreateWindowExA(
		NULL,
		_class,
		0,
		WS_SYSMENU | WS_MINIMIZEBOX,
		EUI_POSX,
		EUI_POSY,
		EUI_WIDTH,
		EUI_HEIGHT,
		0,
		window_menu,
		_hinstance,
		0);

	if (!_window) {
		init_error("failed to create window");
	}

	/* load msftedit.dll (richedit) */
	if (!LoadLibraryA("Msftedit.dll")) {
		init_error("failed to load ui component");
	}

	/* init common controls */
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_WIN95_CLASSES;
	if (!InitCommonControlsEx(&icc)) {
		init_error("failed to initiate common controls");
	}

	create_ui_elements();
	_watch_directory_thread = new util::athread(watch_directory, this);

	init_success();
	
	ShowWindow(_window, SW_NORMAL);

	message_loop();

	delete _watch_directory_thread;
	return;
}

void Form::message_loop() {
	MSG message;
	int ret;

	while ((ret = GetMessage(&message, 0, 0, 0)) != 0) {
		if (ret == 0) {
			// quit message
			return;
		}
		else if (ret == -1) {
			// unexpected error
			return;
		}
		else {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}

int Form::register_window() {
	UnregisterClass(_class, _hinstance);

	WNDCLASSEX nClass;

	nClass.cbSize = sizeof(WNDCLASSEX);
	nClass.style = CS_DBLCLKS;
	nClass.lpfnWndProc = WindowProc;
	nClass.cbClsExtra = 0;
	nClass.cbWndExtra = 0;
	nClass.hInstance = _hinstance;
	nClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // TODO: make an icon for elysian
	nClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	nClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	nClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	nClass.lpszMenuName = 0;
	nClass.lpszClassName = _class;

	if (!RegisterClassEx(&nClass))
		return 0;

	return 1;
}

HMENU Form::create_window_menu() {
	HMENU _menu = CreateMenu();
	if (!_menu)
		return 0;

	HMENU main_dropdown = CreatePopupMenu();
	AppendMenu(main_dropdown, MF_STRING, EUI_EXIT, "Exit");
	AppendMenu(_menu, MF_POPUP, (UINT_PTR)main_dropdown, "Elysian");

	HMENU view_dropdown = CreatePopupMenu();

	HMENU console_menu = CreateMenu();
	AppendMenu(console_menu, MF_STRING, EUI_SHOWCONSOLE, "Toggle");
	AppendMenu(console_menu, MF_STRING, EUI_INITCMDTHREAD, "Start Thread");

	AppendMenu(view_dropdown, MF_POPUP, (UINT_PTR)console_menu, "Console");
	AppendMenu(view_dropdown, MF_STRING, EUI_CREDITS, "Credits");

	AppendMenu(_menu, MF_POPUP, (UINT_PTR)view_dropdown, "View");

	return _menu;
}

void Form::create_ui_elements() {
	#define UI_SET_COLOR(h, r, g, b) SendMessage(h, EM_SETBKGNDCOLOR, 0, RGB(r, g, b))
	#define UI_SET_FONT(h, f) SendMessage(h, WM_SETFONT, (WPARAM)f, MAKELPARAM(TRUE, 0))

	_script_edit = CreateWindowEx(NULL, "RICHEDIT50W", 0, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | WS_BORDER, EUI_PADDING, EUI_PADDING, 560 - VSCROLL_WIDTH - EUI_PADDING, 200, _window, (HMENU)EUI_TEXTBOX, 0, 0);
	_console_edit = CreateWindowEx(NULL, "RICHEDIT50W", 0, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | WS_BORDER | ES_READONLY, EUI_PADDING + 150, EUI_PADDING * 2 + 200, 560 - 6 - EUI_PADDING * 2 - 150, EUI_HEIGHT - 200 - EUI_PADDING * 2 - 59, _window, 0, 0, 0);
	_execute_button = CreateWindowEx(NULL, "BUTTON", "Execute", WS_CHILD | WS_VISIBLE, EUI_PADDING, EUI_PADDING * 2 + 200, 150 - EUI_PADDING, 25, _window, (HMENU)EUI_EXECUTE, 0, 0);
	//HWND extend_button = CreateWindowEx(NULL, "BUTTON", ">", WS_CHILD | WS_VISIBLE, 570 - 22, EUI_PADDING, 12, 312, MainWindow, (HMENU)EUI_EXTEND, 0, 0);
	_file_listview = CreateWindowEx(NULL, WC_LISTVIEW, 0, WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_SINGLESEL | LVS_LIST, EUI_PADDING + (560 - VSCROLL_WIDTH), EUI_PADDING, 160, EUI_HEIGHT - 69, _window, (HMENU)EUI_FILEVIEW, 0, 0);
	//

	SendMessage(_script_edit, WM_SETFONT, (WPARAM)gMainFont, MAKELPARAM(TRUE, 0));
	SendMessage(_script_edit, EM_SETBKGNDCOLOR, 0, RGB(250, 250, 250));
	SendMessage(_script_edit, EM_SETLIMITTEXT, 0x7FFFFFFE, 0);
	RemoveStyle(_script_edit, WS_EX_CLIENTEDGE);

	SendMessage(_console_edit, WM_SETFONT, (WPARAM)gMainFont, MAKELPARAM(TRUE, 0));
	SendMessage(_console_edit, EM_SETBKGNDCOLOR, 0, RGB(250, 250, 250));
	RemoveStyle(_console_edit, WS_EX_CLIENTEDGE);

	SendMessage(_execute_button, WM_SETFONT, (WPARAM)gMainFont, MAKELPARAM(TRUE, 0));

	//SendMessage(extend_button, WM_SETFONT, (WPARAM)GlobalFont, MAKELPARAM(TRUE, 0));

	SendMessage(_file_listview, WM_SETFONT, (WPARAM)gMainFont, MAKELPARAM(TRUE, 0));
	//SendMessage(script_edit, EM_SETLIMITTEXT, EUI_TEXT_CAP, NULL);

	// tooltips

	CreateToolTip(_console_edit, "Console");

}

// --- file list view -- \\

void Form::refresh_list_view(const char* path) {
	ClearListView(_file_listview);

	std::vector<std::string> files;
	std::string dir = path;

	util::GetFilesInDirectory(files, dir, 0);

	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++) {
		std::string* full_path = new std::string;
		*full_path = dir;
		*full_path += "\\";
		*full_path += *it;

		AddListViewItem(_file_listview, it->c_str(), (void*)full_path);
	}
}

int Form::watch_directory(Form* form) {
	char fPath[MAX_PATH];
	util::GetFile(ELYSIAN_DLL, "scripts", fPath, MAX_PATH);

	if (!*fPath)
		return 0;

	form->refresh_list_view(fPath);

	HANDLE fEvent = FindFirstChangeNotification(fPath, 0, FILE_NOTIFY_CHANGE_FILE_NAME);
	if (fEvent == INVALID_HANDLE_VALUE || !fEvent) {
		printf("FindFirstChangeNotification failed in WatchDirectory\r\n");
		return 0;
	}

	while (1) {
		int status = WaitForSingleObject(fEvent, INFINITE);

		if (status == WAIT_OBJECT_0) {
			form->refresh_list_view(fPath);
			if (!FindNextChangeNotification(fEvent)) {
				printf("FindNextChangeNotification failed in WatchDirectory\r\n");
				return 0;
			}
		}
		else {
			printf("Invalid status from WaitForSingleObject in WatchDirectory\r\n");
			return 0;
		}
	}
}

Form lform;
Form* form = &lform;