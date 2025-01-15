//
// Created by xgladius on 8/6/22.
//
#include "../include/Common.h"

namespace Luau::Decompiler {
    AstExprGlobal* makeGlobal(const char* name) {
        return new AstExprGlobal {Location(), AstName(name)};
    }

    AstExprGlobal* genFunctionAst(StkId func) {
        auto cl = func->value.gc->cl;
        return cl.isC ? makeGlobal(cl.c.debugname) : makeGlobal(cl.l.p->debugname->data);
    }

    AstExpr* getConstantAst(TValue* val) {
        if (val == nullptr)
            return nullptr;
        switch (val->tt) {
            case LUA_TNIL:
                return new AstExprConstantNil{Location()};
            case LUA_TBOOLEAN:
                return new AstExprConstantBool { Location(), val->value.b != 0 };
            case LUA_TNUMBER:
                return new AstExprConstantNumber { Location(), val->value.n };
            case LUA_TSTRING: {
                AstArray<char> str{val->value.gc->ts.data, val->value.gc->ts.len};
                return new AstExprConstantString{ Location(), str };
            }
            case LUA_TFUNCTION:
                return genFunctionAst(val);
        }
        return nullptr;
    }
}