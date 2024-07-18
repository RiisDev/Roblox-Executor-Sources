#pragma once
#include <Windows.h>
#include <string>
#include <Psapi.h>

#include "Offsets.h"
#include "LuaWrapper.h"


//99% of the shit in here is by sloppey so lets just say this whole file belongs to him xd


#define log_addy_start 0xB838D
int WINAPI h_MessageBox(
	_In_opt_ HWND    hWnd,
	_In_opt_ LPCTSTR lpText,
	_In_opt_ LPCTSTR lpCaption,
	_In_     UINT    uType
) {
	std::string str_lp = lpText;
	std::string str_cap = lpCaption;
	if (str_cap == "Roblox Crash") {
		DWORD log_start = log_addy_start + (DWORD)GetModuleHandleA(NULL);
		DWORD old;
		for (int i = 0; i < 79; i++) {
			VirtualProtect((LPVOID)(log_start + i), 1, PAGE_EXECUTE_READWRITE, &old);
			*(char*)(log_start + i) = 0x90;
			VirtualProtect((LPVOID)(log_start + i), 1, old, &old);
		}
		str_cap = "Uh Oh...";
		str_lp = "Looks Like Roblox Decided To Commit A Big OOF, But Don't Worry The Logs Have Not Been Uploaded!";
	}
	wchar_t* str_lp_res = new wchar_t[4096];
	wchar_t* str_cap_res = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, NULL, str_lp.c_str(), -1, str_lp_res, 4096);
	MultiByteToWideChar(CP_ACP, NULL, str_cap.c_str(), -1, str_cap_res, 4096);
	return MessageBoxW(hWnd, str_lp_res, str_cap_res, uType);
}


void InitUploadCheckHook() {
	DWORD o;
	VirtualProtect((LPVOID)&MessageBoxA, 1, PAGE_EXECUTE_READWRITE, &o);
	*(char*)(&MessageBoxA) = 0xE9;
	*(DWORD*)((DWORD)&MessageBoxA + 1) = ((DWORD)&h_MessageBox - (DWORD)&MessageBoxA) - 5;
	VirtualProtect((LPVOID)&MessageBoxA, 1, o, &o);
}


DWORD locateINT3() {
	DWORD _s = ASLR(0x400000);
	const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
	for (int i = 0; i < MAX_INT; i++) {
		if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {
			return (_s + i);
		}
	}
	return NULL;
}

int ChatHookHandler(DWORD rL)
{
	typedef int(*_handle)(DWORD);
	_handle handle = (_handle)(r_l_index2(rL, -10003)->value.p);
	return handle(rL);
}


LONG WINAPI ExceptionHandler(PEXCEPTION_POINTERS ex)
{
	switch (ex->ExceptionRecord->ExceptionCode)
	{
	case (DWORD)0x80000003L:
	{
		if (ex->ContextRecord->Eip == bpint3[0])
		{
			ex->ContextRecord->Eip = (DWORD)(int3fnhandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (ex->ContextRecord->Eip == bpint3[1])
		{
			ex->ContextRecord->Eip = (DWORD)(ChatHookHandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		if (ex->ContextRecord->Eip == bpint3[2])
		{
			ex->ContextRecord->Eip = (DWORD)(RLUAYield);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		return -1;
	}
	default: return 0;
	}
	return 0;
}


VOID EnableInt3BPHandler() {
	bpint3.push_back(locateINT3());
	bpint3.push_back(locateINT3());
	AddVectoredExceptionHandler(1, ExceptionHandler);
}
