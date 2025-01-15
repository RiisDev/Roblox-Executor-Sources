//
// Created by xgladius on 8/6/22.
//
#pragma once
#include "../../Common.h"

namespace Luau::Decompiler::AstGen {
    class VirtualAstStack {
        std::vector<AstExpr*> virtualStack;
    public:
        VirtualAstStack();

        AstExpr* get_or(int idx, AstExpr* ore);

        void remove(int idx);

        void set(int idx, AstExpr* ex);

        AstExpr* operator [](int idx);

        void clear();

        int getTop();
    };
}