#include "exceptionfilter.h"

char errstr[128];
const char* exceptionCodeToString(DWORD exceptionCode) {
    const char* str;
    switch (exceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
        str = XorString("EXCEPTION_ACCESS_VIOLATION");
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        str = XorString("EXCEPTION_ARRAY_BOUNDS_EXCEEDED");
    case EXCEPTION_BREAKPOINT:
        str = XorString("EXCEPTION_BREAKPOINT");
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        str = XorString("EXCEPTION_DATATYPE_MISALIGNMENT");
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        str = XorString("EXCEPTION_FLT_DENORMAL_OPERAND");
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        str = XorString("EXCEPTION_FLT_DIVIDE_BY_ZERO");
    case EXCEPTION_FLT_INEXACT_RESULT:
        str = XorString("EXCEPTION_FLT_INEXACT_RESULT");
    case EXCEPTION_FLT_INVALID_OPERATION:
        str = XorString("EXCEPTION_FLT_INVALID_OPERATION");
    case EXCEPTION_FLT_OVERFLOW:
        str = XorString("EXCEPTION_FLT_OVERFLOW");
    case EXCEPTION_FLT_STACK_CHECK:
        str = XorString("EXCEPTION_FLT_STACK_CHECK");
    case EXCEPTION_FLT_UNDERFLOW:
        str = XorString("EXCEPTION_FLT_UNDERFLOW");
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        str = XorString("EXCEPTION_ILLEGAL_INSTRUCTION");
    case EXCEPTION_IN_PAGE_ERROR:
        str = XorString("EXCEPTION_IN_PAGE_ERROR");
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        str = XorString("EXCEPTION_INT_DIVIDE_BY_ZERO");
    case EXCEPTION_INT_OVERFLOW:
        str = XorString("EXCEPTION_INT_OVERFLOW");
    case EXCEPTION_INVALID_DISPOSITION:
        str = XorString("EXCEPTION_INVALID_DISPOSITION");
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        str = XorString("EXCEPTION_NONCONTINUABLE_EXCEPTION");
    case EXCEPTION_PRIV_INSTRUCTION:
        str = XorString("EXCEPTION_PRIV_INSTRUCTION");
    case EXCEPTION_SINGLE_STEP:
        str = XorString("EXCEPTION_SINGLE_STEP");
    case EXCEPTION_STACK_OVERFLOW:
        str = XorString("EXCEPTION_STACK_OVERFLOW");
    default:
        std::string* code = new std::string();
        *code = std::to_string(exceptionCode);
        str = code->c_str();
    }
    memcpy(errstr, new char[_countof(errstr)], _countof(errstr));
    memcpy(errstr, str, strlen(str));
    return errstr;
}

LONG WINAPI TaaprWareExceptionFilter(EXCEPTION_POINTERS* ExceptionInfo) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    EXCEPTION_RECORD* ExceptionRecord = ExceptionInfo->ExceptionRecord;
    CONTEXT* context = ExceptionInfo->ContextRecord;
    if (ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT && context->Eip == addresses::rbx_writevoxels) {
        //printf("C func detected! (inst: %X)\n", (UINT)ExceptionRecord->ExceptionAddress);
        context->Eip = (UINT)(c_function_handler);
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    SetConsoleTextAttribute(hConsole, CONSOLE_COLOR_RED);
    printf(XorString("Exception 0x%X occurred at 0x%p\n"), ExceptionRecord->ExceptionCode, ExceptionRecord->ExceptionAddress);
    printf(XorString("Error: %s ("), exceptionCodeToString(ExceptionRecord->ExceptionCode));
    if (ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
        switch (ExceptionRecord->ExceptionInformation[0]) {
        case 0:
            printf(XorString("Failed to write memory at 0x%X)\n"), ExceptionRecord->ExceptionInformation[1]);
            break;
        case 1:
            printf(XorString("Failed to read memory at 0x%X)\n"), ExceptionRecord->ExceptionInformation[1]);
            break;
        case 8:
            printf(XorString("Failed to execute memory at 0x%X)\n"), ExceptionRecord->ExceptionInformation[1]);
            break;

        }
    } else if (ExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) {
        switch (ExceptionRecord->ExceptionInformation[0]) {
        case 0:
            printf(XorString("Failed to write memory at 0x%X with NTSTATUS 0x%X)\n"), ExceptionRecord->ExceptionInformation[1], ExceptionRecord->ExceptionInformation[2]);
            break;
        case 1:
            printf(XorString("Failed to read memory at 0x%X with NTSTATUS 0x%X)\n"), ExceptionRecord->ExceptionInformation[1], ExceptionRecord->ExceptionInformation[2]);
            break;
        case 8:
            printf(XorString("Failed to execute memory at 0x%X with NTSTATUS 0x%X)\n"), ExceptionRecord->ExceptionInformation[1], ExceptionRecord->ExceptionInformation[2]);
            break;
        }
    } else {
        printf(XorString("Additional ExceptionInformation: 0x%X)\n"), ExceptionRecord->ExceptionInformation[0]);
    }
    printf(XorString("Number of parameters: %d\n"), ExceptionRecord->NumberParameters);
    printf(XorString("EAX: 0x%p\n"), reinterpret_cast<void*>(context->Eax));
    printf(XorString("EBX: 0x%p\n"), reinterpret_cast<void*>(context->Ebx));
    printf(XorString("ECX: 0x%p\n"), reinterpret_cast<void*>(context->Ecx));
    printf(XorString("EDX: 0x%p\n"), reinterpret_cast<void*>(context->Edx));
    printf(XorString("ESI: 0x%p\n"), reinterpret_cast<void*>(context->Esi));
    printf(XorString("EDI: 0x%p\n"), reinterpret_cast<void*>(context->Edi));
    printf(XorString("EBP: 0x%p\n"), reinterpret_cast<void*>(context->Ebp));
    printf(XorString("ESP: 0x%p\n"), reinterpret_cast<void*>(context->Esp));
    printf(XorString("EIP: 0x%p\n"), reinterpret_cast<void*>(context->Eip));
    printf(XorString("Stack trace:\n"));
    void** stackTrace = (void**)context->Ebp;
    int i = 0;
    while (stackTrace && *(stackTrace + 1)) {
        stackTrace = (void**)*stackTrace;
        printf(XorString("%d: 0x%p\n"), ++i, *(stackTrace + 1));
    }
    MessageBoxA(NULL, XorString("TaaprWare V2 has crashed! Screenshot the console and send it to plusgiant5! (Your crash logs have not been sent to Roblox)"), XorString("Crash"), NULL);
    SetConsoleTextAttribute(hConsole, CONSOLE_COLOR_WHITE);
    Sleep(MAXDWORD);
    return EXCEPTION_EXECUTE_HANDLER;
}

const patch_5 SetUnhandledExceptionFilter_bytes = {0x8B, 0xFF, 0x55, 0x8B, 0xEC};
void unpatch_SetUnhandledExceptionFilter() {
    DWORD original;
    VirtualProtect(SetUnhandledExceptionFilter, 5, PAGE_EXECUTE_READWRITE, &original);
    *reinterpret_cast<patch_5*>(SetUnhandledExceptionFilter) = SetUnhandledExceptionFilter_bytes;
    VirtualProtect(SetUnhandledExceptionFilter, 5, original, &original);
}