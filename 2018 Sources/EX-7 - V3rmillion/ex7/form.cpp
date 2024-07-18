/*
 ______  __   __           ______
|  ____| \ \ / /          |____  |
| |__     \ V /   ______      / /
|  __|     > <   |______|    / /
| |____   / . \             / /
|______| /_/ \_\           /_/


ok, so first things first, this code is ABSOLUTELY REVOLTING and I am DISGUSTED to even have to show you it
*/

#include "Includes/Includes.h"
#include "Memory/Memory.h"
#include "Functions.h"
#include "EX7.h"
#include "Serializer.h"
#include "Globals.h"

#pragma comment(lib, "lua5.1.lib")

using namespace func;
using namespace cLua;

lua_State* L;
DWORD rbx_L;

DWORD ScriptContext; 

#define SetThreadIdentity(rbx_L, Context) *(char*)(rbx_L - 0x14) = Context //set context level of thread ("rbx_L" being lua state and context being the context to elevate to)

void RestoreHackFlag() {
	/* they have a "hackflag" in affect to stop messing with the VM, we simple do our operation and then restore the hackflag back to its original state */
	DWORD Flag = ScanAddress(VM_HACKFLAG);
	__asm {
		mov ecx, Flag
		mov eax, 8
		mov[ecx], eax
	}
}

#define get_vm_key(rbx_L) decrypt_ptr((int)decrypt_ptr((int)(rbx_L + 8)) + 28) //ckey

void Compile_Script(DWORD rbx_L, char* Code) {
	RbxOpEncoder Encode;

	DWORD rbx_L_Thread = rbx_lua_newthread(rbx_L); //declare the thread
	string Source(Code); //for some reason do this??
	string Result = LuaSerializer::serialize(L, Source, Encode, get_vm_key(rbx_L_Thread)); //serialize the lua from the input of the exploit to roblox's bytecode using its own serializer
	Deserialize(rbx_L_Thread, Result, "", 1);  //deserialize the bytecode into a proto struct
	RestoreHackFlag(); //restore the "hackflag" back to its origianl state
	SetThreadIdentity(rbx_L_Thread, 7); //elevate context of thread to 7
	Spawn(rbx_L_Thread); //call spawn on the thread
}

void Execute_script(string Source) {
	L = luaL_newstate();

	rbx_L = GetGlobalState(ScriptContext, 0); //i retrieved the globalstate via the execute script function LOL

	string Script;
	Script = "spawn(function() local script = Instance.new('LocalScript', nil) script.Name = '666h' script.Disabled = true\r\n";
	Script += Source;
	Script += "\r\nend)"; //add creation of localscript to the start of the script

	Compile_Script(rbx_L, (char*)Script.c_str()); //compile/run the script

	lua_close(L);
}

std::string currentver = ROBLOXVERSION; //do this for some reason when i could just compare the macro??
std::string site = DownloadURL("http://setup.roblox.com/version");

void EX7::Print(COLORREF col, const char * format, ...) {
	/* hi austins code, i used to use this for ex-7 because i was a very thicc moron */
	char message[500];
	memset(message, 0, sizeof(message));
	va_list vl;
	va_start(vl, format);
	vsnprintf_s(message, 500, format, vl);
	va_end(vl);

	int len = SendMessage(OutputField, WM_GETTEXTLENGTH, NULL, NULL);
	SendMessage(OutputField, EM_SETSEL, len, len);

	CHARFORMAT cfd;
	CHARFORMAT cf;
	SendMessage(OutputField, EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cfd);
	memcpy(&cf, &cfd, sizeof(CHARFORMAT));

	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = col;
	cf.dwEffects = 0;

	SendMessage(OutputField, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	SendMessage(OutputField, EM_REPLACESEL, FALSE, (LPARAM)message);
	SendMessage(OutputField, WM_VSCROLL, SB_BOTTOM, 0L);
	SendMessage(OutputField, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cfd);
}

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{

			//"EX-7" Toolbar
		case UI_EXIT: {
			int exit = MessageBox(NULL, "Are you sure you want to close EX-7?\nRoblox will be also close.\n", "EX-7 | Exit", MB_YESNO);

			if (exit == IDYES)
			{
				system("taskkill /f /im RobloxPlayerBeta.exe");
				break;
			}

			else if (exit == IDNO)
			{
				break;
			}
		}

		case UI_ABOUT: {
			MessageBox(NULL, EX7_ABOUT, "EX-7 | About", MB_OK);
			break;
		}

					//"Web Utilities" Toolbar
		case HELPPAGE:
		{
			system("start http://timberman.xyz/azul/help.html");
			break;
		}

		case CHANGELOG:
		{
			system("start http://timberman.xyz/azul/changelog.html");
			break;
		}

		case LEARNLUA:
		{
			system("start http://tylerneylon.com/a/learn-lua/");
			break;
		}

		case FAQ:
		{
			system("start http://timberman.xyz/azul/faq.html");
			break;
		}

		/* - Buttons - */
		case EXECUTE: {
			int len = GetWindowTextLength(InputField) + 1;
			auto buffer = new char[len];
			GetWindowText(InputField, buffer, len);

			Execute_script(buffer);

			delete[] buffer;
			break;
		}

		case CLEAR: {
			SendMessage(InputField, WM_SETTEXT, (WPARAM)"", (LPARAM)"");
			break;
		}

		case SMAPS:
		{
			/*
			 * SMAP (Sparrow's Map) was a script that was meant to be created (and well was) by ProtectiveManEgg
			 * The Script was essientally in-game tablets that were very nicely made
			 * Due to a disagreement with PME, the development was halted and our friendship was also ended
			 */

			break;
		}

		case OPEN:
		{
			char FileName[260];

			OPENFILENAME ofn;
			ZeroMemory(&FileName, sizeof(FileName));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = MainWindow;
			ofn.lpstrFilter = "Lua Files\0*.lua\0Text Files\0*.txt\0Any File\0*.*\0";
			ofn.lpstrFile = FileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a File";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn))
			{
				SendMessage(InputField, WM_SETTEXT, (WPARAM)"", (LPARAM)"");
				std::string Line;
				std::ifstream s(FileName);
				if (s.is_open()) {
					while (getline(s, Line))
					{
						SendMessage(InputField, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)Line.c_str());
						SendMessage(InputField, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)"\n");
					}
				}
			}
			break;
		}
		}
		break;
	}

	case WM_DESTROY:
		system("taskkill /f /im RobloxPlayerBeta.exe");
		break;

	case WM_QUIT:
		system("taskkill /f /im RobloxPlayerBeta.exe");
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 1;
}

