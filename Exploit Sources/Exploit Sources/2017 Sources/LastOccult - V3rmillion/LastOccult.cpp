
#include "stdafx.h"
#include "OccultLua/OccultVM.h"
#include "Utilities/Scan.h"
#include <ImageHlp.h>

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

	OutputText("Walking the stack...\n");
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
	OutputText("Finished.\n");
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
	MessageBox(NULL, buf, "ROBLOX Error", MB_OK | MB_ICONERROR);
	delete[] buf;
	return OldFilter(pExceptionInfo);
}

DWORD RL = 0;

VOID Main()
{
	OldFilter = SehFunc(SEHHndler);

	CreateUI([]() {
		DWORD base = (DWORD)GetModuleHandleA(NULL);
		DWORD SCVT = base + 0x117EF8C - OCCULT_IDABASE;
		OutputText("Loading... ");
		INT SC = (DWORD)(Memory::Scan(PAGE_READWRITE, (char*)&SCVT, "xxxx", 0x3700000));
		RL = ((DWORD(__thiscall*)(INT, int))(base + OCCULT_STATE_GRABBER - OCCULT_IDABASE))(SC, 2);
		OutputText(RGB(0, 255, 0), "Done!\n");
		OutputText("Script Context:%x\nLua State:%x\n", SC, RL);
	});
	TerminateProcess(GetCurrentProcess(), 1);
}

