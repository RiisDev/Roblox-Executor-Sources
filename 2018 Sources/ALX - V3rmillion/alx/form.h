#ifndef FORM_H
#define FORM_H 1
#include <Windows.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <io.h>

#include "main.h"

#define ALX_CONSOLE_WINDOW		(WM_APP + 500)
#define ALX_INPUT_FIELD			(WM_APP + 501)
#define ALX_WAYPOINT_BOX		(WM_APP + 502)

#define MALX_EXIT				(WM_APP + 600)
#define MALX_RESTART			(WM_APP + 601)
#define MALX_ABOUT				(WM_APP + 602)
#define MALX_CREDITS			(WM_APP + 603)
#define MALX_COMMANDS			(WM_APP + 604)

#define INPUT_CHAR_LIMIT		500

extern HWND ParentWindow;
extern HWND MainWindow;
extern HMENU WindowMenu;
extern HMODULE HInstance;

extern HWND InputField;
extern HWND ConsoleField;
extern HWND WaypointBox;

BOOL InitiateWindow();
void ConsoleOutput(const char * format, ...);
BOOL CreateSubwindows();

//LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//BOOL RegisterWindowClass(const char* wClassName);
//void AddSubWindows();
//int StartMessageLoop();
#endif