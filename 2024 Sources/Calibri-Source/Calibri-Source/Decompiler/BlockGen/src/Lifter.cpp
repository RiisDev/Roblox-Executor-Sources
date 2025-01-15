//
// Created by xgladius on 8/6/22.
//
#include "../include/Lifter.h"

namespace Luau::Decompiler::BlockGen::Lifter {
    void dumpInstructionX(const uint32_t* code, std::string& result, int targetLabel) {
        uint32_t insn = *code++;

        switch (LUAU_INSN_OP(insn))
        {
            case LOP_LOADNIL:
                formatAppend(result, "LOADNIL R%d\n", LUAU_INSN_A(insn));
                break;

            case LOP_LOADB:
                if (LUAU_INSN_C(insn))
                    formatAppend(result, "LOADB R%d %d +%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                else
                    formatAppend(result, "LOADB R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn));
                break;

            case LOP_LOADN:
                formatAppend(result, "LOADN R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_D(insn));
                break;

            case LOP_LOADK:
                formatAppend(result, "LOADK R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_D(insn));
                break;

            case LOP_MOVE:
                formatAppend(result, "MOVE R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn));
                break;

            case LOP_GETGLOBAL:
                formatAppend(result, "GETGLOBAL R%d K%d\n", LUAU_INSN_A(insn), *code++);
                break;

            case LOP_SETGLOBAL:
                formatAppend(result, "SETGLOBAL R%d K%d\n", LUAU_INSN_A(insn), *code++);
                break;

            case LOP_GETUPVAL:
                formatAppend(result, "GETUPVAL R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn));
                break;

            case LOP_SETUPVAL:
                formatAppend(result, "SETUPVAL R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn));
                break;

            case LOP_CLOSEUPVALS:
                formatAppend(result, "CLOSEUPVALS R%d\n", LUAU_INSN_A(insn));
                break;

            case LOP_GETIMPORT:
                formatAppend(result, "GETIMPORT R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_D(insn));
                code++; // AUX
                break;

            case LOP_GETTABLE:
                formatAppend(result, "GETTABLE R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_SETTABLE:
                formatAppend(result, "SETTABLE R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_GETTABLEKS:
                formatAppend(result, "GETTABLEKS R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), *code++);
                break;

            case LOP_SETTABLEKS:
                formatAppend(result, "SETTABLEKS R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), *code++);
                break;

            case LOP_GETTABLEN:
                formatAppend(result, "GETTABLEN R%d R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn) + 1);
                break;

            case LOP_SETTABLEN:
                formatAppend(result, "SETTABLEN R%d R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn) + 1);
                break;

            case LOP_NEWCLOSURE:
                formatAppend(result, "NEWCLOSURE R%d P%d\n", LUAU_INSN_A(insn), LUAU_INSN_D(insn));
                break;

            case LOP_NAMECALL:
                formatAppend(result, "NAMECALL R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), *code++);
                break;

            case LOP_CALL:
                formatAppend(result, "CALL R%d %d %d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn) - 1, LUAU_INSN_C(insn) - 1);
                break;

            case LOP_RETURN:
                formatAppend(result, "RETURN R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn) - 1);
                break;

            case LOP_JUMP:
                formatAppend(result, "JUMP L%d\n", targetLabel);
                break;

            case LOP_JUMPIF:
                formatAppend(result, "JUMPIF R%d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_JUMPIFNOT:
                formatAppend(result, "JUMPIFNOT R%d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_JUMPIFEQ:
                formatAppend(result, "JUMPIFEQ R%d R%d L%d\n", LUAU_INSN_A(insn), *code++, targetLabel);
                break;

            case LOP_JUMPIFLE:
                formatAppend(result, "JUMPIFLE R%d R%d L%d\n", LUAU_INSN_A(insn), *code++, targetLabel);
                break;

            case LOP_JUMPIFLT:
                formatAppend(result, "JUMPIFLT R%d R%d L%d\n", LUAU_INSN_A(insn), *code++, targetLabel);
                break;

            case LOP_JUMPIFNOTEQ:
                formatAppend(result, "JUMPIFNOTEQ R%d R%d L%d\n", LUAU_INSN_A(insn), *code++, targetLabel);
                break;

            case LOP_JUMPIFNOTLE:
                formatAppend(result, "JUMPIFNOTLE R%d R%d L%d\n", LUAU_INSN_A(insn), *code++, targetLabel);
                break;

            case LOP_JUMPIFNOTLT:
                formatAppend(result, "JUMPIFNOTLT R%d R%d L%d\n", LUAU_INSN_A(insn), *code++, targetLabel);
                break;

            case LOP_ADD:
                formatAppend(result, "ADD R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_SUB:
                formatAppend(result, "SUB R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_MUL:
                formatAppend(result, "MUL R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_DIV:
                formatAppend(result, "DIV R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_MOD:
                formatAppend(result, "MOD R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_POW:
                formatAppend(result, "POW R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_ADDK:
                formatAppend(result, "ADDK R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_SUBK:
                formatAppend(result, "SUBK R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_MULK:
                formatAppend(result, "MULK R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_DIVK:
                formatAppend(result, "DIVK R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_MODK:
                formatAppend(result, "MODK R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_POWK:
                formatAppend(result, "POWK R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_AND:
                formatAppend(result, "AND R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_OR:
                formatAppend(result, "OR R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_ANDK:
                formatAppend(result, "ANDK R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_ORK:
                formatAppend(result, "ORK R%d R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_CONCAT:
                formatAppend(result, "CONCAT R%d R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn));
                break;

            case LOP_NOT:
                formatAppend(result, "NOT R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn));
                break;

            case LOP_MINUS:
                formatAppend(result, "MINUS R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn));
                break;

            case LOP_LENGTH:
                formatAppend(result, "LENGTH R%d R%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn));
                break;

            case LOP_NEWTABLE:
                formatAppend(result, "NEWTABLE R%d %d %d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn) == 0 ? 0 : 1 << (LUAU_INSN_B(insn) - 1), *code++);
                break;

            case LOP_DUPTABLE:
                formatAppend(result, "DUPTABLE R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_D(insn));
                break;

            case LOP_SETLIST:
                formatAppend(result, "SETLIST R%d R%d %d [%d]\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), LUAU_INSN_C(insn) - 1, *code++);
                break;

            case LOP_FORNPREP:
                formatAppend(result, "FORNPREP R%d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_FORNLOOP:
                formatAppend(result, "FORNLOOP R%d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_FORGPREP:
                formatAppend(result, "FORGPREP R%d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_FORGLOOP:
                formatAppend(result, "FORGLOOP R%d L%d %d\n", LUAU_INSN_A(insn), targetLabel, *code++);
                break;

            case LOP_FORGPREP_INEXT:
                formatAppend(result, "FORGPREP_INEXT R%d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_FORGLOOP_INEXT:
                formatAppend(result, "FORGLOOP_INEXT R%d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_FORGPREP_NEXT:
                formatAppend(result, "FORGPREP_NEXT R%d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_FORGLOOP_NEXT:
                formatAppend(result, "FORGLOOP_NEXT R%d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_GETVARARGS:
                formatAppend(result, "GETVARARGS R%d %d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn) - 1);
                break;

            case LOP_DUPCLOSURE:
                formatAppend(result, "DUPCLOSURE R%d K%d\n", LUAU_INSN_A(insn), LUAU_INSN_D(insn));
                break;

            case LOP_BREAK:
                formatAppend(result, "BREAK\n");
                break;

            case LOP_JUMPBACK:
                formatAppend(result, "JUMPBACK L%d\n", targetLabel);
                break;

            case LOP_LOADKX:
                formatAppend(result, "LOADKX R%d K%d\n", LUAU_INSN_A(insn), *code++);
                break;

            case LOP_JUMPX:
                formatAppend(result, "JUMPX L%d\n", targetLabel);
                break;

            case LOP_FASTCALL:
                formatAppend(result, "FASTCALL %d L%d\n", LUAU_INSN_A(insn), targetLabel);
                break;

            case LOP_FASTCALL1:
                formatAppend(result, "FASTCALL1 %d R%d L%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), targetLabel);
                break;
            case LOP_FASTCALL2:
            {
                uint32_t aux = *code++;
                formatAppend(result, "FASTCALL2 %d R%d R%d L%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), aux, targetLabel);
                break;
            }
            case LOP_FASTCALL2K:
            {
                uint32_t aux = *code++;
                formatAppend(result, "FASTCALL2K %d R%d K%d L%d\n", LUAU_INSN_A(insn), LUAU_INSN_B(insn), aux, targetLabel);
                break;
            }

            case LOP_COVERAGE:
                formatAppend(result, "COVERAGE\n");
                break;

            case LOP_CAPTURE:
                formatAppend(result, "CAPTURE %s %c%d\n",
                             LUAU_INSN_A(insn) == LCT_UPVAL ? "UPVAL"
                                                            : LUAU_INSN_A(insn) == LCT_REF ? "REF"
                                                                                           : LUAU_INSN_A(insn) == LCT_VAL ? "VAL"
                                                                                                                          : "",
                             LUAU_INSN_A(insn) == LCT_UPVAL ? 'U' : 'R', LUAU_INSN_B(insn));
                break;

            case LOP_JUMPIFEQK:
                formatAppend(result, "JUMPIFEQK R%d K%d L%d\n", LUAU_INSN_A(insn), *code++, targetLabel);
                break;

            case LOP_JUMPIFNOTEQK:
                formatAppend(result, "JUMPIFNOTEQK R%d K%d L%d\n", LUAU_INSN_A(insn), *code++, targetLabel);
                break;

            case LOP_NOP:
                formatAppend(result, "NOP\n");
                break;
            default:
                //LUAU_ASSERT(!"Unsupported opcode");
                break;
        }
    }

    std::vector<int> liftBodyInfo(Proto* p) {
        std::vector<int> bodies;
        bodies.resize(p->sizecode + 1, -1);
        for (size_t i = 0; i < p->sizecode;) {
            int target = getJumpTarget(p->code[i], uint32_t(i));
            if (target >= 0) {
                if (LUAU_INSN_OP(p->code[i]) == LOP_JUMP) {
                    bodies[i + getOpLength(
                            LuauOpcode(LUAU_INSN_OP(p->code[i]))) - 1] = END; // next instr is start of block
                    p->code[i] = LOP_NOP;
                    bodies[target] = END;
                } else if (LUAU_INSN_OP(p->code[i]) == LOP_JUMPIFNOT) {
                    bodies[i + getOpLength(
                            LuauOpcode(LUAU_INSN_OP(p->code[i]))) - 1] = END; // next instr is start of block
                    bodies[target] = END;
                } else if (LUAU_INSN_OP(p->code[i]) == LOP_JUMPBACK) {
                    p->code[i] = LOP_NOP;
                    bodies[target] = WHILESTART;
                } else if (LUAU_INSN_OP(p->code[i]) == LOP_FORNLOOP || LUAU_INSN_OP(p->code[i]) == LOP_FORGLOOP) {
                    // do nothing, else will handle this, we do nothing with this instr in decomp
                    p->code[i] = LOP_NOP;
                } else {
                    bodies[i + getOpLength(LuauOpcode(LUAU_INSN_OP(p->code[i])))- 1] = START; // next instr is start of block
                    bodies[target- 1] = END; // end of block
                }
            }
            i += getOpLength(LuauOpcode(LUAU_INSN_OP(p->code[i])));
        }

        for (size_t i = 0; i < p->sizecode;) {
            std::string res;
            dumpInstructionX(&p->code[i], res, 0);
            i += getOpLength(LuauOpcode(LUAU_INSN_OP(p->code[i])));
            printf("%d: %s", bodies[i], res.c_str());
        }

        return bodies;
    }
}