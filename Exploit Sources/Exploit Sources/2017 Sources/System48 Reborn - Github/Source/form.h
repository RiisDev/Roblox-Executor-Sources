#ifndef SYSFORM
#define SYSFORM
#pragma once
#include <Windows.h>

// --- Definitions and defined Form functions --- // 

// "System48" Menu      (100-199)
#define UI_EXIT                 (WM_APP + 100)
#define UI_CREDITS              (WM_APP + 101)
#define UI_MINIMIZE             (WM_APP + 102)
#define UI_RSPLORER             (WM_APP + 103)

// "Web-Utilities" Menu (200-299)
#define UI_HELPPAGE             (WM_APP + 200)
#define UI_WLCHANGE             (WM_APP + 201)
#define UI_FAQ                  (WM_APP + 202)
#define UI_DISCORD              (WM_APP + 203)
#define UI_TRIAL                (WM_APP + 204)

// "Lighting" Menu      (300-399)
#define REMOVE_FOG              (WM_APP + 300)
#define RED_FOG                 (WM_APP + 301)
#define BLUE_FOG                (WM_APP + 302)
#define GREEN_FOG               (WM_APP + 303)
#define BLACK_FOG               (WM_APP + 304)
#define WHITE_FOG               (WM_APP + 305)
#define TOGGLEDL                (WM_APP + 306)
#define TIME_DAY                (WM_APP + 307)
#define TIME_NIGHT              (WM_APP + 308)

// "QuickCommand" Menu  (400-499)
#define LOCALFORCEFIELD         (WM_APP + 400)
#define LOCALFIRE               (WM_APP + 401)
#define LOCALSMOKE              (WM_APP + 402)
#define LOCALGODMODE            (WM_APP + 403)
#define QUICKRESPAWN            (WM_APP + 404)
#define QUICKWS                 (WM_APP + 4000)

// "Other" SubMenu
#define BTOOLS                  (WM_APP + 4001)
#define GUESTCHAT               (WM_APP + 4002)
#define UNCLEBILL               (WM_APP + 4003)

// Destructive
#define UNANCHORWS              (WM_APP + 405)
#define UNLOCKWS                (WM_APP + 406)
// Other
#define FENCINGREACH            (WM_APP + 407)
#define RECRUITMENTREACH        (WM_APP + 408)

// System48 UI         (500-599)
#define UI_INPUT               (WM_APP + 500)

// UI Config
#define UI_POSX             100
#define UI_POSY             100
#define UI_WIDTH            514
#define UI_HEIGHT           281

#define INPUT_CHAR_LIMIT    500

class System48 {
public:
	HWND                    MainWindow = 0;
	HINSTANCE               HInstance = 0;

	HWND                    InputField;
	HWND                    ScriptInputField;
	HWND                    OutputField;

	HWND                    System48DropDown;
	static bool             UpdateCheck();
	static void             SysInit();
	static int				getState(DWORD ScriptContext);


	/* Colors a message according to the RGB provided */
	void                    Print(COLORREF color, const char* format, ...);

	/* Scans the open files for anything doing something to System48 */
	void                    ScanOpenFiles(int* flag);

	/* Closes the window by task-killing ROBLOX */
	//void Close()            { exit(0); };

	HMENU                   DropDown;
};

extern System48* sys;


#endif
