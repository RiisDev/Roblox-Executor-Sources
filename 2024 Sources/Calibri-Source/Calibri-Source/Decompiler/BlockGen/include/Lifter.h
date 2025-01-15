//
// Created by xgladius on 8/6/22.
//
#pragma once
#include <vector>
#include <string>
#include "Luau/BytecodeBuilder.h"
#include "../../Common.h"

namespace Luau::Decompiler::BlockGen::Lifter {
    enum BlockType : int8_t {
        START = 1,
        END = 2,
        WHILESTART = 3
    };

    void dumpInstructionX(const uint32_t* code, std::string& result, int targetLabel);

    inline bool isJumpD(LuauOpcode op)
    {
        switch (op)
        {
            case LOP_JUMP:
            case LOP_JUMPIF:
            case LOP_JUMPIFNOT:
            case LOP_JUMPIFEQ:
            case LOP_JUMPIFLE:
            case LOP_JUMPIFLT:
            case LOP_JUMPIFNOTEQ:
            case LOP_JUMPIFNOTLE:
            case LOP_JUMPIFNOTLT:
            case LOP_FORNPREP:
            case LOP_FORNLOOP:
            case LOP_FORGPREP:
            case LOP_FORGLOOP:
            case LOP_FORGPREP_INEXT:
            case LOP_FORGLOOP_INEXT:
            case LOP_FORGPREP_NEXT:
            case LOP_FORGLOOP_NEXT:
            case LOP_JUMPBACK:
            case LOP_JUMPIFEQK:
            case LOP_JUMPIFNOTEQK:
                return true;

            default:
                return false;
        }
    }

    inline bool isSkipC(LuauOpcode op)
    {
        switch (op)
        {
            case LOP_LOADB:
            case LOP_FASTCALL:
            case LOP_FASTCALL1:
            case LOP_FASTCALL2:
            case LOP_FASTCALL2K:
                return true;

            default:
                return false;
        }
    }

    static int getJumpTarget(uint32_t insn, uint32_t pc)
    {
        LuauOpcode op = LuauOpcode(LUAU_INSN_OP(insn));

        if (isJumpD(op))
            return int(pc + LUAU_INSN_D(insn) + 1);
        else if (isSkipC(op) && LUAU_INSN_C(insn))
            return int(pc + LUAU_INSN_C(insn) + 1);
        else if (op == LOP_JUMPX)
            return int(pc + LUAU_INSN_E(insn) + 1);
        else
            return -1;
    }

    static int getOpLength(LuauOpcode op)
    {
        switch (op)
        {
            case LOP_GETGLOBAL:
            case LOP_SETGLOBAL:
            case LOP_GETIMPORT:
            case LOP_GETTABLEKS:
            case LOP_SETTABLEKS:
            case LOP_NAMECALL:
            case LOP_JUMPIFEQ:
            case LOP_JUMPIFLE:
            case LOP_JUMPIFLT:
            case LOP_JUMPIFNOTEQ:
            case LOP_JUMPIFNOTLE:
            case LOP_JUMPIFNOTLT:
            case LOP_NEWTABLE:
            case LOP_SETLIST:
            case LOP_FORGLOOP:
            case LOP_LOADKX:
            case LOP_JUMPIFEQK:
            case LOP_JUMPIFNOTEQK:
            case LOP_FASTCALL2:
            case LOP_FASTCALL2K:
                return 2;

            default:
                return 1;
        }
    }

    std::vector<int> liftBodyInfo(Proto* p);
}