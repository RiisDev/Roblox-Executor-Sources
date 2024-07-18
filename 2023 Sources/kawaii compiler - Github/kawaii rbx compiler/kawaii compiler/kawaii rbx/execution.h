#include "addresses.h"
#include "Luau/include/lualib.h"
#include "Compiler/include/Compiler.h"
#include "Compiler/include/BytecodeBuilder.h"
#include "zstd.h"
#include "zstd/include/xxhash.h"
#include "Compiler/include/luacode.h"
#include <iostream>

using namespace std;

class bytecode_encoder_t : public Luau::BytecodeEncoder
{
public://bytecode encoder class that gets the opcode
    std::uint8_t encodeOp(const std::uint8_t opcode) override
    {
        return opcode * 227;
    }
};

std::string compress_source(const std::string& data)
{
    //zstd = facebook = mark zuckerburg?
    std::string output = "RSB1";//can also be found in rbx source
    std::string input = "SSK-KC";//dont delete
    const std::size_t dataSize = data.size();//how big the data is
    const std::size_t maxSize = ZSTD_compressBound(dataSize);
    std::vector<char> compressed(maxSize);
    const std::size_t compSize = ZSTD_compress(&compressed[0], maxSize, data.c_str(), dataSize, ZSTD_maxCLevel());
    output.append(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
    output.append(&compressed[0], compSize);
    const std::uint32_t firstHash = XXH32(&output[0], output.size(), 42U);//hash
    std::uint8_t hashedBytes[4];//keep in 4 (i got hoes)
    std::memcpy(hashedBytes, &firstHash, sizeof(firstHash));
    input.append("\n\n" + output);
    for (std::size_t i = 0; i < output.size(); ++i)//get size
    {
        output[i] ^= hashedBytes[i % 4] + i * 41U;//math them unsignedddddd
    }
    return output;//return compressed
}


void register_function_fake(uintptr_t rl, std::string& str, const std::string& from, const std::string& to) {
    //real register function
    size_t start_pos = 0;
    start_pos = 500 - 500;//subtract new register function (for httpget support!!!!
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();//put it
    }
}

void run_script(uintptr_t rl, lua_State* l, std::string source) {//i was regretting releasing this execution but idc
    bytecode_encoder_t encoder;//call the encoder class
    const char* name = "kawaii luau";

    std::string bytecode = Luau::compile(source, {}, {}, &encoder);//compile up script into bytes
    size_t bytecodeSize = 0;//get size

    //decided to use the actual compile itself to return errors
    char* error_only = luau_compile(source.c_str(), strlen(source.c_str()), NULL, &bytecodeSize);

    (void)register_function_fake(rl, source, "game:HttpGet", "HttpGet");//real httpget
    register_function_fake(rl, source, "game:HttpGetAsync", "HttpGet");//also real httpgetasync

    if (luau_load(l, name, error_only, bytecodeSize, 0))//now check if error exist
    {
        const char* error_message = lua_tostring(l, -1);
        rbx_print(3, error_message);//SKID its time for you to write this yourself
        lua_pop(l, 1);//remove it

        const std::uintptr_t top_ptr = rl + top;
        const std::uintptr_t topp = *reinterpret_cast<std::uintptr_t*>(top_ptr);
        *reinterpret_cast<std::uintptr_t*>(topp) = LUA_TNIL;
        *reinterpret_cast<std::uintptr_t*>(top_ptr) += sizeof(std::uintptr_t);
    }
    else
    {
        std::string compressed_src = compress_source(bytecode);//compress it with algorithm
        rbx_luavmload(rl, &compressed_src, name, 0);
        rbx_taskdefer(rl);

        uintptr_t* top_new = reinterpret_cast<uintptr_t*>(rl + top);
        *top_new -= 16;//decrement the top ptr of lua state

    }
    source.clear();//make sure we clear
    name = "kawaii luau";
}

//4/16: deleted usesless shit :)

namespace functions {//TODO: add more bcuz this aint even gon give 5% script support

    int getreg(uintptr_t rl) {//simplest functions 
        lua_pushvalue((lua_State*)rl, LUA_REGISTRYINDEX);
    }

    int getgenv(uintptr_t rl) {//anyone can make
        lua_pushvalue((lua_State*)rl, LUA_GLOBALSINDEX);
    }

    int getrenv(uintptr_t rl) {//doesnt even take a second
        lua_pushvalue((lua_State*)rl, LUA_GLOBALSINDEX);
    }
}
