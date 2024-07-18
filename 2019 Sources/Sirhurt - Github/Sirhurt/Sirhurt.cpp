#include "windows.h"
#include "globals.h"
#include "retcheck.h"
#include "memory.h"
#include "rlua.h"
#include "Bridge.h"
#include <ctime>
#include "ExceptionSupervisor.hpp"

LONG WINAPI ExampleSupervisor(PEXCEPTION_RECORD pExceptionRecord, PCONTEXT pContext)
{
	if (pContext->Eip == breakpoint)
	{
		pContext->Eip = (DWORD)(lua_jmpbackhandler);
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	if (pExceptionRecord->ExceptionCode == STATUS_ACCESS_VIOLATION) {
		BYTE* FindOut = (BYTE*)pExceptionRecord->ExceptionAddress;

		int addylawl = 0;

		do {
			addylawl++;
			FindOut += 0x1;

			if (addylawl >= 10) {
				break;
			}

		} while (!(FindOut[0] == 0x8B || FindOut[0] == 0x6A));

		if (addylawl == 10) {
			//Log("Unknown Didn't match: %x\n", FindOut[0]);

			pContext->Eip = (int)pExceptionRecord->ExceptionAddress + 0x2;
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		else {
			//Log("Match: %x, Added: %d\n", FindOut[0], addylawl);
			pContext->Eip = (int)FindOut;
			return EXCEPTION_CONTINUE_EXECUTION;
		}

	}

	//pContext->Eip += 10; 
	return EXCEPTION_CONTINUE_SEARCH;
}

void VEH_Bypass() {
	HMODULE mod = GetModuleHandleA("ntdll.dll");
	if (mod) {
		DWORD old;
		DWORD KIException = cast(DWORD, GetProcAddress(mod, "KiUserExceptionDispatcher"));
		VirtualProtect((LPVOID)KIException, 1, PAGE_EXECUTE_READWRITE, &old);
		for (int i = 0; i < 24; i++) {
			*(char*)(KIException + i) = 0x90;
		}
		VirtualProtect((LPVOID)KIException, 1, old, &old);
	}
}

void Execute_Script(const char* data) {
	run(m_L, data);
}

std::string random_string(size_t len)
{
	srand(time(0));
	std::string str = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int pos;
	while (str.size() != len) {
		pos = ((rand() % (str.size() - 1)));
		str.erase(pos, 1);
	}
	return str;
}

void main()
{
	MemUtil::Write(FreeConsole, new byte{ 0xC3 }, 1);
	AllocConsole();

	freopen("conout$", "w", stdout);
	freopen("conin$", "r", stdin);

	Log("Base: %p\n", GetModuleHandle(0));

	uint32_t ScriptContextVftable = *((uint32_t*)(Memory::scan("\xC7\x07\x00\x00\x00\x00\xC7\x47\x00\x00\x00\x00\x00\x8B\x87", "xx????xx?????xx") + 2));//UPDATE
	uintptr_t ScriptContext = Memory::scan((char*)&ScriptContextVftable);//UPDATE

	VEH_Bypass();

	AddSupervisorExceptionHandler(ExampleSupervisor);

	if (!EnableExceptionSupervisor())
	{
		std::cout << "FAILURE";
	}

	DWORD dwOldProtect, dwBkup, dwRelAddr;

	if (VirtualProtectEx((HANDLE)-1, (void*)(VMCheckAddy - 3), 1, PAGE_READONLY, &dwOldProtect) == 0)
		Log("%d\n", GetLastError());

	Log("ScriptContext: %x\n", ScriptContext);

	uintptr_t rL = algorithm;//UPDATE

	lua_State* L = lua_open();

	Log("State: %x\n", rL);

	m_rL = rL;
	m_L = L;

	luaL_openlibs(m_L);

	DWORD* identity2 = (DWORD*)(m_rL - 32); *identity2 ^= (6 ^ (unsigned __int8)*identity2) & 0x1F;
	sandboxThread(m_rL);

	Log("Ready!\n");
	Log("Ready!\n");

	std::string line;
	while (std::getline(std::cin, line))
	{
		run(m_L, line.c_str());
		line = "";
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	//DisableThreadLibraryCalls(hModule);
	if (dwReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
	return 1;
}