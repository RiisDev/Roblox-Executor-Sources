#pragma once
//#define _WIN32_WINNT  0x0501
#include <Windows.h>
#include <string>
#include <iostream>
#include <commctrl.h>
#include <ctime>
#include <cstdlib> 
#include <thread>
#include <sstream>
#include <Richedit.h>
#include <tchar.h>
#include <winuser.h>
#include <windowsx.h>
//#include <CRect.h>
//#include "LUA.h"
//#include "DoCommand.h"
//#include "Memcheck.h"
#include "BytecodeVM.h"
#include "DoCommand.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "comctl32.lib")

std::string XYZ_VERSION = "1.2.9";
std::string OLD_XYZ_VERSION = "5.9.8";

extern void HandleCommand(std::string command);

void HandleCommand(std::string command) {
	try {
//		std::thread run(&DoCommand, command);//Command->Func, vec);
//		run.detach();
		//DoCommand(command);
	}
	catch (std::exception& e) {
		MessageBoxA(0, e.what(), "", 0);
	}
	catch (...) {}
}

std::string replaceAll(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

#define WS_EX_LAYERED           0x00080000    

#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002

#define CHARLIMIT			10000 //0x7FFFFFFE
#define EXITGAME			(WM_APP + 599)
#define EXIT				(WM_APP + 600)

#define SELFGOD				(WM_APP + 601)
#define SELFUNGOD			(WM_APP + 602)
#define CMDS				(WM_APP + 603)
#define NOCLIP				(WM_APP + 604)
#define HIDENAME			(WM_APP + 605)
#define BTOOLS				(WM_APP + 606)
#define MINIMIZABLE			(WM_APP + 607)
#define ALWAYSONTOP			(WM_APP + 608)
#define SCRIPTEXEWINDOW		(WM_APP + 609)

#define EXECUTECODE         (WM_APP + 610)
#define SCRIPTTEXTBOX       (WM_APP + 611)
#define CLEARSCRIPT         (WM_APP + 612)

#define CLEARCONSOLE        (WM_APP + 613)
#define SMALLWINDOW         (WM_APP + 614)
#define FULLWINDOW          (WM_APP + 615)

#define DELETEONCLICK       (WM_APP + 616)
#define TELEPORTONCLICK     (WM_APP + 617)
#define ANCHORONCLICK		(WM_APP + 618)
#define CANCOLLIDEONCLICK	(WM_APP + 619)
#define LOCKEDONCLICK		(WM_APP + 620)
#define DISABLEONCLICK		(WM_APP + 621)

#define UI_SIZE_WIDTH			492 //460
#define UI_SIZE_HEIGHT			439 //330

HWND ScriptWindow;
HMENU hMenu;
HMENU hMenuPopupFile;
HMENU hMenuPopupAbout;
HWND ScriptTextBoxHWND;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HWND ParentWindow = NULL;
HWND MainWindow = NULL;
HWND TabControl = NULL;
HMENU WindowMenu = NULL;
HMODULE hInstance = NULL; // HMODULE

HWND InputField = NULL;
HWND ConsoleField = NULL;
HWND WaypointBox = NULL;
//////////////////////////


HMENU mainDropdown = NULL;
HMENU shortcutDropdown = NULL;
HMENU scriptexeDropdown = NULL;
HMENU windowDropdown = NULL;

bool DeleteOnClick = false;
bool TeleportOnClick = true;
bool AnchorOnClick = false;
bool CanCollideOnClick = false;
bool LockedOnClick = false;

HMODULE hInst;

/////////////////////////
BOOLEAN IsFullWindow = TRUE;

BOOLEAN DoesConsoleExist() {
	return GetConsoleWindow() != NULL;
}

void no_return() {
	return;
}

int DisableChecks() { // close it with: PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);
//	DetourTransactionBegin();
//	DetourUpdateThread(GetCurrentThread());
//	int PWR = (int)&FreeConsole;
//	DetourAttach(&(PVOID&)PWR, (PBYTE)no_return);
//	DetourTransactionCommit();
	return int(0);
}

void CreateConsoleAlternative() {
	DisableChecks();
	AllocConsole();
	std::string Template = VMProtectDecryptStringA("XYZv") + XYZ_VERSION + VMProtectDecryptStringA(" | by 1337IsBack & Variable | ThreadId: 1011");
	SetConsoleTitleA(Template.c_str());
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
}

extern void ClearConsoleOutput() {
	if (ConsoleField != NULL) {
		SendMessage((HWND)ConsoleField, WM_SETTEXT, NULL, (LPARAM)"");
	}
}

extern void ConsoleOutput(std::string ConsoleMessage, ...) {
	if (ConsoleMessage != "" && ConsoleField != NULL) {
		ConsoleMessage = ConsoleMessage + "\n";
		const char * format = ConsoleMessage.c_str();
		char message[CHARLIMIT];
		va_list vl;
		va_start(vl, format);
		vsnprintf_s(message, CHARLIMIT, format, vl);
		va_end(vl);

		int TextLength = GetWindowTextLength(ConsoleField);
		SendMessage(ConsoleField, EM_SETSEL, (WPARAM)TextLength, (LPARAM)TextLength);//-1, 0);
		SendMessage(ConsoleField, EM_REPLACESEL, FALSE, (LPARAM)message);
	}
	else if (ConsoleMessage != "" && ConsoleField == NULL) {
		if (!DoesConsoleExist()) {
			CreateConsoleAlternative();
		}
		std::cout << ConsoleMessage;
	}
}

void CloseOutXYZ() {
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h != INVALID_HANDLE_VALUE) {
		THREADENTRY32 te;
		te.dwSize = sizeof(te);
		if (Thread32First(h, &te)) {
			do {
				if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID)) {
					if (te.th32ThreadID != GetCurrentThreadId() && te.th32OwnerProcessID == GetCurrentProcessId()) {
						HANDLE thread = ::OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
						if (thread != NULL) {
							TerminateThread(thread, 0);
							CloseHandle(thread);
						}
					}
				}
				te.dwSize = sizeof(te);
			} while (Thread32Next(h, &te));
		}
		CloseHandle(h);
	}
}

