
#include <Windows.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include "gui.hpp"
#include "globals.hpp"
#include "wrap.hpp"
#include "security.hpp"
#include "Utilities/Scan.h"
#include "Utilities/Instance.h"
#include "Lua/lua.hpp"

DWORD ScriptContext = 0;
DWORD int3breakpoint = 0;

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <TlHelp32.h>

#define DLURL_SUCCESS				0
#define DLURL_FAILED_REQUEST		1
#define DLURL_FAILED_CERT_QUERY		2
#define DLURL_FAILED_CERT_CHECK		3

// athread

namespace util {
	class athread {
	private:
		HANDLE thread;
		int threadID;

	public:
		athread(void* func, void* args);
		int terminate();
		int wait();
		HANDLE getThread();
		int getExitCode();
		int getThreadId();
		int running();
	};

	class timer {
	private:
		unsigned __int64 _starting_tick;
		unsigned __int64 _finish_tick;

	public:
		timer() { Reset(); }

		/* resets the timer */
		void Reset() {
			_starting_tick = 0;
			_finish_tick = 0;
		}

		/* starts the timer */
		void Start() {
			Reset();
			_starting_tick = GetTickCount64();
		}

		/* stops the timer and returns the elapsed time */
		double Stop() {
			_finish_tick = GetTickCount64();
			return GetElapsedTime();
		}

		/* returns elapsed time */
		double GetElapsedTime() {
			if (_starting_tick & _finish_tick)
				return (_finish_tick - _starting_tick) / 1000.0;

			return 0.0;
		}
	};

	template <typename T>
	class singleton {
	private:
		static T* _instance;
	public:
		static T* instance() {
			if (_instance)
				return _instance;

			_instance = new T;
			return _instance;
		}

		singleton(singleton const&) = delete;
		void operator=(singleton const&) = delete;
	};

	// other

	void GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize);

	long int GetFileSize(FILE* ifile);

	int DownloadURL(const std::string& server, const std::string& path, const std::string& params, std::string& out, unsigned char useSSL, unsigned char dontCache, unsigned char direct);

	int ReadFile(const std::string& path, std::string& out, unsigned char binary);

	int WriteFile(const std::string& path, std::string data, unsigned char binary);

	std::vector<std::string> GetArguments(std::string input);

	int GetProcessByImageName(const char* imageName);

	std::string lowercase(std::string& input);

	std::wstring s2ws(const std::string& str);

	std::string ws2s(const std::wstring& wstr);

	std::string GetRawStringAtDelim(std::string input, int arg, char delim);

	void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory, unsigned char includePath);

	int GetEIP();

	void pause();

	namespace registry {
		int ReadString(HKEY key, const char* value, std::string& out);
	}
};


#include <WinInet.h>
#pragma comment(lib, "WinInet.lib")

#include <locale>
#include <utility>
#include <codecvt>

