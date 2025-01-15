//
// Created by xgladius on 8/7/22.
//
#pragma once
#include "../../Common.h"

namespace Luau::Decompiler::AstGen {
    class Generator {
    public:
        std::vector<AstStat*>* body;

        Generator() : body(new std::vector<AstStat*>{}) {};

        [[nodiscard]] virtual AstStat* generate() = 0;
    };

    class BodyGenerator : public Generator {
    public:
        [[nodiscard]] AstStat* generate() override;
    };

    class AstStatForGenerator : public Generator {
    public:
        AstLocal* var;
        AstExpr* from;
        AstExpr* to;
        AstExpr* step;

        AstStatForGenerator(AstLocal* var, AstExpr* from, AstExpr* to, AstExpr* step) :
                                                            var(var), from(from), to(to), step(step) {};

        [[nodiscard]] AstStat* generate() override;
    };

    class AstStatIfGenerator : public Generator {
    public:
        AstExpr* condition;
        std::vector<AstStat*>* thenBody;
        std::vector<AstStat*>* elseBody;

        [[nodiscard]] AstStat* generate() override;

        explicit AstStatIfGenerator(AstExpr* condition) : condition(condition), elseBody(new std::vector<AstStat*>{}),
                                                          thenBody(new std::vector<AstStat*>{}) {};
    };

    class AstStatWhileGenerator : public Generator {
    public:
        AstExpr* condition;

        [[nodiscard]] AstStat* generate() override;

        explicit AstStatWhileGenerator() {};

        void updateCondition(AstExpr* cond);
    };
}