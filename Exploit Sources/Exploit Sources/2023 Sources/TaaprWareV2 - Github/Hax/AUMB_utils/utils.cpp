#include "utils.hpp"

std::uintptr_t utils::text_clone = 0, utils::vmp0_clone = 0, utils::base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));
utils::segment utils::text_seg = utils::segment{XorString(".text")}, utils::vmp0_seg = utils::segment{XorString(".vmp0")};

std::uintptr_t utils::tramp_hook(std::uintptr_t func, std::uintptr_t new_func, std::size_t inst_size)
{
    constexpr auto extra_size = 5;

    auto clone = reinterpret_cast<std::uintptr_t>(VirtualAlloc(nullptr, inst_size + extra_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

    if (!clone)
        return 0;

    std::memmove(reinterpret_cast<void*>(clone), reinterpret_cast<void*>(func), inst_size);

    *reinterpret_cast<std::uint8_t*>(clone + inst_size) = 0xE9;
    *reinterpret_cast<std::uintptr_t*>(clone + inst_size + 1) = (func - clone - extra_size);

    DWORD old_protect{ 0 };

    VirtualProtect(reinterpret_cast<void*>(func), inst_size, PAGE_EXECUTE_READWRITE, &old_protect);

    std::memset(reinterpret_cast<void*>(func), 0x90, inst_size);

    *reinterpret_cast<std::uint8_t*>(func) = 0xE9;
    *reinterpret_cast<std::uintptr_t*>(func + 1) = (new_func - func - extra_size);

    VirtualProtect(reinterpret_cast<void*>(func), inst_size, old_protect, &old_protect);

    return clone;
}

bool free_console_hooked = false;
std::uintptr_t* old_ptr_jmp_val;
std::uint8_t old_ptr_0x6;
void utils::unhook_free_console() {
    static auto ptr = reinterpret_cast<std::uintptr_t>(&FreeConsole);
    static auto ptr_jmp = ptr + 0x6;

    if (free_console_hooked) {
        free_console_hooked = false;
    }

    DWORD old{};

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, PAGE_EXECUTE_READWRITE, &old);

    *reinterpret_cast<std::uintptr_t**>(ptr + 0x2) = old_ptr_jmp_val;
    *reinterpret_cast<std::uint8_t*>(ptr + 0x6) = old_ptr_0x6;

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, old, &old);
}
void utils::hook_free_console()
{
    static auto ptr = reinterpret_cast<std::uintptr_t>(&FreeConsole);
    static auto ptr_jmp = ptr + 0x6;

    if (!free_console_hooked) {
        free_console_hooked = true;
        old_ptr_jmp_val = *reinterpret_cast<std::uintptr_t**>(ptr + 0x2);
        old_ptr_0x6 = *reinterpret_cast<std::uint8_t*>(ptr + 0x6);

    }

    DWORD old{};

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, PAGE_EXECUTE_READWRITE, &old);

    *reinterpret_cast<std::uintptr_t**>(ptr + 0x2) = &ptr_jmp;
    *reinterpret_cast<std::uint8_t*>(ptr + 0x6) = 0xC3;

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, old, &old);
}

std::vector<std::uintptr_t> utils::pattern_scan(const std::string_view& pattern, const std::string_view& mask)
{
    std::vector<std::uintptr_t> results;

    utils::segment text{XorString(".text")};

    for (auto at = text.start_addr; at < text.end_addr; ++at)
    {
        const auto is_same = [&]() -> bool
        {
            for (auto i = 0u; i < mask.length(); ++i)
            {
                if (*reinterpret_cast<std::uint8_t*>(at + i) != static_cast<std::uint8_t>(pattern[i]) && mask[i] == 'x')
                {
                    return false;
                }
            }

            return true;
        };

        if (is_same())
            results.push_back(at);
    }

    return results;
}

std::uintptr_t utils::calculate_function_size(std::uintptr_t address)
{
    auto bytes = reinterpret_cast<std::uint8_t*>(address);

    do
        bytes += 0x10;
    while (!(bytes[0] == 0x55 && bytes[1] == 0x8B && bytes[2] == 0xEC));

    return reinterpret_cast<std::uintptr_t>(bytes) - address;
}

std::uintptr_t utils::clone_section(std::uintptr_t section_start)
{
    MEMORY_BASIC_INFORMATION mbi;

    VirtualQuery(reinterpret_cast<LPCVOID>(section_start), &mbi, sizeof(mbi));

    auto new_mem = VirtualAlloc(nullptr, mbi.RegionSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    std::memmove(new_mem, reinterpret_cast<void*>(section_start), mbi.RegionSize);

    return reinterpret_cast<std::uintptr_t>(new_mem);
}

__declspec(noinline) std::uintptr_t __stdcall utils::spoof(std::uintptr_t address)
{
    const auto& ref = (address >= text_seg.start_addr && address <= text_seg.end_addr) ? text_clone : vmp0_clone;

    if (address >= vmp0_seg.start_addr && address <= vmp0_seg.end_addr || address >= text_seg.start_addr && address <= text_seg.end_addr)
        address = address - base + ref - 0x1000;

    return address;
}

bool utils::is_prologue(std::uintptr_t address)
{
    return
        !std::memcmp(reinterpret_cast<void*>(address), XorString("\x55\x8B\xEC"), 3) ||
        !std::memcmp(reinterpret_cast<void*>(address), XorString("\x53\x8B\xDC"), 3) ||
        !std::memcmp(reinterpret_cast<void*>(address), XorString("\x56\x8B\xF4"), 3);
};

std::uintptr_t utils::get_prologue(std::uintptr_t function_address)
{
    auto address_copy = function_address;

    while (!is_prologue(address_copy))
        address_copy--;

    return address_copy;
};