namespace util {
	athread::athread(void* func, void* args) {
		thread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)func, args, NULL, (LPDWORD)&threadID);
	}

	int athread::terminate() {
		if (this->running()) {
			if (TerminateThread(thread, 0)) {
				thread = NULL;
				threadID = 0;
			}
		}

		return getExitCode();
	}

	HANDLE athread::getThread() {
		return thread;
	}

	int athread::getExitCode() {
		int exitCode = 0;
		if (thread)
			GetExitCodeThread(thread, (LPDWORD)&exitCode);

		return exitCode;
	}

	int athread::getThreadId() {
		return threadID;
	}

	int athread::wait() {
		DWORD exitCode = 0;

		if (thread) {
			DWORD result;

			do {
				result = WaitForSingleObject(thread, 0);

				if (result == WAIT_OBJECT_0) {
					GetExitCodeThread(thread, &exitCode);
					break;
				}

				Sleep(100);

			} while (1);
		}

		return exitCode;
	}

	int athread::running() {
		return this->getExitCode() == STILL_ACTIVE;
	}

	void GetFile(const char* dllName, const char* fileName, char* buffer, int bfSize) {
		GetModuleFileName(GetModuleHandle(dllName), buffer, bfSize);
		if (strlen(fileName) + strlen(buffer) < MAX_PATH) {
			char* pathEnd = strrchr(buffer, '\\');
			strcpy(pathEnd + 1, fileName);
		}
		else {
			*buffer = 0;
		}
	}

	long int GetFileSize(FILE* ifile) {
		long int fsize = 0;
		long int fpos = ftell(ifile);
		fseek(ifile, 0, SEEK_END);
		fsize = ftell(ifile);
		fseek(ifile, fpos, SEEK_SET);

		return fsize;
	}

	int DownloadURL(const std::string& server, const std::string& path, const std::string& params, std::string& out, unsigned char useSSL, unsigned char dontCache, unsigned char direct) {
		HINTERNET interwebs = NULL;
		HINTERNET hConnect = NULL;
		HINTERNET hRequest = NULL;
		int rResults = 0;

		std::string path_w_params = path + (params.empty() ? "" : "?" + params);

		interwebs = InternetOpen("util/Agent", direct ? INTERNET_OPEN_TYPE_DIRECT : INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);

		if (interwebs)
			hConnect = InternetConnect(interwebs, server.c_str(), useSSL ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

		if (hConnect)
			hRequest = HttpOpenRequest(hConnect, "GET", path_w_params.c_str(), NULL, NULL, NULL, (useSSL ? INTERNET_FLAG_SECURE : 0) | (dontCache ? INTERNET_FLAG_NO_CACHE_WRITE : 0), 0);

		if (hRequest)
			rResults = HttpSendRequest(hRequest, 0, 0, NULL, NULL);

		if (rResults) {
			char buffer[2000];
			DWORD bytesRead = 0;
			do {
				InternetReadFile(hRequest, buffer, 2000, &bytesRead);
				out.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			rResults = DLURL_SUCCESS;
		}
		else
			rResults = DLURL_FAILED_REQUEST;

		if (interwebs) InternetCloseHandle(interwebs);
		if (hConnect) InternetCloseHandle(hConnect);
		if (hRequest) InternetCloseHandle(hRequest);

		return rResults;
	}

	int ReadFile(const std::string& path, std::string& out, unsigned char binary) {
		std::ios::openmode mode = std::ios::in;
		if (binary)
			mode |= std::ios::binary;

		std::ifstream file(path, mode);
		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			out = buffer.str();
			file.close();
			return 1;
		}

		file.close();
		return 0;
	}

	int WriteFile(const std::string& path, std::string data, unsigned char binary) {
		std::ios::openmode mode = std::ios::out;
		if (binary)
			mode |= std::ios::binary;

		std::ofstream file(path, mode);
		if (file.is_open()) {
			file << data;
			file.close();
			return 1;
		}

		file.close();
		return 0;
	}

	std::vector<std::string> GetArguments(std::string input) {
		std::vector<std::string> rtn;

		if (input[0] == ' ') {
			input = input.substr(1);
		}

		BYTE size = input.size();
		DWORD pos1 = 0;

		for (int i = 0; i < size; ++i) {
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

	std::string GetRawStringAtDelim(std::string input, int arg, char delim) {
		char lc = 0;
		int c = 0;

		for (int i = 0; i < input.size(); ++i) {
			if (input[i] == delim && lc != delim)
				c++;

			if (c == arg)
				return input.substr(i + 1);
		}

		return "";
	}

	std::string lowercase(std::string& input) {
		std::string s;
		for (int i = 0; i < input.size(); ++i) {
			s.push_back(tolower(input[i]));
		}

		return s;
	}

	std::wstring s2ws(const std::string& str)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	std::string ws2s(const std::wstring& wstr)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

	int GetProcessByImageName(const char* imageName) {
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(PROCESSENTRY32);

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(snapshot, &entry) == TRUE)
		{
			while (Process32Next(snapshot, &entry) == TRUE)
			{
				if (strcmp(entry.szExeFile, imageName) == 0)
				{
					CloseHandle(snapshot);
					return entry.th32ProcessID;
				}
			}
		}

		CloseHandle(snapshot);
		return 0;
	}

	void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory, unsigned char includePath) // thx stackoverflow
	{
		HANDLE dir;
		WIN32_FIND_DATA file_data;

		if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
			return; /* No files found */

		do {
			const std::string file_name = file_data.cFileName;
			const std::string full_file_name = directory + "/" + file_name;
			const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

			if (file_name[0] == '.')
				continue;

			if (is_directory)
				continue;


			out.push_back(includePath ? full_file_name : file_name);
		} while (FindNextFile(dir, &file_data));

		FindClose(dir);
	}

	/* Gets current time*/


	int __declspec(naked) GetEIP() {
		__asm pop eax
		__asm ret
	}

	void pause() {
		printf("Press enter to continue . . .");
		getchar();
	}

	namespace registry {
		int ReadString(HKEY key, const char* value, std::string& out) {
			int val_sz;
			int val_type;
			char* val;

			if (RegQueryValueEx(key, value, NULL, (LPDWORD)&val_type, NULL, (LPDWORD)&val_sz) != ERROR_SUCCESS)
				return 0;

			if (val_type != REG_SZ || !val_sz)
				return 0;

			val = new (std::nothrow) char[val_sz];
			if (!val) return 0;

			if (RegQueryValueEx(key, value, NULL, NULL, (LPBYTE)val, (LPDWORD)&val_sz) != ERROR_SUCCESS) {
				delete[] val;
				return 0;
			}

			out = val;
			delete[] val;
			return 1;
		}
	}
};


