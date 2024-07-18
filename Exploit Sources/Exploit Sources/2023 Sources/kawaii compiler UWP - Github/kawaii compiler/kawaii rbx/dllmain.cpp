#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <string>
#include <cstdint>
#include <thread>
#include "scheduler.h"
#include <tlhelp32.h>
#include "execution.h"
#include "teleport.hpp"
#include "roblox.hpp"

// UWP edition of kawaii compiler n some thanks to GRH (gave motivation WTF)
// WINAPI functions == dead you cant use pipes/console == stick to no windows function == C++ livez

bool load() {//main loading point

    /*
    OK so the thing is I cant use console or messageboxes. this really disrupts your execution
    BUT there are other methods [such as calling an external UI or perhaps...even in Lua?]

    instead of using allocconsole or msgboxaaa I decided to just print it out in the dev console.
    soooo that means the 1st thing you got to upd8 is the print address. there are more ways, but this is the
    only one that crosses my mind at this point...
    
    */

    rbx_print(0, "kawaii uwp - loaded!");

    uintptr_t script_context_address = get_script_context();//get the script context
    rbx_print(0, "got script context!");
    int roblox_nothing = NULL;//getstate -> case default
    uintptr_t roblox_state_address = rbx_getstate(script_context_address, &roblox_nothing);//pass down to gs lua state
    rbx_print(0, "got lua state!");

    __asm {
        mov eax, roblox_state_address//now rl will hold the roblox state address recieved from getstate->scriptcontext
        mov rl, eax
    }

    int roblox_identity = 7;//your identity but dont make it ODD (some scripts like to do an identity check)
    rbx_setidentity(rl, roblox_identity); //sets the new identity to level 7
    rbx_print(0, "set identity to 7!");

    rbx_checkteleport();//read teleport.hpp

    //means everything is passed (good)
    rbx_print(3, "fluxus loves the taste of NUTS\ndont switch to delta now\nkrnl needs bitchez\nnihon is japanese\ni dont like to inhale oxygen\nkawaii compiler only  W");
    
    //you can run scripts now.
    //also skid: i have already figured out a way of sending scripts to roblox
    //but i wont show it or give it, so TODO!

    rbx_runscript(rl, (lua_State*)luaL_newstate(), "print('Welcome, noobb!!')");
    return true;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        __asm { call load } //imagine using assembly for simple calling.
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}