// most of these functions are unused and not made by me

#pragma once
#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <string_view>
#include <vector>
#include <DbgHelp.h>

#include "../xorstr.h"

#pragma comment( lib, "DbgHelp.lib" )

namespace utils
{
    struct segment
    {
        std::string_view name;
        std::uintptr_t start_addr = 0;
        std::uintptr_t end_addr = 0;
        std::size_t size = 0;

        segment(std::string_view name_s, HMODULE mod = GetModuleHandle(nullptr)) : name{ name_s }
        {
            const auto nt = ImageNtHeader(mod);
            auto section = reinterpret_cast<IMAGE_SECTION_HEADER*>(nt + 1);

            for (auto iteration = 0u; iteration < nt->FileHeader.NumberOfSections; ++iteration, ++section)
            {
                const auto segment_name = reinterpret_cast<const char*>(section->Name);

                if (name == segment_name)
                {
                    start_addr = reinterpret_cast<std::uintptr_t>(mod) + section->VirtualAddress;
                    size = section->Misc.VirtualSize;
                    end_addr = start_addr + size;

                    break;
                }
            }
        }
    };

    extern std::uintptr_t text_clone, vmp0_clone, base;
    extern utils::segment text_seg, vmp0_seg;

    void unhook_free_console();
    void hook_free_console();
    std::uintptr_t calculate_function_size(std::uintptr_t address);
    std::uintptr_t tramp_hook(std::uintptr_t func, std::uintptr_t new_func, std::size_t inst_size);
    std::vector<std::uintptr_t> pattern_scan(const std::string_view& pattern, const std::string_view& mask);
    std::uintptr_t clone_section(std::uintptr_t section_start);
    std::uintptr_t get_prologue(std::uintptr_t function_address);
    __declspec(noinline) std::uintptr_t __stdcall spoof(std::uintptr_t address);
    bool is_prologue(std::uintptr_t address);
}