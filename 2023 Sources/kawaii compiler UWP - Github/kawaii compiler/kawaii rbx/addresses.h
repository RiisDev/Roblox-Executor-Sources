#pragma once
#include <cstdint>
#include <Windows.h>
#include <string>

//i dont know if uwp updates (i havent researched it good enough)

uintptr_t base_address = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr)); //just return module

uintptr_t aslr(uintptr_t address) {
    uintptr_t based = reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr));//subtract from addy and add with base
    return address - 0x400000 + based;
}

uintptr_t rl;//whatever getstate retrieves

int top = 8 , base = 16;//lua state top&base
int id1 = 72, id2 = 24;//extra space and some other idrk

uintptr_t getscheduler_address = 0x772120 + base_address;
uintptr_t getstate_address = 0x419CA0 + base_address;
uintptr_t vmload_address = 0x51F750 + base_address;
uintptr_t taskspawn_address = 0x45E3D0 + base_address;
uintptr_t print_address = 0xAE4580 + base_address;
uintptr_t pseudo2_address = 0x142C950 + base_address;
uintptr_t pushstring_address = 0x143FBF0 + base_address;
uintptr_t nilobject_address;//not required cus we not using index2ahh

//other addys that were implemented [not required]
uintptr_t printidentity_address = 0x443DD0 + base_address;
uintptr_t closestate_address = 0x1446520 + base_address;


using getscheduler_rbx = std::uintptr_t(__cdecl*)();//getscheduler used for the get_job_by_name function
getscheduler_rbx rbx_getscheduler = reinterpret_cast<getscheduler_rbx>(getscheduler_address);

using getstate_rbx = std::uintptr_t(__thiscall*)(std::uintptr_t rl, int* nigger);//case default returns the global state
getstate_rbx rbx_getstate = reinterpret_cast<getstate_rbx>(getstate_address);

using luavmload_rbx = std::uintptr_t(__fastcall*)(std::uintptr_t rl, std::string* source, const char* chunk, int env);//name says it all-> loads the bytecode into the VM
luavmload_rbx rbx_luavmload = reinterpret_cast<luavmload_rbx>(vmload_address);

using taskdefer_rbx = std::uintptr_t(__cdecl*)(std::uintptr_t rl);//task.defer task.delay scriptcontext::resume can work and also spawn
taskdefer_rbx rbx_taskspawn = reinterpret_cast<taskdefer_rbx>(taskspawn_address);

using print_rbx = uintptr_t(__cdecl*)(int rl, const char* source, ...);//prints onto the dev console (PS: dont use video recording to get this address anymore..it doesnt exist)
print_rbx rbx_print = reinterpret_cast<print_rbx>(print_address);

using pseudo2_rbx = uintptr_t(__fastcall*)(uintptr_t rl, int nigger);//if none of the cases in index2 match it just calls pseudo and lets it handle the index
pseudo2_rbx rbx_pseudo2 = reinterpret_cast<pseudo2_rbx>(pseudo2_address);

using pushstring_rbx = uintptr_t(__fastcall*)(int rl, const char* str);//pretty neat checks if the Call Info is lua (if it is it will use luaO_push otherwise it will use pushstring!)
pushstring_rbx rbx_pushstring = reinterpret_cast<pushstring_rbx>(pushstring_address);

using printidentity_rbx = std::uintptr_t(__cdecl*)(std::uintptr_t rl);//prints identity
printidentity_rbx rbx_printidentity = reinterpret_cast<printidentity_rbx>(printidentity_address);

using closestate_rbx = std::uintptr_t(__fastcall*)(std::uintptr_t rl);//closes state (remember to do rl = NULL)
closestate_rbx rbx_closestate = reinterpret_cast<closestate_rbx>(closestate_address);
