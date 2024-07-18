#include "istainted.hpp"
#include <cstdint>


//isnt it beatiful???
//xor eax,eax; ret 04
void hyperion::bypasses::istained::terminate_istained()
{
	const uintptr_t wtdll = reinterpret_cast<uintptr_t>(GetModuleHandleA("WINTRUST.dll"));
	const uintptr_t wvt = reinterpret_cast<uintptr_t>(GetProcAddress(reinterpret_cast<HMODULE>(wtdll), "WinVerifyTrust"));

	const uint8_t nBytes[] = { 0x31, 0xC0, 0x5D, 0xC2, 0x0C, 0x00 };
	const uint32_t nByteLength = sizeof(nBytes);

	DWORD oldProtection;
	VirtualProtect(reinterpret_cast<void*>(wvt), nByteLength, 0x40, &oldProtection);
	memmove(reinterpret_cast<void*>(wvt), nBytes, nByteLength);
	VirtualProtect(reinterpret_cast<void*>(wvt), nByteLength, oldProtection, &oldProtection);
}
