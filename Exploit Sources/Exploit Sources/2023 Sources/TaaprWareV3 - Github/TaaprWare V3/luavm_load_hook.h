#pragma once

#include "roblox.h"

// This allows us to perform classic bytecode conversion in 2023 after the compression changes :)
bool decompressed_luavm_load(uintptr_t state, std::string bytecode);