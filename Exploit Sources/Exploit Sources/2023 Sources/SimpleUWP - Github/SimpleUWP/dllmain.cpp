__pragma(code_seg(push, s1, ".SynapseX")) // MAKE EXLPOIT SAME LEVELL AS SYNAPSEX

#include "execution.h"
#include <thread>

// NOTE: THIS DLL WAS MADE FOR LEARNING PURPOUSES SO I MADE IT UNDERSTANABLE
// LICENSED UNDER NOSKID LICENSE THAT MEANS IF YOU WILL USE THIS SOURCE IN YOUR EXXPLOIT YOUR FEDERAL PROTECTION WILL BE TURNED OFF AND YOU WILL BE IN JAIL

void skid()
{
    // SAY THAT I MADE THE BEST EXPLOIT SO YEAS BE MY SLAEV
    luau::print(1, "The NEST ROBLOX EXPLOIT LOADED!");
    // FINALLY GET THE MAGIC
    std::uintptr_t Fack = TheMagiec::getMagicRobloxThingy();
    // SET OUR LEVEL TO THE MAX LEVEL SO YOU WILL 100% WILL NOT BE DETECTED BY FEDERALS
    *reinterpret_cast<int*>(*reinterpret_cast<std::uintptr_t*>(Fack + offsets::smell_fart::cosmos) + offsets::cosmos::universe_level) = 8; // SET OUR SECURITY LELV
    // GET THE BITHCES!!!
    TheMagiec::DoTheMainMagicViaRobloxUninliedFunctionsAndSomeCompilingWithCompressionsAndTopDecremention(Fack, "printidentity()");

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // if we cumming into roblox then create cum thread
    if (ul_reason_for_call == CUM_ENTRY)
        std::thread(skid).detach();
    // return that everything is fine and we cummed into roblox successfully
    return TRUE;
}