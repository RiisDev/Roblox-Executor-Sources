#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

#include <DbgHelp.h>

#pragma comment( lib, "DbgHelp.lib" )

namespace urs::utils
{
	void hook_free_console();

	std::uintptr_t tramp_hook(std::uintptr_t func, std::uintptr_t new_func, std::size_t inst_size);

    std::string read_string(std::uintptr_t str_address);

    std::uintptr_t get_instance_parent(std::uintptr_t address);

    std::string get_instance_name(std::uintptr_t address);

    std::string get_instance_path(std::uintptr_t address);

    std::vector<std::uintptr_t> pattern_scan(const std::string_view& pattern, const std::string_view& mask);

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
}