unsigned int danghui_rbase()
{
	return (unsigned int)(GetModuleHandleA("RobloxPlayerBeta.exe"));
}

#define EUI_EXIT			(WM_APP + 500)
#define EUI_SHOWCONSOLE		(WM_APP + 501)
#define EUI_INITCMDTHREAD	(WM_APP + 502)
#define EUI_CREDITS			(WM_APP + 503)

// Elysian UI		(600-699)
#define EUI_TEXTBOX			(WM_APP + 600)
#define EUI_EXECUTE			(WM_APP + 601)
#define EUI_CBSCRIPT		(WM_APP + 602)
#define EUI_EXTEND			(WM_APP + 603)
#define EUI_FILEVIEW		(WM_APP + 604)

// Elysian UI Extra	(700-749)
#define EUI_LV_RCM_EXEC		(WM_APP + 700)
#define	EUI_LV_RCM_STOP		(WM_APP + 701)

// --- UI Config --- \\

#define		EUI_PADDING		10

#define		EUI_POSX		300
#define		EUI_POSY		300
#define		EUI_WIDTH		729
#define		EUI_HEIGHT		430

#define		CONSOLE_MESSAGE_LIMIT	255

// --- Utility Macros --- \\

#define VSCROLL_WIDTH		16
#define HSCROLL_HEIGHT		16

typedef void(*CONSOLE_THREAD)();

class Form {
private:
	HWND							_window = 0;
	const char*						_class = 0;
	HINSTANCE						_hinstance = 0;
	util::athread*					_thread = 0;
	util::athread*					_watch_directory_thread = 0;
	util::athread*					_console_thread = 0;
	int								_console_toggled = 0;
	CONSOLE_THREAD					_console_routine = 0;

	const char*						_init_error = 0;
	HANDLE							_init_event = 0;

	HMENU							_menu;
	HWND							_file_listview;
	HWND							_script_edit;
	HWND							_console_edit;
	HWND							_execute_button;

public:
	~Form();

	/* creates a new thread and initiates the window */
	void							Start(const char* window_name, const char* class_name, const char* local_module_name);
	void							SetTitle(const char* title);

	/* returns 1 if the window is running */
	int								IsRunning();

	/* waits for the window thread to exit or form to close */
	void							Wait();

	/* sends a WM_DESTROY message to the window */
	void							Close();
	void							ToggleConsole();
	int								StartConsoleThread();
	void							AssignConsoleRoutine(CONSOLE_THREAD routine);

	/* writes a formatted and colored message to the console textbox */
	void							print(COLORREF col, const char * format, ...);

	HWND GetWindow() { return _window; };
	HWND GetListView() { return _file_listview; };
	HWND GetScriptTextbox() { return _script_edit; };
	HWND GetConsoleTextbox() { return _console_edit; };
	int IsConsoleToggled() { return _console_toggled; };
	int IsConsoleThreadRunning() { if (!_console_thread) return 0; return _console_thread->running(); };

private:
	static void						init_stub(Form* form);
	void							init();
	void							message_loop();

	int								register_window();
	HMENU							create_window_menu();
	void							create_ui_elements();

	static int						watch_directory(Form* form);
	void							refresh_list_view(const char* path);
};

extern Form* form;


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

			danghui_Aexecute(code);

			delete[] code;
			break;
		}
		case EUI_CREDITS:
			MessageBox(form->GetWindow(), "", "Credits", MB_OK);
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
								danghui_Aexecute(data.c_str());
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

	return _menu;
}

