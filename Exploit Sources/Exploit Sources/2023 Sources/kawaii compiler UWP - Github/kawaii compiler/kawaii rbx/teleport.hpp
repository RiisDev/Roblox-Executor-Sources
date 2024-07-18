#include "scheduler.h"

/*
* this is the teleport handler and its REALLY bad i mean it.
* there are WAYYY better ways to do this (such as using datamodel)
* 
* this is the cheapest/reliable/working way and doesnt require addresses
*/

bool teleported;//keep it in one boolean so we can detect

uintptr_t rbx_checkteleport() {//ay SKID its YOUR (thats right!) TIME to make this loop yourself!

    int rbx_null = 0x0;
    uintptr_t rbx_newaddress = rbx_getstate(get_script_context(), &rbx_null);//grab the NEW address from the job

    if (rl == rbx_newaddress)//this means that rl already has the address so no tp
    {
        teleported = false;
    }
    else {//teleport detected/inprogress
        teleported = true;

        uintptr_t script_context_address = get_script_context();
        uintptr_t roblox_state_address = rbx_getstate(script_context_address, &rbx_null);

        __asm {
            mov eax, roblox_state_address
            mov rl, eax
        }

        int roblox_identity = 7;
        rbx_setidentity(rl, roblox_identity); 

        rbx_print(0, "kawaii - teleport success");
    }
}
