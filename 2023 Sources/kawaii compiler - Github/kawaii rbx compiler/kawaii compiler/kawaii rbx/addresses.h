#pragma once
#include <cstdint>
#include <Windows.h>
#include <string>

//updated before byfron :kek:

uintptr_t aslr(uintptr_t address) {
    uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));
    return address - 0x400000 + baseAddress;
}

uintptr_t rl;
lua_State* l;

int top = 8 , base = 16;//Argument 3 missing or nil
int id1 = 72, id2 = 24;//printidentity
DWORD vmload_mm[6];//oldResult somewhere idk

uintptr_t getscheduler_address = 0xB38CC0;//SchedulerRate -> sub function at top
uintptr_t vmload_address = 0x759BA0;//oldResult, moduleRef =... 3rd sub function downwards
uintptr_t taskdefer_address = 0x7DFF20;//Maximum re-entrancy depth (%i) exceeded calling task.defer
uintptr_t print_address = 0x10E99C0;//Video recording stopped
uintptr_t performinit_address = aslr(0);//Luauwatchdog or LuauProfiler
uintptr_t getstate_address = 0x7A0FC0;//challenge -> sub function -> somewhere at mid top of code
uintptr_t pseudo2_address = 0x1968640;//AOB 8D 41 08 8B 49 3C
uintptr_t pushfstring_address = 0x19EBC20;//%s:%d: %s function being called in

using getscheduler_rbx = std::uintptr_t(__cdecl*)();
getscheduler_rbx rbx_getscheduler = reinterpret_cast<getscheduler_rbx>(aslr(getscheduler_address));

using getstate_rbx = std::uintptr_t(__thiscall*)(std::uintptr_t rl, int* nigger);
getstate_rbx rbx_getstate = reinterpret_cast<getstate_rbx>(aslr(getstate_address));

using luavmload_rbx = std::uintptr_t(__fastcall*)(std::uintptr_t rl, std::string* source, const char* chunk, int env);
luavmload_rbx rbx_luavmload = reinterpret_cast<luavmload_rbx>(aslr(vmload_address));

using taskdefer_rbx = std::uintptr_t(__cdecl*)(std::uintptr_t rl);
taskdefer_rbx rbx_taskdefer = reinterpret_cast<taskdefer_rbx>(aslr(taskdefer_address));

using print_rbx = uintptr_t(__cdecl*)(int rl, const char* source);
print_rbx rbx_print = reinterpret_cast<print_rbx>(aslr(print_address));

using pseudo2_rbx = uintptr_t(__fastcall*)(DWORD rl, int scr);
pseudo2_rbx rbx_pseudo2addr = reinterpret_cast<pseudo2_rbx>(aslr(pseudo2_address));

using pushfstring_rbx = uintptr_t(__fastcall*)(int rl, const char* str, ...);
pushfstring_rbx rbx_pushfstring = reinterpret_cast<pushfstring_rbx>(aslr(pushfstring_address));

extern "C" {
    typedef uintptr_t(__thiscall* perinit_rbx)(const char*);
    perinit_rbx rbx_performinit = nullptr;
}

std::uintptr_t rbx_increment_top(const std::uintptr_t rl)
{
    std::uintptr_t* valuePtr = reinterpret_cast<std::uintptr_t*>(rl + top);
    *valuePtr += sizeof(void*);
    return *valuePtr;
}
#define rbx_setnilvalue(obj) ((obj)->tt = LUA_TNIL)
