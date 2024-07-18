#pragma once
#include <cstdint>

namespace urs::arg_handlers
{
    void read_arg(std::uint32_t index, std::uintptr_t arg, const char* const arg_type_name);
}