BOOL RegisterWindowClass(const char* wClassName) {
	WNDCLASSEX nClass;

	nClass.cbSize = sizeof(WNDCLASSEX);
	nClass.style = CS_DBLCLKS;
	nClass.lpfnWndProc = DLLWindowProc;
	nClass.cbClsExtra = 0;
	nClass.cbWndExtra = 0;
	nClass.hInstance = GetModuleHandle(0);
	nClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	nClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
	nClass.hCursor = LoadCursor(0, IDC_ARROW);
	nClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	nClass.lpszMenuName = "EX-7";
	nClass.lpszClassName = wClassName;

	if (!RegisterClassEx(&nClass))
		return 0;

	return 1;
}

LRESULT CALLBACK PatchedWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void HookRobloxWndProc() {
	SetWindowLongPtr(RobloxWindow, GWLP_WNDPROC, (LONG)PatchedWndProc);
}

void RestoreRobloxWndProc() {
	SetWindowLongPtr(RobloxWindow, GWLP_WNDPROC, RobloxWndProc);
}

void StopRobloxInput() {
	/* i forgot who actually made this but i think customality/quad core did 
	 * when you load in a dll and use any sort of text field, it will replicate to the game
	 * this basically stops that from happening
	 */

	RobloxWindow = FindWindowW(0, L"ROBLOX");
	RobloxWndProc = GetWindowLongPtr(RobloxWindow, GWLP_WNDPROC);
	while (1) {
		if (GetForegroundWindow() == MainWindow) {
			HookRobloxWndProc();
		}
		else {
			RestoreRobloxWndProc();
		}
		Sleep(80);
	}
}

void scan() {
	ex7->Print(RGB(0, 0, 0), "Starting Up...\n");
	ex7->Print(RGB(0, 0, 0), "Scanning... ");

	if (currentver != site) {
		ex7->Print(RGB(255, 0, 0), "\nRoblox update detected!\n"); //bad
	}

	else {
		DWORD ScriptContext_VFTable = ScanAddress(SCRIPTCONTEXT);
		ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&ScriptContext_VFTable, "xxxx");
		DataModel = getParent((DWORD)ScriptContext);

		ex7->Print(RGB(0, 150, 0), "OK!\n");

		EnableWindow(OpenButton, true);
		EnableWindow(ExecuteButton, true);
		EnableWindow(ClearButton, true);
		EnableWindow(InputField, true);
	}
}

void Whitelist() {
	/* its better not to even ask this is by far one of the shittest whitelists ive used */

	EnableWindow(OpenButton, false);
	EnableWindow(ExecuteButton, false);
	EnableWindow(ClearButton, false);
	EnableWindow(InputField, false);
	EnableWindow(SmapsButton, false);

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StopRobloxInput, 0, 0, 0);

	/*-- Begin main Authorization sequence --*/
	ex7->Print(RGB(0, 0, 0), "Authorizing User... ");
	Sleep(2000);
	std::string hwidhere = GetMachineID();
	std::string url = DownloadURL("https://timberman.xyz/ex7whitelist/Auth2.php?hwid=" + hwidhere);
	url = base64_decode(url);
	unsigned long long int url2;
	sscanf_s(url.c_str(), "%llu", &url2);
	unsigned long long int hwidhere2;
	sscanf_s(hwidhere.c_str(), "%llu", &hwidhere2);

	if (((url2 - 420) / 69) == hwidhere2) {
		ex7->Print(RGB(0, 150, 0), "OK!\n");
		scan();
	}

	else if (url == "User is not whitelisted") {
		ex7->Print(RGB(255, 0, 0), "\nERROR: User is not whitelisted!\n");
	}

	else if (url == "hwid is empty") {
		ex7->Print(RGB(255, 0, 0), "\nERROR: HardwareID function malfunctioned! Please retry.\n");
	}

	else {
		ex7->Print(RGB(255, 0, 0), "\nERROR: Connection to server failed!\n");
	}
}

