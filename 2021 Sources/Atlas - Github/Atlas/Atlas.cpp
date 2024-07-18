#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "Memory.h"
#include "Addresses.h"
#include "Utility.h"
#include "Gui.h"
#include "Globals.h"

int main() {
    SetConsoleTitle((LPCWSTR)Utility::CreateRandomString(6).c_str());

    DWORD processID = Memory::GetProcessID(L"RobloxPlayerBeta.exe");
    if (!processID) {
        std::cout << "Couldn't get Roblox process ID." << std::endl;
        return 0;
    }

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (!handle) {
        std::cout << "Couldn't get Roblox handle." << std::endl;
        return 0;
    }
    Globals::Handle = handle;

    HWND hwnd = FindWindowA(0, "Roblox");
    Globals::RobloxHWND = hwnd;

    __int64 userID = 0;
    std::string teamName;

    DWORD baseAddress = Memory::GetModuleBaseAddress(processID, L"RobloxPlayerBeta.exe");
    Globals::BaseAddress = baseAddress;

    std::cout << "Welcome to Atlas. Scanning..." << std::endl;

    DataModel game = Memory::Scan(baseAddress, baseAddress + Addresses::DataModel);
    Players players = Players(game.GetService("Players").Address);
    Player localPlayer = players.GetLocalPlayer();
    Instance character = localPlayer.GetCharacter();
    Instance team = localPlayer.GetTeam();

    VisualEngine visualEngine = Memory::Scan(baseAddress, baseAddress + Addresses::VisualEngine);

    Globals::PlayersService = players;
    Globals::LocalPlayer = localPlayer;
    Globals::VisualEngine = visualEngine;

    userID = localPlayer.GetUserID();

    if (team.Address) {
        teamName = team.GetName();
    } else {
        teamName = "None";
    }

    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Base address: " << std::hex << baseAddress << std::dec << std::endl;
    std::cout << "DataModel address: " << std::hex << game.Address << std::dec << std::endl;
    std::cout << "Players address: " << std::hex << players.Address << std::dec << std::endl;
    std::cout << "LocalPlayer address: " << std::hex << localPlayer.Address << std::dec << std::endl;
    std::cout << "Character address: " << std::hex << character.Address << std::dec << std::endl;
    std::cout << "VisualEngine address: " << std::hex << visualEngine.Address << std::dec << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    std::cout << "User ID: " << userID << std::endl;
    std::cout << "Username: " << localPlayer.GetName() << std::endl;
    std::cout << "Team: " << teamName << std::endl;

    Gui gui;
    gui.Setup();

    //system("pause");
}