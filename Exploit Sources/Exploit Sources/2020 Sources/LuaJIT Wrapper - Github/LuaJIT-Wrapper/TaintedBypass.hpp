#pragma once
#include <Windows.h>
static auto IsTaintedBypass() -> void
{
    const auto Tainted = reinterpret_cast<unsigned long>(GetModuleHandleA("WINTRUST.dll")); /* Store the module of WINTRUST.dll */
    const auto isTainted = reinterpret_cast<unsigned long>(GetProcAddress(reinterpret_cast<HMODULE>(Tainted), "WinVerifyTrust")); /* Get the address of WinVerifyTrust function */

    BYTE Bytes[6] =
    {
        0x31, /* 49 */
        0xC0, /* 192 */
        0x5D, /* 93 */
        0xC2, /* 194 */
        0x0C, /* 12 */
        0x00 /* 0 */
    }; /* Store the bytes we are gonna write */

    unsigned long ulOldProtect; /* Used to set back the old protection IsTained was */
    const unsigned long ds = 0;


    VirtualProtect(reinterpret_cast<void*>(isTainted), sizeof(Bytes), 0x40, &ulOldProtect); /* Set the address to read/write */
    for (int Iteration = 0; Iteration < 6; Iteration++) /* Loop the bytes */
    {
        reinterpret_cast<BYTE*>(isTainted + Iteration)[3] = Bytes[Iteration]; /* Add 3 to the current byte and write it */
    }

    VirtualProtect(reinterpret_cast<void*>(isTainted), sizeof(Bytes), ds, &ulOldProtect); /* Set back original protection mode */
}