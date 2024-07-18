#include "EasyUse.h"
#include <tchar.h>
//#include <boost\thread.hpp>
#include <utility>
#include <Richedit.h>
#include <string>
#include <thread>

// Yay! Visual Styles :)
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define ADDPOPUPMENU(hmenu, string) \
 HMENU hSubMenu = CreatePopupMenu(); \
 AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, string);

// Add a menu item
#define ADDMENUITEM(hmenu, ID, string) \
 AppendMenu(hSubMenu, MF_STRING, ID, string);

#define KILL (WM_APP + 610)

static Rosin::EasyUse* thisBas;

extern bool ProcessCommand(std::string cmd);
HFONT hFont = CreateFont(14, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));//18, TEXT("Segoe UI"));

void Load(Rosin::EasyUse* EasyUse, Rosin::OnLoadEU Func) {
	(*Func)(EasyUse);
}

namespace Rosin {
	EasyUse::EasyUse(const char* szWindowClass, const char* Title, OnLoadEU Func) {
		HINSTANCE hInstance = GetModuleHandle(NULL);
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
		this->hWindow = CreateWindow(szWindowClass, Title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 447, 378, 0, 0, hInstance, 0);
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
		MSG Message;
		while (GetMessage(&Message, NULL, 0, 0)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		UnregisterClass(thisBas->WindowClass, GetModuleHandle(NULL));
	}
	EasyUse::~EasyUse() {
		DestroyWindow(this->hWindow);
		BreakWindow = true;
	}

	EasyUse* EasyUse::GetBaseInst() {
		return thisBas;
	}

	void EasyUse::SetupFonts() {
		SendMessage(thisBas->hPlrTextBox, WM_SETFONT, (WPARAM)hFont, 0);
		//SendMessage(thisBas->hWndList, WM_SETFONT, (WPARAM)hFont, 0);
		//SendMessage(thisBas->hWndOutputGroup, WM_SETFONT, (WPARAM)hFont, 0);
		//thisBas->OutputText(std::to_string(GetLastError()));
	}

	std::string GetText() {
		char buffer[1024];

		GetWindowText(thisBas->hPlrTextBox, buffer, 1024);
		std::string str(buffer);
		if (str != "" && str != "Enter Player Here") {
			return str;
		}
		else {
			return "";
		}
	}

	void EasyUse::AddButton(HWND Form, Size s, Position p, std::string name, OnClick clicked) {
		ButtonAmount++;
		HWND hButton = CreateWindowEx(WS_EX_WINDOWEDGE, TEXT("BUTTON"), TEXT(name.c_str()),
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, p.X, p.Y, s.X, s.Y, Form, (HMENU)ButtonAmount, (HINSTANCE)GetWindowLong(Form, GWL_HINSTANCE), NULL);
		SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, 0);
		Button NewButton = Button{
			clicked,
			hButton,
			ButtonAmount
		};
		this->Buttons[ButtonAmount] = NewButton;
	}

