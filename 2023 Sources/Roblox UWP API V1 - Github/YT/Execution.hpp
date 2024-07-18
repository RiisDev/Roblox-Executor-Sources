#include <iostream>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Networking.Sockets.h>
#include <winrt/Windows.Storage.Streams.h>
#include <codecvt> 
#include <Windows.h> 

#include "API.hpp"
#include "Compiler/include/luacode.h"

namespace Execution
{
    void run_script(uintptr_t rl, lua_State* l, const std::string& source)
    {
        bytecode_encoder_t enc;
        const char* name = "";
        std::string bytecode = Luau::compile("spawn(function()\n" + source + "\nend)", {}, {}, &enc);
        size_t bytecodeSize = 0;

        char* error_only = luau_compile(source.c_str(), strlen(source.c_str()), NULL, &bytecodeSize);
        if (bytecode.at(0) != 0)
        {
            if (luau_load(l, name, error_only, bytecodeSize, 0))
            {
                const char* error_message = lua_tostring(l, -1);
                Addresses::r_lua_print(3, error_message);
                lua_pop(l, 1);
            }
            else
            {
                std::string compressed = compress(bytecode);
                Addresses::r_luavm_load(rl, &compressed, "", 0);
                Addresses::r_taskdefer(rl);

                uintptr_t* top_new = reinterpret_cast<uintptr_t*>(rl + Offsets::lua_state_top);
                *top_new -= 16;
            }
        }
    }

    void Execute_Function()
    {
        std::string directoryPath = std::string(getenv("USERPROFILE")) + "\\AppData\\Local\\Packages\\ROBLOXCORPORATION.ROBLOX_55nm5eh3cm0pr\\AC";
        std::string filename = "Execute.lua";
        std::filesystem::path filePath = directoryPath + "\\" + filename;

        while (true) {
            if (std::filesystem::exists(filePath)) {
                std::ifstream file(filePath);
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();

                run_script(exthread, (lua_State*)luaL_newstate(), content);

                std::this_thread::sleep_for(std::chrono::milliseconds(10));

                std::filesystem::remove(filePath);
            }
        }
    }
}