void CreateFWindows() {
	if (ScriptWindow != NULL) {
		HFONT wordFont = CreateFont(18, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
		HFONT labelFont = CreateFont(25, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_EMBEDDED, PROOF_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));

		HWND executeButton = CreateWindow("BUTTON", "EXECUTE", WS_CHILD | WS_VISIBLE, 0, 235, 200, 36, ScriptWindow, (HMENU)EXECUTECODE, hInstance, NULL);
		HWND clearButton = CreateWindow("BUTTON", "CLEAR", WS_CHILD | WS_VISIBLE, 200, 235, 200, 36, ScriptWindow, (HMENU)CLEARSCRIPT, hInstance, NULL);
		ScriptTextBoxHWND = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_BORDER | WS_VSCROLL | WS_HSCROLL, 0, 0, 395, 235, ScriptWindow, (HMENU)SCRIPTTEXTBOX, hInstance, 0);
		SendMessage(ScriptTextBoxHWND, EM_SETLIMITTEXT, 0x7FFFFFFE, 0);

		SendMessage(ScriptTextBoxHWND, WM_SETFONT, (WPARAM)wordFont, MAKELPARAM(TRUE, 0));
		SendMessage(executeButton, WM_SETFONT, (WPARAM)labelFont, MAKELPARAM(TRUE, 0));
		SendMessage(clearButton, WM_SETFONT, (WPARAM)labelFont, MAKELPARAM(TRUE, 0));
	}
}
#define toproto(L,i) (clvalue(L->top+(i))->l.p)

static int writer(lua_State *L, const void *p, size_t size, void *u) {
	SetupLuaState(L);
	return 0;
}


static void compile2(lua_State *L, char *command) {

	const Proto* f;

	if (luaL_loadstring(L, command) != 0) {
		ConsoleOutput("%s\n", lua_tostring(L, -1));
	}

	f = toproto(L, -1);

	lua_lock(L);

	
	luaU_dump(L, f, writer, NULL, 1);

	lua_unlock(L);
}


