#pragma once

#include <Windows.h>
#include <stdio.h>

#include "xorstr.h"

#define LUAVM_SHUFFLE_COMMA ,

#define LUAVM_SHUFFLE2(sep,a0,a1) a0 sep a1
#define LUAVM_SHUFFLE3(sep,a0,a1,a2) a2 sep a1 sep a0
#define LUAVM_SHUFFLE4(sep,a0,a1,a2,a3) a2 sep a1 sep a3 sep a0
#define LUAVM_SHUFFLE5(sep,a0,a1,a2,a3,a4) a0 sep a1 sep a2 sep a3 sep a4
#define LUAVM_SHUFFLE6(sep,a0,a1,a2,a3,a4,a5) a0 sep a1 sep a2 sep a3 sep a4 sep a5
#define LUAVM_SHUFFLE7(sep,a0,a1,a2,a3,a4,a5,a6) a0 sep a1 sep a2 sep a3 sep a4 sep a5 sep a6
#define LUAVM_SHUFFLE8(sep,a0,a1,a2,a3,a4,a5,a6,a7) a0 sep a1 sep a2 sep a3 sep a4 sep a5 sep a6 sep a7
#define LUAVM_SHUFFLE9(sep,a0,a1,a2,a3,a4,a5,a6,a7,a8) a0 sep a1 sep a2 sep a3 sep a4 sep a5 sep a6 sep a7 sep a8

typedef enum {
    ptrenc_none,
    ptrenc_add,
    ptrenc_xor,
} encryption;

UINT get_encrypted_ptr(encryption enc, UINT address_of_ptr);
void set_encrypted_ptr(encryption enc, UINT address_of_ptr, UINT value);

template <typename T>
class ptr_encryption_add {
public:
    operator const T() const {
        return (T)((uintptr_t)storage + reinterpret_cast<uintptr_t>(this));
    }
    void operator=(const T& value) {
        storage = (T)((uintptr_t)value - reinterpret_cast<uintptr_t>(this));
    }
    const T operator->() const {
        return operator const T();
    }
private:
    T storage;
};

template <typename T> class ptr_encryption_xor {
public:
    operator const T() const {
        return (T)((uintptr_t)storage ^ reinterpret_cast<uintptr_t>(this));
    }
    void operator=(const T& value) {
        storage = (T)((uintptr_t)value ^ reinterpret_cast<uintptr_t>(this));
    }
    const T operator->() const {
        return operator const T();
    }
private:
    T storage;
};