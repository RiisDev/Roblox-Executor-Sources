#include <Windows.h>
#include <thread>

#include "memcheck/memcheck.hpp"

constexpr auto gate_clone = 0x2B8DAF0u;
constexpr auto gate_integrity = 0xF27860u;

const auto base_addr = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));

const auto text_addr = base_addr + 0x1000;

std::uintptr_t wow64_transition_address = 0;
std::uintptr_t original_gate = 0;

// bypass gate "integrity check"
std::pair<std::uintptr_t, std::uintptr_t>* __stdcall check_for_wow64_hooks_hook(std::pair<std::uintptr_t, std::uintptr_t>* a1)
{
    a1->first = wow64_transition_address;
    a1->second = wow64_transition_address + 9;

    return a1;
}

std::uint32_t addr;
std::uint32_t info;

// if function fails Roblox doesn't do anything. So what we do here is check if they are scanning .text and if so we set syscall id to another one, which basically does nothing
void __declspec(naked) query_hook()
{
    __asm
    {
        mov eax, [esp + 0xC]
        mov addr, eax

        mov eax, [esp + 0x14]
        mov info, eax

        mov eax, text_addr
        cmp eax, addr
        je spoof_ret

        mov eax, 0x23
        jmp print
    }

    print:
    {
        __asm pushad
        __asm pushfd

        std::printf("[EWTB] NtQueryVirtualMemory: Base: %p | Info: %p\n", addr, info);

        __asm popfd
        __asm popad
        __asm jmp original_gate
    }

    spoof_ret:
    __asm mov eax, 0x25
    __asm jmp print
}

void __declspec(naked) gate_stub_hook()
{
    __asm
    {
        cmp eax, 0x23    // NtQueryVirtualMemory
        jne normal

        jmp query_hook

        normal:
        jmp original_gate
    }    
}

// on x86, to really invoke a syscall you must go through WOW64's transaction, or "heaven's gate," which jmps and runs to WOW64 code that 
// directly calls the syscall for you on 64 bit. 
// Roblox calls **certain** functions through their own clone of the transaction, or "gate"
// the goal here is to replace these clones to redirect to the original gate, and hook the stub in WOW64
// a memcheck bypass is also needed, due to them protecting the original gate stub
void bypass_wow64_transition()
{
    // get the gate location
    __asm
    {
        push edx
        mov edx, fs: 0xC0
        mov wow64_transition_address, edx
        pop edx
    }

    // hook their gate integrity check
    tramp_hook(base_addr + gate_integrity, reinterpret_cast<std::uintptr_t>(check_for_wow64_hooks_hook), 6);

    const auto cloned_transition = *reinterpret_cast<std::uintptr_t*>(base_addr + gate_clone);

    DWORD old{ 0 };

    VirtualProtect(reinterpret_cast<LPVOID>(cloned_transition), 0x4, PAGE_EXECUTE_READWRITE, &old);

    // redirect the cloned gate to original gate
    *reinterpret_cast<std::uintptr_t**>(cloned_transition + 0x2) = &wow64_transition_address;

    VirtualProtect(reinterpret_cast<LPVOID>(cloned_transition), 0x4, old, &old);

    // copy original gate jump
    original_gate = reinterpret_cast<std::uintptr_t>(VirtualAlloc(nullptr, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
    memcpy(reinterpret_cast<void*>(original_gate), reinterpret_cast<void*>(wow64_transition_address), 9);

    VirtualProtect(reinterpret_cast<LPVOID>(wow64_transition_address), 0x4, PAGE_EXECUTE_READWRITE, &old);

    // place jmp which goes to our stub
    const auto rel_location = (reinterpret_cast<std::uintptr_t>(gate_stub_hook) - wow64_transition_address - 5);
    *reinterpret_cast<std::uint8_t*>(wow64_transition_address) = 0xE9;
    *reinterpret_cast<std::uintptr_t*>(wow64_transition_address + 1) = rel_location;

    VirtualProtect(reinterpret_cast<LPVOID>(wow64_transition_address), 0x4, old, &old);
}

void allocate_console()
{
    bypass_free_console();

    AllocConsole();
    FILE* file_stream;
    freopen_s(&file_stream, "CONIN$", "r", stdin);
    freopen_s(&file_stream, "CONOUT$", "w", stdout);
    freopen_s(&file_stream, "CONOUT$", "w", stderr);

    SetConsoleTitleA("EWTB");
}

void d_main()
{
    allocate_console();

    memcheck::bypass();

    bypass_wow64_transition();

    std::printf("[EWTB] WOW64 Transition Address: %p\n", wow64_transition_address);

    std::printf("[EWTB] WOW64 Transition Stub: %p\n\n", gate_stub_hook);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        std::thread(d_main).detach();

    return TRUE;
}
