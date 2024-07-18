#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>


#define base(x) (x - 0x400000 + reinterpret_cast<uintptr_t>(GetModuleHandleA(0)))


int MAX_INT3 = 200;
std::vector<int>       breakpoints;
std::vector<uintptr_t> cclosures;


LONG WINAPI VEH(PEXCEPTION_POINTERS exception)
{
    switch (exception->ExceptionRecord->ExceptionCode)
    {
    case (DWORD)0x80000003L:
    {
        int int3idx = 0;
        for (int INT3 : breakpoints)
        {
            if (exception->ContextRecord->Eip == INT3)
            {
                exception->ContextRecord->Eip = (DWORD)(cclosures[int3idx]);
                return EXCEPTION_CONTINUE_EXECUTION;
            }
            int3idx += 1;
        }
        return -1;
    }
    default:
        return 0;
    }
    return 0;
}


DWORD MAP_INT3() 
{
    int int3_index = 0;
    DWORD BASE = base(0x400000);
    const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
    for (int i = 0; i < INT_MAX; i++) {
        if (memcmp((void*)(BASE + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {
            int3_index += 1;
            if (int3_index > MAX_INT3)
            {
                break;
            }
            breakpoints.push_back(BASE + i);
        }
    }
    return NULL;
}


int ConvertClosure(uintptr_t cclosure = 0)
{
    cclosures.push_back(cclosure);
    int closure_index = cclosures.size() - 1;
    int assigned_int3 = breakpoints[closure_index];
    return assigned_int3;
}