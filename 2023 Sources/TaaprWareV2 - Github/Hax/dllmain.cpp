// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <stdio.h>
#include <thread>
#include <string>
#include <iostream>
#include <Psapi.h>
#include <fstream>
#include <winnt.h>
#include <winternl.h>

//#include "PolyHook.hpp"
#include "rbxinstance.h"
#include "exceptionfilter.h"
#include "functions.h"
#include "AUMB_utils\utils.hpp"
#include "hwid.h"

double old_fps;
FILE* stream;

void hook_vftable(objects::job* job, uint8_t vftable_idx, UINT func) {
    UINT* new_vftable = (UINT*)malloc(4 * 30);
    memcpy(new_vftable, job->vftable, 4 * 30);
    new_vftable[vftable_idx] = func;
    job->vftable = new_vftable;
}

std::string compiled;

DWORD original_protection;

patch_5 old_addscript;
UINT addscript_ecx_v;
__declspec(naked) UINT addscript_hook() {
    __asm mov addscript_ecx_v, ecx;
    //printf(XorString("AddScript called\n"));
    VirtualProtect((void*)addresses::rbx_addscript, 5, PAGE_EXECUTE_READWRITE, &original_protection);
    *reinterpret_cast<patch_5*>(addresses::rbx_addscript) = old_addscript;
    VirtualProtect((void*)addresses::rbx_addscript, 5, original_protection, &original_protection);
    //printf(XorString("AddScript unhooked\n"));
    __asm mov [esp + 0xC], execution_level; // wont change unless they change the args which wont happen in years
    //printf(XorString("Raised level to %d\n"), execution_level);
    __asm mov ecx, addscript_ecx_v;
    __asm jmp addresses::rbx_addscript;
}

patch_5 old_deserializer_detour;
const char* new_bytecode;
int new_bytecode_len;
const char* old_bytecode;
int old_bytecode_len;
bool deserializer_called = false;
#define bytecodel_len_offset 0x78
#define bytecode_offset 0x15C
__declspec(naked) UINT deserializer_detour() {
    // Set the bytecode
    //printf(XorString("Deserializer detoured\n"));
    deserializer_called = true;
    VirtualProtect((void*)addresses::rbx_deserializer_detour, 5, PAGE_EXECUTE_READWRITE, &original_protection);
    *reinterpret_cast<patch_5*>(addresses::rbx_deserializer_detour) = old_deserializer_detour;
    VirtualProtect((void*)addresses::rbx_deserializer_detour, 5, original_protection, &original_protection);
    //printf(XorString("Deserializer unhooked\n"));
    //__debugbreak();
    new_bytecode = compiled.c_str();
    new_bytecode_len = compiled.size();
    __asm {
        mov eax, [ebp - bytecode_offset];
        mov old_bytecode, eax;
        mov eax, [ebp - bytecodel_len_offset];
        mov old_bytecode_len, eax;
    }
    //printf(XorString("Saved vanilla values\n"));
    __asm {
        mov eax, new_bytecode_len;
        mov [ebp - bytecodel_len_offset], eax;
    }
    //printf(XorString("Set bytecode length to 0x%X\n"), new_bytecode_len);
    __asm {
        mov eax, new_bytecode;
        mov [ebp - bytecode_offset], eax;
    }
    //printf(XorString("Set bytecode to %p\n"), new_bytecode);
    __asm jmp addresses::rbx_deserializer_detour;
}
patch_5 old_deserializer_detour_2;
__declspec(naked) UINT deserializer_detour_2() {
    // After the proto has been created, return the bytecode to normal to pass detections
    //printf(XorString("Deserializer hashcheck detoured\n"));
    //__debugbreak();
    VirtualProtect((void*)addresses::rbx_deserializer_detour_2, 5, PAGE_EXECUTE_READWRITE, &original_protection);
    *reinterpret_cast<patch_5*>(addresses::rbx_deserializer_detour_2) = old_deserializer_detour_2;
    VirtualProtect((void*)addresses::rbx_deserializer_detour_2, 5, original_protection, &original_protection);
    //printf(XorString("Deserializer hashcheck unhooked\n"));
    __asm {
        mov eax, old_bytecode_len;
        mov[ebp - bytecodel_len_offset], eax;
    }
    //printf(XorString("Loaded vanilla bytecode length (%d)\n"), old_bytecode_len);
    __asm {
        mov eax, old_bytecode;
        mov[ebp - bytecode_offset], eax;
    }
    //printf("%p\n", (ULONG_PTR)__readfsdword(PcTeb));
    //printf(XorString("Loaded vanilla bytecode (%p)\n"), old_bytecode);
    __asm jmp addresses::rbx_deserializer_detour_2;
}

