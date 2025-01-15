#pragma once

#include <cstdint>

#define vm_shuffle_comma ,
#define vm_shuffle_semicolon ;

//#define vm_shuffle3(d, a1, a2, a3) a2 d a3 d a1 // 06.12.2024
//#define vm_shuffle4(d, a1, a2, a3, a4) a2 d a4 d a3 d a1 // 06.12.2024
//#define vm_shuffle5(d, a1, a2, a3, a4, a5) a5 d a2 d a4 d a3 d a1 // 06.12.2024
//#define vm_shuffle6(s,a1,a2,a3,a4,a5,a6) a2 s a4 s a3 s a1 s a5 s a6 // 06.12.2024
//#define vm_shuffle7(d, a1, a2, a3, a4, a5, a6, a7) a7 d a5 d a6 d a3 d a1 d a2 d a4 //20.11.2024
//#define vm_shuffle8(d, a1, a2, a3, a4, a5, a6, a7, a8) a8 d a2 d a5 d a1 d a4 d a7 d a3 d a6 //20.11.2024
//#define vm_shuffle10(d, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) a7 d a4 d a6 d a9 d a1 d a8 d a2 d a5 d a3 d a10 //20.11.2024

#define vm_shuffle3(s, a1, a2, a3) a3 s a2 s a1 // 11.12.2024
#define vm_shuffle4(s, a1, a2, a3, a4) a4 s a2 s a3 s a1 // 11.12.2024
#define vm_shuffle5(s, a1, a2, a3, a4, a5) a5 s a2 s a4 s a3 s a1 // 11.12.2024
#define vm_shuffle6(s, a1, a2, a3, a4, a5, a6) a2 s a6 s a4 s a1 s a5 s a3 // 11.12.2024
#define vm_shuffle7(s, a1, a2, a3, a4, a5, a6, a7) a2 s a3 s a4 s a1 s a5 s a6 s a7 // 11.12.2024
#define vm_shuffle8(s, a1, a2, a3, a4, a5, a6, a7, a8) a8 s a2 s a7 s a1 s a4 s a6 s a5 s a3 // 11.12.2024
#define vm_shuffle10(s, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) a4 s a5 s a9 s a8 s a7 s a6 s a1 s a3 s a2 s a10 // 11.12.2024

// shuffle10 = shuffle9 and last position ist 10

template <typename T> struct vmvalue_1
{
private:
    T Storage;
public:
    operator const T() const
    {
        return (T)((uintptr_t)this->Storage - (uintptr_t)this);
    }

    void operator=(const T& Value)
    {
        this->Storage = (T)((uintptr_t)Value + (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }

    T Get()
    {
        return operator const T();
    }

    void Set(const T& Value)
    {
        operator=(Value);
    }
};

template <typename T> struct vmvalue_2
{
private:
    T Storage;
public:
    operator const T() const
    {
        return (T)((uintptr_t)this - (uintptr_t)this->Storage);
    }

    void operator=(const T& Value)
    {
        this->Storage = (T)((uintptr_t)this - (uintptr_t)Value);
    }

    const T operator->() const
    {
        return operator const T();
    }

    T Get()
    {
        return operator const T();
    }

    void Set(const T& Value)
    {
        operator=(Value);
    }
};

template <typename T> struct vmvalue_3
{
private:
    T Storage;
public:
    operator const T() const
    {
        return (T)((uintptr_t)this ^ (uintptr_t)this->Storage);
    }

    void operator=(const T& Value)
    {
        this->Storage = (T)((uintptr_t)Value ^ (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }

    T Get()
    {
        return operator const T();
    }

    void Set(const T& Value)
    {
        operator=(Value);
    }
};

template <typename T> struct vmvalue_4
{
private:
    T Storage;
public:
    operator const T() const
    {
        return (T)((uintptr_t)this + (uintptr_t)this->Storage);
    }

    void operator=(const T& Value)
    {
        this->Storage = (T)((uintptr_t)Value - (uintptr_t)this);
    }

    const T operator->() const
    {
        return operator const T();
    }

    T Get()
    {
        return operator const T();
    }

    void Set(const T& Value)
    {
        operator=(Value);
    }
};

#define globalstate_ttname vmvalue_4 //06.12.2024
#define globalstate_tmname vmvalue_4 //06.12.2024

#define luastate_global vmvalue_4 //06.12.2024
#define luastate_stacksize vmvalue_4 //06.12.2024

#define tstring_hash vmvalue_3 //06.12.2024
#define tstring_len vmvalue_4 //06.12.2024

#define udata_metatable vmvalue_4 //06.12.2024

#define closure_c_f vmvalue_2 //06.12.2024
#define closure_c_cont vmvalue_1 //06.12.2024
#define closure_c_debugname vmvalue_3 //06.12.2024

#define proto_member1 vmvalue_2
#define proto_member2 vmvalue_3

#define proto_debugname vmvalue_2 //06.12.2024
#define proto_debuginsn vmvalue_1 //06.12.2024

#define proto_typeinfo vmvalue_4 //06.12.2024

#define table_member vmvalue_2