#include "utils.hpp"

#include "offsets/offsets.hpp"

void urs::utils::hook_free_console()
{
    static auto ptr = reinterpret_cast<std::uintptr_t>(&FreeConsole);
    static auto ptr_jmp = ptr + 0x6;

    DWORD old{ 0 };

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, PAGE_EXECUTE_READWRITE, &old);

    *reinterpret_cast<std::uintptr_t**>(ptr + 0x2) = &ptr_jmp;
    *reinterpret_cast<std::uint8_t*>(ptr + 0x6) = 0xC3;

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, old, &old);
}

std::uintptr_t urs::utils::tramp_hook(std::uintptr_t func, std::uintptr_t new_func, std::size_t inst_size)
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

std::string read_string_of_unknown_length(std::uintptr_t address)
{
    std::string res;
    char character = 0;
    int char_size = sizeof(character);
    int offset = 0;

    res.reserve(204); // 4 * 51 = 204

    while (offset < 200)
    {
        character = *reinterpret_cast<char*>(address + offset);

        if (character == 0) break;

        offset += char_size;

        res.push_back(character);
    }

    return res;
}

std::string urs::utils::read_string(std::uintptr_t str_address)
{
    const auto length = *reinterpret_cast<int*>(str_address + urs::offsets::string_length);

    if (length >= 16u)
    {
        const auto new_name_address = *reinterpret_cast<std::uintptr_t*>(str_address);
        return read_string_of_unknown_length(new_name_address);
    }
    else
    {
        const auto name = read_string_of_unknown_length(str_address);
        return name;
    }
}

std::uintptr_t urs::utils::get_instance_parent(std::uintptr_t address)
{
    return *reinterpret_cast<std::uintptr_t*>(address + urs::offsets::parent);
}

std::string urs::utils::get_instance_name(std::uintptr_t address)
{
    std::uintptr_t name_address = *reinterpret_cast<std::uintptr_t*>(address + urs::offsets::name);

    return read_string(name_address);
}

std::string urs::utils::get_instance_path(std::uintptr_t address)
{
    std::string path = urs::utils::get_instance_name(address);

    std::uintptr_t parent = urs::utils::get_instance_parent(address);

    while (parent)
    {
        path = urs::utils::get_instance_name(parent) + "." + path;

        parent = urs::utils::get_instance_parent(parent);
    }

    return path;
}

std::vector<std::uintptr_t> urs::utils::pattern_scan(const std::string_view& pattern, const std::string_view& mask)
{
    std::vector<std::uintptr_t> results;

    urs::utils::segment text{ ".text" };

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