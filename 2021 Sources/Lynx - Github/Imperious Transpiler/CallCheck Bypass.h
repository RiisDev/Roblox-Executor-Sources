#pragma once

#include "Rlua.h"

#include <vector>
#include <cstdint>
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>

extern "C" {
#include "Lua/llimits.h"

}

#define s(x)(x - 0x400000 + (DWORD)GetModuleHandleA(0))


struct cCache {
    DWORD addr1;
    DWORD addr2;
};

std::vector<cCache> Breakpoint;

std::vector<int> BreakpointList = {};

DWORD locateINT3() {
    DWORD _s = s(0x400000);
    const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
    int times = 0;
    for (int i = 0; i < MAX_INT; i++) {
        if (times == 1000) break;
        if (memcmp((void*)(_s + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {
            BreakpointList.push_back(_s + i);
            times++;
        }
    }
    return NULL;
}

LONG __stdcall CallCheckBypass(PEXCEPTION_POINTERS code) {
    if (code->ExceptionRecord->ExceptionCode == 0x80000003L)
    {
        for (int idx = 0; idx < Breakpoint.size(); idx++) {
            if (code->ContextRecord->Eip == Breakpoint[idx].addr2)
            {
                code->ContextRecord->Eip = Breakpoint[idx].addr1;
                return -1;
            }
        }
        return 0;
    }
}

DWORD Add(DWORD func)
{
    DWORD Size = Breakpoint.size();
    for (int i = 0; i < Size; i++) {
        if ((DWORD)func == Breakpoint[i].addr1) {
            return Breakpoint[i].addr2;
        }
    }
    DWORD addr = BreakpointList[Size];
    Breakpoint.push_back({ (DWORD)func, addr });
    return addr;
}

void BypassCallCheck()
{
    locateINT3();
    AddVectoredExceptionHandler(1, CallCheckBypass);
}
