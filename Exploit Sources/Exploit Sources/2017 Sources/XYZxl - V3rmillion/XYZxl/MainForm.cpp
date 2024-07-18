#include "MainForm.h"
#include <tchar.h>
//#include <boost\thread.hpp>
#include <Richedit.h>
#include <string>
#include <thread>
#include <sstream>

// Yay! Visual Styles :)
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define ADDPOPUPMENU(hmenu, string) \
 HMENU hSubMenu = CreatePopupMenu(); \
 AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, string);

// Add a menu item
#define ADDMENUITEM(hmenu, ID, string) \
 AppendMenu(hSubMenu, MF_STRING, ID, string);

#define Commands (WM_APP + 601)
#define EasyUseA (WM_APP + 602)
#define StatChangerA (WM_APP + 603)

static Rosin::Form* thisBas;

int Ind = 0;
CHARFORMAT CharFormat;

void Load(Rosin::Form* form, Rosin::OnLoad Func) {
	(*Func)(form);
}

#define UI_SIZE_WIDTH			395 //460
#define UI_SIZE_HEIGHT			297 //330

extern bool ProcessCommand(std::string cmd);

void __cdecl DoCommand(const std::string& cmd);


void Rosin::ColorFormat(std::string Text, RGB_Struct* StartColor) {
	int Find = 0;
	std::string Buffer = Text;
	std::string Buffer2 = Text;
	if (Text.find("[") != std::string::npos) {
		Buffer = Text.substr(0, Text.find("["));
		Text = Text.substr(Text.find("[") + 1);
		if (Text.find("]") != std::string::npos) {
			std::string TagInput = Text.substr(0, Text.find("]"));
			Buffer2 = Text.substr(Text.find("]") + 1);
			if (TagInput.substr(0, 1) == "#") {
				int r, g, b;
				TagInput.erase(0, 1);
				std::istringstream(TagInput.substr(0, 2)) >> std::hex >> r;
				std::istringstream(TagInput.substr(2, 2)) >> std::hex >> g;
				std::istringstream(TagInput.substr(4, 2)) >> std::hex >> b;
				if (StartColor != nullptr) thisBas->RawOutputText(Buffer, RGB(StartColor->R, StartColor->G, StartColor->B));
				else thisBas->RawOutputText(Buffer);
				ColorFormat(Buffer2, new RGB_Struct{ r, g, b });
				return;
			}
			else if (TagInput.substr(0, 1) == "/") {
				if (StartColor != nullptr) thisBas->RawOutputText(Buffer, RGB(StartColor->R, StartColor->G, StartColor->B));
				else thisBas->RawOutputText(Buffer);
				ColorFormat(Buffer2);
				return;
			}
			else {
				Buffer += "[" + TagInput + "]";
				if (StartColor != nullptr) thisBas->RawOutputText(Buffer, RGB(StartColor->R, StartColor->G, StartColor->B));
				else thisBas->RawOutputText(Buffer);
				ColorFormat(Buffer2);
				return;
			}
		}
	}
	if (StartColor != nullptr) thisBas->RawOutputText(Buffer, RGB(StartColor->R, StartColor->G, StartColor->B));
	else thisBas->RawOutputText(Buffer);
	return;
}

