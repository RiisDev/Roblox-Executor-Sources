#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include "LuaState.hpp"


namespace Lua
{
    void setlevel(uintptr_t rl, int8_t identity)
    {
        uintptr_t address = *(uintptr_t*)(rl + 72);
        int8_t* identityPtr = reinterpret_cast<int8_t*>(address + 24); 
        *identityPtr = identity;
    }

    //void setlevel(uintptr_t rl, const char* newIdentity) {
    //    uintptr_t address = *(uintptr_t*)(rl + 72);
    //    int8_t* identityPtr = reinterpret_cast<int8_t*>(address + 24);

    //    strcpy(reinterpret_cast<char*>(identityPtr), newIdentity);
    //}
}
