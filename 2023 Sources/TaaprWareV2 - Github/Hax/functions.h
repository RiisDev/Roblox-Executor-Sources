#pragma once

#include <iostream>

#include "Luau\Compiler.h"
#include "Luau\BytecodeBuilder.h"
#include "addresses.h"
#include "rlapi.h"
#include "file.h"
#include "fps.h"
#include "rbxinstance.h"
#include "AUMB_utils\utils.hpp"
#include "xorstr.h"
#include "include\xxhash.h"
#include "include\zstd.h"
#include "hwid.h"

#define RELEASE

class roblox_encoder : public Luau::BytecodeEncoder {
    std::uint8_t encodeOp(const std::uint8_t opcode) {
        return opcode * 227;
    }
};

extern CONTEXT context_record;
extern BYTE* int3_address; // int3 is the 0xCC byte and it causes an exception
extern std::vector<std::string> bytecode_queue;

int c_function_handler(UINT rL);
