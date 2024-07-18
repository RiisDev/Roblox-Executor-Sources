#include <Windows.h>
#include <thread>
#include <tlhelp32.h>

void hook_free_console()
{
	static auto ptr = reinterpret_cast<std::uintptr_t>(&FreeConsole);
    static auto ptr_jmp = ptr + 0x6;

    DWORD old{ 0 };

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, PAGE_EXECUTE_READWRITE, &old);

    *reinterpret_cast<std::uintptr_t**>(ptr + 0x2) = &ptr_jmp;
    *reinterpret_cast<std::uint8_t*>(ptr + 0x6) = 0xC3;

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, old, &old);
}

std::uintptr_t tramp_hook(std::uintptr_t func, std::uintptr_t new_func, std::size_t inst_size)
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

long __stdcall veh_handler(EXCEPTION_POINTERS* const exception_pointers)
{
    // this is the exception Roblox create to check debug registers, so using our veh which is on top of all, we clear the debug registers
    if (*reinterpret_cast<std::uintptr_t*>(exception_pointers->ContextRecord->Eip - 3) == 0x90310F9D)
    {
        std::printf("[RHWB] Exception Created: 0x%X\n", exception_pointers->ContextRecord->Eip);

        // clear debug registers
        std::memset(&exception_pointers->ContextRecord->Dr0, 0, sizeof(DWORD) * 6);
	}

    return EXCEPTION_CONTINUE_SEARCH;
}

auto get_thread_context_original = reinterpret_cast<decltype(&GetThreadContext)>(0x0);
auto set_thread_context_original = reinterpret_cast<decltype(&SetThreadContext)>(0x0);

static std::uint32_t thread_id = 0;
static std::uint32_t flag_to_set0, flag_to_set1, flag_to_set2, flag_to_set3 = 0;

NTSTATUS __stdcall get_thread_context_hook(HANDLE thread, LPCONTEXT context)
{
    const auto result = get_thread_context_original(thread, context);

    // we have the thread Roblox check, set debug registers to the saved flags
    if (GetThreadId(thread) == thread_id)
    {
        context->Dr0 = flag_to_set0;
        context->Dr1 = flag_to_set1;
        context->Dr2 = flag_to_set2;
        context->Dr3 = flag_to_set3;

        std::memset(&context->Dr6, 0, sizeof(DWORD) * 2);

        std::printf("[RHWB] Flag Spoofed: 0x%X\n", flag_to_set0);
    }

    // if the thread is not the one Roblox check, clear debug registers
    else
    {
        // clear debug registers
        std::memset(&context->Dr0, 0, sizeof(DWORD) * 6);
    }

    return result;
}

NTSTATUS __stdcall set_thread_context_hook(HANDLE thread, LPCONTEXT context)
{
    // if u inject too early thread flags won't be set so we check this here
    if (!thread_id && (context->Dr0 != 0 && context->Dr0 < 0x30000))
        thread_id = GetThreadId(thread);

    // set flags to debug registers if the thread is what Roblox check
    if (GetThreadId(thread) == thread_id)
    {
        flag_to_set0 = context->Dr0;
        flag_to_set1 = context->Dr1;
        flag_to_set2 = context->Dr2;
        flag_to_set3 = context->Dr3;

        std::printf("[RHWB] Flag Set: 0x%X\n", flag_to_set0);

        return 0;
    }

    return set_thread_context_original(thread, context);
}

// scans for where Roblox clone wow64 transition
std::uintptr_t scan_for_transition_clone()
{
    std::uintptr_t addr = 0;

    MEMORY_BASIC_INFORMATION mbi;

    while (VirtualQuery(reinterpret_cast<std::uint32_t*>(addr), &mbi, sizeof(mbi)))
    {
        if (addr && mbi.AllocationProtect == PAGE_EXECUTE_READWRITE && mbi.State == MEM_COMMIT && mbi.RegionSize == 0x1000)
        {
            const auto mem = reinterpret_cast<std::uint8_t*>(addr);

            if (mem[0] == 0xFF && mem[10] == 0xEA)
                return addr;
        }

        addr += mbi.RegionSize;
    }

    return {};
}

// finds the thread Roblox set the flags to
bool find_flagged_thread()
{
    THREADENTRY32 te32;

    const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    te32.dwSize = sizeof(THREADENTRY32);

    while (Thread32Next(snapshot, &te32))
    {
        if (te32.th32OwnerProcessID == GetCurrentProcessId())
        {
            static auto thread_handle = INVALID_HANDLE_VALUE;
            thread_handle = OpenThread(THREAD_ALL_ACCESS, false, te32.th32ThreadID);

            if (!thread_handle || thread_handle == INVALID_HANDLE_VALUE)
            {
                CloseHandle(snapshot);

                return false;
            }

            CONTEXT ctx{ 0 };
            ctx.ContextFlags = CONTEXT_ALL;

            GetThreadContext(thread_handle, &ctx);

            if (ctx.Dr0 && ctx.Dr0 < 0x30000)
            {
                thread_id = GetThreadId(thread_handle);

                flag_to_set0 = ctx.Dr0;
                flag_to_set1 = ctx.Dr1;
                flag_to_set2 = ctx.Dr2;
                flag_to_set3 = ctx.Dr3;

                CloseHandle(thread_handle);
                CloseHandle(snapshot);

                return true;
            }

            CloseHandle(thread_handle);
        }
    }

    CloseHandle(snapshot);

    return false;
}

void d_main()
{
    hook_free_console();

    AllocConsole();
    FILE* file_stream;

    freopen_s(&file_stream, "CONIN$", "r", stdin);
    freopen_s(&file_stream, "CONOUT$", "w", stdout);
    freopen_s(&file_stream, "CONOUT$", "w", stderr);

    SetConsoleTitleA("RHWB");

    find_flagged_thread();

    const auto cloned_transition = scan_for_transition_clone();

    if (!cloned_transition)
    {
        std::printf("[RHWB] Couldn't Find NtGetContextThread Clone!\n");

        return;
    }

    const auto ntdll = GetModuleHandleA("ntdll.dll");

    if (!ntdll)
        return;

    const auto get_context_thread = reinterpret_cast<std::uintptr_t>(GetProcAddress(ntdll, "NtGetContextThread"));
    const auto set_context_thread = reinterpret_cast<std::uintptr_t>(GetProcAddress(ntdll, "NtSetContextThread"));

    DWORD old{ 0 };

    // redirect Roblox protected NtGetContextThread call to original so it goes to our hook
    VirtualProtect(reinterpret_cast<LPVOID>(cloned_transition), 0x4, PAGE_EXECUTE_READWRITE, &old);

    *reinterpret_cast<std::uint8_t*>(cloned_transition + 0x65) = 0xE9;
    *reinterpret_cast<std::uintptr_t*>(cloned_transition + 0x65 + 1) = (get_context_thread - (cloned_transition + 0x65) - 5);

    VirtualProtect(reinterpret_cast<LPVOID>(cloned_transition), 0x4, old, &old);

    get_thread_context_original = reinterpret_cast<decltype(&GetThreadContext)>(tramp_hook(get_context_thread, reinterpret_cast<std::uintptr_t>(get_thread_context_hook), 5));
    set_thread_context_original = reinterpret_cast<decltype(&SetThreadContext)>(tramp_hook(set_context_thread, reinterpret_cast<std::uintptr_t>(set_thread_context_hook), 5));

    AddVectoredExceptionHandler(true, veh_handler);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        std::thread{ d_main }.detach();

    return TRUE;
}