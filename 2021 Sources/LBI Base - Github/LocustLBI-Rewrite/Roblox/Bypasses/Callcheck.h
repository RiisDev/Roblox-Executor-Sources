#pragma once
#include <Windows.h>
#include <vector>
#include <limits.h>

struct AddyCache
{
    DWORD fake;
    DWORD real;
};

std::vector<AddyCache> cache;
UINT iteration = 0;

long __stdcall VEH(PEXCEPTION_POINTERS code)
{
    if (code->ExceptionRecord->ExceptionCode == 0x80000003L)
    {
        for (UINT i = 0; i < cache.size(); i++)
        {
            if (code->ContextRecord->Eip == cache[i].fake)
            {
                code->ContextRecord->Eip = cache[i].real;
                return EXCEPTION_CONTINUE_EXECUTION;
            }
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

DWORD LocateInt3BP()
{
    const int start = reinterpret_cast<int>(GetModuleHandleA(0));
    const BYTE signature[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };

    while (iteration < (INT_MAX - 1))
    {
        iteration++;
        if (!memcmp(reinterpret_cast<const void*>(start + iteration), signature, sizeof(signature)))
        {
            return start + iteration;
        }
    }
    return 0x0;
}

DWORD ReturnSafeAddr(DWORD addr)
{
    for (UINT i = 0; i < cache.size(); i++)
    {
        if (addr == cache[i].real)
        {
            return cache[i].fake;
        }
    }

    DWORD safe = LocateInt3BP();
    cache.push_back({ safe, addr });
    return safe;
}