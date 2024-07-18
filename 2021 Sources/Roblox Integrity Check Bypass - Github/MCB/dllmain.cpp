#include <Windows.h>
#include <chrono>
#include <thread>
#include <string_view>
#include <DbgHelp.h>

#pragma comment( lib, "DbgHelp.lib" )

// set to 0 to disable any debug logs from bypass
#define MEMCHECK_DEBUG 1

__declspec(noinline) int empty_func(...);

#if MEMCHECK_DEBUG
#define MEM_DBG(...) std::printf(__VA_ARGS__)
#else
#define MEM_DBG(...) empty_func(__VA_ARGS__)
#endif

// core hasher addresses
constexpr auto core_hasher_end = 0x22F12B;
constexpr auto core_hasher = 0x22F09F;
const auto base = reinterpret_cast<std::uint32_t>(GetModuleHandleA(nullptr));

// silent checker function header
using silent_checker_t = std::uint32_t(*)(std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
silent_checker_t silent_checker;

auto ptr = reinterpret_cast<std::uintptr_t>(&FreeConsole);
std::uintptr_t ptr_jmp;

// hasher addresses with base applied
auto memcheck_core = base + core_hasher;
auto hasher_end = base + core_hasher_end;

// allows you to Allocate a console without risk of it being freed
void bypass_free_console()
{
    DWORD old;

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, PAGE_EXECUTE_READWRITE, &old);

    ptr_jmp = ptr + 0x6;

    *reinterpret_cast<std::uintptr_t**>(ptr + 0x2) = &ptr_jmp;
    *reinterpret_cast<std::uint8_t*>(ptr + 0x6) = 0xC3;

    VirtualProtect(reinterpret_cast<void*>(ptr), 0x6, old, &old);
}

// set stack on call, otherwise will kick
// disable optimization and add noinline attribute to prevent it from not generating function
#pragma optimize("", off)
__declspec(noinline) int empty_func(...)
{
    return 0;
}
#pragma optimize("", on)

