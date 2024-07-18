#include "Form.h"
//#include "Explorer.h"
#include <tchar.h>
//#include <boost\thread.hpp>
#include <Richedit.h>
#include <string>
#include <CommCtrl.h>
#include <wchar.h>

// Yay! Visual Styles :)
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define ADDPOPUPMENU(hmenu, string) \
 HMENU hSubMenu = CreatePopupMenu(); \
 AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, string);

// Add a menu item
#define ADDMENUITEM(hmenu, ID, string) \
 AppendMenu(hSubMenu, MF_STRING, ID, string);

#define Commands (WM_APP + 601)
#define ExplorerB (WM_APP + 602)

static Angel::Form* thisBas;

int Ind = 0;
CHARFORMAT CharFormat;

void Load(Angel::Form* form, Angel::OnLoad Func) {
	(*Func)(form);
}

struct threadArgs {
	Angel::Form* form;
	Angel::OnLoad func;
};

void ExecuteScript(std::string src);

static DWORD WINAPI ThreadStart(void* parameter) {
	threadArgs* t = (threadArgs*)parameter;
	(*t->func)(t->form);
	return 1;
}

extern void ProcessCommand(std::string cmd);

const char *CommandList =
"- noclip - Walk through walls.\r\n"
"- kill [p] - Kill [p]. No shit.\r\n"
"- hidename [p] - Hide [p]'s name. (Useful for stealth)\r\n"
"- showname [p] - Show [p]'s name.\r\n"
"- punish [p] - Move (destroy) [p]'s character to lighting.\r\n"
"- unpunish [p] - Restore [p]'s character.\r\n"
"- nochar [p] - Removes [p]'s clothes, body colors, etc.\r\n"
"- rechar [p] - Gives [p]'s clothes and items back.\r\n"
"- ragdoll [p] - Removes [p]'s clothes and disables them.\r\n"
"- unragdoll [p] - Enables [p].\r\n"
"- removehats [p] - Removes [p]'s hats.\r\n"
"- removep [p] - Removes [p]'s pants.\r\n"
"- gocam [p] - Let's [p] walk around without being seen.\r\n"
"- fixcam [p] - Enables the camera and puts you back in place.\r\n"
"- time [n] - Changes the game's time.\r\n"
"- nocollide [part name] - Makes part in workspace nocollided.\r\n"
"- jump [p] - Makes [p] jump.\r\n"
"- blockhead [p] - Changes [p]'s head to a block shape.\r\n"
"- invis [p] - Makes [p] invisible.\r\n"
"- vis [p] - Makes [p] visible.\r\n"
"- stealff [p1] [p2] - Steal's [p1]'s ff and gives it to [p2].\r\n"
"- removeg [p] - Remove's [p]'s gear.\r\n"
"- tp [p] [p] - Teleport's [p] to [p]. - BEING FIXED\r\n"
"- music [id] - Starts the music. - BEING FIXED\r\n"
"- cmode [p] [0-1] - Set's [p]'s camera mode to firstperson locked, or unlocked.\r\n"
"- stealstat [p] [stat name] - Steal's [p]'s stats and gives them to you.\r\n"
"- spamchat [n] [message] - Spam's the chat with your [n] amount of messages.\r\n"
"- disable - Disable any command you activated.\r\n";

