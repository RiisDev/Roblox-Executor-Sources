//
// Created by xgladius on 8/6/22.
//
#pragma once
#include "Luau/Ast.h"
#include "../../Common.h"

namespace Luau::Decompiler {
    AstExprGlobal* makeGlobal(const char* name);

    AstExprGlobal* genFunctionAst(StkId func);

    AstExpr* getConstantAst(TValue* val);
}