const char* CommandStr = "noclip\r\n"
"swim\r\n"
"superjump\r\n"
"airwalk\r\n"
"jesusfly\r\n"
"ragdoll\r\n"
"disable\r\n"
"ws [p] [speed]\r\n"
"jump [p]\r\n"
"plat [p]\r\n"
"blockhead [p]\r\n"
"invis [p]\r\n"
"vis [p]\r\n"
"jp [p] [power] -- jumppower\r\n"
"confuse [p]\r\n"
"btools [p]\r\n"
"god [p]\r\n"
"nochar [p]\r\n"
"rechar [p]\r\n"
"freeze [p]\r\n"
"unfreeze [p]\r\n"
"ragdoll [p]\r\n"
"removehats [p]\r\n"
"seth [p] [health]\r\n"
"removeg [p]\r\n"
"ungod [p]\r\n"
"kill [p]\r\n"
"punish [p]\r\n"
"unragdoll [p]\r\n"
"naked [p]\r\n"
"kick [p]\r\n"
"gocam [p]\r\n"
"fixcam [p]\r\n"
"unpunish [p]\r\n"
"hidename [p]\r\n"
"showname [p]\r\n"
"fog [num]\r\n"
"time [num]\r\n"
"rainbow [on / off]\r\n"
"fogcolor [r] [g] [b]\r\n"
"waterheight [height]\r\n"
"tp [p] [p]\r\n"
"findsound\r\n"
"music [id]\r\n"
"cmode [p] [0 / 1]\r\n"
"stealstat [p] [stat name]\r\n"
"changestat [p] [statname] [val]\r\n"
"spamchat[times][chat msg]\r\n"
"clearterrain\r\n"
"bighead [p]\r\n"
"reghead [p]\r\n"
"unlockchar [p]\r\n"
"lockchar [p]\r\n"
"removechar [p]\r\n"
"removesounds\r\n"
"setname [p] [new name+]\r\n"
"char [p] [id/off]\r\n"
"setbc [p] [0-3]\r\n"
"exe [script] -- (ONLY WORKS AT YOUR OWN PLACE)\r\n"
"rbxlock [p]\r\n"
"unrbxlock [p]\r\n"
"checkrbxlock [p]\r\n"
"setage [p] [age]\r\n"
"getage [p]\r\n"
"shiny [p]\r\n"
"unshiny [p]\r\n"
"neon [p]\r\n"
"unneon [p]\r\n"
"rlegs [p]\r\n"
"rarms [p]\r\n"
"rlimbs [p]\r\n"
"stealgear [p]\r\n"
"stealhats [p]\r\n"
"loopkill [p]\r\n"
"unloopkill [p]\r\n"
"showstats [p]\r\n"
"sit [p]\r\n"
"lklist -- loopkill list\r\n"
"skydive [p]\r\n"
"metal [p]\r\n"
"unmetal [p]\r\n"
"loopcmd [cmd+]\r\n"
"stoploop\r\n"
"getloopcmd\r\n"
"looptime [milliseconds] -- controls how fast loopcmd runs\r\n"
"sgod [p]\r\n"
"unsgod [p]\r\n"
"sgodlist\r\n"
"skybox [id]\r\n"
"hat [id]\r\n"
"gear [id]\r\n"
"exescript [script name] -- for use with the lua api\r\n"
"ff [p]\r\n"
"fire [p]\r\n"
"sp [p]\r\n"
"reload\r\n"
"exit\r\n"
"clr";

HWND RobloxForm = NULL;
HWND MainForm = NULL;