void CreateToolbarMenu(HMENU hhMenu)
{
	ex7->hMenuPopupFile = CreatePopupMenu();
	AppendMenu(ex7->hMenuPopupFile, MF_STRING, UI_EXIT, TEXT("Exit"));
	InsertMenu(ex7->hMenuPopupFile, 1, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
	AppendMenu(ex7->hMenuPopupFile, MF_STRING, UI_ABOUT, TEXT("About"));
	SetMenuDefaultItem(ex7->hMenuPopupFile, 1, TRUE);
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)ex7->hMenuPopupFile, TEXT("EX-7"));

	ex7->hMenuPopupUtils = CreatePopupMenu();
	AppendMenu(ex7->hMenuPopupUtils, MF_STRING, LEARNLUA, TEXT("Learn Basic Lua"));
	AppendMenu(ex7->hMenuPopupUtils, MF_STRING, FAQ, TEXT("Frequently Asked Questions"));
	AppendMenu(ex7->hMenuPopupUtils, MF_STRING, CHANGELOG, TEXT("Changelog"));
	AppendMenu(ex7->hMenuPopupUtils, MF_STRING, HELPPAGE, TEXT("Environment/Help"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)ex7->hMenuPopupUtils, TEXT("Web-Utilities"));
}

HMENU CreateDLLWindowMenu()
{
	HMENU EX7Menu;
	EX7Menu = CreateMenu();

	if (EX7Menu == NULL)
		return FALSE;

	CreateToolbarMenu(EX7Menu);
	return EX7Menu;
}

DWORD WINAPI Main(LPVOID Param) {
	/* create winapi form */

	HFONT font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
	HFONT Font = CreateFontA(15, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Courier New"));

	LoadLibrary("Riched20.dll");

	if (!LoadLibrary("Riched20.dll")) {
		MessageBox(NULL, "A UI library failed to load!", "Error", MB_OK);
		throw std::exception("Failed to load dll");
	}

	HMENU WindowMenu = CreateDLLWindowMenu();
	UnregisterClass("_EX-7", HInstance);
	RegisterWindowClass("_EX-7");
	MainWindow = CreateWindowEx(NULL, "_EX-7", EX7_TITLE, WS_SYSMENU | WS_OVERLAPPEDWINDOW, UI_POSX, UI_POSY, UI_HEIGHT, UI_WIDTH, 0, WindowMenu, HInstance, 0);

	SetWindowPos(MainWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	OutputField = CreateWindowEx(NULL, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_READONLY | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_MULTILINE, 5, 254, 476, 79, MainWindow, NULL, HInstance, NULL);
	InputField = CreateWindowEx(NULL, RICHEDIT_CLASS, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_MULTILINE, 5, 12, 476, 197, MainWindow, NULL, HInstance, NULL);
	OpenButton = CreateWindowEx(NULL, "BUTTON", "Open", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 111, 219, 100, 29, MainWindow, (HMENU)OPEN, HInstance, NULL);
	ExecuteButton = CreateWindowEx(NULL, "BUTTON", "Execute", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 5, 219, 100, 29, MainWindow, (HMENU)EXECUTE, HInstance, NULL);
	ClearButton = CreateWindowEx(NULL, "BUTTON", "Clear", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 217, 219, 100, 29, MainWindow, (HMENU)CLEAR, HInstance, NULL);
	SmapsButton = CreateWindowEx(NULL, "BUTTON", "Sparrow's Map", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 381, 219, 100, 29, MainWindow, (HMENU)SMAPS, HInstance, NULL);

	SendMessage(InputField, WM_SETFONT, (WPARAM)Font, NULL);
	SendMessage(OutputField, WM_SETFONT, (WPARAM)Font, NULL);

	SendMessage(ExecuteButton, WM_SETFONT, (WPARAM)font, NULL);
	SendMessage(OpenButton, WM_SETFONT, (WPARAM)font, NULL);
	SendMessage(ClearButton, WM_SETFONT, (WPARAM)font, NULL);
	SendMessage(SmapsButton, WM_SETFONT, (WPARAM)font, NULL);

	ShowWindow(MainWindow, SW_SHOW);
	RemoveStyle(OutputField, WS_EX_CLIENTEDGE);
	RemoveStyle(InputField, WS_EX_CLIENTEDGE);

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Whitelist, 0, 0, 0);

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, Main, 0, 0, 0);
	}
	return 1;
}

/* Copyright (C) | Team [AzuL] | AzuLX & Quad Core - All Rights Reserved */

EX7 lex7;
EX7* ex7 = &lex7;