
/*
*
*	EXCEPTION SUPERVISOR
*	by Louka (@mb_louka)
*	https://github.com/LoukaMB
*
*/

#include "ExceptionSupervisor.hpp"
#include <vector>

/* stores our supervisor exception handlers */
std::vector<SupervisorExceptionHandler> Supervisors;

/* if no supervisor caught the exception (EXCEPTION_CONTINUE_SEARCH), its important
to resume to the rest of KiUserExceptionDispatcher to let SEH and VEH occur naturally */
DWORD _ResumeKiUserExceptionDispatcher;

/* location of the ZwContinue function, used by KiUserExceptionDispatcher */
PVOID _ZwContinue;

/* if ZwContinue errors (an exception has occured), we must resume after the
ZwContinue call in KiUserExceptionDispatcher by jumping to it (this contains the address) */
DWORD _ResumeAfterZwContinue;

LONG WINAPI GeneralSupervisorExceptionHandler(PEXCEPTION_RECORD pExceptionRecord, PCONTEXT pContext)
{
	for (SupervisorExceptionHandler Handler : Supervisors)
		if (Handler(pExceptionRecord, pContext) == EXCEPTION_CONTINUE_EXECUTION) return TRUE;
	return FALSE; /* execute normal handlers */
}

/* KiUserExceptionDispatcher is the function that handles EVERY exception (SEH and VEH) going on in
your program. By hooking it, we can essentially catch every exception being thrown. The function receives
two arguments, namely an exception record and a CONTEXT struct. we'll simply pass that to our exception handlers.*/
__declspec(naked) void NTAPI H_KiUserExceptionDispatcher()
{
	static PEXCEPTION_RECORD pExceptionRecord;
	static PCONTEXT pContext;
	__asm mov ebx, [esp]		/* location of the exception record */
		__asm mov ecx, [esp + 4]	/* location of the context record */
		__asm mov pExceptionRecord, ebx
	__asm mov pContext, ecx
	if (GeneralSupervisorExceptionHandler(pExceptionRecord, pContext))
	{
		/* if one of the handlers returned EXCEPTION_CONTINUE_EXECUTION,
		invoke ZwContinue (passing pContext) to resume the thread */
		__asm mov ecx, [esp + 4]
			__asm push 0
		__asm push ecx
		__asm call _ZwContinue				/* only returns when an exception is thrown */
		__asm jmp _ResumeAfterZwContinue	/* this will eventually call RtlRaiseException */
	}
	__asm cld
	__asm mov ecx, [esp + 4]
		__asm jmp _ResumeKiUserExceptionDispatcher /* resume original KiUserExceptionDispatcher */
}

/* quick function to look up exports, nothing of importance */
inline PVOID ExportLookup(const char* DllName, const char* FnName)
{
	return GetProcAddress(GetModuleHandleA(DllName), FnName);
}

/* another quick function, this time to hook stuff... general utility */
BOOL PlaceHook(DWORD Where, PVOID JumpTo)
{
	DWORD nOldProtect;
	if (!VirtualProtect((LPVOID)Where, 5, PAGE_EXECUTE_READWRITE, &nOldProtect))
		return FALSE;
	*(BYTE*)(Where) = 0xE9;
	*(DWORD*)(Where + 1) = ((DWORD)JumpTo) - Where - 5;
	if (!VirtualProtect((LPVOID)Where, 5, nOldProtect, &nOldProtect))
		return FALSE;
	return TRUE;
}

VOID AddSupervisorExceptionHandler(SupervisorExceptionHandler ExceptionHandler)
{
	Supervisors.push_back(ExceptionHandler);
}

BOOL EnableExceptionSupervisor()
{
	DWORD KiUserExceptionDispatcher = (DWORD)(ExportLookup("ntdll.dll", "KiUserExceptionDispatcher"));
	_ZwContinue = ExportLookup("ntdll.dll", "ZwContinue");
	if (!KiUserExceptionDispatcher || !_ZwContinue)
		return FALSE;
	_ResumeAfterZwContinue = KiUserExceptionDispatcher + 0x41;
	_ResumeKiUserExceptionDispatcher = KiUserExceptionDispatcher + 0x1C;
	return PlaceHook(KiUserExceptionDispatcher + 0x17, H_KiUserExceptionDispatcher);
}