job_step_func original_US14116;
patch_5 unpatched_ac, patched_ac; // For when vftable hooking gets patched
BYTE original_writevoxels_bytes[rbx_writevoxels_instsize]{};
JobStatus __fastcall US14116_hook(objects::job* self, int fakearg, int a2) {
    // Revert the patch, then continue the job as normal (US14116 is for memcheck + dll injection)
    VirtualProtect((void*)(addresses::rbx_writevoxels), 1, PAGE_EXECUTE_READWRITE, &original_protection);
    for (int offset = 0; offset < rbx_writevoxels_instsize; offset++) {
        *reinterpret_cast<BYTE*>(addresses::rbx_writevoxels + offset) = original_writevoxels_bytes[offset];
    }
    VirtualProtect((void*)(addresses::rbx_writevoxels), 1, original_protection, &original_protection);
    return original_US14116(self,  fakearg, a2);
}

job_step_func original_waiting_hybrid_scripts_job;
std::vector<std::string> bytecode_queue = {};
std::vector<objects::instance*> script_ignore_list;
JobStatus __fastcall waiting_hybrid_scripts_job_hook(objects::job* self, int fakearg, int a2) {
    // Patch it again (this will run after US14116)
    VirtualProtect((void*)(addresses::rbx_writevoxels), rbx_writevoxels_instsize, PAGE_EXECUTE_READWRITE, &original_protection);
    *reinterpret_cast<BYTE*>(addresses::rbx_writevoxels) = 0x68;
    *reinterpret_cast<UINT*>(addresses::rbx_writevoxels + 1) = (UINT)c_function_handler;
    *reinterpret_cast<BYTE*>(addresses::rbx_writevoxels + 5) = 0xC3;
    for (int offset = 6; offset < rbx_writevoxels_instsize; offset++) {
        *reinterpret_cast<BYTE*>(addresses::rbx_writevoxels + offset) = 0x90;
    }
    VirtualProtect((void*)(addresses::rbx_writevoxels), rbx_writevoxels_instsize, original_protection, &original_protection);
    //printf("patch\n");
    if (!bytecode_queue.empty()) {
        printf(XorString("WaitingHybridScriptsJob stepped\n"));
        for (std::string bytecode : bytecode_queue) {
            objects::instance* datamodel = get_datamodel();
            //printf(XorString("DataModel %p\n"), datamodel);
            bool retried = false;
        FindScript:
            printf(XorString("Searching recursively for a usable script...\n"));
            objects::instance* script = instance::FindFirstChildOfClass(datamodel, XorString("LocalScript"), true, script_ignore_list);
            if (!script) {
                if (script_ignore_list.size() > 0) {
                    printf(XorString("Script blacklist of %d scripts will be cleared\n"), script_ignore_list.size());
                    script_ignore_list.clear();
                    goto FindScript;
                } else {
                    printf(XorString("[ERROR] Game has no scripts... Try another game or execute again when a script exists\n"));
                    continue;
                }
            }
            printf(XorString("Found a script: %s (%p)\n"), instance::GetFullName(script).c_str(), script);
            script_ignore_list.push_back(script);
            compiled = bytecode;
            objects::instance* runtimescriptservice = instance::FindFirstChildOfClass(datamodel, XorString("RuntimeScriptService"));
            //printf(XorString("RuntimeScriptService: %p\n"), runtimescriptservice);

            // Detour to set identity (identity is just passed as an argument to addscript)
            //printf(XorString("Hooking AddScript\n"));
            if (*reinterpret_cast<BYTE*>(addresses::rbx_addscript) == 0xE9) {
                printf(XorString("[ERROR] AddScript already hooked!\n"));
                continue;
            }
            UINT relative_jump_addscript = (UINT)addscript_hook - addresses::rbx_addscript - 5;
            old_addscript = *reinterpret_cast<patch_5*>(addresses::rbx_addscript);
            VirtualProtect((void*)addresses::rbx_addscript, 5, PAGE_EXECUTE_READWRITE, &original_protection);
            *reinterpret_cast<BYTE*>(addresses::rbx_addscript) = 0xE9;
            *reinterpret_cast<UINT*>(addresses::rbx_addscript + 1) = relative_jump_addscript;
            VirtualProtect((void*)addresses::rbx_addscript, 5, original_protection, &original_protection);

            // Detour to set bytecode
            //printf(XorString("Detouring Deserializer\n"));
            if (*reinterpret_cast<BYTE*>(addresses::rbx_deserializer_detour) == 0xE9) {
                printf(XorString("[ERROR] Deserializer already detoured!\n"));
                continue;
            }
            UINT relative_jump_deserializer_detour = (UINT)deserializer_detour - addresses::rbx_deserializer_detour - 5;
            old_deserializer_detour = *reinterpret_cast<patch_5*>(addresses::rbx_deserializer_detour);
            VirtualProtect((void*)addresses::rbx_deserializer_detour, 5, PAGE_EXECUTE_READWRITE, &original_protection);
            *reinterpret_cast<BYTE*>(addresses::rbx_deserializer_detour) = 0xE9;
            *reinterpret_cast<UINT*>(addresses::rbx_deserializer_detour + 1) = relative_jump_deserializer_detour;
            VirtualProtect((void*)addresses::rbx_deserializer_detour, 5, original_protection, &original_protection);

            // Detour to bypass hashcheck (sets the bytecode back to normal, so the game thinks the hashes match and you won't be kicked)
            //printf(XorString("Detouring HashCheck\n"));
            if (*reinterpret_cast<BYTE*>(addresses::rbx_deserializer_detour_2) == 0xE9) {
                printf(XorString("[ERROR] HashCheck already detoured!\n"));
                continue;
            }
            UINT relative_jump_deserializer_detour_2 = (UINT)deserializer_detour_2 - addresses::rbx_deserializer_detour_2 - 5;
            old_deserializer_detour_2 = *reinterpret_cast<patch_5*>(addresses::rbx_deserializer_detour_2);
            VirtualProtect((void*)addresses::rbx_deserializer_detour_2, 5, PAGE_EXECUTE_READWRITE, &original_protection);
            *reinterpret_cast<BYTE*>(addresses::rbx_deserializer_detour_2) = 0xE9;
            *reinterpret_cast<UINT*>(addresses::rbx_deserializer_detour_2 + 1) = relative_jump_deserializer_detour_2;
            VirtualProtect((void*)addresses::rbx_deserializer_detour_2, 5, original_protection, &original_protection);

            printf(XorString("Calling RunScript\n"));
            functions::runtimescriptservice::rbx_runscript(runtimescriptservice, 0, script);
            if (deserializer_called) {
                printf(XorString("Execution cycle completed successfully\n"));
            } else {
                printf(XorString("Execution cycle failed (script already in use)\n"));
                VirtualProtect((void*)addresses::rbx_addscript, 5, PAGE_EXECUTE_READWRITE, &original_protection);
                *reinterpret_cast<patch_5*>(addresses::rbx_addscript) = old_addscript;
                VirtualProtect((void*)addresses::rbx_addscript, 5, original_protection, &original_protection);
                VirtualProtect((void*)addresses::rbx_deserializer_detour, 5, PAGE_EXECUTE_READWRITE, &original_protection);
                *reinterpret_cast<patch_5*>(addresses::rbx_deserializer_detour) = old_deserializer_detour;
                VirtualProtect((void*)addresses::rbx_deserializer_detour, 5, original_protection, &original_protection);
                VirtualProtect((void*)addresses::rbx_deserializer_detour_2, 5, PAGE_EXECUTE_READWRITE, &original_protection);
                *reinterpret_cast<patch_5*>(addresses::rbx_deserializer_detour_2) = old_deserializer_detour_2;
                VirtualProtect((void*)addresses::rbx_deserializer_detour_2, 5, original_protection, &original_protection);
                if (!retried) {
                    retried = true;
                    printf(XorString("Retrying once...\n"));
                    goto FindScript;
                }
            }
            deserializer_called = false;
        }
        bytecode_queue.clear();
    }
    return original_waiting_hybrid_scripts_job(self, fakearg, a2);
}

