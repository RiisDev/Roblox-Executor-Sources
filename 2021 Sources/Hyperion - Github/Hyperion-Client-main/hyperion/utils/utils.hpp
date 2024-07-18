#pragma once
#include <cstdint>
#include <chrono>

namespace compiler::utils
{
    template <typename T>
    static constexpr bool is_even(const T& value) noexcept
    {
        return !(value % 2);
    }

    static bool is_num(const char& value) noexcept
    {
        return (static_cast<std::uint32_t>(value) - '0' < 10);
    }

    static bool is_newline(const char& value) noexcept
    {
        return (value == '\n' || value == '\r');
    }

    static bool is_space(const char& value) noexcept
    {
        return (value == '\n' || value == '\r'
            || value == '\t' || value == ' ');
    }

    static bool is_alpha(const char& value) noexcept
    {
        return (value >= 'A' && value <= 'Z'
            || value >= 'a' && value <= 'z');
    }

    template <typename T>
    struct timer
    {
        void start()
        {
            start_time = std::chrono::steady_clock::now();
        }

        const auto end()
        {
            const auto end = std::chrono::steady_clock::now();
            const auto elapsed = std::chrono::duration_cast<T>(end - start_time);

            return elapsed.count();
        }
    protected:
        volatile int i = 0;

        std::chrono::steady_clock::time_point start_time;
    };
}

