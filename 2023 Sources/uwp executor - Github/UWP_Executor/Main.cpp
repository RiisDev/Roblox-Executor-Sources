#include <Windows.h>
#include <thread>

#include "Roblox.hpp"
#include "Execution.hpp"

void entry() noexcept {
    try {
        // Disable the new identity system.
		*reinterpret_cast<bool*>(Roblox::base + Offsets::identity_flag) = false;
        
        // Execute script(s).
        Execution::execute_script("printidentity()");
    } catch (const std::exception& error) {
        // Caught an exception, print it to the console.
        Roblox::print(3, "The executor has caught an exception: %s", error.what());
    }
}

bool __stdcall DllMain(HMODULE h_module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(h_module);
        std::thread(entry).detach();
    }
    return true;
}
