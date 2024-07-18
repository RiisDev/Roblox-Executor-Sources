#pragma once
#include <Windows.h>
#include <vector>
#include <stdint.h>
#include <tuple>
#include <map>
#include <fstream>

extern "C"
{
#include "Lua/lstate.h"
#include "Lua/lopcodes.h"
}

#define cast(t, exp)    ((t)(exp))

#define cast_void(i)    cast(void, (i))
#define cast_voidp(i)   cast(void *, (i))
#define cast_num(i)     cast(lua_Number, (i))
#define cast_int(i)     cast(int, (i))
#define cast_uint(i)    cast(unsigned int, (i))
#define cast_byte(i)    cast(lu_byte, (i))
#define cast_uchar(i)   cast(unsigned char, (i))
#define cast_char(i)    cast(char, (i))
#define cast_charp(i)   cast(char *, (i))
#define cast_sizet(i)   cast(size_t, (i))


typedef lu_int32 r_Instruction;

#define R_SIZE_OP			8
#define R_SIZE_A			8
#define R_SIZE_B			8
#define R_SIZE_C			8
#define R_SIZE_Bx			(R_SIZE_B + R_SIZE_C)


#define R_POS_OP			0
#define R_POS_A				(R_POS_OP + R_SIZE_OP)
#define R_POS_B				(R_POS_A + R_SIZE_A)
#define R_POS_C				(R_POS_B + R_SIZE_B)
#define R_POS_Bx			R_POS_B

#if R_SIZE_Bx < LUAI_BITSINT - 1

#define R_MAXARG_Bx			((1 << R_SIZE_Bx) - 1)
#define R_MAXARG_sBx		(R_MAXARG_Bx >> 1)

#else

#define R_MAXARG_Bx			MAX_INT
#define R_MAXARG_sBx		MAX_INT

#endif

#define R_MAXARG_A			((1 << SIZE_A) - 1)
#define R_MAXARG_B			((1 << SIZE_B) - 1)
#define R_MAXARG_C			((1 << SIZE_C) - 1)


#define R_GET_OPCODE(i)		(cast(R_LUAU, ((i) >> R_POS_OP) & MASK1(R_SIZE_OP, 0)))
#define R_SET_OPCODE(i, o)	((i) = (((i) & MASK0(R_SIZE_OP, R_POS_OP)) | \
		((cast(r_Instruction, o) << R_POS_OP) & MASK1(R_SIZE_OP, R_POS_OP))))

#define R_GETARG_A(i)		(cast(int, ((i) >> R_POS_A) & MASK1(R_SIZE_A, 0)))
#define R_SETARG_A(i, u)	((i) = (((i) & MASK0(R_SIZE_A, R_POS_A)) | \
		((cast(r_Instruction, u) << R_POS_A) & MASK1(R_SIZE_A, R_POS_A))))

#define R_GETARG_B(i)		(cast(int, ((i) >> R_POS_B) & MASK1(R_SIZE_B, 0)))
#define R_SETARG_B(i, b)	((i) = (((i)&MASK0(R_SIZE_B, R_POS_B)) | \
		((cast(r_Instruction, b) << R_POS_B) & MASK1(R_SIZE_B, R_POS_B))))

#define R_GETARG_C(i)		(cast(int, ((i) >> R_POS_C) & MASK1(R_SIZE_C, 0)))
#define R_SETARG_C(i, b)	((i) = (((i) & MASK0(R_SIZE_C, R_POS_C)) | \
		((cast(r_Instruction, b) << R_POS_C) & MASK1(R_SIZE_C, R_POS_C))))

#define R_GETARG_Bx(i)		(cast(int, ((i) >> R_POS_Bx) & MASK1(R_SIZE_Bx, 0)))
#define R_SETARG_Bx(i, b)	((i) = (((i) & MASK0(R_SIZE_Bx, R_POS_Bx)) | \
		((cast(r_Instruction, b) << R_POS_Bx) & MASK1(R_SIZE_Bx, R_POS_Bx))))

