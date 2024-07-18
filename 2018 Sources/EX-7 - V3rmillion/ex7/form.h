#ifndef EX7FORM
#define EX7FORM
#pragma once
#include "Includes\Includes.h"

HWND MainWindow = 0;
HINSTANCE HInstance = GetModuleHandle(0);

HWND OpenButton;
HWND ExecuteButton;
HWND ClearButton;
HWND SmapsButton;

HWND InputField;
HWND OutputField;

// --- Definitions and defined Form functions --- // 

// Buttons               (500-599)
#define EXECUTE                  (WM_APP + 501)
#define CLEAR                    (WM_APP + 502)
#define OPEN                     (WM_APP + 503)
#define SMAPS                    (WM_APP + 504)

// "EX7" Menu            (100-199) 
#define UI_EXIT                  (WM_APP + 100)
#define UI_ABOUT                 (WM_APP + 102)

// "Web Utilities" Menu  (200-299)
#define HELPPAGE                 (WM_APP + 200)
#define CHANGELOG                (WM_APP + 201)
#define LEARNLUA                 (WM_APP + 202)
#define FAQ                      (WM_APP + 203)

// UI Config
#define UI_WIDTH                 393
#define UI_HEIGHT                497
#define UI_POSY                  100
#define UI_POSX                  100
#define MAX_PATH                 260

class EX7 { //make a class for some reason?
public:
	HMENU                        hMenuPopupFile;
	HMENU                        hMenuPopupUtils;

public:
	/* Colors a message according to the RGB provided */
	void                        Print(COLORREF color, const char* format, ...);
};

extern EX7* ex7;

#endif