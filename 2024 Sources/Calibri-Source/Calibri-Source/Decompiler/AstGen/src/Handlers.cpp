//
// Created by xgladius on 8/7/22.
//
#include "../include/Handlers.h"

namespace Luau::Decompiler::AstGen::Handlers {
    AstExprCall* getCallAst(VirtualAstStack& virtualStack, unsigned int insn)  {
        auto function = virtualStack[LUAU_INSN_A(insn)];
        bool isSelf = false;
        if (function->is<AstExprIndexName>()) {
            isSelf = function->as<AstExprIndexName>()->op == ':';
        }

        auto* argVec = new std::vector<AstExpr *>{};
        int argCount = LUAU_INSN_B(insn) == 0 ? virtualStack.getTop() : LUAU_INSN_B(insn) - 1;
        unsigned int startReg = isSelf ? LUAU_INSN_A(insn) + 2 : LUAU_INSN_A(insn) + 1;
        for (unsigned int i = startReg; i <= LUAU_INSN_A(insn) + argCount; i++) {
            auto reg = virtualStack[i];
            if (reg) {
                argVec->push_back(reg);
                if (LUAU_INSN_B(insn) != 0)
                    virtualStack.remove(i);
            }
        }

        AstArray<AstExpr *> args {argVec->data(), argVec->size()};
        virtualStack.remove(LUAU_INSN_A(insn));
        return new AstExprCall { Location(), function, args, isSelf, Location() };
    }

    AstExprIndexName* genAstExprName(VirtualAstStack& virtualStack, Proto* proto, unsigned int insn, unsigned int aux, char symbol) {
        auto name = AstName(proto->k[aux].value.gc->ts.data);
        return new AstExprIndexName{Location(), virtualStack[LUAU_INSN_B(insn)],
                                                            name, Location(), Position(0, 0), symbol};
    }

    AstStatAssign* getSetGlobalAssignment(VirtualAstStack& virtualStack, Proto* proto, unsigned int insn, unsigned int aux) {
        auto* vars_arr = new std::vector<AstExpr *>;
        auto* values_arr = new std::vector<AstExpr *>;

        vars_arr->push_back(Luau::Decompiler::makeGlobal(proto->k[aux].value.gc->ts.data));
        values_arr->push_back(virtualStack[LUAU_INSN_A(insn)]);

        AstArray<AstExpr*> vars {vars_arr->data(), vars_arr->size()};
        AstArray<AstExpr*> vals {values_arr->data(), values_arr->size()};

        return new AstStatAssign {Location(), vars, vals};
    }

    locVar genLocalStat(VirtualAstStack& virtualStack, std::unordered_map<int, AstLocal*>& locVars, unsigned int insn) {
        auto *vars_arr = new std::vector<AstLocal *>;
        auto name = new std::string(std::string("loc") + std::to_string(LUAU_INSN_B(insn)));
        AstLocal *loc;
        bool existed = false;
        if (locVars.find(LUAU_INSN_B(insn)) != locVars.end()) {
            loc = locVars.at(LUAU_INSN_B(insn));
            existed = true;
        } else {
            loc = new AstLocal{AstName(name->c_str()), Location(), nullptr, 1, 1, nullptr};
            locVars.insert(std::make_pair(LUAU_INSN_B(insn), loc));
        }
        vars_arr->push_back(loc);
        auto *values_arr = new std::vector<AstExpr *>;
        values_arr->push_back(virtualStack[LUAU_INSN_B(insn)]);
        auto stat = new AstStatLocal{Location(), {vars_arr->data(), vars_arr->size()},
                                     {values_arr->data(), values_arr->size()},
                                     std::make_optional<Location>(Location())};
        return {existed, stat, new AstExprLocal{Location(), loc, false}};
    }

    AstExprTable* genTableAst(VirtualAstStack& virtualStack, unsigned int insn, int index) {
        auto *items_arr = new std::vector<AstExprTable::Item>;
        int argCount = LUAU_INSN_C(insn) == 0 ? virtualStack.getTop() : LUAU_INSN_C(insn) - 1;
        for (unsigned int i = LUAU_INSN_B(insn); i <= LUAU_INSN_A(insn) + argCount; i++) {
            items_arr->push_back(AstExprTable::Item{AstExprTable::Item::Kind::List, nullptr, virtualStack[i]});
        }
        return new AstExprTable{Location(), {items_arr->data(), items_arr->size()}};
    }

    AstStatAssign* genSetTable(VirtualAstStack& virtualStack, unsigned int insn) {
        auto* vars_arr = new std::vector<AstExpr *>;
        auto* values_arr = new std::vector<AstExpr *>;

        vars_arr->push_back(new AstExprIndexExpr{ Location(),
                                                  virtualStack[LUAU_INSN_B(insn)], virtualStack[LUAU_INSN_C(insn)]});
        values_arr->push_back(virtualStack[LUAU_INSN_A(insn)]);

        AstArray<AstExpr*> vars {vars_arr->data(), vars_arr->size()};
        AstArray<AstExpr*> vals {values_arr->data(), values_arr->size()};

        return new AstStatAssign {Location(), vars, vals};
    }
}