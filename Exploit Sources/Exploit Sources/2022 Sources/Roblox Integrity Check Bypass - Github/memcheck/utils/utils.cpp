#include "utils.hpp"

void memcheck::utils::open_console()
{
    const auto lib = LoadLibraryA("KERNEL32.dll");

    if (!lib)
        return;

    const auto free_console = reinterpret_cast<std::uintptr_t>(GetProcAddress(lib, "FreeConsole"));

    if (free_console)
    {
        static auto jmp = free_console + 0x6;

        DWORD old{};

        VirtualProtect(reinterpret_cast<void*>(free_console), 0x6, PAGE_EXECUTE_READWRITE, &old);

        *reinterpret_cast<std::uintptr_t**>(free_console + 0x2) = &jmp;
        *reinterpret_cast<std::uint8_t*>(free_console + 0x6) = 0xC3;

        VirtualProtect(reinterpret_cast<void*>(free_console), 0x6, old, &old);
    }

    AllocConsole();

    FILE* file_stream;

    freopen_s(&file_stream, "CONIN$", "r", stdin);
    freopen_s(&file_stream, "CONOUT$", "w", stdout);
    freopen_s(&file_stream, "CONOUT$", "w", stderr);

    SetConsoleTitleA("EMB");
}

void memcheck::utils::hook(std::uintptr_t to_patch, std::uintptr_t func, std::size_t size)
{
    DWORD old_protect;
    VirtualProtect(reinterpret_cast<void*>(to_patch), size, PAGE_EXECUTE_READWRITE, &old_protect);

    std::memset(reinterpret_cast<void*>(to_patch), 0x90, size);

    *reinterpret_cast<std::uint8_t*>(to_patch) = 0xE9;
    *reinterpret_cast<std::uintptr_t*>(to_patch + 1) = (func - to_patch - 5);

    VirtualProtect(reinterpret_cast<void*>(to_patch), size, old_protect, &old_protect);
}