void Form::create_ui_elements() {
#define UI_SET_COLOR(h, r, g, b) SendMessage(h, EM_SETBKGNDCOLOR, 0, RGB(r, g, b))
#define UI_SET_FONT(h, f) SendMessage(h, WM_SETFONT, (WPARAM)f, MAKELPARAM(TRUE, 0))

	_script_edit = CreateWindowEx(NULL, "RICHEDIT50W", 0, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | WS_BORDER, 12, 128, 540, 240, _window, (HMENU)EUI_TEXTBOX, 0, 0);
	_console_edit = CreateWindowEx(NULL, "RICHEDIT50W", 0, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE | WS_BORDER | ES_READONLY, 12, 12, 540, 110, _window, 0, 0, 0);
	_execute_button = CreateWindowEx(NULL, "BUTTON", "Execute", WS_CHILD | WS_VISIBLE, 534, 374, 183, 23, _window, (HMENU)EUI_EXECUTE, 0, 0);
	//HWND extend_button = CreateWindowEx(NULL, "BUTTON", ">", WS_CHILD | WS_VISIBLE, 570 - 22, EUI_PADDING, 12, 312, MainWindow, (HMENU)EUI_EXTEND, 0, 0);
	_file_listview = CreateWindowEx(NULL, WC_LISTVIEW, 0, WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_SINGLESEL | LVS_LIST,558, 12, 159, 356, _window, (HMENU)EUI_FILEVIEW, 0, 0);
	//

	SendMessage(_script_edit, WM_SETFONT, (WPARAM)gMainFont, MAKELPARAM(TRUE, 0));
	SendMessage(_script_edit, EM_SETLIMITTEXT, 0x7FFFFFFE, 0);
	RemoveStyle(_script_edit, WS_EX_CLIENTEDGE);

	SendMessage(_console_edit, WM_SETFONT, (WPARAM)gMainFont, MAKELPARAM(TRUE, 0));
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
	util::GetFile("Reaxus.dll", "scripts", fPath, MAX_PATH);

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

unsigned int danghui_grabscriptcontext()
{
	if (ScriptContext)
		return ScriptContext;
	DWORD SCVT = aslr(0x103C3CC);
	ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&SCVT, "xxxx");
	return ScriptContext;
}

unsigned int danghui_grabstate(unsigned int scriptcontext, int idx)
{
	DWORD* ScriptContext = (DWORD*)scriptcontext;
	return ScriptContext[41] - (DWORD)&ScriptContext[41];
}

DWORD RtlUnwindJmpTo;
DWORD KiUserExceptionDispatcherJmpTo;

__declspec(naked) void VEHK_Hook()
{
	__asm
	{
		mov ebx, [esp]
		push ecx
		push ebx
		call RtlUnwindJmpTo
		jmp KiUserExceptionDispatcherJmpTo
	}
}

DWORD NtDllLocate(PCHAR FnName)
{
	static HMODULE NtDll;
	if (!NtDll)
		NtDll = GetModuleHandleA("ntdll.dll");

	return (DWORD)(GetProcAddress(NtDll, FnName));
}

DWORD KernelBaseLocate(PCHAR FnName)
{
	static HMODULE KernelBase;
	if (!KernelBase)
		KernelBase = GetModuleHandleA("KERNELBASE.dll");

	return (DWORD)(GetProcAddress(KernelBase, FnName));
}

BOOL PlaceHook(DWORD Where, PVOID JumpTo)
{
	DWORD nOldProtect;
	if (!VirtualProtect((LPVOID)Where, 5, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(Where) = 0xE9;
	*(DWORD*)(Where + 1) = ((DWORD)JumpTo) - Where - 5;
	if (!VirtualProtect((LPVOID)Where, 5, nOldProtect, &nOldProtect))
		return FALSE;
	return TRUE;
}

BOOL CreateHooks()
{
	VMProtectBeginMutation("CreateHooks (VEH)");
	DWORD RtlUnwind = NtDllLocate("RtlUnwind");
	DWORD KiUserExceptionDispatcher = NtDllLocate("KiUserExceptionDispatcher");
	if (RtlUnwind && KiUserExceptionDispatcher)
	{
		RtlUnwindJmpTo = RtlUnwind + 0x13E;
		KiUserExceptionDispatcherJmpTo = KiUserExceptionDispatcher + 0xF;
		return PlaceHook(KiUserExceptionDispatcher + 5, VEHK_Hook);
	}
	VMProtectEnd();
	return FALSE;
}

LONG WINAPI danghui_int3cbreakpoint(PEXCEPTION_POINTERS ex)
{
	if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		if (ex->ContextRecord->Eip == int3breakpoint)
		{
			ex->ContextRecord->Eip = (DWORD)(danghui_int3fnhandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void danghui_main()
{

	VMProtectBeginMutation("eeeeeeee");
	try {
		form->Start("Titan | V0.0.0 Beta", "e01043", "Reaxus.dll");
	}
	catch (std::exception e) {
		//exit(e.what());
	}
	form->print(RGB(0, 0, 200), "Scanning...\n");
	danghui_grabscriptcontext();
	int3breakpoint = aslr(0x40104B);
	AddVectoredExceptionHandler(TRUE, danghui_int3cbreakpoint);
	form->print(RGB(0, 0, 0), "OK!\n");
	VMProtectEnd();
}

BOOL WINAPI DllMain(HMODULE Dll, DWORD jReason, LPVOID)
{
	VMProtectBegin("sopt");
	if (jReason == DLL_PROCESS_ATTACH)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)danghui_main, NULL, NULL, NULL);
	VMProtectEnd();
	return TRUE;
}