#define R_GETARG_sBx(i)		(short)((i) >> 16)
#define R_SETARG_sBx(i, x)  ((i) = ((i) & 0x0000FFFF | ((x) << 16) & 0xFFFF0000))

#define R_GETARG_sAx(i)		((int)(i) >> 8)
#define R_SETARG_sAx(i, x)  ((i) = ((i) & 0x000000FF | ((x) << 8) & 0xFFFFFF00))

enum R_LUAU
{
    NOOP = 0x00,
    MARKUPVAL = 0x12,
    INITVA = 0xA3,
    INIT = 0xC0,
    MOVE = 0x52,
    LOADK = 0x6F,
    LOADNIL = 0xC6,
    LOADBOOL = 169,
    LOADTABLE = 0xE2,
    LOADINT = 0x8C,
    NEWTABLE = 0xFF,
    GETUPVAL = 0xFB,
    GETGLOBAL = 0x35,
    GETGLOBALOPT = 0xA4,
    GETTABLE = 0x87,
    GETTABLEK = 0x4D,
    GETTABLEN = 0x13,
    SETUPVAL = 0xDE,
    SETTABLE = 0x6A,
    SETGLOBAL = 0x18,
    SETTABLEK = 0x30,
    SETLIST = 0xC5,
    UNM = 0x39,
    NOT = 0x56,
    LEN = 0x1C,
    CONCAT = 0x73,
    TFORLOOP = 0x6E,
    FORPREP = 0xA8,
    FORLOOP = 0x8B,
    PAIRSPREP = 0x17,
    PAIRSLOOP = 0xFA,
    IPAIRSPREP = 0, // UNKNOWN
    IPAIRSLOOP = 0x34, // UNKNOWN
    SELF = 0xBC,
    ADD = 0x95,
    SUB = 0x78,
    MUL = 0x5B,
    DIV = 0x3E,
    POW = 0x04,
    MOD = 0x21,
    ADDK = 0x43,
    SUBK = 0x26,
    MULK = 0x9,
    DIVK = 0xEC,
    POWK = 0xB2,
    MODK = 0xCF,
    JMP = 0x65,
    MEDIUMJMP = 0x48,
    LONGJMP = 0x69,
    EQ = 0xF1,
    NEQ = 0x9A,
    LT = 0xB7,
    GT = 0x60,
    LE = 0xD4,
    GE = 0x7D,
    AND = 0xAD,
    TESTJMP = 0x2B,
    TESTEQ = 0, // UNKNOWN
    TEST = 0x0E,
    CALL = 0x9F,
    VARARG = 0xDD,
    CLOSURE = 0xD9,
    CLOSE = 0xC1,
    RETURN = 0x82,
};
union LuauInstruction
{
    struct
    {
        uint8_t op;
        uint8_t a;
        union
        {
            struct
            {
                uint8_t b;
                uint8_t c;
            };
            uint16_t b_x;
            int16_t s_b_x;
            int8_t sax;
        };
    };
    uint32_t Value;



    explicit LuauInstruction()
    {
        Value = 0;
    }

    explicit LuauInstruction(const Instruction C)
    {
        Value = C;
    }

    explicit operator Instruction() const
    {
        return Value;
    }

    explicit operator DWORD() const
    {
        return Value;
    }
};

LuauInstruction generateABx(uint8_t op, uint8_t a, uint16_t bx)
{
    LuauInstruction inst(0);
    inst.op = op;
    inst.a = a;
    inst.b_x = bx;
    return inst;
}

LuauInstruction generateAB(uint8_t op, uint8_t a, uint8_t b)
{
    LuauInstruction inst(0);
    inst.op = op;
    inst.a = a;
    inst.b = b;
    return inst;
}

LuauInstruction generateA(uint8_t op, uint8_t a)
{
    LuauInstruction inst(0);
    inst.op = op;
    inst.a = a;
    return inst;
}