void move_job(std::vector<std::shared_ptr<objects::job>>& jobs, std::string job_name, int index) {
    std::shared_ptr<objects::job> job = nullptr;
    for (std::shared_ptr<objects::job> possible_job : jobs) {
        if (possible_job.get()->name == job_name) {
            job = possible_job;
        }
    }
    if (!job) {
        printf(XorString("[ERROR] %s couldn't be found in stage 1\n"), job_name.c_str());
        return;
    }
    auto it = std::find(jobs.begin(), jobs.end(), job);
    if (it == jobs.end()) {
        printf(XorString("[ERROR] %s couldn't be found in stage 2\n"), job_name.c_str());
        return;
    }
    jobs.erase(it);
    jobs.insert(jobs.begin() + index, job);
    printf(XorString("Moved %s to jobs[%d]\n"), job_name.c_str(), index);
}

void on_inject() {
    printf(XorString("Saving original bytes\n"));
    for (int offset = 0; offset < rbx_writevoxels_instsize; offset++) {
        printf(XorString("%X + %d\n"), addresses::rbx_writevoxels, offset);
        original_writevoxels_bytes[offset] = *reinterpret_cast<BYTE*>(addresses::rbx_writevoxels + offset);
    }
    printf(XorString("Getting jobs\n"));
    std::vector<std::shared_ptr<objects::job>> jobs = get_jobs();
    printf(XorString("Moving AC job\n"));
    // Bypassing memcheck by making every job think the memory is unchanged (because it is when their checks run)
    move_job(jobs, XorString("US14116"), 0); // This job is hooked and reverts the patches as if nothing happened
    //move_job(jobs, XorString("WaitingHybridScriptsJob"), 1); // This job is hooked and brings the patches back edit: no need to hook this as its always after and messes things up
    for (std::shared_ptr<objects::job> job : jobs) {
        printf(XorString("Job %p: %s\n"), job.get(), job.get()->name.c_str());
    }
    functions::rbx_getscheduler()->jobs = jobs;
    for (std::shared_ptr<objects::job> job : jobs) {
        if (job.get()->name == std::string(XorString("WaitingHybridScriptsJob"))) {
            printf(XorString("Found WaitingHybridScriptsJob: %p\n"), job.get());
            original_waiting_hybrid_scripts_job = reinterpret_cast<job_step_func>(job.get()->vftable[2]);
            hook_vftable(job.get(), 2, (UINT)waiting_hybrid_scripts_job_hook);
            printf(XorString("Hooked WaitingHybridScriptsJob\n"));
        } else if (job.get()->name == std::string(XorString("US14116"))) {
            printf(XorString("Found US14116: %p\n"), job.get());
            original_US14116 = reinterpret_cast<job_step_func>(job.get()->vftable[2]);
            hook_vftable(job.get(), 2, (UINT)US14116_hook);
            printf(XorString("Hooked US14116\n"));
        }
    }
    printf(XorString("Printing types\n"));
    for (int i = 0; i < 10; i++) {
        printf(XorString("Type %d: %s\n"), i, functions::luau::type_name(0, i));
    }
    for (int i = 0; i < 20; i++) {
        printf(XorString("Event %d: %s\n"), i, functions::luau::type_name(0, i + 10));
    }
    const char* pipe_name = (std::string(XorString("\\\\.\\pipe\\TaaprWare2_Pipe")) + std::to_string(GetProcessId(GetCurrentProcess()))).c_str();
    std::string whole_script = "";
    HANDLE pipe;
    char* buffer = new char[0xFFFFFF];
    DWORD dword_read;
    pipe = CreateNamedPipeA(
        pipe_name,
        PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
        PIPE_WAIT,
        1,
        0xFFFFFF,
        0xFFFFFF,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL
    );
    printf(XorString("Listening to named pipe \"%s\"\n"), pipe_name);
    std::cout << XorString(R"(
    %%%%%######(,%%%%%%%%%%%%%,*,,%%%#%%%%####%######%%%%%% 
         %%%%%%#(*%%%%%%/&%%%%%*%%%%%%/&%%%####%%#%%%%      
            %%#####%&,*,,,,###%###((...%&%%%#,.,%%          
             %%%%%%%%%%%%%%%%%%%%%%%%%%#(/*...,%#           
               %%%%&&&&&&&%%&%%%%%%&&&&&%%%%(/*             
              %%%&&&&&&%#%&%##(####((##&&&&&%%#(/           
            %&&&&%#####(*/(#(/,/.(/*/((&%%%##&&%%#          
          &&&&&&##//((&(**,,,...  *,/(,*(#(/(##&&&%#        
        %&&&&%%%%%%%%&&&&&&&%%%%%%%%%&&&&&%%%%%%%%&%%%#     
        &&&&&&&&&&#((@@@@@@@&&&%%%&&@@@@&(((&&&&&&&&&%      
        &&&&&&&&&&&&@@(@&%******@@&&&&%%%%&%%&&&&&&&&&      
        &&&&&&&&&&(,*@@%*&*/****/*(,&@@&&&&&/&&&&&&&&&      
        &&&&&&&&&/************************,%/,&&&&&&&&      
  ###(((&&&&&&&&&**************************//*&&&&&&&&((((((
((       /&&&&&&&************************/**/*&&&&&&/(//(((/
.. //*,,,,,,,((/  *******************/(*#***  (((((((//(((//
######((((#((#(((#((  @&%%%%%&&@@%/*****. .#((((/////*//****
(((((((///**((/(((*   ******************,* .#(#((((((/////**
((((/**/*,*,     ../#%******************%%(**,,,...***/////*
.....,,,***//((#%%%%%%((///******////(#@&%%%&%##(//***,,,...
%%%%%%&&&&&%%%&%%%%%%%&%(//******///(&&@&%%%%%%%%%%%%%%%%%%#
&&%%%%%%%%%%%%%%%%%%%%%%%%%(//*//(%%%%&@&%%%%%%%%%%%%####%%%
&&%%%%%%%%%%%%#%%%%%%%&%%%%%%%%%%%%%%%&@&&%%%%%%%%%%%%%%%%%%
&&&%%%%%%%%%%%%%%%%%%%&%%%%%%%%%%%%%%%&@@&%&%%%%%%%%%%%%%%%%

)");
    printf(XorString("Injection completed!\n"));
    while (pipe != INVALID_HANDLE_VALUE) {
        whole_script = "";
        if (ConnectNamedPipe(pipe, NULL) != FALSE) {
            while (ReadFile(pipe, buffer, sizeof(buffer) - 1, &dword_read, NULL) != FALSE) {
                buffer[dword_read] = '\0';
                whole_script = whole_script + buffer;
                if (dword_read < sizeof(buffer) - 1) {
                    break;
                }
            }
            PurgeComm(pipe, PURGE_RXCLEAR | PURGE_TXCLEAR);
            printf(XorString("------------------ TaaprWare V2 ------------------\nCompiling\n"));
            roblox_encoder encoder{};
            whole_script = whole_script;
            compiled = Luau::compile(whole_script, {}, {}, &encoder);
            UINT8 version = compiled.c_str()[0];
            if (version < LBC_VERSION_MIN || version > LBC_VERSION_MAX) {
                functions::rbx_printf(Error, XorString("[TaaprWare V2] Syntax Error: %s"), compiled.c_str() + 1);
                printf(XorString("Compile error (check roblox output)\n"));
                continue;
            }
            bytecode_queue.push_back(compiled);
            printf(XorString("Added to queue\n"));
        }
        DisconnectNamedPipe(pipe);
    }
    delete &buffer[0xFFFFFF];
    printf(XorString("Goodbye"));
}

BOOL __stdcall fake_FreeConsole() {
    printf(XorString("FreeConsole called\n"));
    return TRUE;
}

DWORD freeconsole_original_protection;
bool __stdcall DllMain(HMODULE hModule, unsigned int ul_reason_for_call, void* lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        utils::hook_free_console();
        AllocConsole();
        FILE* file_stream;
        freopen_s(&file_stream, XorString("CONIN$"), "r", stdin);
        freopen_s(&file_stream, XorString("CONOUT$"), "w", stdout);
        freopen_s(&file_stream, XorString("CONOUT$"), "w", stderr);
        SetConsoleTitleA("TaaprWare V2.4.4 Internal");
        printf(XorString("Hi\n"));
        unpatch_SetUnhandledExceptionFilter();
        printf(XorString("Unpatched SetUnhandledExceptionFilter %p\n"), SetUnhandledExceptionFilter);
        SetUnhandledExceptionFilter(TaaprWareExceptionFilter);
        printf(XorString("Set exception filter\n"));
        update_hwid();
        printf(XorString("Got HWID: %s\n"), hwid.c_str());
        printf(XorString("-----Credit-----\n"));
        printf(XorString("https://github.com/TheSeaweedMonster - ScriptStart execution method and Lua custom functions\n"));
        printf(XorString("https://github.com/NezyDev - Roblox bytecode compression for loadstring function\n"));
        printf(XorString("---Credit End---\n"));
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)on_inject, NULL, NULL, NULL);
	}
	return true;
}
