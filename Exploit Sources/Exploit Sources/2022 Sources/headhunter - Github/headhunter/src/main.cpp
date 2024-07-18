#include <Windows.h>
#include <thread>

#include "console/console.h"
#include "cheat/cheat.h"
#include "scheduler/scheduler.h"
#include "execution/execution.h"
#include "communication/communication.h"

const console output{ "headhunter - fishy" };
const scheduler_t scheduler{};
const execution_t execution{ &scheduler };
const communication_t communication{};

void mainfunc()
{
    output << console::color::cyan << "Welcome to headhunter! A simple exploit made by fishy.\n";
    output << console::color::white << "Current scheduler: 0x" << (scheduler.get() - reinterpret_cast<std::uintptr_t>(GetModuleHandle(NULL))) << "\n";

    scheduler.print_jobs();

    output << console::color::pink << "Got Datamodel: 0x" << scheduler.get_datamodel() << "\n";
    output << console::color::pink << "Got ScriptContext: 0x" << scheduler.get_script_context() << "\n";
    output << console::color::pink << "Got luastate: 0x" << scheduler.get_global_luastate() << "\n";

    execution.hook_waiting_scripts_job();
    execution.set_identity(7);

    output << console::color::pink << "Successfully set identity to 7!\n";

    while (true)
    {
        execution.run_script(communication.read_pipe()); // note: won't show error message it'll just try to call the error msg lmao
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::thread(mainfunc).detach();
        break;
    }
    return TRUE;
}