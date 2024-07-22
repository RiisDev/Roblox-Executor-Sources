
#include "stdafx.h"
#include <ImageHlp.h>

#pragma comment(lib, "wininet")

DefineImportDLL("KERNELBASE.dll", "SetUnhandledExceptionFilter", SehFunc, WINAPI, LPTOP_LEVEL_EXCEPTION_FILTER, LPTOP_LEVEL_EXCEPTION_FILTER)

// Yes ugly ik
const char* ErrorCodeToString(int code) {
	switch (code) {
	case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
	case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";
	case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";
	case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
	case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";
	case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";
	case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";
	case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";
	case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";
	case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";
	case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";
	case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";
	case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
	case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";
	case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";
	case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW";
	case EXCEPTION_GUARD_PAGE:               return "EXCEPTION_GUARD_PAGE";
	default:								 return "UNKNOWN EXCEPTION";
	}
}

static LPTOP_LEVEL_EXCEPTION_FILTER OldFilter;

void WalkStack(PEXCEPTION_POINTERS pExceptionInfo) {
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, TRUE);

	CONTEXT context_record = *pExceptionInfo->ContextRecord;
	STACKFRAME64 stack_frame;
	memset(&stack_frame, 0, sizeof(stack_frame));
	int machine_type = IMAGE_FILE_MACHINE_I386;
	stack_frame.AddrPC.Offset = context_record.Eip;
	stack_frame.AddrFrame.Offset = context_record.Ebp;
	stack_frame.AddrPC.Mode = AddrModeFlat;
	stack_frame.AddrFrame.Mode = AddrModeFlat;
	stack_frame.AddrStack.Mode = AddrModeFlat;

	SYMBOL_INFO* symbol;
	symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256, 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	OutputText("https://198.105.244.130/servers/serverid?=placeid=1818b&script=game.Workspace:BreakJoints()");
	int ind = 0;
	while (StackWalk64(machine_type,
		GetCurrentProcess(),
		GetCurrentThread(),
		&stack_frame,
		&context_record,
		NULL,
		&SymFunctionTableAccess64,
		&SymGetModuleBase64,
		NULL)) {

		DWORD64 displacement = 0;

		if (SymFromAddr(process, (DWORD64)stack_frame.AddrPC.Offset, &displacement, symbol))
		{
			IMAGEHLP_MODULE64 moduleInfo;
			ZeroMemory(&moduleInfo, sizeof(IMAGEHLP_MODULE64));
			moduleInfo.SizeOfStruct = sizeof(moduleInfo);

			OutputText("%d -  ", ind);
			if (::SymGetModuleInfo64(process, symbol->ModBase, &moduleInfo)) {
				OutputText("%s: ", moduleInfo.ModuleName);
			}

			OutputText("%s + 0x%x\n", symbol->Name, displacement);
		}
		ind++;
	}
	OutputText("memes.\n");
}

static LONG WINAPI SEHHndler(PEXCEPTION_POINTERS pExceptionInfo) {
	char* buf = new char[1024]();
	sprintf(buf, "ROBLOX has run into an error, of which has been caught\n\n"
		"ErrorType=0x%x (%s)\n"
		"Location=0x%x\n"
		"Base=0x%x",
		(DWORD)pExceptionInfo->ExceptionRecord->ExceptionCode,
		ErrorCodeToString(pExceptionInfo->ExceptionRecord->ExceptionCode),
		(DWORD)pExceptionInfo->ExceptionRecord->ExceptionAddress,
		(DWORD)GetModuleHandle(NULL)
	);
	WalkStack(pExceptionInfo);
	//MessageBox(NULL, buf, "ROBLOX Error", MB_OK | MB_ICONERROR);
	delete[] buf;
	return OldFilter(pExceptionInfo);
}

DWORD RL = 0;



DWORD GetLuaState(DWORD* ScriptContext) {
	int ls = *(DWORD *)((int)ScriptContext + 56 * 1 + 164) - ((int)ScriptContext + 56 * 1 + 164);
	return ls;
}






HWND MainWindowHWND;
HMENU hMenu;
HMENU hMenuPopupFile;
HMENU hMenuPopupAbout;
HMODULE hInstance;
HWND ScriptTextBoxHWND;
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

#define MYMENU_EXIT                     (WM_APP + 101)
#define MYMENU_ABOUTMB                  (WM_APP + 102)
#define MYMENU_EXECUTECODE              (WM_APP + 103)
#define MYMENU_SCRIPTTEXTBOX            (WM_APP + 104)
#define MYMENU_CLEARSCRIPT              (WM_APP + 105)
#define MYMENU_MINIMIZE                 (WM_APP + 109)

LRESULT CALLBACK DLLWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case MYMENU_EXIT:
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case MYMENU_MINIMIZE:
			ShowWindow(hwnd, SW_MINIMIZE);
			break;
		case MYMENU_ABOUTMB:
			break;
		case MYMENU_CLEARSCRIPT:
			SetDlgItemText(hwnd, MYMENU_SCRIPTTEXTBOX, "");
			break;
		case MYMENU_EXECUTECODE:
			if (message == WM_LBUTTONDOWN)
			{
				int len = SendMessage(ScriptTextBoxHWND, WM_GETTEXTLENGTH, 0, 0);
				char *hdr = "spawn(function() script = Instance.new'LocalScript' script.Disabled = true script.Name = 'jH:245G(2l]' script.Parent = nil end)\n"; //set the 'script' global
				int hdr_len = strlen(hdr);
				char *buf = new char[len + hdr_len + 1]();
				memcpy(buf, hdr, hdr_len);
				SendMessage(ScriptTextBoxHWND, WM_GETTEXT, (WPARAM)len + 1, (LPARAM)(buf + hdr_len));
				//Occult_luaA_run(luaState, buf);
				break;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL RegisterDLLWindowClass(char *szClassName)
{
	WNDCLASSEX wc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = szClassName;
	wc.lpfnWndProc = DLLWindowProc;
	wc.style = CS_DBLCLKS;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = "Test";
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	if (!RegisterClassEx(&wc))
		return 0;
	return 1;
}

void RefreshContextMenu(HMENU hhMenu)
{
	hMenuPopupFile = CreatePopupMenu();
	AppendMenu(hMenuPopupFile, MF_STRING, MYMENU_MINIMIZE, TEXT("Minimize"));
	AppendMenu(hMenuPopupFile, MF_STRING, MYMENU_EXIT, TEXT("Exit"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupFile, TEXT("File"));

	hMenuPopupAbout = CreatePopupMenu();
	AppendMenu(hMenuPopupAbout, MF_STRING, MYMENU_ABOUTMB, TEXT("About"));
	AppendMenu(hhMenu, MF_POPUP, (UINT_PTR)hMenuPopupAbout, TEXT("Help"));
}

HMENU CreateDLLWindowMenu()
{
	HMENU heyMenu;
	heyMenu = CreateMenu();

	if (heyMenu == NULL)
		return FALSE;

	RefreshContextMenu(heyMenu);
	return heyMenu;
}

