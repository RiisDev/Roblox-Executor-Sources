#pragma once
#include <Windows.h>
#include <iostream>
#include <Richedit.h>
#include <CommCtrl.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <Shlwapi.h>
#include "Wrapper.h"
#include "Scanner.h"

#pragma comment(lib, "wininet.lib")

#include "rlua.h"


using namespace std;



DWORD int3breakpoint;


LONG WINAPI int3cbreakpoint(PEXCEPTION_POINTERS ex)
{
	if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{
		if (ex->ContextRecord->Eip == int3breakpoint)
		{
			ex->ContextRecord->Eip = (DWORD)(int3fnhandler);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}







DWORD locateINT3() {
	DWORD _s = ASLR(0x400000);
	const char i3_8opcode[15] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC};
	for (int i = 0; i < MAX_INT; i++) {
		if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {
			return (_s + i);
		}
	}
	return NULL;
}




void ConsoleHacks()
{
	DWORD nothing;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nothing); //roblox has a "freeconsole" gay thingy
	*(BYTE*)(&FreeConsole) = 0xC3;
}

void Console(char* title) { //void console haxs
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow(); //gets the console window
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW); //make program topmost (above roblox)
																													   //topmost function: Look at http://www.cplusplus.com/forum/windows/125528/
	::ShowWindow(ConsoleHandle, SW_NORMAL); //show window
}

void VEH_Bypass() {
	HMODULE mod = GetModuleHandleA("ntdll.dll");
	DWORD old;
	DWORD KIException = cast(DWORD, GetProcAddress(mod, "KiUserExceptionDispatcher"));
	VirtualProtect((LPVOID)KIException, 1, PAGE_EXECUTE_READWRITE, &old);
	for (int i = 0; i < 24; i++) {
		*(char*)(KIException + i) = 0x90;
	}
	VirtualProtect((LPVOID)KIException, 1, old, &old);
}

void Main()
{

	ConsoleHacks();
	Console((PCHAR)"Omithus | Console Version // By Ice bear & OmithusJayaility");

	VEH_Bypass();


	printf("Scanning.. ");
	int ScriptContextVFtable = ASLR(0x100A120);
	ScriptContext = Memory::findPattern((BYTE*)&ScriptContextVFtable, (PCHAR)"xxxx");

	printf("ScriptContext: %x\n", ScriptContext);

	rLEnv = ScriptContext + 164 + *(DWORD*)(ScriptContext + 164);

	//findbreakpoint();
	int3breakpoint = locateINT3();


	AddVectoredExceptionHandler(TRUE, int3cbreakpoint);

    //rLEnv = ScriptContext + 164 + *(DWORD*)(ScriptContext + 164);
	lua_State* L = lua_open();
	luaL_openlibs(L);
	*(DWORD*)ASLR(0x157BED4) = (DWORD)6; // <- Identity
	//LuaWrapper::ExecuteScript("game.Players.LocalPlayer.Chatted:connect(function(msg) loadstring(msg)() end)");

	do
	{
		string x;
		getline(cin, x);
		ExecuteScript(x.c_str());
	} while (true);
}