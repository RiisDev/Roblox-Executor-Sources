//
// Created by xgladius on 8/6/22.
//
#include "../include/VirtualAstStack.h"

#include <utility>

namespace Luau::Decompiler::AstGen {
    VirtualAstStack::VirtualAstStack() : virtualStack(256, nullptr) {}

    AstExpr* VirtualAstStack::get_or(int idx, AstExpr* ore) {
        auto get = virtualStack[idx];
        if (get != nullptr) {
            virtualStack[idx] = nullptr;
            return get;
        }
        return ore;
    }

    void VirtualAstStack::remove(int idx) {
        virtualStack[idx] = nullptr;
    }

    void VirtualAstStack::set(int idx, AstExpr* ex) {
        virtualStack[idx] = std::move(ex);
    }

    AstExpr* VirtualAstStack::operator[](int idx) {
        return virtualStack[idx];
    }

    void VirtualAstStack::clear() {
        for (auto & i: virtualStack) {
            i = nullptr;
        }
    }

    int VirtualAstStack::getTop() {
        for (auto i = 0; i < virtualStack.size(); i++) {
            if (virtualStack[i] == nullptr)
                return i - 1;
        }
        return 0;
    }
}