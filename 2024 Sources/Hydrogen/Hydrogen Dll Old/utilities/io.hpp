#pragma once

#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <string>

#pragma section(".text")
__declspec(allocate(".text")) const std::uint8_t ret_stub[] = { 0x90,0xC3 };

namespace utilities::io
{
    constexpr auto debug = true;

    static bool console_open = debug;

    inline const char* tag = "Hydrogen";
    inline void* con_handle = nullptr;

    enum class log_options_t : std::uint32_t {
		none = 0x00000000,
		
		log = 0x00000000,
        error = 0x00000001,
        warning = 0x00000002,
        debug = 0x00000003,

		no_tag = 0x00000100,
		no_timestamps = 0x00001000,
		no_newline = 0x00010000,

		raw = no_tag | no_timestamps | no_newline,
    };

    static void close( )
    {
        if ( console_open )
        {
            ::ShowWindow( ::GetConsoleWindow( ), SW_HIDE );

            console_open = !console_open;
        }
    }

    static void open( )
    {
        if ( !console_open )
        {
            ::ShowWindow( ::GetConsoleWindow( ), SW_RESTORE );

            console_open = !console_open;
        }
    }

    inline bool log_option_has(const log_options_t& option, const log_options_t& flag) {
		return (static_cast<std::uint32_t>(option) & static_cast<std::uint32_t>(flag)) == static_cast<std::uint32_t>(flag);
    }

    inline void initiate(const char* const console_name)
    {
        static auto global_stub_pointer = reinterpret_cast<std::uintptr_t>(ret_stub);

        if (const auto free_console_address = reinterpret_cast<std::uintptr_t>(&FreeConsole))
        {
            DWORD old_protection;

            constexpr const auto size = sizeof(std::uintptr_t) + sizeof(std::uint8_t) * 2;

            VirtualProtect(reinterpret_cast<void*>(&FreeConsole), size, PAGE_EXECUTE_READWRITE, &old_protection);

            *reinterpret_cast<void**>(free_console_address + sizeof(std::uint8_t) * 2) = &global_stub_pointer;

            VirtualProtect(reinterpret_cast<void*>(&FreeConsole), size, old_protection, &old_protection);
        }

        AllocConsole();

        FILE* file_stream;

        freopen_s(&file_stream, "CONIN$", "r", stdin);
        freopen_s(&file_stream, "CONOUT$", "w", stdout);
        freopen_s(&file_stream, "CONOUT$", "w", stderr);

        SetConsoleTitleA(console_name);

        con_handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if constexpr ( !debug ) 
            ::ShowWindow( ::GetConsoleWindow( ), SW_HIDE );
    }
    	
    inline void log(const log_options_t& opts, const char* const format, const auto&... args)
    {
        if constexpr (debug) {    
            if (!log_option_has(opts, log_options_t::no_timestamps)) {
				const auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

                std::printf("[");
                SetConsoleTextAttribute(con_handle, 8);
                std::cout << std::put_time(std::localtime(&time), "%H:%M:%S");
                SetConsoleTextAttribute(con_handle, 7);
                std::printf("] ");
            }

            if (!log_option_has(opts, log_options_t::no_tag)) {
                const int clr =
                    log_option_has(opts, log_options_t::debug) ? 95 :
                    log_option_has(opts, log_options_t::warning) ? 111 :
                    log_option_has(opts, log_options_t::error) ? 79 :
                    47;
                const char* const type =
                    log_option_has(opts, log_options_t::debug) ? "DBG" :
                    log_option_has(opts, log_options_t::warning) ? "WRN" :
                    log_option_has(opts, log_options_t::error) ? "ERR" :
                    "LOG";

                std::printf("[");
                SetConsoleTextAttribute(con_handle, clr);
                std::printf(" %s ", type);
                SetConsoleTextAttribute(con_handle, 7);
                std::printf("] ");
            }

            std::printf(format, args...);

            if (!log_option_has(opts, log_options_t::no_newline)) std::printf("\n");
        }
    }

    inline void log(const char* const format, const auto&... args)
    {
        if constexpr (debug) log(log_options_t::none, format, args...);
    }     
}