#pragma once
#include <Windows.h>

#define aslr(x)(x - 0x400000 + (DWORD)GetModuleHandleA(0))
#define ceaslr(x)(x - 0x13ED000 + (DWORD)GetModuleHandleA("RobloxPlayerBeta.exe"))