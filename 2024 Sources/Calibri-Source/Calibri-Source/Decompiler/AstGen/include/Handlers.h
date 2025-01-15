//
// Created by xgladius on 8/7/22.
//
#pragma once
#include <unordered_map>
#include "../include/Common.h"
#include "../../Common.h"
#include "VirtualAstStack.h"

namespace Luau::Decompiler::AstGen::Handlers {
    AstExprCall* getCallAst(VirtualAstStack& virtualStack, unsigned int insn);

    AstExprIndexName* genAstExprName(VirtualAstStack& virtualStack, Proto* proto, unsigned int insn, unsigned int aux, char symbol);

    AstStatAssign* getSetGlobalAssignment(VirtualAstStack& virtualStack, Proto* proto, unsigned int insn, unsigned int aux);

    struct locVar {
        bool existed;
        AstStatLocal* stat;
        AstExprLocal* expr;
    };

    locVar genLocalStat(VirtualAstStack& virtualStack, std::unordered_map<int, AstLocal*>& locVars, unsigned int insn);

    AstExprTable* genTableAst(VirtualAstStack& virtualStack, unsigned int insn, int index);

    AstStatAssign* genSetTable(VirtualAstStack& virtualStack, unsigned int insn);
}