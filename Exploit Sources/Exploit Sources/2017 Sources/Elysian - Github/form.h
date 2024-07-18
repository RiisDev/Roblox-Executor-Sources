#ifndef FORM_H
#define FORM_H
#pragma once

#include <Windows.h>
#include "util.h"

// --- Window Commands --- \\

// Elysian Menu		(500-599)
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
#define		EUI_HEIGHT		380

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

	HWND GetWindow()				{ return _window; };
	HWND GetListView()				{ return _file_listview; };
	HWND GetScriptTextbox()			{ return _script_edit; };
	HWND GetConsoleTextbox()		{ return _console_edit; };
	int IsConsoleToggled()			{ return _console_toggled; };
	int IsConsoleThreadRunning()	{ if (!_console_thread) return 0; return _console_thread->running(); };

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

#endif