extern void ExitXYZ() {
	if (DoesConsoleExist() == TRUE) {
		::CloseWindow(GetConsoleWindow());
	}
	HMODULE XYZ_Module = GetModuleHandle(NULL);
	FreeLibraryAndExitThread(XYZ_Module, 0);
}

extern void ExitGameAndXYZ() {
	CloseOutXYZ();
	//ExitThread(0);
	//TerminateProcess(GetModuleHandle(NULL), 0);
}

HMENU xhPopupMenu = NULL;
int xPos = 0;
int yPos = 0;
#define WM_CONTEXTMENU                  0x007B

HWND okaaa;
LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CTLCOLORSTATIC: {
		HDC hEdit = (HDC)wParam;
		SetTextColor(hEdit, RGB(0, 0, 0));
		SetBkColor(hEdit, RGB(255, 255, 255)); // 255, 255, 255
		return (LRESULT)GetStockObject(RGB(34, 139, 34));//WHITE_BRUSH);
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case EXIT:
			ExitXYZ();
			//ExitThread(0);
			break;
		case EXITGAME:
			ExitGameAndXYZ();
			//ExitThread(0);
			break;
		case SMALLWINDOW:
			if (IsFullWindow != FALSE) {
				IsFullWindow = FALSE;
				ShowWindow(ConsoleField, SW_HIDE);
				SetWindowPos(InputField, NULL, 10, 5, UI_SIZE_WIDTH, 85, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);
				SetWindowPos(MainWindow, NULL, 0, 0, UI_SIZE_WIDTH, 85, SWP_DRAWFRAME | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOZORDER);
				ShowWindow(MainWindow, SW_NORMAL);
			}
			break;
		case FULLWINDOW:
			if (IsFullWindow != TRUE) {
				IsFullWindow = TRUE;
				SetWindowPos(InputField, NULL, 10, 216, UI_SIZE_WIDTH, 85, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);
				SetWindowPos(MainWindow, NULL, 0, 0, UI_SIZE_WIDTH, UI_SIZE_HEIGHT, SWP_DRAWFRAME | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOZORDER);
				ShowWindow(ConsoleField, SW_SHOW);
				ShowWindow(MainWindow, SW_NORMAL);
			}
			break;
		case MINIMIZABLE:
			SetWindowPos(MainWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			ShowWindow(MainWindow, SW_NORMAL);
			break;
		case ALWAYSONTOP:
			SetWindowPos(MainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			ShowWindow(MainWindow, SW_NORMAL);
			break;
		case CLEARCONSOLE:
			SendMessage((HWND)ConsoleField, WM_SETTEXT, NULL, (LPARAM)"");
			break;
		case SCRIPTEXEWINDOW:
			if (ScriptWindow == NULL) {
				ScriptWindow = CreateWindowEx(WS_EX_TOPMOST, "XWIN", "", WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, hMenu, hInstance, NULL);
				CreateFWindows();
				ShowWindow(ScriptWindow, SW_NORMAL);
			}
			else if (IsWindowVisible(ScriptWindow)) {
				ShowWindow(ScriptWindow, SW_HIDE);
			}
			else {
				ShowWindow(ScriptWindow, SW_SHOW);
			}
			break;
		case SELFGOD:
			HandleCommand("god me");
			break;
		case SELFUNGOD:
			HandleCommand("ungod me");
			break;
		case NOCLIP:
			HandleCommand("noclip");
			break;
		case HIDENAME:
			HandleCommand("hidename me");
			break;
		case BTOOLS:
			HandleCommand("btools me");
			break;
		case DELETEONCLICK:
			TeleportOnClick = false;
			AnchorOnClick = false;
			CanCollideOnClick = false;
			LockedOnClick = false;
			DeleteOnClick = true;
			break;
		case TELEPORTONCLICK:
			DeleteOnClick = false;
			AnchorOnClick = false;
			CanCollideOnClick = false;
			LockedOnClick = false;
			TeleportOnClick = true;
			break;
		case ANCHORONCLICK:
			DeleteOnClick = false;
			TeleportOnClick = false;
			CanCollideOnClick = false;
			LockedOnClick = false;
			AnchorOnClick = true;
			break;
		case CANCOLLIDEONCLICK:
			DeleteOnClick = false;
			TeleportOnClick = false;
			AnchorOnClick = false;
			LockedOnClick = false;
			CanCollideOnClick = true;
			break;
		case LOCKEDONCLICK:
			DeleteOnClick = false;
			TeleportOnClick = false;
			AnchorOnClick = false;
			CanCollideOnClick = false;
			LockedOnClick = true;
			break;
		case DISABLEONCLICK:
			DeleteOnClick = false;
			TeleportOnClick = false;
			AnchorOnClick = false;
			CanCollideOnClick = false;
			LockedOnClick = false;
			break;
		case CMDS:
			//MessageBox(hwnd, Commands, "Commands", MB_OK);
//			ConsoleOutput(std::string(Commands));
			break;
		case CLEARSCRIPT:
			SetDlgItemText(hwnd, SCRIPTTEXTBOX, "");
			break;
		case EXECUTECODE:
			int length;
			length = SendMessage(ScriptTextBoxHWND, WM_GETTEXTLENGTH, 0, 0);
			if (length == -1)
				break;
			char buff[18896];
			char len[255];
			_itoa_s(length, len, 10);
			if (ScriptStateHolder != 0) {
				luaState = ScriptStateHolder;
			}
			ScriptStateHolder = rbxlua_newthread(luaState);
			GetDlgItemText(hwnd, SCRIPTTEXTBOX, buff, length + 1); //clearStack();
			ExecuteScript(buff);
			break;
		}
	case (WM_APP + 501) :
		if (HIWORD(wParam) == EN_MAXTEXT) {
			char cText[CHARLIMIT];
			SendMessage((HWND)lParam, WM_GETTEXT, CHARLIMIT, (LPARAM)cText); Sleep(10);
			if (strcmp(cText, "") == 0)
				break;

			SendMessage((HWND)lParam, WM_SETTEXT, NULL, (LPARAM)"");
			//std::string wot = *(std::string*)(NetworkClient + 0x0AC8);
			//	ConsoleOutput(wot.c_str(), TRUE);
			std::string outputmsg = std::string("> ") +  std::string(cText) + std::string("\r\n");
			ConsoleOutput(outputmsg);
			//ConsoleOutput(cText);
			DoCommand(cText); Sleep(100);//DoCommand(cText); Sleep(100);
											 //ConsoleOutput("\n");
		}
		break;
	/*case WM_CONTEXTMENU://case WM_RBUTTONDOWN:
		ConsoleOutput("rbutton\r\n");
		xhPopupMenu = CreatePopupMenu();
		InsertMenu(xhPopupMenu, 0, MF_BYPOSITION | MF_STRING, EXITGAME, _T("Exit"));
		InsertMenu(xhPopupMenu, 0, MF_BYPOSITION | MF_STRING, EXITGAME, _T("Play"));
		SetForegroundWindow(hwnd);
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		TrackPopupMenu(xhPopupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, xPos, yPos, 0, hwnd, NULL);
		break;/**/
	/*case WM_KEYDOWN:
		if (GetKeyState(VK_CONTROL) & 0x8000 && wParam == 'A') {
			SendMessage(hwnd, EM_SETSEL, 0, -1);
			ConsoleOutput("yep");
		}
		break;*/
	case WM_CLOSE:
		//PostQuitMessage(0);
		ExitXYZ();
		break;
	case WM_DESTROY:
		ExitGameAndXYZ();
		break;
	case WM_QUIT:
		//ExitThread(0);
		ExitGameAndXYZ();
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL RegisterWindowClass(const char* wClassName) {
	WNDCLASSEX nClass;

	nClass.cbSize = sizeof(WNDCLASSEX);
	nClass.style = CS_HREDRAW | CS_VREDRAW;//nClass.style = CS_DBLCLKS;
	nClass.lpfnWndProc = &DLLWindowProc;
	nClass.cbClsExtra = 0;
	nClass.cbWndExtra = 0;
	nClass.hInstance = hInstance;//GetModuleHandle(NULL);
	nClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));//LoadIcon(NULL, IDI_APPLICATION);
	nClass.hIconSm = LoadIcon(nClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));//LoadIcon(NULL, IDI_APPLICATION);
	nClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	nClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//CreateSolidBrush(0x000000ff);//(HBRUSH)GetStockObject(LTGRAY_BRUSH);//(HBRUSH)(COLOR_WINDOW);//(HBRUSH)GetStockObject(LTGRAY_BRUSH);
	nClass.lpszMenuName = ""; // ya
	nClass.lpszClassName = wClassName;

	if (!RegisterClassEx(&nClass))
		UnregisterClass(wClassName, hInstance);
	RegisterClassEx(&nClass);
	return 0;

	return 1;
}

