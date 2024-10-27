#pragma once
#include "RBX.hpp"
#include <vector>
#include <cstdint>
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include "Libs/MinHook/MinHook.hpp"

// how to get offset look for string "cannot create event to secure" go up 8 lines to call ds:dword_14DC51c the .text at the side is the offset 
void BypassCallCheck();