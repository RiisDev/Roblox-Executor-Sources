#pragma once
#include <Windows.h>
#include "XorString.hpp"
#include <string>
#define log_addy_start 0xB838D
int WINAPI h_MessageBox(
    _In_opt_ HWND    hWnd,
    _In_opt_ LPCTSTR lpText,
    _In_opt_ LPCTSTR lpCaption,
    _In_     UINT    uType
) {
    std::string str_lp = lpText;
    std::string str_cap = lpCaption;
    if (str_cap == "Roblox Crash") {
        DWORD log_start = log_addy_start + (DWORD)GetModuleHandleA(NULL);
        DWORD old;
        for (int i = 0; i < 79; i++) {
            VirtualProtect((LPVOID)(log_start + i), 1, PAGE_EXECUTE_READWRITE, &old);
            *(char*)(log_start + i) = 0x90;
            VirtualProtect((LPVOID)(log_start + i), 1, old, &old);
        }
    }
    wchar_t* str_lp_res = new wchar_t[4096];
    wchar_t* str_cap_res = new wchar_t[4096];
    OBFUSCATE_STR(title, "Lua Crash");
    OBFUSCATE_STR(message, "Environment crashed - but don't worry, no logs were sent");
    MultiByteToWideChar(CP_ACP, NULL, message.decrypt(), -1, str_lp_res, 4096);
    MultiByteToWideChar(CP_ACP, NULL, title.decrypt(), -1, str_cap_res, 4096);
    return MessageBoxW(hWnd, str_lp_res, str_cap_res, uType);
}


void LoadLogBypass() {
    DWORD o;
    VirtualProtect((LPVOID)&MessageBoxA, 1, PAGE_EXECUTE_READWRITE, &o);
    *(char*)(&MessageBoxA) = 0xE9;
    *(DWORD*)((DWORD)&MessageBoxA + 1) = ((DWORD)&h_MessageBox - (DWORD)&MessageBoxA) - 5;
    VirtualProtect((LPVOID)&MessageBoxA, 1, o, &o);
}