BOOL StartMessageLoop() {
	MSG Message;
	BOOL bRet;
	POINT mousePos;
	HWND hwnd;

	while ((bRet = GetMessage(&Message, NULL, 0, 0)) != 0) {
		if (bRet == -1 || bRet == 0) {
			return 1;
		}
		hwnd = Message.hwnd;
		//if (BreakWindow) break;
		//if (Message.message == WM_KEYDOWN && Message.wParam == VK_LBUTTON && GetAsyncKeyState(VK_CONTROL)) {//&& GetFocus() == InputField) {
		//	ConsoleOutput("ya\r\n");
		//	continue;
		//}
		//else if (Message.message == WM_KEYDOWN && Message.wParam == VK_RETURN && GetFocus() == InputField) {
		//	SendMessage(InputField, EM_SETSEL, 0, -1);
		//}
		
		/*if (Message.wParam == VK_RBUTTON && Message.hwnd == ConsoleField) {
			if (xhPopupMenu == NULL) {
				xhPopupMenu = CreatePopupMenu();
				InsertMenu(xhPopupMenu, 0, MF_BYPOSITION | MF_STRING, CLEARCONSOLE, _T("Clear Console"));
				//InsertMenu(xhPopupMenu, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, CLEARCONSOLE, "Clear Console");
			}
			GetCursorPos(&mousePos);
			TrackPopupMenu(xhPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, mousePos.x, mousePos.y, 0, Message.hwnd, NULL);
			continue;
		}*/
		if (hwnd == ConsoleField || hwnd == InputField || hwnd == ScriptTextBoxHWND) {
			if (Message.message == WM_KEYDOWN) {
				if (GetKeyState(VK_CONTROL) & 0x8000 && Message.wParam == 'A') {
					SendMessage(hwnd, EM_SETSEL, 0, -1);
					continue;
				}
			}
		}

		//SendMessage(InputField, EM_HIDESELECTION, 1, 0);

		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return 0;
}

BOOL CreateSubwindows() {
	//0,37
	ConsoleField = CreateWindowEx(WS_EX_STATICEDGE, "EDIT", "", WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL, 12, 12, 468, 310, MainWindow, (HMENU)(WM_APP + 501), hInstance, 0);
	InputField = CreateWindowEx(WS_EX_WINDOWEDGE, "EDIT", "", WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VISIBLE, 12, 347, 468, 36, MainWindow, (HMENU)(WM_APP + 501), hInstance, 0);
	//10,214
	SendMessage(InputField, EM_SETLIMITTEXT, 0x7FFFFFFE, NULL);

	HFONT textFont1 = CreateFont(18, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_EMBEDDED, PROOF_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));
	HFONT textFont2 = CreateFont(20, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_EMBEDDED, PROOF_QUALITY, VARIABLE_PITCH, TEXT("Segoe UI"));

	//HWND inputFieldLabel = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE, 10, 245, 100, 20, MainWindow, NULL, hInstance, NULL);
	//SendMessage(inputFieldLabel, WM_SETTEXT, NULL, (LPARAM)"Input");

	//SendMessage(inputFieldLabel, WM_SETFONT, (WPARAM)textFont, MAKELPARAM(TRUE, 0));
	SendMessage(InputField, WM_SETFONT, (WPARAM)textFont2, MAKELPARAM(TRUE, 0));
	SendMessage(ConsoleField, WM_SETFONT, (WPARAM)textFont1, MAKELPARAM(TRUE, 0));
	SendMessage(ConsoleField, EM_SETLIMITTEXT, 0x7FFFFFFE, 0);

	if (LocalPlayer != 0) {
		ConsoleOutput(std::string("Welcome, ") + *GetName(LocalPlayer) + std::string("!\r\n"));
	}
	//ConsoleOutput("TEMPORARY FREE TRIAL VERSION OF XYZ! :)"); ConsoleOutput("\n");
	const char* Message = " - A unknown error has occured!\n\n";
	if (Workspace != 0) {
		BYTE _FilteringEnabled = FilteringEnabledCheck(Workspace);
		if (_FilteringEnabled == 1) {
			Message = "Filtering is ENABLED! Most commands will not work properly!\n\n";
		}
		else if (_FilteringEnabled == 0) {
			Message = "Filtering is disabled :)!\n\n";
		}
	}
	ConsoleOutput((std::string)Message);
	UpdateWindow(MainWindow);
	return 1;
}

