#include <Windows.h>
#include <cstdio>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>
#include "Lua.h"
#include "CallCheck.h"
#include "Functions.h"
#include "Execution.h"
#include "Settings.h"

using namespace CallCheck;

void ConsoleBypass() {

    VirtualProtect(FreeConsole, 1, PAGE_EXECUTE_READWRITE, new unsigned long);
    *reinterpret_cast<unsigned int*>(FreeConsole) = 0xC3;
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 50, 20, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    ShowWindow(GetConsoleWindow(), 1);
    SetConsoleTitleA("ExploitNameHere | Lua Bytecode Interperter");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void EntryPoint() {
    
    ConsoleBypass();

    std::cout << ConsoleIntro << "Getting Lua State...\n";
    DWORD DataModel = ReturnDataModel();
    DWORD ScriptContext = FindFirstClass(DataModel, "ScriptContext");
    rL = r_newthread(RBX_LuaState(ScriptContext));
    std::cout << ConsoleIntro << "Setting Context Level...\n";
    *GetLevel(rL) = 7;
    std::cout << ConsoleIntro << "Initializing Lua Bytecode Interperter...\n";
    BytecodeInit();
    CallCheck::Init();
    std::cout << ConsoleIntro << "Registering Lua Functions...\n";
    RegisterLuaFunctions();
    std::cout << ConsoleIntro << "Ready For Execution\n";
    std::cout << "\n";
    Sleep(500);
    while (true)
    {
        std::cout << " > ";
        std::string InputScript;
        std::getline(std::cin, InputScript);
        Execute(rL, InputScript);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        std::thread(EntryPoint).detach();
        break;
    }
    return TRUE;
}