namespace Angel {
	Form::Form(const char* szWindowClass, const char* Title, OnLoad Func) {
		this->hInstance = GetModuleHandle(NULL);
		HINSTANCE RichTextModule = LoadLibrary("riched32.dll");
		if (RichTextModule == 0) {
			MessageBox(NULL, TEXT("Failed to load riched32.dll"), TEXT("ERROR"), NULL);
			std::abort();
			return;
		}
		thisBas = this;
		this->WindowClass = szWindowClass;

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = &WindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wcex.lpszClassName = szWindowClass;
		wcex.lpszMenuName = "random";
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

		if (!RegisterClassEx(&wcex)) {
			MessageBox(NULL, "Failed to register window class", "ERROR", NULL);
			return;
		}
		this->hWindow = CreateWindow(szWindowClass, Title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 448, 478, 0, 0, hInstance, 0);
		if (!this->hWindow) {
			MessageBox(NULL, "Failed to create window", "ERROR", NULL);
			return;
		}
		ShowWindow(this->hWindow, SW_SHOWDEFAULT);
		UpdateWindow(this->hWindow);
		if (Func != NULL) {
			//(*Func)(this);
			threadArgs asd{
				this,
				Func
			};
			CreateThread(NULL, 0, ThreadStart, (void*)&asd, 0, 0);
		}
		MSG Message;
		while (GetMessage(&Message, NULL, 0, 0)) {
			if (Message.message == WM_KEYDOWN && Message.wParam == VK_RETURN && GetFocus() == this->hWndEdit) {
				char buffer[1024];

				GetWindowText(this->hWndEdit, buffer, 1024);
				std::string str(buffer);
				if (str != "") {
					SetWindowText(this->hWndEdit, "");
					this->OutputText("> " + str);
					ExecuteScript(str);
				}
				continue;
			}
			else if (Message.message == WM_KEYDOWN && Message.wParam == VK_RETURN && GetFocus() == this->hWndEdit) {
				SendMessage(this->hWndEdit, EM_SETSEL, 0, -1);
			}

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	Form::~Form() {
		DestroyWindow(this->hWindow);
		BreakWindow = true;
	}

	Form* Form::GetBaseInst() {
		return thisBas;
	}

	void Form::OutputText(std::string Text, COLORREF color) {
		/*
		SendMessage(this->hWndList, LB_INSERTSTRING, Ind, (LPARAM)Text.c_str());
		SendMessage(this->hWndList, LB_SETTOPINDEX, (WPARAM)Ind, 0);
		*/
		Text += "\n";
		memset(&CharFormat, 0, sizeof(CharFormat));
		CharFormat.cbSize = sizeof(CharFormat);
		CharFormat.dwMask = CFM_COLOR;
		if (color != NULL) CharFormat.crTextColor = color;
		SendMessage(this->hWndList, EM_SETCHARFORMAT, (LPARAM)SCF_SELECTION, (LPARAM)&CharFormat);
		CHARRANGE CharRange;
		CharRange.cpMin = -1, CharRange.cpMax = -1;
		SendMessage(this->hWndList, EM_EXSETSEL, 0, (LPARAM)&CharRange);
		SendMessage(this->hWndList, EM_REPLACESEL, FALSE, (LPARAM)Text.c_str());
		SendMessage(this->hWndList, WM_VSCROLL, SB_BOTTOM, (LPARAM)0);
		Ind++;

	}

	void Form::RawOutputText(std::string Text, COLORREF color) {
		/*
		SendMessage(this->hWndList, LB_INSERTSTRING, Ind, (LPARAM)Text.c_str());
		SendMessage(this->hWndList, LB_SETTOPINDEX, (WPARAM)Ind, 0);
		*/
		memset(&CharFormat, 0, sizeof(CharFormat));
		CharFormat.cbSize = sizeof(CharFormat);
		CharFormat.dwMask = CFM_COLOR;
		if (color != NULL) CharFormat.crTextColor = color;
		SendMessage(this->hWndList, EM_SETCHARFORMAT, (LPARAM)SCF_SELECTION, (LPARAM)&CharFormat);
		CHARRANGE CharRange;
		CharRange.cpMin = -1, CharRange.cpMax = -1;
		SendMessage(this->hWndList, EM_EXSETSEL, 0, (LPARAM)&CharRange);
		SendMessage(this->hWndList, EM_REPLACESEL, FALSE, (LPARAM)Text.c_str());
		SendMessage(this->hWndList, WM_VSCROLL, SB_BOTTOM, (LPARAM)0);
		Ind++;

	}

	void Form::SetupFonts() {
		HFONT hFont = CreateFont(14, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));//18, TEXT("Segoe UI"));
		SendMessage(thisBas->hWndEdit, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(thisBas->hWndList, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(thisBas->hWndOutputGroup, WM_SETFONT, (WPARAM)hFont, 0);
		//thisBas->OutputText(std::to_string(GetLastError()));
	}

	LRESULT CALLBACK Form::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		int Error;
		switch (uMsg) {
		case WM_CREATE: {
			//thisBas->hWndOutputGroup = CreateWindowEx(WS_EX_WINDOWEDGE, TEXT("BUTTON"), TEXT("Output"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 5, 1, 475, 316, hWnd, NULL, NULL, NULL);
			thisBas->hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""),
				WS_CHILD | WS_VISIBLE, 0, 305, 451,
				100, hWnd, NULL, NULL, NULL);
			thisBas->hWndList = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, TEXT(""),
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_DISABLED, 0, 0, 451, 285, 
				hWnd, NULL, NULL, NULL);
			thisBas->hMenuBar = CreateMenu();
			ADDPOPUPMENU(thisBas->hMenuBar, "&File");
			ADDMENUITEM(thisBas->hMenuBar, Commands, "&Commands");
			ADDMENUITEM(thisBas->hMenuBar, ExplorerB, "&Explorer");
			SetMenu(hWnd, thisBas->hMenuBar);
			thisBas->SetupFonts();
			EnableScrollBar(thisBas->hWndList, SB_BOTH, ESB_DISABLE_BOTH);
		}
						break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
			case Commands:
				MessageBox(hWnd, CommandList, "Commands", NULL);
				break;
			case ExplorerB:
				//new Angel::Explorer(thisBas->WindowClass, "Explorer", thisBas->hInstance, NULL);
				break;
			}
		}
						 break;
		default:
			break;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}