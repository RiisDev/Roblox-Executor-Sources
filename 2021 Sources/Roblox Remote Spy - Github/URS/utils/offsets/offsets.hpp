#pragma once
#include <cstdint>

namespace urs::offsets
{
    inline std::uintptr_t fire_server_address = 0;
    inline std::uintptr_t invoke_server_address = 0;

    constexpr std::uint16_t parent = 0x34;
    constexpr std::uint16_t name = 0x28;
    constexpr std::uint16_t string_length = 0x14;

    inline void init_offsets()
    {
        const auto call_fire_server_address = urs::utils::pattern_scan("\xE8\x00\x00\x00\x00\x8D\x4D\xA4\xE8\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\x8B\x75\xEC", "x????xxxx????xx?????xxx").back();
        urs::offsets::invoke_server_address = urs::utils::pattern_scan("\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF8\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x53\x81\xEC\x00\x00\x00\x00\x56\x57\x8B\xF9\xC7\x45\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx????xxxx????xxx????xxxxxx?????x????xx").back();

        urs::offsets::fire_server_address = *reinterpret_cast<std::uintptr_t*>(call_fire_server_address + 1) + call_fire_server_address + 5;
    }
}