std::string GetFilePath() {
	char Buffer[2048];
	GetModuleFileName(hInst, Buffer, sizeof(Buffer));
	return std::string(Buffer);
}

std::vector<std::string> getFiles(std::string folder) {
	std::vector<std::string> names;
	char search_path[200];
	sprintf(search_path, "%s/*.*", folder.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

void AutoExecuteScripts() {
	std::string FilePath = GetFilePath();
	std::string Directory;
	if (FilePath.find_last_of('\\') != std::string::npos) {
		Directory = FilePath.substr(0, FilePath.find_last_of('\\'));
	}

	std::vector<std::string> files = getFiles(Directory + "\\autoexec");

	if (files.size() != 0) {
		for (int c = 0; c < files.size(); c++) {
			std::ifstream File;
			long FileLength;
			File.open(Directory + "\\autoexec\\" + files[c], std::ios::binary | std::ios::in);
			File.seekg(0, std::ios::end);
			FileLength = File.tellg();
			File.seekg(0, std::ios::beg);

			char* Data = new char[FileLength];
			File.read(Data, FileLength);

			std::string Source = std::string(Data, FileLength);

//			RunLua(Source);
		}
	}
}

BOOL CreateWindowMenu() {
	WindowMenu = CreateMenu();
	if (!WindowMenu) {
		MessageBoxA(0, "Windows menu wasn't able to be created, try again. :(", "XYZ", 0);
		return 0;
	}

	mainDropdown = CreatePopupMenu();
	HMENU ExitOptions = CreatePopupMenu();
	AppendMenu(mainDropdown, MF_STRING, CMDS, "Commands");
	AppendMenu(mainDropdown, MF_STRING | MF_POPUP, (UINT_PTR)ExitOptions, "Exit");
	AppendMenu(ExitOptions, MF_STRING, EXIT, "Halcyon");
	//AppendMenu(ExitOptions, MF_STRING, EXITGAME, "Game + XYZ");
	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)mainDropdown, "Menu");

	shortcutDropdown = CreatePopupMenu();
	HMENU LocalPlayerOptions = CreatePopupMenu();
	HMENU ClickDropdown = CreatePopupMenu();
	HMENU ClickOptions = CreatePopupMenu();
	//HMENU ServerOptions = CreatePopupMenu();
	AppendMenu(shortcutDropdown, MF_STRING | MF_POPUP, (UINT_PTR)LocalPlayerOptions, "LocalPlayer");
	AppendMenu(LocalPlayerOptions, MF_STRING, SELFGOD, "god");
	AppendMenu(LocalPlayerOptions, MF_STRING, SELFUNGOD, "ungod");
	AppendMenu(LocalPlayerOptions, MF_STRING, NOCLIP, "noclip");
	AppendMenu(LocalPlayerOptions, MF_STRING | MF_MENUBREAK, HIDENAME, "hidename");
	AppendMenu(LocalPlayerOptions, MF_STRING, BTOOLS, "btools");
	//AppendMenu(windowDropdown, MF_STRING | MF_POPUP, (UINT_PTR)ServerOptions, "Server");

	AppendMenu(shortcutDropdown, MF_STRING | MF_POPUP, (UINT_PTR)ClickOptions, "CTRL+CLICK");
	AppendMenu(ClickOptions, MF_STRING, TELEPORTONCLICK, "Teleport");
	AppendMenu(ClickOptions, MF_STRING, DELETEONCLICK, "Delete");
	AppendMenu(ClickOptions, MF_STRING, ANCHORONCLICK, "Toggle Anchor");
	AppendMenu(ClickOptions, MF_STRING, CANCOLLIDEONCLICK, "Toggle CanCollide");
	AppendMenu(ClickOptions, MF_STRING, LOCKEDONCLICK, "Toggle Locked");
	AppendMenu(ClickOptions, MF_STRING, DISABLEONCLICK, "Disable CTRL+CLICK");

	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)shortcutDropdown, "Shortcut");

	windowDropdown = CreatePopupMenu();
	HMENU ConsoleOptions = CreatePopupMenu();
	HMENU DisplayOptions = CreatePopupMenu();
	AppendMenu(windowDropdown, MF_STRING | MF_POPUP, (UINT_PTR)ConsoleOptions, "Console");
	AppendMenu(ConsoleOptions, MF_STRING, CLEARCONSOLE, "Clear Console");
	AppendMenu(ConsoleOptions, MF_STRING, SMALLWINDOW, "Small Console");
	AppendMenu(ConsoleOptions, MF_STRING, FULLWINDOW, "Full Console");
	AppendMenu(windowDropdown, MF_STRING | MF_POPUP, (UINT_PTR)DisplayOptions, "Display");
	AppendMenu(DisplayOptions, MF_STRING, ALWAYSONTOP, "Always on top");
	AppendMenu(DisplayOptions, MF_STRING, MINIMIZABLE, "Minimizable");
	AppendMenu(WindowMenu, MF_POPUP, (UINT_PTR)windowDropdown, "Window");

	scriptexeDropdown = CreatePopupMenu();
	//AppendMenu(scriptexeDropdown, MF_STRING, SCRIPTEXEWINDOW, "Large Script Window");
	AppendMenu(WindowMenu, MF_POPUP, SCRIPTEXEWINDOW, "Beta Script Execution");

	return 1;
}

