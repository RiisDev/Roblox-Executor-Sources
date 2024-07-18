#pragma once
#include <Windows.h>
#include <string>
#include <Psapi.h>

#include "Offsets.h"
#include "LuaWrapper.h"


LONG __stdcall ExceptionHandler(PEXCEPTION_POINTERS ex)
{
	DWORD Code = ex->ExceptionRecord->ExceptionCode;
	DWORD ExceptionType = ex->ContextRecord->Eip;
	if (Code == 0x80000003L) {
		if (ExceptionType == int3breakpoint)
		{
			ex->ContextRecord->Eip = (DWORD)(RobloxFunctionHandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
}


VOID InitBreakpoints() {
	AddVectoredExceptionHandler(1, ExceptionHandler);
}
