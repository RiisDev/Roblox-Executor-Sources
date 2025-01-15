//
// Created by xgladius on 8/7/22.
//
#include "../include/Generator.h"

#include <memory>

namespace Luau::Decompiler::AstGen {

    AstStat* AstStatForGenerator::generate()  {
        AstArray<AstStat*> bodyArray {body->data(), body->size()};
        return new AstStatFor {Location(), var,
                                from, to, step,
                                new AstStatBlock {Location(), bodyArray},
                                true, Location(), true };
    }

    AstStat* AstStatIfGenerator::generate() {
        AstArray<AstStat*> bodyArray {body->data(), body->size()};
        AstArray<AstStat*> ebodyArray {elseBody->data(), elseBody->size()};
        return new AstStatIf{ Location(), condition,
                              new AstStatBlock {Location(), bodyArray},
                              nullptr,
                              Location(), Location(), true };
    }

    AstStat* BodyGenerator::generate() {
        AstArray<AstStat*> bodyArray {body->data(), body->size()};
        return new AstStatBlock{ Location(), bodyArray };
    }

    AstStat* AstStatWhileGenerator::generate() {
        AstArray<AstStat*> bodyArray {body->data(), body->size()};
        return new AstStatWhile { Location(), condition, new AstStatBlock {Location(), bodyArray},
                                  true, Location(), true };
    }

    void AstStatWhileGenerator::updateCondition(AstExpr *cond) {
        condition = cond;
    }
}