typedef DWORD(WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);

static PSLWA pSetLayeredWindowAttributes = NULL;
static BOOL initialized = FALSE;

BOOL InitiateWindow() {
	//::CloseWindow(GetConsoleWindow()); // FreeConsole();
	hInstance = GetModuleHandle(NULL);

	UnregisterClass("XWIN", hInstance);
	RegisterWindowClass("XWIN");

	if (!CreateWindowMenu())
		return 0;
	std::string WindowTitle = "Halcyonv" + XYZ_VERSION + " | by Jayden";
	if (!(MainWindow = CreateWindowExA(
		WS_EX_TOPMOST | WS_EX_APPWINDOW | WS_EX_COMPOSITED,//NULL,
		"XWIN",
		WindowTitle.c_str(),
		WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION | CW_USEDEFAULT | WS_OVERLAPPED,//NULL, WS_SYSMENU
		200, //1680
		200,
		UI_SIZE_WIDTH,
		UI_SIZE_HEIGHT,
		NULL, //ParentWindow,
		WindowMenu,
		hInstance,
		NULL))) return 0;

	ScrollWindowEx(MainWindow, 0, 560, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN | SW_SMOOTHSCROLL);
	EnableScrollBar(MainWindow, SB_VERT, ESB_ENABLE_BOTH);

	CreateSubwindows();
	
	//HWND hMenuWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_NOACTIVATE, "MyCustomMenu", NULL, WS_POPUP | WS_BORDER, 0, 0, 1, 1, MainWindow, 0, hInstance, 0);

	ShowWindow(MainWindow, SW_SHOWNORMAL);
	UpdateWindow(MainWindow);

	//AutoExecuteScripts();

	return StartMessageLoop();
}









