
#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <signal.h>
#include "Luau-Core/include/lua.h"
#include <intrin.h>

// Dump By YT_Dumper V1

int ASLR(uintptr_t address)
{
    return address - 0x400000 + reinterpret_cast<uintptr_t>(GetModuleHandleA(0));
}
int aslr_0(uintptr_t address)
{
    return address + reinterpret_cast<uintptr_t>(GetModuleHandleA(0));
}

uintptr_t exthread;

namespace Offsets
{
    // 'Argument 2 missing or nil' string
    const uintptr_t lua_state_top = 20;
    const uintptr_t lua_state_base = 8;
}

namespace Addresses
{
    // remember v30 = sub_979070(pServiceName, 0);
    using r_luavm_load_t = uintptr_t(__fastcall*)(uintptr_t rl, std::string* source, const char* chunk, int env);
    r_luavm_load_t r_luavm_load = (r_luavm_load_t)(ASLR(0x980FE0)); // string (oldResult, moduleRef  = ...)

    using r_taskdefer_t = uintptr_t(__cdecl*)(uintptr_t rl);// [Maximum re-entrancy depth (%i) exceeded calling task.defer] or [defer]
    r_taskdefer_t r_taskdefer = (r_taskdefer_t)(ASLR(0x980070)); // push

    using r_getscheduler_t = uintptr_t(__cdecl*)(); // [Watchdog or LuauWatchdog]
    r_getscheduler_t r_getscheduler = (r_getscheduler_t)(ASLR(0xBD15B0)); // after jnz get the call sub_xxxxxx 

    using r_print_t = uintptr_t(__cdecl*)(int type, const char* source);
    r_print_t r_lua_print = (r_print_t)(ASLR(0x118A5C0)); // string (Current identity is %d) 

    using get_global_state = uintptr_t(__thiscall*)(int, int*, int*);
    get_global_state get_global_state_2 = (get_global_state)(ASLR(0x87F1C0)); // Script Start or %s %s detected as malicious.  %s will not run. // most be in line 825 pseudocode

    uint64_t check_PlaceId = aslr_0(0x30EBFB0); // Windows10Universal.exe+30EBFB0

}

namespace Deobfuscation {
    uintptr_t luastate(uintptr_t state)
    {
        auto gay = state + 276; // remeber to check + or - or ^
        auto girl = gay - *reinterpret_cast<uintptr_t*>(gay);
        return girl;
    }
}
 

