#include <Windows.h>
#include <cstdint>

void terminate_suef_hook()
{
	const uintptr_t Kernel = reinterpret_cast<uintptr_t>(GetModuleHandleA("KERNEL32.dll"));
	const uintptr_t Filter = reinterpret_cast<uintptr_t>(GetProcAddress(reinterpret_cast<HMODULE>(Kernel), "SetUnhandledExceptionFilter"));

	const uint8_t nBytes[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	const uint32_t nByteLength = sizeof(nBytes);

	DWORD oldProtection;
	VirtualProtect(reinterpret_cast<void*>(Filter), nByteLength, 0x40, &oldProtection);
	memmove(reinterpret_cast<void*>(Filter), nBytes, nByteLength);
	VirtualProtect(reinterpret_cast<void*>(Filter), nByteLength, oldProtection, &oldProtection);

	SetUnhandledExceptionFilter(top_level_exception_handler);
}

long WINAPI top_level_exception_handler(PEXCEPTION_POINTERS pExceptionInfo)
{
	MessageBoxA(0, "Roblox crashed!", "Crash Handler", MB_OK);
	exit(1);

	return EXCEPTION_EXECUTE_HANDLER;
}
