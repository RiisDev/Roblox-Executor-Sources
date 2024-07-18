#pragma once
#include <iostream>
#include <mutex>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <filesystem>
#include <minwinbase.h>

bool teleport = false;


std::string ReadFromFile(const char* filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));
        file.close();
        return content;
    }
    return "";
}

int autoexec() {
    std::string Path = std::string(getenv("USERPROFILE")) + "\\AppData\\Local\\Packages\\ROBLOXCORPORATION.ROBLOX_55nm5eh3cm0pr\\AC\\autoexec";
    for (const auto& entry : std::filesystem::directory_iterator(Path)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();
            std::string extension = filePath.substr(filePath.find_last_of(".") + 1);
            if (extension == "text" || extension == "txt" || extension == "lua") {
                std::string readContent = ReadFromFile(filePath.c_str());
                Execution::run_script(exthread, (lua_State*)luaL_newstate(), readContent.c_str());
              //Sleep(100);
            }
        }
    }
}

void update_Roblox_State1_in()
{
    exthread = (uintptr_t)LuaState::get_robloxstate();
    Lua::setlevel(exthread, 8);
    Addresses::r_lua_print(0, "YT Ready");
    autoexec();
}

void detected_teleport_in() {
    if (exthread != LuaState::get_robloxstate()) {
        if (!teleport) {
            teleport = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(6000));
            update_Roblox_State1_in();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            teleport = false;
        }
    }
    else {
        teleport = false;
    }
}

void start_in() {
    while (true) {
        if (!teleport) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            detected_teleport_in();
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }
    }
}
