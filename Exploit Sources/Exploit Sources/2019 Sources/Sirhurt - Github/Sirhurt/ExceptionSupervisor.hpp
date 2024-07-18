#pragma once

/*
*
*	EXCEPTION SUPERVISOR, designed for Windows 10
*	by Louka (@mb_louka)
*	https://github.com/LoukaMB
*
*	This library implements an additional chain that receives every thrown
*	exception before the SEH or VEH chain does. It can be used to prevent an
*	exception from being caught by normal exception handlers entirely; it can
*	also be used to hide or modify exception data before proceeding to the
*	other exception handlers set by your or another program.
*
*	Your exception handler should function the same way as a normal exception
*	handler, returning proper values such as EXCEPTION_CONTINUE_EXECUTION and
*	EXCEPTION_CONTINUE_SEARCH, which will perform their expected behavior.
*
*	To use this library with different Windows versions, you'll have to disassemble
*	ntdll.dll, take a look at KiUserExceptionHandler and adjust offsets in the .cpp
*/

#include <Windows.h>

typedef LONG(WINAPI* SupervisorExceptionHandler)(PEXCEPTION_RECORD, PCONTEXT);
VOID AddSupervisorExceptionHandler(SupervisorExceptionHandler ExceptionHandler);
BOOL EnableExceptionSupervisor();