// creates a jmp to your detour and returns the address of an allocation containing overwritten bytes and a post-jmp
std::uintptr_t tramp_hook(std::uintptr_t func, std::uintptr_t new_func, std::size_t inst_size)
{
    constexpr auto extra_size = 5;

    auto clone = reinterpret_cast<std::uintptr_t>(VirtualAlloc(nullptr, inst_size + extra_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

    if (!clone)
        return 0;

    std::memmove(reinterpret_cast<void*>(clone), reinterpret_cast<void*>(func), inst_size);

    const auto jmp_pos = (func - clone - extra_size);

    *reinterpret_cast<std::uint8_t*>(clone + inst_size) = 0xE9;
    *reinterpret_cast<std::uintptr_t*>(clone + inst_size + 1) = jmp_pos;

    DWORD old_protect;

    VirtualProtect(reinterpret_cast<void*>(func), inst_size, 0x40, &old_protect);

    std::memset(reinterpret_cast<void*>(func), 0x90, inst_size);

    const auto rel_location = (new_func - func - extra_size);
    *reinterpret_cast<std::uint8_t*>(func) = 0xE9;
    *reinterpret_cast<std::uintptr_t*>(func + 1) = rel_location;

    VirtualProtect(reinterpret_cast<void*>(func), inst_size, old_protect, &old_protect);

    return clone;
}

// holds info on a memory segment, ex: ".text", ".rdata"
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

        // iterate through all sections and find one specified by `name_s`, then write struct info
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

// vector containing all sigged silent checkers
std::vector<std::uintptr_t> silent_checkers;

std::uintptr_t text_clone;
std::uintptr_t vmp_clone;

const auto text = segment(".text");
const auto vmp0 = segment(".vmp0");

// redirect address to be hashed to the one in our cloned segment
std::uint32_t spoof(std::uint32_t a1)
{
    const auto& ref = (a1 >= text.start_addr && a1 <= text.end_addr) ? text_clone : vmp_clone;

    if (a1 >= vmp0.start_addr && a1 <= vmp0.end_addr || a1 >= text.start_addr && a1 <= text.end_addr)
        a1 = a1 - base + ref - 0x1000;

    return a1;
}

// spoof the start address of the hash to our cloned section
std::uint32_t silent_checker_hook(std::uint32_t a1, std::uint32_t a2, std::uint32_t a3, std::uint32_t a4)
{
    // we are pushing 4 args even tho using 1 because we need the stack to be exactly like this
    if (a1 == base + 0x1000)
        MEM_DBG("Silent Checker Fired: 0x%X\n", a1, a2, a3, a4);

    return silent_checker(spoof(a1), a2, a3, a4);
}

// allocates memory and returns a clone of the page containing `section_start`
std::uintptr_t clone_section(std::uintptr_t section_start)
{
    MEMORY_BASIC_INFORMATION mbi;

    VirtualQuery(reinterpret_cast<LPCVOID>(section_start), &mbi, sizeof(mbi));

    auto new_mem = VirtualAlloc(nullptr, mbi.RegionSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    std::memmove(new_mem, reinterpret_cast<void*>(section_start), mbi.RegionSize);

    return reinterpret_cast<std::uintptr_t>(new_mem);
}

// calculates rough (inaccurate) size of function using prologue bytes of next function
std::uintptr_t calculate_function_size(std::uintptr_t originaladdress)
{
    auto bytes = reinterpret_cast<std::uint8_t*>(originaladdress);

    do
        bytes += 0x10;
    while (!(bytes[0] == 0x55 && bytes[1] == 0x8B && bytes[2] == 0xEC));

    return reinterpret_cast<std::uintptr_t>(bytes) - originaladdress;
}

// allocates memory for and returns a clone of function starting at `original_address` with the calculated size of the function
std::uintptr_t clone_function(std::uintptr_t original_address, std::size_t function_size = 0)
{
    if (!function_size)
        function_size = calculate_function_size(original_address);

    auto clone = VirtualAlloc(nullptr, function_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    std::memmove(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(clone) + 0x50), reinterpret_cast<void*>(original_address), function_size);

    return reinterpret_cast<std::uintptr_t>(clone) + 0x50;
}

// global variable containing our clone's hashed address
std::uintptr_t to_spoof;

// recreated version of roblox hasher, loads spoofed address into stack and uses our address every time hasher references real address.
// Since roblox's hasher also hashes the current address (and not only to what it points to) we can't just spoof the addresses like we do in silent checkers hook,
// instead we have to recreate the function itself.
__declspec(naked) std::uintptr_t stub()
{
    __asm
    {
        mov edx, 0x3FB59B39
        mov edx, esp
        pop edx

        mov ebx, [ebp - 0x10]
        mov to_spoof, ebx
    }

    if (to_spoof == base + 0x1000)
        MEM_DBG("Core Hasher Fired: 0x%X \n", to_spoof);

    // sets our global to the spoofed address in our clone
    to_spoof = spoof(to_spoof);
     
    __asm
    {
        mov ecx, [ebp - 0x48]
        mov esi, [ebp - 0x18]
        mov edi, [ebp - 0x24]

        mov eax, to_spoof
        mov [esp], eax

        hasher_begin:

        mov eax, [esp]
        mov eax, [eax]
        add eax, ebx
        imul eax, eax, 0x1594FE2D
        add eax, edi
        rol eax, 0x13
        imul edi, eax, 0xCBB4ABF7

        lea eax, [ebx + 4]

        push eax
        mov eax, [esp + 0x4]
        mov eax, [eax + 0x4]
        sub [esp], eax
        pop eax

        add ebx, 0x8
        add DWORD PTR [esp], 0x08

        imul eax, eax, 0x344B5409
        add eax, [ebp - 0x20]
        rol eax, 0x11
        imul eax, eax, 0x1594FE2D
        mov [ebp - 0x20], eax

        mov eax, [esp]
        mov eax, [eax]
        xor eax, ebx
        add ebx, 0x04
        add DWORD PTR [esp], 0x04
        imul eax, eax, 0x1594FE2D
        add eax, [ebp - 0x1C]
        rol eax, 0x0D
        imul eax, eax, 0xCBB4ABF7
        mov [ebp - 0x1C], eax

        mov eax, [esp]
        mov eax, [eax]
        sub eax, ebx
        add ebx, 0x04
        add DWORD PTR[esp], 0x04
        imul eax, eax, 0x344B5409
        add eax, esi
        rol eax, 0x0F
        imul esi, eax, 0x1594FE2D
        cmp ebx, ecx
        jb hasher_begin

        mov esp, [esp + 0x08]
        jmp [hasher_end]
    }

}

void d_main()
{
    bypass_free_console();

    // create a console and enable input, output, error
    AllocConsole();

    FILE* file_stream;

    freopen_s(&file_stream, "CONIN$", "r", stdin);
    freopen_s(&file_stream, "CONOUT$", "w", stdout);
    freopen_s(&file_stream, "CONOUT$", "w", stderr);

    SetConsoleTitleA("Memcheck Bypass V3");

    // finds all silent checkers
    for (auto addr = text.start_addr; addr < text.end_addr; ++addr)
    {
        const auto bytes = reinterpret_cast<std::uint8_t*>(addr);

        // 3B ?? 73 ?? 2B ?? 8D ?? 02

        if (bytes[0] == 0x3B && bytes[2] == 0x73 && bytes[4] == 0x2B && bytes[6] == 0x8D && bytes[8] == 0x02 || bytes[0] == 0x3B && bytes[2] == 0x0F && bytes[8] == 0x2B && bytes[10] == 0x8D && bytes[12] == 0x02)
        {
            auto temp_addr = addr;

            // locates start addresses of all silent checkers
            while (true)
            {
                const auto temp_bytes = reinterpret_cast<std::uint8_t*>(temp_addr);

                if (temp_bytes[0] == 0x55 && temp_bytes[1] == 0x8B && temp_bytes[2] == 0xEC)
                {
                    std::printf("Silent Checker Found At: 0x%X\n", temp_addr);

                    silent_checkers.push_back(temp_addr);

                    break;
                }

                --temp_addr;
            }
        }
    }

    // clones both .vmp0 and .text segments
    text_clone = clone_section(text.start_addr);
    vmp_clone = clone_section(vmp0.start_addr);

    // clone the specific checker we need, if checkers position change, you will need to update this
    const auto cloned_checker = clone_function(silent_checkers[6]);

    // prevent stack corruption
    std::memset(reinterpret_cast<void*>(cloned_checker + 0x12), 0x90, 0x6);

    // clones specific checker and redirect all other checkers to this one
    silent_checker = reinterpret_cast<silent_checker_t>(cloned_checker);

    for (const auto checker : silent_checkers)
        tramp_hook(checker, reinterpret_cast<std::uintptr_t>(silent_checker_hook), 6);

    // places jmp to our modified version of the core hasher
    DWORD old;

    VirtualProtect(reinterpret_cast<void*>(memcheck_core), 5, 0x40, &old);

    const auto rel_location = (reinterpret_cast<std::uintptr_t>(stub) - memcheck_core - 5);
    *reinterpret_cast<std::uint8_t*>(memcheck_core) = 0xE9;
    *reinterpret_cast<uintptr_t*>(memcheck_core + 1) = rel_location;

    VirtualProtect(reinterpret_cast<void*>(memcheck_core), 5, old, &old);

    MEM_DBG("\nSilent Checkers Hooked!\n\n");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        std::thread(d_main).detach();

    return TRUE;
}