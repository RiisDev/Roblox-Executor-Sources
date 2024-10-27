#pragma once

#include <dependencies/common_includes.hpp>

#include <sys/mman.h>

#include <EGL/egl.h>
#include <unordered_map>

namespace kronhook
{
    static void hook(void* src, void* dst, void** old)
    {
        auto old_function = mmap(0, 16, PROT_READ | PROT_WRITE | PROT_EXEC,
            MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

        auto iold_fn = reinterpret_cast<std::uintptr_t>(old_function);
        auto isrc = reinterpret_cast<std::uintptr_t>(src);

        auto addy = reinterpret_cast<void*>(isrc & -sysconf(_SC_PAGESIZE));

        memset(old_function, 0x0, 16);

        mprotect(addy, isrc + 8 - reinterpret_cast<std::uintptr_t>(addy),
            PROT_EXEC | PROT_READ | PROT_WRITE);

        memcpy(old_function, src, 8);

        *reinterpret_cast<uintptr_t*>(iold_fn + 8) = 0xF000F8DF;
        *reinterpret_cast<uintptr_t*>(iold_fn + 12) = isrc + 9;

        *reinterpret_cast<uintptr_t*>(isrc) = 0xF000F8DF;
        *reinterpret_cast<uintptr_t*>(isrc + 4) = reinterpret_cast<std::uintptr_t>(dst);

        mprotect(addy, isrc + 8 - reinterpret_cast<std::uintptr_t>(addy), PROT_READ | PROT_EXEC);

        *old = reinterpret_cast<void*>(iold_fn + 1);
    }
}

namespace managers
{
    template<typename fn_def>
    class hook_t
    {
        fn_def original_fn;
    public:

        void hook_virtual_function(std::pair< std::uintptr_t, void* > addresses, std::pair< std::size_t, std::size_t > info)
        {
            const auto [object_address, function_address] = addresses;
            const auto [index, sz] = info;

            auto virtual_function_table = new void* [sz];

            std::memcpy(virtual_function_table, *reinterpret_cast<std::uintptr_t**>(object_address), sz * sizeof(void*));

            original_fn = reinterpret_cast<fn_def>(virtual_function_table[index]);

            virtual_function_table[index] = function_address;

            *reinterpret_cast<std::uintptr_t*>(object_address) = reinterpret_cast<std::uintptr_t>(virtual_function_table);
        }

        /* Swaps rdata ptr within vftable **will trigger memcheck if is roblox vftable, use function above** */
        void hook_vftable(std::pair< std::uintptr_t, void* > addresses, std::size_t index)
        {
            const auto [object_address, function_address] = addresses;

            const auto virtual_function_table = *reinterpret_cast<void***>(object_address);

            original_fn = reinterpret_cast<fn_def>(virtual_function_table[index]);

            const auto address_to_patch = reinterpret_cast<std::uintptr_t>(&virtual_function_table[index]);

            const auto page_sz = sysconf(_SC_PAGESIZE);

            const auto page_addr = reinterpret_cast<void*>(address_to_patch & -page_sz);

            mprotect(page_addr, page_sz, PROT_EXEC | PROT_READ | PROT_WRITE);

            virtual_function_table[index] = function_address;

            mprotect(page_addr, page_sz, PROT_READ);
        }

        void hook_vftable_rdata(std::pair< std::uintptr_t, void* > addresses, std::size_t index)
        {
            const auto [virtual_function_table_address, function_address] = addresses;

            const auto virtual_function_table = reinterpret_cast<void**>(virtual_function_table_address);

            const auto address_to_patch = reinterpret_cast<std::uintptr_t>(virtual_function_table + index);

            const auto page_sz = sysconf(_SC_PAGESIZE);

            const auto page_addr = reinterpret_cast<void*>(address_to_patch & -page_sz);

            mprotect(page_addr, page_sz, PROT_EXEC | PROT_READ | PROT_WRITE);

            original_fn = reinterpret_cast<fn_def>(virtual_function_table[index]);

            __android_log_print(ANDROID_LOG_INFO, "[Hydrogen]", "virtual_function_table %p tohk %p Orig %x\n", virtual_function_table, virtual_function_table + index, *reinterpret_cast<std::uintptr_t*>(virtual_function_table + index));

            *reinterpret_cast<std::uintptr_t*>(virtual_function_table + index) = reinterpret_cast<std::uintptr_t>(function_address);

            __android_log_print(ANDROID_LOG_INFO, "[Hydrogen]", "new %p\n", *reinterpret_cast<std::uintptr_t*>(virtual_function_table + index));

            mprotect(page_addr, page_sz, PROT_READ);
        }

        void hook(void* to_hook, void* function_address)
        {            
            //kronhook::hook(to_hook, function_address, reinterpret_cast<void**>(&original_fn));
			DobbyHook( to_hook, function_address, reinterpret_cast< void** >( &original_fn ) );		
        }

        template<typename... args_t>
        auto operator()(args_t... args) -> std::invoke_result_t<fn_def, args_t...>
        {
            return original_fn(args...);
        }
    };

    class hook_manager_t
    {
    public:
        struct
        {
            hook_t < std::uint32_t(*)(const void* job, const std::uintptr_t stats)> on_waiting_script_job_step;
            hook_t < std::uint32_t(*)(int, uint64_t)> on_game_leave;
			
            hook_t < int(*)(void*)> focus_keyboard;
            hook_t < int(*)( void*, bool, int, int)> unfocus_keyboard_internal;

            hook_t < int( * )( int a1, int a2, int a3, const char* a4 ) > some_obscure_shit;
            hook_t < int( * )( JNIEnv* a1, int a2, int a3, int a4, jstring a5, int a6 ) > native_pass_text;
            hook_t < int( * )( int a1, int a2, int a3, int a4, int a5, int a6 ) > native_pass_key;
            hook_t < int( * )( void* a1, void* eventId, int eventType, float xPos, float yPos ) > native_pass_input;
            hook_t < int( * )( void* ptr, void* wnd ) > gl_context_ctor;
            hook_t < int( * )( void* ptr ) > gl_context_dtor;
            hook_t < EGLBoolean( * )( void* ptr ) > egl_swapbuffers;
        } hooks;
    };

    extern hook_manager_t hook_mgr;
}

