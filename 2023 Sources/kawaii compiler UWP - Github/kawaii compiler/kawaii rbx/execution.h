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

std::string rbx_compress(const std::string& data)
{
    //zstd = facebook = mark zuckerburg?
    std::string output = "RSB1";//can also be found in rbx source
    std::string input = "SSK-UWP";//dont delete
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

void rbx_runscript(uintptr_t rl, lua_State* l, std::string source) {//i was regretting releasing this execution but idc
    bytecode_encoder_t encoder;//call the encoder class
    const char* name = "kawaii luau";//chunkname

    source = "spawn(function()\n" + source + "\nend)";

    std::string bytecode = Luau::compile(source, {}, {}, &encoder);//compile up script into bytes
    size_t bytecodeSize = 0;//get size

    //decided to use the actual compile itself to return errors
    char* error_only = luau_compile(source.c_str(), strlen(source.c_str()), NULL, &bytecodeSize);

    if (luau_load(l, name, error_only, bytecodeSize, 0))//now check if error exist
    {
        const char* error_message = lua_tostring(l, -1);
        rbx_print(3, error_message);
        lua_pop(l, 1);//remove it
    }
    else
    {
        std::string compressed_src = rbx_compress(bytecode);//compress it with algorithm
        rbx_luavmload(rl, &compressed_src, name, 0);
        rbx_taskspawn(rl);

        uintptr_t* top_new = reinterpret_cast<uintptr_t*>(rl + top);
        *top_new -= 16;//decrement the top ptr of lua state

    }
    source.clear();//make sure we clear
}

void rbx_setidentity(uintptr_t rL, int identity) {//sets identity 
    uintptr_t* gs = reinterpret_cast<uintptr_t*>(rL + id1);
    int* id = reinterpret_cast<int*>(*gs + id2);
    *id = identity;
}

//4/16: deleted usesless shit :)
