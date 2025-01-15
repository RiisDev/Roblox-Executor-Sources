#pragma once

#include <cstdint>

#define vm_shuffle_comma ,
#define vm_shuffle_semicolon ;

#define vm_shuffle3(d, a1, a2, a3) a1 d a3 d a2 //25.09.2024
#define vm_shuffle4(d, a1, a2, a3, a4) a2 d a3 d a1 d a4 //25.09.2024
#define vm_shuffle5(d, a1, a2, a3, a4, a5) a5 d a3 d a2 d a4 d a1 //25.09.2024
#define vm_shuffle6(d, a1, a2, a3, a4, a5, a6) a2 d a6 d a4 d a1 d a5 d a3 //25.09.2024
#define vm_shuffle7(d, a1, a2, a3, a4, a5, a6, a7) a2 d a3 d a4 d a1 d a5 d a6 d a7 //25.09.2024
#define vm_shuffle8(d, a1, a2, a3, a4, a5, a6, a7, a8) a1 d a7 d a3 d a8 d a5 d a6 d a4 d a2 //25.09.2024
#define vm_shuffle10(d, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) a7 d a4 d a6 d a9 d a1 d a8 d a2 d a5 d a3 d a10 //25.09.2024

enum operation
{
    Add,
    Sub,
    Xor
};

enum direction
{
    Right,
    Left
};

template <typename t, operation type_get, operation type_set, direction direction_get, direction direction_set>
class vm_value
{
public:
    operator const t() const
    {
        std::uint64_t base = (std::uint64_t)(this);
        std::uint64_t offset = (std::uint64_t)(storage);

        if (direction_get == Right)
        {
            if (type_get == Add)
            {
                return (t)(base + offset);
            }
            else if (type_get == Sub)
            {
                return (t)(base - offset);
            }
            else if (type_get == Xor)
            {
                return (t)(base ^ offset);
            }
        }
        else
        {
            if (type_get == Add)
            {
                return (t)(offset + base);
            }
            else if (type_get == Sub)
            {
                return (t)(offset - base);
            }
            else if (type_get == Xor)
            {
                return (t)(offset ^ base);
            }
        }
    }

    void operator=(const t& value)
    {
        std::uint64_t base = (std::uint64_t)(this);
        std::uint64_t input_value = (std::uint64_t)(value);

        if (direction_set == Right)
        {
            if (type_set == Add)
            {
                storage = (t)(base + input_value);
            }
            else if (type_set == Sub)
            {
                storage = (t)(base - input_value);
            }
            else if (type_set == Xor)
            {
                storage = (t)(base ^ input_value);
            }
        }
        else
        {
            if (type_set == Add)
            {
                storage = (t)(input_value + base);
            }
            else if (type_set == Sub)
            {
                storage = (t)(input_value - base);
            }
            else if (type_set == Xor)
            {
                storage = (t)(input_value ^ base);
            }
        }
    }

    const t operator->() const
    {
        return operator const t();
    }
private:
    t storage;
};

#define vm_add_sub(type) vm_value<type, Add, Sub, Left, Left>
#define vm_sub_add(type) vm_value<type, Sub, Add, Left, Left>
#define vm_sub_sub(type) vm_value<type, Sub, Sub, Right, Right>
#define vm_xor_xor(type) vm_value<type, Xor, Xor, Right, Right>

#define globalstate_ttname(type) vm_xor_xor(type) //25.09.2024
#define globalstate_tmname(type) vm_xor_xor(type) //25.09.2024

#define luastate_global(type) vm_xor_xor(type) //25.09.2024
#define luastate_stacksize(type) vm_xor_xor(type) //25.09.2024

#define tstring_hash(type) vm_sub_add(type) //25.09.2024
#define tstring_len(type) vm_xor_xor(type) //25.09.2024

#define udata_metatable(type) vm_xor_xor(type)   //25.09.2024

#define closure_c_f(type) vm_add_sub(type) //25.09.2024
#define closure_c_cont(type) vm_sub_sub(type) //25.09.2024
#define closure_c_debugname(type) vm_sub_add(type) //25.09.2024

#define closure_l_p(type) vm_add_sub(type) //25.09.2024

#define proto_k(type) vm_add_sub(type) //25.09.2024
#define proto_code(type) vm_add_sub(type) //25.09.2024
#define proto_p(type) vm_add_sub(type) //25.09.2024

#define proto_lineinfo(type) vm_sub_add(type) //25.09.2024
#define proto_abslineinfo(type) vm_sub_add(type) //25.09.2024
#define proto_locvars(type) vm_sub_add(type) //25.09.2024
#define proto_upvalues(type) vm_sub_add(type) //25.09.2024
#define proto_source(type) vm_sub_add(type) //25.09.2024

#define proto_debugname(type) vm_add_sub(type) //25.09.2024
#define proto_debuginsn(type) vm_sub_sub(type) //25.09.2024

#define proto_typeinfo(type) vm_xor_xor(type) //25.09.2024

#define table_metatable(type) vm_add_sub(type) //25.09.2024
#define table_array(type) vm_add_sub(type) //25.09.2024
#define table_node(type) vm_add_sub(type) //25.09.2024