void FormLoop() {
	while (true) {
		if (RobloxForm == NULL || MainForm == NULL) break;
		if (IsIconic(MainForm) == 0 && GetForegroundWindow() == RobloxForm) {
			SetWindowPos(MainForm, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		else {
			SetWindowPos(MainForm, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		Sleep(1);
	}
}

namespace Rosin {
	Form::Form(const char* szWindowClass, const char* Title, OnLoad Func) {
		HINSTANCE hInstance = GetModuleHandle(NULL);
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
			UnregisterClass(szWindowClass, hInstance);
			RegisterClassEx(&wcex);
			return;
		}
		this->hWindow = CreateWindow(szWindowClass, Title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, 0, 0, hInstance, 0);
		if (!this->hWindow) {
			MessageBox(NULL, "Failed to create window", "ERROR", NULL);
			return;
		}
		ShowWindow(this->hWindow, SW_SHOWDEFAULT);
		UpdateWindow(this->hWindow);
		if (Func != NULL) {
			//(*Func)(this);
			std::thread t(*Func, this);
			t.detach();
		}
		/*RobloxForm = FindWindow(NULL, "ROBLOX");
		if (RobloxForm != INVALID_HANDLE_VALUE) {
		//MainForm = this->hWindow;
		std::thread t(CloseRoblox, RobloxForm);
		t.detach();
		}*/

		MSG Message;
		while (GetMessage(&Message, NULL, 0, 0)) {
			//if (BreakWindow) break;
			if (Message.message == WM_KEYDOWN && Message.wParam == VK_RETURN && GetFocus() == this->hWndEdit) {
				char buffer[1024];

				GetWindowText(this->hWndEdit, buffer, 1024);
				std::string str(buffer);
				if (str != "") {
					SetWindowText(this->hWndEdit, "");
					this->OutputText("> " + str);
					DoCommand(str);
				}
				continue;
			}
			else if (Message.message == WM_KEYDOWN && Message.wParam == VK_RETURN && GetFocus() == this->hWndEdit) {
				SendMessage(this->hWndEdit, EM_SETSEL, 0, -1);
			}

			SendMessage(this->hWndList, EM_HIDESELECTION, 1, 0);

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		if (!BreakWindow) {
			DestroyWindow(this->hWindow);
			BreakWindow = true;
			exit(0);
		}
	}
	Form::~Form() {
		if (!BreakWindow) {
			DestroyWindow(this->hWindow);
			BreakWindow = true;
		}
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

	void Form::CloseWindow() {
		DestroyWindow(this->hWindow);
		BreakWindow = true;
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

	void Form::ClearOutput() {
		SETTEXTEX fat;
		fat.codepage = CP_ACP;
		fat.flags = ST_KEEPUNDO;
		SendMessage(this->hWndList, EM_SETTEXTEX, (WPARAM)&fat, 0);
	}

	void Form::SetupFonts() {
		HFONT hFont = CreateFont(14, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));//18, TEXT("Segoe UI"));
		SendMessage(thisBas->hWndEdit, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(thisBas->hWndList, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(thisBas->hWndOutputGroup, WM_SETFONT, (WPARAM)hFont, 0);
		//thisBas->OutputText(std::to_string(GetLastError()));
	}

	void OpenEasyUseGUI() {
//		new Rosin::EasyUse("ROSIN_EASY_USE", "Easy Use");
	}

	void OpenStatChangerGUI() {
//		new Rosin::StatChanger("ROSIN_STAT_CHANGER", "Stat Changer");
	}

	LRESULT CALLBACK Form::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		int Error;
		switch (uMsg) {
		case WM_CREATE: {
			thisBas->hWndOutputGroup = CreateWindowEx(WS_EX_WINDOWEDGE, TEXT("BUTTON"), TEXT("Output"), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 5, 1, 475, 316, hWnd, NULL, NULL, NULL);
			thisBas->hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""),
				WS_CHILD | WS_VISIBLE, 5, 323, 475,
				22, hWnd, NULL, NULL, NULL);
			thisBas->hWndList = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, TEXT(""),
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, 10, 20, 465,
				290, hWnd, NULL, NULL, NULL);
			thisBas->hMenuBar = CreateMenu();
			ADDPOPUPMENU(thisBas->hMenuBar, "&Stuff");
			ADDMENUITEM(thisBas->hMenuBar, Commands, "&Commands");
			ADDMENUITEM(thisBas->hMenuBar, EasyUseA, "&Easy Use GUI");
			ADDMENUITEM(thisBas->hMenuBar, StatChangerA, "&Stat Changer");
			SetMenu(hWnd, thisBas->hMenuBar);
			thisBas->SetupFonts();
		}
						break;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case Commands:
				thisBas->OutputText(std::string("\nCommands:\n") + CommandStr);
				break;
			case EasyUseA: {
				std::thread t(OpenEasyUseGUI);
				t.detach();
			}
						   break;
			case StatChangerA: {
				std::thread a(OpenStatChangerGUI);
				a.detach();
			}
							   break;
			}
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SETFOCUS:
			uMsg = WM_KILLFOCUS;
			break;
		default:
			break;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	void ExtOutputText(std::string s) {
		thisBas->OutputText(s);
	}

}

