// windows includes
#include <Windows.h>

// I got this mapped out crap from the leaked roblox from 2016
// https://github.com/ui0ppk/roblox-master
typedef enum {
    MESSAGE_OUTPUT,
    MESSAGE_INFO,
    MESSAGE_WARNING,
    MESSAGE_ERROR,
    MESSAGE_SENSITIVE,
    MESSAGE_TYPE_MAX
} MessageType;

// black magic i wrote
template <typename R, typename... Args>
R CallFunc(uintptr_t func, Args... args) {
    return ((R(*)(Args...))func)(args...);
}

// roblox base address
uintptr_t roblox = (uintptr_t)GetModuleHandleA("Windows10Universal.exe");

// wrapper for print
void R_Print(MessageType type, const char* message) {
    return CallFunc<void, int, const char*>(roblox + 0xB568A0, type, message);
    Sleep(1);
}

// i wonder what this does :SCRAER<M:!@#$
void InitClient() {
    //f9 to see these
    R_Print(MESSAGE_OUTPUT, "Hello, world!");
    R_Print(MESSAGE_INFO, "Hello, world!");
    R_Print(MESSAGE_WARNING, "Hello, world!");
    R_Print(MESSAGE_ERROR, "Hello, world!");

    // these two dont print to the roblox console they r for actual
    // development or smth idk tbh but its used back in the 2016 code
    R_Print(MESSAGE_SENSITIVE, "Hello, world!");
    R_Print(MESSAGE_TYPE_MAX, "Hello, world!");

}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    // create new init client thread when dll gets injected into process like ur mother and me last night
    if (reason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InitClient, module, 0, 0);
    }
    return TRUE;
}

