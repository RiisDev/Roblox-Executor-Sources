
#include <Windows.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include "gui.hpp"
#include "globals.hpp"
#include "wrap.hpp"
#include "security.hpp"
#include "Utilities/Scan.h"
#include "Utilities/Instance.h"
#include "Lua/lua.hpp"

DWORD ScriptContext = 0;
DWORD int3breakpoint = 0;

unsigned int danghui_rbase()
{
	return (unsigned int)(GetModuleHandleA("RobloxPlayerBeta.exe"));
}

unsigned int danghui_grabscriptcontext()
{
	if (ScriptContext)
		return ScriptContext;
	DWORD SCVT = danghui_rbase() + DH_SCRIPTC;
	ScriptContext = Memory::Scan(PAGE_READWRITE, (char*)&SCVT, "xxxx");
	return ScriptContext;
}

unsigned int danghui_grabstate(unsigned int scriptcontext, int idx)
{
	DWORD* ScriptContext = (DWORD*)scriptcontext;
	return ScriptContext[41] - (DWORD)&ScriptContext[41];
}

DWORD RtlUnwindJmpTo;
DWORD KiUserExceptionDispatcherJmpTo;

__declspec(naked) void VEHK_Hook()
{
	__asm
	{
		mov ebx, [esp]
		push ecx
		push ebx
		call RtlUnwindJmpTo
		jmp KiUserExceptionDispatcherJmpTo
	}
}

DWORD NtDllLocate(PCHAR FnName)
{
	static HMODULE NtDll;
	if (!NtDll)
		NtDll = GetModuleHandleA("ntdll.dll");

	return (DWORD)(GetProcAddress(NtDll, FnName));
}

DWORD KernelBaseLocate(PCHAR FnName)
{
	static HMODULE KernelBase;
	if (!KernelBase)
		KernelBase = GetModuleHandleA("KERNELBASE.dll");

	return (DWORD)(GetProcAddress(KernelBase, FnName));
}

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

BOOL CreateHooks()
{
	VMProtectBeginMutation("CreateHooks (VEH)");
	DWORD RtlUnwind = NtDllLocate("RtlUnwind");
	DWORD KiUserExceptionDispatcher = NtDllLocate("KiUserExceptionDispatcher");
	if (RtlUnwind && KiUserExceptionDispatcher)
	{
		RtlUnwindJmpTo = RtlUnwind + 0x13E;
		KiUserExceptionDispatcherJmpTo = KiUserExceptionDispatcher + 0xF;
		return PlaceHook(KiUserExceptionDispatcher + 5, VEHK_Hook);
	}
	VMProtectEnd();
	return FALSE;
}

LONG WINAPI danghui_int3cbreakpoint(PEXCEPTION_POINTERS ex)
{
	if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		if (ex->ContextRecord->Eip == int3breakpoint)
		{
			ex->ContextRecord->Eip = (DWORD)(danghui_int3fnhandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void danghui_main()
{

	danghui_grabscriptcontext();
	int3breakpoint = danghui_rbase() + 0x10A3FF;
	CreateHooks();
	AddVectoredExceptionHandler(TRUE, danghui_int3cbreakpoint);
	OpenGui();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

BOOL WINAPI DllMain(HMODULE Dll, DWORD jReason, LPVOID)
{
	if (jReason == DLL_PROCESS_ATTACH)
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)danghui_main, NULL, NULL, NULL);
	return TRUE;
}