LuauInstruction generateSbx(uint8_t op, uint8_t a, int16_t sbx)
{
    LuauInstruction inst(0);
    inst.op = op;
    inst.a = a;
    inst.s_b_x = sbx;
    return inst;
}

LuauInstruction generateABC(uint8_t op, uint8_t a, uint8_t b, uint8_t c)
{
    LuauInstruction inst(0);
    inst.op = op;
    inst.a = a;
    inst.b = b;
    inst.c = c;
    return inst;
}


static std::vector<LuauInstruction> push_instrs(Proto* p, std::vector<int>& new_lineinfo)
{
    struct RelocationData
    {
        int OriginalPc;
        int TranslatedPc;
        Instruction OriginalInstr;
        Instruction TranslatedInstr;
        int32_t shift;
    };


    auto ret = std::vector<LuauInstruction>();
    auto realloc = std::vector<RelocationData>();
    auto pcTrans = reinterpret_cast<int*>(malloc(p->sizecode * 4));
    auto selfAllocation = std::vector<Instruction>();

   
    ret.emplace_back(generateABC(INITVA, p->numparams, 0, 0));
    std::vector<std::string> globals;
    new_lineinfo.push_back(p->lineinfo[0]);

    for (auto at = 0; at < p->sizecode; at++)
    {
        auto i = p->code[at];
        auto op = GET_OPCODE(i);
        pcTrans[at] = ret.size();
        auto R_Bx = GETARG_Bx(i);
        switch (op)
        {
        case OP_MARKUPVAL:
            ret.push_back(generateAB(R_LUAU::MARKUPVAL, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        case OP_MOVE:
            ret.emplace_back(generateAB(R_LUAU::MOVE, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        case OP_LOADK:
            ret.emplace_back(generateABx(R_LUAU::LOADK, GETARG_A(i), R_Bx));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        case OP_LOADBOOL:
            ret.emplace_back(generateABC(R_LUAU::LOADBOOL, GETARG_A(i), GETARG_B(i), GETARG_C(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        case OP_LOADNIL:
        {
            auto A = GETARG_A(i);
            auto B = GETARG_B(i);

            while (B >= A)
            {
                ret.emplace_back(generateA(R_LUAU::LOADNIL, B--));
            }

            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }
        case OP_GETUPVAL:
            ret.emplace_back(generateAB(R_LUAU::GETUPVAL, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;

        case OP_GETGLOBAL:
            ret.emplace_back(generateABx(R_LUAU::GETGLOBAL, GETARG_A(i), 0));
            ret.emplace_back(GETARG_Bx(i));
            new_lineinfo.push_back(p->lineinfo[at]);
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        case OP_GETTABLE:
        {
            ret.emplace_back(generateABC(R_LUAU::GETTABLE, GETARG_A(i), GETARG_B(i), GETARG_C(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }
        case OP_SETGLOBAL:
            ret.emplace_back(generateABx(R_LUAU::SETGLOBAL, GETARG_A(i), std::uint16_t(0)));
            ret.emplace_back(GETARG_Bx(i));
            new_lineinfo.push_back(p->lineinfo[at]);
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        case OP_SETTABLE:
        {
            auto base = GETARG_C(i);
            ret.emplace_back(generateABC(R_LUAU::SETTABLE, base, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);

            break;
        }
        case OP_SETUPVAL:
            ret.emplace_back(generateAB(R_LUAU::SETUPVAL, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);

            break;
        case OP_NEWTABLE:
        {
            ret.emplace_back(generateABC(R_LUAU::NEWTABLE, GETARG_A(i), 0, 0));
            ret.emplace_back(0);
            new_lineinfo.push_back(p->lineinfo[at]);
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }
        case OP_SELF:
        {
            selfAllocation.push_back(i);
            break;
        }
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_POW:
        case OP_DIV:
        case OP_MOD: {
            auto pos = GETARG_B(i);
            switch (GET_OPCODE(i)) {
            case OP_DIV: {
                ret.emplace_back(generateABC(!ISK(GETARG_C(i)) ? DIVK : 0x3E, GETARG_A(i), pos, !ISK(GETARG_C(i)) ? GETARG_C(i) : INDEXK(GETARG_C(i))));
                break;
            }
            case OP_ADD: {
                ret.emplace_back(generateABC(!ISK(GETARG_C(i)) ? ADDK : 0x95, GETARG_A(i), pos, !ISK(GETARG_C(i)) ? GETARG_C(i) : INDEXK(GETARG_C(i))));
                break;
            }
            case OP_MUL: {
                ret.emplace_back(generateABC(!ISK(GETARG_C(i)) ? MULK : 0x5B, GETARG_A(i), pos, !ISK(GETARG_C(i)) ? GETARG_C(i) : INDEXK(GETARG_C(i))));
                break;
            }
            case OP_MOD: {
                ret.emplace_back(generateABC(!ISK(GETARG_C(i)) ? MODK : 0x21, GETARG_A(i), pos, !ISK(GETARG_C(i)) ? GETARG_C(i) : INDEXK(GETARG_C(i))));
                break;
            }
            case OP_POW: {
                ret.emplace_back(generateABC(!ISK(GETARG_C(i)) ? POWK : 0x04, GETARG_A(i), pos, !ISK(GETARG_C(i)) ? GETARG_C(i) : INDEXK(GETARG_C(i))));
                break;
            }
            case OP_SUB: {
                ret.emplace_back(generateABC(!ISK(GETARG_C(i)) ? SUBK : 0x78, GETARG_A(i), pos, !ISK(GETARG_C(i)) ? GETARG_C(i) : INDEXK(GETARG_C(i))));
                break;
            }
            }

            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_UNM:
        {
            ret.emplace_back(generateAB(R_LUAU::UNM, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_NOT:
        {
            ret.emplace_back(generateAB(R_LUAU::NOT, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_LEN:
        {
            ret.emplace_back(generateAB(R_LUAU::LEN, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_CONCAT:
        {
            ret.emplace_back(generateABC(R_LUAU::CONCAT, GETARG_A(i), GETARG_B(i), GETARG_C(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_JMP:
        {
            ret.emplace_back(generateSbx(R_LUAU::JMP, GETARG_A(i), GETARG_sBx(i)));
            realloc.push_back({ at, static_cast<int>(ret.size() - 1), i, (Instruction)ret.at(ret.size() - 1), -1 });
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_EQ:
        {
            ret.emplace_back(generateSbx(!GETARG_A(i) ? R_LUAU::EQ : R_LUAU::NEQ, GETARG_B(i), 2));
            ret.emplace_back(GETARG_C(i));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_LT:
        {
            ret.emplace_back(generateSbx(!GETARG_A(i) ? R_LUAU::LT : R_LUAU::GT, GETARG_B(i), 2));
            ret.emplace_back(GETARG_C(i));
            new_lineinfo.push_back(p->lineinfo[at]);
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_LE:
        {
            ret.emplace_back(generateSbx(!GETARG_A(i) ? R_LUAU::LE : R_LUAU::GE, GETARG_B(i), 2));
            ret.emplace_back(GETARG_C(i));
            new_lineinfo.push_back(p->lineinfo[at]);
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_TEST:
        {
            ret.emplace_back(generateSbx(GETARG_C(i) ? R_LUAU::TEST : R_LUAU::TESTJMP, GETARG_A(i), 1));
            new_lineinfo.push_back(p->lineinfo[at]);
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_TESTSET:
        {
            ret.emplace_back(generateSbx(!GETARG_C(i) ? R_LUAU::TEST : R_LUAU::TESTJMP, GETARG_A(i), 2));
            ret.emplace_back(generateAB(R_LUAU::MOVE, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }


        case OP_CALL:
        case OP_TAILCALL:
        {
            for (size_t itr = 0; itr < selfAllocation.size(); ++itr)
            {
                auto self = selfAllocation[itr];
                if (GETARG_A(self) == GETARG_A(i) && !ISK(GETARG_C(self)))
                {
                    std::printf("self\n");
                    ret.push_back(generateAB(R_LUAU::SELF, GETARG_A(self), GETARG_B(self)));
                    ret.emplace_back(INDEXK(GETARG_C(self)));
                    selfAllocation.erase(selfAllocation.begin() + itr);
                }
            }
            ret.emplace_back(generateABC(R_LUAU::CALL, GETARG_A(i), GETARG_B(i), GETARG_C(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_RETURN:
        {
            ret.emplace_back(generateABC(R_LUAU::CLOSE, 0, 1, 0));
            new_lineinfo.push_back(p->lineinfo[at]);
            ret.emplace_back(generateAB(R_LUAU::RETURN, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }
        case OP_FORPREP: {

            auto A = GETARG_A(i);

            ret.emplace_back(generateAB(MOVE, A + 3, A + 0));
            ret.emplace_back(generateAB(MOVE, A + 0, A + 1));
            ret.emplace_back(generateAB(MOVE, A + 1, A + 2));
            ret.emplace_back(generateAB(MOVE, A + 2, A + 3));
            ret.emplace_back(generateSbx(FORPREP, A, 1));
            realloc.push_back({ at, static_cast<int>(ret.size() - 1), i, (Instruction)ret.at(ret.size() - 1), 0 });
            ret.emplace_back(generateAB(MOVE, A + 3, A + 2));
            break;
        }

        case OP_FORLOOP:
        {
            ret.emplace_back(generateSbx(R_LUAU::FORLOOP, GETARG_A(i), 1));
            realloc.push_back({ at, static_cast<int>(ret.size() - 1), i, (Instruction)ret.at(ret.size() - 1), -2 });
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }


        case OP_TFORLOOP:
        {
            auto ra = GETARG_A(i);
            ret.emplace_back(generateSbx(R_LUAU::TFORLOOP, ra, 2));
            ret.emplace_back(GETARG_C(i));
            ret.emplace_back(generateSbx(R_LUAU::JMP, 0, 1));
            new_lineinfo.push_back(p->lineinfo[at]);
            new_lineinfo.push_back(p->lineinfo[at]);
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }


        case OP_SETLIST: {
            ret.emplace_back(generateABC(SETLIST, GETARG_A(i), GETARG_A(i) + 1, GETARG_B(i) == 0 ? 0 : GETARG_B(i) + 1));
            ret.emplace_back(static_cast<uint16_t>((GETARG_C(i) - 1) * LFIELDS_PER_FLUSH + 1));
            break;
        }

        case OP_CLOSE:
        {
            ret.emplace_back(generateA(R_LUAU::CLOSE, GETARG_A(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_CLOSURE:
        {
            ret.emplace_back(generateABx(R_LUAU::CLOSURE, GETARG_A(i), GETARG_Bx(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        case OP_VARARG:
        {
            ret.emplace_back(generateAB(R_LUAU::VARARG, GETARG_A(i), GETARG_B(i)));
            new_lineinfo.push_back(p->lineinfo[at]);
            break;
        }

        }

    }


    for (RelocationData& rel : realloc)
    {
        auto Target = pcTrans[rel.OriginalPc + GETARG_sBx(rel.OriginalInstr) + 1];
        auto Sbx = Target - rel.TranslatedPc + rel.shift;

        auto dist = static_cast<unsigned int>(Sbx);

        if (Sbx > 32768 || Sbx < -32768)
        {
            if (ret[rel.TranslatedInstr].op == R_LUAU::JMP)
            {
                ret[rel.TranslatedPc].op = R_LUAU::LONGJMP;
                ret[rel.TranslatedPc].sax = static_cast<signed int>(Sbx);
            }
            else {
                ret[rel.TranslatedPc].s_b_x = static_cast<signed short>(Sbx);
            }
        }
        else
        {
            ret[rel.TranslatedPc].s_b_x = static_cast<signed short>(Sbx);
        }
    }

    return ret;
}