	void EasyUse::AddTextBox(HWND Form, Size s, Position p, std::string startingText, std::string StringId) {
		HWND hTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(startingText.c_str()),
			WS_CHILD | WS_VISIBLE, p.X, p.Y, s.X,
			s.Y, Form, NULL, NULL, NULL);
		SendMessage(hTextBox, WM_SETFONT, (WPARAM)hFont, 0);
		TextBox newTextBox = TextBox{
			hTextBox,
			StringId
		};
		this->TextBoxes[StringId] = newTextBox;
	}

	std::string EasyUse::GetStringById(std::string BoxId) {
		if (TextBoxes.find(BoxId) != TextBoxes.end()) {
			char buffer[1024];
			GetWindowText(TextBoxes.at(BoxId).hTextBox, buffer, 1024);
			std::string str(buffer);
			return str;
		}
		return "";
	}

	LRESULT CALLBACK EasyUse::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		int Error;
		int ID;
		std::thread t;
		switch (uMsg) {
		case WM_CREATE: {
			//Increment by 85
			thisBas->AddButton(hWnd, { 80, 25 }, { 10, 10 }, "Kill", []()->void {
				ProcessCommand("kill " + GetText());
			});

			thisBas->AddButton(hWnd, { 80, 25 }, { 10, 45 }, "Remove Hats", []()->void {
				ProcessCommand("removehats " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 10, 80 }, "Steal Hats", []()->void {
				ProcessCommand("stealhats " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 10, 115 }, "Neon", []()->void {
				ProcessCommand("neon " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 10, 150 }, "Char Clone", []()->void {
				ProcessCommand("charclone " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 10, 185 }, "Goto", []()->void {
				ProcessCommand("tp me " + GetText());
			});
			thisBas->AddTextBox(hWnd, { 80, 25 }, { 10, 220 }, "16", "WS_BOX");

			//

			thisBas->AddButton(hWnd, { 80, 25 }, { 95, 10 }, "God", []()->void {
				ProcessCommand("god " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 95, 45 }, "Freeze", []()->void {
				ProcessCommand("freeze " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 95, 80 }, "Steal Gear", []()->void {
				ProcessCommand("stealgear " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 95, 115 }, "Unneon", []()->void {
				ProcessCommand("unneon " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 95, 150 }, "Remove Char", []()->void {
				ProcessCommand("removechar " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 95, 185 }, "Bring", []()->void {
				ProcessCommand("tp " + GetText() + " me");
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 95, 220 }, "Set WS", []()->void {
				std::string asd = thisBas->GetStringById("WS_BOX");
				ProcessCommand("ws " + GetText() + " " + asd);
			});

			//

			thisBas->AddButton(hWnd, { 80, 25 }, { 180, 10 }, "Btools", []()->void {
				ProcessCommand("btools " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 180, 45 }, "Unfreeze", []()->void {
				ProcessCommand("unfreeze " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 180, 80 }, "Shiny", []()->void {
				ProcessCommand("shiny " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 180, 115 }, "Invisible", []()->void {
				ProcessCommand("invis " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 180, 150 }, "Remove Legs", []()->void {
				ProcessCommand("rlegs " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 180, 185 }, "Trip (plat)", []()->void {
				ProcessCommand("plat " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 180, 220 }, "Lock FPS", []()->void {
				ProcessCommand("cmode " + GetText() + " 1");
			});

			//

			thisBas->AddButton(hWnd, { 80, 25 }, { 265, 10 }, "Big Head", []()->void {
				ProcessCommand("bighead " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 265, 45 }, "Jump", []()->void {
				ProcessCommand("jump " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 265, 80 }, "Unshiny", []()->void {
				ProcessCommand("unshiny " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 265, 115 }, "Visible", []()->void {
				ProcessCommand("vis " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 265, 150 }, "Remove Arms", []()->void {
				ProcessCommand("rarms " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 265, 185 }, "Confuse", []()->void {
				ProcessCommand("confuse " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 265, 220 }, "Unlock FPS", []()->void {
				ProcessCommand("cmode " + GetText() + " 0");
			});

			//

			thisBas->AddButton(hWnd, { 80, 25 }, { 350, 10 }, "Regular Head", []()->void {
				ProcessCommand("reghead " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 350, 45 }, "Kick", []()->void {
				ProcessCommand("kick " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 350, 80 }, "Naked", []()->void {
				ProcessCommand("naked " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 350, 115 }, "Block Head", []()->void {
				ProcessCommand("blockhead " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 350, 150 }, "Remove Limbs", []()->void {
				ProcessCommand("rlimbs " + GetText());
			});
			thisBas->AddButton(hWnd, { 80, 25 }, { 350, 185 }, "Ungod", []()->void {
				ProcessCommand("ungod " + GetText());
			});

			thisBas->hPlrTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("Enter Player Here"),
				WS_CHILD | WS_VISIBLE, 5, 323, 431,
				22, hWnd, NULL, NULL, NULL);
			thisBas->SetupFonts();
		}
						break;
		case WM_COMMAND:
			ID = LOWORD(wParam);
			for (auto i : thisBas->Buttons) {
				if (i.first == ID) {
					t = std::thread(*i.second.Click);
					t.detach();
				}
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
}