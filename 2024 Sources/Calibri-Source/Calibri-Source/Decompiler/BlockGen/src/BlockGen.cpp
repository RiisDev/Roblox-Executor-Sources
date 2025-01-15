//
// Created by xgladius on 8/6/22.
//
#include "../include/BlockGen.h"

#include <utility>

namespace Luau::Decompiler::BlockGen {
    template<> BlockGen<false>::BlockGen(Proto* p) : proto(p),
                                            blockInfo(Lifter::liftBodyInfo(p)),
                                            functionArgs(new std::vector<AstLocal*>{}) {}

    template<> BlockGen<true>::BlockGen(Proto *p,
            std::vector<AstExpr*> subFuncs) : proto(p),
                                                    blockInfo(Lifter::liftBodyInfo(p)),
                                                    functionArgs(new std::vector<AstLocal*>{}),
                                                    subFuncs(std::move(subFuncs)) {}

    template<> void BlockGen<false>::generateFunctionArgs() {
        if (proto->numparams) {
            functionArgs->reserve(proto->numparams);
            for (auto i = 0; i < proto->numparams; i++) {
                auto name = new std::string(std::string("arg") + std::to_string(i));
                auto local = new AstLocal{AstName((name->c_str())),
                                           Location(), nullptr, 1, 1, nullptr};
                functionArgs->push_back(local);
                virtualStack.set(i, new AstExprLocal { Location(), local, false });
            }
        }
    }

    template<> void BlockGen<true>::generateFunctionArgs() {}

    template<bool isMain> AstStatBlock* BlockGen<isMain>::handleAllInstructions() {
        bodyHandler.makeMain();
        for (auto pc = 0; pc < proto->sizecode;) {
            auto *insn = &proto->code[pc];
            handleInstruction(insn, pc);
            pc += Luau::Decompiler::BlockGen::Lifter::getOpLength(LuauOpcode(*insn));
        }
        auto ret = bodyHandler.get()->template as<AstStatBlock>();
        return ret;
    }

    template<> AstStatBlock* BlockGen<true>::generate() {
        return handleAllInstructions();
    }

    template<> AstExpr* BlockGen<false>::generate() {
        generateFunctionArgs();
        AstStatBlock* block = handleAllInstructions();
        auto name = AstName(proto->debugname ? proto->debugname->data : "");
        auto func = new AstExprFunction{AstExprFunction {Location(), AstArray<AstGenericType>{}, AstArray<AstGenericTypePack>{},
                                                         new AstLocal(name, Location(), nullptr, 1, 1, nullptr),
                                                         AstArray<AstLocal*>{functionArgs->data(), functionArgs->size()},
                                                         proto->is_vararg ? std::make_optional(Location()) : std::nullopt, block, 1, AstName(name), {}}};
        if (!proto->debugname) {
            return new AstExprGroup {Location(), func};
        }
        return func;
    }

