#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <string>
#include <cstdint>
#include <thread>
#include "scheduler.h"
#include <tlhelp32.h>
#include "execution.h"

void pipe() {//make pipe name
    std::string script_source;//handle all scripts into this
    char buffer[999999];//whatever buffer size it is
    const char* skid = "SKID!";//skid
    DWORD rd;
    void* pipe_handle = CreateNamedPipeA(TEXT("\\\\.\\pipe\\kawaii"), PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, PIPE_WAIT, 1, 999999, 999999, NMPWAIT_USE_DEFAULT_WAIT, 0);

    while (true) {//so we can keep running the script
        if (ConnectNamedPipe(pipe_handle, 0)) {
            while (ReadFile(pipe_handle, buffer, sizeof(buffer) - 1, &rd, 0)) {//read whatever the pipe is retrieving
                buffer[rd] = '\0';
                script_source.append(buffer);
            }

            run_script(rl, luaL_newstate(), script_source);//run the script finally
            script_source = "";//make sure we clear the script so we dont run the same script forever
        }

        DisconnectNamedPipe(pipe_handle);
    }
}

void load() {
    //load the console into your daddy >:0
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    printf("kawaii compiler loaded skid!\n");

    rl = (uintptr_t)get_robloxstate();//get the new roblox lua state
    std::cout << "got roblox state 0x" << rl << std::endl;
    std::cout << "got lua state 0x" << luaL_newstate() << std::endl;
  
    uintptr_t* gs = reinterpret_cast<uintptr_t*>(rl + id1);
    int* id = reinterpret_cast<int*>(*gs + id2);
    *id = 7;//dont go above 7 or its skidded for sure

    std::cout << "set identity " << id << " to 7!" << std::endl;
    std::thread(pipe).detach();//make a new thread as in the load function
    std::cout << "pipe thread now done!" << std::endl;

    rbx_print(3, "kawaii compiler - dont skid");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::thread(load).detach();//make a new thread as in the load function
    case DLL_THREAD_ATTACH://do nothing. not even break it
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
