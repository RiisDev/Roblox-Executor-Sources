#pragma once

#include <Exploit/Offset/Offsets.hpp>

#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>

#if defined(_MSC_VER) && !defined(__clang__)
#define LUAU_FASTMATH_BEGIN __pragma(float_control(precise, off, push))
#define LUAU_FASTMATH_END __pragma(float_control(pop))
#else
#define LUAU_FASTMATH_BEGIN
#define LUAU_FASTMATH_END
#endif

#if (defined(__x86_64__) || defined(_M_X64)) && !defined(__SSE4_1__) && !defined(__AVX__)
#if defined(_MSC_VER) && !defined(__clang__)
#define LUAU_TARGET_SSE41
#elif defined(__GNUC__) && defined(__has_attribute)
#if __has_attribute(target)
#define LUAU_TARGET_SSE41 __attribute__((target("sse4.1")))
#endif
#endif
#endif

#if defined(__GNUC__)
#define LUA_PRINTF_ATTR(fmt, arg) __attribute__((format(printf, fmt, arg)))
#else
#define LUA_PRINTF_ATTR(fmt, arg)
#endif

#ifdef _MSC_VER
#define LUA_NORETURN __declspec(noreturn)
#else
#define LUA_NORETURN __attribute__((__noreturn__))
#endif

#ifndef LUA_API
#define LUA_API extern
#endif

#define LUALIB_API LUA_API

#if defined(__GNUC__)
#define LUAI_FUNC __attribute__((visibility("hidden"))) extern
#define LUAI_DATA LUAI_FUNC
#else
#define LUAI_FUNC extern
#define LUAI_DATA extern
#endif

#ifndef LUA_USE_LONGJMP
#define LUA_USE_LONGJMP 0
#endif

#ifndef LUA_IDSIZE
#define LUA_IDSIZE 256
#endif

#ifndef LUA_MINSTACK
#define LUA_MINSTACK 20
#endif

#ifndef LUAI_MAXCSTACK
#define LUAI_MAXCSTACK 8000
#endif

#ifndef LUAI_MAXCALLS
#define LUAI_MAXCALLS 20000
#endif

#ifndef LUAI_MAXCCALLS
#define LUAI_MAXCCALLS 200
#endif

#ifndef LUA_BUFFERSIZE
#define LUA_BUFFERSIZE 512
#endif

#ifndef LUA_UTAG_LIMIT
#define LUA_UTAG_LIMIT 128
#endif

#ifndef LUA_SIZECLASSES
#define LUA_SIZECLASSES 32
#endif

#ifndef LUA_MEMORY_CATEGORIES
#define LUA_MEMORY_CATEGORIES 256
#endif

#ifndef LUA_MINSTRTABSIZE
#define LUA_MINSTRTABSIZE 32
#endif

#ifndef LUA_MAXCAPTURES
#define LUA_MAXCAPTURES 32
#endif

#ifndef LUA_CUSTOM_EXECUTION
#define LUA_CUSTOM_EXECUTION 0
#endif

#define LUAI_USER_ALIGNMENT_T \
    union \
    { \
        double u; \
        void* s; \
        long l; \
    }

#ifndef LUA_VECTOR_SIZE
#define LUA_VECTOR_SIZE 3 
#endif

#define LUA_EXTRA_SIZE (LUA_VECTOR_SIZE - 2)


#define VM_SHUFFLE3(a1, a2, a3) a1; a3; a2; //23.06.2023
#define VM_SHUFFLE4(a1, a2, a3, a4) a3; a4; a2; a1; //23.06.2023
#define VM_SHUFFLE5(a1, a2, a3, a4, a5) a1; a5; a2; a3; a4; //23.06.2023
#define VM_SHUFFLE6(a1, a2, a3, a4, a5, a6) a2; a6; a4; a1; a5; a3; //23.06.2023
#define VM_SHUFFLE8(a1, a2, a3, a4, a5, a6, a7, a8) a6; a4; a1; a2; a8; a5; a7; a3; //23.06.2023
//#define VM_SHUFFLE9(a1, a2, a3, a4, a5, a6, a7, a8, a9) a7; a5; a1; a8; a3; a4; a9; a2; a6; //23.06.2023
#define VM_SHUFFLE9(a1, a2, a3, a4, a5, a6, a7, a8, a9) a5; a1; a7; a3; a4; a9; a2; a6; a8; //23.06.2023


template <typename T> struct vmvalue1
{
public:
    operator const T() const
    {
        return (T)((uintptr_t)storage - (uintptr_t)this);
    }

    void operator=(const T& value)
    {
        storage = (T)((uintptr_t)value + (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }
private:
    T storage;
};


template <typename T> struct vmvalue2
{
public:
    operator const T() const
    {
        return (T)((uintptr_t)this - (uintptr_t)storage);
    }

    void operator=(const T& value)
    {
        storage = (T)((uintptr_t)this - (uintptr_t)value);
    }

    const T operator->() const
    {
        return operator const T();
    }
private:
    T storage;
};


template <typename T> struct vmvalue3
{
public:
    operator const T() const
    {
        return (T)((uintptr_t)this ^ (uintptr_t)storage);
    }

    void operator=(const T& value)
    {
        storage = (T)((uintptr_t)value ^ (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }
private:
    T storage;
};


template <typename T> struct vmvalue4
{
public:
    operator const T() const
    {
        return (T)((uintptr_t)this + (uintptr_t)storage);
    }

    void operator=(const T& value)
    {
        storage = (T)((uintptr_t)value - (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }
private:
    T storage;
};

static uintptr_t _getscheduler = Address::robloxScheduler;
static uintptr_t _pushinstance = Address::robloxPushInstance;
static uintptr_t _print = Address::robloxStdOut;
static uintptr_t _globalstate = Address::robloxGetGlobalState;
static uintptr_t _pseudo2 = Address::robloxPseudo2;
static uintptr_t _luau_execute = Address::luauExecute;
static uintptr_t _nilobject = Address::luaOnilobject;
static uintptr_t _dummynode = Address::luaHdummynode;