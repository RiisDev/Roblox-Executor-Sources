#pragma once

#include <Windows.h>
#include <iostream>

enum ScanFlags { CALL_REL_32 = 1,
    SCAN_REVERSE = 2 };

namespace Memory {
template <class T>
void Write(void* location, T value)
{
    DWORD dwOldProtect;
    VirtualProtect(location, sizeof(T), PAGE_READWRITE, &dwOldProtect);
    *reinterpret_cast<T*>(location) = value;
    VirtualProtect(location, sizeof(T), dwOldProtect, &dwOldProtect);
}

class VTHook {
    void* baseClass;
    void** vt;
    void** backupvt;

public:
    size_t FunctionCount() const
    {
        size_t count = 0;
        while ((reinterpret_cast<void**>(this->vt))[count++])
            ;
        return count;
    }

    void ReplaceFunc(size_t idx, void* func, void* orig = 0)
    {
        if (orig) {
            *reinterpret_cast<void**>(orig) = vt[idx];
        }

        DWORD dwOldProtect;
        VirtualProtect(&vt[idx], sizeof(void*), PAGE_READWRITE, &dwOldProtect);
        vt[idx] = func;
        VirtualProtect(&vt[idx], sizeof(void*), dwOldProtect, &dwOldProtect);
    }

    VTHook(void* func)
    {
        this->baseClass = func;
        this->vt = *reinterpret_cast<void***>(func);

        size_t count = FunctionCount();
        this->backupvt = new void*[count];

        for (size_t i = 0; i < count; i++) {
            this->backupvt[i] = this->vt[i];
        }
    }
};

std::uintptr_t ResolveRelativeCall(const std::uintptr_t loc);
std::uintptr_t Scan(const std::uintptr_t start, const std::uintptr_t end,
    const std::string& pattern, const int flags = 0,
    const int offset = 0);
std::uintptr_t Scan(const std::string& pattern, const int flags = 0,
    const int offset = 0);
DWORD unprotect(DWORD addr);
} // namespace Memory
