#pragma once
#include <vector>
#include <string_view>

#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "DbgHelp.lib")

namespace hwid::utils
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

    static std::vector<std::uintptr_t> pattern_scan(const char* const pattern, const char* const mask, std::string_view seg_str = ".text")
    {
        std::vector<std::uintptr_t> results;

        segment seg{ seg_str };

        for (auto at = seg.start_addr; at < seg.end_addr; ++at)
        {
            const auto is_same = [&]() -> bool
            {
                for (auto i = 0u; i < std::strlen(mask); ++i)
                {
                    if (*reinterpret_cast<std::uint8_t*>(at + i) != static_cast<std::uint8_t>(pattern[i]) && mask[i] != '?')
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
}