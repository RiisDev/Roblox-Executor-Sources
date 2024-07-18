#pragma once

#include <windows.h>
#include <vector>
#include <limits.h>
#include <exception>
#include <iostream>
#include <unordered_map>

typedef std::unordered_map<unsigned int, unsigned int> ccMap; 
ccMap ccCache; 

const unsigned int exists(unsigned int addr) {
    for (ccMap::const_iterator it = ccCache.begin(); it != ccCache.end(); ++it) {
        if (it->second == addr) return it->first;
    }
    return 0;
}

LONG __stdcall ccVEH(PEXCEPTION_POINTERS code) { 
    if (code->ExceptionRecord->ExceptionCode == 0x80000003L)
    {
        code->ContextRecord->Eip = ccCache.at(code->ContextRecord->Eip);
        return -1;
    }
}

unsigned int currentInt3PbIteration = 0; 

unsigned int GenerateSafeAddress(unsigned int addr) 
{
    const unsigned int e0 = exists(addr);
    if (e0 != 0) return e0;

    static const unsigned int startAddr = reinterpret_cast<unsigned int>(GetModuleHandleA(NULL));

    static const byte int3bpSignature[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };

    unsigned int fndInt3BpAddr = 0x0;

    while (currentInt3PbIteration < (INT_MAX - 1)) {
        ++currentInt3PbIteration;
        if (memcmp((void*)(startAddr + currentInt3PbIteration), int3bpSignature, sizeof(int3bpSignature)) == 0) {
            ccCache.insert(ccMap::value_type(startAddr + currentInt3PbIteration, addr));
            fndInt3BpAddr = startAddr + currentInt3PbIteration;
            break;
        }
    }

    return fndInt3BpAddr;
}

namespace CallCheck
{
    void Init()
    {
        AddVectoredExceptionHandler(1, ccVEH);
    }
    void RegisterFunction(DWORD rL, int function, const char* name) {
        r_pushcclosure(rL, GenerateSafeAddress(function), 0, 0, 0);
        r_setfield(rL, -10002, name);
    }
}
