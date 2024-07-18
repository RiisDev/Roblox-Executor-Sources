#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

using std::string; //
using std::vector;

HMODULE hInstance = 0;
HWND ScriptTextBoxHWND = 0;
HMENU hMenu;
HWND main_window = 0;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#define MYMENU_EXIT                     (WM_APP + 101)
#define MYMENU_ABOUTMB                  (WM_APP + 102)
#define MYMENU_EXECUTECODE              (WM_APP + 103)
#define MYMENU_SCRIPTTEXTBOX            (WM_APP + 104)
#define MYMENU_CLEARSCRIPT              (WM_APP + 105)
#define MYMENU_MINIMIZE                 (WM_APP + 109)
#define OPEN (WM_APP + 110)
#define TOPKEK (WM_APP + 111)

class SampleUI
{
public:
	void CreateUIWindow();
	void CreateUI();
	void print(COLORREF color, const char* fmn, ...);

private:
	~SampleUI()
	{

	}
};