    template<bool isMain> void BlockGen<isMain>::handleInstruction(unsigned int *insn, int pc) {
        if (blockInfo[pc] == Lifter::BlockType::END) {
            if (bodyHandler.getType() == BodyType::IF) {
                bodyHandler.addStat(bodyHandler.get()->template as<AstStatIf>());
            }
        } else if (blockInfo[pc] == Lifter::BlockType::WHILESTART) {
            bodyHandler.makeWhile();
        }
        switch (LUAU_INSN_OP(*insn)) {
            case LOP_LOADK: {
                virtualStack.set(LUAU_INSN_A(*insn), getConstantAst(&proto->k[LUAU_INSN_D(*insn)]));
                break;
            }
            case LOP_LOADB: {
                //virtualStack.set(LUAU_INSN_A(*insn), new AstExprConstantBool { Location(),
                //                                                               LUAU_INSN_B(*insn) != 0 });
                break;
            }
            case LOP_CALL: {
                auto callExpr = Luau::Decompiler::AstGen::Handlers::getCallAst(virtualStack, *insn);

                if (LUAU_INSN_C(*insn) - 1 != 0) {
                    for (auto i = 0; i <= LUAU_INSN_C(*insn); i++) {
                        virtualStack.set(LUAU_INSN_A(*insn) + i, callExpr);
                    }
                } else {
                    bodyHandler.addStat(new AstStatExpr {AstStatExpr{Location(), callExpr} });
                }
                break;
            }
            case LOP_SETGLOBAL: {
                auto stat = Luau::Decompiler::AstGen::Handlers::getSetGlobalAssignment(virtualStack, proto, *insn, *(insn + 1));
                bodyHandler.addStat(stat);
                break;
            }
            case LOP_GETGLOBAL: {
                virtualStack.set(LUAU_INSN_A(*insn), makeGlobal(proto->k[*(insn + 1)].value.gc->ts.data));
                break;
            }
            case LOP_GETTABLEKS: {
                virtualStack.set(LUAU_INSN_A(*insn), Luau::Decompiler::AstGen::Handlers::genAstExprName(virtualStack, proto, *insn, *(insn + 1), '.'));
                break;
            }
            case LOP_NAMECALL: {
                virtualStack.set(LUAU_INSN_A(*insn), Luau::Decompiler::AstGen::Handlers::genAstExprName(virtualStack, proto, *insn, *(insn + 1), ':'));
                break;
            }
            case LOP_NEWCLOSURE: {
                virtualStack.set(LUAU_INSN_A(*insn), subFuncs[LUAU_INSN_D(*insn)]);
                break;
            }
            case LOP_NEWTABLE: {
                auto *items_arr = new std::vector<AstExprTable::Item>;
                virtualStack.set(LUAU_INSN_A(*insn),
                                 new AstExprTable{Location(), {items_arr->data(), items_arr->size()}});
                break;
            }
            case LOP_SETLIST: {
                virtualStack.set(LUAU_INSN_A(*insn), Luau::Decompiler::AstGen::Handlers::genTableAst(virtualStack, *insn, *(insn + 1)));
                break;
            }
            case LOP_SETTABLE: {
                bodyHandler.addStat(Luau::Decompiler::AstGen::Handlers::genSetTable(virtualStack, *insn));
                break;
            }
            case LOP_MOVE: {
                if (!functionArgs->empty()) {
                    if (functionArgs->at(LUAU_INSN_B(*insn))) { // is an arg of the function
                        virtualStack.set(LUAU_INSN_A(*insn), virtualStack[LUAU_INSN_B(*insn)]);
                        break;
                    }
                }
                auto locVar = Luau::Decompiler::AstGen::Handlers::genLocalStat(virtualStack, locVars, *insn);
                if (!locVar.existed)
                    bodyHandler.addStat(locVar.stat);
                virtualStack.set(LUAU_INSN_A(*insn), locVar.expr);
                break;
            }
            case LOP_JUMPIFEQ:
            case LOP_JUMPIFLE:
            case LOP_JUMPIFLT:
            case LOP_JUMPIFNOTEQ:
            case LOP_JUMPIFNOTLE:
            case LOP_JUMPIFNOTLT:
            {
                unsigned int aux = *(insn + 1);
                auto *left = virtualStack[LUAU_INSN_A(*insn)];
                auto *right = virtualStack[aux];
                AstExprBinary::Op op;
                switch(LUAU_INSN_OP(*insn)) {
                    case LOP_JUMPIFEQ:
                        op = AstExprBinary::CompareNe;
                        break;
                    case LOP_JUMPIFNOTEQ:
                        op = AstExprBinary::CompareEq;
                        break;
                    case LOP_JUMPIFLE:
                        op = AstExprBinary::CompareGe;
                        break;
                    case LOP_JUMPIFLT:
                        op = AstExprBinary::CompareGt;
                        break;
                    case LOP_JUMPIFNOTLE:
                        op = AstExprBinary::CompareLe;
                        break;
                    case LOP_JUMPIFNOTLT:
                        op = AstExprBinary::CompareLt;
                        break;
                    default:
                        break;
                }
                if (bodyHandler.getType() == BodyType::WHILE) {
                    bodyHandler.updateWhileCond(new AstExprBinary{Location(), op, left, right});
                    break;
                }
                bodyHandler.makeIf(new AstExprBinary{Location(), op, left, right});
                break;
            }
            case LOP_JUMPIFNOT: {
                if (bodyHandler.getType() == BodyType::WHILE) {
                    bodyHandler.updateWhileCond(virtualStack[LUAU_INSN_A(*insn)]);
                    break;
                } else {
                    bodyHandler.makeIf(virtualStack[LUAU_INSN_A(*insn)]);
                }
                break;
            }
            case LOP_FORNPREP: {
                auto name = new std::string(std::string("R") + std::to_string(LUAU_INSN_A(*insn) + 2));
                auto local = new AstLocal {AstName(name->c_str()), Location(), nullptr, 1, 1, nullptr};
                locVars.insert(std::make_pair(LUAU_INSN_A(*insn) + 2, local));
                bodyHandler.makeFor(local, virtualStack[LUAU_INSN_A(*insn) + 2],
                        virtualStack[LUAU_INSN_A(*insn)], virtualStack[LUAU_INSN_A(*insn) + 1]);
                break;
            }
            case LOP_NOP: {
                if (bodyHandler.getType() == BodyType::FORPREP) {
                    bodyHandler.addStat(bodyHandler.get()->template as<AstStatFor>());
                } else if (bodyHandler.getType() == BodyType::WHILE) {
                    bodyHandler.addStat(bodyHandler.get()->template as<AstStatWhile>());
                }
            }
            case LOP_GETUPVAL: {
                break;
            }
            case LOP_GETVARARGS: {
                virtualStack.set(LUAU_INSN_A(*insn), new AstExprVarargs{Location()});
                break;
            }
            case LOP_ADD:
                virtualStack.set(LUAU_INSN_A(*insn), new AstExprBinary {Location(), AstExprBinary::Op::Add,
                                                                        virtualStack[LUAU_INSN_B(*insn)],
                                                                        virtualStack[LUAU_INSN_C(*insn)]});
                break;
            case LOP_SUB:
                virtualStack.set(LUAU_INSN_A(*insn), new AstExprBinary {Location(), AstExprBinary::Op::Sub,
                                                                        virtualStack[LUAU_INSN_B(*insn)],
                                                                        virtualStack[LUAU_INSN_C(*insn)]});
                break;
            case LOP_MUL:
                virtualStack.set(LUAU_INSN_A(*insn), new AstExprBinary {Location(), AstExprBinary::Op::Mul,
                                                                        virtualStack[LUAU_INSN_B(*insn)],
                                                                        virtualStack[LUAU_INSN_C(*insn)]});
                break;
            case LOP_DIV:
                virtualStack.set(LUAU_INSN_A(*insn), new AstExprBinary {Location(), AstExprBinary::Op::Div,
                                                                        virtualStack[LUAU_INSN_B(*insn)],
                                                                        virtualStack[LUAU_INSN_C(*insn)]});
                break;
            case LOP_MOD:
                virtualStack.set(LUAU_INSN_A(*insn), new AstExprBinary {Location(), AstExprBinary::Op::Mod,
                                                                        virtualStack[LUAU_INSN_B(*insn)],
                                                                        virtualStack[LUAU_INSN_C(*insn)]});
                break;
            case LOP_POW:
                virtualStack.set(LUAU_INSN_A(*insn), new AstExprBinary {Location(), AstExprBinary::Op::Pow,
                                                                        virtualStack[LUAU_INSN_B(*insn)],
                                                                        virtualStack[LUAU_INSN_C(*insn)]});
                break;
            case LOP_RETURN: {
                if (LUAU_INSN_B(*insn) - 1 > 0) {
                    auto* argVec = new std::vector<AstExpr *>{};
                    int argCount = LUAU_INSN_B(*insn) == 0 ? virtualStack.getTop() + 1 : LUAU_INSN_B(*insn) - 1;
                    unsigned int startReg = LUAU_INSN_A(*insn);
                    for (unsigned int i = startReg; i < startReg + argCount; i++) {
                        if (virtualStack[i])
                            argVec->push_back(virtualStack[i]);
                    }
                    AstArray<AstExpr *> args {argVec->data(), argVec->size()};
                    bodyHandler.addStat(new AstStatReturn {Location(), args});
                }
                break;
            }
            default:
                break;
        }
    }
}